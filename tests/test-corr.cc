
# include <tclap/CmdLine.h>

# include <ah-stl-utils.H>
# include <ah-comb.H>
# include <ahFunctional.H>

# include <pvt-correlations.H>

using namespace TCLAP;

DynList<DynList<double>>
generate_samples(const DynList<CorrelationPar> & pars, double ratio)
{
  auto steps = pars.map<double>([ratio] (auto par)
				{
				  return (par.max_val - par.min_val)*ratio;
				});
  auto samples =
    zip(pars.map<const CorrelationPar*>([] (const auto & par) { return &par; }),
	steps).map<DynList<double>>([] (auto p)
    {
      auto par = p.first;
      auto step = p.second;
      DynList<double> ret;
      for (double val = par->min_val; val <= par->max_val; val += step)
	ret.append(val);
      return ret;
    });

  return comb(samples);
}

void full_test(const Correlation * const corr_ptr, double step, bool verbose)
{
  if (step <= 0 or step >= 1)
    {
      cout << "step " << step << " is not inside (0, 1) " << endl;
      abort();
    }
  
  auto samples = generate_samples(corr_ptr->get_preconditions(), step);
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

T find_extremes(const Correlation * const corr_ptr, double step)
{
  if (step <= 0 or step >= 1)
    {
      cout << "step " << step << " is not inside (0, 1) " << endl;
      abort();
    }
  
  T init = make_pair(make_pair(numeric_limits<double>::max(), DynList<double>()),
		     make_pair(numeric_limits<double>::min(), DynList<double>()));
  
  auto samples = generate_samples(corr_ptr->get_preconditions(), step);

  return samples.foldl<T>(init, [corr_ptr] (auto a, auto s)
    {
      auto r = corr_ptr->compute(s);

      T ret = a;
      
      if (r < a.first.first)
	ret.first = make_pair(r, s);

      if (r > a.second.first)
	ret.second = make_pair(r, s);

      return ret;
    });
} 

void test(int argc, char *argv[])
{
  CmdLine cmd(argv[0], ' ', "0");

  vector<string> correlations;
  Correlation::list().for_each([&correlations] (auto p)
			       { correlations.push_back(p->name); });
  ValuesConstraint<string> allowed = correlations;
  ValueArg<string> correlation = { "c", "correlation", "correlation name",
				   true, "", &allowed};
  cmd.add(correlation);

  SwitchArg print = { "p", "print", "print correlation information", false };
  cmd.add(print);

  SwitchArg full = { "f", "full", "full test", false };
  cmd.add(full);

  ValueArg<double> step = { "s", "step", "step in full test", false,
			    0.1, "step" };
  cmd.add(step);

  UnlabeledMultiArg<double> pars = { "pars", "correlation parameter values",
				     false, "values" };
  cmd.add(pars);

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

  if (full.getValue())
    {
      full_test(correlation_ptr, step.getValue(), verbose.getValue());
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
