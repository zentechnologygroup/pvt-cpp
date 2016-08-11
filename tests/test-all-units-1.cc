
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
		 const size_t nsamples, double max, double epsilon,
		 gsl_rng * r)
{
  using Puv = pair<const Unit * const, DynList<double>>;

      // only the units related to physical quantity pq
  auto units = Unit::units().filter([p = &pq] (auto u)
				    { return &u->physical_quantity == p; });

      // generate the random samples
  auto samples = units.map<Puv>([nsamples, r, max] (auto unit_ptr)
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
      for (size_t i = 0; i < nsamples; ++i)
	{
	  auto val = min + urange*gsl_rng_uniform(r);
	  values.append(val);
	}
      return make_pair(unit_ptr, move(values));
    });

      // generate the rows
  auto rows = samples.map<DynList<string>>([&units, epsilon] (Puv p)
    {
      DynList<string> conversions;
      const DynList<double> & samples = p.second;
      for (auto it = samples.get_it(); it.has_curr(); it.next())
	{
	  double val = it.get_curr();
	  VtlQuantity q(*p.first);
	  try
	    {
	      q = VtlQuantity(*p.first, val);
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
		  conv = VtlQuantity(*unit_ptr, q); // conversion
		}
	      catch (exception & e)
		{
		  cout << "Error converting " << q << " to " << unit_ptr->name
		       << endl
		       << e.what() << endl;
		  abort();
		}

	      VtlQuantity inv = { *p.first, conv };
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

  ValueArg<double> max = { "m", "max", "maximum value", false, 1000,
			   "maximum value of physical magnitude" };
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

  auto mat = test_conversions(*ptr, nsamples.getValue(),
			      max.getValue(), epsilon.getValue(), r.get());

  if (csv.getValue())
    cout << to_string(format_csv(mat)) << endl;
  else
    cout << to_string(format(mat)) << endl;

  return 0;
}
