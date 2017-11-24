/** PVT correlation calculator

    This is the backend program for generating computing PVT correlations

    Compile and then type
    
        ./test-corr --help

    In order to see all the options

    Aleph-w Leandro Rabindranath Leon
 */
# include <tclap/CmdLine.h>

# include <ah-stl-utils.H>
# include <ahSort.H>
# include <ah-comb.H>
# include <ahFunctional.H>
# include <tpl_array.H>
# include <correlations/correlation.H>

# include <correlations/pvt-correlations.H>

using namespace TCLAP;

bool check = false;

DynList<DynList<double>>
generate_pars_values(const Correlation * const corr_ptr, size_t n)
{
  return
    corr_ptr->get_preconditions().maps<DynList<double>>([n] (const auto & par)
      {
	const double s = (par.max_val.get_value()-par.min_val.get_value())/(n-1);
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
  const Unit * unit_ptr = nullptr;
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

DynList<DynList<double>> generate_pars_values(const DynList<RangeDesc> & l)
{
  return l.maps<DynList<double>>([] (const auto & r)
    {
      DynList<double> ret;
      const double step = (r.max - r.min) / (r.n - 1);
      if (step == 0)
	{
	  ret.append(r.min);
	  return ret;
	}
      
      double v = r.min;
      for (size_t i = 0; i < r.n and v <= r.max; ++i, v += step)
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
  auto results = samples.maps<string>([corr_ptr, ignore_exception] (auto sample)
    {
      if (ignore_exception)
	{
	  string ret;
	  try
	    {
	      ret = to_string(corr_ptr->compute_and_check(sample, check));
	    }
	  catch (exception & e)
	    {
	      ret = to_string(corr_ptr->compute(sample, check));
	    }
	  return ret;
	}
      else
	return to_string(corr_ptr->compute_and_check(sample, check));
    });

  auto mat = zip(samples, results).maps<DynList<string>>([] (auto p)
    {
      DynList<string> ret;
      ret.append(p.first.template maps<string>([] (auto v)
					      { return to_string(v); }));
      ret.append(p.second);
      return ret;
    });

  auto header = corr_ptr->get_preconditions().maps<string>([] (const auto & pre)
    {
      ostringstream s;
      s << pre.name << " (" << pre.unit.symbol << ")";
      return s.str();
    });
  header.append(corr_ptr->name + " (" + corr_ptr->unit.symbol + ")");

  mat.insert(header);

  cout << to_string(format_string(mat)) << endl;
}

void full_mat(const Correlation * const corr_ptr, const DynList<RangeDesc> & l,
	      bool ignore_exception)
{  
  auto header = corr_ptr->get_preconditions().maps<string>([] (const auto & pre)
    {
      ostringstream s;
      s << pre.name << " (" << pre.unit.symbol << ")";
      return s.str();
    });
  header.append(corr_ptr->name + " (" + corr_ptr->unit.symbol + ")");

  auto samples = generate_pars_values(l);
  auto lens = fold_perm<DynList<size_t>>
    (header.maps<size_t>([] (const auto & t) { return t.size(); }), samples,
     [corr_ptr, ignore_exception]
     (const DynList<size_t> & acu, const DynList<double> & sample)
    {
      string ret;
      try
	{
	  ret = to_string(corr_ptr->compute_and_check(sample, check));
	}
      catch (exception & e)
	{
	  if (ignore_exception)
	    ret = "NA";
	  else
	    throw;
	}
      auto row = sample.maps<string>([] (auto v) { return to_string(v); });
      row.append(ret);

      return zip(acu, row).maps<size_t>([] (auto p)
        { return max(p.first, p.second.size()); });
    });

  // here lens contains the maximums size of each column

  zip(lens, header).for_each([] (auto p)
    {
      cout << string(p.first - p.second.size(), ' ') << p.second << " ";
    });
  cout << endl;
  for_each_perm(samples, [corr_ptr, ignore_exception, &lens]
		(const DynList<double> & sample)
    {
      string result;
      try
	{
	  result = to_string(corr_ptr->compute_and_check(sample, check));
	}
      catch (exception & e)
	{
	  result = "NA";
	}
      auto row = sample.maps<string>([] (auto v) { return to_string(v); });
      row.append(result);
      auto line = zip(lens, row).template maps<string>([] (auto p)
	{
	  const string blanks(p.first - p.second.size(), ' ');
	  return blanks + p.second + " ";
	});
      line.for_each([] (const auto & s) { cout << s; });
      cout << endl;
    });
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
	  auto r = corr_ptr->compute_and_check(s, check);
	  cout << r << endl;
	}
      catch (...)
	{
	  cout << endl;
	}
      return true;
    });
}

void mat_csv(const Correlation * const corr_ptr, const DynList<RangeDesc> & l,
	     bool ignore_exception)
{      // csv header
  corr_ptr->get_preconditions().for_each([] (auto pre)
    {
      cout << pre.name << " (" << pre.unit.symbol << "), ";
    });
  cout << corr_ptr->name << " (" << corr_ptr->unit.name << ")" << endl;

  auto pars = generate_pars_values(l);
  traverse_perm(pars, [corr_ptr, ignore_exception] (const auto & s)
    {
      try
	{
	  s.for_each([] (auto v) { cout << v << ", "; });
	  auto r = corr_ptr->compute_and_check(s, check);
	  cout << r << endl;
	}
      catch (...)
	{
	  if (ignore_exception)
	    cout << endl;
	  else
	    throw;
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
    auto r = corr_ptr->compute(pars, check);
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
    corr_ptr->get_preconditions().maps<DynList<double>>([n] (const auto & par)
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

T find_extremes(const Correlation * const corr_ptr,
		const DynList<RangeDesc> & ranges, bool verbose)
{
  Find_Extremes finder = { corr_ptr, verbose };
  traverse_perm(generate_pars_values(ranges), finder);

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

  friend ostream& operator << (ostream &os, const ArgUnit & a) 
  {
    return os << a.i << " " << a.symbol;
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

  SwitchArg print_full = { "P", "Print", "print correlation information", cmd };

  SwitchArg mat = { "f", "mattrix", "generate matrix", cmd };

  SwitchArg csv = { "c", "csv", "generate csv", cmd };

  SwitchArg json = { "j", "json", "output in json", cmd };

  ValueArg<string> json_corr = { "J", "json-corr", "json for correlation",
				 false, "", "json for correlation", cmd };

  ValueArg<string> json_subtype = { "S", "subtype", "json for subtype", false,
				    "", "json for subtype", cmd };

  SwitchArg server = { "X", "server-execute", "output adapted to server", cmd };

  ValueArg<size_t> n = { "n", "num-steps",
			 "number of steps by parameter", false, 10,
			 "number of samples", cmd };

  UnlabeledMultiArg<double> pars = { "pars", "correlation parameter values",
				     false, "values", cmd };

  SwitchArg extremes = { "x", "extremes", "compute extremes", cmd };

  SwitchArg verbose = { "v", "verbose", "verbose mode", cmd };

  SwitchArg ignore = { "i", "ignore-exception", "ignore exception", cmd, false };

  SwitchArg python = { "y", "print-python-call", "print python call", cmd };

  SwitchArg list_corr = { "l", "list", "list correlations", cmd };

  SwitchArg list_Corr = { "L", "List", "detailed list of correlations", cmd };

  SwitchArg symbols = { "s", "latex-symbols", "latex symbols", cmd };

  SwitchArg check_arg = { "", "enable-ranges-check",
			  "enable checking of correlation input ranges", cmd,
			  false };

  SwitchArg ruby = { "R", "ruby-hash", "generate ruby hash", cmd };

  cmd.parse(argc, argv);

  if (ruby.getValue())
    {
      cout << Correlation::to_ruby_def() << endl;
      exit(0);
    }

  if (symbols.getValue())
    {
      auto l =
	Correlation::latex_symbols().maps<DynList<string>>([] (auto p)
          {
	    return DynList<string>({p.first, p.second});
	  });
      l.insert(DynList<string>({"parameter name", "latex symbol"}));
      cout << to_string(format_string(l)) << endl;
      exit(0);
    }

  if (json_corr.isSet())
    {
      auto corr_ptr = Correlation::search_by_name(json_corr.getValue());
      if (corr_ptr == nullptr)
	{
	  cout << "Correlation name " << json_corr.getValue() << " not found"
	       << endl;
	  abort();
	}
      cout << corr_ptr->to_json() << endl;
      exit(0);
    }

  if (json_subtype.isSet())
    {
      cout << Correlation::to_json(json_subtype.getValue()) << endl;
      exit(0);
    }

  check = check_arg.getValue();

  if (list_corr.getValue())
    {
      if (json.getValue())
	{
	  cout << Correlation::json_of_all_correlations() << endl;
	  exit(0);
	}
      
      auto l = correlation_list.maps<DynList<string>>([] (auto p)
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

  if (print_full.getValue())
    {
      cout << correlation_ptr->full_desc() << endl
	   << endl;
	exit(0);
    }

  if (print.getValue())
    {
      cout << *correlation_ptr << endl
	   << endl;
      if (pars.getValue().size() == 0 and (not csv.getValue() and
					   not mat.getValue() and
					   not extremes.getValue()))
	exit(0);
    }

  if (csv.getValue() or mat.getValue() or extremes.getValue())
    {
      size_t i = 0;
      Array<RangeDesc> ranges = correlation_ptr->get_preconditions().
	maps<RangeDesc>([&i, &n] (const auto & p)
          {
	    RangeDesc r;
	    r.i = i++;
	    r.min = p.min_val.get_value();
	    r.max = p.max_val.get_value();
	    r.n = n.getValue();
	    return r;
	  });

      const size_t num_pars = correlation_ptr->get_num_pars();

          // Review if there is change of units
      for (const auto & a : arg_unit.getValue())
	{
	  if (a.i > num_pars)
	    {
	      cout << "In unit specification of parameter " << a.i
		   << " : the correlation has " << num_pars << endl;
	      abort();								
	    }
	  auto ptr = Unit::search_by_symbol(a.symbol);
	  if (not ptr)
	    {
	      cout << "Unit symbol " << a.symbol << " not found" << endl;
	      abort();
	    }
	  ranges(a.i - 1).unit_ptr = ptr;
	}

      for (auto r : range.getValue())
	{
	  if (r.i == 0 or r.i > num_pars)
	    {
	      cout << "In range specification: parameter " << r.i
		   << " is not inside of [1, "<< num_pars << "]" << endl;
	      abort();
	    }
	  if (r.n == 0)
	    {
	      cout << "In range specification: parameter " << r.i
		   << ": number of steps must be greter than zero" << endl;
	      abort();
	    }

	  auto & range = ranges(r.i - 1);
	  if (range.unit_ptr)
	    {
	      auto par_unit =
		correlation_ptr->get_preconditions().nth(r.i - 1).unit;
	      auto convert_fct = search_conversion(*range.unit_ptr, par_unit);
	      if (not convert_fct)
		{
		  cout << "Conversion from " << range.unit_ptr->symbol
		       << " to " << par_unit.symbol << " not found" << endl;
		  abort();
		}
	      r.min = (*convert_fct)(r.min);
	      r.max = (*convert_fct)(r.max);
	    }

	  if (r.min > r.max)
	    {
	      cout << "In range specification of parameter " << r.i
		   << ": min value " << r.min
		   << " is greater or equal than max value " << r.max << endl;
	      abort();
	    }
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

      if (csv.getValue())
	{
	  //mat_csv(correlation_ptr, n.getValue());
	  mat_csv(correlation_ptr, ranges.keys(), ignore.getValue());
	  return;
	}

      if (mat.getValue())
	{
	  //full_mat(correlation_ptr, n.getValue(), ignore.getValue());
	  full_mat(correlation_ptr, ranges.keys(), ignore.getValue());
	  return;
	}
      
      if (extremes.getValue())
	{
	  auto vals =
	    find_extremes(correlation_ptr, ranges.keys(), verbose.getValue());
	  cout << "min at " << correlation_ptr->call_string(vals.first.second)
	       << " = " << vals.first.first << " "
	       << correlation_ptr->unit.symbol <<endl
	       << "max at " << correlation_ptr->call_string(vals.second.second)
	       << " = " << vals.second.first << " "
	       << correlation_ptr->unit.symbol << endl
	       << endl;
	  return;
	}

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

      auto unit_ptr = Unit::search_by_name(p.symbol);
      if (unit_ptr == nullptr)
	unit_ptr = Unit::search_by_symbol(p.symbol);
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
    maps<VtlQuantity>([] (auto p) { return VtlQuantity(*p.first, p.second); });

  if (python.getValue())
    cout << endl
	 << correlation_ptr->python_call(pars_list) << endl
	 << endl;

  if (server.getValue())
    {
      auto ret = correlation_ptr->execute(pars_list);
      cout << get<0>(ret) << "@ " << get<1>(ret) << "@ "
	   << (get<2>(ret) ? "true" : "false") << " @" << get<3>(ret)
	       << "\"" << endl;
      exit(0);
    }

  if (ignore.getValue())
    {
      auto ret = correlation_ptr->compute(pars_list, check);
      cout << correlation_ptr->call_string(pars_list) << " = " << ret << endl;
    }
  else
    {
      auto ret = correlation_ptr->compute_and_check(pars_list, check);
      cout << correlation_ptr->call_string(pars_list) << " = " << ret << endl;
    }

  cout << endl;
}

int main(int argc, char *argv[])
{
  try
    {
      test(argc, argv);
    }
  catch (overflow_error & e)
    {
      cout << e.what() << " (probably one o more parameters are missing)"
	   << endl;
    }
  catch (exception & e)
    {
      cout << e.what() << endl;
    }
}


