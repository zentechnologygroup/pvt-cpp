
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

  // valid_pb.for_each([] (auto p) { cout << *p << endl; });

  // pvt.compute_constant_correlations(valid_pb).for_each([] (auto r)
  //   {
  //     cout << get<2>(r)->name << " = " << get<1>(r) << " " << get<3>(r) << endl;
  //   });
  // cout << endl;

  // pvt.compute_constant_correlations(all_pb, false).for_each([] (auto r)
  //   {
  //     cout << get<2>(r)->name << " = " << get<1>(r) << " " << get<3>(r) << endl;
  //   });
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

  auto rs_best = pvt.best_rs_correlations();

  rs_best.for_each([] (auto t)
    {
      cout << get<2>(t)->name << ":";
      get<1>(t).for_each([] (auto v) { cout << " " << v; }); cout << endl;
    });

  return 0;

  
}




