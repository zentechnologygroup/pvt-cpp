
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

struct Property
{
  const string target_name = "invalid";
  const Unit * tunit = nullptr;
  const Unit * punit = nullptr;
  const Unit * yunit = nullptr;
  double t = PVT_INVALID_VALUE, pb = PVT_INVALID_VALUE;
  DynList<double> values;

  Property(const string & __target_name) : target_name(__target_name) {}
  void read_header(istringstream & iss)
  {
    
  }
};

struct Rs : public Property
{
  Rs() : Property("rs") {}
  Rs & operator = (const string & str)
  {
    istringstream s;


    return *this;
  }
};

namespace TCLAP
{
  template<> struct ArgTraits<Rs> { typedef StringLike ValueCategory; };
}

MultiArg<Rs> rs_arg = { "", "rs", "rs", false,
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
