
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
  double bobp = PVT_INVALID_VALUE, uod = PVT_INVALID_VALUE,
    uobp = PVT_INVALID_VALUE;
  DynList<double> values;
  size_t n = 0; // number of items of values
  Array<double> p, y;

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

  void separate()
  {
    if (n % 2 != 0)
      ZENTHROW(CommandLineError,
	       "number of pressure values plus property values is not even");
    for (size_t i = 0; i < n/2; ++i)
      p.append(values.remove_first());
    for (size_t i = 0; i < n/2; ++i)
      y.append(values.remove_first());
  }
  
  friend ostream & operator << (ostream & out, const Property & p)
  {
     out << "t = " << p.t << ", pb = " << p.pb << " " << p.punit->name
	 << ", yname = " << p.target_name;
     if (p.bobp != PVT_INVALID_VALUE)
       out << " , bobp = " << p.bobp;
     if (p.uod != PVT_INVALID_VALUE)
       out << " , uod = " << p.uod;
     if (p.uobp != PVT_INVALID_VALUE)
       out << " , uobp = " << p.uobp;
     return out << " (" << p.yunit->name << ")" << endl
		<< "p =" << join(p.p, ", ") << endl
		<< "y =" << join(p.y, ", ") << endl;
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
    separate();
    return *this;
  }
};

struct Coa : public Property
{
  Coa() : Property("coa") {}
  Coa & operator = (const string & str)
  {
    istringstream iss(str);
    read_header(iss, psia_1::get_instance());
    read_values(iss);
    if (n % 3 != 0)
      ZENTHROW(CommandLineError,
	       "number of pressure plus property values is not multiple of 3");
    const size_t N = n/3;
    for (size_t i = 0; i < 2*N; ++i)
      p.append(values.remove_first());
    for (size_t i = 0; i < N; ++i)
      y.append(values.remove_first());
    return *this;
  }
};

struct Bo : public Property
{
  Bo(const string & target_name) : Property(target_name) {}
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
    separate();
    return *this;
  }
};

struct Bob : public Bo
{
  Bob() : Bo("bob") {}
  Bob & operator = (const string & str)
  {
    *static_cast<Bo*>(this) = str;
    return *this;
  }
};

struct Boa : public Bo
{
  Boa() : Bo("boa") {}
  Boa & operator = (const string & str)
  {
    *static_cast<Bo*>(this) = str;
    return *this;
  }
};

struct Uo : public Property
{
  Uo(const string & target_name) : Property(target_name) {}
  Uo & operator = (const string & str)
  {
    istringstream iss(str);
    read_header(iss, CP::get_instance());

    string data;
    if (not (iss >> data))
      ZENTHROW(CommandLineError, "cannot read uobp value");
    if (not is_double(data))
      ZENTHROW(CommandLineError, data + " is not a double");
    uobp = atof(data);

    read_values(iss);
    separate();
    return *this;
  }
};

struct Uob : public Uo
{
  Uob() : Uo("uob") {}
  Uob & operator = (const string & str)
  {
    *static_cast<Uo*>(this) = str;
    return *this;
  }
};

struct Uoa : public Uo
{
  Uoa() : Uo("uoa") {}
  Uoa & operator = (const string & str)
  {
    *static_cast<Uo*>(this) = str;
    return *this = str;
  }  
};

struct Uosplit : public Property
{
  Uosplit() : Property("uo") {}
  Uosplit & operator = (const string & str)
  {
    return *this = str;
  }  
};

namespace TCLAP
{
  template<> struct ArgTraits<Rs> { typedef StringLike ValueCategory; };
  template<> struct ArgTraits<Coa> { typedef StringLike ValueCategory; };
  template<> struct ArgTraits<Bob> { typedef StringLike ValueCategory; };
  template<> struct ArgTraits<Boa> { typedef StringLike ValueCategory; };
  template<> struct ArgTraits<Uob> { typedef StringLike ValueCategory; };
  template<> struct ArgTraits<Uoa> { typedef StringLike ValueCategory; };
  template<> struct ArgTraits<Uosplit> { typedef StringLike ValueCategory; };
}

// Vector paramaters
MultiArg<Rs> rs_arg = { "", "rs", "rs", false,
			"tunit punit rs_unit t pb p-vals rs-vals", cmd};

MultiArg<Coa> coa_arg = { "", "coa", "coa", false,
			  "tunit punit rs_unit t pb p-vals coa-vals", cmd};

MultiArg<Bob> bob_arg = { "", "bob", "bob", false,
			  "tunit punit bob_unit t pb bobp p-vals bob-vals", cmd};

MultiArg<Boa> boa_arg = { "", "boa", "boa", false,
			  "tunit punit boa_unit t pb bobp p-vals boa-vals", cmd};

MultiArg<Uob> uob_arg = { "", "uob", "uob", false,
			  "tunit punit boa_unit t pb uobp p-vals uoa-vals", cmd};

MultiArg<Uoa> uoa_arg = { "", "uoa", "uoa", false,
			  "tunit punit uoa_unit t pb uobp p-vals uoa-vals", cmd};

MultiArg<Uosplit> uo_arg = { "", "uo", "uo", false,
			     "tunit punit uo_unit t pb p-vals uo-vals", cmd};

// Constant parameters
ValueArg<double> api = { "", "api", "api", false, 0, "api", cmd };
ValueArg<double> rsb = { "", "rsb", "rsb", false, 0, "rsb in SCF_STB", cmd };
ValueArg<double> yg = { "", "yg", "yg", false, 0, "yg in Sgg", cmd };
ValueArg<double> tsep = { "", "tsep", "tsep", false, 0, "tsep in Fahrenheit",
			  cmd };
ValueArg<double> psep = { "", "psep", "psep", false, 0, "psep in psia", cmd };
ValueArg<double> h2s = { "", "h2s", "h2s", false, 0, "h2s in MolePercent", cmd };
ValueArg<double> co2 = { "", "co2", "co2", false, 0, "co2 in MolePercent", cmd };
ValueArg<double> n2 = { "", "n2", "n2", false, 0, "n2 in MolePercent", cmd };
ValueArg<double> nacl = { "", "nacl", "nacl", false, 0, "nacl in Molality_NaCl",
			  cmd };

// allowed parameter names (they are values ​​or ranges, but they are
// not correlations). This table is used for validating change of
// units
static DynSetTree<string> const_name_tbl =
  { "api", "rsb", "yg", "tsep", "psep", "h2s", "co2", "n2", "nacl" };

// input parameter unit change specification
//
// form is: --unit "par-name unit"
struct ArgUnit
{
  string name;
  const Unit * unit_ptr = nullptr;

  ArgUnit & operator = (const string & str)
  {
    string unit_name;
    istringstream iss(str);
    if (not (iss >> name >> unit_name))
      ZENTHROW(CommandLineError, str + " is not a pair par-name unit");

    if (not const_name_tbl.contains(name))
      ZENTHROW(CommandLineError, name + " is an invalid parameter name");

    unit_ptr = Unit::search(unit_name);
    if (unit_ptr == nullptr)
      ZENTHROW(CommandLineError, "cannot find unit " + unit_name);

    return *this;
  }

  ArgUnit() {}

  friend ostream& operator << (ostream &os, const ArgUnit & a) 
  {
    return os << a.name << " " << a.unit_ptr->name;
  }
};

namespace TCLAP
{
  template<> struct ArgTraits<ArgUnit> { typedef StringLike ValueCategory; };
}

// Unit change specification. Suitable for any parameter
MultiArg<ArgUnit> unit = { "", "unit", "change unit of input data", false,
			   "unit \"par-name unit\"", cmd };

const Unit * test_unit(const string & par_name, const Unit & dft_unit)
{
  
  if (not const_name_tbl.has(par_name))
    ZENTHROW(CommandLineError, "unknown parameter name " + par_name);

  const Unit * ret = &dft_unit;
  for (auto & par : unit.getValue())
    if (par.name == par_name)
      {
	if (&dft_unit.physical_quantity != &par.unit_ptr->physical_quantity)
	  ZENTHROW(CommandLineError, par_name + " unit: physical quantity " +
		   ret->physical_quantity.name + " is invalid");
	return ret;
      }
  return ret;
}

void build_pvt_data()
{
  if (api.isSet())
    data.add_const("api", api.getValue(),
		   *test_unit("api", Api::get_instance()));
  if (rsb.isSet())
    data.add_const("rsb", rsb.getValue(), *test_unit("rsb",
						     SCF_STB::get_instance()));
  if (yg.isSet())
    data.add_const("yg", yg.getValue(), *test_unit("yg", Sgg::get_instance()));
  if (tsep.isSet())
    data.add_const("tsep", tsep.getValue(),
		   *test_unit("tsep", Fahrenheit::get_instance()));
  if (psep.isSet())
    data.add_const("psep", psep.getValue(),
		   *test_unit("psep", psia::get_instance()));
  if (h2s.isSet())
    data.add_const("h2s", h2s.getValue(),
		   *test_unit("h2s", MolePercent::get_instance()));
  if (co2.isSet())
    data.add_const("co2", co2.getValue(),
		   *test_unit("co2", MolePercent::get_instance()));
  if (n2.isSet())
    data.add_const("n2", n2.getValue(),
		   *test_unit("n2", MolePercent::get_instance()));
  if (nacl.isSet())
    data.add_const("nacl", nacl.getValue(),
		   *test_unit("nacl", Molality_NaCl::get_instance()));

  for (auto rs : rs_arg.getValue())
    cout << rs << endl;

  for (auto bob : bob_arg.getValue())
    cout << bob << endl;

  for (auto coa : coa_arg.getValue())
    cout << coa << endl;
    // data.add_vector(rs.t, rs.pb, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
    // 		    PVT_INVALID_VALUE, rs.p, *a.punit_ptr,
    // 		    a.target_name, a.values, *a.unit_ptr);
}

int main(int argc, char *argv[])
{
  cmd.parse(argc, argv);
  try
    {
      build_pvt_data();
    }
  catch (exception & e)
    {
      cout << e.what() << endl;
    }
}
