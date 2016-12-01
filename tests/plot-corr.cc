
# include <memory>

# include <tclap/CmdLine.h>

# include <ah-zip.H>
# include <tpl_dynMapTree.H>

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

SwitchArg check_arg = { "", "check", "check correlation ranges", cmd };
bool check = false;
void set_check()
{
  check = check_arg.getValue();
}

// Mandatory command arguments

ValueArg<double> api_arg = { "", "api", "api", true, 0, "api", cmd };
Correlation::NamedPar api_par;
void set_api()
{
  api_par = make_tuple(true, "api", api_arg.getValue(), &Api::get_instance());
}

ValueArg<double> rsb_arg = { "", "rsb", "rsb", true, 0, "rsb in scf/STB", cmd };
Correlation::NamedPar rsb_par;
void set_rsb()
{
  rsb_par = make_tuple(true, "rsb", rsb_arg.getValue(),
		       &SCF_STB::get_instance());
}

ValueArg<double> yg_arg = { "", "yg", "yg", true, 0, "yg in Sgg", cmd };
Correlation::NamedPar yg_par;
void set_yg()
{
  yg_par = make_tuple(true, "yg", yg_arg.getValue(), &Sgg::get_instance());
}

// optional command arguments

ValueArg<double> tsep_arg = { "", "tsep", "separator temperature", false, 0,
			      "separator temperature in degF", cmd };
Correlation::NamedPar tsep_par;
void set_tsep()
{
  tsep_par = make_tuple(tsep_arg.isSet(), "tsep",
			tsep_arg.getValue(), &Fahrenheit::get_instance());
}

ValueArg<double> psep_arg = { "", "psep", "separator pressure", false, 0,
			      "separator pressure in psia", cmd };
Correlation::NamedPar psep_par;
void set_psep()
{
  psep_par = make_tuple(psep_arg.isSet(), "psep",
			psep_arg.getValue(), &psia::get_instance());
}

ValueArg<double> n2_concentration_arg =
  { "", "n2-concentration", "n2-concentration", false, 0,
    "n2-concentration in MolePercent", cmd };
Correlation::NamedPar n2_concentration_par;
void set_n2_concentration()
{
  n2_concentration_par = make_tuple(n2_concentration_arg.isSet(),
				    "n2_concentration",
				    n2_concentration_arg.getValue(),
				    &MolePercent::get_instance());
}

ValueArg<double> co2_concentration_arg =
  { "", "co2-concentration", "co2-concentration", false, 0,
    "co2-concentration in MolePercent", cmd };
Correlation::NamedPar co2_concentration_par;
void set_co2_concentration()
{
  co2_concentration_par = make_tuple(co2_concentration_arg.isSet(),
				     "co2_concentration",
				     co2_concentration_arg.getValue(),
				     &MolePercent::get_instance());
}

ValueArg<double> h2s_concentration_arg =
  { "", "h2s-concentration", "h2s-concentration", false, 0,
    "h2s-concentration in MolePercent", cmd };
Correlation::NamedPar h2s_concentration_par;
void set_h2s_concentration()
{
  h2s_concentration_par = make_tuple(h2s_concentration_arg.isSet(),
				     "h2s_concentration",
				     h2s_concentration_arg.getValue(),
				     &MolePercent::get_instance());
}

ValueArg<double> c_pb_arg = { "", "c-pb", "pb adjustment", false, 0,
			      "pb adjustment", cmd };
ValueArg<string> pb_arg = { "", "pb", "correlation for pb", true, "",
			    "correlation for pb", cmd };
const Correlation * pb_corr = nullptr;
DynList<Correlation::NamedPar> pb_pars;
void set_pb()
{
  assert(yg_arg.isSet() and rsb_arg.isSet() and api_arg.isSet());
  pb_corr = Correlation::search_by_name(pb_arg.getValue());
  if (pb_corr == nullptr)
    error_msg(pb_arg.getValue() + " correlation not found");

  if (pb_corr->target_name() != "pb")
    error_msg(pb_arg.getValue() + " correlation is not for pb");

  pb_pars.append(yg_par);
  pb_pars.append(rsb_par);
  pb_pars.append(api_par);

  if (tsep_arg.isSet())
    pb_pars.append(tsep_par);

  if (psep_arg.isSet())
    pb_pars.append(psep_par);

  if (n2_concentration_arg.isSet())
    pb_pars.append(n2_concentration_par);

  if (co2_concentration_arg.isSet())
    pb_pars.append(co2_concentration_par);

  if (h2s_concentration_arg.isSet())
    pb_pars.append(h2s_concentration_par);
}

Correlation::NamedPar compute_pb(const double t)
{
  Correlation::NamedPar t_par = make_tuple(true, "t",
					   t, &Fahrenheit::get_instance());
  pb_pars.insert(t_par);
  auto ret = pb_corr->tuned_compute_by_names(pb_pars,
					     c_pb_arg.getValue(), 0, check);
  pb_pars.remove_first();
  return make_tuple(true, "pb", ret.raw(), &ret.unit);
}

void set_correlation(ValueArg<string> & corr_name_arg,
		     const string & target_name,
		     const Correlation *& corr_ptr,
		     bool force_set = false)
{
  if (force_set and not corr_name_arg.isSet())
    error_msg("Correlation fo "+ target_name +
	      " has not been specified in command line");
  if (not corr_name_arg.isSet())
    return;
  const string & corr_name = corr_name_arg.getValue();
  corr_ptr = Correlation::search_by_name(corr_name);
  if (corr_ptr == nullptr)
    error_msg("Correlation for rs " + corr_name +  " not found");
  if (corr_ptr->target_name() != target_name)
    error_msg("Correlation " + corr_ptr->name + " is not for " + target_name);
}


ValueArg<double> c_rs_arg = { "", "c-rs", "rs c", false, 0, "rs c", cmd };
ValueArg<double> m_rs_arg = { "", "m-rs", "rs m", false, 1, "rs m", cmd };
ValueArg<string> rs_corr_arg = { "", "rs", "correlation for rs", false, "",
				 "correlation for rs", cmd };
const Correlation * rs_corr = nullptr;
void set_rs_corr(bool force = false)
{ set_correlation(rs_corr_arg, "rs", rs_corr, force); }

ValueArg<double> c_bob_arg = { "", "c-bob", "ob c", false, 0, "bob c", cmd };
ValueArg<double> m_bob_arg = { "", "m-bob", "bob m", false, 1, "bob m", cmd };
ValueArg<string> bob_corr_arg = { "", "bob", "correlation for bob", false, "",
				  "correlation for bob", cmd };
const Correlation * bob_corr = nullptr;
void set_bob_corr(bool force = false)
{ set_correlation(bob_corr_arg, "bob", bob_corr, force); }

ValueArg<double> c_boa_arg = { "", "c-boa", "ob c", false, 0, "boa c", cmd };
ValueArg<double> m_boa_arg = { "", "m-boa", "boa m", false, 1, "boa m", cmd };
ValueArg<string> boa_corr_arg = { "", "boa", "correlation for boa", false, "",
				  "correlation for boa", cmd };
const Correlation * boa_corr = nullptr;
void set_boa_corr(bool force = false)
{ set_correlation(boa_corr_arg, "boa", boa_corr, force); }

ValueArg<double> c_uod_arg = { "", "c-uod", "uod c", false, 0, "uod c", cmd };
ValueArg<double> m_uod_arg = { "", "m-ruod", "uod m", false, 1, "uod m", cmd };
ValueArg<string> uod_corr_arg = { "", "uod", "correlation for uod", false, "",
				 "correlation for ruod", cmd };
const Correlation * uod_corr = nullptr;
void set_uod_corr(bool force = false)
{ set_correlation(uod_corr_arg, "uod", uod_corr, force); }

ValueArg<double> c_cob_arg = { "", "c-cob", "cob c", false, 0, "cob c", cmd };
ValueArg<double> m_cob_arg = { "", "m-cob", "cob m", false, 1, "cob m", cmd };
ValueArg<string> cob_corr_arg = { "", "cob", "correlation for cob", false, "",
				  "correlation for cob", cmd };
const Correlation * cob_corr = nullptr;
void set_cob_corr(bool force = false)
{ set_correlation(cob_corr_arg, "cob", cob_corr, force); }

ValueArg<double> c_coa_arg = { "", "c-coa", "coa c", false, 0, "coa c", cmd };
ValueArg<double> m_coa_arg = { "", "m-coa", "coa m", false, 1, "coa m", cmd };
ValueArg<string> coa_corr_arg = { "", "coa", "correlation for coa", false, "",
				  "correlation for coa", cmd };
const Correlation * coa_corr = nullptr;
void set_coa_corr(bool force = false)
{ set_correlation(coa_corr_arg, "coa", coa_corr, force); }

ValueArg<double> c_uob_arg = { "", "c-uob", "uob c", false, 0, "uob c", cmd };
ValueArg<double> m_uub_arg = { "", "m-uob", "uob m", false, 1, "uob m", cmd };
ValueArg<string> uob_corr_arg = { "", "uob", "correlation for uob", false, "",
				  "correlation for uob", cmd };
const Correlation * uob_corr = nullptr;
void set_uob_corr(bool force = false)
{ set_correlation(uob_corr_arg, "uob", uob_corr, force); }

ValueArg<double> c_uoa_arg = { "", "c-uoa", "uoa c", false, 0, "uoa c", cmd };
ValueArg<double> m_uua_arg = { "", "m-uoa", "uoa m", false, 1, "uoa m", cmd };
ValueArg<string> uoa_corr_arg = { "", "uoa", "correlation for uoa", false, "",
				  "correlation for uoa", cmd };
const Correlation * uoa_corr = nullptr;
void set_uoa_corr(bool force = false)
{ set_correlation(uoa_corr_arg, "uoa", uoa_corr, force); }

SwitchArg grid_arg = { "", "grid", "generate grid for all", cmd };

struct RangeDesc
{
  double min = 0, max = 0;
  size_t n = 1; // num of steps

  RangeDesc & operator = (const string & str)
  {
    istringstream iss(str);
    if (not (iss >> min >> max >> n))
      throw TCLAP::ArgParseException(str + " is not of form \"min max n\"");

    if (min > max)
      {
	ostringstream s;
	s << "min value " << min <<  " greater than max value " << max;
	throw TCLAP::ArgParseException(s.str());
      }

    return *this;
  }

  double step() const noexcept { return (max - min) / (n - 1); }

  friend ostream & operator << (ostream & os, const RangeDesc & d)
  {
    return os << d.min<< " " << d.max << " " << d.n;
  }
  
  ostream& print(ostream &os) const
  {
    return os << *this;
  }
};

namespace TCLAP
{
  template<> struct ArgTraits<RangeDesc> { typedef StringLike ValueCategory; };
}

void set_range(const RangeDesc & range, const string & name,
	       const Unit & unit, DynList<Correlation::NamedPar> & l)
{
  assert(l.is_empty());
  const auto & step = range.step();
  double val = range.min;
  for (size_t i = 0; i < range.n; ++i, val += step)
    l.append(make_tuple(true, name, val, &unit));
}

ValueArg<RangeDesc> t_range =
  { "", "t", "range spec for temperature", true, RangeDesc(),
    "range spec \"min max n\" for temperature", cmd };
DynList<Correlation::NamedPar> t_values;
void set_t_range()
{
  set_range(t_range.getValue(), "t", Fahrenheit::get_instance(), t_values);
}

ValueArg<RangeDesc> p_range =
  { "", "p", "range spec for pressure", true, RangeDesc(),
    "range spec \"min max n\" for pressure", cmd };
DynList<Correlation::NamedPar> p_values;
void set_p_range()
{
  set_range(p_range.getValue(), "p", psia::get_instance(), p_values);
}

ValueArg<double> cb_arg = { "", "cb", "c for below range", false, 0,
			    "c for below range", cmd };

ValueArg<double> mb_arg = { "", "mb", "m for below range", false, 1,
			    "m for below range", cmd };

ValueArg<double> ca_arg = { "", "ca", "m for above range", false, 0,
			    "c for above range", cmd };

ValueArg<double> ma_arg = { "", "ma", "m for above range", false, 1,
			    "c for above range", cmd };

string target_name;

ValueArg<string> below_corr_arg = { "", "below", "below correlation name", true, 
				    "", "below correlation name", cmd };
const Correlation * below_corr_ptr = nullptr;
void set_below_corr()
{
  if (grid_arg.isSet())
    error_msg("grid option is incompatible with below option");
  below_corr_ptr = Correlation::search_by_name(below_corr_arg.getValue());
  if (below_corr_ptr == nullptr)
    error_msg(below_corr_arg.getValue() + " below correlation not found");
  target_name = below_corr_ptr->target_name();
  if (target_name != "rs" and target_name != "bob" and target_name != "uob")
    error_msg(below_corr_arg.getValue() +
	      " below correlation has an invalid target name"
	      " (must be rs, bob or uob family)");
  if ((target_name == "bob" or target_name == "uob") and not rs_corr_arg.isSet())
    error_msg("Correlation for rs has not been specified");
  if (target_name == "uob" and not bob_corr_arg.isSet())
    error_msg("Correlation for bob has not been specified");
  if (target_name == "uob" and not boa_corr_arg.isSet())
    error_msg("Correlation for boa has not been specified");
  if (target_name == "uob" and not uod_corr_arg.isSet())
    error_msg("Correlation for uod has not been specified");
}

ValueArg<string> above_corr_arg = { "", "above", "above correlation name", false,
				    "", "above correlation name", cmd };
const Correlation * above_corr_ptr = nullptr;
void set_above_corr()
{
  if (grid_arg.isSet())
    error_msg("grid option is incompatible with above option");
  if (target_name == "rs")
    {
      if (not above_corr_arg.isSet())
	{
	  above_corr_ptr = &RsAbovePb::get_instance();
	  return;
	}
      else
	error_msg("Above correlation must not be defined for rs");
    }

  above_corr_ptr = Correlation::search_by_name(above_corr_arg.getValue());
  if (above_corr_ptr == nullptr)
    error_msg(above_corr_arg.getValue() + " above correlation not found");
  
  const string above_target_name = above_corr_ptr->target_name();
  if (above_target_name == "boa")
    {
      if (target_name != "bob")
	error_msg("Above target " + above_target_name +
		  " cannot be combined with below correlation " +
		  below_corr_ptr->name);
      else
	return;
    }

  if (above_target_name == "uoa" and target_name == "uob")
    return;
  
  error_msg("Above target " + above_target_name + " cannot be combined with "
	    "below correlation " + below_corr_ptr->name);
}

vector<string> out_type = { "csv", "mat", "json", "R" };
ValuesConstraint<string> allowed_out_type = out_type;
ValueArg<string> output_type = { "t", "output-type", "output type", false,
				 "mat", &allowed_out_type, cmd };

enum class OutputType { mat, csv, R, json, undefined };

OutputType get_output_type(const string & type)
{
  if (type == "mat") return OutputType::mat;
  if (type == "csv") return OutputType::csv;
  if (type == "R") return OutputType::R;
  if (type == "json") return OutputType::json;
  error_msg("Invalid output type " + type);
  return OutputType::undefined;
}

vector<string> sort_values = { "p", "t", "value"};
ValuesConstraint<string> allow_sort = sort_values;
ValueArg<string> sort_type = { "s", "sort", "output sorting type", false,
			       "t", &allow_sort, cmd };

enum class SortType { Pressure, Temperature, Value, Undefined };

SortType get_sort_type(const string & type)
{
  if (type == "p") return SortType::Pressure;
  if (type == "t") return SortType::Temperature;
  if (type == "value") return SortType::Value;
  error_msg("Invalid sort type");
  return SortType::Undefined;
}

DefinedCorrelation target_correlation(const double pb_val)
{
  double max_p = psia::get_instance().max_val;
  DefinedCorrelation ret("p");
  ret.add_tuned_correlation(below_corr_ptr, psia::get_instance().min_val, pb_val,
			    cb_arg.getValue(), mb_arg.getValue());
  ret.add_tuned_correlation(above_corr_ptr, nextafter(pb_val, max_p), max_p,
			    ca_arg.getValue(), ma_arg.getValue());
  return ret;
}

void
test_parameter(const DynList<pair<string, DynList<string>>> & required,
	       const Correlation::NamedPar & par,
	       DynList<Correlation::NamedPar> & pars_list)
{
  if (required.exists([&par] (const auto & p) { return p.first == get<1>(par) or
      p.second.exists([&par] (const auto & s) { return s == get<1>(par); }); }))
    pars_list.append(par);
}

DynList<Correlation::NamedPar> compute_pb_and_load_constant_parameters()
{
  DynList<Correlation::NamedPar> pars_list;
  auto t_par = t_values.get_first();
  pb_pars.insert(t_par);
  auto pb_val =
    pb_corr->tuned_compute_by_names(pb_pars, c_pb_arg.getValue(), 1, check);
  pb_pars.remove_first();
  auto required_pars = target_correlation(pb_val.raw()).parameter_list();
  test_parameter(required_pars, api_par, pars_list);
  test_parameter(required_pars, rsb_par, pars_list);
  test_parameter(required_pars, yg_par, pars_list);
  test_parameter(required_pars, tsep_par, pars_list);
  test_parameter(required_pars, psep_par, pars_list);
  test_parameter(required_pars, n2_concentration_par, pars_list);
  test_parameter(required_pars, co2_concentration_par, pars_list);
  test_parameter(required_pars, h2s_concentration_par, pars_list);

  return pars_list;
}

/// target, p, t
DynList<DynList<double>> generate_rs_values()
{
  DynList<Correlation::NamedPar> pars_list =
    compute_pb_and_load_constant_parameters();

  DynList<DynList<double>> vals; /// target, p, t
  DynList<double> row;

  for (auto t_it = t_values.get_it(); t_it.has_curr(); t_it.next())
    {
      auto t_par = t_it.get_curr();
      pars_list.insert(t_par);
      row.insert(get<2>(t_par));

      pb_pars.insert(t_par);
      auto pb_val =
	pb_corr->tuned_compute_by_names(pb_pars, c_pb_arg.getValue(), 1, check);
      pb_pars.remove_first(); // remove t_par

      pars_list.insert(make_tuple(true, "pb", pb_val.raw(), &pb_val.unit));

      auto def_corr = target_correlation(pb_val.raw());

      for (auto p_it = p_values.get_it(); p_it.has_curr(); p_it.next())
	{
	  auto p_par = p_it.get_curr();
	  pars_list.insert(p_par);
	  row.insert(get<2>(p_par));
	  
	  auto val = def_corr.compute_by_names(pars_list, check);

	  row.insert(val);
	  vals.append(row);
	  row.remove_first(); // val
	  
	  row.remove_first(); // p_par
	  pars_list.remove_first(); // p_par
	}

      row.remove_first(); // t_par

      pars_list.remove_first(); // pb_val
      pars_list.remove_first(); // t_par
    }

  return vals;
}

DynList<Correlation::NamedPar>
load_constant_parameters(const DynList<const Correlation*> & l)
{
  DynList<Correlation::NamedPar> pars_list;
  auto required_pars = DefinedCorrelation::parameter_list(l);
				    
  test_parameter(required_pars, api_par, pars_list);
  test_parameter(required_pars, rsb_par, pars_list);
  test_parameter(required_pars, yg_par, pars_list);
  test_parameter(required_pars, tsep_par, pars_list);
  test_parameter(required_pars, psep_par, pars_list);
  test_parameter(required_pars, n2_concentration_par, pars_list);
  test_parameter(required_pars, co2_concentration_par, pars_list);
  test_parameter(required_pars, h2s_concentration_par, pars_list);

  return pars_list;
}

/// target, p, t
DynList<DynList<double>> generate_bo_values()
{
  assert(rs_corr);
  DynList<Correlation::NamedPar> bo_pars_list =
    compute_pb_and_load_constant_parameters();
  
  DynList<Correlation::NamedPar> rs_pars_list =
    load_constant_parameters({rs_corr, &RsAbovePb::get_instance()});

  unique_ptr<DefinedCorrelation> defined_rs_corr;
  const Unit & rs_unit = rs_corr->unit;
  const double rmin = rs_unit.min_val;
  const double rmax = rs_unit.max_val;

  DynList<DynList<double>> vals; /// target, p, t
  DynList<double> row;

  for (auto t_it = t_values.get_it(); t_it.has_curr(); t_it.next())
    {
      auto t_par = t_it.get_curr();
      bo_pars_list.insert(t_par);
      rs_pars_list.insert(t_par);
      row.insert(get<2>(t_par));

      pb_pars.insert(t_par);
      auto pb_val =
	pb_corr->tuned_compute_by_names(pb_pars, c_pb_arg.getValue(), 1, check);
      pb_pars.remove_first(); // remove t_par

      bo_pars_list.insert(make_tuple(true, "pb", pb_val.raw(), &pb_val.unit));
      rs_pars_list.insert(make_tuple(true, "pb", pb_val.raw(), &pb_val.unit));

      auto def_corr = target_correlation(pb_val.raw());
      defined_rs_corr = make_unique<DefinedCorrelation>("p");
      defined_rs_corr->add_tuned_correlation(rs_corr, rmin, pb_val.raw(),
					     c_rs_arg.getValue(),
					     m_rs_arg.getValue());
      defined_rs_corr->add_correlation(&RsAbovePb::get_instance(),
				       nextafter(pb_val.raw(), rmax), rmax);

      bo_pars_list.insert(make_tuple(true, "p", pb_val.raw(), &pb_val.unit));
      bo_pars_list.insert(make_tuple(true, "rs",
				     get<2>(rsb_par), get<3>(rsb_par)));
      auto bobp =
	below_corr_ptr->tuned_compute_by_names(bo_pars_list, ca_arg.getValue(),
					       ma_arg.getValue(), check);
      bo_pars_list.remove_first(); // rs
      bo_pars_list.remove_first(); // p
      bo_pars_list.insert(make_tuple(true, "bobp", bobp.raw(), &bobp.unit));

      for (auto p_it = p_values.get_it(); p_it.has_curr(); p_it.next())
	{
	  auto p_par = p_it.get_curr();
	  bo_pars_list.insert(p_par);
	  rs_pars_list.insert(p_par);
	  row.insert(get<2>(p_par));

	  auto rs = defined_rs_corr->compute_by_names(rs_pars_list, check);
	  rs_pars_list.remove_first(); // remove p_par

	  bo_pars_list.insert(make_tuple(true, "rs", rs, &rs_corr->unit));

	  auto bo = def_corr.compute_by_names(bo_pars_list, check);

	  row.insert(bo);
	  vals.append(row);
	  
	  row.remove_first(); // val
	  row.remove_first(); // p_par
	  
	  bo_pars_list.remove_first(); // rs
	  bo_pars_list.remove_first(); // p_par
	}

      row.remove_first(); // t_par

      rs_pars_list.remove_first(); // pb_par
      rs_pars_list.remove_first(); // t_par

      bo_pars_list.remove_first(); // bobp
      bo_pars_list.remove_first(); // pb_val
      bo_pars_list.remove_first(); // t_par
    }

  return vals;
}

/// target, p, t
DynList<DynList<double>> generate_uo_values()
{
  assert(rs_corr);
  DynList<Correlation::NamedPar> uo_pars_list =
    compute_pb_and_load_constant_parameters();
  
  DynList<Correlation::NamedPar> rs_pars_list =
    load_constant_parameters({rs_corr, &RsAbovePb::get_instance()});

  DynList<Correlation::NamedPar> uod_pars_list =
    load_constant_parameters({uod_corr});

  unique_ptr<DefinedCorrelation> defined_rs_corr;
  const Unit & rs_unit = rs_corr->unit;
  const double rmin = rs_unit.min_val;
  const double rmax = rs_unit.max_val;

  DynList<DynList<double>> vals; /// target, p, t
  DynList<double> row;

  for (auto t_it = t_values.get_it(); t_it.has_curr(); t_it.next())
    {
      auto t_par = t_it.get_curr();
      uo_pars_list.insert(t_par);
      rs_pars_list.insert(t_par);
      row.insert(get<2>(t_par));

      pb_pars.insert(t_par);
      auto pb_val =
	pb_corr->tuned_compute_by_names(pb_pars, c_pb_arg.getValue(), 1, check);
      pb_pars.remove_first(); // remove t_par

      uod_pars_list.insert(t_par);
      auto uod_val = uod_corr->compute_by_names(uod_pars_list, check);
      uod_pars_list.remove_first();

      uo_pars_list.insert(make_tuple(true, "pb", pb_val.raw(), &pb_val.unit));
      uo_pars_list.insert(make_tuple(true, "uod", uod_val.raw(), &uod_val.unit));
      rs_pars_list.insert(make_tuple(true, "pb", pb_val.raw(), &pb_val.unit));

      auto def_corr = target_correlation(pb_val.raw());
      defined_rs_corr = make_unique<DefinedCorrelation>("p");
      defined_rs_corr->add_tuned_correlation(rs_corr, rmin, pb_val.raw(),
					     c_rs_arg.getValue(),
					     m_rs_arg.getValue());
      defined_rs_corr->add_correlation(&RsAbovePb::get_instance(),
				       nextafter(pb_val.raw(), rmax), rmax);

      uo_pars_list.insert(make_tuple(true, "p", pb_val.raw(), &pb_val.unit));
      uo_pars_list.insert(make_tuple(true, "rs",
				     get<2>(rsb_par), get<3>(rsb_par)));
      auto uobp =
	below_corr_ptr->tuned_compute_by_names(uo_pars_list, cb_arg.getValue(),
					       mb_arg.getValue(), check);
      uo_pars_list.remove_first(); // rs
      uo_pars_list.remove_first(); // p
      uo_pars_list.insert(make_tuple(true, "uobp", uobp.raw(), &uobp.unit));

      for (auto p_it = p_values.get_it(); p_it.has_curr(); p_it.next())
	{
	  auto p_par = p_it.get_curr();
	  uo_pars_list.insert(p_par);
	  rs_pars_list.insert(p_par);
	  row.insert(get<2>(p_par));

	  auto rs = defined_rs_corr->compute_by_names(rs_pars_list, check);
	  rs_pars_list.remove_first(); // remove p_par
	  
	  uo_pars_list.insert(make_tuple(true, "rs", rs, &rs_corr->unit));

	  auto uo = def_corr.compute_by_names(uo_pars_list, check);

	  row.insert(uo);
	  vals.append(row);
	  
	  row.remove_first(); // val
	  row.remove_first(); // p_par
	  
	  uo_pars_list.remove_first(); // rs
	  uo_pars_list.remove_first(); // p_par
	}

      row.remove_first(); // t_par

      rs_pars_list.remove_first(); // pb_par
      rs_pars_list.remove_first(); // t_par

      uo_pars_list.remove_first(); // uobp
      uo_pars_list.remove_first(); // pb_val
      uo_pars_list.remove_first(); // uod_val
      uo_pars_list.remove_first(); // t_par
    }

  return vals;
}

DynList<DynList<double>> generate_grid()
{
  set_check();
  set_api();
  set_rsb();
  set_yg();
  set_tsep();
  set_psep();
  set_pb();
  set_rs_corr(true);
  set_bob_corr(true);
  set_boa_corr(true);
  set_uod_corr(true);
  set_cob_corr(true);
  set_coa_corr(true);
  set_uob_corr(true);
  set_uoa_corr(true);
  set_t_range();
  set_p_range();
  set_below_corr();
  set_above_corr();

  DynList<Correlation::NamedPar> pb_pars_list =
    compute_pb_and_load_constant_parameters();
  
  DynList<Correlation::NamedPar> rs_pars_list =
    load_constant_parameters({rs_corr, &RsAbovePb::get_instance()});

  DynList<Correlation::NamedPar> uod_pars_list =
    load_constant_parameters({uod_corr});

  DynList<Correlation::NamedPar> co_pars_list =
    load_constant_parameters({cob_corr, coa_corr});

  DynList<Correlation::NamedPar> uo_pars_list =
    load_constant_parameters({uob_corr, uoa_corr});

  unique_ptr<DefinedCorrelation> defined_rs_corr;
  const Unit & rs_unit = rs_corr->unit;
  const double rmin = rs_unit.min_val;
  const double rmax = rs_unit.max_val;

  DynList<DynList<double>> vals; /// target, p, t
  DynList<double> row;

  for (auto t_it = t_values.get_it(); t_it.has_curr(); t_it.next())
    {
      auto t_par = t_it.get_curr();
      uo_pars_list.insert(t_par);
      rs_pars_list.insert(t_par);
      row.insert(get<2>(t_par));

      pb_pars.insert(t_par);
      auto pb_val =
	pb_corr->tuned_compute_by_names(pb_pars, c_pb_arg.getValue(), 1, check);
      pb_pars.remove_first(); // remove t_par

      uod_pars_list.insert(t_par);
      auto uod_val = uod_corr->compute_by_names(uod_pars_list, check);
      uod_pars_list.remove_first();

      uo_pars_list.insert(make_tuple(true, "pb", pb_val.raw(), &pb_val.unit));
      uo_pars_list.insert(make_tuple(true, "uod", uod_val.raw(), &uod_val.unit));
      rs_pars_list.insert(make_tuple(true, "pb", pb_val.raw(), &pb_val.unit));

      auto def_corr = target_correlation(pb_val.raw());
      defined_rs_corr = make_unique<DefinedCorrelation>("p");
      defined_rs_corr->add_tuned_correlation(rs_corr, rmin, pb_val.raw(),
					     c_rs_arg.getValue(),
					     m_rs_arg.getValue());
      defined_rs_corr->add_correlation(&RsAbovePb::get_instance(),
				       nextafter(pb_val.raw(), rmax), rmax);

      uo_pars_list.insert(make_tuple(true, "p", pb_val.raw(), &pb_val.unit));
      uo_pars_list.insert(make_tuple(true, "rs",
				     get<2>(rsb_par), get<3>(rsb_par)));
      auto uobp =
	below_corr_ptr->tuned_compute_by_names(uo_pars_list, cb_arg.getValue(),
					       mb_arg.getValue(), check);
      uo_pars_list.remove_first(); // rs
      uo_pars_list.remove_first(); // p
      uo_pars_list.insert(make_tuple(true, "uobp", uobp.raw(), &uobp.unit));

      for (auto p_it = p_values.get_it(); p_it.has_curr(); p_it.next())
	{
	  auto p_par = p_it.get_curr();
	  uo_pars_list.insert(p_par);
	  rs_pars_list.insert(p_par);
	  row.insert(get<2>(p_par));

	  auto rs = defined_rs_corr->compute_by_names(rs_pars_list, check);
	  rs_pars_list.remove_first(); // remove p_par
	  
	  uo_pars_list.insert(make_tuple(true, "rs", rs, &rs_corr->unit));

	  auto uo = def_corr.compute_by_names(uo_pars_list, check);

	  row.insert(uo);
	  vals.append(row);
	  
	  row.remove_first(); // val
	  row.remove_first(); // p_par
	  
	  uo_pars_list.remove_first(); // rs
	  uo_pars_list.remove_first(); // p_par
	}

      row.remove_first(); // t_par

      rs_pars_list.remove_first(); // pb_par
      rs_pars_list.remove_first(); // t_par

      uo_pars_list.remove_first(); // uobp
      uo_pars_list.remove_first(); // pb_val
      uo_pars_list.remove_first(); // uod_val
      uo_pars_list.remove_first(); // t_par
    }

  return vals;
}

using OptionPtr = DynList<DynList<double>> (*)();

DynMapTree<string, OptionPtr> dispatch_tbl;

void init_dispatcher()
{
  dispatch_tbl.insert("rs", generate_rs_values);
  dispatch_tbl.insert("bob", generate_bo_values);
  dispatch_tbl.insert("uob", generate_uo_values);
}

DynList<DynList<double>> dispatch_option(const string & op)
{
  auto command = dispatch_tbl.search(op);
  if (command == nullptr)
    error_msg("Option " + op + " not registered");
  return (*command->second)();
}

auto cmp_p = [] (const DynList<double> & row1, const DynList<double> & row2)
{
  const auto & p1 = row1.nth(1);
  const auto & p2 = row2.nth(1);
  if (p1 == p2)
    return row1.get_last() < row2.get_last();
  return p1 < p2;
};

auto cmp_t = [] (const DynList<double> & row1, const DynList<double> & row2)
{
  const auto & t1 = row1.get_last();
  const auto & t2 = row2.get_last();
  if (t1 == t2)
    return row1.nth(1) < row2.nth(1);
  return t1 < t2;
};

void sort(DynList<DynList<double>> & vals, SortType type)
{
  switch (type)
    {
    case SortType::Pressure:
      in_place_sort(vals, cmp_p);
      break;
    case SortType::Temperature:
      in_place_sort(vals, cmp_t);
      break;
    case SortType::Value:
            in_place_sort(vals, [] (const auto & row1, const auto & row2)
		    {
		      return row1.get_first() < row2.get_first();
		    });
	    break;
    default:
      error_msg("Invalid sort type");
      break;
  }
}

string mat_format(const DynList<DynList<double>> & vals)
{
 DynList<DynList<string>> mat = vals.maps<DynList<string>>([] (const auto & row)
  {
    return row.template maps<string>([] (auto v) { return to_string(v); }).rev();
  });
  mat.insert({"t", "p", target_name});

  return to_string(format_string(mat)); 
}

string csv_format(const DynList<DynList<double>> & vals)
{
 DynList<DynList<string>> mat = vals.maps<DynList<string>>([] (const auto & row)
  {
    return row.template maps<string>([] (auto v) { return to_string(v); }).rev();
  });
  mat.insert({"t", "p", target_name});

  return to_string(format_string_csv(mat));
}

string R_format(const DynList<DynList<double>> & mat)
{
  auto values = transpose(mat);
  auto vals = values.get_first();
  auto pressure = values.nth(1);
  auto temperature = values.get_last();

  auto min_v = numeric_limits<double>::max();
  auto max_v = numeric_limits<double>::min();

  ostringstream s;

  if (get_sort_type(sort_type.getValue()) == SortType::Temperature)
    {
      const size_t num_samples = p_range.getValue().n;
      auto min_p = numeric_limits<double>::max();
      auto max_p = numeric_limits<double>::min();
      Array<DynList<double>> p_array;
      Array<DynList<double>> v_array;
      DynList<string> suffix;
      for (auto it = get_zip_it(temperature, pressure, vals); it.has_curr();)
	{
	  auto t = it.get_curr();
	  suffix.append(to_string(get<0>(t)));
	  DynList<double> p;
	  DynList<double> v;

	  for (size_t i = 0; i < num_samples; ++i, it.next())
	    {
	      auto tt = it.get_curr();
	      auto p_val = get<1>(tt);
	      auto v_val = get<2>(tt);
	      p.append(p_val);
	      v.append(v_val);
	      min_p = min(min_p, p_val);
	      max_p = max(max_p, p_val);
	      min_v = min(min_v, v_val);
	      max_v = max(max_v, v_val);
	    }
	  p_array.append(move(p));
	  v_array.append(move(v));
	}

      DynList<string> p_names, v_names, t_names;
      for (auto it = get_zip_it(suffix, p_array, v_array);
	   it.has_curr(); it.next())
	{
	  auto t = it.get_curr();	  
	  string p_name = "p_" + get<0>(t);
	  string v_name = "v_" + get<0>(t);
	  string t_name = "\"T = " + get<0>(t) + "\"";
	  s << Rvector(p_name, get<1>(t)) << endl
	    << Rvector(v_name, get<2>(t)) << endl;
	  p_names.append(move(p_name));
	  v_names.append(move(v_name));
	  t_names.append(move(t_name));
	}
      s << "plot(0, type=\"n\", xlim=c(" << min_p << "," << max_p << "),ylim=c("
	<< min_v << "," << max_v << "))" << endl;
      for (auto it = get_enum_zip_it(p_names, v_names); it.has_curr(); it.next())
	{
	  auto t = it.get_curr();
	  s << "lines(" << get<0>(t) << "," << get<1>(t) << ",col="
	    << get<2>(t) + 1 << ")" << endl;
	}
      s << Rvector("cnames", t_names) << endl
	<< Rvector("cols", range<size_t>(1, p_array.size())) << endl
	<< "legend(\"topleft\", legend=cnames, lty=1, col=cols)";
      return s.str();
    }

  const size_t num_samples = t_range.getValue().n;
  auto min_t = numeric_limits<double>::max();
  auto max_t = numeric_limits<double>::min();
  Array<DynList<double>> t_array;
  Array<DynList<double>> v_array;
  DynList<string> suffix;
  for (auto it = get_zip_it(temperature, pressure, vals); it.has_curr();)
    {
      auto tt = it.get_curr();
      suffix.append(to_string(get<1>(tt)));
      DynList<double> t;
      DynList<double> v;

      for (size_t i = 0; i < num_samples; ++i, it.next())
	{
	  auto tt = it.get_curr();
	  auto t_val = get<0>(tt);
	  auto v_val = get<2>(tt);
	  t.append(t_val);
	  v.append(v_val);
	  min_t = min(min_t, t_val);
	  max_t = max(max_t, t_val);
	  min_v = min(min_v, v_val);
	  max_v = max(max_v, v_val);
	}
      t_array.append(move(t));
      v_array.append(move(v));
    }

      DynList<string> p_names, v_names, t_names;
      for (auto it = get_zip_it(suffix, t_array, v_array);
	   it.has_curr(); it.next())
	{
	  auto t = it.get_curr();	  
	  string t_name = "t_" + get<0>(t);
	  string v_name = "v_" + get<0>(t);
	  string p_name = "\"P = " + get<0>(t) + "\"";
	  s << Rvector(t_name, get<1>(t)) << endl
	    << Rvector(v_name, get<2>(t)) << endl;
	  p_names.append(move(p_name));
	  v_names.append(move(v_name));
	  t_names.append(move(t_name));
	}
      s << "plot(0, type=\"n\", xlim=c(" << min_t << "," << max_t << "),ylim=c("
	<< min_v << "," << max_v << "))" << endl;
      for (auto it = get_enum_zip_it(t_names, v_names); it.has_curr(); it.next())
	{
	  auto t = it.get_curr();
	  s << "lines(" << get<0>(t) << "," << get<1>(t) << ",col="
	    << get<2>(t) + 1 << ")" << endl;
	}
      s << Rvector("cnames", p_names) << endl
	<< Rvector("cols", range<size_t>(1, t_array.size())) << endl
	<< "legend(\"topleft\", legend=cnames, lty=1, col=cols)";

  return s.str();
}


int main(int argc, char *argv[])
{
  init_dispatcher();
  cmd.parse(argc, argv);

  set_check();
  set_api();
  set_rsb();
  set_yg();
  set_tsep();
  set_psep();
  set_pb();
  set_rs_corr();
  set_uod_corr();
  set_t_range();
  set_p_range();
  set_below_corr();
  set_above_corr();

  auto vals = dispatch_option(target_name);

  sort(vals, get_sort_type(sort_type.getValue()));

  switch (get_output_type(output_type.getValue()))
    {
    case OutputType::mat:
      cout << mat_format(vals) << endl;
      break;
    case OutputType::csv:
      cout << csv_format(vals) << endl;
      break;
    case OutputType::json:
      error_msg("json not yet implemented");
      break;
    case OutputType::R:
      cout << R_format(vals) << endl;
      break;
    default:
      error_msg("Invalid output type");
    }
}
