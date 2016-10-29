

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
  auto bob_valid = pvt.bob_valid_correlations();

  //auto velarde = 

  cout << "Bob valid correlations" << endl;
  for (auto it = bob_valid.get_it(); it.has_curr(); it.next())
    {
      const Correlation * ptr = it.get_curr();
      EmpiricalData & data = pvt.get_data();
      DynList<double> r = data.compute(0, ptr);
      cout << Rvector("bob", bob_samples) << endl
	   << Rvector("tuned." + ptr->name, r) << endl;
    }

  cout << endl;

  auto bob_lfits = pvt.bob_correlations_lfits();

  cout << Rvector("bob", bob_samples) << endl;

  bob_lfits.for_each([&pvt] (auto t)
		     {
		       cout << pvt.to_R(t) << endl;
		     });
}

