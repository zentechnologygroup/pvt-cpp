
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

DynSetTree<string> par_name_tbl =
  { "api", "rsb", "yg", "tsep", "t", "p", "psep", "h2s-concentration",
    "co2-concentration", "n2-concentration", "nacl-concentration" };
struct ArgUnit
{
  string par_name;
  string symbol;

  ArgUnit & operator = (const string & str)
  {
    istringstream iss(str);
    if (not (iss >> par_name >> symbol))
      throw TCLAP::ArgParseException(str + " is not a pair par-name unit");

    if (not par_name_tbl.contains(par_name))
      throw TCLAP::ArgParseException(par_name + " is an invalid parameter name");

    return *this;
  }

  ArgUnit() {}

  friend ostream& operator << (ostream &os, const ArgUnit & a) 
  {
    return os << a.par_name << " " << a.symbol;
  }
};

namespace TCLAP
{
  template<> struct ArgTraits<ArgUnit> { typedef StringLike ValueCategory; };
}

MultiArg<ArgUnit> unit = { "", "unit", "unit \"par-name unit\"", false,
			   "unit \"par-name unit\"", cmd };
const Unit * test_unit_change(const string & par_name, const Unit & ref_unit)
{
  if (not par_name_tbl.contains(par_name))
    {
      cout << "for option --unit " << par_name << ": invalid parameter name"
	   << endl;
      abort();
    }
  
  const Unit * ret = &ref_unit;
  for (const auto & par : unit.getValue()) 
    if (par.par_name == par_name)
      {
	const Unit * ret = Unit::search_by_symbol(par.symbol);
	if (ret == nullptr)
	  {
	    cout << "In unit change for " << par_name << ": unit symbol "
		 << par.symbol << " not found" << endl;
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

// Mandatory command arguments

ValueArg<double> api_arg = { "", "api", "api", true, 0, "api", cmd };
Correlation::NamedPar api_par;
void set_api()
{
  auto api_unit = test_unit_change("api", Api::get_instance());
  api_par = make_tuple(true, "api", api_arg.getValue(), api_unit);
}

ValueArg<double> rsb_arg = { "", "rsb", "rsb", true, 0, "rsb in scf/STB", cmd };
Correlation::NamedPar rsb_par;
void set_rsb()
{
  auto rsb_unit = test_unit_change("rsb", SCF_STB::get_instance());
  rsb_par = make_tuple(true, "rsb", rsb_arg.getValue(), rsb_unit);
}

inline VtlQuantity par(const Correlation::NamedPar & par)
{
  return VtlQuantity(*get<3>(par), get<2>(par));
}

inline Correlation::NamedPar npar(const string & name, const BaseQuantity & p)
{
  return make_tuple(true, name, p.raw(), &p.unit);
}

# define NPAR(par) npar(#par, par)

ValueArg<double> yg_arg = { "", "yg", "yg", true, 0, "yg in Sgg", cmd };
Correlation::NamedPar yg_par;
VtlQuantity yg = VtlQuantity::null_quantity;
void set_yg()
{
  auto yg_unit = test_unit_change("yg", Sgg::get_instance());
  yg_par = make_tuple(true, "yg", yg_arg.getValue(), yg_unit);
  yg.set(par(yg_par));
}

// optional command arguments

ValueArg<double> tsep_arg = { "", "tsep", "separator temperature", false, 0,
			      "separator temperature in degF", cmd };
Correlation::NamedPar tsep_par;
VtlQuantity tsep = VtlQuantity::null_quantity;
void set_tsep()
{
  auto tsep_unit = test_unit_change("tsep", Fahrenheit::get_instance());
  tsep_par = make_tuple(tsep_arg.isSet(), "tsep",
			tsep_arg.getValue(), tsep_unit);
  tsep.set(par(tsep_par));
}

ValueArg<double> psep_arg = { "", "psep", "separator pressure", false, 0,
			      "separator pressure in psia", cmd };
Correlation::NamedPar psep_par;
VtlQuantity psep = VtlQuantity::null_quantity;
void set_psep()
{
  auto psep_unit = test_unit_change("psep", psia::get_instance());
  psep_par = make_tuple(psep_arg.isSet(), "psep",
			psep_arg.getValue(), psep_unit);
  psep.set(par(psep_par));
}

ValueArg<double> n2_concentration_arg =
  { "", "n2-concentration", "n2-concentration", false, 0,
    "n2-concentration in MolePercent", cmd };
Correlation::NamedPar n2_concentration_par;
VtlQuantity n2_concentration = VtlQuantity::null_quantity;
void set_n2_concentration()
{
  auto n2_concentration_unit = test_unit_change("n2-concentration",
						MolePercent::get_instance());
  n2_concentration_par = make_tuple(n2_concentration_arg.isSet(),
				    "n2_concentration",
				    n2_concentration_arg.getValue(),
				    n2_concentration_unit);
  n2_concentration.set(par(n2_concentration_par));
}

ValueArg<double> co2_concentration_arg =
  { "", "co2-concentration", "co2-concentration", false, 0,
    "co2-concentration in MolePercent", cmd };
Correlation::NamedPar co2_concentration_par;
VtlQuantity co2_concentration = VtlQuantity::null_quantity;
void set_co2_concentration()
{
  auto co2_concentration_unit = test_unit_change("co2-concentration",
						 MolePercent::get_instance());
  co2_concentration_par = make_tuple(co2_concentration_arg.isSet(),
				     "co2_concentration",
				     co2_concentration_arg.getValue(),
				     co2_concentration_unit);
  co2_concentration.set(par(co2_concentration_par));
}

ValueArg<double> h2s_concentration_arg =
  { "", "h2s-concentration", "h2s-concentration", false, 0,
    "h2s-concentration in MolePercent", cmd };
Correlation::NamedPar h2s_concentration_par;
VtlQuantity h2s_concentration = VtlQuantity::null_quantity;
void set_h2s_concentration()
{
  auto h2s_concentration_unit = test_unit_change("h2s-concentration",
						 MolePercent::get_instance());
  h2s_concentration_par = make_tuple(h2s_concentration_arg.isSet(),
				     "h2s_concentration",
				     h2s_concentration_arg.getValue(),
				     h2s_concentration_unit);
  h2s_concentration.set(par(h2s_concentration_par));
}

ValueArg<double> nacl_concentration_arg =
  { "", "nacl-concentration", "nacl-concentration", false, 0,
    "nacl-concentration in MolePercent", cmd };
Correlation::NamedPar nacl_concentration_par;
VtlQuantity nacl_concentration = VtlQuantity::null_quantity;
void set_nacl_concentration()
{
  auto nacl_concentration_unit = test_unit_change("nacl-concentration",
						  Molality_NaCl::get_instance());
  nacl_concentration_par = make_tuple(nacl_concentration_arg.isSet(),
				      "nacl_concentration",
				      nacl_concentration_arg.getValue(),
				      nacl_concentration_unit);
  nacl_concentration.set(par(nacl_concentration_par));
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
    error_msg("Correlation for "+ target_name +
	      " has not been specified in command line (" +
	      corr_name_arg.longID() + ")");
  if (not corr_name_arg.isSet())
    return;
  const string & corr_name = corr_name_arg.getValue();
  corr_ptr = Correlation::search_by_name(corr_name);
  if (corr_ptr == nullptr)
    error_msg("Correlation for " + target_name + " " +
	      corr_name +  " not found");
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
ValueArg<double> m_uob_arg = { "", "m-uob", "uob m", false, 1, "uob m", cmd };
ValueArg<string> uob_corr_arg = { "", "uob", "correlation for uob", false, "",
				  "correlation for uob", cmd };
const Correlation * uob_corr = nullptr;
void set_uob_corr(bool force = false)
{ set_correlation(uob_corr_arg, "uob", uob_corr, force); }

ValueArg<double> c_uoa_arg = { "", "c-uoa", "uoa c", false, 0, "uoa c", cmd };
ValueArg<double> m_uoa_arg = { "", "m-uoa", "uoa m", false, 1, "uoa m", cmd };
ValueArg<string> uoa_corr_arg = { "", "uoa", "correlation for uoa", false, "",
				  "correlation for uoa", cmd };
const Correlation * uoa_corr = nullptr;
void set_uoa_corr(bool force = false)
{ set_correlation(uoa_corr_arg, "uoa", uoa_corr, force); }

ValueArg<string> ppchc_corr_arg = { "", "ppchc", "Correlation for ppchc",
				    false, "", "Correlation for ppchc", cmd };
const Correlation * ppchc_corr = nullptr;
void set_ppchc_corr()
{
  set_correlation(ppchc_corr_arg, "ppchc", ppchc_corr, true);
}

ValueArg<string> ppcm_mixing_corr_arg =
  { "", "ppcm-mixing", "Correlation for ppcm mixing rule",
    false, "PpcmKayMixingRule", "Correlation for ppcm mixing rule", cmd };
const Correlation * ppcm_mixing_corr = nullptr;
void set_ppcm_mixing_corr()
{
  ppcm_mixing_corr = &PpcmKayMixingRule::get_instance(); // mandotory here!
  set_correlation(ppcm_mixing_corr_arg, "ppccm", ppcm_mixing_corr, false);
}

ValueArg<string> adjustedppcm_corr_arg =
  { "", "adjustedppcm", "Correlation for ajustedppcm",
    false, "AdjustedppcmWichertAziz", "Correlation for adjustedppcm", cmd };
const Correlation * adjustedppcm_corr = nullptr;
void set_adjustedppcm_corr()
{
  adjustedppcm_corr = &AdjustedppcmWichertAziz::get_instance(); // mandotory here!
  set_correlation(adjustedppcm_corr_arg, "adjustedppccm",
		  adjustedppcm_corr, false);
}

ValueArg<string> tpchc_corr_arg = { "", "tpchc", "Correlation for tpchc",
				    false, "", "Correlation for tpchc", cmd };
const Correlation * tpchc_corr = nullptr;
void set_tpchc_corr()
{
  set_correlation(tpchc_corr_arg, "tpchc", tpchc_corr, true);
}

ValueArg<string> tpcm_mixing_corr_arg =
  { "", "tpcm", "Correlation for tpcm mixing rule",
    false, "TpcmKayMixingRule", "Correlation for tpcm mixing rule", cmd };
const Correlation * tpcm_mixing_corr = nullptr;
void set_tpcm_mixing_corr()
{
  tpcm_mixing_corr = &TpcmKayMixingRule::get_instance(); // mandotory here!
  set_correlation(tpcm_mixing_corr_arg, "tpcm", tpcm_mixing_corr, false);
}

ValueArg<string> adjustedtpcm_corr_arg =
  { "", "adjustedtpcm", "Correlation for adjustedtpcm",
    false, "AdjustedtpcmWichertAziz", "Correlation for adjustedtpcm", cmd };
const Correlation * adjustedtpcm_corr = nullptr;
void set_adjustedtpcm_corr()
{
  adjustedtpcm_corr = &AdjustedtpcmWichertAziz::get_instance(); // mandotory here!
  set_correlation(adjustedtpcm_corr_arg, "adjustedtpcm", adjustedtpcm_corr, false);
}

ValueArg<string> zfactor_corr_arg =
  { "", "zfactor", "Correlation for zfactor", false, "",
    "Correlation for zfactor", cmd };
const Correlation * zfactor_corr = nullptr;
void set_zfactor_corr()
{
  set_correlation(zfactor_corr_arg, "zfactor", zfactor_corr, true);
}

ValueArg<string> ug_corr_arg =
  { "", "ug", "Correlation for ug", false, "UgLeeGE", "Correlation for ug", cmd };
const Correlation * ug_corr = nullptr;
void set_ug_corr()
{
  ug_corr = &UgLeeGE::get_instance(); // mandatory here
  set_correlation(ug_corr_arg, "ug", ug_corr, false);
}

ValueArg<string> bwb_corr_arg =
  { "", "bwb", "Correlation for bwb", false, "", "Correlation for bwb", cmd };
const Correlation * bwb_corr = nullptr;
void set_bwb_corr()
{
  set_correlation(bwb_corr_arg, "bwb", bwb_corr, true);
}

ValueArg<string> bwa_corr_arg =
  { "", "bwa", "Correlation for bwa", false, "", "Correlation for bwa", cmd };
const Correlation * bwa_corr = nullptr;
void set_bwa_corr()
{
  set_correlation(bwa_corr_arg, "bwa", bwa_corr, true);
}

ValueArg<string> uw_corr_arg =
  { "", "uw", "Correlation for uw", false, "", "Correlation for uw", cmd };
const Correlation * uw_corr = nullptr;
void set_uw_corr()
{
  set_correlation(uw_corr_arg, "uw", uw_corr, true);
}

ValueArg<string> pw_corr_arg =
  { "", "pw", "Correlation for pw", false, "", "Correlation for pw", cmd };
const Correlation * pw_corr = nullptr;
void set_pw_corr()
{
  set_correlation(pw_corr_arg, "pw", pw_corr, true);
}

ValueArg<string> cwb_corr_arg =
  { "", "cwb", "Correlation for cwb", false, "", "Correlation for cwb", cmd };
const Correlation * cwb_corr = nullptr;
void set_cwb_corr()
{
  set_correlation(cwb_corr_arg, "cwb", cwb_corr, true);
}

ValueArg<string> cwa_corr_arg =
  { "", "cwa", "Correlation for cwa", false, "", "Correlation for cwa", cmd };
const Correlation * cwa_corr = nullptr;
void set_cwa_corr()
{
  set_correlation(cwa_corr_arg, "cwa", cwa_corr, true);
}

ValueArg<string> rsw_corr_arg =
  { "", "rsw", "Correlation for rsw", false, "", "Correlation for rsw", cmd };
const Correlation * rsw_corr = nullptr;
void set_rsw_corr()
{
  set_correlation(rsw_corr_arg, "rsw", rsw_corr, true);
}

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
  auto t_unit = test_unit_change("t", Fahrenheit::get_instance());
  set_range(t_range.getValue(), "t", *t_unit, t_values);
}

ValueArg<RangeDesc> p_range =
  { "", "p", "range spec for pressure", true, RangeDesc(),
    "range spec \"min max n\" for pressure", cmd };
DynList<Correlation::NamedPar> p_values;
void set_p_range()
{
  auto p_unit = test_unit_change("p", psia::get_instance());
  set_range(p_range.getValue(), "p", *p_unit, p_values);
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

ValueArg<string> below_corr_arg = { "", "below", "below correlation name", false, 
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

DefinedCorrelation
define_correlation(double pb_val,
		   const Correlation * below_corr_ptr, double cb, double mb,
		   const Correlation * above_corr_ptr,
		   double ca = 0, double ma = 1)
{
  double max_p = psia::get_instance().max_val;
  DefinedCorrelation ret("p", psia::get_instance());
  ret.add_tuned_correlation(below_corr_ptr, psia::get_instance().min_val, pb_val,
			    cb, mb);
  ret.add_tuned_correlation(above_corr_ptr, nextafter(pb_val, max_p), max_p,
			    ca, ma);
  return ret;
}

DefinedCorrelation
define_correlation(double pb_val,
		   const Correlation * below_corr_ptr, 
		   const Correlation * above_corr_ptr)
{
  double max_p = psia::get_instance().max_val;
  DefinedCorrelation ret("p", psia::get_instance());
  ret.add_tuned_correlation(below_corr_ptr, psia::get_instance().min_val, pb_val,
			    0, 1);
  ret.add_tuned_correlation(above_corr_ptr, nextafter(pb_val, max_p), max_p,
			    0, 1);
  return ret;
}

DefinedCorrelation target_correlation(double pb_val)
{
  return define_correlation(pb_val, below_corr_ptr,
			    cb_arg.getValue(), mb_arg.getValue(),
			    above_corr_ptr, 
			    ca_arg.getValue(), ma_arg.getValue());
}

DefinedCorrelation
set_rs_correlation(double pb_val, double rsb, const Correlation * rs_corr,
		   double c, double m)
{
  DefinedCorrelation ret = define_correlation(pb_val, rs_corr, c, m,
					      &RsAbovePb::get_instance());
  ret.set_max(rsb);
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

void print_parameters(const DynList<Correlation::NamedPar> & l)
{
  l.for_each([] (const auto & par)
	     {
	       cout << get<1>(par) << " = " << get<2>(par) << " ";
	     });
  cout << endl;
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

template <typename ... Args> inline
VtlQuantity compute(const Correlation * corr_ptr,
		    double c, double m, bool check,
		    DynList<Correlation::NamedPar> & pars_list, Args ... args)
{
  size_t n = insert_in_container(pars_list, args...);
  VtlQuantity ret = corr_ptr->tuned_compute_by_names(pars_list, c, m, check);
  for (; n; --n)
    pars_list.remove_first();
  return ret;
}

template <typename ... Args> inline
double compute(const DefinedCorrelation & corr, bool check,
	       DynList<Correlation::NamedPar> & pars_list, Args ... args)
{
  size_t n = insert_in_container(pars_list, args ...);
  double ret = corr.compute_by_names(pars_list, check);
  for (; n; --n) 
    pars_list.remove_first();
  return ret;
}

template <typename ... Args> inline
VtlQuantity compute(const Correlation * corr_ptr,
		    double c, double m, bool check, Args ... args)
{
  DynList<Correlation::NamedPar> pars_list;
  return compute(corr_ptr, c, m, check, pars_list, args...);
}

template <typename ... Args> inline
double compute(const DefinedCorrelation & corr, bool check, Args ... args)
{
  DynList<Correlation::NamedPar> pars_list;
  return compute(corr, check, pars_list, args...);
}

//

template <typename ... Args> inline
VtlQuantity compute(const Correlation * corr_ptr, double c, double m, bool check,
		    ParList & pars_list, Args ... args)
{
  insert_in_container(pars_list, args...);
  VtlQuantity ret = corr_ptr->tuned_compute_by_names(pars_list, c, m, check);
  remove_from_container(pars_list, args ...);    
  return ret;
}

template <typename ... Args> inline
double compute(const DefinedCorrelation & corr, bool check,
	       ParList & pars_list, Args ... args)
{
  insert_in_container(pars_list, args ...);
  double ret = corr.compute_by_names(pars_list, check);
  remove_from_container(pars_list, args ...);
  return ret;
}

template <typename ... Args> inline
VtlQuantity __compute(const Correlation * corr_ptr,
		    double c, double m, bool check, Args ... args)
{
  ParList pars_list;
  return compute(corr_ptr, c, m, check, pars_list, args...);
}

template <typename ... Args> inline
double __compute(const DefinedCorrelation & corr, bool check, Args ... args)
{
  ParList pars_list;
  return compute(corr, check, pars_list, args...);
}

/// target, p, t
DynList<DynList<double>> generate_rs_values()
{
  auto pars_list = compute_pb_and_load_constant_parameters();

  DynList<DynList<double>> vals; /// target, p, t
  DynList<double> row;

  for (auto t_it = t_values.get_it(); t_it.has_curr(); t_it.next())
    {
      auto t_par = t_it.get_curr();
      pars_list.insert(t_par);
      row.insert(get<2>(t_par));

      auto pb_val = compute(pb_corr, c_pb_arg.getValue(), 1, check,
			    pb_pars, t_par);

      pars_list.insert(make_tuple(true, "pb", pb_val.raw(), &pb_val.unit));
      auto rs_corr =
	set_rs_correlation(pb_val.raw(), rsb_arg.getValue(), below_corr_ptr,
			   cb_arg.getValue(), mb_arg.getValue());

      for (auto p_it = p_values.get_it(); p_it.has_curr(); p_it.next())
	{
	  auto p_par = p_it.get_curr();

	  auto val = compute(rs_corr, check, pars_list, p_par);

	  size_t n = row.ninsert(get<2>(p_par), val);
	  vals.append(row);
	  row.mutable_drop(n);
	}

      row.remove_first(); // t_par

      pars_list.mutable_drop(2); // pb_val, t_par
    }

  return vals;
}

/// Retorna la lista de parámetros necesarios por el conjunto de
/// correlaciones que están en l
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
  test_parameter(required_pars, nacl_concentration_par, pars_list);

  return pars_list;
}



/// target, p, t
DynList<DynList<double>> generate_bo_values()
{
  assert(rs_corr);
  auto bo_pars_list = compute_pb_and_load_constant_parameters();
  
  auto rs_pars_list =
    load_constant_parameters({rs_corr, &RsAbovePb::get_instance()});

  DynList<DynList<double>> vals; /// target, p, t
  DynList<double> row;

  for (auto t_it = t_values.get_it(); t_it.has_curr(); t_it.next())
    {
      auto t_par = t_it.get_curr();

      row.insert(get<2>(t_par));

      auto pb_val =
	compute(pb_corr, c_pb_arg.getValue(), 1, check, pb_pars, t_par);
      auto pb_par = make_tuple(true, "pb", pb_val.raw(), &pb_val.unit);

      auto bo_corr = target_correlation(pb_val.raw());
      bo_pars_list.ninsert(t_par, pb_par);

      auto rs_corr = define_correlation(pb_val.raw(), ::rs_corr,
					c_rs_arg.getValue(), m_rs_arg.getValue(),
					&RsAbovePb::get_instance());
      rs_pars_list.ninsert(t_par, pb_par);

      // último bo de la correlación bob. Este será el punto de partida de boa
      auto bobp =
	compute(below_corr_ptr, cb_arg.getValue(),
		mb_arg.getValue(), check, bo_pars_list,
		make_tuple(true, "p", pb_val.raw(), &pb_val.unit),
		make_tuple(true, "rs", get<2>(rsb_par), get<3>(rsb_par)));

      bo_pars_list.insert(make_tuple(true, "bobp", bobp.raw(), &bobp.unit));

      for (auto p_it = p_values.get_it(); p_it.has_curr(); p_it.next())
	{
	  auto p_par = p_it.get_curr();

	  auto rs = compute(rs_corr, check, rs_pars_list, p_par);

	  auto bo = compute(bo_corr, check, bo_pars_list, p_par,
			    make_tuple(true, "rs", rs, &::rs_corr->unit));

	  size_t n = row.ninsert(get<2>(p_par), bo);
	  vals.append(row);
	  row.mutable_drop(n);
	}

      row.remove_first(); // t_par

      rs_pars_list.mutable_drop(2); // pb_par, t_par

      bo_pars_list.mutable_drop(3); // bobp, pb_val, t_par
    }

  return vals;
}

/// target, p, t
DynList<DynList<double>> generate_uo_values()
{
  assert(rs_corr);
  auto uo_pars_list = compute_pb_and_load_constant_parameters();
  
  auto rs_pars_list =
    load_constant_parameters({rs_corr, &RsAbovePb::get_instance()});

  auto uod_pars_list = load_constant_parameters({uod_corr});

  DynList<DynList<double>> vals; /// target, p, t
  DynList<double> row;

  for (auto t_it = t_values.get_it(); t_it.has_curr(); t_it.next())
    {
      auto t_par = t_it.get_curr();
      row.insert(get<2>(t_par));

      auto pb_val =
	compute(pb_corr, c_pb_arg.getValue(), 1, check, pb_pars, t_par);
      auto pb_par = make_tuple(true, "pb", pb_val.raw(), &pb_val.unit);

      auto uod_val = compute(uod_corr, 0, 1, check, uod_pars_list, t_par);

      rs_pars_list.ninsert(t_par, pb_par);
      auto rs_corr = define_correlation(pb_val.raw(), ::rs_corr,
					c_rs_arg.getValue(), m_rs_arg.getValue(),
					&RsAbovePb::get_instance());

      auto uo_corr = target_correlation(pb_val.raw());
      uo_pars_list.ninsert(t_par, pb_par,
			   make_tuple(true, "uod", uod_val.raw(), &uod_val.unit));

      auto uobp =
	compute(below_corr_ptr, cb_arg.getValue(), mb_arg.getValue(),
		check, uo_pars_list, 
		make_tuple(true, "p", pb_val.raw(), &pb_val.unit),
		make_tuple(true, "rs", get<2>(rsb_par), get<3>(rsb_par)));

      uo_pars_list.insert(make_tuple(true, "uobp", uobp.raw(), &uobp.unit));

      for (auto p_it = p_values.get_it(); p_it.has_curr(); p_it.next())
	{
	  auto p_par = p_it.get_curr();

	  auto rs = compute(rs_corr, check, rs_pars_list, p_par);

	  auto uo = compute(uo_corr, check, uo_pars_list, p_par,
			    make_tuple(true, "rs", rs, &::rs_corr->unit));

	  size_t n = row.ninsert(get<2>(p_par), uo);
	  vals.append(row);
	  row.mutable_drop(n);
	}

      row.remove_first(); // t_par

      rs_pars_list.mutable_drop(2); // pb_par, t_par

      uo_pars_list.mutable_drop(4); // uobp, pb_val, uod_val, t_par
    }

  return vals;
}

# define INVALID_VALUE numeric_limits<double>::max()

void print_row(const DynList<double> & row)
{
  for (auto it = row.get_it(); it.has_curr(); it.next())
    {
      const auto & val = it.get_curr();
      if (val != INVALID_VALUE)
	cout << val;
      if (&val != &row.get_last())
	cout << ",";
    }
  cout << endl;
}

void generate_grid()
{
  set_check();
  set_api();
  set_rsb();
  set_yg();
  set_tsep();
  set_psep();
  set_pb();
  set_h2s_concentration();
  set_co2_concentration();
  set_n2_concentration();
  set_nacl_concentration();
  
  set_rs_corr(true);
  set_bob_corr(true);
  set_boa_corr(true);
  set_uod_corr(true);
  set_cob_corr(true);
  set_coa_corr(true);
  set_uob_corr(true);
  set_uoa_corr(true);
  set_ppchc_corr();
  set_tpchc_corr();
  set_ppcm_mixing_corr();
  set_tpcm_mixing_corr();
  set_adjustedppcm_corr();
  set_adjustedtpcm_corr();
  set_zfactor_corr();
  set_ug_corr();
  set_bwb_corr();
  set_bwa_corr();
  set_uw_corr();
  set_pw_corr();
  set_rsw_corr();
  set_cwb_corr();
  set_cwa_corr();

  set_t_range();
  set_p_range();
  if (below_corr_arg.isSet())
    error_msg("below option is incompatible with grid option");
  if (above_corr_arg.isSet())
    error_msg("above option is incompatible with grid option");

  // cálculo de constantes para Z
  auto yghc =
    YghcWichertAziz::get_instance().call(yg, n2_concentration,
					 co2_concentration, h2s_concentration);
  auto ppchc = compute(ppchc_corr, 0, 1, check, NPAR(yghc),
		       NPAR(n2_concentration), NPAR(co2_concentration),
		       NPAR(h2s_concentration));
  auto ppcm = compute(ppcm_mixing_corr, 0, 1, check, NPAR(ppchc),
		      NPAR(n2_concentration), NPAR(co2_concentration),
		      NPAR(h2s_concentration));
		       
  auto tpchc = compute(tpchc_corr, 0, 1, check, NPAR(yghc));
  auto tpcm = compute(tpcm_mixing_corr, 0 ,1, check, NPAR(tpchc),
		      NPAR(n2_concentration), NPAR(co2_concentration),
		      NPAR(h2s_concentration));

  auto adjustedppcm = compute(adjustedppcm_corr, 0, 1, check, NPAR(ppcm),
			      NPAR(tpcm), NPAR(co2_concentration),
			      NPAR(h2s_concentration));

  auto adjustedtpcm = compute(adjustedtpcm_corr, 0, 1, check, NPAR(ppcm),
			      NPAR(tpcm), NPAR(co2_concentration),
			      NPAR(h2s_concentration));  

  auto pb_pars_list = load_constant_parameters({pb_corr});
  auto rs_pars_list =
    load_constant_parameters({rs_corr, &RsAbovePb::get_instance()});
  auto uod_pars_list = load_constant_parameters({uod_corr});
  auto bo_pars_list = load_constant_parameters({bob_corr, boa_corr});
  auto co_pars_list = load_constant_parameters({cob_corr, coa_corr});
  auto uo_pars_list = load_constant_parameters({uob_corr, uoa_corr});
  auto po_pars_list = load_constant_parameters({&PobBradley::get_instance(),
	&PoaBradley::get_instance()});
  auto ug_pars_list = load_constant_parameters({ug_corr});
  auto bw_pars_list = load_constant_parameters({bwb_corr, bwa_corr});
  auto uw_pars_list = load_constant_parameters({uw_corr});
  auto pw_pars_list = load_constant_parameters({pw_corr});
  auto rsw_pars_list = load_constant_parameters({rsw_corr});
  auto cw_pars_list = load_constant_parameters({cwb_corr, cwa_corr});
  auto cwa_pars_list = load_constant_parameters({cwa_corr});

  // Nuevo valor tiene que entrar de 1ro aqui
  cout << "cw " << cwb_corr->unit.name
       << ",rsw " << rsw_corr->unit.name
       << ",pw " << pw_corr->unit.name
       << ",uw " << uw_corr->unit.name
       << ",bw " << bwb_corr->unit.name
       << ",pg " << Pg::get_instance().unit.name
       << ",ug " << ug_corr->unit.name
       << ",bg " << Bg::get_instance().unit.name 
       << ",zfactor " << Zfactor::get_instance().name
       << ",po " << PobBradley::get_instance().unit.name
       << ",uo " << uob_corr->unit.name
       << ",bo " << bob_corr->unit.name
       << ",co " << cob_corr->unit.name
       << ",rs " << ::rs_corr->unit.name
       << ",p " << get<3>(p_values.get_first())->name
       << ",uobp " << uob_corr->unit.name
       << ",bobp " << bob_corr->unit.name
       << ",uod " << uod_corr->unit.name
       << ",pb " << pb_corr->unit.name
       << ",t " << get<3>(t_values.get_first())->name
       << endl;

  auto rs_pb = make_tuple(true, "rs", get<2>(rsb_par), get<3>(rsb_par)); 
  DynList<double> row;
  for (auto t_it = t_values.get_it(); t_it.has_curr(); t_it.next())
    {
      auto t_par = t_it.get_curr();
      auto tpr_val = Tpr::get_instance().call(par(t_par), adjustedtpcm);

      auto pb_val = // calculo de pb
	compute(pb_corr, c_pb_arg.getValue(), 1, check, pb_pars, t_par);
      auto pb = pb_val.raw();
      auto pb_par = make_tuple(true, "pb", pb, &pb_val.unit);
      auto p_pb = make_tuple(true, "p", pb, &pb_val.unit);

      // cálculo de uod
      auto uod_val = compute(uod_corr, 0, 1, check, uod_pars_list, t_par);

      rs_pars_list.ninsert(t_par, pb_par);
      auto rs_corr = define_correlation(pb, ::rs_corr, c_rs_arg.getValue(),
					m_rs_arg.getValue(),
					&RsAbovePb::get_instance());

      co_pars_list.ninsert(t_par, pb_par);
      auto co_corr =
	define_correlation(pb,
			   cob_corr, c_cob_arg.getValue(), m_cob_arg.getValue(),
			   coa_corr, c_coa_arg.getValue(), m_coa_arg.getValue());
      auto bo_corr =
	define_correlation(pb,
			   bob_corr, c_bob_arg.getValue(), m_bob_arg.getValue(),
			   boa_corr, c_boa_arg.getValue(), m_boa_arg.getValue());

      uo_pars_list.ninsert(t_par, pb_par,
			   make_tuple(true, "uod", uod_val.raw(), &uod_val.unit));
      auto uo_corr =
	define_correlation(pb,
			   uob_corr, c_uob_arg.getValue(), m_uob_arg.getValue(),
			   uoa_corr, c_uoa_arg.getValue(), m_uoa_arg.getValue());
      
      auto po_corr = define_correlation(pb, &PobBradley::get_instance(),
					&PoaBradley::get_instance());

      auto bw_corr = define_correlation(pb, bwb_corr, bwa_corr);

      auto cw_corr = define_correlation(pb, cwb_corr, cwa_corr);

      // cálculo de bobp
      bo_pars_list.insert(t_par);
      auto bobp = compute(bob_corr, c_bob_arg.getValue(), m_bob_arg.getValue(),
			  check, bo_pars_list, p_pb, rs_pb);
      
      /// Calculo de uobp
      auto uobp = compute(uob_corr, c_uob_arg.getValue(), m_uob_arg.getValue(),
			  check, uo_pars_list, p_pb, rs_pb);

      bo_pars_list.ninsert(pb_par,
			   make_tuple(true, "bobp", bobp.raw(), &bobp.unit));

      uo_pars_list.insert(make_tuple(true, "uobp", uobp.raw(), &uobp.unit));

      // Cálculo de pobp
      auto pobp = compute(&PobBradley::get_instance(), 0, 1, check, po_pars_list,
			  rs_pb, make_tuple(true, "bob", bobp.raw(), &bobp.unit));

      po_pars_list.ninsert(pb_par,
			   make_tuple(true, "pobp", pobp.raw(), &pobp.unit));
      ug_pars_list.ninsert(npar("tpr", tpr_val), t_par);
      bw_pars_list.insert(t_par);
      uw_pars_list.insert(t_par);
      pw_pars_list.insert(t_par);
      rsw_pars_list.insert(t_par);
      cw_pars_list.insert(t_par);
      cwa_pars_list.insert(t_par);

      //auto cwpb = compute(cwa_corr, 0, 1, check, cw_pars_list,
      size_t n =
	row.ninsert(get<2>(t_par), pb, uod_val.raw(), bobp.raw(), uobp.raw());

      for (auto p_it = p_values.get_it(); p_it.has_curr(); p_it.next())
	{
	  auto p_par = p_it.get_curr();
	  auto ppr_val = Ppr::get_instance().call(par(p_par), adjustedppcm);

	  auto rs = compute(rs_corr, check, rs_pars_list, p_par);
	  auto rs_par = make_tuple(true, "rs", rs, &::rs_corr->unit);

	  auto co = compute(co_corr, check, co_pars_list, p_par);
	  auto co_par = make_tuple(true, "co", co, co_corr.result_unit);

	  auto bo = compute(bo_corr, check, bo_pars_list, p_par, rs_par);

	  auto uo = compute(uo_corr, check, uo_pars_list, p_par, rs_par);

	  auto po = compute(po_corr, check, po_pars_list, p_par, rs_par, co_par,
			    make_tuple(true, "bob", bo, bo_corr.result_unit));

	  double zfactor = INVALID_VALUE, bg = INVALID_VALUE, ug = INVALID_VALUE,
	    pg = INVALID_VALUE, bw = INVALID_VALUE, pw = INVALID_VALUE,
	    rsw = INVALID_VALUE, cw = INVALID_VALUE;
	  try
	    {
	      auto z = zfactor_corr->compute({ppr_val, tpr_val});
	      zfactor = z.raw();
	      auto __bg = Bg::get_instance().call(par(t_par), par(p_par), z);
	      bg = __bg.raw();
	      ug = compute(ug_corr, 0, 1, check, ug_pars_list,
			   p_par, p_par, npar("ppr", ppr_val), NPAR(z)).raw();
	      pg = Pg::get_instance().call(yg, par(t_par), par(p_par), z).raw();

	      // TODO TMP: esta fallando. Por eso se pone aquí
	      bw = compute(bw_corr, check, bw_pars_list, p_par);
	      auto bw_par = make_tuple(true, "bw", bw, bw_corr.result_unit);
	      // TODO: idem que anterior
	      pw = compute(pw_corr, 0, 1, check, pw_pars_list,
			   p_par, bw_par).raw();
	      auto __rsw = compute(rsw_corr, 0, 1, check, rsw_pars_list, p_par);
	      auto rsw_par = npar("rsw", __rsw);
	      rsw = __rsw.raw();

	      auto cwa = compute(cwa_corr, 0, 1, check, cwa_pars_list,
				 p_par, rsw_par);
	      cw = compute(cw_corr, check, cw_pars_list, p_par, NPAR(z),
			   npar("bg", __bg), rsw_par, bw_par, NPAR(cwa));
	    }
	  catch (domain_error & e)
	    {
	      //cout << e.what() << endl;
	      /* ignore it! */
	    }

	  auto ppw = PpwSpiveyMN::get_instance().call(par(t_par), par(p_par));

	  auto uw = compute(uw_corr, 0, 1, check, uw_pars_list,
			    p_par, NPAR(ppw)).raw();

	  size_t n = row.ninsert(get<2>(p_par), rs, co, bo, uo, po, zfactor, bg,
				 ug, pg, bw, uw, pw, rsw, cw);

	  assert(row.size() == 20);

	  print_row(row);

	  row.mutable_drop(n);
	}

      row.mutable_drop(n);

      rs_pars_list.mutable_drop(2); // pb_par, t_par
      co_pars_list.mutable_drop(2); // pb_par, t_par
      bo_pars_list.mutable_drop(3); // bobp, pb_val, t_par
      uo_pars_list.mutable_drop(4); // uobp, pb_val, uod_val, t_par
      po_pars_list.mutable_drop(2); // pb, pobp
      ug_pars_list.mutable_drop(2); // t_par, tpr
      bw_pars_list.remove_first(); // t_par
      uw_pars_list.remove_first(); // t_par
      pw_pars_list.remove_first(); // t_par
      cw_pars_list.remove_first(); // t_par
      rsw_pars_list.remove_first(); // t_par
    }
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
  for (auto it = get_zip_it(suffix, t_array, v_array); it.has_curr(); it.next())
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

  if (grid_arg.getValue())
    {
      generate_grid();
      return 0;
    }

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
