
# include <istream>

# include <ah-dispatcher.H>
# include <ah-stl-utils.H>

# include <tclap/CmdLine.h>

# include <json.hpp>

# include <metadata/pvt-calibrate.H>

using namespace std;
using namespace TCLAP;
using namespace Aleph;

PvtData data;

# define Define_Check_Property(pname, pq_name)				\
  static void check_##pname(const ValuesArg & arg)			\
  {									\
    if (&arg.unit_ptr->physical_quantity != &pq_name::get_instance())	\
      ALEPHTHROW(InvalidTargetUnit, arg.unit_ptr->name +			\
	       " is not an unit for " + pq_name::get_instance().name);	\
  }

static const DynSetTree<string> valid_targets =
  { "rs", "bob", "boa", "uob", "uoa", "coa", "bo", "uo" };

// Defines a input of form
// "property-name property-unit t tunit pb punit p-list property-list"
struct ValuesArg
{
  static AHDispatcher<string, void (*)(const ValuesArg&)> check_dispatcher;

  string target_name; // property name
  const Unit * unit_ptr = nullptr; // unit for property

  double t; // temperature
  const Unit * tunit_ptr = nullptr; // temperature unit

  double uod  = -1; // uod
  double uobp = -1;
  double bobp = -1;

  double pb = -1; // bubble point
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

  Define_Check_Property(rs, GORGLRvolumeRatio);
  Define_Check_Property(bo, FVFvolumeRatio);
  Define_Check_Property(uo, DynamicViscosity);
  Define_Check_Property(coa, IsothermalCompressibility);

  void validate_property()
  {
    if (not valid_targets.contains(target_name))
      ALEPHTHROW(InvalidProperty, "target name " + target_name +
	       " is not valid");
  }

  void read_coa(istringstream & iss)
  {
    string data;
    DynList<double> vals;
    size_t n = 0;
    for (; iss >> data; ++n)
      {
	if (not is_double(data))
	  ALEPHTHROW(CommandLineError, data + " is not a double");
	vals.append(atof(data));
      }

    if ((n % 3) != 0)
      ALEPHTHROW(CommandLineError,
	       "In input of coa values: number of values is not multiple of 3");
    
    const size_t dim = n/3;
    DynList<double> pmin, pmax;
    each(dim, [&vals, &pmin] () { pmin.append(vals.remove_first()); });
    each(dim, [&vals, &pmax] () { pmax.append(vals.remove_first()); });
    if (not (zip_all([] (auto t) { return get<0>(t) < get<1>(t); }, pmin, pmax)
        or zip_all([] (auto t) { return get<0>(t) > get<1>(t); }, pmin, pmax)))
      ALEPHTHROW(CommandLineError,
	       "At least a pressure pair for compressibility is invalid");

    each(dim, [&vals, this] () { values.append(vals.remove_first()); });

    p = zip_maps<double>([] (auto t) { return (get<1>(t) + get<0>(t))/2; },
			 pmin, pmax);
    if (not is_sorted(p))
      {
	p = p.rev();
	values = values.rev();
      }
  }

  void set_uod(DynList<double> & vals)
  {
    uod = vals.remove_first();
  }

  ValuesArg & operator = (const string & str)
  {
    istringstream iss(str);

    // read target_name
    if (not (iss >> target_name))
      ALEPHTHROW(CommandLineError, str + " does not contain target name");

    const bool compound_target = target_name == "bo" or target_name == "uo";

    validate_property();
    if (not compound_target and not Correlation::array().exists([this] (auto ptr)
        { return ptr->target_name() == target_name; }))
      ALEPHTHROW(CommandLineError, target_name + " is invalid as target name");

    { // read property unit
      string property_unit;
      if (not (iss >> property_unit))
	ALEPHTHROW(CommandLineError, "cannot read property unit");
      unit_ptr = Unit::search(property_unit);
      if (unit_ptr == nullptr)
	ALEPHTHROW(CommandLineError, property_unit + " unit for " + target_name +
		 " property not found");
    }

    check_dispatcher.run(target_name, *this);

     // read temperature unit
    string unit_name;
    if (not (iss >> unit_name))
      ALEPHTHROW(CommandLineError, "cannot read temperature unit");
    tunit_ptr = Unit::search(unit_name);
    if (tunit_ptr == nullptr)
      ALEPHTHROW(CommandLineError, unit_name + " for temperature not found");
    if (&tunit_ptr->physical_quantity != &Temperature::get_instance())
      ALEPHTHROW(CommandLineError, unit_name + " is not a temperature unit");

    // read temperature value
    string data;
    if (not (iss >> data))
      ALEPHTHROW(CommandLineError, "temperature value not found");
    if (not is_double(data))
      ALEPHTHROW(CommandLineError, "temperature value " + data +
	       " is not a double");
    t = atof(data);

    // read pressure unit
    if (not (iss >> unit_name))
      ALEPHTHROW(CommandLineError, str + " does not contain unit name");
    punit_ptr = Unit::search(unit_name);
    if (punit_ptr == nullptr)
      ALEPHTHROW(CommandLineError, "unit " + unit_name + " for pressure not found");
    if (&punit_ptr->physical_quantity != &Pressure::get_instance())
      ALEPHTHROW(CommandLineError, unit_name + " is not for pressure");

    if (not compound_target)
      {
	// read pb value
	if (not (iss >> data))
	  ALEPHTHROW(CommandLineError, "pb value not found");
	if (not is_double(data))
	  ALEPHTHROW(CommandLineError, "pb value " + data + " is not a double");
	pb = atof(data);
      }

    if (target_name == "coa")
      {
	read_coa(iss);
	return *this;
      }

    DynList<double> vals;
    size_t n = 0;
    for (; iss >> data; ++n)
      {
	if (not is_double(data))
	  ALEPHTHROW(CommandLineError, data + " is not a double");
	vals.append(atof(data));
      }

    bool regression_required = false;
    if (target_name == "uob" or target_name == "uoa")
      {
	if (target_name == "uob")
	  if ((n % 2) != 0)
	    set_uod(vals);
	  else
	    regression_required = true;
	else if ((n % 2) == 0)
	  ALEPHTHROW(CommandLineError, "In uoa input: expecting uod value");
	else
	  set_uod(vals);
      }

    auto it = vals.get_it();
    for (size_t i = 0; i < n/2; ++i, it.next())
      p.append(it.get_curr());

    bool p_reversed = false;
    if (not is_sorted(p))
      {
	p = p.rev();
	p_reversed = true;
      }

    bool uobp_required = false;
    if (not compound_target and not p.exists([this] (auto v) { return v == pb; }))
      if (target_name != "uob")
	ALEPHTHROW(CommandLineError, "pb value " + to_string(pb) +
		 " not found in pressures array");
      else
	uobp_required = true;

    const size_t num_samples = n/2;

    for (size_t i = 0; i < num_samples; ++i, it.next())
      values.append(it.get_curr());

    if (p_reversed)
      values = values.rev();

    if (uobp_required)
      {
	assert(target_name == "uob");
	const double p1 = p.nth(num_samples - 2);
	const double p2 = p.get_last();
	const double uo1 = values.nth(num_samples - 2);
	const double uo2 = values.get_last();
	const double uobp = extrapolate_right(p1, p2, uo1, uo2, pb);
	p.append(pb);
	values.append(uobp);
      }
      
    if (target_name == "boa")
      bobp = unit_convert(*unit_ptr, values.get_first(), RB_STB::get_instance());
    if (target_name == "uoa")
      uobp = unit_convert(*unit_ptr, values.get_first(), CP::get_instance());

    if (regression_required)
      uod = LFit(p, values).c;

    if (target_name.size() == 3 and target_name[2] == 'a')
      p.get_first() = nextafter(p.get_first(), p.get_last());

    return *this;
  }
};

AHDispatcher<string, void (*)(const ValuesArg&)>
ValuesArg::check_dispatcher("rs", ValuesArg::check_rs,
			    "bob", ValuesArg::check_bo,
			    "boa", ValuesArg::check_bo,
			    "bo", ValuesArg::check_bo,
			    "uob", ValuesArg::check_uo,
			    "uoa", ValuesArg::check_uo,
			    "uo", ValuesArg::check_uo,
			    "coa", ValuesArg::check_coa);

namespace TCLAP
{
  template <> struct ArgTraits<ValuesArg> { typedef StringLike ValueCategory; };
}

// allowed parameter names (they are values ​​or ranges, but they are
// not correlations). This table is used for validating change of
// units
DynSetTree<string> const_name_tbl =
  { "api", "rsb", "yg", "tsep", "psep", "h2s", "co2", "n2", "nacl" };

vector<string> valid_consts = to_vector(const_name_tbl);

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
      ALEPHTHROW(CommandLineError, str + " is not a pair par-name unit");

    if (not const_name_tbl.contains(name))
      ALEPHTHROW(CommandLineError, name + " is an invalid parameter name");

    unit_ptr = Unit::search(unit_name);
    if (unit_ptr == nullptr)
      ALEPHTHROW(CommandLineError, "cannot find unit " + unit_name);

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

struct GenerateInput
{
  string target_name = "No-defined";
  string src_name = "No-defined";

  friend ostream & operator << (ostream & out, const GenerateInput & i)
  {
    return out << "Target name = " << i.target_name << endl
	       << "Source name = " << i.src_name;
  }

  GenerateInput() {}

  GenerateInput & operator = (const string & str)
  {
    istringstream iss(str);
    if (not (iss >> target_name))
      ALEPHTHROW(CommandLineError, "Cannot read target name property");
    if (not valid_targets.contains(target_name))
      ALEPHTHROW(CommandLineError, target_name +  " is not a valid target name");

    if (not (iss >> src_name))
      ALEPHTHROW(CommandLineError, "Cannot read source name property");
    if (not valid_targets.contains(src_name))
      ALEPHTHROW(CommandLineError, src_name +  " is not a valid source name");

    return *this;
  }
};

namespace TCLAP
{
  template<> struct ArgTraits<GenerateInput>
  { typedef StringLike ValueCategory; };
}

struct RmProperty
{
  string yname = "No-Defined";
  double t = -1;

  RmProperty() {}

  RmProperty & operator = (const string & str)
  {
    istringstream iss(str);
    if (not (iss >> yname))
      ALEPHTHROW(CommandLineError, "Cannot read target name property");
    if (not valid_targets.contains(yname))
      ALEPHTHROW(InvalidProperty, yname + " is not a valid property");
    string data;
    if (not (iss >> data))
      ALEPHTHROW(CommandLineError, "in rm option: cannot read temperature value");
    if (not is_double(data))
      ALEPHTHROW(CommandLineError, "value " + data + " is not a double");
    t = atof(data);
    return *this;
  }
};

struct CorrArgs
{
  DynList<const Correlation*> corr_list;

  CorrArgs & operator = (const string & str)
  {
    istringstream iss(str);
    string name;
    while (iss >> name)
      {
	auto corr_ptr = Correlation::search_by_name(name);
	if (corr_ptr == nullptr)
	  ALEPHTHROW(CommandLineError, "correlation " + name + " not found");
	corr_list.append(corr_ptr);
      }

    if (corr_list.is_empty())
      ALEPHTHROW(CommandLineError, "Not correlation specified for calibration");

    name = corr_list.get_first()->target_name();
    if (not corr_list.all([&name] (auto p) { return p->target_name() == name; }))
      ALEPHTHROW(CommandLineError,
	       "Not all the correlations to be calibrated have target " + name);

    return *this;
  }
};

struct RangeDesc
{
  double min = 0, max = 0;
  size_t n = 1; // num of steps

  RangeDesc & operator = (const string & str)
  {
    istringstream iss(str);
    if (not (iss >> min >> max >> n))
      ALEPHTHROW(CommandLineError, str + " is not of form \"min max n\"");

    if (n == 0)
      ALEPHTHROW(CommandLineError, ::to_string(n) + " n cannot be zero");

    if (min > max)
      {
	ostringstream s;
	s << "min value " << min << " greater than max value " << max;
	ALEPHTHROW(CommandLineError, s.str());
      }

    return *this;
  }

  double step() const noexcept { return (max - min) / (n - 1); }
};

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
	  ALEPHTHROW(CommandLineError, data + " is not a double");

	values.append(atof(data));
      }

    if (values.is_empty())
      ALEPHTHROW(CommandLineError, "cannot read array");

    in_place_sort(values);
 
    return *this;
  }
};

namespace TCLAP
{
  template<> struct ArgTraits<RmProperty> { typedef StringLike ValueCategory; };
  template<> struct ArgTraits<CorrArgs> { typedef StringLike ValueCategory; };
  template<> struct ArgTraits<RangeDesc> { typedef StringLike ValueCategory; };
  template<> struct ArgTraits<ArrayDesc> { typedef StringLike ValueCategory; };
}

CmdLine cmd = { "adjust", ' ', "0" };

ValueArg<RangeDesc> t = { "t", "t", "t range", false, RangeDesc(),
			  "t min max n", cmd };

ValueArg<RangeDesc> p = { "p", "p", "p range", false, RangeDesc(),
			  "p min max n", cmd };

ValueArg<ArrayDesc> tarray = { "", "t_array", "t array", false, ArrayDesc(),
			       "list-t-values", cmd };

ValueArg<ArrayDesc> parray = { "", "p_array", "p array", false, ArrayDesc(),
			       "list-p-values", cmd };

SwitchArg Cplot = { "", "Cplot", "generate simple cplot command", cmd };

vector<string> r_types = { "rs", "co", "bo", "uo" };
ValuesConstraint<string> allowed_r_types = r_types;
ValueArg<string> R = { "R", "R", "direct R output", false, "",
		       &allowed_r_types, cmd };

// Unit change specification. Suitable for any parameter
MultiArg<ArgUnit> unit = { "", "unit", "change unit of input data", false,
			   "unit \"par-name unit\"", cmd };

ValueArg<string> p_out_unit = { "", "p-out-unit", "pressure output unit", false,
				"psia", "pressure output unit", cmd };

ValueArg<string> y_out_unit = { "", "y-out-unit", "y output unit", false,
				"", "y output unit", cmd };

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

vector<string> relax_names =
  {
    "api", "yo", "rsb", "yg", "tsep", "psep", "h2s", "co2", "n2", "nacl", "pb",
    "uod", "rs", "bobp", "uobp", "coa", "bob", "boa", "uob", "uoa"
  };
ValuesConstraint<string> allowed_relax_names = relax_names;
MultiArg<string> relax_pars =
  { "", "relax", "relax parameter range application check", false,
    &allowed_relax_names, cmd };

MultiArg<ValuesArg> property =
  { "", "property", "property array", false, 
    "property array in format \"property property-unit t tunit pb punit p-list "
    "property-list\"", cmd };

DynSetTree<string> input_types =
  { "rs", "bob", "boa", "uob", "uob", "cob", "coa" };

MultiArg<GenerateInput> input = { "", "input", "input from correlation", false,
				  "input tgt src", cmd };
				  
vector<string> output_types = { "R", "csv", "mat" };
ValuesConstraint<string> allowed_output_types = output_types;
ValueArg<string> output = { "", "output", "output type", false,
			    "mat", &allowed_output_types, cmd };

vector<string> sort_types = { "r2", "mse", "sigma", "sumsq", "c", "m" };
ValuesConstraint<string> allowed_sort_types = sort_types;
ValueArg<string> sort = { "", "sort", "sort type", false,
			  "r2", &allowed_sort_types, cmd };

vector<string> mode_types = { "single", "calibrated", "both" };
ValuesConstraint<string> allowed_mode_types = mode_types;
ValueArg<string> mode_type = { "", "mode", "mode", false, "both",
			       &allowed_mode_types, cmd };

SwitchArg json = { "", "json", "generate json of data", cmd };

SwitchArg split_uo_arg = { "", "split-uo", "split uo vector", cmd };
SwitchArg save = { "", "save", "save data to json", cmd };
SwitchArg transpose_out = { "", "transpose", "transpose output", cmd };

MultiArg<RmProperty> rm_property =
  { "", "rm-property", "remove property t", false, "\"property-tag t\"", cmd };

ValuesConstraint<string> allowed_consts = valid_consts;
MultiArg<string> rm_const = { "", "rm-const", "remove const", false,
			      &allowed_consts, cmd };

ValueArg<string> file = { "f", "file", "load json", false, "", "load json", cmd };

SwitchArg print = { "", "print", "print stored data", cmd };

ValueArg<string> Print = { "P", "Print", "print stored data", false, "",
			   "constants|correlations|property-name", cmd };

ValueArg<string> list_corr = { "l", "list", "list correlations", false, "",
			       "list property", cmd };

ValueArg<string> match = { "m", "match", "print matching correlations", false, "",
			   "match property", cmd };

ValueArg<string> apply = { "a", "apply", "print applying correlations", false, "",
			   "apply property", cmd };

ValueArg<string> napply =
  { "n", "napply", "print non applying correlations and reasons", false, "",
    "napply property", cmd };

ValueArg<CorrArgs> cal = { "", "lcal", "calibrate correlations", false,
			   CorrArgs(), "calibrate correlation-list", cmd };

ValueArg<CorrArgs> pb_cal = { "", "pbcal", "calibrate correlations", false,
			      CorrArgs(), "calibrate correlation-list", cmd };

ValueArg<CorrArgs> uod_cal = { "", "uodcal", "calibrate correlations", false,
			       CorrArgs(), "calibrate correlation-list", cmd };

SwitchArg cplot = { "", "cplot", "generate cplot command", cmd };

SwitchArg eol { "", "eol", "add to output an end of line", cmd };

SwitchArg auto_arg = { "", "auto", "automatic calibration", cmd };

SwitchArg Auto_arg =
  { "", "Auto", "set correlation given by automatic calibration", cmd };

SwitchArg exp_arg = { "", "exp", "put experimental pressures", cmd };

SwitchArg pbexp_arg = { "", "pbexp", "put experimental pb values", cmd };

# define Corr_Arg(NAME)							\
  ValueArg<string> NAME##_corr_arg =					\
    { "", #NAME, "set " #NAME " correlation", false, "",		\
      "set " #NAME " correlation", cmd };				\
									\
  ValueArg<string> NAME##_cal_corr_arg =				\
    { "", #NAME "-cal", "set calibrated " #NAME " correlation", false,	\
      "", "set calibrated " #NAME " correlation", cmd };		\
									\
  const Correlation * NAME##_corr = nullptr;				\
  double c_##NAME = 0;							\
  double m_##NAME = 1;							\
									\
  void set_##NAME##_corr()						\
  {									\
    if (not NAME##_corr_arg.isSet() and not NAME##_cal_corr_arg.isSet()) \
      return;								\
    if (NAME##_corr_arg.isSet() and NAME##_cal_corr_arg.isSet())	\
      ALEPHTHROW(CommandLineError, "options " + NAME##_corr_arg.getName() + \
	       NAME##_cal_corr_arg.getName() + " cannot be used together"); \
    const bool calibrated = NAME##_cal_corr_arg.isSet();		\
    const string corr_name = calibrated ? NAME##_cal_corr_arg.getValue() : \
      NAME##_corr_arg.getValue();					\
    auto corr_ptr = Correlation::search_by_name(corr_name);		\
    if (corr_ptr == nullptr)						\
      ALEPHTHROW(CommandLineError, "correlation " + corr_name + " not found"); \
    if (corr_ptr->target_name() != #NAME)				\
      ALEPHTHROW(CommandLineError, "correlation " + corr_ptr->name +	\
	       " is not for " #NAME);					\
    data.NAME##_corr = corr_ptr;					\
    if (calibrated)							\
      {									\
	const PvtData::StatsDesc s = data.stats(corr_ptr);		\
	data.c_##NAME = CorrStat::c(s.desc);				\
	data.m_##NAME = CorrStat::m(s.desc);				\
      }									\
  }

Corr_Arg(pb);
Corr_Arg(rs);
Corr_Arg(bob);
Corr_Arg(boa);
Corr_Arg(uob);
Corr_Arg(uoa);
Corr_Arg(uod);
Corr_Arg(coa);

struct BanList
{
  DynSetTree<const Correlation*> corr_list;

  BanList & operator = (const string & str)
  {
    string data;
    istringstream iss(str);
    while (iss >> data)
      {
	auto corr_ptr = Correlation::search_by_name(data);
	if (corr_ptr == nullptr)
	  ALEPHTHROW(CorrelationNotFound, "Correlation" + data + " not found");

	if (corr_list.has(corr_ptr))
	  ALEPHTHROW(DuplicatedCorrelationName, "Correlation " + data +
		   " is already defined in ban list");

	corr_list.append(corr_ptr);
      }

    return *this;
  }
};

namespace TCLAP
{
  template <> struct ArgTraits<BanList> { typedef StringLike ValueCategory; };
}

vector<string> auto_types = { "r2", "mse", "sigma", "sumsq", "c", "m" };
ValuesConstraint<string> allowed_auto_types = auto_types;
ValueArg<string> auto_type = { "", "auto-type", "auto triage type", false, "r2",
		       &allowed_auto_types, cmd };

ValueArg<BanList> ban = { "b", "ban",
			  "ban correlation list for automatic calibration",
			  false, BanList(), "banned correlation list", cmd };

ValueArg<double> threshold = { "", "threshold", "auto threshold", false, 0.0,
			"auto threshold", cmd };

DynMapTree<string, PvtData::AutoApplyType> auto_map =
  {
    { "r2", PvtData::AutoApplyType::r2 },
    { "mse", PvtData::AutoApplyType::mse },
    { "sigma", PvtData::AutoApplyType::sigma },
    { "sumsq", PvtData::AutoApplyType::sumsq },
    { "c", PvtData::AutoApplyType::c },
    { "m", PvtData::AutoApplyType::m }
  };

const Unit * test_unit(const string & par_name, const Unit & dft_unit)
{
  if (not const_name_tbl.has(par_name))
    ALEPHTHROW(CommandLineError, "unknown parameter name " + par_name);

  const Unit * ret = &dft_unit;
  for (auto & par : unit.getValue())
    if (par.name == par_name)
      {
	if (&dft_unit.physical_quantity != &par.unit_ptr->physical_quantity)
	  ALEPHTHROW(CommandLineError, par_name + " unit: physical quantity " +
		   ret->physical_quantity.name + " is invalid");
	return ret;
      }
  return ret;
}

# define Declare_Test_Out_Unit(NAME)						\
  const Unit * test_##NAME##_out_unit(const Unit * curr_unit)		\
  {									\
    if (not NAME##_out_unit.isSet())					\
      return curr_unit;							\
									\
    const string & NAME##unit_name = NAME##_out_unit.getValue();	\
    const Unit * ret_unit = Unit::search(NAME##unit_name);		\
    if (ret_unit == nullptr)						\
      ALEPHTHROW(UnitNotFound, #NAME " unit " + NAME##unit_name + " not found"); \
    if (not ret_unit->is_sibling(*curr_unit))				\
      ALEPHTHROW(WrongSiblingUnit, NAME##unit_name + " and " + curr_unit->name + \
	       " are not physically related");				\
    return ret_unit;							\
  }

Declare_Test_Out_Unit(p);
Declare_Test_Out_Unit(y);

DynSetTree<string> relax_names_tbl;
void set_relax_names()
{
  for (auto & p : relax_pars.getValue())
    relax_names_tbl.append(p);
}

void remove_consts()
{
  for (auto & c : rm_const.getValue())
    data.rm_const(c);
}

void remove_properties()
{
  for (auto & p : rm_property.getValue())
    data.rm_vector(p.t, p.yname);
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

  for (auto & a : property.getValue())
    data.add_vector(a.t, a.pb, a.uod, a.bobp, a.uobp, a.p, *a.punit_ptr,
		    a.target_name, a.values, *a.unit_ptr);
}

void input_data(const GenerateInput & in)
{
  DynList<const VectorDesc*> src_vectors = data.search_vectors(in.src_name);
  if (src_vectors.is_empty())
    ALEPHTHROW(CommandLineError, "target name " + in.target_name +
	     " not found in data set");

  DynList<const VectorDesc*> tgt_vectors = data.search_vectors(in.target_name);
  DynSetTree<double> temps = tgt_vectors.maps<double>([] (auto ptr)
    {
      return ptr->t;
    });

  auto pars = data.get_corr(in.target_name);
  const Correlation * corr_ptr = get<0>(pars);
  if (corr_ptr == nullptr)
    ALEPHTHROW(CommandLineError, "Correlation for target " + in.target_name +
	     " has not been set");
      
  const double & c = get<1>(pars);
  const double & m = get<2>(pars);

  for (auto it = src_vectors.get_it(); it.has_curr(); it.next())
    {
      auto src_ptr = it.get_curr();
      if (temps.has(src_ptr->t))
	continue;

      // TODO: buscar pares de entradas
      VectorDesc d = data.build_samples(src_ptr, corr_ptr, c, m);
      temps.insert(d.t);
      data.add_vector(move(d));
    }	 
}

void input_data()
{
  for (auto & d : input.getValue())
    input_data(d);
}

void print_correlations(const DynList<DynList<string>> & l)
{
  if (l.is_empty())
    cout << "Not found" << endl;
  else      
    l.for_each([] (auto & l)
	       {
		 cout << l.get_first() << "(";
		 auto & last = l.get_last();
		 for (auto it = l.get_it(1); it.has_curr(); it.next())
		   {
		     auto & curr = it.get_curr();
		     cout << curr;
		     if (&curr != &last)
		       cout << ", ";
		   }
		 cout << ")" << endl;
	       });
}

void terminate_app()
{
  if (eol.getValue())
    cout << endl;
  exit(0);
}

SwitchArg print_uod = { "", "print_uod", "only print uod value", cmd };

void process_print_data()
{
  if (not print.getValue())
    return;
  if (json.getValue())
    cout << data.to_json().dump(2) << endl;
  else 
    if (print_uod.getValue())
      data.vectors.for_each([] (auto & v)
        {
	  cout << "t = " << v.t << " degF uod = "<< v.uod << " cP" << endl;
	});
    else
      cout << data << endl;
  terminate_app();
}

void process_Print_data()
{
  if (not Print.isSet())
    return;

  const string & type = Print.getValue();
  if (type == "constants")
    cout << "Constants:" << endl
	 << shift_lines_to_left(data.const_list(), 2) << endl;
  else if (type == "correlations")
    cout << "Correlations:" << endl
	 << shift_lines_to_left(data.corr_list(), 2) << endl;
  else
    {
      auto vectors = data.search_vectors(type);
      if (vectors.is_empty())
	cout << "Property " << type << " not found in data set" << endl;
      else
	{
	  cout << type << ":" << endl;
	  for (auto it = vectors.get_it(); it.has_curr(); it.next())
	    cout << shift_lines_to_left(it.get_curr()->to_string(), 2) << endl;
	}
    }
  terminate_app();
}

void process_list()
{
  if (not list_corr.isSet())
    return;
  print_correlations(Correlation::array().
    filter([tgt = list_corr.getValue()] (auto p)
	   {
	     return p->target_name() == tgt;
	   }).maps<DynList<string>>([] (auto p) { return p->to_dynlist(); }));
  terminate_app();
}

void process_match()
{
  if (not match.isSet())
    return;
  print_correlations(data.matches_with_pars(match.getValue()).
		     maps<DynList<string>>([] (auto p)
					   { return p->to_dynlist(); }));
  terminate_app();
}

using T = PvtData::StatsDesc;

# define Define_Cmp(name)						\
  auto cmp_##name = [] (const T & d1, const T & d2)			\
  {									\
    return CorrStat::name(d1.desc) < CorrStat::name(d2.desc);		\
  }

# define Define_1_Cmp(name)						\
  auto cmp_##name = [] (const T & d1, const T & d2)			\
  {									\
    return fabs(1 - CorrStat::name(d1.desc)) < fabs(1 - CorrStat::name(d2.desc)); \
  }

Define_1_Cmp(r2);
Define_Cmp(mse);
Define_Cmp(sumsq);
Define_Cmp(sigma);
Define_1_Cmp(m);

auto cmp_c = [] (const T & d1, const T & d2)
{
  return abs(CorrStat::c(d1.desc)) < abs(CorrStat::c(d2.desc));
};

DynMapTree<string, bool (*)(const T&, const T&)> cmp =
  { { "r2", cmp_r2}, {"mse", cmp_mse}, {"sigma", cmp_sigma},
    {"sumsq", cmp_sumsq}, {"c", cmp_c}, {"m", cmp_m} }; 

void process_apply()
{
  if (not apply.isSet())
    return;

  auto property_name = apply.getValue();
  if (not valid_targets.contains(property_name) and property_name != "pb" and
      property_name != "uod")
    ALEPHTHROW(CommandLineError, "target name " + property_name + " is not valid");
  
  auto corr_list = data.can_be_applied(property_name, relax_names_tbl,
				       ban.getValue().corr_list);

  DynList<T> stats = Aleph::sort(corr_list.maps<T>([] (auto corr_ptr)
    {
      return data.stats(corr_ptr);
    }), cmp[::sort.getValue()]);

  DynList<DynList<string>> rows = stats.maps<DynList<string>>([] (auto & t)
    {
      DynList<string> ret = build_dynlist<string>(t.corr_ptr->name);
      auto stats = t.valid ? CorrStat::desc_to_dynlist(t.desc) :
        CorrStat::invalid_desc_to_dynlist();
      ret.append(stats);
      return ret;
    });
  
  DynList<string> header = build_dynlist<string>("Correlation");
  header.append(CorrStat::stats_header());

  rows.insert(header);

  const auto & out_type = output.getValue();
  if (out_type == "csv")
    cout << Aleph::to_string(format_string_csv(rows)) << endl;
  else
    cout << Aleph::to_string(format_string(rows)) << endl;

  terminate_app();
}

DynList<PvtData::AutoDesc> best_list()
{
  const double & threshold = ::threshold.getValue();
  
  return data.auto_apply(relax_names_tbl, ban.getValue().corr_list,
			 threshold, auto_map[auto_type.getValue()]);
}

void process_auto()
{
  if (not auto_arg.isSet())
    return;

  auto corr_list = best_list();

  DynList<DynList<string>> rows = corr_list.maps<DynList<string>>([] (auto & a)
    {
      DynList<string> ret = build_dynlist<string>(a.corr_ptr->name);
      ret.append(CorrStat::desc_to_dynlist(a.d));
      return ret;
    });
  
  DynList<string> header = build_dynlist<string>("Correlation");
  header.append(CorrStat::stats_header());
  rows.insert(header);

  const auto & out_type = output.getValue();
  if (out_type == "csv")
    cout << Aleph::to_string(format_string_csv(rows)) << endl;
  else
    cout << Aleph::to_string(format_string(rows)) << endl;

  terminate_app();
}

void process_Auto()
{
  if (not Auto_arg.isSet())
    return;

  if (not Cplot.isSet() and not cplot.isSet() and not print.isSet() and
      not Print.isSet())
    error_msg("Option " + Auto_arg.getName() + " must be used in combination "
	      "with " + cplot.getName() + " or " + Cplot.getName() + " or " +
	      print.getName() + " or " + Print.getName());

  best_list().for_each([ptr = &data] (auto & a) { ptr->set_correlation(a); });
}

void process_napply()
{
  if (not napply.isSet())
    return;
  
  auto property_name = napply.getValue();
  if (not valid_targets.contains(property_name) and property_name != "pb" and
      property_name != "uod")
    ALEPHTHROW(CommandLineError, "target name " + property_name + " is not valid");

  auto missing_list = data.list_restrictions(property_name, relax_names_tbl,
					     ban.getValue().corr_list);

  DynList<DynList<string>> rows =
    missing_list.maps<DynList<string>>([] (auto p)
    {
      DynList<string> row = build_dynlist<string>(p.first->name);
      row.append(p.second.template maps<string>([] (pair<string, bool> p)
        {
	  return build_dynlist<string>(p.first + " (" +
				       (p.second ? "range)" : "missing)"));
      	}));
      return row;
    });

  const auto & out_type = output.getValue();
  if (out_type == "csv")
    cout << to_string(format_string_csv(complete_rows(rows))) << endl;
  else
    cout << to_string(format_string(complete_rows(rows))) << endl;

  terminate_app();
}

void put_sample(const Correlation * corr_ptr,
		DynList<DynList<double>> & rows, DynList<string> & header,
		double temp, const Unit * yunit,
		DynList<double> & yc, double c, double m)
{
  const string name = corr_ptr->name + "_" + ::to_string((int(temp)));
  const auto & mode = mode_type.getValue();

  const Unit * y_out_unit = test_y_out_unit(yunit);

  if (mode == "single")
    {
      mutable_unit_convert(*yunit, yc, *y_out_unit);
      rows.append(move(yc));
      header.append(name);
    }
  else if (mode == "calibrated")
    {
      rows.append(yc.maps([c, m] (auto y) { return c + m*y; }));
      header.append(name + "_adjusted");
    }
  else
    {
      rows.append(unit_convert(*yunit,
			       yc.maps([c, m] (auto y) { return c + m*y; }),
			       *y_out_unit));
      mutable_unit_convert(*yunit, yc, *y_out_unit);
      rows.append(move(yc));
      header.append(name + "_adjusted");
      header.append(name);
    }
}

void put_pb_sample(const Correlation * corr_ptr,
		   DynList<DynList<double>> & rows, DynList<string> & header,
		   DynList<double> & pb, double c, double m)
{
  const string & name = corr_ptr->name;
  const auto & mode = mode_type.getValue();

  if (mode == "single")
    {
      rows.append(move(pb));
      header.append(name);
    }
  else if (mode == "calibrated")
    {
      rows.append(pb.maps([c, m] (auto y) { return c + m*y; }));
      header.append(name + "_adjusted");
    }
  else
    {
      rows.append(pb);
      rows.append(pb.maps([c, m] (auto y) { return c + m*y; }));
      header.append(name);
      header.append(name + "_adjusted");
    }
}

void put_uod_sample(const Correlation * corr_ptr,
		   DynList<DynList<double>> & rows, DynList<string> & header,
		   DynList<double> & uod, double c, double m)
{
  const string & name = corr_ptr->name;
  const auto & mode = mode_type.getValue();

  if (mode == "single")
    {
      rows.append(move(uod));
      header.append(name);
    }
  else if (mode == "calibrated")
    {
      rows.append(uod.maps([c, m] (auto y) { return c + m*y; }));
      header.append(name + "_adjusted");
    }
  else
    {
      rows.append(uod);
      rows.append(uod.maps([c, m] (auto y) { return c + m*y; }));
      header.append(name);
      header.append(name + "_adjusted");
    }
}

void process_local_calibration() 
{
  static auto print_R = [] (const DynList<DynList<string>> & l) -> string
    {
      struct Tmp
      {
	DynList<string> p;
	DynList<string> y;
	DynList<DynList<string>> yc;
      };
      auto cols = transpose(l).maps<DynList<string>>([] (auto & l)
        {
	  return l.filter([] (auto & s) { return s.size(); });
	}); // first contains column name

      //         temp    all other stuff related to temp value
      DynMapTree<string, Tmp> temps;
      for (auto it = cols.get_it(); it.has_curr(); it.next())
	{
	  DynList<string> & col = it.get_curr();
	  const string & header = col.get_first();
	  auto header_parts = split(header, '_');
	  const string prefix = header_parts[0];
	  const string type = header_parts[1];
	  if (prefix == "p")
	    temps[type].p = move(col);
	  else if (islower(prefix[0])) // is a Correlation name
	    temps[type].y = move(col);
	  else
	    temps[type].yc.append(move(col));
	}

      double xmin = numeric_limits<double>::max(), ymin = xmin;
      double xmax = 0, ymax = 0;

      ostringstream s;
      for (auto it = temps.get_it(); it.has_curr(); it.next())
	{
	  auto & p = it.get_curr();
	  const Tmp & tmp = p.second;
	  tmp.p.each(1, 1, [&xmin, &xmax] (auto v)
		     { xmin = min(xmin, atof(v)); xmax = max(xmax, atof(v)); });
	  tmp.y.each(1, 1, [&ymin, &ymax] (auto v)
		     { ymin = min(ymin, atof(v)); ymax = max(ymax, atof(v)); });
	  s << Rvector(tmp.p.get_first(), tmp.p.drop(1)) << endl
	    << Rvector(tmp.y.get_first(), tmp.y.drop(1)) << endl;
	  for (auto it = tmp.yc.get_it(); it.has_curr(); it.next())
	    {
	      const DynList<string> & col = it.get_curr();
	      col.each(1, 1, [&ymin, &ymax] (auto v)
		       { ymin = min(ymin, atof(v)); ymax = max(ymax, atof(v)); });
	      s << Rvector(col.get_first(), col.drop(1)) << endl;
	    }
	}

      s << "plot(0, type=\"n\", xlim=c(" << xmin << "," << xmax << "), ylim=c("
        << ymin << "," << ymax << "))" << endl;

      size_t pch = 1;
      size_t col = 1;
      DynList<string> colnames;
      DynList<int> colors;
      DynList<string> ltys;
      DynList<string> pchs;
      for (auto it = temps.get_it(); it.has_curr(); it.next())
	{
	  auto & pp = it.get_curr();
	  const Tmp & tmp = pp.second;
	  const auto & pname = tmp.p.get_first();
	  const string & name = tmp.y.get_first();
	  colnames.append("\"" + name + "\"");
	  colors.append(1);
	  ltys.append("NA");
	  pchs.append(to_string(pch));
	  s << "points(" << pname << "," << name << ",pch=" << pch++ << ")"
	    << endl;
	  for (auto it = tmp.yc.get_it(); it.has_curr(); it.next(), ++col)
	    {
	      const string & name = it.get_curr().get_first();
	      s << "lines(" << pname << "," << name << ",col=" << col << ")"
		<< endl;
	      colnames.append("\"" + name + "\"");
	      colors.append(col);
	      pchs.append("NA");
	      ltys.append("1");
	    }
	}
      s << Rvector("cnames", colnames) << endl
        << Rvector("cols", colors) << endl
        << Rvector("pchs", pchs) << endl
        << Rvector("ltys", ltys) << endl
        << "legend(\"topright\", legend=cnames, col=cols, pch=pchs, lty=ltys)"
        << endl;

      execute_R_script(s.str());

      return s.str();
    };

  static auto print_csv = [] (const DynList<DynList<string>> & l)
  {
    if (transpose_out.getValue())
      return to_string(format_string_csv(transpose(l)));
    else
      return to_string(format_string_csv(l));
  };

  static auto print_mat = [] (const DynList<DynList<string>> & l)
  {
    return to_string(format_string(l));
  };

  static AHDispatcher<string, string (*)(const DynList<DynList<string>>&)>
    print_dispatcher = { "R", print_R, "csv", print_csv, "mat", print_mat };

  if (not cal.isSet())
    return;

  auto & corr_list = cal.getValue().corr_list;

  const auto & mode = mode_type.getValue();
  
  DynList<pair<double, double>> comb; // coefficients c and m

  // First we must verify that each read correlation applies to the data set
  for (auto it = corr_list.get_it(); it.has_curr(); it.next())
    {
      auto corr_ptr = it.get_curr();
      if (mode != "single")
	{
	  auto stats = data.istats(corr_ptr);
	  comb.append(make_pair(CorrStat::c(stats.desc),
				CorrStat::m(stats.desc)));
	}
      else
	comb.append(make_pair(0, 1));
    }

  auto fst_corr = corr_list.get_first();
  auto target_name = fst_corr->target_name();
  auto fst = data.iapply(fst_corr);
  const Unit * punit = get<0>(fst);
  const Unit * yunit = get<1>(fst);

  // First correlation here because the pressure and lab values are
  // the same through all correlations
  DynList<DynList<double>> rows;
  DynList<string> header;

  for (auto it = get<2>(fst).get_it(); it.has_curr(); it.next())
    {
      PvtData::CorrDesc & vals = it.get_curr();
      const Unit * out_p_unit = test_p_out_unit(punit);
      mutable_unit_convert(*punit, vals.p, *out_p_unit);

      const Unit * out_y_unit = test_y_out_unit(yunit);
      mutable_unit_convert(*yunit, vals.y, *out_y_unit);
      
      const string t_tag = ::to_string((int) vals.t);
      header.append(build_dynlist<string>("p_" + t_tag,
					  target_name + "_" + t_tag));
      rows.append(build_dynlist<DynList<double>>(move(vals.p), move(vals.y)));
    }

  for (auto it = zip_it(corr_list, comb); it.has_curr(); it.next())
    {
      auto curr = it.get_curr();
      const Correlation * corr_ptr = get<0>(curr);
      auto vals = data.iapply(corr_ptr);
      const Unit * yunit = get<1>(vals);

      const auto & cm = get<1>(curr);
      const double & c = cm.first;
      const double & m = cm.second;
      for (auto it = get<2>(vals).get_it(); it.has_curr(); it.next())
	{
	  PvtData::CorrDesc & curr = it.get_curr();
	  put_sample(corr_ptr, rows, header, curr.t, yunit, curr.yc, c, m);
	}
    }

  auto row_str = rows.maps<DynList<string>>([] (auto & l)
    {
      return l.template maps<string>([] (auto v) { return to_str(v); });
    });

  DynList<DynList<string>> result =
    transpose(rows.maps<DynList<string>>([] (auto & l)
    {
      return l.template maps<string>([] (auto v) { return to_str(v); });
    }));
  result.insert(header);

  assert(equal_length(rows, header));

  cout << print_dispatcher.run(output.getValue(), result) << endl;
  terminate_app();
}

void process_pb_calibration() 
{
  static auto print_R = [] (const DynList<DynList<string>> & l) -> string
    {
      struct Tmp
      {
	string y;
	DynList<string> yc;
      };
      auto cols = transpose(l); // first contains column name
      double xmin = numeric_limits<double>::max(), ymin = xmin;
      double xmax = 0, ymax = 0;
      ostringstream s;
      for (auto it = cols.get_it(); it.has_curr(); it.next())
	{
	  const auto & p = it.get_curr();
	  p.for_each([] (auto &s) { cout << s << "-"; }); cout << endl;
	  const string & header = p.get_first();
	  if (header[0] == 't')
	    p.each(1, 1, [&xmin, &xmax] (auto v)
		   {
		     xmin = min(xmin, atof(v));
		     xmax = max(xmax, atof(v));
		   });
	  else
	    p.each(1, 1, [&ymin, &ymax] (auto v)
		   {
		     ymin = min(ymin, atof(v));
		     ymax = max(ymax, atof(v));
		   });
	  s << Rvector(p.get_first(), p.drop(1)) << endl;
	}

      s << "plot(0, type=\"n\", xlim=c(" << xmin << "," << xmax << "), ylim=c("
        << ymin << "," << ymax << "))" << endl
        << "points(t, pb)" << endl;

      size_t col = 1;
      DynList<string> colnames;
      DynList<int> colors;
      for (auto it = cols.get_it(2); it.has_curr(); it.next(), ++col)
	{
	  auto & p = it.get_curr();
	  const auto & pname = p.get_first();
	  s << "lines(t," << pname << ",col=" << col << ")" << endl;
	  colnames.append("\"" + pname + "\"");
	  colors.append(col);
	}
      s << Rvector("cnames", colnames) << endl
        << Rvector("cols", colors) << endl
        <<  "legend(\"topright\", legend=cnames, col=cols, lty=1)"
        << endl;

      execute_R_script(s.str());

      return s.str();
    };

  static auto print_csv = [] (const DynList<DynList<string>> & l)
  {
    if (transpose_out.getValue())
      return to_string(format_string_csv(transpose(l)));
    else
      return to_string(format_string_csv(l));
  };

  static auto print_mat = [] (const DynList<DynList<string>> & l)
  {
    return to_string(format_string(l));
  };

  static AHDispatcher<string, string (*)(const DynList<DynList<string>>&)>
    print_dispatcher = { "R", print_R, "csv", print_csv, "mat", print_mat };

  if (not pb_cal.isSet())
    return;

  auto & corr_list = pb_cal.getValue().corr_list;

  const auto & mode = mode_type.getValue();
  
  DynList<pair<double, double>> comb; // coefficients c and m

  // First we must verify that each read correlation applies to the data set
  for (auto it = corr_list.get_it(); it.has_curr(); it.next())
    {
      auto corr_ptr = it.get_curr();
      if (not data.can_be_applied(corr_ptr, relax_names_tbl,
				  ban.getValue().corr_list))
	ALEPHTHROW(CommandLineError,
		 corr_ptr->name + " does not apply to data set");
      
      if (mode != "single")
	{
	  const PvtData::PbStats stats = data.pbstats(corr_ptr);
	  comb.append(make_pair(CorrStat::c(stats.desc),
				CorrStat::m(stats.desc)));
	}
      else
	comb.append(make_pair(0, 1));
    }

  auto tp_tuples = data.tp_sets();
  auto vals = t_unzip(tp_tuples);
  DynList<DynList<double>> rows =
    build_dynlist<DynList<double>>(get<0>(vals), get<1>(vals));
  DynList<string> header = build_dynlist<string>("t", "pb");

  for (auto it = zip_it(corr_list, comb); it.has_curr(); it.next())
    {
      auto curr = it.get_curr();
      const Correlation * corr_ptr = get<0>(curr);
      const DynList<PvtData::PbDesc> vals = data.pbapply(corr_ptr);
      const auto & cm = get<1>(curr);
      const double & c = cm.first;
      const double & m = cm.second;
      DynList<double> pbvals = 
	vals.maps<double>([] (auto & d) { return d.pb_corr; });
      put_pb_sample(corr_ptr, rows, header, pbvals, c,  m);
    }

  DynList<DynList<string>> result =
    transpose(rows.maps<DynList<string>>([] (auto & l)
    {
      return l.template maps<string>([] (auto v) { return to_str(v); });
    }));
  result.insert(header);

  assert(equal_length(rows, header));

  cout << print_dispatcher.run(output.getValue(), result) << endl;
  terminate_app();
}

void process_uod_calibration() 
{
  static auto print_R = [] (const DynList<DynList<string>> & l) -> string
    {
      struct Tmp
      {
	string y;
	DynList<string> yc;
      };
      auto cols = transpose(l); // first contains column name
      double xmin = numeric_limits<double>::max(), ymin = xmin;
      double xmax = 0, ymax = 0;
      ostringstream s;
      for (auto it = cols.get_it(); it.has_curr(); it.next())
	{
	  const auto & p = it.get_curr();
	  const string & header = p.get_first();
	  if (header[0] == 't')
	    p.each(1, 1, [&xmin, &xmax] (auto v)
		   {
		     xmin = min(xmin, atof(v));
		     xmax = max(xmax, atof(v));
		   });
	  else
	    p.each(1, 1, [&ymin, &ymax] (auto v)
		   {
		     ymin = min(ymin, atof(v));
		     ymax = max(ymax, atof(v));
		   });
	  s << Rvector(p.get_first(), p.drop(1)) << endl;
	}

      s << "plot(0, type=\"n\", xlim=c(" << xmin << "," << xmax << "), ylim=c("
        << ymin << "," << ymax << "))" << endl
        << "points(t, uod)" << endl;

      size_t col = 1;
      DynList<string> colnames;
      DynList<int> colors;
      for (auto it = cols.get_it(2); it.has_curr(); it.next(), ++col)
	{
	  auto & p = it.get_curr();
	  const auto & pname = p.get_first();
	  s << "lines(t," << pname << ",col=" << col << ")" << endl;
	  colnames.append("\"" + pname + "\"");
	  colors.append(col);
	}
      s << Rvector("cnames", colnames) << endl
        << Rvector("cols", colors) << endl
        <<  "legend(\"topright\", legend=cnames, col=cols, lty=1)"
        << endl;

      execute_R_script(s.str());

      return s.str();
    };

  static auto print_csv = [] (const DynList<DynList<string>> & l)
  {
    if (transpose_out.getValue())
      return to_string(format_string_csv(transpose(l)));
    else
      return to_string(format_string_csv(l));
  };

  static auto print_mat = [] (const DynList<DynList<string>> & l)
  {
    return to_string(format_string(l));
  };

  static AHDispatcher<string, string (*)(const DynList<DynList<string>>&)>
    print_dispatcher = { "R", print_R, "csv", print_csv, "mat", print_mat };

  if (not uod_cal.isSet())
    return;

  auto corr_list = uod_cal.getValue().corr_list;

  const auto & mode = mode_type.getValue();
  
  DynList<pair<double, double>> comb; // coefficients c and m

  // First we must verify that each read correlation applies to the data set
  for (auto it = corr_list.get_it(); it.has_curr(); it.next())
    {
      auto corr_ptr = it.get_curr();
      if (not data.can_be_applied(corr_ptr, relax_names_tbl,
				  ban.getValue().corr_list))
	ALEPHTHROW(CommandLineError,
		 corr_ptr->name + " does not apply to data set");
      
      if (mode != "single")
	{
	  auto s = data.cm(corr_ptr);
	  comb.append(make_pair(s.first, s.second));
	}
      else
	comb.append(make_pair(0, 1));
    }

  auto tp_tuples = data.tp_sets();
  auto vals = t_unzip(tp_tuples);
  DynList<DynList<double>> rows =
    build_dynlist<DynList<double>>(get<0>(vals), get<2>(vals));
  DynList<string> header = build_dynlist<string>("t", "uod");

  for (auto it = zip_it(corr_list, comb); it.has_curr(); it.next())
    {
      auto curr = it.get_curr();
      const Correlation * corr_ptr = get<0>(curr);
      auto vals = data.uodapply(corr_ptr);
      auto svals = vals.maps<DynList<string>>([] (auto & d)
        {
	  return build_dynlist<string>(to_string(d.t), to_string(d.uod_lab),
				       to_string(d.uod_corr));
	});
      const auto & cm = get<1>(curr);
      const double & c = cm.first;
      const double & m = cm.second;
      DynList<double> uodvals =
	vals.maps<double>([] (auto & d) { return d.uod_corr; });
      put_uod_sample(corr_ptr, rows, header, uodvals, c,  m);
    }

  DynList<DynList<string>> result =
    transpose(rows.maps<DynList<string>>([] (auto & l)
    {
      return l.template maps<string>([] (auto v) { return to_str(v); });
    }));
  result.insert(header);

  assert(equal_length(rows, header));

  cout << print_dispatcher.run(output.getValue(), result) << endl;
  terminate_app();
}

void process_cplot()
{
  if (not cplot.getValue())
    return;
  
  cout << "./cplot --grid simple " << data.cplot_consts() << data.cplot_corrs()
       << " --zfactor ZfactorDranchukAK ";
  if (t.isSet())
    {
      const RangeDesc & val = t.getValue();
      cout << "--t \"" << val.min << " " << val.max << " " << val.n << "\" ";
    }
  if (p.isSet())
    {
      const RangeDesc & val = p.getValue();
      cout << "--p \"" << val.min << " " << val.max << " " << val.n << "\" ";
    }
  if (tarray.isSet())
    {
      cout << "--t_array \"";
      tarray.getValue().values.for_each([] (auto v) { cout << " " << v; });
      cout << "\" ";
    }
  if (parray.isSet())
    {
      cout << "--p_array \"";
      parray.getValue().values.for_each([] (auto v) { cout << " " << v; });
      cout << "\" ";
    }
  terminate_app();
}

string plot_cmd()
{
  ostringstream s;
  s << "./cplot --grid simple " << data.cplot_consts() << data.cplot_corrs()
    << " --zfactor ZfactorDranchukAK --t_array \""
    << join(data.get_temperatures().maps<string>([] (auto v)
						 { return to_string(v); }) , " ")
    << "\" ";
  if (exp_arg.getValue())
    s << "--p_array \"" << join(data.all_pressures(), " ") << "\"";
  else if (pbexp_arg.getValue())
    s << "--p_array \"" << join(data.all_pb(), " ") << "\"";
  else
    s << "--p \""
      << data.pmin() << " " << data.pmax() << " 100\"";
  return s.str();
}

void process_CPLOT()
{
  if (not Cplot.getValue())
    return;

  cout << plot_cmd() << endl;
  terminate_app();
}

void process_R()
{
  if (not R.isSet())
    return;

  const string plot = plot_cmd() + " > tmp.csv";
  system(plot.c_str());
  cout << plot << endl;

  const string plotr = "./plot-r -f tmp.csv -P " + R.getValue() + " -R";
  system(plotr.c_str());
}

void split_uo()
{
  DynList<const VectorDesc*> uo_vectors = data.search_vectors("uo");
  if (uo_vectors.is_empty())
    ALEPHTHROW(VarNameNotFound, "data does not contain uo");

  for (auto it = uo_vectors.get_it(); it.has_curr(); it.next())
    {
      auto uo_ptr = it.get_curr();
      auto p = uo_ptr->split_uo();
      data.add_vector(p.first);
      data.add_vector(p.second);
    }
}

void test_load_file()
{
  if (not file.isSet())
    return;
  
  ifstream in(file.getValue());
  if (in)
    {
      try
	{
	  data = PvtData(in);
	}
      catch (exception & e)
	{
	  if (not save.getValue())
	    ALEPHTHROW(InvalidJson, "reading json: " + string(e.what()));
	}
    }
}

void set_correlations()
{
  set_pb_corr();
  set_rs_corr();
  set_bob_corr();
  set_boa_corr();
  set_coa_corr();
  set_uod_corr();
  set_uob_corr();
  set_uoa_corr();
}    

int main(int argc, char *argv[])
{
  UnitsInstancer::init();
  cmd.parse(argc, argv);

  test_load_file();

  build_pvt_data();
  remove_consts();
  remove_properties();

  if (split_uo_arg.getValue())
    split_uo();

  set_correlations();
  set_relax_names();
  input_data();

  if (not data.defined())
    ALEPHTHROW(CommandLineError, "data is not defined");

  process_Auto();
  process_print_data();
  process_Print_data();

  if (save.getValue())
    {
      if (not file.isSet())
	ALEPHTHROW(InvalidTargetName, "json name not defined (--file)");
      ofstream out(file.getValue());
      out << data.to_json().dump(2);
      if (out.bad())
	ALEPHTHROW(CommandLineError, "cannot write to " + file.getValue() +
		 " file");
      terminate_app();
    }

  process_list();
  process_match();
  process_auto();
  process_apply();
  process_napply();
  process_local_calibration();
  process_pb_calibration();
  process_uod_calibration();
  process_R();
  process_CPLOT();
  process_cplot();

  cout << "Not given action" << endl;
}
