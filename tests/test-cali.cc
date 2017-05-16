
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
      ZENTHROW(InvalidTargetUnit, arg.unit_ptr->name +			\
	       " is not an unit for " + pq_name::get_instance().name);	\
  }

static const DynSetTree<string> valid_targets =
  { "rs", "bob", "boa", "uob", "uoa", "coa", "zfactor", "bo", "uo" };

// Defines a input of form
// "property-name property-unit t tunit pb punit p-list property-list"
struct ValuesArg
{
  static AHDispatcher<string, void (*)(const ValuesArg&)> check_dispatcher;

  string target_name; // property name
  const Unit * unit_ptr = nullptr; // unit for property

  double t; // temperature
  const Unit * tunit_ptr = nullptr; // temperature unit

  double uod = -1; // uod
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
  Define_Check_Property(zfactor, CompressibilityFactor);

  void validate_property()
  {
    if (not valid_targets.contains(target_name))
      ZENTHROW(InvalidProperty, "target name " + target_name +
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
	  ZENTHROW(CommandLineError, data + " is not a double");
	vals.append(atof(data));
      }

    if ((n % 3) != 0)
      ZENTHROW(CommandLineError,
	       "In input of coa values: number of values is not multiple of 3");
    
    const size_t dim = n/3;
    DynList<double> pmin, pmax;
    each(dim, [&vals, &pmin] () { pmin.append(vals.remove_first()); });
    each(dim, [&vals, &pmax] () { pmax.append(vals.remove_first()); });
    if (not (zip_all([] (auto t) { return get<0>(t) < get<1>(t); }, pmin, pmax)
        or zip_all([] (auto t) { return get<0>(t) > get<1>(t); }, pmin, pmax)))
      ZENTHROW(CommandLineError,
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
      ZENTHROW(CommandLineError, str + " does not contain target name");

    const bool compound_target = target_name == "bo" or target_name == "uo";

    validate_property();
    if (not compound_target and not Correlation::array().exists([this] (auto ptr)
        { return ptr->target_name() == target_name; }))
      ZENTHROW(CommandLineError, target_name + " is invalid as target name");

    { // read property unit
      string property_unit;
      if (not (iss >> property_unit))
	ZENTHROW(CommandLineError, "cannot read property unit");
      unit_ptr = Unit::search(property_unit);
      if (unit_ptr == nullptr)
	ZENTHROW(CommandLineError, property_unit + " unit for " + target_name +
		 " property not found");
    }

    check_dispatcher.run(target_name, *this);

     // read temperature unit
    string unit_name;
    if (not (iss >> unit_name))
      ZENTHROW(CommandLineError, "cannot read temperature unit");
    tunit_ptr = Unit::search(unit_name);
    if (tunit_ptr == nullptr)
      ZENTHROW(CommandLineError, unit_name + " for temperature not found");
    if (&tunit_ptr->physical_quantity != &Temperature::get_instance())
      ZENTHROW(CommandLineError, unit_name + " is not a temperature unit");

    // read temperature value
    string data;
    if (not (iss >> data))
      ZENTHROW(CommandLineError, "temperature value not found");
    if (not is_double(data))
      ZENTHROW(CommandLineError, "temperature value " + data +
	       " is not a double");
    t = atof(data);

    // read pressure unit
    if (not (iss >> unit_name))
      ZENTHROW(CommandLineError, str + " does not contain unit name");
    punit_ptr = Unit::search(unit_name);
    if (punit_ptr == nullptr)
      ZENTHROW(CommandLineError, "unit " + unit_name + " for pressure not found");
    if (&punit_ptr->physical_quantity != &Pressure::get_instance())
      ZENTHROW(CommandLineError, unit_name + " is not for pressure");

    if (not compound_target)
      {
	// read pb value
	if (not (iss >> data))
	  ZENTHROW(CommandLineError, "pb value not found");
	if (not is_double(data))
	  ZENTHROW(CommandLineError, "pb value " + data + " is not a double");
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
	  ZENTHROW(CommandLineError, data + " is not a double");
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

    if (not compound_target and not
	p.exists([this] (auto v) { return v == pb; }))
      ZENTHROW(CommandLineError, "pb value " + to_string(pb) +
	       " not found in pressures array");

    for (size_t i = 0; i < n/2; ++i, it.next())
      values.append(it.get_curr());

    if (p_reversed)
      values = values.rev();

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
			    "coa", ValuesArg::check_coa,
			    "zfactor", ValuesArg::check_zfactor);

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
      ZENTHROW(CommandLineError, "Cannot read target name property");
    if (not valid_targets.contains(target_name))
      ZENTHROW(CommandLineError, target_name +  " is not a valid target name");

    if (not (iss >> src_name))
      ZENTHROW(CommandLineError, "Cannot read source name property");
    if (not valid_targets.contains(src_name))
      ZENTHROW(CommandLineError, src_name +  " is not a valid source name");

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

ValueArg<ArrayDesc> tarray = { "", "t-array", "t array", false, ArrayDesc(),
			       "list-t-values", cmd };

ValueArg<ArrayDesc> parray = { "", "p-array", "p array", false, ArrayDesc(),
			       "list-p-values", cmd };

// Unit change specification. Suitable for any parameter
MultiArg<ArgUnit> unit = { "", "unit", "change unit of input data", false,
			   "unit \"par-name unit\"", cmd };

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

SwitchArg split_bo_arg = { "", "split-bo", "split bo vector", cmd };
SwitchArg split_uo_arg = { "", "split-uo", "split uo vector", cmd };
SwitchArg save = { "", "save", "save data to json", cmd };

MultiArg<RmProperty> rm_property =
  { "", "rm-property", "remove property t", false, "remove \"property t\"", cmd };

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
	auto s = data.stats(corr_ptr);					\
	data.c_##NAME = CorrStat::c(get<3>(s));				\
	data.m_##NAME = CorrStat::m(get<3>(s));				\
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
    ZENTHROW(CommandLineError, "target name " + in.target_name +
	     " not found in data set");

  DynList<const VectorDesc*> tgt_vectors = data.search_vectors(in.target_name);
  DynSetTree<double> temps = tgt_vectors.maps<double>([] (auto ptr)
    {
      return ptr->t;
    });

  auto pars = data.get_corr(in.target_name);
  const Correlation * corr_ptr = get<0>(pars);
  if (corr_ptr == nullptr)
    ZENTHROW(CommandLineError, "Correlation for target " + in.target_name +
	     " has not been set");
      
  const double & c = get<1>(pars);
  const double & m = get<2>(pars);

  for (auto it = src_vectors.get_it(); it.has_curr(); it.next())
    {
      auto src_ptr = it.get_curr();
      if (temps.has(src_ptr->t))
	continue;

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

void process_print_data()
{
  if (not print.getValue())
    return;
  if (json.getValue())
    cout << data.to_json().dump(2) << endl;
  else
    cout << data << endl;
  exit(0);
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
  exit(0);
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
  exit(0);
}

void process_match()
{
  if (not match.isSet())
    return;
  print_correlations(data.matches_with_pars(match.getValue()).
		     maps<DynList<string>>([] (auto p)
					   { return p->to_dynlist(); }));
  exit(0);
}

using T = PvtData::T;

# define Define_Cmp(name)						\
  auto cmp_##name = [] (const T & d1, const T & d2)			\
  {									\
    return CorrStat::name(get<3>(d1)) < CorrStat::name(get<3>(d2));	\
  }

# define Define_1_Cmp(name)						\
  auto cmp_##name = [] (const T & d1, const T & d2)			\
  {									\
    return abs(1 - CorrStat::name(get<3>(d1))) <			\
    abs(1 - CorrStat::name(get<3>(d2)));				\
  }

Define_1_Cmp(r2);
Define_Cmp(mse);
Define_Cmp(sumsq);
Define_Cmp(sigma);
Define_1_Cmp(m);

auto cmp_c = [] (const T & d1, const T & d2)
{
  return abs(CorrStat::c(get<3>(d1))) < abs(CorrStat::c(get<3>(d2)));
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
  
  auto corr_list = data.can_be_applied(property_name, relax_names_tbl);

  DynList<T> stats = Aleph::sort(corr_list.maps<T>([] (auto corr_ptr)
    {
      return data.stats(corr_ptr);
    }), cmp[::sort.getValue()]);

  DynList<DynList<string>> rows = stats.maps<DynList<string>>([] (auto & t)
    {
      DynList<string> ret = build_dynlist<string>(get<0>(t)->name);
      auto stats = get<4>(t) ? CorrStat::desc_to_dynlist(get<3>(t)) :
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

  exit(0);
}

void process_napply()
{
  if (not napply.isSet())
    return;
  
  auto property_name = napply.getValue();
  if (not valid_targets.contains(property_name) and property_name != "pb" and
      property_name != "uod")
    ZENTHROW(CommandLineError, "target name " + property_name + " is not valid");

  auto missing_list = data.list_restrictions(property_name, relax_names_tbl);

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

  exit(0);
}

void put_sample(const Correlation * corr_ptr,
		DynList<DynList<double>> & rows, DynList<string> & header,
		double temp,
		DynList<double> & yc, double c, double m)
{
  const string name = corr_ptr->name + "_" + ::to_string((int(temp)));
  const auto & mode = mode_type.getValue();

  if (mode == "single")
    {
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
      rows.append(yc.maps([c, m] (auto y) { return c + m*y; }));
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
      rows.append(pb.maps([c, m] (auto y) { return c + m*y; }));
      rows.append(move(pb));
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
        <<  "legend(\"topright\", legend=cnames, col=cols, pch=pchs, lty=ltys)"
        << endl;

      return s.str();
    };

  static auto print_csv = [] (const DynList<DynList<string>> & l)
  {
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
	  comb.append(make_pair(CorrStat::c(get<3>(stats)),
				CorrStat::m(get<3>(stats))));
	}
      else
	comb.append(make_pair(0, 1));
    }

  auto fst_corr = corr_list.get_first();
  auto target_name = fst_corr->target_name();
  auto fst = data.iapply(fst_corr);

  // First correlation here because the pressure and lab values are
  // the same through all correlations
  DynList<DynList<double>> rows;
  DynList<string> header;

  for (auto it = get<2>(fst).get_it(); it.has_curr(); it.next())
    {
      PvtData::TYPE & vals = it.get_curr();
      const string t_tag = ::to_string((int) get<0>(vals));
      header.append(build_dynlist<string>("p_" + t_tag,
					  target_name + "_" + t_tag));
      rows.append(build_dynlist<DynList<double>>(get<5>(vals), get<6>(vals)));
    }

  for (auto it = zip_it(corr_list, comb); it.has_curr(); it.next())
    {
      auto curr = it.get_curr();
      const Correlation * corr_ptr = get<0>(curr);
      auto vals = data.iapply(corr_ptr);
      const auto & cm = get<1>(curr);
      const double & c = cm.first;
      const double & m = cm.second;
      for (auto it = get<2>(vals).get_it(); it.has_curr(); it.next())
	{
	  auto & curr = it.get_curr();
	  const double & temp = get<0>(curr);
	  DynList<double> & yc = get<7>(curr);
	  put_sample(corr_ptr, rows, header, temp, yc, c, m);
	}
    }
  DynList<DynList<string>> result =
    transpose(rows.maps<DynList<string>>([] (auto & l)
    {
      return l.template maps<string>([] (auto v) { return to_str(v); });
    }));
  result.insert(header);

  assert(equal_length(rows, header));

  cout << print_dispatcher.run(output.getValue(), result) << endl;
  exit(0);
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

      return s.str();
    };

  static auto print_csv = [] (const DynList<DynList<string>> & l)
  {
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
      if (not data.can_be_applied(corr_ptr))
	ZENTHROW(CommandLineError,
		 corr_ptr->name + " does not apply to data set");
      
      if (mode != "single")
	{
	  auto stats = data.pbstats(corr_ptr);
	  comb.append(make_pair(CorrStat::c(get<3>(stats)),
				CorrStat::m(get<3>(stats))));
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
      auto vals = data.pbapply(corr_ptr);
      const auto & cm = get<1>(curr);
      const double & c = cm.first;
      const double & m = cm.second;
      DynList<double> pbvals =
	vals.maps<double>([] (auto t) { return get<1>(t); });
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
  exit(0);
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

      return s.str();
    };

  static auto print_csv = [] (const DynList<DynList<string>> & l)
  {
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
      if (not data.can_be_applied(corr_ptr))
	ZENTHROW(CommandLineError,
		 corr_ptr->name + " does not apply to data set");
      
      if (mode != "single")
	{
	  auto s = data.cm(corr_ptr);
	  comb.append(make_pair(s.first, s.second));
	  cout << "c = " << s.first << " m = " << s.second << endl;
	}
      else
	comb.append(make_pair(0, 1));
    }

  auto tp_tuples = data.tp_sets();
  auto vals = t_unzip(tp_tuples);
  DynList<DynList<double>> rows =
    build_dynlist<DynList<double>>(get<0>(vals), get<1>(vals));
  DynList<string> header = build_dynlist<string>("t", "uod");

  for (auto it = zip_it(corr_list, comb); it.has_curr(); it.next())
    {
      auto curr = it.get_curr();
      const Correlation * corr_ptr = get<0>(curr);
      auto vals = data.uodapply(corr_ptr);
      const auto & cm = get<1>(curr);
      const double & c = cm.first;
      const double & m = cm.second;
      DynList<double> pbvals =
	vals.maps<double>([] (auto t) { return get<1>(t); });
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
  exit(0);
}

void process_cplot()
{
  if (not cplot.getValue())
    return;
  if (not data.are_all_correlations_defined())
    {
      ostringstream s;
      s << "cannot generate cplot because the following correlations are "
	"not defined:";
      data.missing_correlations().for_each([&s] (auto & name)
					   {
					     s << "  " << name;
					   });
      ZENTHROW(CommandLineError, s.str());
    }
  cout << "./cplot --grid simple " << data.cplot_consts() << data.cplot_corrs()
       << " ";
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
  exit(0);      
}

void split_bo()
{
  auto bo_vectors = data.search_vectors("bo");
  if (bo_vectors.is_empty())
    ZENTHROW(VarNameNotFound, "data does not contain bo");

  for (auto it = bo_vectors.get_it(); it.has_curr(); it.next())
    {
      auto bo_ptr = it.get_curr();
      auto p = bo_ptr->split_bo();
      data.add_vector(p.first);
      data.add_vector(p.second);
    }
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

  if (split_bo_arg.getValue())
    split_bo();

  if (split_uo_arg.getValue())
    split_uo();

  set_correlations();
  set_relax_names();
  input_data();

  if (not data.defined())
    ZENTHROW(CommandLineError, "data is not defined");

  process_print_data();
  process_Print_data();

  if (save.getValue())
    {
      if (not file.isSet())
	ZENTHROW(InvalidTargetName, "json name not defined (--file)");
      ofstream out(file.getValue());
      out << data.to_json().dump(2);
      exit(0);
    }

  process_list();
  process_match();
  process_apply();
  process_napply();
  process_local_calibration();
  process_pb_calibration();
  process_uod_calibration();
  process_cplot();

  cout << "Not given action" << endl;
}
