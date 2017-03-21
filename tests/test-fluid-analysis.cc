
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
    
  auto all_pb = pvt.pb_correlations();

  // all_pb.for_each([] (auto p) { cout << *p << endl; });

  cout << "================================================================"
       << endl;

  auto valid_pb = pvt.pb_valid_correlations();

  valid_pb.for_each([] (auto p) { cout << *p << endl; });

  pvt.compute_constant_correlations(valid_pb).for_each([] (auto r)
    {
      cout << get<2>(r)->name << " = " << get<1>(r) << " " << get<3>(r) << endl;
    });
  cout << endl;

  pvt.compute_constant_correlations(all_pb, false).for_each([] (auto r)
    {
      cout << get<2>(r)->name << " = " << get<1>(r) << " " << get<3>(r) << endl;
    });
  cout << endl
       << endl
       << "Best pb correlations" << endl;
  auto l =
    pvt.best_pb_correlations().maps<DynList<string>>([pb = pvt.get_pb()] (auto r)
    {
      auto v = get<1>(r);
      return DynList<string>( { get<2>(r)->name, "=", to_string(v),
	    to_string((min(v, pb)/max(v, pb))*100) + " %",  get<3>(r)});
    });

  cout << to_string(format_string(l)) << endl;

  auto rs_l = pvt.rs_correlations();
  //.for_each([] (auto p) { cout << *p << endl; });

  auto rs_v = pvt.rs_valid_correlations();
  //.for_each([] (auto p) { cout << *p << endl; });

  auto cmp = [] (const PvtAnalyzer::VarCorrStatus & s1,
		 const PvtAnalyzer::VarCorrStatus & s2)
    {
      return get<3>(s1)->name < get<3>(s2)->name;
    };

  auto rs_best_sigma = sort(pvt.best_rs_correlations("sigma_distance"), cmp);
  auto rs_best_r2 = sort(pvt.best_rs_correlations("r2"), cmp);
  auto rs_best_mse = sort(pvt.best_rs_correlations("mse"), cmp);

  cout << Rvector("rs", pvt.get_data().values(0, "rs")) << endl;

  auto best = t_zip(rs_best_r2, rs_best_mse, rs_best_sigma).
    maps<tuple<const Correlation *, double, double, double>>([] (auto t)
    {
      return make_tuple(get<3>(get<0>(t)), get<2>(get<0>(t)),
			get<2>(get<1>(t)), get<2>(get<2>(t)));
    });

  rs_best_mse.for_each([] (auto t)
    {
      cout << Rvector(get<3>(t)->name, get<1>(t)) << endl;
    });

  auto best_list = best.maps<DynList<string>>([] (auto t)
    {
      return DynList<string>({ get<0>(t)->name, to_string(get<1>(t)),
	    to_string(get<2>(t)), to_string(get<3>(t)) });
    });
  best_list.insert({"Correlation", "r2", "mse", "sigma"});
    
  cout << endl
       << to_string(format_string(best_list)) << endl;
  
  // best.for_each([] (auto t)
  // 	     {
  // 	       cout << get<0>(t)->name << " " << get<1>(t) << " "
  // 		    << get<2>(t) << " " << get<3>(t) << endl;
  // 	     });

  cout << pvt.to_R(pvt.get_data().values(0, "rs"),
		   pvt.get_data().values(0, "p"), "Pressure", "Rs",
		   rs_best_mse) << endl;

  cout << "Non linear" << endl;
  auto best_nlfits = pvt.rs_correlations_nlfits();

  auto best_nlfits_l = best_nlfits.maps<DynList<string>>([] (auto t)
    {
      auto r = get<4>(t); // nlfit
      return DynList<string>({ get<2>(t)->name, 
	    to_string(r.c), to_string(r.m), to_string(r.sum_line) });
    });

  best_nlfits_l.insert({"Correlation", "c", "m", "sumsq"});

  auto best_lfits = pvt.rs_correlations_lfits();

  auto best_lfits_l = best_lfits.maps<DynList<string>>([] (auto t)
    {
      auto r = get<4>(t); // nlfit
      return DynList<string>({ get<2>(t)->name, 
	    to_string(r.c), to_string(r.m), to_string(r.sumsq) });
    });

  best_lfits_l.insert({"Correlation", "c", "m", "sumsq"});

  auto nl_tuned = best_nlfits.maps<pair<string, DynList<double>>>([&pvt] (auto t)
    {
      auto r = get<4>(t);
      auto ptr = get<2>(t);
      auto values = pvt.get_data().tuned_compute(0, ptr, r.c, r.m);
      return make_pair(ptr->name, move(values));
    });

  auto l_tuned = best_lfits.maps<pair<string, DynList<double>>>([&pvt] (auto t)
    {
      auto r = get<4>(t);
      auto ptr = get<2>(t);
      auto values = pvt.get_data().tuned_compute(0, ptr, r.c, r.m);
      return make_pair(ptr->name, move(values));
    });

  cout << pvt.to_R("nltuned.", nl_tuned) << endl
       << endl
       << pvt.to_R("ltuned.", l_tuned) << endl;

  CorrStat stat(pvt.get_data().values(0, "rs"));
  auto all_tuned =
    l_tuned.maps<pair<string, DynList<double>>>([] (auto p)
      {
	return make_pair("ltuned." + p.first, p.second);
      });
  all_tuned.append(nl_tuned.maps<pair<string, DynList<double>>>([] (auto p)
      {
	return make_pair("nltuned." + p.first, p.second);
      }));
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
       << "Non linear" << endl
       << to_string(format_string(best_nlfits_l)) << endl
       << "Linear" << endl
       << to_string(format_string(best_lfits_l)) << endl
       << endl;


  cout << "Bob valid correlations" << endl;
  pvt.bob_valid_correlations().for_each([&pvt] (auto p)
    {
      cout << p->name;
      p->names().for_each([] (const auto & s) { cout << " " << s; });
      pvt.get_data().missing_parameters(0, p).for_each([] (const auto & s)
        {
	  cout << " *" << s;
	});
      cout << endl;
    });

  cout << endl;

  auto bob_lfits = pvt.bob_correlations_lfits();

  cout << Rvector("bob", pvt.get_data().values(0, "bob")) << endl;

  bob_lfits.for_each([&pvt] (auto t)
		     {
		       cout << pvt.to_R(t) << endl;
		     });

  

  // cout << "cob correlations" << endl;
  // pvt.cob_correlations().for_each([] (auto p) { cout << p->name << endl; });

  // cout << "All matching correlations" << endl;
  // pvt.get_data().matching_correlations(0).
  //   for_each([] (auto p) { cout << "  " << p->name << endl; });
}

