#ifndef CALIBRATE_H
#define CALIBRATE_H

# include <gsl/gsl_rng.h>
# include <metadata/pvt-calibrate.H>

using ArrayP = pair<Array<double>, Array<double>>;

static unique_ptr<gsl_rng, void(*)(gsl_rng*)> r(gsl_rng_alloc(gsl_rng_mt19937),
						gsl_rng_free);
constexpr unsigned long Max_Num_Points = 10;
constexpr double Max_Pressure = 10000;
constexpr double Max_uob = 30000;
constexpr double Max_uoa = 30000;

ArrayP generate_random_uo()
{
  // select random nuob and nuoa. Both must be greater than or equal to 2
  size_t nuob = 2 + gsl_rng_uniform_int(r.get(), Max_Num_Points);
  size_t nuoa = 2 + gsl_rng_uniform_int(r.get(), Max_Num_Points);

  // Generate nuob + nuoa pressure values
  Array<double> p;
  for (size_t i = 0; i < nuob + nuoa; ++i)
    p.append(gsl_rng_uniform_pos(r.get())*Max_Pressure);
  in_place_sort(p); // pressure must be sorted

  // select a random uobp between uo_percentile and 2*uo_percentile
  const double uo_percentile = 0.1*Max_Pressure;
  const double uo_pivot =
    uo_percentile + uo_percentile*gsl_rng_uniform_pos(r.get());
  cout << uo_percentile << " " << uo_pivot << endl;
  assert(uo_pivot > uo_percentile and uo_pivot < 2*uo_percentile);

  // generate nuob random values inversely sorted
  DynList<double> uob;
  for (size_t i = 0; i < nuob - 1; ++i)
    uob.append(uo_pivot + (Max_uob - uo_pivot)*gsl_rng_uniform_pos(r.get()));
  uob.append(uo_pivot);
  uob = sort(uob).rev();

  // generate nuoa random values sorted
  DynList<double> uoa;
  for (size_t i = 0; i < nuoa; ++i)
    uoa.append(uo_pivot + (Max_uoa - uo_pivot)*gsl_rng_uniform_pos(r.get()));
  in_place_sort(uoa);

  // cout << "uob ="; uob.for_each([] (auto v) { cout << " " << v; }); cout << endl;
  // cout << "uoa ="; uoa.for_each([] (auto v) { cout << " " << v; }); cout << endl;

  uob.append(uoa);
  Array<double> uo = uob;

  assert(p.size() == uo.size());

  // cout << "p ="; p.for_each([] (auto v) { cout << " " << v; }); cout << endl;
  // cout << "uo ="; uo.for_each([] (auto v) { cout << " " << v; }); cout << endl;

  return ArrayP(p, Array<double>(uo));
}


#endif
