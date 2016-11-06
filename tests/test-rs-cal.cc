
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
    
  auto cmp = [] (const PvtAnalyzer::VarCorrStatus & s1,
		 const PvtAnalyzer::VarCorrStatus & s2)
    {
      return get<2>(s1)->name < get<2>(s2)->name;
    };

  auto rs_best_mse = sort(pvt.best_rs_correlations("mse"), cmp);

  cout << Rvector("rs", pvt.get_data().values(0, "rs")) << endl;

  rs_best_mse.for_each([] (auto t)
    {
      cout << Rvector(get<2>(t)->name, get<1>(t)) << endl;
    });

  auto best_list = rs_best_mse.maps<DynList<string>>([] (auto t)
    {
      return DynList<string>({ get<2>(t)->name, to_string(get<3>(t)),
	    to_string(get<4>(t)), to_string(get<5>(t)) });
    });
  best_list.insert({"Correlation", "r2", "mse", "sigma"});
    
  cout << endl
       << to_string(format_string(best_list)) << endl;
  
  cout << pvt.to_R(pvt.get_data().values(0, "rs"),
		   pvt.get_data().values(0, "p"), "Pressure", "Rs",
		   rs_best_mse) << endl;

  auto best_lfits = pvt.rs_correlations_lfits();

  cout << get<4>(best_lfits.get_first()).to_string() << endl
       << pvt.rs_lfit(get<2>(best_lfits.get_first())).to_string() << endl;
         
  assert(get<4>(best_lfits.get_first()) ==
	 pvt.rs_lfit(get<2>(best_lfits.get_first())));

  auto best_lfits_l = best_lfits.maps<DynList<string>>([] (auto t)
    {
      auto r = get<4>(t); // nlfit
      return DynList<string>({ get<2>(t)->name, 
	    to_string(r.c), to_string(r.m), to_string(r.sumsq) });
    });

  best_lfits_l.insert({"Correlation", "c", "m", "sumsq"});

  auto l_tuned = best_lfits.maps<pair<string, DynList<double>>>([&pvt] (auto t)
    {
      auto r = get<4>(t);
      auto ptr = get<2>(t);
      auto values = pvt.get_data().tuned_compute(0, "p", ptr, r.c, r.m);
      return make_pair(ptr->name, move(values));
    });

  cout << pvt.to_R("ltuned.", l_tuned) << endl;

  CorrStat stat(pvt.get_data().values(0, "rs"));
  auto all_tuned =
    l_tuned.maps<pair<string, DynList<double>>>([] (auto p)
      {
	return make_pair("ltuned." + p.first, p.second);
      });

  auto all_tuned_stats =
    sort(all_tuned.maps<tuple<string, double, double>>([&stat] (auto p)
      {
	return make_tuple(p.first, stat.mse(p.second),
			  stat.sigma_distance(p.second));
      }), [] (auto t1, auto t2) { return get<1>(t1) < get<1>(t2); }).
    maps<DynList<string>>([] (auto t)
      {
	return DynList<string>({ get<0>(t), to_string(get<1>(t)),
	      to_string(get<2>(t)) });
      });
  all_tuned_stats.
    insert(DynList<string>({"Correlation", "mse", "sigma_distance"}));

   cout << "Tuned correlations sorted by mse:" << endl
        << to_string(format_string(all_tuned_stats)) << endl;

  cout << endl
       << "Linear" << endl
       << to_string(format_string(best_lfits_l)) << endl
       << endl;
}

