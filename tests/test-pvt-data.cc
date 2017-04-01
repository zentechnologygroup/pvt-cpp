
# include <istream>

# include <ah-dispatcher.H>
# include <ah-stl-utils.H>

# include <tclap/CmdLine.h>

# include <metadata/pvt-data.H>

using namespace std;
using namespace TCLAP;
using namespace Aleph;

// Defines a input of form
// "property-name property-unit t tunit pb punit p-list property-list"
struct ValuesArg
{
  string target_name; // property name
  const Unit * unit_ptr = nullptr; // unit for property

  double t; // temperature
  const Unit * tunit_ptr = nullptr; // temperature unit

  double pb; // bubble point
  const Unit * punit_ptr = nullptr; // pressure unit (for pb and p)
  Array<double> p;
  
  Array<double> values; // property values

  friend ostream & operator << (ostream & out, const ValuesArg & a)
  {
    out << a.target_name << " " << a.unit_ptr->name << " " << a.t << " "
	<< a.tunit_ptr->name << " " << a.pb << " " << a.punit_ptr->name << " ";
    zip_for_each([&out] (auto t)
		 { out << " " << get<0>(t) << " " << get<1>(t); },
		 a.p, a.values);
    return out;
  }

  ValuesArg() {}

  ValuesArg & operator = (const string & str)
  {
    istringstream iss(str);

    // read target_name
    if (not (iss >> target_name))
      ZENTHROW(CommandLineError, str + " does not contain target name");
    if (not Correlation::array().exists([this] (auto ptr)
				{ return ptr->target_name() == target_name; }))
      ZENTHROW(CommandLineError, target_name + " is invalid as target name");

    { // read property unit
      string property_unit;
      if (not (iss >> property_unit))
	ZENTHROW(CommandLineError, "cannot read property unit");
      unit_ptr = Unit::search(property_unit);
      if (punit_ptr == nullptr)
	ZENTHROW(CommandLineError, property_unit + " unit for " + target_name +
		 " property not found");
    }

    // read temperature value
    string data;
    if (not (iss >> data))
      ZENTHROW(CommandLineError, "temperature value not found");
    if (not is_double(data))
      ZENTHROW(CommandLineError, "temperature value " + data +
	       " is not a double");
    t = atof(data);

     // read temperature unit
    string unit_name;
    if (not (iss >> unit_name))
      ZENTHROW(CommandLineError, "cannot read temperature unit");
    tunit_ptr = Unit::search(unit_name);
    if (tunit_ptr == nullptr)
      ZENTHROW(CommandLineError, unit_name + " for temperature not found");
    if (&tunit_ptr->physical_quantity != &Temperature::get_instance())
      ZENTHROW(CommandLineError, unit_name + " is not a temperature unit");

    // read pb value
    if (not (iss >> data))
      ZENTHROW(CommandLineError, "pb value not found");
    if (not is_double(data))
      ZENTHROW(CommandLineError, "pb value " + data + " is not a double");
    pb = atof(data);

    // read pressure unit
    if (not (iss >> unit_name))
      ZENTHROW(CommandLineError, str + " does not contain unit name");
    punit_ptr = Unit::search(unit_name);
    if (punit_ptr == nullptr)
      ZENTHROW(CommandLineError, unit_name + " for pressure not found");
    if (&punit_ptr->physical_quantity != &Pressure::get_instance())
      ZENTHROW(CommandLineError, unit_name + " is not for pressure");

    DynList<double> vals;
    size_t n = 0;
    for (; iss >> data; ++n)
      {
	if (not is_double(data))
	  ZENTHROW(CommandLineError, data + " is not a double");
	vals.append(atof(data));
      }

    if (n % 2 != 0)
      ZENTHROW(CommandLineError, "number of values is not even");

    auto it = vals.get_it();
    for (size_t i = 0; i < n/2; ++i, it.next())
      p.append(it.get_curr());

    if (not p.exists([this] (auto v) { return v == pb; }))
      ZENTHROW(CommandLineError, "pb value " + to_string(pb) +
	       " not found in pressures array");

    for (size_t i = 0; i < n/2; ++i, it.next())
      values.append(it.get_curr());

    return *this;
  }
};

namespace TCLAP
{
  template <> struct ArgTraits<ValuesArg> { typedef StringLike ValueCategory; };
}

// allowed parameter names (they are values ​​or ranges, but they are
// not correlations). This table is used for validating change of
// units
DynSetTree<string> const_name_tbl =
  { "api", "rsb", "yg", "tsep", "psep", "h2s", "co2", "n2", "nacl" };

// input parameter unit change specification
//
// form is: --unit "par-name unit"
struct ArgUnit
{
  string name;
  string unit_name;

  ArgUnit & operator = (const string & str)
  {
    istringstream iss(str);
    if (not (iss >> name >> unit_name))
      ZENTHROW(CommandLineError, str + " is not a pair par-name unit");

    if (not const_name_tbl.contains(name))
      ZENTHROW(CommandLineError, name + " is an invalid parameter name");

    return *this;
  }

  ArgUnit() {}

  friend ostream& operator << (ostream &os, const ArgUnit & a) 
  {
    return os << a.name << " " << a.unit_name;
  }
};

namespace TCLAP
{
  template<> struct ArgTraits<ArgUnit> { typedef StringLike ValueCategory; };
}

struct ActionType
{
  static const DynSetTree<string> valid_actions;
  static const AHDispatcher<string, void (*)(ActionType*, istringstream*)>
  dispatcher;
  string type;
  DynList<const Correlation*> corr_list;
  DynList<pair<const Correlation*, const Correlation*>> pair_list;

  ActionType() {}

  ActionType & operator = (const string & str)
  {
    istringstream iss(str);

    if (not (iss >> type))
      ZENTHROW(CommandLineError, "cannot read action type");
    if (not valid_actions.contains(type))
      {
	ostringstream s;
	s << type << " is not a valid action (must be";
	valid_actions.for_each([&s] (auto & a) { s << " " << a; });
	s << ")";
	ZENTHROW(CommandLineError, s.str());
      }

    dispatcher.run(type, this, &iss);

    return *this;
  }

  static void list_correlations(ActionType * action, istringstream * iss)
  {
    string property_name;
    if (not (*iss >> property_name))
      ZENTHROW(CommandLineError, "cannot read property name");
    action->corr_list = Correlation::array().filter([&property_name] (auto p)
      {
	return p->target_name() == property_name;
      });
    if (action->corr_list.is_empty())
      ZENTHROW(CommandLineError, property_name + " not found");
  }

  static void dummy(ActionType*, istringstream*)
  {
    cout << "Not implemented" << endl;
    abort();
  };
};

namespace TCLAP
{
  template<> struct ArgTraits<ActionType> { typedef StringLike ValueCategory; };
}

const DynSetTree<string> ActionType::valid_actions = 
    { "list", "match", "apply", "local_calibration", "global_calibration" };

const string actions =
 ActionType::valid_actions.take(ActionType::valid_actions.size() - 1).
	    foldl<string>("", [] (auto & a, auto & s)
	      {
		return a + s + " | ";
	      }) + ActionType::valid_actions.get_last();

const AHDispatcher<string, void (*)(ActionType*, istringstream*)>
ActionType::dispatcher 
(
 "list", ActionType::list_correlations,
 "match", ActionType::dummy,
 "apply", ActionType::dummy,
 "local_calibration", ActionType::dummy,
 "global_calibration", ActionType::dummy
);

CmdLine cmd = { "calibrate", ' ', "0" };

/// TODO: documentar action lista 
ValueArg<ActionType> action = { "", "action", "action", true, ActionType(),
				actions, cmd };

// Unit change specification. Suitable for any parameter
MultiArg<ArgUnit> unit = { "", "unit", "change unit of input data", false,
			   "unit \"par-name unit\"", cmd };

// Constant parameters
ValueArg<double> api = { "", "api", "api", true, 0, "api", cmd };
ValueArg<double> rsb = { "", "rsb", "rsb", true, 0, "rsb in SCF_STB", cmd };
ValueArg<double> yg = { "", "yg", "yg", true, 0, "yg in Sgg", cmd };
ValueArg<double> tsep = { "", "tsep", "tsep", false, 0, "tsep in Fahrenheit",
			  cmd };
ValueArg<double> psep = { "", "psep", "psep", false, 0, "psep in psia", cmd };
ValueArg<double> h2s = { "", "h2s", "h2s", false, 0, "h2s in MolePercent", cmd };
ValueArg<double> co2 = { "", "co2", "co2", false, 0, "co2 in MolePercent", cmd };
ValueArg<double> n2 = { "", "n2", "n2", false, 0, "n2 in MolePercent", cmd };
ValueArg<double> nacl = { "", "nacl", "nacl", false, 0, "nacl in Molality_NaCl",
			  cmd };

MultiArg<ValuesArg> target =
  { "", "property", "property array", false, 
    "property array in format \"property property-unit t tunit pb punit p-list "
    "property-list\"", cmd };



const Unit * test_unit(const string & par_name, const Unit & dft_unit)
{
  if (not const_name_tbl.has(par_name))
    ZENTHROW(CommandLineError, "unknown parameter name " + par_name);

  const Unit * ret = &dft_unit;
  for (auto & par : unit.getValue())
    {
      const Unit * ret = Unit::search_by_name(par.unit_name);
      if (ret == nullptr)
	ZENTHROW(CommandLineError, par.unit_name + " unit requested for " +
		 par_name + " does not exist");
      if (&dft_unit.physical_quantity != &ret->physical_quantity)
	ZENTHROW(CommandLineError, par_name + " unit: physical quantity " +
		 ret->physical_quantity.name + " is invalid");
      return ret;
    }
  return ret;
}

PvtData data;

void build_pvt_data()
{
  data.add_const("api", api.getValue(), *test_unit("api", Api::get_instance()));
  data.add_const("rsb", rsb.getValue(), *test_unit("rsb", psia::get_instance()));
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

  for (auto & a : target.getValue())
    data.add_vector(a.t, a.pb, a.p, *a.punit_ptr,
		    a.target_name, a.values, *a.unit_ptr);
}

void dummy()
{
  cout << "No implemented" << endl;
}

// TODO:"que process retornen listas de lineas ==> ordenamiento es natural

void process_list()
{
  ActionType & action = ::action.getValue();
  
  assert(action.type == "list");
  // TODO: ponerlos parámetros con sus tipos
  action.corr_list.for_each([] (auto p) { cout << p->name << endl; });
}

const AHDispatcher<string, void (*)()> dispatcher =
  {
    "list", process_list,
    "match", dummy,
    "apply", dummy,
    "local_calibration", dummy,
    "global_calibration", dummy
  };

int main(int argc, char *argv[])
{
  cmd.parse(argc, argv);

  dispatcher.run(action.getValue().type);

  // TODO: falta sort
}
