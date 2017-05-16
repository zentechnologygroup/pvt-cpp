
# include <memory>

# include <tclap/CmdLine.h>

# include <ah-zip.H>
# include <ahSort.H>
# include <ah-dispatcher.H>
# include <tpl_dynMapTree.H>
# include <tpl_dynBinHeap.H>

# include <json.hpp>

using json = nlohmann::json;

# include <correlations/pvt-correlations.H>
# include <correlations/defined-correlation.H>

using namespace std;
using namespace TCLAP;

void error_msg(const string & msg)
{
  cout << msg << endl;
  abort();
}

CmdLine cmd = { "plot-corr", ' ', "0" };

// indicates that correlation parameter ranges must be verified
SwitchArg check_arg = { "", "check", "check correlation ranges", cmd };
bool check = false;
void set_check()
{
  check = check_arg.getValue();
}

// indicates that exceptions must be reported
SwitchArg catch_exceptions = { "", "exceptions", "report exceptions", cmd };
bool report_exceptions = false;
DynList<string> exception_list; // Exceptions messages are saved in this list

// allowed parameter names (they are values ​​or ranges, but they are
// not correlations). This table is used for validating change of
// units
DynSetTree<string> par_name_tbl =
  { "api", "rsb", "yg", "tsep", "tsep2", "t", "p", "psep", "h2s", "co2", "n2",
    "nacl", "ogr" };

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

    if (not par_name_tbl.contains(name))
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

// Unit change specification. Suitable for any parameter
MultiArg<ArgUnit> unit = { "", "unit", "change unit of input data", false,
			   "unit \"par-name unit\"", cmd };

// Checks whether the parameter par_name has a change of
// unity. ref_unit is the default unit of the parameter. If there was
// no change specification for par_name, then returns ref_unit
const Unit * test_par_unit_change(const string & par_name,
				  const Unit & ref_unit)
{
  if (not par_name_tbl.contains(par_name))
    {
      cout << "for option --unit " << par_name << ": invalid parameter name"
	   << endl;
      abort();
    }
 
  const Unit * ret = &ref_unit;
  for (const auto & par : unit.getValue()) // traverse list of changes
    if (par.name == par_name)
      {
	const Unit * ret = Unit::search_by_name(par.unit_name);
	if (ret == nullptr)
	  {
	    cout << "In unit change for " << par_name << ": unit name "
		 << par.unit_name << " not found" << endl;
	    abort();
	  }

	if (&ref_unit.physical_quantity != &ret->physical_quantity)
	  {
	    cout << "For " << par_name << " unit: physical quantity "
		 << ret->physical_quantity.name << " is invalid" << endl;
	    abort();
	  }
	return ret;
      }
 
  return ret;
}

struct PropertyUnit
{
  string name;
  string unit_name;

  PropertyUnit & operator = (const string & str)
  {
    istringstream iss(str);
    if (not (iss >> name >> unit_name))
      ZENTHROW(CommandLineError, str + " is not a pair par-name unit");

    return *this;
  }

  PropertyUnit() {}

  friend ostream& operator << (ostream &os, const PropertyUnit & a) 
  {
    return os << a.name << " " << a.unit_name;
  }
};

namespace TCLAP
{
  template<> struct ArgTraits<PropertyUnit> { typedef StringLike ValueCategory; };
}

MultiArg<PropertyUnit> property_unit = { "", "property-unit",
					 "change unit of property", false,
					 "unit \"property-name unit\"", cmd };

DynMapTree<string, const Unit*> property_units_changes;

// Build the property_units_changes table containing pairs of form
// property,unit_ptr. Only verify that given unit name exists. The
// property name will be verified after when the row name is given
void process_property_unit_changes()
{
  for (auto & p : property_unit.getValue())
    {
      const string & unit_name = p.unit_name;
      const Unit * unit_ptr = Unit::search_by_name(unit_name);
      if (unit_ptr)
	{
	  property_units_changes.insert(p.name, unit_ptr);
	  continue;
	}
      ostringstream s;
      s << "For correlation unit change (flag " << property_unit.getName()
	<< "): unit name " << unit_name << " does not exist";\
      ZENTHROW(InvalidValue, s.str());
    }
}

// Given the csv stack with names and units, this routine builds a
// parallel stack of changed unit columns through --property-unit flags
pair<FixedStack<const Unit*>, FixedStack<Unit_Convert_Fct_Ptr>>
build_stack_of_property_units(const FixedStack<pair<string, const Unit*>> & h)
{
  const size_t n = h.size();
  FixedStack<const Unit*> ret(n); // this will be field first of return value
  FixedStack<Unit_Convert_Fct_Ptr> fcts(n); // and this will be the field second

  // just for avoiding time consumption in case where user has not set
  // any --property-unit flag, we first test if there are unit changes
  if (property_units_changes.size() == 0) 
    { // no unit change ==> return original units and null conversions
      h.for_each([&ret, &fcts] (auto & p) // build return value 
		 {
		   ret.insert(p.second); // original unit
		   fcts.insert(nullptr); // no conversion
		 });
      return make_pair(ret, fcts);
    }

  // traverse the header and for each column verify if there is an unit change
  for (auto it = h.get_it(); it.has_curr(); it.next())
    {
      const pair<string, const Unit*> & col = it.get_curr(); // header column
      const string & csv_col = col.first;
      const string property_name = split_to_list(csv_col, " ").get_first();
      auto p = property_units_changes.search(property_name);
      if (not p) // does property_name have an unit change?
	{ // No!
	  ret.insert(col.second);
	  fcts.insert(nullptr); // nullptr as sentinel for knowing if
				// conversion is needed
	  continue;
	}

      // here we have an unit change ==> we validate that changed unit
      // is sibling of original unit
      auto old_unit_ptr = col.second;
      auto new_unit_ptr = p->second;
      if (not old_unit_ptr->is_sibling(*new_unit_ptr))
	{
	  ostringstream s;
	  s << "In flag --" << property_unit.getName() << " \"" << property_name
	    << " " << new_unit_ptr->name << "\" : unit " << new_unit_ptr->name
	    << " is not sibling of " << old_unit_ptr->name;
	  ZENTHROW(InvalidValue, s.str());
	}
      ret.insert(p->second);
      fcts.insert(search_conversion(*old_unit_ptr, *new_unit_ptr));
      property_units_changes.remove(property_name);
    }

  if (not property_units_changes.is_empty())
    {
      ostringstream s;
      s << "For correlation unit change (flag --" << property_unit.getName()
	<< "): " << property_units_changes.get_first().first
	<< " is not a valid property name";
      ZENTHROW(InvalidValue, s.str());
    }

  return make_pair(ret, fcts);
}

// helper to handle parameter passing to correlations

// construct a VtlQuantity from a parameter by name
inline VtlQuantity par(const Correlation::NamedPar & par)
{
  return VtlQuantity(*get<3>(par), get<2>(par));
}

// construct a parameter by name name from an amount (VtlQuantity or
// Quantity <Unit>)
inline Correlation::NamedPar npar(const string & name, const BaseQuantity & p)
{
  return Correlation::NamedPar(true, name, p.raw(), &p.unit);
}

inline Correlation::NamedPar
npar(const string & name, double v, const Unit * unit)
{
  return Correlation::NamedPar(true, name, v, unit);
}

inline Correlation::NamedPar npar(const string & name,
				  const Correlation::NamedPar & par)
{
  return Correlation::NamedPar(true, name, get<2>(par), get<3>(par));
}

// macro that constructs a parameter by name with name par from a VtlQuantity
# define NPAR(par) npar(#par, par)

// Declare a command line argument for a double type.
//
// Parameters:
//
// - name: name of variable set to be declared
// - UnitName: type of unit
// - desc: string describing the parameter command line parameter
//
// The macro creates:
//
// - A variable ValueArg<double> with name name_arg
// - A variable Correlation::NamedPar with name name_par
// - A variable VtlQuantity with name name
# define Declare_Command_Line_Arg(name, UnitName, desc)		\
  ValueArg<double> name##_arg = { "", #name, #name, false, 0,	\
				  desc " in " #name, cmd };	\
  Correlation::NamedPar name##_par;				\
  VtlQuantity name;

// Declare a command line argument for a double type plus a validation
// function with name set_name(). The validation makes the parameter
// mandatory. That is, the routine aborts if the parameter was not
// specified on the command line
# define Command_Arg_Value(name, UnitName, desc)			\
  Declare_Command_Line_Arg(name, UnitName, desc);			\
  void set_##name()							\
  {									\
    if (not name##_arg.isSet())						\
      error_msg("mandatory parameter " #name " has not been set");	\
    auto name##_unit = test_par_unit_change(#name, UnitName::get_instance()); \
    name##_par = npar(#name, name##_arg.getValue(), name##_unit);	\
    name.set(par(name##_par));						\
  }

// Declare a command line argument for a double type plus a validation
// function with name set_name(). The validation does not make the
// parameter mandatory. That is, the routine does not abort if the
// parameter was not specified on the command line
# define Command_Arg_Optional_Value(name, UnitName, desc)		\
  Declare_Command_Line_Arg(name, UnitName, desc);			\
  void set_##name()							\
  {									\
    auto name##_unit = test_par_unit_change(#name, UnitName::get_instance()); \
    name##_par = npar(#name, name##_arg.getValue(), name##_unit);	\
    name.set(par(name##_par));						\
  }

Command_Arg_Value(api, Api, "api");
Command_Arg_Value(rsb, SCF_STB, "rsb");
Command_Arg_Value(yg, Sgg, "yg");
Command_Arg_Optional_Value(tsep, Fahrenheit, "separator temperature");
Command_Arg_Optional_Value(tsep2, Fahrenheit, "temperature of 2nd separator");

// return true if two separator temperatures were specified
inline bool two_separators()
{
  bool ret = tsep_arg.isSet() and tsep2_arg.isSet();
  if (ret and tsep < tsep2)
    ZENTHROW(WrongCombinationInputValues, "separator temp " + tsep.to_string() +
	     " is less than separator temp " + tsep2.to_string());
  return ret;
}

Command_Arg_Value(ogr, STB_MMscf, "Condensate gas ratio")
Command_Arg_Optional_Value(psep, psia, "separator pressure");
Command_Arg_Optional_Value(n2, MolePercent, "n2");
Command_Arg_Optional_Value(co2, MolePercent, "co2");
Command_Arg_Optional_Value(h2s, MolePercent, "h2s");
Command_Arg_Optional_Value(nacl, Molality_NaCl, "nacl");

// Initialize a correlation specified from the command line via
// corr_name_arg. Verify that the correlation is for the target_name
// property and if so, then the found correlation is placed in the
// output parameter corr_ptr.
//
// The mandatory parameter indicates that the correlation is required
// on the command line.
void set_correlation(ValueArg<string> & corr_name_arg,
		     const string & target_name, const Correlation *& corr_ptr,
		     bool mandatory)
{
  if (mandatory and not corr_name_arg.isSet())
    error_msg("Correlation for "+ target_name +
	      " has not been specified in command line (" +
	      corr_name_arg.longID() + ")");
  if (not corr_name_arg.isSet())
    return;
  const string & corr_name = corr_name_arg.getValue();
  corr_ptr = Correlation::search_by_name(corr_name);
  if (corr_ptr == nullptr)
    error_msg("Correlation for " + target_name + " " +
	      corr_name + " not found");
  if (corr_ptr->target_name() != target_name)
    error_msg("Correlation " + corr_ptr->name + " is not for " + target_name);
}

// Declare a command line argument for receiving the a correlation target_name.
//
// The target name is the agreed name for the physical property. For
// example, for the bubble point, the target name would be pb.
//
// The macro instantiates two variables:
//
// - target_name_corr_arg: the command line argument for the
//  correlation target name.
// - target_name_corr: a pointer to the correlation (it would be
//  validated by the set_target_name_corr() routine that is generated
//  by another macro 
# define Declare_Corr_Arg(target_name)				  \
  ValueArg<string> target_name##_corr_arg =			  \
    { "", #target_name, "correlation for " #target_name, false, "",	\
      "correlation for " #target_name, cmd };				\
 								  \
  const Correlation * target_name##_corr = nullptr;

// Declare a command line argument for receiving a correlation
// target_name plus the mandatory validation function
// set_target_name_corr(). After call to this routine, the
// target_name_corr pointer is set to the correlation specified in the
// command line
# define Command_Arg_Mandatory_Correlation(target_name)			\
  Declare_Corr_Arg(target_name);					\
  									\
  void set_##target_name##_corr()					\
  {									\
    set_correlation(target_name##_corr_arg, #target_name,		\
		    target_name##_corr, true);				\
  }

// Declare a command line argument for receiving a correlation
// target_name plus the validation function
// set_target_name_corr(). After call to this routine, the
// target_name_corr pointer is set to the correlation specified in the
// command line. If no correlation is specified in the command line,
// then the pointer is set to the default value CorrName
# define Command_Arg_Optional_Correlation(target_name, CorrName)	\
  Declare_Corr_Arg(target_name);					\
									\
  void set_##target_name##_corr()					\
  {									\
    target_name##_corr = &CorrName::get_instance();			\
    set_correlation(target_name##_corr_arg, #target_name,		\
		    target_name##_corr, false);				\
  }

// Declare a command line argument for a tuning parameter c for the
// target correlation name target_name. By default, if the argument is
// not input, then the value is 0.0
# define Declare_c_par(target_name)			\
  ValueArg<double> c_##target_name##_arg =			\
    { "", "c-" #target_name, #target_name " c", false, 0,	\
      #target_name " c", cmd };

// Declare a command line argument for a tuning parameter m for the
// target correlation name target_name. By default, if the argument is
// not input, then the value is 1.0
# define Declare_m_par(target_name)			\
  ValueArg<double> m_##target_name##_arg =		\
    { "", "m-" #target_name, #target_name " m", false, 1,	\
      #target_name " m", cmd };

// Defines a calibrated correlation along with its calibration
// parameters. The command line argument is mandatory
# define Command_Arg_Tuned_Correlation(target_name)	\
  Command_Arg_Mandatory_Correlation(target_name);	\
  Declare_c_par(target_name);				\
  Declare_m_par(target_name);

Command_Arg_Tuned_Correlation(pb);
Command_Arg_Tuned_Correlation(rs);
Command_Arg_Tuned_Correlation(bob);
Command_Arg_Tuned_Correlation(boa);
Command_Arg_Tuned_Correlation(uod);
Command_Arg_Tuned_Correlation(cob);
Command_Arg_Tuned_Correlation(coa);
Command_Arg_Tuned_Correlation(uob);
Command_Arg_Tuned_Correlation(uoa);

Command_Arg_Optional_Correlation(ppchc, PpchcStanding);
Command_Arg_Optional_Correlation(ppcm_mixing, PpcmKayMixingRule);
Command_Arg_Optional_Correlation(adjustedppcm, AdjustedppcmWichertAziz);
Command_Arg_Optional_Correlation(tpchc, TpchcStanding);
Command_Arg_Optional_Correlation(tpcm_mixing, TpcmKayMixingRule);
Command_Arg_Optional_Correlation(adjustedtpcm, AdjustedtpcmWichertAziz);
Command_Arg_Optional_Correlation(zfactor, ZfactorDranchukAK);
Command_Arg_Optional_Correlation(cg, CgMattarBA);
Command_Arg_Optional_Correlation(ug, UgCarrKB);
Command_Arg_Optional_Correlation(bwb, BwbSpiveyMN);
Command_Arg_Optional_Correlation(bwa, BwaSpiveyMN);
Command_Arg_Optional_Correlation(uw, UwMcCain);
Command_Arg_Optional_Correlation(pw, PwSpiveyMN);
Command_Arg_Optional_Correlation(cwb, CwbSpiveyMN);
Command_Arg_Optional_Correlation(cwa, CwaSpiveyMN);
Command_Arg_Optional_Correlation(rsw, RswSpiveyMN);
Command_Arg_Optional_Correlation(sgo, SgoBakerSwerdloff);
Command_Arg_Optional_Correlation(sgw, SgwJenningsNewman);

vector<string> grid_types =
  { "blackoil", "wetgas", "drygas", "brine", "gascondensate", "simple" };
ValuesConstraint<string> allowed_grid_types = grid_types;
ValueArg<string> grid = { "", "grid", "grid type", false,
			  "blackoil", &allowed_grid_types, cmd };

SwitchArg print_types = { "", "fluid-types", "print fluid types", cmd };

void print_fluid_types()
{
  assert(print_types.isSet() and print_types.getValue());

  for (auto & type : grid_types)
    cout << type << endl;
  exit(0);
}

// Command line range specification.
//
// To be used for the temperature and pressure
//
// Parameter has form --property "min max num-of-steps"
struct RangeDesc
{
  double min = 0, max = 0;
  size_t n = 1; // num of steps

  RangeDesc & operator = (const string & str)
  {
    istringstream iss(str);
    if (not (iss >> min >> max >> n))
      ZENTHROW(CommandLineError, str + " is not of form \"min max n\"");

    if (n == 0)
      ZENTHROW(CommandLineError, ::to_string(n) + " n cannot be zero");

    if (min > max)
      {
	ostringstream s;
	s << "min value " << min << " greater than max value " << max;
	ZENTHROW(CommandLineError, s.str());
      }

    return *this;
  }

  double step() const noexcept { return (max - min) / (n - 1); }

  friend ostream & operator << (ostream & os, const RangeDesc & d)
  {
    return os << d.min<< " " << d.max << " " << d.n;
  }
};

namespace TCLAP
{
  template<> struct ArgTraits<RangeDesc> { typedef StringLike ValueCategory; };
}

// Given a RangeDesc, put in the correlation parameters list l the
// range values Each value is a named correlation parameter; i.e. a
// tuple <true, name, value, unit>
size_t set_range(const RangeDesc & range, const string & name,
		 const Unit & unit, DynList<Correlation::NamedPar> & l)
{
  assert(l.is_empty());
  const auto & step = range.step();
  double val = range.min;
  for (size_t i = 0; i < range.n; ++i, val += step)
    l.append(make_tuple(true, name, val, &unit));

  return range.n;
}

struct RowDesc
{
  double t, p; // temperature and pressure

  RowDesc & operator = (const string & str)
  {
    istringstream iss(str);
    if (not (iss >> t >> p))
      ZENTHROW(CommandLineError, str + " is not of form \"t p\"");

    if (t <= 0 or p <= 0)
      ZENTHROW(CommandLineError, "t and p must be greater than zero");

    return *this;
  }

  friend ostream & operator << (ostream & os, const RowDesc & d)
  {
    return os << d.t<< " " << d.p;
  }
};

namespace TCLAP
{
  template<> struct ArgTraits<RowDesc> { typedef StringLike ValueCategory; };
}

MultiArg<RowDesc> row = { "", "tp_pair", "add a pair of t, p values",
			  false, "tp \"t p\"", cmd };

SwitchArg permute = { "", "permute", "permute tp pairs", cmd };

struct ArrayDesc
{
  DynList<double> values;

  ArrayDesc & operator = (const string & str)
  {
    string data;
    istringstream iss(str);

    while (iss >> data)
      {
	if (not is_double(data))
	  ZENTHROW(CommandLineError, data + " is not a double");

	values.append(atof(data));
      }

    if (values.is_empty())
      ZENTHROW(CommandLineError, "cannot read array");

    in_place_sort(values);
 
    return *this;
  }
};

namespace TCLAP
{
  template<> struct ArgTraits<ArrayDesc> { typedef StringLike ValueCategory; };
}

size_t set_array(const ArrayDesc & rowset, const string & name,
		 const Unit & unit, DynList<Correlation::NamedPar> & l)
{
  assert(l.is_empty() and not rowset.values.is_empty());
  const auto & values = rowset.values;
  for (auto it = values.get_it(); it.has_curr(); it.next())
    l.append(make_tuple(true, name, it.get_curr(), &unit));
  return values.size();
}

vector<string> sort_types = { "no_sort", "t", "p" };
ValuesConstraint<string> allowed_sort_types = sort_types;
ValueArg<string> sort_type = { "", "sort", "sorting type", false,
			       "no_sort", &allowed_sort_types, cmd };

// Declare a range command line parameter compound by
//
// - prefix: prefix for _range
// - name: name of property
// - UnitName
# define Command_Line_Range(prefix, name)				\
  ValueArg<RangeDesc> prefix##_range =					\
    { "", #prefix, "min max n", false, RangeDesc(),			\
      "range spec \"min max n\" for " #name, cmd };			\
									\
  ValueArg<ArrayDesc> prefix##_array =					\
    { "", #prefix "_array", #prefix " values", false, ArrayDesc(),	\
      #prefix " values", cmd };						\
									\
  DynList<Correlation::NamedPar> prefix##_values;			\
  size_t prefix##_num_items = 0;					\
									\
  const Unit * prefix##_unit = nullptr;					\
									\
  size_t set_##prefix##_range()					\
  {									\
    return set_range(prefix##_range.getValue(), #prefix,		\
		     *prefix##_unit, prefix##_values);			\
  }									\
									\
  size_t set_##prefix##_array()					\
  {									\
    return set_array(prefix##_array.getValue(), #prefix,		\
		     *prefix##_unit, prefix##_values);			\
  }

# define T_UNIT Fahrenheit
# define P_UNIT psia

Command_Line_Range(t, "temperature");
Command_Line_Range(p, "pressure");

SwitchArg transpose_par = { "", "transpose", "transpose grid", cmd };

using TPPair = pair<Correlation::NamedPar, Correlation::NamedPar>;

// This list is only used with --tp_pair option and --permute is not set
DynList<TPPair> tp_values;

void sort_by_t()
{
  in_place_sort(tp_values, [] (const TPPair & p1, const TPPair & p2)
		{
		  const double & t1 = get<2>(p1.first);
		  const double & t2 = get<2>(p2.first);
		  if (t1 != t2)
		    return t1 < t2;
		  const double & pr1 = get<2>(p1.second);
		  const double & pr2 = get<2>(p2.second);
		  return pr1 < pr2;
		});
}

void sort_by_p()
{
  in_place_sort(tp_values, [] (const TPPair & p1, const TPPair & p2)
		{
		  const double & pr1 = get<2>(p1.second);
		  const double & pr2 = get<2>(p2.second);
		  if (pr1 != pr2)
		    return pr1 < pr2;
		  const double & t1 = get<2>(p1.first);
		  const double & t2 = get<2>(p2.first);
		  return t1 < t2;
		});
}

const AHDispatcher<string, void (*)()> sort_dispatcher("no_sort", [] () {},
						       "t", sort_by_t,
						       "p", sort_by_p);

void set_ranges()
{
  // putting these two unit settings here makes this test unique for
  // both cases (--t + --p and --tp_pair)
  t_unit = test_par_unit_change("t", Fahrenheit::get_instance());
  p_unit = test_par_unit_change("p", psia::get_instance());

  bool t_values_set = false;
  bool p_values_set = false;

  if (t_range.isSet())
    {
      if (row.isSet())
	error_msg(t_range.getName() + " option cannot be used with " +
		  row.getName() + " option");
      if (t_array.isSet())
	error_msg(t_range.getName() + " option cannot be used with " +
		  t_array.getName() + " option");
      t_num_items = set_t_range();
      t_values_set = true;
    }

  if (t_array.isSet())
    {
      if (row.isSet())
	error_msg(t_array.getName() + " option cannot be used with " +
		  row.getName() + " option");
      t_num_items = set_t_array();
      t_values_set = true;
    }

  if (p_range.isSet())
    {
      if (row.isSet())
	error_msg(p_range.getName() + " option cannot be used with " +
		  row.getName() + " option");
      if (p_array.isSet())
	error_msg(p_range.getName() + " option cannot be used with " +
		  p_array.getName() + " option");
      p_num_items = set_p_range();
      p_values_set = true;
    }

  if (p_array.isSet())
    {
      if (row.isSet())
	error_msg(t_array.getName() + " option cannot be used with " +
		  row.getName() + " option");
      p_num_items = set_p_array();
      p_values_set = true;
    }

  if (t_values_set and p_values_set)
    return;

  if (t_values_set or p_values_set)
    error_msg("options specifying t or p array must be used together");

  auto & pairs = row.getValue();
  if (pairs.size() == 0)
    error_msg("option " + row.getName() + " is mandatory in absence of " +
	      t_range.getName() + " and " + p_range.getName() + " options");

  if (not permute.getValue())
    {
      for (auto & p : pairs)
	tp_values.append(pair<Correlation::NamedPar, Correlation::NamedPar>
			 (make_tuple(true, "t", p.t, t_unit),
			  make_tuple(true, "p", p.p, p_unit)));
      cout << "pairs" << endl;
      tp_values.for_each([] (auto & p)
        {
	  cout << get<2>(p.first) << ", " << get<2>(p.second) << endl;
	});

      sort_dispatcher.run(sort_type.getValue());

      return;
    }

  DynBinHeap<double> theap, pheap;
  for (auto & p : pairs)
    {
      theap.insert(p.t);
      pheap.insert(p.p);
    }

  t_num_items = theap.size();
  p_num_items = pheap.size();

  while (not theap.is_empty())
    t_values.append(make_tuple(true, "t", theap.get(), t_unit));

  while (not pheap.is_empty())
    p_values.append(make_tuple(true, "p", pheap.get(), p_unit));
}

DefinedCorrelation
define_correlation(const VtlQuantity & pb,
		   const Correlation * below_corr_ptr, double cb, double mb,
		   const Correlation * above_corr_ptr,
		   double ca = 0, double ma = 1)
{
  DefinedCorrelation ret("p", pb.unit);
  ret.add_tuned_correlation(below_corr_ptr, psia::get_instance().min(), pb,
			    cb, mb);
  ret.add_tuned_correlation(above_corr_ptr, pb.next(),
			    psia::get_instance().max(), ca, ma);
  return ret;
}

DefinedCorrelation
define_correlation(const VtlQuantity & pb,
		   const Correlation * below_corr_ptr, 
		   const Correlation * above_corr_ptr)
{
  DefinedCorrelation ret("p", pb.unit);
  ret.add_tuned_correlation(below_corr_ptr, psia::get_instance().min(), pb,
			    0, 1);
  ret.add_tuned_correlation(above_corr_ptr, pb.next(),
			    psia::get_instance().max(), 0, 1);
  return ret;
}

void
test_parameter(const DynList<pair<string, DynList<string>>> & required,
	       const Correlation::NamedPar & par, ParList & pars_list)
{
  if (required.exists([&par] (const auto & p) { return p.first == get<1>(par) or
      p.second.exists([&par] (const auto & s) { return s == get<1>(par); }); }))
    pars_list.insert(par);
}

const double Invalid_Value = Unit::Invalid_Value;

// global values only set during the grid generation and can be accessed by anyone
double temperature = 0, pressure = 0; 
bool exception_thrown = false;

// save exception e that was thrown during calculation of correlation corr_name
void store_exception(const string & corr_name, const exception & e)
{
  exception_thrown = true;
  ostringstream s;
  s << corr_name << ": " << temperature << " " << t_unit->name << ", "
    << pressure << " " << p_unit->name << ": " << e.what() << endl;
  exception_list.append(s.str());
}

/* Helper that meta-inserts par into pars_list but stops if any
   parameter is invalid.

   Returns true if all parameters were valid (!= Invalid_Value)

   Otherwise the insertion stops at the first invalid parameter, the
   parameters previously inserted in the list are deleted and false is
   returned
*/
inline bool insert_in_pars_list(ParList&) { return true; }

template <typename ... Args> inline
bool insert_in_pars_list(ParList & pars_list, 
			 const Correlation::NamedPar & par, Args & ... args)
{
  if (get<2>(par) == Invalid_Value)
    return false;
 
  pars_list.insert(par);
  if (insert_in_pars_list(pars_list, args...))
    return true;

  pars_list.remove(par); // If recursive insertion fails ==> remove pair

  return false;
}

/* CONVENTION ON THE NAMES OF WRAPPERS TO CALL THE CORRELATIONS

   - compute(corr_ptr, check, pars_list, args...): direct call to
   corr_ptr correlation

   - compute_exc(corr_ptr, check, pars_list, args...): direct call to
   corr_ptr correlation but aborts program if an exception is thrown

   - tcompute(corr_ptr, c, m, check, pars_list, args...): Call to
   corr_ptr correlation with tuning parameters c and m
  
   - dcompute(def_corr, check, p_q, pars_list, args...): call to defined
   correlation def_corr with pivot parameter p_q

   - CALL(corr_name, var, args...): this macro first declares a VtlQuantity
   var then assigns it the result of correlation call
   corr_name::get_instance().impl(args...);
*/
template <typename ... Args> inline
VtlQuantity tcompute(const Correlation * corr_ptr,
		     double c, double m, bool check,
		     ParList & pars_list, Args ... args)
{
  try
    {
      if (not insert_in_pars_list(pars_list, args...))
	return VtlQuantity();

      auto ret = corr_ptr->tuned_compute_by_names(pars_list, c, m, check);
      remove_from_container(pars_list, args...);
      return ret;
    }
  catch (UnitConversionNotFound) {}
  catch (exception & e)
    {
      if (report_exceptions)
	store_exception(corr_ptr->name, e);
   
      remove_from_container(pars_list, args ...);
    }
  return VtlQuantity();
}

template <typename ... Args> inline
VtlQuantity compute(const Correlation * corr_ptr, bool check,
		    ParList & pars_list, Args ... args)
{
  try
    {
      if (not insert_in_pars_list(pars_list, args...))
	return VtlQuantity();

      auto ret = corr_ptr->compute_by_names(pars_list, check);
      remove_from_container(pars_list, args...);
      return ret;
    }
  catch (UnitConversionNotFound) {}  
  catch (exception & e)
    {
      if (report_exceptions)
	store_exception(corr_ptr->name, e);
   
      remove_from_container(pars_list, args ...);
    }
  return VtlQuantity();
}

// return true if all args... are valid
inline bool valid_args() { return true; }
template <typename ... Args> inline
bool valid_args(const VtlQuantity & par, Args ... args)
{
  if (par.is_null())
    return false;
  return valid_args(args...);
} 

template <typename ... Args> inline
string correlation_call(const Correlation * corr_ptr, Args ... args)
{
  DynList<Correlation::NamedPar> pars;
  append_in_container(pars, args...);

  ostringstream s;
  s << corr_ptr->name << "(";
  for (auto it = pars.get_it(); it.has_curr(); it.next())
    {
      const auto & par = it.get_curr();
      s << get<1>(par) << " = " << get<2>(par) << " " << get<3>(par)->name;
      if (&par != &pars.get_last())
	s << ", ";
    }
  s << ")";
  return s.str();
}

template <typename ... Args> inline
VtlQuantity compute_exc(const Correlation * corr_ptr, bool check,
			ParList & pars_list, Args ... args)
{
  try
    {
      if (not insert_in_pars_list(pars_list, args...))
	return VtlQuantity();

      auto ret = corr_ptr->compute_by_names(pars_list, check);
      remove_from_container(pars_list, args...);
      return ret;
    }
  catch (UnitConversionNotFound) {}
  catch (exception & e)
    {
      remove_from_container(pars_list, args ...);
      cout << "ERROR initializing " << correlation_call(corr_ptr, args...)
	   << "@ " << e.what();
      abort();
    }
  return VtlQuantity();
}

// Macro for creating `var` variable with value returned by correlation corr_name
# define CALL(corr_name, var, args...)				\
  VtlQuantity var;						\
  try								\
    {								\
      if (valid_args(args))					\
	var = corr_name::get_instance().call(args);		\
    }								\
  catch (UnitConversionNotFound) {}				\
  catch (exception & e)						\
    {								\
      store_exception(corr_name::get_instance().name, e);	\
    }

inline bool
insert_in_pars_list(const DefinedCorrelation&, const VtlQuantity&, ParList&)
{
  return true;
}

template <typename ... Args> inline
bool insert_in_pars_list(const DefinedCorrelation & corr,
			 const VtlQuantity & p_q,
			 ParList & pars_list,
			 const Correlation::NamedPar & par,
			 Args & ... args)
{
  if (get<2>(par) == Invalid_Value)
    {
      const string & par_name = get<1>(par);   
      if (corr.search_parameters(p_q).contains(par_name))
	return false; // here the correlation would receive
      // Invalid_Value and would fail
    }
 
  pars_list.insert(par);
  if (insert_in_pars_list(corr, p_q, pars_list, args...))
    return true;

  pars_list.remove(par); // If recursive insertion fails ==> remove par

  return false;
}

template <typename ... Args> inline
VtlQuantity dcompute(const DefinedCorrelation & corr, bool check,
		     const VtlQuantity & p_q,
		     ParList & pars_list, Args ... args)
{
  if (not insert_in_pars_list(corr, p_q, pars_list, args...))
    return VtlQuantity();
 
  try
    {
      auto ret = corr.compute_by_names(pars_list, check);
      remove_from_container(pars_list, args ...);
      return ret;
    }
  catch (UnitConversionNotFound) {}
  catch (exception & e)
    {
      if (report_exceptions)
	{
	  auto triggering_corr_ptr = corr.search_correlation(p_q);
	  string names = corr.correlations().
	    foldl<string>("", [triggering_corr_ptr] (auto & acu, auto ptr)
			  {
			    if (triggering_corr_ptr == ptr)
			      return acu + "*" + ptr->name + " ";
			    return acu + ptr->name + " ";
			  });
	  store_exception("{ " + names + "}", e);
	}

      remove_from_container(pars_list, args ...);
    }
  return VtlQuantity();
}

template <typename ... Args> inline
VtlQuantity tcompute(const Correlation * corr_ptr,
		     double c, double m, bool check, Args ... args)
{ // static for creating it once and thus to gain time. But beware!
  // The function is not reentrant and can not be used in a
  // multithreaded environment
  static ParList pars_list;
  return tcompute(corr_ptr, c, m, check, pars_list, args...);
}

template <typename ... Args> inline
VtlQuantity compute(const Correlation * corr_ptr, bool check, Args ... args)
{ // static for creating it once and thus to gain time. But beware!
  // The function is not reentrant and can not be used in a
  // multithreaded environment
  static ParList pars_list;
  return compute(corr_ptr, check, pars_list, args...);
}

template <typename ... Args> inline
VtlQuantity compute_exc(const Correlation * corr_ptr, bool check, Args ... args)
{ // static for creating it once and thus to gain time. But beware!
  // The function is not reentrant and can not be used in a
  // multithreaded environment
  static ParList pars_list;
  return compute_exc(corr_ptr, check, pars_list, args...);
}

template <typename ... Args> inline
VtlQuantity dcompute(const DefinedCorrelation & corr, bool check,
		     const VtlQuantity & p_q, Args ... args)
{ // static for creating it once and thus to gain time. But beware!
  // The function is not reentrant and can not be used in a
  // multithreaded environment
  static ParList pars_list;
  return dcompute(corr, check, pars_list, p_q, args...);
}

/// Returns the list of parameters required by the set of correlations
/// that are in l 
ParList load_constant_parameters(const DynList<const Correlation*> & l)
{
  ParList pars_list;
  auto required_pars = DefinedCorrelation::parameter_list(l);
				  
  test_parameter(required_pars, api_par, pars_list);
  test_parameter(required_pars, rsb_par, pars_list);
  test_parameter(required_pars, yg_par, pars_list);
  test_parameter(required_pars, tsep_par, pars_list);
  test_parameter(required_pars, psep_par, pars_list);
  test_parameter(required_pars, n2_par, pars_list);
  test_parameter(required_pars, co2_par, pars_list);
  test_parameter(required_pars, h2s_par, pars_list);
  test_parameter(required_pars, nacl_par, pars_list);

  return pars_list;
}

void insert_in_row(FixedStack<const VtlQuantity*> &, size_t&) {}

template <class ... Args>
void insert_in_row(FixedStack<const VtlQuantity*> & row, size_t & n,
		   const VtlQuantity & q, Args & ... args)
{
  row.insert(&q);
  ++n;
  insert_in_row(row, n, args...);
}

template <class ... Args>
size_t insert_in_row(FixedStack<const VtlQuantity*> & row,
		     const VtlQuantity & q, Args & ... args)
{
  size_t n = 0;
  insert_in_row(row, n, q, args...);
  return n;
}

using Row = pair<Array<string>, Array<double>>;

bool transposed = false;
Array<Row> rows; // only used if transposed is set

inline void buffer_row(const FixedStack<const VtlQuantity*> & row,
		       const FixedStack<Unit_Convert_Fct_Ptr> & row_convert)
{
  const size_t n = row.size();
  const VtlQuantity ** ptr = &row.base();

  Row p;
  if (exception_thrown)
    {
      p.first.append("\"true\"");
      exception_thrown = false;
    }
  else
    p.first.append("\"false\"");

  const Unit_Convert_Fct_Ptr * tgt_unit_ptr = &row_convert.base();
  for (long i = n - 1; i >= 0; --i)
    {
      Unit_Convert_Fct_Ptr convert_fct = tgt_unit_ptr[i];
      const VtlQuantity & q = *ptr[i];
      if (not q.is_null())
	p.second.append(convert_fct ? convert_fct(q.raw()) : q.raw());
      else
	p.second.append(Invalid_Value);
    }

  rows.append(move(p));
}

inline void buffer_row_pb(const FixedStack<const VtlQuantity*> & row,
			  const FixedStack<Unit_Convert_Fct_Ptr> & row_convert,
			  bool is_pb)
{
  Row p;
  p.first.append(is_pb ? "\"true\"" : "\"false\"");

  const size_t n = row.size();
  const VtlQuantity ** ptr = &row.base();

  if (exception_thrown)
    {
      p.first.append("\"true\"");
      exception_thrown = false;
    }
  else
    p.first.append("\"false\"");

  const Unit_Convert_Fct_Ptr * tgt_unit_ptr = &row_convert.base();
  for (long i = n - 1; i >= 0; --i)
    {
      Unit_Convert_Fct_Ptr convert_fct = tgt_unit_ptr[i];
      const VtlQuantity & q = *ptr[i];
      if (not q.is_null())
	p.second.append(convert_fct ? convert_fct(q.raw()) : q.raw());
      else
	p.second.append(Invalid_Value);
    }

  rows.append(move(p));
}

inline void process_row(const FixedStack<const VtlQuantity*> & row,
			const FixedStack<Unit_Convert_Fct_Ptr> & row_convert)
{
  const size_t n = row.size();
  const VtlQuantity ** ptr = &row.base();

  if (exception_thrown)
    {
      printf("\"true\",");
      exception_thrown = false;
    }
  else
    printf("\"false\",");

  const Unit_Convert_Fct_Ptr * tgt_unit_ptr = &row_convert.base();
  for (long i = n - 1; i >= 0; --i)
    {
      Unit_Convert_Fct_Ptr convert_fct = tgt_unit_ptr[i];
      const VtlQuantity & q = *ptr[i];
      if (not q.is_null())
	printf("%f", convert_fct ? convert_fct(q.raw()) : q.raw());

      // Comment line above and uncomment below in order to get maximum precision
      //printf("%.17g", convert_fct ? convert_fct(q.raw()) : q.raw());

      if (i > 0)
	printf(",");
    }
  printf("\n");
}

inline void process_row_pb(const FixedStack<const VtlQuantity*> & row,
			   const FixedStack<Unit_Convert_Fct_Ptr> & row_convert,
			   bool is_pb)
{
  printf(is_pb ? "\"true\"," : "\"false\",");
  process_row(row, row_convert);
}

using RowFctPb = void (*)(const FixedStack<const VtlQuantity*>&,
			  const FixedStack<Unit_Convert_Fct_Ptr>&, bool);

inline void no_row_pb(const FixedStack<const VtlQuantity*>&,
		      const FixedStack<Unit_Convert_Fct_Ptr>&, bool) {}

RowFctPb row_fct_pb = nullptr;

using RowFct = void (*)(const FixedStack<const VtlQuantity*>&,
			const FixedStack<Unit_Convert_Fct_Ptr>&);

inline void no_row(const FixedStack<const VtlQuantity*>&,
		   const FixedStack<Unit_Convert_Fct_Ptr>&) {}

RowFct row_fct = nullptr;

Array<string> col_names;

// Print out the csv header according to passed args and return a
// stack of definitive units for each column. Also it sets row_fct
template <typename ... Args>
FixedStack<Unit_Convert_Fct_Ptr> print_csv_header(Args ... args)
{
  FixedStack<pair<string, const Unit*>> header;

  insert_in_container(header, args...);

  auto ret = build_stack_of_property_units(header);

  if (report_exceptions)
    {
      row_fct_pb = &no_row_pb;
      row_fct = &no_row;
      return ret.second;
    }

  const size_t n = header.size();
  pair<string, const Unit*> * col_ptr = &header.base();

  const Unit ** final_units = &ret.first.base();

  if (transposed)
    {
      rows.reserve(t_num_items*p_num_items + 10);
      for (long i = n - 1; i >= 0; --i)
	{
	  const pair<string, const Unit*> & val = col_ptr[i];
	  col_names.append(val.first + " " + final_units[i]->name);
	}
      row_fct_pb = &buffer_row_pb;
      row_fct = &buffer_row;
    }
  else
    {
      for (long i = n - 1; i >= 0; --i)
	{
	  const pair<string, const Unit*> & val = col_ptr[i];
	  printf("%s %s", val.first.c_str(), final_units[i]->name.c_str());
	  if (i > 0)
	    printf(",");
	}
      printf("\n");
      row_fct_pb = &process_row_pb;
      row_fct = &process_row;
    }

  return ret.second;
}

void print_transpose()
{
  assert(transposed);

  const size_t nrow = rows.size();
  const size_t str_ncol = rows(0).first.size();
  for (size_t j = 0; j < str_ncol; ++j)
    {
      printf("%s,", col_names(j).c_str());
      for (size_t i = 0; i < nrow; ++i)
	if (i != nrow - 1)	
	  printf("%s,", rows(i).first(j).c_str());
	else
	  printf(rows(i).first(j).c_str());
      printf("\n");
    }

  const size_t val_ncol = rows(0).second.size();
  for (size_t j = 0; j < val_ncol; ++j)
    {
      printf("%s,", col_names(j + str_ncol).c_str());
      for (size_t i = 0; i < nrow; ++i)
	{
	  const double & val = rows(i).second(j);
	  if (val != Invalid_Value)
	     if (i != nrow - 1)
	       printf("%f,", rows(i).second(j));
	     else
	       printf("%f", rows(i).second(j));
	  else if (i != nrow - 1)
	    printf(",");
	}
      printf("\n");
    }
}

void print_notranspose()
{
  const size_t nrow = rows.size();
  const size_t str_ncol = rows(0).first.size();
  for (size_t j = 0; j < str_ncol; ++j)
    {
      printf("%s,", col_names(j).c_str());
      for (size_t i = 0; i < nrow; ++i)
	if (i != nrow - 1)	
	  printf("%s,", rows(i).first(j).c_str());
	else
	  printf(rows(i).first(j).c_str());
      printf("\n");
    }

  const size_t val_ncol = rows(0).second.size();
  for (size_t j = str_ncol; j < str_ncol + val_ncol; ++j)
    {
      printf("%s,", col_names(j).c_str());
      for (size_t i = 0; i < nrow; ++i)
	if (i != nrow - 1)
	  printf("%f,", rows(i).second(j));
	else
	  printf("%f", rows(i).second(j));
      printf("\n");
    }
}

# define Blackoil_Init()						\
  set_api(); /* Initialization of constant data */			\
  set_rsb();								\
  set_yg();								\
  set_tsep();								\
  set_psep();								\
  set_h2s();								\
  set_co2();								\
  set_n2();								\
  set_nacl();								\
									\
  set_pb_corr(); /* Initialization of correlations */			\
  set_rs_corr();							\
  set_bob_corr();							\
  set_boa_corr();							\
  set_uod_corr();							\
  set_cob_corr();							\
  set_coa_corr();							\
  set_uob_corr();							\
  set_uoa_corr();							\
  set_ppchc_corr();							\
  set_tpchc_corr();							\
  set_ppcm_mixing_corr();						\
  set_tpcm_mixing_corr();						\
  set_adjustedppcm_corr();						\
  set_adjustedtpcm_corr();						\
  set_zfactor_corr();							\
  set_cg_corr();							\
  set_ug_corr();							\
  set_bwb_corr();							\
  set_bwa_corr();							\
  set_uw_corr();							\
  set_pw_corr();							\
  set_rsw_corr();							\
  set_cwb_corr();							\
  set_cwa_corr();							\
  set_sgo_corr();							\
  set_sgw_corr();							\
									\
  /* Calculation of constants for Z */					\
  auto yghc = compute_exc(YghcWichertAziz::correlation(), true, NPAR(yg), \
			  NPAR(n2), NPAR(co2), NPAR(h2s));		\
  auto ppchc = compute_exc(ppchc_corr, true, NPAR(yghc),		\
			   NPAR(n2), NPAR(co2), NPAR(h2s));		\
  auto ppcm = compute_exc(ppcm_mixing_corr, true, NPAR(ppchc),		\
			  NPAR(n2), NPAR(co2), NPAR(h2s));		\
  auto tpchc = tpchc_corr->compute(check, yghc);			\
  auto tpcm = compute_exc(tpcm_mixing_corr, true, NPAR(tpchc),		\
			  NPAR(n2), NPAR(co2), NPAR(h2s));		\
  auto adjustedppcm = compute_exc(adjustedppcm_corr, true, NPAR(ppcm),	\
				  NPAR(tpcm), NPAR(co2), NPAR(h2s));	\
  auto adjustedtpcm = compute_exc(adjustedtpcm_corr, true, NPAR(tpcm),	\
				  NPAR(co2), NPAR(h2s));		\
  /* End calculation constants for z */					\
									\
  /* Initialization of correlation parameter lists */			\
  auto pb_pars = load_constant_parameters({pb_corr});			\
  auto rs_pars = load_constant_parameters({rs_corr,			\
	&RsAbovePb::get_instance()});					\
  auto uod_pars = load_constant_parameters({uod_corr});			\
  auto bo_pars = load_constant_parameters({bob_corr, boa_corr});	\
  auto co_pars = load_constant_parameters({cob_corr, coa_corr});	\
  auto uo_pars = load_constant_parameters({uob_corr, uoa_corr});	\
  auto po_pars = load_constant_parameters({&PobBradley::get_instance(),	\
	&PoaBradley::get_instance()});					\
  auto ug_pars = load_constant_parameters({ug_corr});			\
  insert_in_container(ug_pars, npar("tpc", adjustedtpcm),		\
		      npar("ppc", adjustedppcm));			\
  auto bw_pars = load_constant_parameters({bwb_corr, bwa_corr});	\
  auto uw_pars = load_constant_parameters({uw_corr});			\
  auto pw_pars = load_constant_parameters({pw_corr});			\
  auto rsw_pars = load_constant_parameters({rsw_corr});			\
  auto cw_pars = load_constant_parameters({cwb_corr, cwa_corr});	\
  auto cwa_pars = load_constant_parameters({cwa_corr});			\
  ParList cg_pars; cg_pars.insert(npar("ppc", ppcm));			\
  auto sgo_pars = load_constant_parameters({sgo_corr});			\
  ParList sgw_pars;							\
									\
  using P = pair<string, const Unit*>;					\
  auto row_units =							\
	     print_csv_header(P("t", t_unit),				\
			      P("pb", &pb_corr->unit),			\
			      P("uod", &uod_corr->unit),		\
			      P("p", p_unit),				\
			      P("rs", &::rs_corr->unit),		\
			      P("co", &cob_corr->unit),			\
			      P("bo", &bob_corr->unit),			\
			      P("uo", &uob_corr->unit),			\
			      P("po", &PobBradley::get_instance().unit), \
			      P("zfactor", &Zfactor::get_instance()),	\
			      P("cg", &cg_corr->unit),			\
			      P("bg", &Bg::get_instance().unit),	\
			      P("ug", &ug_corr->unit),			\
			      P("pg", &Pg::get_instance().unit),	\
			      P("bw", &bwb_corr->unit),			\
			      P("uw", &uw_corr->unit),			\
			      P("pw", &pw_corr->unit),			\
			      P("rsw", &rsw_corr->unit),		\
			      P("cw", &cwb_corr->unit),			\
			      P("sgo", &sgo_corr->unit),		\
			      P("sgw", &sgw_corr->unit),		\
			      P("exception", &Unit::null_unit),		\
			      P("pbrow", &Unit::null_unit));		\
									\
  auto rs_pb = npar("rs", rsb_par);					\
									\
  /* Here are the values. Ensure that the insertion order is the same*/	\
  /* as for the csv header temperature loop */				\
									\
  FixedStack<const VtlQuantity*> row(25); 

# define Blackoil_Temperature_Calculations()				\
  VtlQuantity t_q = par(t_par);						\
  temperature = t_q.raw();						\
  CALL(Tpr, tpr, t_q, adjustedtpcm);					\
  auto tpr_par = NPAR(tpr);						\
									\
  VtlQuantity pb_q =							\
    tcompute(pb_corr, c_pb_arg.getValue(), 1, check, pb_pars, t_par);   \
  auto pb_par = npar("pb", pb_q);					\
  auto p_pb = npar("p", pb_q);						\
									\
  auto uod_val = compute(uod_corr, check, uod_pars, t_par, pb_par);	\
									\
  insert_in_container(rs_pars, t_par, pb_par);				\
  auto rs_corr = define_correlation(pb_q, ::rs_corr, c_rs_arg.getValue(), \
				      m_rs_arg.getValue(),		\
				      &RsAbovePb::get_instance());	\
									\
  insert_in_container(co_pars, t_par, pb_par);				\
  auto co_corr =							\
    define_correlation(pb_q,	\
		       cob_corr, c_cob_arg.getValue(), m_cob_arg.getValue(), \
		       coa_corr, c_coa_arg.getValue(), m_coa_arg.getValue()); \
  auto bo_corr =							\
    define_correlation(pb_q,	\
		       bob_corr, c_bob_arg.getValue(), m_bob_arg.getValue(), \
		       boa_corr, c_boa_arg.getValue(), m_boa_arg.getValue()); \
									\
  insert_in_container(uo_pars, t_par, pb_par, npar("uod", uod_val));	\
  auto uo_corr =							\
    define_correlation(pb_q,	\
		       uob_corr, c_uob_arg.getValue(), m_uob_arg.getValue(), \
		       uoa_corr, c_uoa_arg.getValue(), m_uoa_arg.getValue()); \
  									\
  auto po_corr = define_correlation(pb_q, &PobBradley::get_instance(),	\
				    &PoaBradley::get_instance());	\
									\
  auto bw_corr = define_correlation(pb_q, bwb_corr, bwa_corr);		\
									\
  auto cw_corr = define_correlation(pb_q, cwb_corr, cwa_corr);		\
									\
  bo_pars.insert(t_par);						\
  auto bobp = tcompute(bob_corr, c_bob_arg.getValue(), m_bob_arg.getValue(), \
		       check, bo_pars, p_pb, rs_pb);			\
									\
  auto uobp = tcompute(uob_corr, c_uob_arg.getValue(), m_uob_arg.getValue(), \
		       check, uo_pars, p_pb, rs_pb);			\
									\
  insert_in_container(bo_pars, pb_par, NPAR(bobp));			\
									\
  uo_pars.insert("uobp", uobp.raw(), &uobp.unit);			\
									\
  auto pobp = compute(&PobBradley::get_instance(), check, po_pars,	\
		      rs_pb, npar("bob", bobp));			\
									\
  auto bwbp = compute(bwb_corr, check, bw_pars, t_par, npar("p", pb_q)); \
									\
  insert_in_container(po_pars, pb_par, NPAR(pobp));			\
  insert_in_container(ug_pars, t_par, tpr_par);				\
  insert_in_container(bw_pars, t_par, pb_par, NPAR(bwbp));		\
  cg_pars.insert(tpr_par);						\
  uw_pars.insert(t_par);						\
  pw_pars.insert(t_par);						\
  rsw_pars.insert(t_par);						\
  cw_pars.insert(t_par);						\
  cwa_pars.insert(t_par);						\
  sgo_pars.insert(t_par);						\
  sgw_pars.insert(t_par);						\
									\
  size_t n = insert_in_row(row, t_q, pb_q, uod_val);

# define Blackoil_Pressure_Calculations()				\
  pressure = p_q.raw();							\
  CALL(Ppr, ppr, p_q, adjustedppcm);					\
  auto ppr_par = NPAR(ppr);						\
  auto rs = dcompute(rs_corr, check, p_q, rs_pars, p_par);		\
  rs = min(rs, rsb);							\
  auto rs_par = NPAR(rs);						\
  auto coa = dcompute(co_corr, check, p_q, co_pars, p_par);		\
  auto coa_par = NPAR(coa);						\
  auto bo = dcompute(bo_corr, check, p_q, bo_pars, p_par, rs_par, coa_par); \
  auto uo = dcompute(uo_corr, check, p_q, uo_pars, p_par, rs_par);	\
  auto po = dcompute(po_corr, check, p_q, po_pars, p_par, rs_par, coa_par, \
		     npar("bob", bo));					\
  VtlQuantity z;							\
  if (p_q <= pb_q)							\
    z = compute(zfactor_corr, check, ppr_par, tpr_par);			\
  auto z_par = NPAR(z);							\
  auto cg = compute(cg_corr, check, cg_pars, ppr_par, z_par);		\
  CALL(Bg, bg, t_q, p_q, z);						\
  auto ug = compute(ug_corr, check, ug_pars, p_par, ppr_par, z_par);	\
  CALL(Pg, pg, yg, t_q, p_q, z);					\
  auto rsw = compute(rsw_corr, check, rsw_pars, p_par);			\
  auto rsw_par = NPAR(rsw);						\
  auto cwa = compute(cwa_corr, check, cwa_pars, p_par, rsw_par);	\
  auto bw = dcompute(bw_corr, check, p_q, bw_pars, p_par, NPAR(cwa));	\
  auto bw_par = NPAR(bw);						\
  auto pw = compute(pw_corr, check, pw_pars, p_par, bw_par);		\
  auto cw = dcompute(cw_corr, check, p_q, cw_pars, p_par, z_par,	\
		     NPAR(bg), rsw_par, bw_par, NPAR(cwa));		\
  CALL(PpwSpiveyMN, ppw, t_q, p_q);					\
  auto uw = compute(uw_corr, check, uw_pars, p_par, NPAR(ppw));		\
  auto sgo = compute(sgo_corr, check, sgo_pars, p_par);			\
  auto sgw = compute(sgw_corr, check, sgw_pars, p_par);			\
									\
  size_t n = insert_in_row(row, p_q, rs, coa, bo, uo, po, z, cg, bg,	\
			   ug, pg, bw, uw, pw, rsw, cw, sgo, sgw);	\
									\
  assert(row.size() == 21);

# define Blackoil_Pop_Temperature_Parameters()			\
  row.popn(n);							\
  remove_from_container(rs_pars, "pb", t_par);			\
  remove_from_container(co_pars, "pb", t_par);			\
  remove_from_container(bo_pars, "bobp", "pb", t_par);		\
  remove_from_container(uo_pars, "uobp", "pb", "uod", t_par);	\
  remove_from_container(po_pars, "pb", "pobp");			\
  remove_from_container(ug_pars, t_par, tpr_par);		\
  remove_from_container(bw_pars, t_par, pb_par, "bwbp");	\
  sgo_pars.remove(t_par);					\
  sgw_pars.remove(t_par);					\
  cg_pars.remove(tpr_par);					\
  uw_pars.remove(t_par);					\
  pw_pars.remove(t_par);					\
  cw_pars.remove(t_par);					\
  rsw_pars.remove(t_par);

void generate_grid_blackoil()
{
  Blackoil_Init();

  for (auto t_it = t_values.get_it(); t_it.has_curr(); t_it.next()) 
    {
      Correlation::NamedPar t_par = t_it.get_curr();
      Blackoil_Temperature_Calculations();
      auto pb = pb_q.raw();
      double next_pb = nextafter(pb, numeric_limits<double>::max());
      VtlQuantity next_pb_q = { pb_q.unit, next_pb };

      auto first_p_point = p_values.get_first();
      bool first_p_above_pb = VtlQuantity(*get<3>(first_p_point),
					  get<2>(first_p_point)) > pb_q; 

      size_t i = 0;
      for (auto p_it = p_values.get_it(); p_it.has_curr(); ) // pressure loop
	{
	  Correlation::NamedPar p_par = p_it.get_curr();
	  VtlQuantity p_q = par(p_par);

	  bool pb_row = false; /* true if this line concerns to bubble point */	

	  /* WARNING: these predicates must be evaluated exactly in
	     this order */
	  if (p_q <= pb_q or (not (i < 2)) or first_p_above_pb)
	    p_it.next();
	  else
	    {
	      pb_row = true;
	      p_par = npar("p", ++i == 1 ? pb_q : next_pb_q);
	      p_q = par(p_par);
	      assert(i <= 2);
	    }		

	  Blackoil_Pressure_Calculations();
	  row_fct_pb(row, row_units, pb_row);
	  row.popn(n);
	}
      Blackoil_Pop_Temperature_Parameters();
    }
}

void generate_rows_blackoil()
{
  assert(not tp_values.is_empty());
 
  Blackoil_Init();
  for (auto it = tp_values.get_it(); it.has_curr(); it.next())
    {
      auto & curr = it.get_curr();
      Correlation::NamedPar t_par = curr.first;
      Correlation::NamedPar p_par = curr.second;
      Blackoil_Temperature_Calculations();

      VtlQuantity p_q = par(p_par);
      {
	Blackoil_Pressure_Calculations();
	row_fct_pb(row, row_units, false);
	row.popn(n);
      }
      Blackoil_Pop_Temperature_Parameters();
    }
}

# define Simple_Init()						\
  set_api(); /* Initialization of constant data */			\
  set_rsb();								\
  set_yg();								\
  set_tsep();								\
  set_psep();								\
  set_h2s();								\
  set_co2();								\
  set_n2();								\
  set_nacl();								\
									\
  set_pb_corr(); /* Initialization of correlations */			\
  set_rs_corr();							\
  set_bob_corr();							\
  set_boa_corr();							\
  set_uod_corr();							\
  set_cob_corr();							\
  set_coa_corr();							\
  set_uob_corr();							\
  set_uoa_corr();							\
  set_ppchc_corr();							\
  set_tpchc_corr();							\
  set_ppcm_mixing_corr();						\
  set_tpcm_mixing_corr();						\
  set_adjustedppcm_corr();						\
  set_adjustedtpcm_corr();						\
  set_zfactor_corr();							\
									\
  /* Calculation of constants for Z */					\
  auto yghc = compute_exc(YghcWichertAziz::correlation(), true, NPAR(yg), \
			  NPAR(n2), NPAR(co2), NPAR(h2s));		\
  auto ppchc = compute_exc(ppchc_corr, true, NPAR(yghc),		\
			   NPAR(n2), NPAR(co2), NPAR(h2s));		\
  auto ppcm = compute_exc(ppcm_mixing_corr, true, NPAR(ppchc),		\
			  NPAR(n2), NPAR(co2), NPAR(h2s));		\
  auto tpchc = tpchc_corr->compute(check, yghc);			\
  auto tpcm = compute_exc(tpcm_mixing_corr, true, NPAR(tpchc),		\
			  NPAR(n2), NPAR(co2), NPAR(h2s));		\
  auto adjustedppcm = compute_exc(adjustedppcm_corr, true, NPAR(ppcm),	\
				  NPAR(tpcm), NPAR(co2), NPAR(h2s));	\
  auto adjustedtpcm = compute_exc(adjustedtpcm_corr, true, NPAR(tpcm),	\
				  NPAR(co2), NPAR(h2s));		\
  /* End calculation constants for z */					\
									\
  /* Initialization of correlation parameter lists */			\
  auto pb_pars = load_constant_parameters({pb_corr});			\
  auto rs_pars = load_constant_parameters({rs_corr,			\
	&RsAbovePb::get_instance()});					\
  auto uod_pars = load_constant_parameters({uod_corr});			\
  auto bo_pars = load_constant_parameters({bob_corr, boa_corr});	\
  auto co_pars = load_constant_parameters({cob_corr, coa_corr});	\
  auto uo_pars = load_constant_parameters({uob_corr, uoa_corr});	\
									\
  using P = pair<string, const Unit*>;					\
  auto row_units =							\
	     print_csv_header(P("t", t_unit),				\
			      P("pb", &pb_corr->unit),			\
			      P("uod", &uod_corr->unit),		\
			      P("p", p_unit),				\
			      P("rs", &::rs_corr->unit),		\
			      P("co", &cob_corr->unit),			\
			      P("bo", &bob_corr->unit),			\
			      P("uo", &uob_corr->unit),			\
			      P("zfactor", &Zfactor::get_instance()),	\
			      P("exception", &Unit::null_unit),		\
			      P("pbrow", &Unit::null_unit));		\
									\
  auto rs_pb = npar("rs", rsb_par);					\
									\
  /* Here are the values. Ensure that the insertion order is the same*/	\
  /* as for the csv header temperature loop */				\
									\
  FixedStack<const VtlQuantity*> row(25); 

# define Simple_Temperature_Calculations()				\
  VtlQuantity t_q = par(t_par);						\
  temperature = t_q.raw();						\
  CALL(Tpr, tpr, t_q, adjustedtpcm);					\
  auto tpr_par = NPAR(tpr);						\
									\
  VtlQuantity pb_q =							\
    tcompute(pb_corr, c_pb_arg.getValue(), 1, check, pb_pars, t_par);   \
  auto pb_par = npar("pb", pb_q);					\
  auto p_pb = npar("p", pb_q);						\
									\
  auto uod_val = compute(uod_corr, check, uod_pars, t_par, pb_par);	\
									\
  insert_in_container(rs_pars, t_par, pb_par);				\
  auto rs_corr = define_correlation(pb_q, ::rs_corr, c_rs_arg.getValue(), \
				      m_rs_arg.getValue(),		\
				      &RsAbovePb::get_instance());	\
									\
  insert_in_container(co_pars, t_par, pb_par);				\
  auto co_corr =							\
    define_correlation(pb_q,	\
		       cob_corr, c_cob_arg.getValue(), m_cob_arg.getValue(), \
		       coa_corr, c_coa_arg.getValue(), m_coa_arg.getValue()); \
  auto bo_corr =							\
    define_correlation(pb_q,	\
		       bob_corr, c_bob_arg.getValue(), m_bob_arg.getValue(), \
		       boa_corr, c_boa_arg.getValue(), m_boa_arg.getValue()); \
									\
  insert_in_container(uo_pars, t_par, pb_par, npar("uod", uod_val));	\
  auto uo_corr =							\
    define_correlation(pb_q,	\
		       uob_corr, c_uob_arg.getValue(), m_uob_arg.getValue(), \
		       uoa_corr, c_uoa_arg.getValue(), m_uoa_arg.getValue()); \
  									\
  bo_pars.insert(t_par);						\
  auto bobp = tcompute(bob_corr, c_bob_arg.getValue(), m_bob_arg.getValue(), \
		       check, bo_pars, p_pb, rs_pb);			\
									\
  auto uobp = tcompute(uob_corr, c_uob_arg.getValue(), m_uob_arg.getValue(), \
		       check, uo_pars, p_pb, rs_pb);			\
									\
  insert_in_container(bo_pars, pb_par, NPAR(bobp));			\
									\
  uo_pars.insert("uobp", uobp.raw(), &uobp.unit);			\
									\
  size_t n = insert_in_row(row, t_q, pb_q, uod_val);

# define Simple_Pressure_Calculations()				\
  pressure = p_q.raw();							\
  CALL(Ppr, ppr, p_q, adjustedppcm);					\
  auto ppr_par = NPAR(ppr);						\
  auto rs = dcompute(rs_corr, check, p_q, rs_pars, p_par);		\
  rs = min(rs, rsb);							\
  auto rs_par = NPAR(rs);						\
  auto coa = dcompute(co_corr, check, p_q, co_pars, p_par);		\
  auto coa_par = NPAR(coa);						\
  auto bo = dcompute(bo_corr, check, p_q, bo_pars, p_par, rs_par, coa_par); \
  auto uo = dcompute(uo_corr, check, p_q, uo_pars, p_par, rs_par);	\
  VtlQuantity z;							\
  if (p_q <= pb_q)							\
    z = compute(zfactor_corr, check, ppr_par, tpr_par);			\
  auto z_par = NPAR(z);							\
									\
  size_t n = insert_in_row(row, p_q, rs, coa, bo, uo, z);

# define Simple_Pop_Temperature_Parameters()			\
  row.popn(n);							\
  remove_from_container(rs_pars, "pb", t_par);			\
  remove_from_container(co_pars, "pb", t_par);			\
  remove_from_container(bo_pars, "bobp", "pb", t_par);		\
  remove_from_container(uo_pars, "uobp", "pb", "uod", t_par);

void generate_grid_simple()
{
  Simple_Init();

  for (auto t_it = t_values.get_it(); t_it.has_curr(); t_it.next()) 
    {
      Correlation::NamedPar t_par = t_it.get_curr();
      Simple_Temperature_Calculations();
      auto pb = pb_q.raw();
      double next_pb = nextafter(pb, numeric_limits<double>::max());
      VtlQuantity next_pb_q = { pb_q.unit, next_pb };

      auto first_p_point = p_values.get_first();
      bool first_p_above_pb = VtlQuantity(*get<3>(first_p_point),
					  get<2>(first_p_point)) > pb_q; 

      size_t i = 0;
      for (auto p_it = p_values.get_it(); p_it.has_curr(); ) // pressure loop
	{
	  Correlation::NamedPar p_par = p_it.get_curr();
	  VtlQuantity p_q = par(p_par);

	  bool pb_row = false; /* true if this line concerns to bubble point */	

	  /* WARNING: these predicates must be evaluated exactly in
	     this order */
	  if (p_q <= pb_q or (not (i < 2)) or first_p_above_pb)
	    p_it.next();
	  else
	    {
	      pb_row = true;
	      p_par = npar("p", ++i == 1 ? pb_q : next_pb_q);
	      p_q = par(p_par);
	      assert(i <= 2);
	    }		

	  Simple_Pressure_Calculations();
	  row_fct_pb(row, row_units, pb_row);
	  row.popn(n);
	}
      Simple_Pop_Temperature_Parameters();
    }
}

void generate_rows_simple()
{
  assert(not tp_values.is_empty());
 
  Blackoil_Init();
  for (auto it = tp_values.get_it(); it.has_curr(); it.next())
    {
      auto & curr = it.get_curr();
      Correlation::NamedPar t_par = curr.first;
      Correlation::NamedPar p_par = curr.second;
      Blackoil_Temperature_Calculations();

      VtlQuantity p_q = par(p_par);
      {
	Blackoil_Pressure_Calculations();
	row_fct_pb(row, row_units, false);
	row.popn(n);
      }
      Blackoil_Pop_Temperature_Parameters();
    }
}


// This routine is invoked to validate the use of one or two separators
void check_second_separator_case()
{
  if (not tsep_arg.isSet())
    error_msg("It is mandatory to specify at least a separator temperature");
}

// These correlations only apply in wetgas case
Command_Arg_Optional_Correlation(rsp1, Rsp1);
Command_Arg_Optional_Correlation(veqsp, VeqspMcCain);
Command_Arg_Optional_Correlation(veqsp2, Veqsp2McCain);
Command_Arg_Optional_Correlation(gpasp, GpaspMcCain);
Command_Arg_Optional_Correlation(gpasp2, Gpasp2McCain);

# define Webgas_Init()							\
  /* Initialization of constant data */					\
  set_api();								\
  set_yg();								\
  set_tsep();								\
  set_tsep2();								\
  set_psep();								\
  set_h2s();								\
  set_co2();								\
  set_n2();								\
  set_nacl();								\
  set_ogr();								\
									\
  set_ppchc_corr();							\
  set_tpchc_corr();							\
  set_ppcm_mixing_corr();						\
  set_tpcm_mixing_corr();						\
  set_adjustedppcm_corr();						\
  set_adjustedtpcm_corr();						\
  set_zfactor_corr();							\
  set_cg_corr();							\
  set_ug_corr();							\
  set_bwb_corr();							\
  set_uw_corr();							\
  set_pw_corr();							\
  set_rsw_corr();							\
  set_cwb_corr();							\
  set_sgw_corr();							\
  set_rsp1_corr();							\
  set_gpasp_corr();							\
  set_gpasp2_corr();							\
  set_veqsp_corr();							\
  set_veqsp2_corr();							\
									\
  check_second_separator_case();					\
									\
  /* Calculation of constants required for grid generation */		\
  auto rsp1 = compute_exc(rsp1_corr, true, ogr_par);			\
  auto gpa = compute_exc(two_separators() ? gpasp2_corr : gpasp_corr, true, \
			 tsep_par, tsep2_par, psep_par, yg_par, api_par); \
									\
  auto yo = Quantity<Sg_do>(api);					\
									\
  auto veq = compute_exc(two_separators() ? veqsp2_corr : veqsp_corr, true, \
			 tsep_par, tsep2_par, psep_par, yg_par, api_par); \
									\
  auto ywgr = compute_exc(YwgrMcCain::correlation(), true, yg_par,	\
			  NPAR(yo),					\
			  NPAR(rsp1), NPAR(gpa), NPAR(veq));		\
									\
  auto yghc = compute_exc(YghcWichertAziz::correlation(), true,		\
			  npar("yg", ywgr),				\
			  NPAR(n2), NPAR(co2), NPAR(h2s));		\
  auto ppchc = compute_exc(ppchc_corr, true, NPAR(yghc),		\
			   NPAR(n2), NPAR(co2), NPAR(h2s));		\
  auto ppcm = compute_exc(ppcm_mixing_corr, true, NPAR(ppchc),		\
			  NPAR(n2), NPAR(co2), NPAR(h2s));		\
  auto tpchc = tpchc_corr->compute(check, yghc);			\
  auto tpcm = compute_exc(tpcm_mixing_corr, true, NPAR(tpchc),		\
			  NPAR(n2), NPAR(co2), NPAR(h2s));		\
  auto adjustedppcm = compute_exc(adjustedppcm_corr, true, NPAR(ppcm),	\
				  NPAR(tpcm), NPAR(co2), NPAR(h2s));	\
  auto adjustedtpcm = compute_exc(adjustedtpcm_corr, true, NPAR(tpcm),	\
				  NPAR(co2), NPAR(h2s));		\
  /* End calculation constants required for grid generation */		\
									\
  /* Initialization of correlation parameter lists */			\
  auto ug_pars = load_constant_parameters({ug_corr});			\
  insert_in_container(ug_pars, npar("tpc", adjustedtpcm),		\
		      npar("ppc", adjustedppcm));			\
  auto bwb_pars = load_constant_parameters({bwb_corr});			\
  auto uw_pars = load_constant_parameters({uw_corr});			\
  auto pw_pars = load_constant_parameters({pw_corr});			\
  auto rsw_pars = load_constant_parameters({rsw_corr});			\
  auto cwb_pars = load_constant_parameters({cwb_corr});			\
  ParList cg_pars; cg_pars.insert(npar("ppc", ppcm));			\
  ParList sgw_pars;							\
									\
  using P = pair<string, const Unit*>;					\
  auto row_units = print_csv_header(P("t", t_unit),			\
				    P("p", p_unit),			\
				    P("zfactor", &Zfactor::get_instance()), \
				    P("cg", &cg_corr->unit),		\
				    P("bwg", &Bwg::get_instance().unit), \
				    P("ug", &ug_corr->unit),		\
				    P("pg", &Pg::get_instance().unit),	\
				    P("bwb", &bwb_corr->unit),		\
				    P("uw", &uw_corr->unit),		\
				    P("pw", &pw_corr->unit),		\
				    P("rsw", &rsw_corr->unit),		\
				    P("cwb", &cwb_corr->unit),		\
				    P("sgw", &sgw_corr->unit),		\
				    P("exception", &Unit::null_unit));	\
  /* Here are the values. Ensure that the insertion order is the same as */ \
  /* for the csv header temperature loop */				\
  FixedStack<const VtlQuantity*> row(25)

# define Wetgas_Temperature_Calculations()	\
  VtlQuantity t_q = par(t_par);			\
  temperature = t_q.raw();			\
  CALL(Tpr, tpr, t_q, adjustedtpcm);		\
  auto tpr_par = NPAR(tpr);			\
						\
  insert_in_container(ug_pars, t_par, tpr_par);	\
  bwb_pars.insert(t_par);			\
  cg_pars.insert(tpr_par);			\
  rsw_pars.insert(t_par);			\
  uw_pars.insert(t_par);			\
  pw_pars.insert(t_par);			\
  cwb_pars.insert(t_par);			\
  sgw_pars.insert(t_par);			\
						\
  size_t n = insert_in_row(row, t_q)

# define Wetgas_Pressure_Calculations()					\
  VtlQuantity p_q = par(p_par);						\
									\
  pressure = p_q.raw();							\
  CALL(Ppr, ppr, p_q, adjustedppcm);					\
  auto ppr_par = NPAR(ppr);						\
									\
  VtlQuantity z = compute(zfactor_corr, check, ppr_par, tpr_par);	\
  auto z_par = NPAR(z);							\
  auto cg = compute(cg_corr, check, cg_pars, ppr_par, z_par);		\
  CALL(Bwg, bwg, t_q, p_q, z, rsp1, veq);				\
  auto ug = compute(ug_corr, check, ug_pars, p_par, ppr_par, z_par);	\
  CALL(Pg, pg, yg, t_q, p_q, z);					\
  auto rsw = compute(rsw_corr, check, rsw_pars, p_par);			\
  auto rsw_par = NPAR(rsw);						\
  auto bwb = compute(bwb_corr, check, bwb_pars, p_par);			\
  auto bw_par = npar("bw", bwb);					\
  auto pw = compute(pw_corr, check, pw_pars, p_par, bw_par);		\
  auto cwb = compute(cwb_corr, check, cwb_pars, p_par, z_par,		\
		     NPAR(bwg), rsw_par, bw_par);			\
  CALL(PpwSpiveyMN, ppw, t_q, p_q);					\
  auto uw = compute(uw_corr, check, uw_pars, p_par, NPAR(ppw));		\
  auto sgw = compute(sgw_corr, check, sgw_pars, p_par);			\
									\
  size_t n = insert_in_row(row, p_q, z, cg, bwg, ug, pg, bwb, uw,	\
			   pw, rsw, cwb, sgw);				\
									\
  assert(row.size() == 13);						\
									\
  row_fct(row, row_units);						\
  row.popn(n)

# define Wetgas_Pop_Temperature_Parameters()		\
  row.popn(n);						\
  remove_from_container(ug_pars, t_par, tpr_par);	\
  bwb_pars.remove(t_par);				\
  sgw_pars.remove(t_par);				\
  cg_pars.remove(tpr_par);				\
  uw_pars.remove(t_par);				\
  pw_pars.remove(t_par);				\
  cwb_pars.remove(t_par);				\
  rsw_pars.remove(t_par)

void generate_grid_wetgas()
{ 
  Webgas_Init();

  // Temperature loop
  for (auto t_it = t_values.get_it(); t_it.has_curr(); t_it.next()) 
    {
      Correlation::NamedPar t_par = t_it.get_curr();
   
      Wetgas_Temperature_Calculations();

      // pressure loop
      for (auto p_it = p_values.get_it(); p_it.has_curr(); p_it.next())
	{
	  Correlation::NamedPar p_par = p_it.get_curr();
	  Wetgas_Pressure_Calculations();
	}
      Wetgas_Pop_Temperature_Parameters();
    }
}

void generate_rows_wetgas()
{ 
  Webgas_Init();

  for (auto it = tp_values.get_it(); it.has_curr(); it.next())
    {
      auto & curr = it.get_curr();
      Correlation::NamedPar t_par = curr.first;
      Correlation::NamedPar p_par = curr.second;

      Wetgas_Temperature_Calculations();
      {
	Wetgas_Pressure_Calculations();
      }
      Wetgas_Pop_Temperature_Parameters();
    }
}

# define Drygas_Init()							\
  /* Initialization of constant data */					\
  set_yg();								\
  set_h2s();								\
  set_co2();								\
  set_n2();								\
  set_nacl();								\
									\
  set_ppchc_corr();							\
  set_tpchc_corr();							\
  set_ppcm_mixing_corr();						\
  set_tpcm_mixing_corr();						\
  set_adjustedppcm_corr();						\
  set_adjustedtpcm_corr();						\
  set_zfactor_corr();							\
  set_cg_corr();							\
  set_ug_corr();							\
  set_bwb_corr();							\
  set_uw_corr();							\
  set_pw_corr();							\
  set_rsw_corr();							\
  set_cwb_corr();							\
  set_sgw_corr();							\
									\
  /* Calculation of constants for Z */					\
  auto yghc = compute_exc(YghcWichertAziz::correlation(), true, NPAR(yg), \
			  NPAR(n2), NPAR(co2), NPAR(h2s));		\
  auto ppchc = compute_exc(ppchc_corr, true, NPAR(yghc),		\
			   NPAR(n2), NPAR(co2), NPAR(h2s));		\
  auto ppcm = compute_exc(ppcm_mixing_corr, true, NPAR(ppchc),		\
			  NPAR(n2), NPAR(co2), NPAR(h2s));		\
  auto tpchc = tpchc_corr->compute(check, yghc);			\
  auto tpcm = compute_exc(tpcm_mixing_corr, true, NPAR(tpchc),		\
			  NPAR(n2), NPAR(co2), NPAR(h2s));		\
  auto adjustedppcm = compute_exc(adjustedppcm_corr, true, NPAR(ppcm),	\
				  NPAR(tpcm), NPAR(co2), NPAR(h2s));	\
  auto adjustedtpcm = compute_exc(adjustedtpcm_corr, true, NPAR(tpcm),	\
				  NPAR(co2), NPAR(h2s));		\
  /* End calculation constants for z */					\
									\
  /* Initialization of correlation parameter lists */			\
  auto ug_pars = load_constant_parameters({ug_corr});			\
  insert_in_container(ug_pars, npar("tpc", adjustedtpcm),		\
		      npar("ppc", adjustedppcm));			\
  auto bwb_pars = load_constant_parameters({bwb_corr});			\
  auto uw_pars = load_constant_parameters({uw_corr});			\
  auto pw_pars = load_constant_parameters({pw_corr});			\
  auto rsw_pars = load_constant_parameters({rsw_corr});			\
  auto cwb_pars = load_constant_parameters({cwb_corr});			\
  ParList cg_pars; cg_pars.insert(npar("ppc", ppcm));			\
  ParList sgw_pars;							\
									\
  using P = pair<string, const Unit*>;					\
  auto row_units = print_csv_header(P("t", t_unit),			\
				    P("p", p_unit),			\
				    P("zfactor", &Zfactor::get_instance()), \
				    P("cg", &cg_corr->unit),		\
				    P("bg", &Bg::get_instance().unit),	\
				    P("ug", &ug_corr->unit),		\
				    P("pg", &Pg::get_instance().unit),	\
				    P("bwb", &bwb_corr->unit),		\
				    P("uw", &uw_corr->unit),		\
				    P("pw", &pw_corr->unit),		\
				    P("rsw", &rsw_corr->unit),		\
				    P("cwb", &cwb_corr->unit),		\
				    P("sgw", &sgw_corr->unit),		\
				    P("exception", &Unit::null_unit));	\
									\
  /* Here are the values. Ensure that the insertion order is the same as */ \
  /* for the csv header temperature loop */				\
  FixedStack<const VtlQuantity*> row(25)

# define Drygas_Temperature_Calculations()	\
  VtlQuantity t_q = par(t_par);			\
  temperature = t_q.raw();			\
  CALL(Tpr, tpr, t_q, adjustedtpcm);		\
  auto tpr_par = NPAR(tpr);			\
						\
  insert_in_container(ug_pars, t_par, tpr_par);	\
  bwb_pars.insert(t_par);			\
  cg_pars.insert(tpr_par);			\
  rsw_pars.insert(t_par);			\
  uw_pars.insert(t_par);			\
  pw_pars.insert(t_par);			\
  cwb_pars.insert(t_par);			\
  sgw_pars.insert(t_par);			\
						\
  size_t n = insert_in_row(row, t_q)

# define Drygas_Pressure_Calculations()					\
  VtlQuantity p_q = par(p_par);						\
									\
  pressure = p_q.raw();							\
  CALL(Ppr, ppr, p_q, adjustedppcm);					\
  auto ppr_par = NPAR(ppr);						\
 									\
  VtlQuantity z = compute(zfactor_corr, check, ppr_par, tpr_par);	\
  auto z_par = NPAR(z);							\
  auto cg = compute(cg_corr, check, cg_pars, ppr_par, z_par);		\
  CALL(Bg, bg, t_q, p_q, z);						\
  auto ug = compute(ug_corr, check, ug_pars, p_par, ppr_par, z_par);	\
  CALL(Pg, pg, yg, t_q, p_q, z);					\
  auto rsw = compute(rsw_corr, check, rsw_pars, p_par);			\
  auto rsw_par = NPAR(rsw);						\
  auto bwb = compute(bwb_corr, check, bwb_pars, p_par);			\
  auto bw_par = npar("bw", bwb);					\
  auto pw = compute(pw_corr, check, pw_pars, p_par, bw_par);		\
  auto cwb = compute(cwb_corr, check, cwb_pars, p_par, z_par,		\
		       NPAR(bg), rsw_par, bw_par);			\
  CALL(PpwSpiveyMN, ppw, t_q, p_q);					\
  auto uw = compute(uw_corr, check, uw_pars, p_par, NPAR(ppw));		\
  auto sgw = compute(sgw_corr, check, sgw_pars, p_par);			\
									\
  size_t n = insert_in_row(row, p_q, z, cg, bg, ug, pg, bwb, uw,	\
			     pw, rsw, cwb, sgw);			\
									\
  assert(row.size() == 13);						\
									\
  row_fct(row, row_units);						\
  row.popn(n)

# define Drygas_Pop_Temperature_Parameters()		\
  row.popn(n);						\
  remove_from_container(ug_pars, t_par, tpr_par);	\
  bwb_pars.remove(t_par);				\
  sgw_pars.remove(t_par);				\
  cg_pars.remove(tpr_par);				\
  uw_pars.remove(t_par);				\
  pw_pars.remove(t_par);				\
  cwb_pars.remove(t_par);				\
  rsw_pars.remove(t_par)

void generate_grid_drygas()
{
  Drygas_Init();
  for (auto t_it = t_values.get_it(); t_it.has_curr(); t_it.next()) 
    {
      Correlation::NamedPar t_par = t_it.get_curr();

      Drygas_Temperature_Calculations();

      // pressure loop
      for (auto p_it = p_values.get_it(); p_it.has_curr(); p_it.next())
	{
	  Correlation::NamedPar p_par = p_it.get_curr();
	  Drygas_Pressure_Calculations();
	}

      Drygas_Pop_Temperature_Parameters();
    }
}

void generate_rows_drygas()
{
  Drygas_Init();
 
  for (auto it = tp_values.get_it(); it.has_curr(); it.next())
    {
      auto & curr = it.get_curr();
      Correlation::NamedPar t_par = curr.first;
      Correlation::NamedPar p_par = curr.second;

      Drygas_Temperature_Calculations();

      {
	Drygas_Pressure_Calculations();
      }

      Drygas_Pop_Temperature_Parameters();
    }
}

void process_brine()
{
  cout << "brine options not yet implemented" << endl;
  abort();
}

void process_gascondensate()
{
  cout << "gascondensed options not yet implemented" << endl;
  abort();
}

# define Define_Process_Fluid_Fct(name)		\
  void process_##name()				\
  {						\
    if (tp_values.is_empty())			\
      generate_grid_##name();			\
    else					\
      generate_rows_##name();			\
  }

Define_Process_Fluid_Fct(blackoil);
Define_Process_Fluid_Fct(wetgas);
Define_Process_Fluid_Fct(drygas);
Define_Process_Fluid_Fct(simple);

AHDispatcher<string, void (*)()>
grid_dispatcher("blackoil", process_blackoil,
		"wetgas", process_wetgas,
		"drygas", process_drygas,
		"brine", process_brine,
		"gascondensate", process_gascondensate,
		"simple", process_simple);

void generate_grid(const string & fluid_type)
{
  set_check(); 
  report_exceptions = catch_exceptions.getValue();

  set_ranges();

  transposed = transpose_par.getValue();
  grid_dispatcher.run(fluid_type);

  if (transposed)
    print_transpose();
  else if (report_exceptions)
    {
      cout << endl
	   << "Exceptions:" << endl;
      exception_list.for_each([] (const auto & s) { printf(s.c_str()); });
    }

  exit(0);
}

using OptionPtr = DynList<DynList<double>> (*)();

int main(int argc, char *argv[])
{
  cmd.parse(argc, argv);

  if (transpose_par.isSet() and catch_exceptions.isSet())
    error_msg("--transpose and --exceptions cannot be set together"
	      " (due to performance reasons)");

  if (print_types.getValue())
    print_fluid_types();

  process_property_unit_changes();

  if (grid.isSet())
    generate_grid(grid.getValue());

  cout << "No " << grid.getName() << " or " << print_types.getName()
       << " have been set" << endl;
  abort();
}
