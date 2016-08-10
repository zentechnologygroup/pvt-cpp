
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

  return mat.map<DynList<string>>([&maxs] (const DynList<string> & l)
    {
      return zip(maxs, l).template map<string>([] (auto p)
	{
	  const string blanks(p.first - p.second.size(), ' ');
	  return blanks + p.second + " ";
	});
    });
}

DynList<DynList<string>> format_csv(const DynList<DynList<string>> & mat)
{
  DynList<DynList<string>> ret;
  for (auto row_it = mat.get_it(); row_it.has_curr(); row_it.next())
    {
      const DynList<string> & curr_row = row_it.get_curr();
      const string & last = curr_row.get_last();
      DynList<string> row;
      cout << "size = " << curr_row.size() << endl;
      for (auto it = curr_row.get_it(); it.has_curr(); it.next())
	{
	  const string & s = it.get_curr();
	  if (&s == &last)
	    row.append(s);
	  else
	    row.append(s + ",");
	}
      ret.append(row);
    }

  return ret;
}

string to_string(const DynList<DynList<string>> & mat)
{
  ostringstream s;
  mat.for_each([&s] (const auto & row)
	       {
		 row.for_each([&s] (const string & str) { s << str; });
		 s << endl;
	       });
  return s.str();
}

DynList<DynList<string>>
test_conversions(const PhysicalQuantity & pq,
		 const size_t ntries, double max, gsl_rng * r)
{
  using Puv = pair<const Unit * const, DynList<double>>;

      // only the units related to physical quantity pq
  auto units = Unit::units().filter([p = &pq] (auto u)
				    { return &u->physical_quantity == p; });

      // generate the random samples
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

      // generate the rows
  auto rows = samples.map<DynList<string>>([&units] (Puv p)
    {
      DynList<string> conversions;
      const DynList<double> & samples = p.second;
      for (auto it = samples.get_it(); it.has_curr(); it.next())
	{
	  double val = it.get_curr();
	  VtlQuantity q = { *p.first, val };
	  for (auto ut = units.get_it(); ut.has_curr(); ut.next())
	    {
	      const Unit * const unit_ptr = ut.get_curr();
	      VtlQuantity conv = { *unit_ptr, q }; // conversion
	      conversions.append(to_string(conv.get_value()));
	    }
	}
      
      DynList<string> ret;
      cout << "Samples size = " << samples.size() << endl
      << "conversions size = " << conversions.size() << endl;
      ret.append(p.first->name);
      ret.append(samples.map<string>([] (auto v) { return to_string(v); }));
      ret.append(conversions);

      return ret;
    });

      // generate title row
  DynList<string> title = { "Unit name" };
  DynList<string> vals = range(ntries).map<string>([] (auto i)
                         { return "sample-" + to_string(i); });
  title.append(vals);

  for (auto it = vals.get_it(); it.has_curr(); it.next())
    {
      const string & val = it.get_curr();
      for (auto uit = units.get_it(); uit.has_curr(); uit.next())
	{
	  const string name = val + "-" + uit.get_curr()->symbol;
	  title.append(name);
	}
    }

  DynList<DynList<string>> ret = move(title);
  ret.append(rows);

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

  SwitchArg csv("c", "csv", "output in csv format", false);
  cmd.add(csv);

  cmd.parse(argc, argv);

  unique_ptr<gsl_rng, decltype(gsl_rng_free)*>
    r(gsl_rng_alloc(gsl_rng_mt19937), gsl_rng_free);

  auto mat = test_conversions(Temperature::get_instance(), ntries.getValue(),
			      max.getValue(), r.get());

  if (csv.getValue())
    cout << to_string(format_csv(mat)) << endl;
  else
    cout << to_string(format(mat)) << endl;

  return 0;
}
