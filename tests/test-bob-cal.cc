

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

  auto rs = pvt.get_data().values(0, "rs");

  cout << Rvector("rs", rs) << endl;

  cout << "Bob valid correlations" << endl;
  bob_valid.for_each([] (auto p) { cout << p->call_string() << endl; });
  cout << endl;

  cout << pvt.to_string(pvt.best_bob_correlations()) << endl
       << endl;

  auto bob_lfits = pvt.bob_correlations_lfits();

  bob_lfits.for_each([&pvt] (auto t)
		     {
		       cout << pvt.to_R(t) << endl
			    << pvt.to_string(t) << endl;
		     });
}

