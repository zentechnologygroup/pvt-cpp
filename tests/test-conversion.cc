
# include <tclap/CmdLine.h>
# include "pvt-units.H"

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

  cmd.parse(argc, argv);

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
    cout << "    " << val << " = " << VtlQuantity(*u, val) << endl;
}

int main(int argc, char *argv[])
{
  test(argc, argv);
  return 0;
}
