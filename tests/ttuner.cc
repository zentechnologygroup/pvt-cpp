
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

  void read_units_and_temp(istringstream & iss, const Unit & y_ref_unit)
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
  }

  void read_header(istringstream & iss, const Unit & y_ref_unit)
  {
    read_units_and_temp(iss, y_ref_unit);
    string data;
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
    read_units_and_temp(iss, CP::get_instance()); 
    read_values(iss);
    separate();
    return *this;
  }
  void read_pb_uod_and_uobp(istringstream & iss)
  {
    string data;
    if (not (iss >> data))
      ZENTHROW(CommandLineError, "cannot read pb value");
    if (not is_double(data))
      ZENTHROW(CommandLineError, "pb is not numeric");
    pb = atof(data);
    
    if (not (iss >> data))
      ZENTHROW(CommandLineError, "cannot read uod value");
    if (not is_double(data))
      ZENTHROW(CommandLineError, "uod is not numeric");
    uod = atof(data);
    
    if (not (iss >> data))
      ZENTHROW(CommandLineError, "cannot read uobp value");
    if (not is_double(data))
      ZENTHROW(CommandLineError, "uobp is not numeric");
    uobp = atof(data);
  }
  void read_zone(const string & str)
  {
    istringstream iss(str);
    read_units_and_temp(iss, CP::get_instance());
    read_pb_uod_and_uobp(iss);
    read_values(iss);
    separate();
  }
};

struct Uob : public Uo
{
  Uob() : Uo("uob") {}
  Uob & operator = (const string & str)
  {
    read_zone(str);
    return *this;
  }
};

struct Uoa : public Uo
{
  Uoa() : Uo("uoa") {}
  Uoa & operator = (const string & str)
  {
    read_zone(str);
    return *this;
  }  
};

struct Uosplit : public Property
{
  Uosplit() : Property("uo") {}
  Uosplit & operator = (const string & str)
  {
    istringstream iss(str);
    read_units_and_temp(iss, CP::get_instance());
    return *this;
  }  
};

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
	  ZENTHROW(CorrelationNotFound, "Correlation" + data + " not found");

	if (corr_list.has(corr_ptr))
	  ZENTHROW(DuplicatedCorrelationName, "Correlation " + data +
		   " is already defined in ban list");

	corr_list.append(corr_ptr);
      }

    return *this;
  }
};

// allowed parameter names (they are values ​​or ranges, but they are
// not correlations). This table is used for validating change of
// units
static DynSetTree<string> const_name_tbl =
  { "api", "rsb", "yg", "tsep", "psep", "h2s", "co2", "n2", "nacl" };

static const DynSetTree<string> valid_targets =
  { "rs", "bob", "boa", "uob", "uoa", "coa", "bo", "uo" };

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

struct RmProperty
{
  string yname = "No-Defined";
  double t = -1;

  RmProperty() {}

  RmProperty & operator = (const string & str)
  {
    istringstream iss(str);
    if (not (iss >> yname))
      ZENTHROW(CommandLineError, "Cannot read target name property");
    if (not valid_targets.contains(yname))
      ZENTHROW(InvalidProperty, yname + " is not a valid property");
    string data;
    if (not (iss >> data))
      ZENTHROW(CommandLineError, "in rm option: cannot read temperature value");
    if (not is_double(data))
      ZENTHROW(CommandLineError, "value " + data + " is not a double");
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
	  ZENTHROW(CommandLineError, "correlation " + name + " not found");
	corr_list.append(corr_ptr);
      }

    if (corr_list.is_empty())
      ZENTHROW(CommandLineError, "Not correlation specified for calibration");

    name = corr_list.get_first()->target_name();
    if (not corr_list.all([&name] (auto p) { return p->target_name() == name; }))
      ZENTHROW(CommandLineError,
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
  template<> struct ArgTraits<Rs> { typedef StringLike ValueCategory; };
  template<> struct ArgTraits<Coa> { typedef StringLike ValueCategory; };
  template<> struct ArgTraits<Bob> { typedef StringLike ValueCategory; };
  template<> struct ArgTraits<Boa> { typedef StringLike ValueCategory; };
  template<> struct ArgTraits<Uob> { typedef StringLike ValueCategory; };
  template<> struct ArgTraits<Uoa> { typedef StringLike ValueCategory; };
  template<> struct ArgTraits<Uosplit> { typedef StringLike ValueCategory; };
  template<> struct ArgTraits<BanList> { typedef StringLike ValueCategory; };
  template<> struct ArgTraits<ArgUnit> { typedef StringLike ValueCategory; };
  template<> struct ArgTraits<RmProperty> { typedef StringLike ValueCategory; };
  template<> struct ArgTraits<CorrArgs> { typedef StringLike ValueCategory; };
  template<> struct ArgTraits<RangeDesc> { typedef StringLike ValueCategory; };
  template<> struct ArgTraits<ArrayDesc> { typedef StringLike ValueCategory; };
}

// Vector paramaters
MultiArg<Rs> rs_arg = { "", "rs_data", "rs", false,
			"tunit punit rs_unit t pb p-vals rs-vals", cmd};

MultiArg<Coa> coa_arg = { "", "coa_data", "coa", false,
			  "tunit punit rs_unit t pb p-vals coa-vals", cmd};

MultiArg<Bob> bob_arg = { "", "bob_data", "bob", false,
			  "tunit punit bob_unit t pb bobp p-vals bob-vals", cmd};

MultiArg<Boa> boa_arg = { "", "boa_data", "boa", false,
			  "tunit punit boa_unit t pb bobp p-vals boa-vals", cmd};

MultiArg<Uob> uob_arg = { "", "uob_data", "uob", false,
			  "tunit punit boa_unit t pb uobp p-vals uoa-vals", cmd};

MultiArg<Uoa> uoa_arg = { "", "uoa_data", "uoa", false,
			  "tunit punit uoa_unit t pb uobp p-vals uoa-vals", cmd};

MultiArg<Uosplit> uo_arg = { "", "uo_data", "uo", false,
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

vector<string> relax_names =
  {
    "api", "yo", "rsb", "yg", "tsep", "psep", "h2s", "co2", "n2", "nacl", "pb",
    "uod", "rs", "bobp", "uobp", "coa", "bob", "boa", "uob", "uoa"
  };
ValuesConstraint<string> allowed_relax_names = relax_names;
MultiArg<string> relax_pars =
  { "", "relax", "relax parameter range application check", false,
    &allowed_relax_names, cmd };

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

# define READ_PROPERTY_DEF(name)					\
  for (auto name : name##_arg.getValue())				\
    data.add_vector(name.t, name.pb, name.bobp, name.uod, name.uobp,	\
		    name.p, *name.punit, #name, name.y, *name.yunit);

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

  READ_PROPERTY_DEF(rs);
  READ_PROPERTY_DEF(bob);
  READ_PROPERTY_DEF(coa);
  READ_PROPERTY_DEF(boa);
  READ_PROPERTY_DEF(uob);
  READ_PROPERTY_DEF(uoa);
}

SwitchArg eol { "", "eol", "add to output an end of line", cmd };
SwitchArg print = { "", "print", "print stored data", cmd };
SwitchArg json = { "", "json", "generate json of data", cmd };
SwitchArg split_uo_arg = { "", "split-uo", "split uo vectors", cmd };
SwitchArg save = { "", "save", "save data to json", cmd };
ValueArg<string> file = { "f", "file", "load json", false, "", "load json", cmd };
ValueArg<string> Print = { "P", "Print", "print stored data", false, "",
			   "constants|correlations|property-name", cmd };
SwitchArg transpose_out = { "", "transpose", "transpose output", cmd };

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
      ZENTHROW(CommandLineError, "options " + NAME##_corr_arg.getName() + \
	       NAME##_cal_corr_arg.getName() + " cannot be used together"); \
    const bool calibrated = NAME##_cal_corr_arg.isSet();		\
    const string corr_name = calibrated ? NAME##_cal_corr_arg.getValue() : \
      NAME##_corr_arg.getValue();					\
    auto corr_ptr = Correlation::search_by_name(corr_name);		\
    if (corr_ptr == nullptr)						\
      ZENTHROW(CommandLineError, "correlation " + corr_name + " not found"); \
    if (corr_ptr->target_name() != #NAME)				\
      ZENTHROW(CommandLineError, "correlation " + corr_ptr->name +	\
	       " is not for " #NAME);					\
    data.NAME##_corr = corr_ptr;					\
    if (calibrated)							\
      {									\
	const auto s = data.NAME##_stats(corr_ptr);			\
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

ValueArg<BanList> ban = { "b", "ban",
			  "ban correlation list for automatic calibration",
			  false, BanList(), "banned correlation list", cmd };
MultiArg<RmProperty> rm_property =
  { "", "rm-property", "remove property t", false, "\"property-tag t\"", cmd };
vector<string> valid_consts = to_vector(const_name_tbl);
ValuesConstraint<string> allowed_consts = valid_consts;
MultiArg<string> rm_const = { "", "rm-const", "remove const", false,
			      &allowed_consts, cmd };

ValueArg<string> list_corr = { "l", "list", "list correlations", false, "",
			       "property-name", cmd };

ValueArg<string> match = { "m", "match", "print matching correlations", false, "",
			   "property-name", cmd };

ValueArg<string> apply = { "a", "apply", "print applying correlations", false, "",
			   "property-name", cmd };

ValueArg<string> napply =
  { "n", "napply", "print non applying correlations and reasons", false, "",
    "napply property", cmd };

ValueArg<CorrArgs> cal = { "", "lcal", "calibrate correlations", false,
			   CorrArgs(), "calibrate correlation-list", cmd };

void terminate_app()
{
  if (eol.getValue())
    cout << endl;
  exit(0);
}
void process_print_data()
{
  if (not print.getValue())
    return;
  if (json.getValue())
    cout << data.to_json().dump(2) << endl;
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
	    ZENTHROW(InvalidJson, "reading json: " + string(e.what()));
	}
    }
}

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

void split_uo()
{
  DynList<const VectorDesc*> uo_vectors = data.search_vectors("uo");
  if (uo_vectors.is_empty())
    ZENTHROW(VarNameNotFound, "data does not contain uo");

  for (auto it = uo_vectors.get_it(); it.has_curr(); it.next())
    {
      auto uo_ptr = it.get_curr();
      auto p = uo_ptr->split_uo();
      data.add_vector(p.first);
      data.add_vector(p.second);
    }
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
    ZENTHROW(CommandLineError, "target name " + property_name + " is not valid");
  
  auto corr_list = data.can_be_applied(property_name, relax_names_tbl,
				       ban.getValue().corr_list);

  DynList<T> stats = Aleph::sort(corr_list.maps<T>([] (auto corr_ptr)
    {
      return data.apply(corr_ptr);
    }), cmp[::sort.getValue()]);

  DynList<DynList<string>> rows = stats.maps<DynList<string>>([] (auto & t)
    {
      DynList<string> ret = build_dynlist<string>(t.corr_ptr->name);
      auto stats = t.is_valid() ? CorrStat::desc_to_dynlist(t.desc) :
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

void process_napply()
{
  if (not napply.isSet())
    return;
  
  auto property_name = napply.getValue();
  if (not valid_targets.contains(property_name) and property_name != "pb" and
      property_name != "uod")
    ZENTHROW(CommandLineError, "target name " + property_name + " is not valid");

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
	  return l.filter([] (auto & s) { return s.size() > 0; });
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

  auto stats = corr_list.maps<PvtData::StatsDesc>([] (auto ptr)
						  { return data.apply(ptr); });

  const string target_name = corr_list.get_first()->target_name();
  DynList<string> header = { "t", target_name };
  auto fst_s = stats.get_first();
  DynList<DynList<double>> mat = { fst_s.t, fst_s.ylab };
  if (not (target_name == "pb" or target_name == "uod"))
    {
      header.append("p");
      mat.append(fst_s.p);
    }  
  
  static AHDispatcher<string,
		      DynList<DynList<double>> (*)(const PvtData::StatsDesc&,
						   DynList<string>*)>
    mode_dispatch =
    {
      "single", [] (const PvtData::StatsDesc& s, DynList<string> * header)
      {
	header->append(s.corr_ptr->name);
	return s.simple_matrix();
      },
      "calibrated", [] (const PvtData::StatsDesc& s, DynList<string> * header)
      {
	header->append(s.corr_ptr->name + "_cal");
	return s.cal_matrix();
      },
      "both", [] (const PvtData::StatsDesc& s, DynList<string> * header)
      {
	header->append(s.corr_ptr->name);
	header->append(s.corr_ptr->name + "_cal");
	return s.both_matrix();
      }
    };

  const auto & mode = mode_type.getValue();
  for (auto it = stats.get_it(); it.has_curr(); it.next())
    mat.append(mode_dispatch.run(mode, it.get_curr(), &header));

  auto rows = mat.maps<DynList<string>>([] (auto & row)
    {
      return row.template maps<string>([] (auto v) { return to_string(v); });
    });
  auto result = transpose(rows);
  result.insert(header);

  cout << print_dispatcher.run(output.getValue(), result) << endl;

  terminate_app();
}

int main(int argc, char *argv[])
{
  cmd.parse(argc, argv);
  try
    {
      test_load_file();
      build_pvt_data();
      remove_consts();
      remove_properties();

      if (split_uo_arg.getValue())
	split_uo();

      set_correlations();
      set_relax_names();
      //input_data();

      if (not data.defined())
	error_msg("data is not defined");

      //process_Auto();
      process_print_data();
      process_Print_data();

      if (save.getValue())
	{
	  if (not file.isSet())
	    ZENTHROW(InvalidTargetName, "json name not defined (--file)");
	  ofstream out(file.getValue());
	  out << data.to_json().dump(2);
	  if (out.bad())
	    ZENTHROW(CommandLineError, "cannot write to " + file.getValue() +
		     " file");
	  terminate_app();
	}

      process_list();
      process_match();
      // process_auto();
      process_apply();
      process_napply();
      process_local_calibration();
      // process_pb_calibration();
      // process_uod_calibration();
      // process_R();
      // process_CPLOT();
      // process_cplot();

      cout << "Not given action" << endl;
    }
  catch (exception & e)
    {
      cout << e.what() << endl;
    }
}
