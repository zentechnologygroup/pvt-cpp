
# include <tclap/CmdLine.h>
# include "temperature-unit.H"

using namespace TCLAP;

void test(int argc, char *argv[])
{
  CmdLine cmd(argv[0], ' ', "0");

  ValueArg<double> temp("t", "temperature", "temperature", true, 0, "temperature");
  cmd.add(temp);

  vector<string> units = { "Kelvin", "Fahrenheit", "Celsius", "Rankine" };
  ValuesConstraint<string> allowed_units(units);
  ValueArg<string> u("u", "unit", "unit", false, "Celsius", &allowed_units);
  cmd.add(u);

  cmd.parse(argc, argv);

  cout << "Unit is " << u.getValue() << endl;

  VtlQuantity t(u.getValue(), temp.getValue());

  // cout << "The units are: " << endl;
  // Unit::units().for_each([] (auto u) { cout << *u << endl; });
  // cout << endl;

  cout << "Read input is " << t << endl;

  for (auto str : units)
    {
      if (str == u.getValue())
	continue;

      cout << "Converted to " << VtlQuantity(str, t) << endl;
    }
  cout << endl;
}

int main(int argc, char *argv[])
{
  test(argc, argv);
  return 0;
}
