
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
  double t = PVT_INVALID_VALUE, // t is in Fahrenheit
    pb = PVT_INVALID_VALUE; // pb is in psia
  DynList<double> values;
  size_t n = 0; // number of items of values

  Property(const string & __target_name) : target_name(__target_name) {}
  void read_header(istringstream & iss, const Unit & y_ref_unit)
  {
    string data;
    if (not (iss >> data))
      ZENTHROW(CommandLineError, "cannot read tunit");
    tunit = Unit::search(data);
    if (tunit == nullptr)
      ZENTHROW(UnitNotFound, "not found t unit " + data);
    if (&tunit->physical_quantity != &Temperature::get_instance())
      ZENTHROW(InvalidUnit, data + " unit is not for temperature");
    
    if (not (iss >> data))
      ZENTHROW(CommandLineError, "cannot read punit");
    punit = Unit::search(data);
    if (punit == nullptr)
      ZENTHROW(UnitNotFound, "not found p unit " + data);
    if (&punit->physical_quantity != &Pressure::get_instance())
      ZENTHROW(InvalidUnit, data + " unit is not for pressure");
    
    if (not (iss >> data))
      ZENTHROW(CommandLineError, "cannot read punit");
    yunit = Unit::search(data);
    if (yunit == nullptr)
      ZENTHROW(UnitNotFound, "not found y unit " + data);
    if (not yunit->is_sibling(y_ref_unit))
      ZENTHROW(InvalidUnit, data + " is not an unit for " +
	       y_ref_unit.physical_quantity.name);

    if (not (iss >> data))
      ZENTHROW(CommandLineError, "cannot read t value");
    if (not is_double(data))
      ZENTHROW(CommandLineError, data + " for t value is not numeric");
    Quantity<Fahrenheit> tq = VtlQuantity(*tunit, atof(data));
    t = tq.raw();

    if (not (iss >> data))
      ZENTHROW(CommandLineError, "cannot read pb value");
    if (not is_double(data))
      ZENTHROW(CommandLineError, data + " for pb value is not numeric");
    pb = VtlQuantity(*punit, atof(data)).raw();
  }
  void read_values(istringstream & iss)
  {
    string data;
    for (; iss >> data; ++n)
      {
	if (not is_double(data))
	  ZENTHROW(CommandLineError, data + " is not a double");
	values.append(atof(data));
      }
  }
  friend ostream & operator << (ostream & out, const Property & p)
  {
    out << "t = " << p.t << ", pb = " << p.pb << " " << p.punit->name
	<< ", yname = " << p.target_name << " (" << p.yunit->name << ")" << endl;
    p.values.for_each([&out] (auto v) { out << v << " "; });
    return out;
  }
};

struct Rs : public Property
{
  Rs() : Property("rs") {}
  Rs & operator = (const string & str)
  {
    istringstream iss(str);
    read_header(iss, SCF_STB::get_instance());
    read_values(iss);
    return *this;
  }
};

struct Bo : public Property
{
  double bobp = PVT_INVALID_VALUE;
  Bo() : Property("bob") {}
  Bo & operator = (const string & str)
  {
    istringstream iss(str);
    read_header(iss, RB_STB::get_instance());

    string data;
    if (not (iss >> data))
      ZENTHROW(CommandLineError, "cannot read bobp value");
    if (not is_double(data))
      ZENTHROW(CommandLineError, data + " is not a double");
    bobp = atof(data);

    read_values(iss);
    return *this;
  }
};

namespace TCLAP
{
  template<> struct ArgTraits<Rs> { typedef StringLike ValueCategory; };
  template<> struct ArgTraits<Bo> { typedef StringLike ValueCategory; };
}

MultiArg<Rs> rs_arg = { "", "rs", "rs", false,
			"tunit punit rs_unit t pb p-vals rs-vals", cmd};

MultiArg<Bo> bob_arg = { "", "bob", "bob", false,
			"tunit punit bob_unit t pb bobp p-vals bob-vals", cmd};

MultiArg<Rs> boa_arg = { "", "boa", "boa", false,
			"tunit punit boa_unit t pb p-vals boa-vals", cmd};

int main(int argc, char *argv[])
{
  cmd.parse(argc, argv);
  try
    {
      for (auto rs : rs_arg.getValue())
	cout << rs << endl;
      for (auto bob : bob_arg.getValue())
	cout << bob << endl;
      for (auto boa : boa_arg.getValue())
	cout << boa << endl;
    }
  catch (exception & e)
    {
      cout << e.what() << endl;
    }
}
