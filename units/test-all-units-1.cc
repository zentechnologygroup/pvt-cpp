
# include <gsl/gsl_rng.h>
# include <ctime>
# include <memory>
# include <algorithm>
# include <tclap/CmdLine.h>
# include <ahFunctional.H>
# include <pvt-units.H>

using namespace std;
using namespace TCLAP;

DynList<DynList<string>> format(const DynList<DynList<string>> & mat)
{
  DynList<size_t> ilens = rep<size_t>(mat.nth(0).size(), 0);
  DynList<size_t> maxs = mat.foldl(ilens, [] (const DynList<size_t> & acu,
					      const DynList<string> & l)
    {
      return zip(acu, l).map<size_t>([] (auto p)
        { return max(p.first, p.second.size()); });
    });

  // here maxs contains the maximum size of each column

  return mat.map<DynList<string>>([&maxs] (const auto & l)
    {
      return zip(maxs, l).template map<DynList<string>>([] (auto p)
	{
	  const string blanks(p.first - p.second.size(), ' ');
	  return blanks + p.second;
	});
    });
}

DynList<DynList<string>>
test_conversions(const PhysicalQuantity & pq,
		 const size_t ntries, double max, gsl_rng * r)
{
  using Puv = pair<const Unit * const, DynList<double>>;
  auto units = Unit::units().filter([p = &pq] (auto u)
				    { return &u->physical_quantity == p; });
  auto samples = units.map<Puv>([ntries, r, max] (auto unit_ptr)
    {
      auto min = unit_ptr->min_val;
      if (max > unit_ptr->max_val or max < min)
	{
	  ostringstream s;
	  s << "max value " << max << " is invalid";
	  throw domain_error(s.str());
	}
      auto urange = max - min;
      DynList<double> values;
      for (size_t i = 0; i < ntries; ++i)
	{
	  auto val = min + urange*gsl_rng_uniform(r);
	  values.append(val);
	}
      return make_pair(unit_ptr, move(values));
    });

  samples.for_each([] (auto p)
		   {
		     cout << *p.first << endl;
		     p.second.for_each([] (auto v) { cout << " " << v; });
		     cout << endl
			  << endl;
		   });

  DynList<DynList<string>> ret;

  return ret;
}

void test()
{
  auto all_units = Unit::units();
  auto physical_quantities = PhysicalQuantity::quantities();
  for (auto pq_it = physical_quantities.get_it(); pq_it.has_curr(); pq_it.next())
    {
      const PhysicalQuantity * const pq_ptr = pq_it.get_curr();
      cout << "Physical quantity: " << pq_ptr->name << endl;

      auto units = all_units.filter([pq_ptr] (const auto & u)
        { return &u->physical_quantity == pq_ptr; });
      for (auto it = units.get_it(); it.has_curr(); it.next())
	{
	  auto u = it.get_curr();
	  cout << "Unit: " << endl
	       << *u << endl
	       << endl;
	}
    }
}

int main(int argc, char *argv[])
{
  CmdLine cmd(argv[0], ' ', "0");

  ValueArg<size_t> ntries = { "n", "num-tries", "number of tries", false, 3,
			      "number of tries" };
  cmd.add(ntries);

  ValueArg<double> max = { "m", "max", "maximum value", false, 1000,
			   "maximum value" };
  cmd.add(max);

  unsigned long dft_seed = time(nullptr);
  ValueArg<unsigned long> seed = { "s", "seed", "seed for random number generator",
				  false, dft_seed, "random seed" };
  cmd.add(seed);

  cmd.parse(argc, argv);

  unique_ptr<gsl_rng, decltype(gsl_rng_free)*>
    r(gsl_rng_alloc(gsl_rng_mt19937), gsl_rng_free);

  //test();

  test_conversions(Temperature::get_instance(), ntries.getValue(),
		   max.getValue(), r.get());

  return 0;
}
