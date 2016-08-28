
# include <tclap/CmdLine.h>

# include <ah-stl-utils.H>
# include <ah-comb.H>
# include <ahFunctional.H>

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

void test(int argc, char *argv[])
{
  CmdLine cmd(argv[0], ' ', "0");

   auto __units =
     to_vector(Unit::units().map<string>([] (auto u) { return u->symbol; }));
   ValuesConstraint<string> units = __units;
   ValueArg<string> p1 = { "1", "unit-1", "unit for parameter 1", false, "",
			   &units };
   ValueArg<string> p2 = { "2", "unit-2", "unit for parameter 2", false, "",
			   &units };
  ValueArg<string> p3 = { "3", "unit-3", "unit for parameter 3", false, "",
  			  &units };
  ValueArg<string> p4 = { "4", "unit-4", "unit for parameter 4", false, "",
  			  &units };
  ValueArg<string> p5 = { "5", "unit-5", "unit for parameter 5", false, "",
  			  &units };
  ValueArg<string> p6 = { "6", "unit-6", "unit for parameter 6", false, "",
  			  &units };
  ValueArg<string> p7 = { "7", "unit-7", "unit for parameter 7", false, "",
  			  &units };
  ValueArg<string> p8 = { "8", "unit-8", "unit for parameter 8", false, "",
  			  &units };
  ValueArg<string> p9 = { "9", "unit-9", "unit for parameter 9", false, "",
  			  &units };
  cmd.add(p1);
  cmd.add(p2);
  cmd.add(p3);
  cmd.add(p4);
  cmd.add(p5);
  cmd.add(p6);
  cmd.add(p7);
  cmd.add(p8);
  cmd.add(p9);

  DynList<ValueArg<string>*> unit_ptrs =
    { &p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9 };

  vector<string> correlations;
  auto correlation_list = Correlation::list();
  correlation_list.for_each([&correlations] (auto p)
			    { correlations.push_back(p->name); });
  ValuesConstraint<string> allowed = correlations;
  ValueArg<string> correlation = { "C", "correlation", "correlation name",
				   false, "", &allowed};
  cmd.add(correlation);

  SwitchArg print = { "p", "print", "print correlation information", false };
  cmd.add(print);

  SwitchArg mat = { "f", "mattrix", "generate matrix", false };
  cmd.add(mat);

  SwitchArg csv = { "c", "csv", "generate csv", false };
  cmd.add(csv);

  ValueArg<size_t> n = { "n", "num-steps",
			 "number of steps by parameter", false, 10,
			 "number of samples" };
  cmd.add(n);

  UnlabeledMultiArg<double> pars = { "pars", "correlation parameter values",
				     false, "values" };
  cmd.add(pars);

  SwitchArg extremes = { "x", "extremes", "compute extremes", false };
  cmd.add(extremes);

  SwitchArg verbose = { "v", "verbose", "verbose mode", false };
  cmd.add(verbose);

  SwitchArg ignore = { "i", "ignore-exception", "ignore exception", false };
  cmd.add(ignore);

  SwitchArg python = { "y", "print-python-call", "print python call", false };
  cmd.add(python);

  SwitchArg list_corr = { "l", "list", "list correlations", false };
  cmd.add(list_corr);

  SwitchArg list_Corr = { "L", "List", "detailed list of correlations", false };
  cmd.add(list_Corr);

  ValueArg<string> describe = { "D", "describe-correlation", 
				"describe correlation", false, "", &allowed};
  cmd.add(describe);

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

  int i = 0;
  auto par_types = zip(correlation_ptr->get_par_types(), unit_ptrs).
    map<const Unit * const >([&i] (auto p)
      {
	i++;
	if (not p.second->isSet())
	  return p.first;
	const Unit * const unit_ptr =
	  Unit::search_by_symbol(p.second->getValue());
	return unit_ptr;
      });

  auto pars_list =
    zip(par_types, to_DynList(pars.getValue())).
   map<VtlQuantity>([] (auto p) { return VtlQuantity(*p.first, p.second); });

  if (python.getValue())
    cout << correlation_ptr->python_call(pars_list) << endl
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
