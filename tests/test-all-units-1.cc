
# include <gsl/gsl_rng.h>
# include <ctime>
# include <cmath>
# include <memory>
# include <algorithm>

# include <tclap/CmdLine.h>

# include <ahFunctional.H>
# include <ah-string-utils.H>

# include <pvt-units.H>

using namespace std;
using namespace TCLAP;
using namespace Aleph;

DynList<DynList<string>>
test_conversions(const PhysicalQuantity & pq, bool verbose,
		 const size_t nsamples, double max, double epsilon,
		 gsl_rng * r)
{
  using Puv = pair<const Unit * const, DynList<double>>;

      // only the units related to physical quantity pq
  auto units = Unit::units().filter([p = &pq] (auto u)
				    { return &u->physical_quantity == p; });

  if (verbose)
    cout << "Generating random samples " << endl;

      // generate the random samples
  auto samples = units.map<Puv>([nsamples, r, max, verbose] (auto unit_ptr)
    {
      if (verbose)
	cout << "    For " << unit_ptr->name << ":";
      auto min = unit_ptr->min_val;
      auto urange = unit_ptr->max_val - min;
      urange = std::min(urange, max);
      DynList<double> values;
      for (size_t i = 0; i < nsamples; ++i)
	{
	  auto val = min + urange*gsl_rng_uniform(r);
	  values.append(val);
	  if (verbose)
	    cout << " " << val;
	}
      if (verbose)
	cout << endl;
      
      return make_pair(unit_ptr, move(values));
    });

  if (verbose)
    cout << endl
	 << "Testing " << endl
	 << endl;

      // generate the rows
  auto rows = samples.map<DynList<string>>([&units, epsilon, verbose] (Puv p)
    {
      DynList<string> conversions;
      const DynList<double> & samples = p.second;
      for (auto it = samples.get_it(); it.has_curr(); it.next())
	{
	  double val = it.get_curr();
	  VtlQuantity q(*p.first);
	  try
	    {
	      if (verbose)
		cout << "    sample = " << val << endl;
	      q = VtlQuantity(*p.first, val);
	      if (verbose)
		cout << "    Instantiated = " << q << endl;
	    }
	  catch (exception & e)
	    {
	      cout << "Error with generated sample" << endl
		   << e.what() << endl
		   << "Sample " << val << " is not inside [" << p.first->min_val
		   << ", " << p.first->max_val << "]" << endl;
	      abort();
	    }
	  for (auto ut = units.get_it(); ut.has_curr(); ut.next())
	    {
	      const Unit * const unit_ptr = ut.get_curr();
	      VtlQuantity conv(*unit_ptr);
	      try
		{
		  if (verbose)
		    cout << "        Converting " << q << " to "
			 << unit_ptr->symbol << endl;
		  conv = VtlQuantity(*unit_ptr, q); // conversion
		  if (verbose)
		    cout << "        done = " << conv << endl;
		}
	      catch (exception & e)
		{
		  cout << "Error converting " << q << " to " << unit_ptr->name
		       << endl
		       << e.what() << endl;
		  abort();
		}

	      if (verbose)
		cout << "        Returning to " << p.first->symbol << endl;
	      VtlQuantity inv = { *p.first, conv };
	      if (verbose)
		cout << "        done = " << inv << endl
		     << endl;
	      if (abs(q.get_value() - inv.get_value()) > epsilon)
		{
		  ostringstream s;
		  s << "Conversion for value " << val << " from unit "
		    << p.first->name << " to unit " << unit_ptr->name
		    << " does not satisfy epsilon threshold " << epsilon << endl
		    << "Original value     = " << q << endl
		    << "Intermediate value = " << conv << endl
		    << "Returned value = " << inv << endl;
		  throw range_error(s.str());
		}

	      conversions.append(to_string(conv.get_value()));
	    }
	}
      
      DynList<string> ret;
      ret.append(p.first->name);
      ret.append(samples.map<string>([] (auto v) { return to_string(v); }));
      ret.append(conversions);

      return ret;
    });

      // generate title row
  DynList<string> title = { "Unit name" };
  DynList<string> vals = range(nsamples).map<string>([] (auto i)
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

int main(int argc, char *argv[])
{
  CmdLine cmd(argv[0], ' ', "0");

  ValueArg<size_t> nsamples = { "n", "num-samples",
				"number of random samples", false,
				3, "number of samples" };
  cmd.add(nsamples);

  ValueArg<double> max = { "m", "max", "maximum range of a unit", false, 1000,
			   "maximum range of a unit" };
  cmd.add(max);

  unsigned long dft_seed = time(nullptr);
  ValueArg<unsigned long> seed = { "s", "seed",
				   "seed for random number generator",
				   false, dft_seed, "random seed" };
  cmd.add(seed);

  SwitchArg csv("c", "csv", "output in csv format", false);
  cmd.add(csv);

  ValueArg<double> epsilon("E", "epsilon",
			   "precision threshold for numeric comparaisons",
			   false, 1e-20, "precision threshold");
  cmd.add(epsilon);

  vector<string> pq;
  PhysicalQuantity::quantities().for_each([&pq] (auto p)
					  { pq.push_back(p->name); });
  ValuesConstraint<string> allowed(pq);
  ValueArg<string> pm = { "P", "physical-quantity", "name of physical quantity",
			  true, "", &allowed };
  cmd.add(pm);

  SwitchArg ver = { "v", "verbose", "verbose mode", false };
  cmd.add(ver);

  cmd.parse(argc, argv);

  unique_ptr<gsl_rng, decltype(gsl_rng_free)*>
    r(gsl_rng_alloc(gsl_rng_mt19937), gsl_rng_free);
  gsl_rng_set(r.get(), seed.getValue() % gsl_rng_max(r.get()));

  auto ptr = PhysicalQuantity::search(pm.getValue());
  if (ptr == nullptr)
    {
      cout << "Physical quantity " << pm.getValue() << " not found " << endl;
      abort();
    }

  cout << "Seed = " << seed.getValue() << endl;

  auto mat = test_conversions(*ptr, ver.getValue(), nsamples.getValue(),
			      max.getValue(), epsilon.getValue(), r.get());

  if (csv.getValue())
    cout << to_string(format_string_csv(mat)) << endl;
  else
    cout << to_string(format_string(mat)) << endl;

  return 0;
}
