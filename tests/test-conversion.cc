
# include <tclap/CmdLine.h>
# include "correlation.H"

using namespace TCLAP;

void test(int argc, char *argv[])
{
  CmdLine cmd(argv[0], ' ', "0");

  vector<string> units;
  Unit::units().for_each([&units] (auto u) { units.push_back(u->symbol); });
  ValuesConstraint<string> allowed(units);
  ValueArg<string> unit = { "u", "unit-symbol", "symbol of unit",
			    true, "", &allowed };
  cmd.add(unit);

  ValueArg<double> sample("s", "sample", "sample", true, 0, "sample");
  cmd.add(sample);

  SwitchArg v("v", "verbose", "verbose mode", false);
  cmd.add(v);

  cmd.parse(argc, argv);

  auto verbose = v.getValue();

  auto unit_ptr = Unit::search_by_symbol(unit.getValue());
  if (unit_ptr == nullptr)
    {
      cout << "Unit symbol " << unit.getValue() << " not found" << endl;
      abort();
    }

  cout << *unit_ptr << endl
       << endl
       << "Conversions:" << endl
       << endl;

  VtlQuantity val(*unit_ptr, sample.getValue());

  for (auto u : Unit::units(unit_ptr->physical_quantity))
    {
      if (verbose)
	cout << "    " << val << " to " << u->symbol << endl;
      if (exist_conversion(*unit_ptr, *u))
	cout << "    " << val << " = " << VtlQuantity(*u, val) << endl;
      else
	cout << "    "  << val << " = "
	     << "WARNING: there is no conversion from " << unit_ptr->symbol
	     << " to " << u->symbol << endl;
    }
}

int main(int argc, char *argv[])
{
  test(argc, argv);
  return 0;
}
