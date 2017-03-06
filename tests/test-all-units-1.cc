
# include <gsl/gsl_rng.h>
# include <ctime>
# include <cmath>
# include <memory>
# include <algorithm>


# include <tclap/CmdLine.h>

# include <ahFunctional.H>
# include <ah-string-utils.H>
# include <ah-stl-utils.H>
# include <htlist.H>

# include <units/pvt-units.H>

using namespace std;
using namespace TCLAP;
using namespace Aleph;

size_t precision = 6;

DynList<string>
generate_row(const Unit & unit, double val, double epsilon, bool verbose)
{
  DynList<string> conversions;
  
  VtlQuantity q(unit);
  try
    {
      if (verbose)
	cout << "    sample = " << val << " (" << unit.symbol << ")" << endl;
      q = VtlQuantity(unit, val);
      if (verbose)
	cout << "    Instantiated = " << q << endl;
    }
  catch (exception & e)
    {
      cout << "Error with generated sample" << val << endl
	   << e.what() << endl
	   << "Sample " << val << " is not inside [" << unit.min_val
	   << ", " << unit.max_val << "]" << endl;
      abort();
    }
  
  for (auto ut = unit.family_units().get_it(); ut.has_curr(); ut.next())
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
	  cout << "Error " << q << " to " << unit_ptr->name << endl
	       << e.what() << endl;
	  abort();
	}

      if (verbose)
	cout << "        Returning to " << unit.symbol << endl;
      VtlQuantity inv = { unit, conv };
      if (verbose)
	cout << "        done = " << inv << endl
	     << endl;
      if (abs(q.get_value() - inv.get_value()) > epsilon)
	{
	  ostringstream s;
	  s << "Conversion for value " << val << " from unit "
	    << unit.name << " to unit " << unit_ptr->name
	    << " does not satisfy epsilon threshold " << epsilon << endl
	    << "Original value     = " << q << endl
	    << "Intermediate value = " << conv << endl
	    << "Returned value = " << inv << endl;
	  throw range_error(s.str());
	}

      conversions.append(to_string(conv.get_value(), precision));
    }

  return conversions;
}

DynList<DynList<string>>
test_extremes_conversions(const PhysicalQuantity & pq, bool verbose,
			  double max_range, double epsilon)
{
      // only the units related to physical quantity pq
  auto units = pq.units();
  if (verbose)
    cout << endl
	 << "Testing with extremes" << endl
	 << endl;

  DynList<DynList<string>> rows;
      // generate the rows
  for (auto it = units.get_it(); it.has_curr(); it.next())
    {
      auto unit_ptr = it.get_curr();
      const double min = unit_ptr->min_val;
      const double urange = std::min(unit_ptr->max_val - min, max_range);
      const double max = min + urange;

      DynList<string> conversions;
      conversions.append(generate_row(*unit_ptr, min, epsilon, verbose));
      conversions.append(generate_row(*unit_ptr, max, epsilon, verbose));

      DynList<string> row;
      row.append(unit_ptr->name);
      row.append(to_string(min));
      row.append(to_string(max));
      row.append(conversions);

      rows.append(row);
    }

      // generate title row
  DynList<string> title = { "Unit name" };
  DynList<string> vals = { "min", "max" };
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

  DynList<DynList<string>> ret = { move(title) } ;
  ret.append(rows);

  return ret;
}


DynList<DynList<string>>
test_random_conversions(const PhysicalQuantity & pq, bool verbose,
			const size_t nsamples, double max, double epsilon,
			gsl_rng * r)
{
  using Puv = pair<const Unit * const, DynList<double>>;

      // only the units related to physical quantity pq
  auto units = pq.units();

  if (verbose)
    cout << "Generating random samples " << endl;

      // generate the random samples
  auto samples = units.maps<Puv>([nsamples, r, max, verbose] (auto unit_ptr)
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
  auto rows = samples.maps<DynList<string>>([&units, epsilon, verbose] (Puv p)
    {
      DynList<string> conversions;
      const DynList<double> & samples = p.second;
      for (auto it = samples.get_it(); it.has_curr(); it.next())
	conversions.append(generate_row(*p.first, it.get_curr(),
					epsilon, verbose));
      
      DynList<string> ret;
      ret.append(p.first->name);
      ret.append(samples.maps<string>([] (auto v) { return to_string(v); }));
      ret.append(conversions);

      return ret;
    });

      // generate title row
  DynList<string> title = { "Unit name" };
  DynList<string> vals = range(nsamples).maps<string>([] (auto i)
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

  DynList<DynList<string>> ret = { move(title) } ;
  ret.append(rows);

  return ret;
}

struct Epsilon
{
  string symbol;
  double epsilon;

  Epsilon & operator = (const string & str)
  {
    istringstream iss(str);
    if (not (iss >> symbol >> epsilon))
      throw TCLAP::ArgParseException(str + " is not a pair unit-symbol epsilon");

    return *this;
  }

  friend ostream& operator << (ostream &os, const Epsilon & a) 
  {
    return os << a.symbol << " " << a.epsilon;
  }
  
  ostream& print(ostream &os) const
  {
    return os << *this;
  }
};

namespace TCLAP
{
  template<> struct ArgTraits<Epsilon> { typedef StringLike ValueCategory; };
}

int main(int argc, char *argv[])
{
  CmdLine cmd(argv[0], ' ', "0");

  ValueArg<size_t> nsamples = { "n", "num-samples",
				"number of random samples", false,
				3, "number of samples", cmd };

  ValueArg<double> m = { "m", "max", "maximum range of a unit", false, 1000,
			 "maximum range of a unit", cmd };

  unsigned long dft_seed = time(nullptr);
  ValueArg<unsigned long> seed = { "s", "seed",
				   "seed for random number generator",
				   false, dft_seed, "random seed", cmd };

  SwitchArg csv("c", "csv", "output in csv format", cmd, false);

  ValueArg<double> epsilon("E", "Epsilon",
			   "precision threshold for numeric comparaisons",
			   false, 1e-4, "precision threshold", cmd);

  MultiArg<Epsilon> epsilons("e", "epsilon",
			     "epsilon of form \"unit-symbol epsilon\"",
			     false, "epsilon threshold", cmd);

  vector<string> pq = to_vector(PhysicalQuantity::names());
  PhysicalQuantity::quantities().for_each([&pq] (auto p)
					  { pq.push_back(p->name); });
				ValuesConstraint<string> allowed(pq);
  ValueArg<string> pm = { "Q", "physical-quantity", "name of physical quantity",
			  false, "", &allowed, cmd };

  SwitchArg extremes = { "x", "extremes", "test units extremes", cmd, false};

  SwitchArg print = { "p", "print", "print units", cmd, false };

  SwitchArg print_pq =
    { "P", "print-pq", "print physical quantities", cmd, false };

  SwitchArg ver = { "v", "verbose", "verbose mode", cmd, false };

  ValueArg<size_t> d = { "d", "digits", "number of digits", false, 10,
			 "number of digits", cmd };

  cmd.parse(argc, argv);

  if (print_pq.getValue())
    {
      PhysicalQuantity::names().for_each([] (const string & name)
        {
	  cout << name << endl;
	});
      exit(0);
    }
				
  if (not pm.isSet())
    {
      cout << "PARSE ERROR:" << endl
	   << "             Required argument missing: physical-quantity"
	   << endl;
      abort();
    }

  auto verbose = ver.getValue();

  unique_ptr<gsl_rng, decltype(gsl_rng_free)*>
    r(gsl_rng_alloc(gsl_rng_mt19937), gsl_rng_free);
  gsl_rng_set(r.get(), seed.getValue() % gsl_rng_max(r.get()));

  auto ptr = PhysicalQuantity::search(pm.getValue());
  if (ptr == nullptr)
    {
      cout << "Physical quantity " << pm.getValue() << " not found " << endl;
      abort();
    }

  auto p = check_conversions(*ptr);
  if (not p.first)
    {
      cout << "Missing conversions: " << endl;
      p.second.for_each([] (auto s) { cout << "    " << s << endl; });
      abort();
    }

  for (auto ep : epsilons.getValue())
    {
      auto unit_ptr = Unit::search_by_symbol(ep.symbol);
      if (unit_ptr == nullptr)
	{
	  cout << "In epsilon specification: unit symbol " << ep.symbol
	       << " not found" << endl;
	  abort();
	}

      if (ep.epsilon <= 0 or ep.epsilon >= 1)
	{
	  cout << "In epsilon specification of unit" << unit_ptr->name
	       << " (" << ep.symbol << "): value " << ep.epsilon
	       << " is not inside the range (0, 1)" << endl;
	  abort();
	}
      unit_ptr->set_epsilon(ep.epsilon);
    }

  if (print.getValue())
    {
      cout << ptr->to_string(60) << endl;
      ptr->units().for_each([] (auto u)
			    {
			      cout << *u << endl
				   << endl;
			    });
    }

  if (d.isSet())
    precision = d.getValue();

  DynList<DynList<string>> mat;
  if (extremes.getValue())
    mat = test_extremes_conversions(*ptr, verbose,
				    m.getValue(), epsilon.getValue());
  else
    mat = test_random_conversions(*ptr, verbose, nsamples.getValue(),
				  m.getValue(), epsilon.getValue(), r.get());
  
  cout << "Seed = " << seed.getValue() << endl;

  if (csv.getValue())
    cout << to_string(format_string_csv(mat)) << endl;
  else
    cout << to_string(format_string(mat)) << endl;

  return 0;
}
