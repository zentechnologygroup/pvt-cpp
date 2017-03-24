
# include <istream>

# include <ah-stl-utils.H>

# include <tclap/CmdLine.h>

# include <metadata/pvt-data.H>

using namespace TCLAP;

int main(int argc, char *argv[])
{
  PvtData pvtdata;

  pvtdata.add_const("rsb", 1110, SCF_STB::get_instance());
  pvtdata.add_const("api", 27, Api::get_instance());
  pvtdata.add_const("t", 189, Fahrenheit::get_instance());

  pvtdata.const_values.for_each([] (auto & d) { cout << d << endl; });
  
  return 0;
}
