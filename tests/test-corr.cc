
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
	const double s = (par.max_val - par.min_val)/n;
	DynList<double> ret;
	for (double v = par.min_val; v <= par.max_val; v += s)
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

void full_test(const Correlation * const corr_ptr, size_t n, bool verbose)
{
  auto samples = generate_samples(corr_ptr, n);
  auto results = samples.map<double>([corr_ptr, verbose] (auto sample)
    {
      if (verbose)
	{
	  double ret = 0;
	  cout << corr_ptr->call_string(sample) << " = ";
	  try
	    {
	      ret = corr_ptr->compute_and_check(sample);
	    }
	  catch (exception & e)
	    {
	      cout << e.what();
	    }
	  cout << ret << " " << corr_ptr->unit.symbol << endl;
	  return ret;
	}
      
      try
	{
	  return corr_ptr->compute_and_check(sample);
	}
      catch (exception & e)
	{
	  ostringstream s;
	  s << corr_ptr->call_string(sample) << ": " << e.what();
	  throw domain_error(s.str());
	}
    });

  auto mat = zip(samples, results).map<DynList<string>>([] (auto p)
    {
      DynList<string> ret;
      ret.append(p.first.template map<string>([] (auto v)
					      { return to_string(v); }));
      ret.append(to_string(p.second));
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
	const double s = (par.max_val - par.min_val)/n;
	DynList<double> ret;
	for (double v = par.min_val; v <= par.max_val; v += s)
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

  vector<string> correlations;
  Correlation::list().for_each([&correlations] (auto p)
			       { correlations.push_back(p->name); });
  ValuesConstraint<string> allowed = correlations;
  ValueArg<string> correlation = { "C", "correlation", "correlation name",
				   true, "", &allowed};
  cmd.add(correlation);

  SwitchArg print = { "p", "print", "print correlation information", false };
  cmd.add(print);

  SwitchArg full = { "f", "full", "full test", false };
  cmd.add(full);

  ValueArg<size_t> n = { "n", "num-samples",
			 "number of samples by parameter", false, 10,
			 "number of samples" };
  cmd.add(n);

  UnlabeledMultiArg<double> pars = { "pars", "correlation parameter values",
				     false, "values" };
  cmd.add(pars);

  SwitchArg extremes = { "x", "extremes", "compute extremes", false };
  cmd.add(extremes);

  SwitchArg verbose = { "v", "verbose", "verbose mode", false };
  cmd.add(verbose);

  cmd.parse(argc, argv);

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

  if (full.getValue())
    {
      full_test(correlation_ptr, n.getValue(), verbose.getValue());
      return;
    }

  auto pars_list =
    zip(correlation_ptr->get_par_types(), vector_to_DynList(pars.getValue())).
   map<VtlQuantity>([] (auto p) { return VtlQuantity(*p.first, p.second); });

  auto ret = correlation_ptr->compute_and_check(pars_list);

  cout << correlation_ptr->call_string(pars_list) << ") = " << ret << endl;

  cout << endl;
}

int main(int argc, char *argv[])
{
  test(argc, argv);
  
  return 0;
}
