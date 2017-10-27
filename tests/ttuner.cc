
# include <istream>

# include <ah-dispatcher.H>
# include <ah-stl-utils.H>

# include <tclap/CmdLine.h>

# include <json.hpp>

# include <units.H>

auto & __units_system = UnitsInstancer::init();

# include <metadata/pvt-tuner.H>

using namespace std;
using namespace TCLAP;
using namespace Aleph;

PvtData data;

CmdLine cmd = { "ttuner", ' ', "0" };

struct ValuesArg
{
  const Unit * tunit = nullptr;
  const Unit * punit = nullptr;
  const Unit * yunit = nullptr;
  double t = PVT_INVALID_VALUE, pb = PVT_INVALID_VALUE;
  DynList<double> values;

  ValuesArg() {}
  ValuesArg(const string & target_name) {}
};

namespace TCLAP
{
  template<> struct ArgTraits<ValuesArg> { typedef StringLike ValueCategory; };
}

MultiArg<ValuesArg> rs_arg = { "", "rs", "rs", false,
			       "tunit punit rs_unit t pb p-vals rs-vals", cmd};

int main(int argc, char *argv[])
{
  cmd.parse(argc, argv);
  try
    {

    }
  catch (exception & e)
    {
      cout << e.what() << endl;
    }
}
