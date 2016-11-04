

# include <tuple>
# include <iostream>

# include <tclap/CmdLine.h>

# include <metadata/pvt-analyse.H>

using namespace std;
using namespace TCLAP;

int main(int argc, char *argv[])
{
  CmdLine cmd = { argv[0], ' ', "0" };

  ValueArg<string> json_file = { "f", "json-file", "json file", false,
				 "data.json", "json file name", cmd };

  cmd.parse(argc, argv);

  ifstream input(json_file.getValue());
  if (not input)
    {
      cout << "cannot open " << json_file.getValue() << endl;
      return 0;
    }

  PvtAnalyzer pvt(input);

  cout << pvt.get_data().full_desc() << endl;

  pvt.check_data();

  auto bob_samples = pvt.get_data().values(0, "bob");
  cout << Rvector("bob", bob_samples) << endl;

  auto bob_valid = pvt.bob_valid_correlations();

  auto c = bob_valid.get_first();
  cout << c->call_string() << endl << endl;

  auto p = pvt.get_data().values(0, "p");
  auto rs = pvt.get_data().values(0, "rs");

  cout << Rvector("rs", rs) << endl
       << Rvector("p", p) << endl;

  cout << "Bob valid correlations" << endl;
  bob_valid.for_each([] (auto p) { cout << p->call_string() << endl; });
  cout << endl;

  cout << pvt.to_string(pvt.best_bob_correlations()) << endl
       << endl;

  pvt.best_bob_correlations().for_each([&pvt] (auto t)
    {
      cout << Rvector(get<2>(t)->name,
		      pvt.get_data().compute(0, "rs", get<2>(t)))
	   << endl;
    });

  auto bob_lfits = sort(pvt.bob_correlations_lfits(),
			[] (const auto & t1, const auto &t2)
			{
			  return get<4>(t1).sumsq < get<4>(t2).sumsq;
			});

  cout << to_string(format_string(bob_lfits.maps<DynList<string>>([] (auto t)
    {
      auto fit = get<4>(t);
      return DynList<string>({get<2>(t)->name, to_string(fit.c),
	    to_string(fit.m), to_string(fit.sumsq) });
    }))) << endl;

  auto bob_lfits_list = pvt.bob_lfits_list(bob_lfits);

  cout << pvt.to_R("tuned.", bob_lfits_list) << endl;

  cout << pvt.to_R(pvt.get_data().values(0, "bob"),
   		   pvt.get_data().values(0, "p"), "p", "Bob",
		   pvt.best_bob_correlations()) << endl;

  auto best_corr = get<2>(bob_lfits.get_first());

  cout << "The best correlation is " << best_corr->call_string() << endl;

  using P = tuple<bool, string, double, const Unit *>;

  DynList<Correlation::NamedPar> pars = best_corr->get_preconditions().
    maps<P>([&pvt] (const auto & par)
    {
      auto par_alias = par.names();
      for (auto it = par_alias.get_it(); it.has_curr(); it.next())
	{
	  auto syn = it.get_curr();
	  const auto & name = syn.first;
	  cout << "Searching " << name << " for " << par.name << endl;
	  try
	    {
	      auto values = pvt.get_data().quantities("Below Pb", name);
	      if (values.first.is_empty())
		continue;
	      return make_tuple(true, par.name,
				values.first.get_last(), syn.second);
	    }
	  catch (...) { /* ignore it! it could be in another synonym */ }
	}
      return make_tuple(false, par.name, 0.0, &Unit::null_unit);
    });

  cout << "List of parameters values used for computing "
       << best_corr->call_string() << endl;
  pars.for_each([] (const auto & t)
		{
		  cout << get<1>(t);
		  if (not get<0>(t))
		    cout << " Not found in data set";
		  cout << endl;
		});

  pars = pvt.get_data().get_last_row_named_pars(best_corr, 0);

  cout << "List of parameters values used for computing "
       << best_corr->call_string() << endl;
  pars.for_each([] (const auto & t)
		{
		  cout << get<1>(t);
		  if (not get<0>(t))
		    cout << " Not found in data set";
		  cout << endl;
		});
}

