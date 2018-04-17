
# include <istream>

# include <ah-dispatcher.H>
# include <ah-stl-utils.H>

# include <tclap/CmdLine.h>

# include <metadata/pvt-data.H>

using namespace std;
using namespace TCLAP;
using namespace Aleph;

PvtData data;

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
      ALEPHTHROW(CommandLineError, str + " does not contain target name");
    if (not Correlation::array().exists([this] (auto ptr)
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

    // read temperature value
    string data;
    if (not (iss >> data))
      ALEPHTHROW(CommandLineError, "temperature value not found");
    if (not is_double(data))
      ALEPHTHROW(CommandLineError, "temperature value " + data +
	       " is not a double");
    t = atof(data);

     // read temperature unit
    string unit_name;
    if (not (iss >> unit_name))
      ALEPHTHROW(CommandLineError, "cannot read temperature unit");
    tunit_ptr = Unit::search(unit_name);
    if (tunit_ptr == nullptr)
      ALEPHTHROW(CommandLineError, unit_name + " for temperature not found");
    if (&tunit_ptr->physical_quantity != &Temperature::get_instance())
      ALEPHTHROW(CommandLineError, unit_name + " is not a temperature unit");

    // read pb value
    if (not (iss >> data))
      ALEPHTHROW(CommandLineError, "pb value not found");
    if (not is_double(data))
      ALEPHTHROW(CommandLineError, "pb value " + data + " is not a double");
    pb = atof(data);

    // read pressure unit
    if (not (iss >> unit_name))
      ALEPHTHROW(CommandLineError, str + " does not contain unit name");
    punit_ptr = Unit::search(unit_name);
    if (punit_ptr == nullptr)
      ALEPHTHROW(CommandLineError, unit_name + " for pressure not found");
    if (&punit_ptr->physical_quantity != &Pressure::get_instance())
      ALEPHTHROW(CommandLineError, unit_name + " is not for pressure");

    DynList<double> vals;
    size_t n = 0;
    for (; iss >> data; ++n)
      {
	if (not is_double(data))
	  ALEPHTHROW(CommandLineError, data + " is not a double");
	vals.append(atof(data));
      }

    if (n % 2 != 0)
      ALEPHTHROW(CommandLineError, "number of values is not even");

    auto it = vals.get_it();
    for (size_t i = 0; i < n/2; ++i, it.next())
      p.append(it.get_curr());

    if (not p.exists([this] (auto v) { return v == pb; }))
      ALEPHTHROW(CommandLineError, "pb value " + to_string(pb) +
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
  { "api", "rsb", "yg", "tsep", "psep", "h2s", "co2", "n2", "nacl", "uod" };

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

struct ActionType
{
  static const DynSetTree<string> valid_actions;
  static const AHDispatcher<string, void (*)(ActionType*, istringstream*)>
  dispatcher;
  string type;
  string property_name;
  DynList<const Correlation*> corr_list;
  DynList<pair<const Correlation*, const Correlation*>> corr_pair_list;

  ActionType() {}

  ActionType & operator = (const string & str)
  {
    istringstream iss(str);

    if (not (iss >> type))
      ALEPHTHROW(CommandLineError, "cannot read action type");
    if (not valid_actions.contains(type))
      {
	ostringstream s;
	s << type << " is not a valid action (must be";
	valid_actions.for_each([&s] (auto & a) { s << " " << a; });
	s << ")";
	ALEPHTHROW(CommandLineError, s.str());
      }

    dispatcher.run(type, this, &iss);

    return *this;
  }

  static void read_property_name(ActionType * action, istringstream * iss)
  {
    if (not (*iss >> action->property_name))
      ALEPHTHROW(CommandLineError, "cannot read property name");
  }

  static void read_local_calibration(ActionType * action, istringstream * iss)
  {
    string corr_name;
    while (*iss >> corr_name)
      {
	auto corr_ptr = Correlation::search_by_name(corr_name);
	if (corr_ptr == nullptr)
	  ALEPHTHROW(CommandLineError, "correlation " + corr_name + " not found");
	action->corr_list.append(corr_ptr);
      }

    if (action->corr_list.is_empty())
      ALEPHTHROW(CommandLineError, "list of correlations is empty");

    const string & subtype = action->corr_list.get_first()->subtype_name;
    if (not action->corr_list.all([&subtype] (auto p)
				  { return p->subtype_name == subtype; }))
      ALEPHTHROW(CommandLineError, "correlation must be of same subtype");
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
  { "print", "list", "match", "apply", "global_apply", "local_calibration",
    "tmp_calibration", "global_calibration" };

const string actions =
 ActionType::valid_actions.take(ActionType::valid_actions.size() - 1).
	    foldl<string>("", [] (auto & a, auto & s)
	      {
		return a + s + " | ";
	      }) + ActionType::valid_actions.get_last();

const AHDispatcher<string, void (*)(ActionType*, istringstream*)>
ActionType::dispatcher 
(
 "print", [] (ActionType*, istringstream*) {},
 "list", ActionType::read_property_name,
 "match", ActionType::read_property_name,
 "apply", ActionType::read_property_name,
 "global_apply", ActionType::dummy,
 "local_calibration", ActionType::read_local_calibration,
 "tmp_calibration", ActionType::read_local_calibration,
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
ValueArg<double> uod = { "", "uod", "uod", false, 0, "uod in cP", cmd };


MultiArg<ValuesArg> target =
  { "", "property", "property array", false, 
    "property array in format \"property property-unit t tunit pb punit p-list "
    "property-list\"", cmd };

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

void build_pvt_data()
{
  data.add_const("api", api.getValue(), *test_unit("api", Api::get_instance()));
  data.add_const("rsb", rsb.getValue(), *test_unit("rsb",
						   SCF_STB::get_instance()));
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

void print_correlations(const DynList<DynList<string>> & l)
{
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

void print_data()
{
  cout << data << endl;
}

void process_list()
{
  print_correlations(Correlation::array().
    filter([tgt = action.getValue().property_name] (auto p)
	   {
	     return p->target_name() == tgt;
	   }).maps<DynList<string>>([] (auto p) { return p->to_dynlist(); }));
}

void process_match()
{
  print_correlations(data.matches_with_pars(action.getValue().property_name).
		     maps<DynList<string>>([] (auto p)
					   { return p->to_dynlist(); }));
}

using T = PvtData::T;

# define Define_Cmp(name)			\
  auto cmp_##name = [] (const T & d1, const T & d2)			\
  {									\
    return CorrStat::name(get<3>(d1)) < CorrStat::name(get<3>(d2));	\
  }

# define Define_1_Cmp(name)			\
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
  auto corr_list = data.can_be_applied(action.getValue().property_name);

  DynList<T> stats = Aleph::sort(corr_list.maps<T>([&] (auto corr_ptr)
    {
      return data.stats(corr_ptr);
    }), cmp[::sort.getValue()]);

  DynList<DynList<string>> rows = stats.maps<DynList<string>>([] (auto & t)
    {
      DynList<string> ret = build_dynlist<string>(get<0>(t)->name);
      auto stats = CorrStat::desc_to_dynlist(get<3>(t));
      //template maps<string>([] (auto v) { return ::to_string(v); });
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
}

void put_sample(const Correlation * corr_ptr,
		DynList<DynList<double>> & rows, DynList<string> & header,
		const double temp,
		const DynList<double> & yc, double c, double m)
{
  int t = temp;
  const string name = corr_ptr->name + "_" + ::to_string(t);
  const auto & mode = mode_type.getValue();

  if (mode == "single")
    {
      rows.append(yc);
      header.append(name);
    }
  else if (mode == "calibrated")
    {
      rows.append(yc.maps([c, m] (auto y) { return c + m*y; }));
      header.append(name + "_adjusted");
    }
  else
    {
      rows.append(yc);
      rows.append(yc.maps([c, m] (auto y) { return c + m*y; }));
      header.append(name);
      header.append(name + "_adjusted");
    }
}

void proccess_local_calibration() 
{
  static auto print_R = [] (const DynList<DynList<string>> & l) -> string
  {
    auto names = l.get_first();
    auto vals = transpose(l.drop(1));

    auto p = vals.remove_first(); names.remove_first();
    auto y = vals.remove_first();
    auto yname = split_to_list(names.remove_first(), " ").get_first();

    ostringstream s;
    s << Rvector("p", p) << endl
      << Rvector(yname, y) << endl;
    double ymin = numeric_limits<double>::max(), ymax = 0;
    DynList<string> ynames;
    for (auto it = get_zip_it(names, vals); it.has_curr(); it.next())
    {
      auto t = it.get_curr();
      auto & yc = get<1>(t);
      auto & yname = get<0>(t);
      s << Rvector(yname, yc) << endl;
      ynames.append(yname);
      ymin = yc.foldl(ymin, [] (auto a, auto y) { return min(a, atof(y)); });
      ymax = yc.foldl(ymax, [] (auto a, auto y) { return max(a, atof(y)); });
    }

    s << "plot(p, " << yname << ",ylim=c(" << ymin << "," << ymax << "))"
      << endl;
    size_t col = 1;
    DynList<string> colnames;
    DynList<string> cols;
    for (auto it = ynames.get_it(); it.has_curr(); it.next(), ++col)
      {
	auto & yname = it.get_curr();
	colnames.append("\"" + yname + "\"");
	cols.append(to_string(col));
	s << "lines(p, " << yname << ", col=" << col << ")" << endl;
      }

    s << Rvector("cnames", colnames) << endl
      << Rvector("cols", cols) << endl
      <<  "legend(\"topleft\", legend=cnames, lty=1, col=cols)" << endl;

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

  const auto & mode = mode_type.getValue();
  
  const auto & corr_list = action.getValue().corr_list;
  DynList<pair<double, double>> comb; // coefficients c and m

  // First we must verify that each read correlation applies to the data set
  for (auto it = corr_list.get_it(); it.has_curr(); it.next())
    {
      auto corr_ptr = it.get_curr();
      if (not data.can_be_applied(corr_ptr))
	ALEPHTHROW(CommandLineError,
		 corr_ptr->name + " does not apply to data set");
      
      if (mode != "single")
	{
	  auto stats = data.tstats(corr_ptr);
	  comb.append(make_pair(CorrStat::c(get<3>(stats)),
				CorrStat::m(get<3>(stats))));
	}
      else
	comb.append(make_pair(0, 1));
    }

  const Correlation * corr_ptr = corr_list.get_first();
  auto vals = data.apply(corr_ptr);
  DynList<double> pressures = get<0>(vals);
  DynList<double> y = get<2>(vals);
  DynList<double> yc = get<3>(vals);
  const Unit * punit = get<1>(vals);
  const Unit * yunit = get<4>(vals);
  auto p = comb.get_first();
  auto c = p.first;
  auto m = p.second;

  DynList<DynList<double>> rows = build_dynlist<DynList<double>>(pressures, y);
  DynList<string> header =
    build_dynlist<string>("p " + punit->name,
			  corr_ptr->target_name() + " " + yunit->name);

  put_sample(corr_ptr, rows, header, 0, yc, c, m);

  for (auto it = zip_it_pos(1, corr_list, comb); it.has_curr(); it.next())
    {
      auto curr = it.get_curr();
      corr_ptr = get<0>(curr);
      vals = data.apply(corr_ptr);
      auto & pvals = get<0>(vals);
      if (not zip_all([] (auto t) { return get<0>(t) == get<1>(t); },
		      pressures, pvals))
	ALEPHTHROW(InvariantError, "pressures for correlation " + corr_ptr->name);

      yc = get<2>(vals);
      put_sample(corr_ptr, rows, header, 0, yc, c, m);
    }

  DynList<DynList<string>> result =
    transpose(rows.maps<DynList<string>>([] (auto & l)
    {
      return l.template maps<string>([] (auto v) { return to_string(v); });
    }));
  result.insert(header);

  assert(equal_length(rows, header));

  cout << print_dispatcher.run(output.getValue(), result) << endl;
}

  // TODO: rutina que reciba corr_ptr, data, rows, header y añada los cálculos;
void
compute_results(DynList<DynList<double>> & rows, DynList<string> & header,
		const Correlation * corr_ptr, double c, double m,
		const PvtData & data)
{
  const string target_name = corr_ptr->target_name();
  auto vals = data.rapply(corr_ptr);
  for (auto it = vals.get_it(); it.has_curr(); it.next())
    {
      auto & curr = it.get_curr();
      auto t = get<0>(curr);
      DynList<double> yc = get<6>(curr);
      put_sample(corr_ptr, rows, header, t, yc, c, m);
    }
}

void proccess_tmp_calibration() 
{
  static auto print_R = [] (const DynList<DynList<string>> & l) -> string
  {
    struct Tmp
    {
      DynList<string> p;
      DynList<string> y;
      DynList<DynList<string>> yc;
    };
    auto cols = transpose(l); // first contains column name

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
      <<  "legend(\"topleft\", legend=cnames, col=cols, pch=pchs, lty=ltys)"
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

  const auto & mode = mode_type.getValue();
  
  const auto & corr_list = action.getValue().corr_list;
  DynList<pair<double, double>> coef; // coefficients c and m

      // verify that each read correlation applies to the data set
  for (auto it = corr_list.get_it(); it.has_curr(); it.next())
    {
      auto corr_ptr = it.get_curr();
      if (not data.can_be_applied(corr_ptr))
	ALEPHTHROW(CommandLineError,
		   corr_ptr->name + " does not apply to data set");
      
      if (mode != "single")
	{
	  auto stats = data.rstats(corr_ptr);
	  coef.append(make_pair(CorrStat::c(get<3>(stats)),
				CorrStat::m(get<3>(stats))));
	}
      else
	coef.append(make_pair(0, 1));
    }

  const Correlation * corr_ptr = corr_list.get_first();
  auto vals = data.rapply(corr_ptr);
  DynList<DynList<double>> rows;
  DynList<string> header;
  for (auto it = vals.get_it(); it.has_curr(); it.next())
    {
      auto & curr = it.get_curr();
      const int t = get<0>(curr);
      const string suffix = ::to_string(t);
      DynList<double> & p = get<3>(curr);
      DynList<double> & y = get<5>(curr);
      append_in_container(rows, move(p), move(y));
      append_in_container(header, "p_" + suffix,
			  corr_ptr->target_name() + "_" + suffix);
    }

  for (auto it = zip_it(corr_list, coef); it.has_curr(); it.next())
    {
      auto t = it.get_curr();
      const Correlation * corr_ptr = get<0>(t);
      const auto p = get<1>(t);
      const double c = p.first;
      const double m = p.second;
      compute_results(rows, header, corr_ptr, c, m, data);
    }
  
  DynList<DynList<string>> result =
    transpose(rows.maps<DynList<string>>([] (auto & l)
    {
      return l.template maps<string>([] (auto v) { return to_string(v); });
    }));
  result.insert(header);

  assert(equal_length(rows, header));

  cout << print_dispatcher.run(output.getValue(), result) << endl;
}

const AHDispatcher<string, void (*)()> dispatcher =
  {
    "print", print_data,
    "list", process_list,
    "match", process_match,
    "apply", process_apply,
    "global_apply", dummy,
    "local_calibration", proccess_local_calibration,
    "tmp_calibration", proccess_tmp_calibration,
    "global_calibration", dummy
  };

int main(int argc, char *argv[])
{
  UnitsInstancer::init();
  cmd.parse(argc, argv);
  build_pvt_data();
  dispatcher.run(action.getValue().type);

  // TODO: falta sort
}
