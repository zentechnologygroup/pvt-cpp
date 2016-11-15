
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

  ValueArg<double> bobp = { "b", "bobp", "bo at bubble point", false,
			    0.0, "bo at bubble point", cmd };

  ValueArg<string> bobp_unit = { "u", "bobp-unit", "unit used for bobp", false,
				 "", "unit symbol used for bobp", cmd };

  cmd.parse(argc, argv);

  ifstream input(json_file.getValue());
  if (not input)
    {
      cout << "cannot open " << json_file.getValue() << endl;
      return 0;
    }

  PvtAnalyzer pvt(input);

  auto bob_vals = pvt.get_data().search_variable("Below Pb", "bob");
  if (not get<0>(bob_vals))
    {
      cout << "The data set does not contain the bob variable" << endl;
      abort();
    }

  auto bob_val_unit = get<2>(bob_vals);

  double last_bob_val = get<1>(bob_vals).get_last();

  if (bobp_unit.isSet())
    {
      auto unit_ptr = Unit::search_by_symbol(bobp_unit.getValue());
      if (unit_ptr == nullptr)
	{
	  cout << "Unit symbol " << bobp_unit.getValue() << " not found"
	       << endl;
	  abort();
	}

      if (not unit_ptr->is_sibling(*bob_val_unit))
	{
	  cout << "Defined unit for bobp " << unit_ptr->name
	       << " is not physically related to defined unit for bob "
	       << bob_val_unit->name << endl;
	  abort();
	}

      bob_val_unit = unit_ptr;
    }

  if (bobp.isSet())
    pvt.get_data().def_const("bobp", bobp.getValue(), bob_val_unit);
  else
    pvt.get_data().def_const("bobp", last_bob_val, bob_val_unit);

  pvt.check_data();

  cout << pvt.get_data().full_desc() << endl
       << endl
       << "Uod correlations:" << endl;
  pvt.uod_correlations().for_each([] (auto p)
				  {
				    cout << "  " << p->call_string() << endl;
				  });
  cout << endl
       << "valid Uod corelations:" << endl;
  pvt.uod_valid_correlations().for_each([] (auto p)
    { 
      cout << "  " << p->call_string() << endl;
    });
  cout << endl
       << "Uob complete correlations:" << endl;
  pvt.uob_correlations().for_each([] (auto p)
    { 
      cout << "  " << p->call_string() << endl;
    });
  cout << endl
     << "Uob valid correlations:" << endl;
  pvt.uob_valid_correlations().for_each([] (auto p)
    { 
      cout << "  " << p->call_string() << endl;
    });
  cout << endl
       << "Uob incomplete correlations:" << endl;
  pvt.uob_incomplete_correlations().for_each([&] (auto p)
    {
      cout << "  " << p.first->call_string() << endl;
      p.second.for_each([&] (auto p)
      {
	cout << "    " << p.first << endl;
	p.second.for_each([&] (auto p)
          {
	    cout << "      " << p->call_string() << " "
		 << pvt.get_data().can_be_applied(p) << endl;
	  });
      });
      cout << endl;
    });

  auto required = pvt.uob_required_values();
  cout << "Uob required values:";
  required.for_each([] (const auto & s) { cout << " " << s; });
  cout << endl
       << endl
       << "Available correlations" << endl;

  pvt.compute_required_values(required).for_each([] (auto t)
    {
      cout << get<0>(t) << " = " << get<2>(t) << " " << get<1>(t)->call_string()
	   << endl;
    });

  auto uob_correlations = pvt.uob_incomplete_correlations();
  cout << "rows:" << endl;
  pvt.best_incomplete_correlations(uob_correlations, 0, "p");
}

