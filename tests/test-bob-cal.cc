

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
      cout << Rvector(get<2>(t)->name, pvt.get_data().compute(0, "rs", get<2>(t)))
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
}

