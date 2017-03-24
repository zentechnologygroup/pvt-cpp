
# include <istream>

# include <ah-stl-utils.H>

# include <tclap/CmdLine.h>

# include <metadata/pvt-data.H>

using namespace TCLAP;

int main(int argc, char *argv[])
{
  PvtData pvtdata;

  pvtdata.add_const("t", 189, Fahrenheit::get_instance());
  pvtdata.add_const("api", 26, Api::get_instance());
  pvtdata.add_const("pb", 3891.98, psia::get_instance());
  pvtdata.add_const("rsb", 1110, SCF_STB::get_instance());
  pvtdata.add_const("yg", 0.71, Sgg::get_instance());
  pvtdata.add_const("tsep", 90, Fahrenheit::get_instance());
  pvtdata.add_const("psep", 60, psia::get_instance());
  pvtdata.add_vector("p", psia::get_instance(),
		     { 1000, 2000, 3000, 3500, 3602.9, 3891.98 },
		     "bob", RB_STB::get_instance(),
		     { 1.32591, 1.45799, 1.61047, 1.70097, 1.70942, 1.77922 });
  pvtdata.add_vector("p", psia::get_instance(),
		     { 1000, 2000, 3000, 3500, 3602.9, 3891.98 },
		     "rs", SCF_STB::get_instance(),
		     { 287.701, 521.132, 801.125, 967.446, 982.957, 1110 });


  pvtdata.matches_with_pars("bob").for_each([] (auto ptr)
    {
      cout << ptr->name << endl;
    });
  
  return 0;
}
