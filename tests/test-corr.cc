
# include <tclap/CmdLine.h>

# include <ah-stl-utils.H>
# include <ahSort.H>
# include <ah-comb.H>
# include <ahFunctional.H>
# include <tpl_array.H>

# include <pvt-correlations.H>

using namespace TCLAP;

DynList<DynList<double>>
generate_pars_values(const Correlation * const corr_ptr, size_t n)
{
  return
    corr_ptr->get_preconditions().map<DynList<double>>([n] (const auto & par)
      {
	const double s = (par.max_val.get_value() - par.min_val.get_value())/n;
	DynList<double> ret;
	double v = par.min_val.get_value();
	for (size_t i = 0; i < n; v += s, ++i)
	  ret.append(v);
	return ret;
      });
}

struct Samples_Fct
{
  DynList<DynList<double>> ret;
  const Correlation * const corr_ptr;
  
  Samples_Fct(const Correlation * const corr_ptr) : corr_ptr(corr_ptr) {}

  bool operator () (const DynList<double> & pars)
  {
    ret.append(pars);
    return true;
  }
};

// Define a range of values for a correlation parameter
struct RangeDesc
{
  size_t i = 0; // par number
  double min = 0, max = 0;
  size_t n = 1; // num of steps

  RangeDesc & operator = (const string & str)
  {
    istringstream iss(str);
    if (not (iss >> i >> min >> max >> n))
      throw TCLAP::ArgParseException(str + " is not a trio of form i min max n");

    return *this;
  }

  friend ostream & operator << (ostream & os, const RangeDesc & d)
  {
    return os << d.i << " " << d.min<< " " << d.max << " " << d.n;
  }
  
  ostream& print(ostream &os) const
  {
    return os << *this;
  }
};

DynList<DynList<double>> generate_samples(const DynList<RangeDesc> & l)
{
  return l.map<DynList<double>>([] (const auto & r)
    {
      DynList<double> ret;
      const double step = (r.max - r.min)/r.n;
      double v = r.min;
      for (size_t i = 0; i < r.n; ++i, v += step)
	ret.append(v);
      return ret;
    });  
}

DynList<DynList<double>>
generate_samples(const Correlation * const corr_ptr, size_t n)
{
  Samples_Fct samples_fct = corr_ptr;
  traverse_perm(generate_pars_values(corr_ptr, n), samples_fct);

  return move(samples_fct.ret);
}

void full_mat(const Correlation * const corr_ptr, size_t n,
	      bool ignore_exception)
{
  auto samples = generate_samples(corr_ptr, n);
  auto results = samples.map<string>([corr_ptr, ignore_exception] (auto sample)
    {
      if (ignore_exception)
	{
	  string ret;
	  try
	    {
	      ret = to_string(corr_ptr->compute_and_check(sample));
	    }
	  catch (exception & e)
	    {
	      ret = "NA";
	    }
	  return ret;
	}
      else
	return to_string(corr_ptr->compute_and_check(sample));
    });

  auto mat = zip(samples, results).map<DynList<string>>([] (auto p)
    {
      DynList<string> ret;
      ret.append(p.first.template map<string>([] (auto v)
					      { return to_string(v); }));
      ret.append(p.second);
      return ret;
    });

  auto header = corr_ptr->get_preconditions().map<string>([] (const auto & pre)
    {
      ostringstream s;
      s << pre.name << " (" << pre.unit.symbol << ")";
      return s.str();
    });
  header.append(corr_ptr->name + " (" + corr_ptr->unit.symbol + ")");

  mat.insert(header);

  cout << to_string(format_string(mat)) << endl;
}

void mat_csv(const Correlation * const corr_ptr, size_t n)
{
      // csv header
  corr_ptr->get_preconditions().for_each([] (auto pre)
    {
      cout << pre.name << " (" << pre.unit.symbol << "), ";
    });
  cout << corr_ptr->name << " (" << corr_ptr->unit.name << ")" << endl;

  auto pars = generate_pars_values(corr_ptr, n);
  traverse_perm(pars, [corr_ptr] (const auto & s)
    {
      try
	{
	  s.for_each([] (auto v) { cout << v << ", "; });
	  auto r = corr_ptr->compute_and_check(s);
	  cout << r << endl;
	}
      catch (...)
	{
	  cout << endl;
	}
      return true;
    });
}

using P = pair<double, DynList<double>>;
using T = pair<P, P>;

struct Find_Extremes
{
  const Correlation * const corr_ptr = nullptr;
  double min_val = numeric_limits<double>::max();
  DynList<double> min_pars;
  double max_val = numeric_limits<double>::min();
  DynList<double> max_pars;
  bool verbose = false;
  
  Find_Extremes(const Correlation * const ptr, bool verbose)
    : corr_ptr(ptr), verbose(verbose) {}

  bool operator () (const DynList<double> & pars) 
  {
    auto r = corr_ptr->compute(pars);
    if (r < min_val)
      {
	if (verbose)
	  cout << "New min at " << corr_ptr->call_string(pars) << " = "
	       << r << " " << corr_ptr->unit.symbol << endl;
	min_val = r;
	min_pars = pars;
      }
    if (r > max_val)
      {
	if (verbose)
	  cout << "New max at " << corr_ptr->call_string(pars) << " = "
	       << r << " " << corr_ptr->unit.symbol<< endl;
	max_val = r;
	max_pars = pars;
      }
    return true;
  }
};

T find_extremes(const Correlation * const corr_ptr, size_t n, bool verbose)
{
  auto samples =
    corr_ptr->get_preconditions().map<DynList<double>>([n] (const auto & par)
      {
	const double min = par.min_val.get_value();
	const double max = par.max_val.get_value();
	const double s = (max - min)/n;
	DynList<double> ret;
	for (double v = min; v <= max; v += s) 
	  ret.append(v);
	return ret;
      });
  Find_Extremes finder = { corr_ptr, verbose };
  traverse_perm(samples, finder);

  return make_pair(make_pair(finder.min_val, finder.min_pars),
		   make_pair(finder.max_val, finder.max_pars));
} 

struct ArgUnit
{
  size_t i;
  string symbol;

  ArgUnit & operator = (const string & str)
  {
    istringstream iss(str);
    if (not (iss >> i >> symbol))
      throw TCLAP::ArgParseException(str + " is not a pair i unit");

    return *this;
  }
  
  ostream& print(ostream &os) const
  {
    return os << i << " " << symbol;
  }
};


namespace TCLAP
{
  template<> struct ArgTraits<ArgUnit> { typedef StringLike ValueCategory; };
  template<> struct ArgTraits<RangeDesc> { typedef StringLike ValueCategory; };
}

void test(int argc, char *argv[])
{
  CmdLine cmd(argv[0], ' ', "0");

  MultiArg<ArgUnit> arg_unit = { "u", "unit",
				 "unit of form \"par-num unit-symbol\"",
				 false, "unit for a parameter number", cmd };

  MultiArg<RangeDesc> range = { "r", "range",
				"range for parameter of form \"i min max n\"",
				false, "range for parameter number", cmd };

  vector<string> correlations;
  auto correlation_list = Correlation::list();
  correlation_list.for_each([&correlations] (auto p)
			    { correlations.push_back(p->name); });
  ValuesConstraint<string> allowed = correlations;
  ValueArg<string> correlation = { "C", "correlation", "correlation name",
				   false, "", &allowed, cmd};

  SwitchArg print = { "p", "print", "print correlation information", cmd };

  SwitchArg mat = { "f", "mattrix", "generate matrix", cmd };

  SwitchArg csv = { "c", "csv", "generate csv", cmd };

  ValueArg<size_t> n = { "n", "num-steps",
			 "number of steps by parameter", false, 10,
			 "number of samples", cmd };

  UnlabeledMultiArg<double> pars = { "pars", "correlation parameter values",
				     false, "values", cmd };

  SwitchArg extremes = { "x", "extremes", "compute extremes", cmd };

  SwitchArg verbose = { "v", "verbose", "verbose mode", cmd };

  SwitchArg ignore = { "i", "ignore-exception", "ignore exception", cmd };

  SwitchArg python = { "y", "print-python-call", "print python call", cmd };

  SwitchArg list_corr = { "l", "list", "list correlations", cmd };

  SwitchArg list_Corr = { "L", "List", "detailed list of correlations", cmd };

  ValueArg<string> describe = { "D", "describe-correlation",
				"describe correlation", false, "",
				&allowed, cmd};

  cmd.parse(argc, argv);

  if (list_corr.getValue())
    {
      auto l = correlation_list.map<DynList<string>>([] (auto p)
        {
	  return DynList<string>({p->name, p->subtype_name, p->type_name});
	});
      cout << to_string(format_string(l)) << endl;
      exit(0);
    }

  if (list_Corr.getValue())
    {
      cout << "Available correlations:" << endl;
      correlation_list.for_each([] (auto p) { cout << *p << endl; });
      exit(0);
    }

  if (describe.isSet())
    {
      auto ptr = Correlation::search_by_name(describe.getValue());
      cout << *ptr << endl
	   << endl;
      exit(0);
    }

  if (not correlation.isSet())
    {
      cout << "Error: -C option not set" << endl
	   << "if option -l is not set then option -C is mandatory" << endl;
      exit(0);
    }

  if (mat.getValue() and csv.getValue())
    {
      cout << "Error: options -c and -f cannot be together" << endl;
      abort();
    }

  auto correlation_ptr = Correlation::search_by_name(correlation.getValue());
  if (correlation_ptr == nullptr)
    {
      cout << "Error: correlation name " << correlation.getValue()
	   << " not found" << endl;
      abort();
    }

  if (print.getValue())
    cout << *correlation_ptr << endl
	 << endl;

  if (extremes.getValue())
    {
      auto vals =
	find_extremes(correlation_ptr, n.getValue(), verbose.getValue());
      cout << "min at " << correlation_ptr->call_string(vals.first.second)
	   << " = " << vals.first.first << " " << correlation_ptr->unit.symbol
	   <<endl
	   << "max at " << correlation_ptr->call_string(vals.second.second)
	   << " = " << vals.second.first << " " << correlation_ptr->unit.symbol
	   << endl
	   << endl;
      return;
    }

  if (csv.getValue() or mat.getValue())
    {
      size_t i = 0;
      Array<RangeDesc> ranges = correlation_ptr->get_preconditions().
	map<RangeDesc>([&i, &n] (const auto & p)
          {
	    RangeDesc r;
	    r.i = i++;
	    r.min = p.min_val.get_value();
	    r.max = p.max_val.get_value();
	    r.n = n.getValue();
	    return r;
	  });

      const size_t num_pars = correlation_ptr->get_num_pars();
      for (const auto & r : range.getValue())
	{
	  if (r.i > num_pars)
	    {
	      cout << "In range specification: parameter " << r.i
		   << " is greater than number of parameters " << num_pars
		   << endl;
	      abort();
	    }
	  if (r.min >= r.max)
	    {
	      cout << "In range specification of parameter " << r.i
		   << ": min value " << r.min
		   << " is greater or equal tnan max value " << r.max << endl;
	      abort();
	    }
	  auto & range = ranges(r.i - 1);
	  const auto & min = range.min;
	  if (r.min < min)
	    {
	      cout << "In range specification of parameter " << r.i
		   << ": min value " << r.min
		   << " is lesser that min value " << min
		   << " allowed for the parameter" << endl;
	      abort();
	    }
	  const auto & max = range.max;
	  if (r.max > max)
	    {
	      cout << "In range specification of parameter " << r.i
		   << ": max value " << r.max
		   << " is greater that max value " << max
		   << " allowed for the parameter" << endl;
	      abort();
	    }
	  range.i = r.i - 1;
	  range.min = r.min;
	  range.max = r.max;
	  range.n = r.n;	  
	}
      ranges.for_each([] (const auto & r) { cout << r << endl; });
      exit(0); // TMP
      // generar rangos

      if (csv.getValue())
	{
	  mat_csv(correlation_ptr, n.getValue());
	  return;
	}

      if (mat.getValue())
	{
	  full_mat(correlation_ptr, n.getValue(), ignore.getValue());
	  return;
	}
    }

  if (pars.getValue().size() != correlation_ptr->get_num_pars())
    {
      cout << "Correlation " << correlation_ptr->name << " expects "
	   << correlation_ptr->get_num_pars() << " parameters but "
	   << pars.getValue().size() << " were passed" << endl;
      abort();
    }
  
  Array<const Unit*> params = correlation_ptr->get_par_types(); 
  for (const auto & p : arg_unit.getValue())
    {
      if (p.i > params.size())
	{
	  cout << "In specification of unit " << p.symbol 
	       << " : parameter number " << p.i
	       << " is greater than number of passed parameter" << endl;
	  abort();
	}

      auto unit_ptr = Unit::search_by_symbol(p.symbol);
      if (unit_ptr == nullptr)
	{
	  cout << "In specification of unit for parameter number " << p.i
	       << " : symbol " << p.symbol << " does not match with any unit"
	       << endl;
	  abort();
	}
      params(p.i - 1) = unit_ptr;
    }

  auto pars_list = zip(params, to_DynList(pars.getValue())).
   map<VtlQuantity>([] (auto p) { return VtlQuantity(*p.first, p.second); });

  if (python.getValue())
    cout << endl
	 << correlation_ptr->python_call(pars_list) << endl
	 << endl;

    if (ignore.getValue())
      {
	auto ret = correlation_ptr->compute(pars_list);
	cout << correlation_ptr->call_string(pars_list) << ") = " << ret << endl;
      }
    else
      {
	auto ret = correlation_ptr->compute_and_check(pars_list);
	cout << correlation_ptr->call_string(pars_list) << ") = " << ret << endl;
      }

  cout << endl;
}

int main(int argc, char *argv[])
{
  test(argc, argv);
  
  return 0;
}
