
# include <memory>

# include <tclap/CmdLine.h>

# include <ah-zip.H>
# include <ah-dispatcher.H>
# include <tpl_dynMapTree.H>

# include <json.hpp>

using json = nlohmann::json;

# include <correlations/pvt-correlations.H>
# include <correlations/defined-correlation.H>

using namespace std;
using namespace TCLAP;

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
  { "api", "rsb", "yg", "tsep", "tsep2", "t", "p", "psep", "h2s-concentration",
    "co2-concentration", "n2-concentration", "nacl-concentration", "org" };
struct ArgUnit
{
  string par_name;
  string unit_name;

  ArgUnit & operator = (const string & str)
  {
    istringstream iss(str);
    if (not (iss >> par_name >> unit_name))
      throw TCLAP::ArgParseException(str + " is not a pair par-name unit");

    if (not par_name_tbl.contains(par_name))
      throw TCLAP::ArgParseException(par_name + " is an invalid parameter name");

    return *this;
  }

  ArgUnit() {}

  friend ostream& operator << (ostream &os, const ArgUnit & a) 
  {
    return os << a.par_name << " " << a.unit_name;
  }
};

namespace TCLAP
{
  template<> struct ArgTraits<ArgUnit> { typedef StringLike ValueCategory; };
}

// Unit change specification. Suitable for any parameter
MultiArg<ArgUnit> unit = { "", "unit", "unit \"par-name unit\"", false,
			   "unit \"par-name unit\"", cmd };

// Checks whether the parameter par_name has a change of
// unity. ref_unit is the default unit of the parameter. If there was
// no change specification for par_name, then returns ref_unit
const Unit * test_unit_change(const string & par_name, const Unit & ref_unit)
{
  if (not par_name_tbl.contains(par_name))
    {
      cout << "for option --unit " << par_name << ": invalid parameter name"
	   << endl;
      abort();
    }
  
  const Unit * ret = &ref_unit;
  for (const auto & par : unit.getValue()) // traverse list of changes
    if (par.par_name == par_name)
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

// helper to handle parameter passing to correlations

// construct a VtlQuantity from a parameter by name
inline VtlQuantity par(const Correlation::NamedPar & par)
{
  return VtlQuantity(*get<3>(par), get<2>(par));
}

// construct a parameter by name name from an amount (VtlQuantity or Quantity <Unit>)
inline Correlation::NamedPar npar(const string & name, const BaseQuantity & p)
{
  return Correlation::NamedPar(true, name, p.raw(), &p.unit);
}

inline Correlation::NamedPar npar(const string & name, double v, const Unit * unit)
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

// Mandatory arguments in the command line that are validated in the
// command line parsing

ValueArg<double> api_arg = { "", "api", "api", true, 0, "api", cmd };
Correlation::NamedPar api_par;
void set_api()
{
  auto api_unit = test_unit_change("api", Api::get_instance());
  api_par = npar("api", api_arg.getValue(), api_unit);
}

ValueArg<double> rsb_arg = { "", "rsb", "rsb", true, 0, "rsb in scf/STB", cmd };
Correlation::NamedPar rsb_par;
const Unit * rsb_unit = nullptr;
void set_rsb()
{
  rsb_unit = test_unit_change("rsb", SCF_STB::get_instance());
  rsb_par = npar("rsb", rsb_arg.getValue(), rsb_unit);
}

ValueArg<double> yg_arg = { "", "yg", "yg", true, 0, "yg in Sgg", cmd };
Correlation::NamedPar yg_par;
VtlQuantity yg;
const Unit * yg_unit = nullptr;
void set_yg()
{
  yg_unit = test_unit_change("yg", Sgg::get_instance());
  yg_par = npar("yg", yg_arg.getValue(), yg_unit);
  yg.set(par(yg_par));
}

// optional command line arguments

ValueArg<double> tsep_arg = { "", "tsep", "separator temperature", false, 0,
			      "separator temperature in degF", cmd };
Correlation::NamedPar tsep_par;
VtlQuantity tsep;
const Unit * tsep_unit = nullptr;
void set_tsep()
{
  tsep_unit = test_unit_change("tsep", Fahrenheit::get_instance());
  tsep_par = make_tuple(tsep_arg.isSet(), "tsep",
			tsep_arg.getValue(), tsep_unit);
  tsep.set(par(tsep_par));
}

ValueArg<double> tsep2_arg = { "", "tsep2", "second separator temperature",
			       false, 0,
			       "second separator temperature in degF", cmd };
Correlation::NamedPar tsep2_par;
VtlQuantity tsep2;
const Unit * tsep2_unit = nullptr;
void set_tsep2()
{
  tsep2_unit = test_unit_change("tsep2", Fahrenheit::get_instance());
  tsep2_par = make_tuple(tsep2_arg.isSet(), "tsep2",
			 tsep2_arg.getValue(), tsep2_unit);
  tsep2.set(par(tsep2_par));
}

ValueArg<double> ogr_arg = { "", "ogr", "Condensed gas ratio",
			       false, 0, "Condensed gas ratio", cmd };
Correlation::NamedPar ogr_par;
VtlQuantity ogr;
const Unit * ogr_unit = nullptr;
void set_ogr()
{
  ogr_unit = test_unit_change("ogr", STB_MMscf::get_instance());
  ogr_par = make_tuple(ogr_arg.isSet(), "ogr",
		       ogr_arg.getValue(), ogr_unit);
  ogr.set(par(ogr_par));
}

ValueArg<double> psep_arg = { "", "psep", "separator pressure", false, 0,
			      "separator pressure in psia", cmd };
Correlation::NamedPar psep_par;
VtlQuantity psep;
const Unit * psep_unit = nullptr;
void set_psep()
{
  psep_unit = test_unit_change("psep", psia::get_instance());
  psep_par = make_tuple(psep_arg.isSet(), "psep",
			psep_arg.getValue(), psep_unit);
  psep.set(par(psep_par));
}

ValueArg<double> n2_concentration_arg =
  { "", "n2-concentration", "n2-concentration", false, 0,
    "n2-concentration in MolePercent", cmd };
Correlation::NamedPar n2_concentration_par;
VtlQuantity n2_concentration;
const Unit * n2_concentration_unit = nullptr;
void set_n2_concentration()
{
  n2_concentration_unit = test_unit_change("n2-concentration",
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
VtlQuantity co2_concentration;
const Unit * co2_concentration_unit = nullptr;
void set_co2_concentration()
{
  co2_concentration_unit = test_unit_change("co2-concentration",
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
VtlQuantity h2s_concentration;
const Unit * h2s_concentration_unit = nullptr;
void set_h2s_concentration()
{
  h2s_concentration_unit = test_unit_change("h2s-concentration",
					    MolePercent::get_instance());
  h2s_concentration_par = make_tuple(h2s_concentration_arg.isSet(),
				     "h2s_concentration",
				     h2s_concentration_arg.getValue(),
				     h2s_concentration_unit);
  h2s_concentration.set(par(h2s_concentration_par));
}

ValueArg<double> nacl_concentration_arg =
  { "", "nacl-concentration", "nacl-concentration", false, 0,
    "nacl-concentration in mol_NaCl/Kg_H2O", cmd };
Correlation::NamedPar nacl_concentration_par;
VtlQuantity nacl_concentration;
const Unit * nacl_concentration_unit = nullptr;
void set_nacl_concentration()
{
  nacl_concentration_unit = test_unit_change("nacl-concentration",
					     Molality_NaCl::get_instance());
  nacl_concentration_par = make_tuple(nacl_concentration_arg.isSet(),
				      "nacl_concentration",
				      nacl_concentration_arg.getValue(),
				      nacl_concentration_unit);
  nacl_concentration.set(par(nacl_concentration_par));
}

ValueArg<double> c_pb_arg = { "", "c-pb", "pb adjustment", false, 0,
			      "pb adjustment", cmd };
ValueArg<string> pb_arg = { "", "pb", "correlation for pb", false, "",
			    "correlation for pb", cmd };
const Correlation * pb_corr = nullptr;
ParList pb_pars;
void set_pb_corr()
{
  assert(yg_arg.isSet() and rsb_arg.isSet() and api_arg.isSet());
  pb_corr = Correlation::search_by_name(pb_arg.getValue());
  if (pb_corr == nullptr)
    error_msg(pb_arg.getValue() + " correlation not found");

  if (pb_corr->target_name() != "pb")
    error_msg(pb_arg.getValue() + " correlation is not for pb");

  pb_pars.insert(yg_par);
  pb_pars.insert(rsb_par);
  pb_pars.insert(api_par);
}

// Initialize a correlation specified from the command line via
// corr_name_arg. Verify that the correlation is for the target_name
// property and if so, then the found correlation is placed in the
// output parameter corr_ptr.
//
// The force_set parameter indicates that the correlation is required
// on the command line.
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
  ppchc_corr = &PpchcStanding::get_instance(); // mandotory here!
  set_correlation(ppchc_corr_arg, "ppchc", ppchc_corr, false);
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
  tpchc_corr = &TpchcStanding::get_instance(); // by default
  set_correlation(tpchc_corr_arg, "tpchc", tpchc_corr, false);
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
  zfactor_corr = &ZfactorDranchukAK::get_instance(); // by default
  set_correlation(zfactor_corr_arg, "zfactor", zfactor_corr, false);
}

ValueArg<string> cg_corr_arg =
  { "", "cg", "Correlation for cg", false, "", "Correlation for cg", cmd };
const Correlation * cg_corr = nullptr;
void set_cg_corr()
{
  cg_corr = &CgMattarBA::get_instance();
  set_correlation(cg_corr_arg, "cg", cg_corr, false);
}

ValueArg<string> ug_corr_arg =
  { "", "ug", "Correlation for ug", false, "", "Correlation for ug", cmd };
const Correlation * ug_corr = nullptr;
void set_ug_corr()
{
  ug_corr = &UgCarrKB::get_instance(); // by default
  set_correlation(ug_corr_arg, "ug", ug_corr, false);
}

ValueArg<string> bwb_corr_arg =
  { "", "bwb", "Correlation for bwb", false, "", "Correlation for bwb", cmd };
const Correlation * bwb_corr = nullptr;
void set_bwb_corr()
{
  bwb_corr = &BwbSpiveyMN::get_instance();
  set_correlation(bwb_corr_arg, "bwb", bwb_corr, false);
}

ValueArg<string> bwa_corr_arg =
  { "", "bwa", "Correlation for bwa", false, "", "Correlation for bwa", cmd };
const Correlation * bwa_corr = nullptr;
void set_bwa_corr()
{
  bwa_corr = &BwaSpiveyMN::get_instance();
  set_correlation(bwa_corr_arg, "bwa", bwa_corr, false);
}

ValueArg<string> uw_corr_arg =
  { "", "uw", "Correlation for uw", false, "", "Correlation for uw", cmd };
const Correlation * uw_corr = nullptr;
void set_uw_corr()
{
  uw_corr = &UwMcCain::get_instance();
  set_correlation(uw_corr_arg, "uw", uw_corr, false);
}

ValueArg<string> pw_corr_arg =
  { "", "pw", "Correlation for pw", false, "", "Correlation for pw", cmd };
const Correlation * pw_corr = nullptr;
void set_pw_corr()
{
  pw_corr = &PwSpiveyMN::get_instance();
  set_correlation(pw_corr_arg, "pw", pw_corr, false);
}

ValueArg<string> cwb_corr_arg =
  { "", "cwb", "Correlation for cwb", false, "", "Correlation for cwb", cmd };
const Correlation * cwb_corr = nullptr;
void set_cwb_corr()
{
  cwb_corr = &CwbSpiveyMN::get_instance();
  set_correlation(cwb_corr_arg, "cwb", cwb_corr, false);
}

ValueArg<string> cwa_corr_arg =
  { "", "cwa", "Correlation for cwa", false, "", "Correlation for cwa", cmd };
const Correlation * cwa_corr = nullptr;
void set_cwa_corr()
{
  cwa_corr = &CwaSpiveyMN::get_instance();
  set_correlation(cwa_corr_arg, "cwa", cwa_corr, false);
}

ValueArg<string> rsw_corr_arg =
  { "", "rsw", "Correlation for rsw", false, "", "Correlation for rsw", cmd };
const Correlation * rsw_corr = nullptr;
void set_rsw_corr()
{
  rsw_corr = &RswSpiveyMN::get_instance();
  set_correlation(rsw_corr_arg, "rsw", rsw_corr, false);
}

ValueArg<string> sgo_corr_arg =
  { "", "sgo", "Correlation for sgo", false, "", "Correlation for sgo", cmd };
const Correlation * sgo_corr = nullptr;
void set_sgo_corr()
{
  set_correlation(sgo_corr_arg, "sgo", sgo_corr, true);
}

ValueArg<string> sgw_corr_arg =
  { "", "sgw", "Correlation for sgw", false, "", "Correlation for sgw", cmd };
const Correlation * sgw_corr = nullptr;
void set_sgw_corr()
{
  sgw_corr = &SgwJenningsNewman::get_instance();
  set_correlation(sgw_corr_arg, "sgw", sgw_corr, false);
}

vector<string> grid_types =
  { "blackoil", "wetgas", "drygas", "brine", "gascondensated" };
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
  { "", "t", "min max n", true, RangeDesc(),
    "range spec \"min max n\" for temperature", cmd };
DynList<Correlation::NamedPar> t_values;
const Unit * t_unit = nullptr;
void set_t_range()
{
  t_unit = test_unit_change("t", Fahrenheit::get_instance());
  set_range(t_range.getValue(), "t", *t_unit, t_values);
}

ValueArg<RangeDesc> p_range =
  { "", "p", "min max n", true, RangeDesc(),
    "range spec \"min max n\" for pressure", cmd };
DynList<Correlation::NamedPar> p_values;
const Unit * p_unit = nullptr;
void set_p_range()
{
  p_unit = test_unit_change("p", psia::get_instance());
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

DefinedCorrelation
set_rs_correlation(const VtlQuantity & pb,
		   const VtlQuantity & rsb, const Correlation * rs_corr,
		   double c, double m)
{
  DefinedCorrelation ret = define_correlation(pb, rs_corr, c, m,
					      &RsAbovePb::get_instance());
  ret.set_max(rsb);
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

double temperature = 0, pressure = 0; // global values set during the grid generation
bool exception_thrown = false;

void store_exception(const string & corr_name, const exception & e)
{
  exception_thrown = true;
  ostringstream s;
  s << corr_name << ": " << VtlQuantity(*t_unit, temperature)
    << ", " << VtlQuantity(*p_unit, pressure) << ": " << e.what()
    << endl;
  exception_list.append(s.str());
}

/* Meta-inserts par into pars_list but stops if any parameter is invalid.

  Returns true if all parameters were valid (! = Invalid_Value)

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

template <typename ... Args> inline
VtlQuantity compute(const Correlation * corr_ptr,
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
  catch (exception & e)
    {
      if (report_exceptions)
	store_exception(corr_ptr->name, e);
      
      remove_from_container(pars_list, args ...);
    }
  return VtlQuantity();
}

template <typename ... Args> inline
VtlQuantity dcompute(const Correlation * corr_ptr, bool check,
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
  catch (exception & e)
    {
      if (report_exceptions)
	store_exception(corr_ptr->name, e);
      
      remove_from_container(pars_list, args ...);
    }
  return VtlQuantity();
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
VtlQuantity dcompute_noexcep(const Correlation * corr_ptr, bool check,
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
  catch (exception & e)
    {
      remove_from_container(pars_list, args ...);
      cout << "ERROR initializing " << correlation_call(corr_ptr, args...)
	   << "@ " << e.what();
      abort();
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

// Macro for creating `var` variable with value returned by correlation corr_name
# define CALL(corr_name, var, args...)					\
  VtlQuantity var;							\
  try									\
    {									\
      if (valid_args(args))						\
	var = corr_name::get_instance().call(args);			\
    }									\
  catch (exception & e)							\
    {									\
      store_exception(corr_name::get_instance().name, e);		\
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
	return false; // here the correlation would receive Invalid_Value and would fail
    }
  
  pars_list.insert(par);
  if (insert_in_pars_list(corr, p_q, pars_list, args...))
    return true;

  pars_list.remove(par); // If recursive insertion fails ==> remove par

  return false;
}

template <typename ... Args> inline
VtlQuantity compute(const DefinedCorrelation & corr, bool check,
		    const VtlQuantity & p_q,
		    ParList & pars_list, Args ... args)
{
  try
    {
      if (not insert_in_pars_list(corr, p_q, pars_list, args...))
	return VtlQuantity();
      auto ret = corr.compute_by_names(pars_list, check);
      remove_from_container(pars_list, args ...);
      return ret;
    }
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
VtlQuantity compute(const Correlation * corr_ptr,
		    double c, double m, bool check, Args ... args)
{ // static for creating it once and thus to gain time. But beware!
  // The function is not reentrant and can not be used in a
  // multithreaded environment
  static ParList pars_list;
  return compute(corr_ptr, c, m, check, pars_list, args...);
}

template <typename ... Args> inline
VtlQuantity dcompute(const Correlation * corr_ptr, bool check, Args ... args)
{ // static for creating it once and thus to gain time. But beware!
  // The function is not reentrant and can not be used in a
  // multithreaded environment
  static ParList pars_list;
  return dcompute(corr_ptr, check, pars_list, args...);
}

template <typename ... Args> inline
VtlQuantity
dcompute_noexcep(const Correlation * corr_ptr, bool check, Args ... args)
{ // static for creating it once and thus to gain time. But beware!
  // The function is not reentrant and can not be used in a
  // multithreaded environment
  static ParList pars_list;
  return dcompute_noexcep(corr_ptr, check, pars_list, args...);
}

template <typename ... Args> inline
VtlQuantity compute(const DefinedCorrelation & corr, bool check,
		    const VtlQuantity & p_q, Args ... args)
{ // static for creating it once and thus to gain time. But beware!
  // The function is not reentrant and can not be used in a
  // multithreaded environment
  static ParList pars_list;
  return compute(corr, check, pars_list, p_q, args...);
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
  test_parameter(required_pars, n2_concentration_par, pars_list);
  test_parameter(required_pars, co2_concentration_par, pars_list);
  test_parameter(required_pars, h2s_concentration_par, pars_list);
  test_parameter(required_pars, nacl_concentration_par, pars_list);

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

void print_row(const FixedStack<const VtlQuantity*> & row)
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

  for (long i = n - 1; i >= 0; --i)
    {
      const auto & val = ptr[i]->raw();
      if (val != Invalid_Value)
	printf("%f", val);
      //printf("%.17g", val); // uncomment this and comment above for maximum precision
      if (i > 0)
	printf(",");
    }
  printf("\n");
}

void print_row(const FixedStack<const VtlQuantity*> & row, bool is_pb)
{
  if (is_pb)
    printf("\"true\",");
  else
    printf("\"false\",");

  print_row(row);
}

// TODO: debe retornar un vector de unidades que se le pasa a
// print_row. Pero antes hay que determinar, por una tabla de cambios
// de unidad de correlación, cuáles columnas tienen cambios de
// unidad. De este modo print_row simplemente recibe lel valor en
// VtlQuantity y la conversión sucedería si fiese distinta
template <typename ... Args>
void print_csv_header(Args ... args)
{
  FixedStack<pair<string, const Unit*>> header;
  insert_in_container(header, args...);

  ostringstream s;
  const size_t n = header.size();
  pair<string, const Unit*> * ptr = &header.base();
  for (long i = n - 1; i >= 0; --i)
    {
      const pair<string, const Unit*> & val = ptr[i];
      printf("%s %s", val.first.c_str(), val.second->name.c_str());
      if (i > 0)
	printf(",");
    }
  
  printf("\n");
}

void generate_grid_blackoil()
{
  set_check(); // Initialization of constant data
  set_api();
  set_rsb();
  set_yg();
  set_tsep();
  set_psep();
  set_h2s_concentration();
  set_co2_concentration();
  set_n2_concentration();
  set_nacl_concentration();
  
  set_pb_corr();  
  set_rs_corr(true); // Initialization of correlations
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
  set_cg_corr();
  set_ug_corr();
  set_bwb_corr();
  set_bwa_corr();
  set_uw_corr();
  set_pw_corr();
  set_rsw_corr();
  set_cwb_corr();
  set_cwa_corr();
  set_sgo_corr();
  set_sgw_corr();
  report_exceptions = catch_exceptions.getValue();

  set_t_range();
  set_p_range();

  // Calculation of constants for Z
  auto yghc = dcompute_noexcep(YghcWichertAziz::correlation(), true, NPAR(yg),
			       NPAR(n2_concentration), NPAR(co2_concentration),
			       NPAR(h2s_concentration));
  auto ppchc = dcompute_noexcep(ppchc_corr, true, NPAR(yghc),
				NPAR(n2_concentration), NPAR(co2_concentration),
				NPAR(h2s_concentration));
  auto ppcm = dcompute_noexcep(ppcm_mixing_corr, true, NPAR(ppchc),
			       NPAR(n2_concentration), NPAR(co2_concentration),
			       NPAR(h2s_concentration));		       
  auto tpchc = tpchc_corr->compute(check, yghc);
  auto tpcm = dcompute_noexcep(tpcm_mixing_corr, true, NPAR(tpchc),
			       NPAR(n2_concentration), NPAR(co2_concentration),
			       NPAR(h2s_concentration));
  auto adjustedppcm = dcompute_noexcep(adjustedppcm_corr, true, NPAR(ppcm),
				       NPAR(tpcm), NPAR(co2_concentration),
				       NPAR(h2s_concentration));
  auto adjustedtpcm = dcompute_noexcep(adjustedtpcm_corr, true, NPAR(tpcm),
				       NPAR(co2_concentration),
				       NPAR(h2s_concentration));
  // End calculation constants for z

  // Initialization of correlation parameter lists
  auto pb_pars = load_constant_parameters({pb_corr});
  auto rs_pars = load_constant_parameters({rs_corr, &RsAbovePb::get_instance()});
  auto uod_pars = load_constant_parameters({uod_corr});
  auto bo_pars = load_constant_parameters({bob_corr, boa_corr});
  auto co_pars = load_constant_parameters({cob_corr, coa_corr});
  auto uo_pars = load_constant_parameters({uob_corr, uoa_corr});
  auto po_pars = load_constant_parameters({&PobBradley::get_instance(),
	&PoaBradley::get_instance()});
  auto ug_pars = load_constant_parameters({ug_corr});
  insert_in_container(ug_pars, npar("tpc", adjustedtpcm),
		      npar("ppc", adjustedppcm));
  auto bw_pars = load_constant_parameters({bwb_corr, bwa_corr});
  auto uw_pars = load_constant_parameters({uw_corr});
  auto pw_pars = load_constant_parameters({pw_corr});
  auto rsw_pars = load_constant_parameters({rsw_corr});
  auto cw_pars = load_constant_parameters({cwb_corr, cwa_corr});
  auto cwa_pars = load_constant_parameters({cwa_corr});
  ParList cg_pars; cg_pars.insert(npar("ppc", ppcm));
  auto sgo_pars = load_constant_parameters({sgo_corr});
  ParList sgw_pars;

  using P = pair<string, const Unit*>;
  print_csv_header(P("t", get<3>(t_values.get_first())),
		   P("pb", &pb_corr->unit),
		   P("uod", &uod_corr->unit),
		   P("p", get<3>(p_values.get_first())),
		   P("rs", &::rs_corr->unit),
		   P("co", &cob_corr->unit), 
		   P("bo", &bob_corr->unit),
		   P("uo", &uob_corr->unit),
		   P("po", &PobBradley::get_instance().unit),
		   P("zfactor", &Zfactor::get_instance()),
		   P("cg", &cg_corr->unit),
		   P("bg", &Bg::get_instance().unit),
		   P("ug", &ug_corr->unit),
		   P("pg", &Pg::get_instance().unit),
		   P("bw", &bwb_corr->unit),
		   P("uw", &uw_corr->unit),
		   P("pw", &pw_corr->unit),
		   P("rsw", &rsw_corr->unit),
		   P("cw", &cwb_corr->unit),
		   P("sgo", &sgo_corr->unit),
		   P("sgw", &sgw_corr->unit),
		   P("exception", &Unit::null_unit),
		   P("pbrow", &Unit::null_unit));

  auto rs_pb = npar("rs", rsb_par);

  FixedStack<const VtlQuantity*> row(25); // Here are the
					  // values. Ensure that the
					  // insertion order is the
					  // same as for the csv
					  // header temperature loop
  for (auto t_it = t_values.get_it(); t_it.has_curr(); t_it.next()) 
    {
      Correlation::NamedPar t_par = t_it.get_curr();
      VtlQuantity t_q = par(t_par);
      temperature = t_q.raw();
      CALL(Tpr, tpr, t_q, adjustedtpcm);
      auto tpr_par = NPAR(tpr);

      VtlQuantity pb_q =
	compute(pb_corr, c_pb_arg.getValue(), 1, check, pb_pars, t_par);
      auto pb = pb_q.raw();
      double next_pb = nextafter(pb, numeric_limits<double>::max());
      VtlQuantity next_pb_q = { pb_q.unit, next_pb };
      auto pb_par = npar("pb", pb_q);
      auto p_pb = npar("p", pb_q);

      auto first_p_point = p_values.get_first();
      bool first_p_above_pb = VtlQuantity(*get<3>(first_p_point),
					  get<2>(first_p_point)) > pb_q;
  
      auto uod_val = dcompute(uod_corr, check, uod_pars, t_par, pb_par);

      insert_in_container(rs_pars, t_par, pb_par);
      auto rs_corr = define_correlation(pb_q, ::rs_corr, c_rs_arg.getValue(),
					m_rs_arg.getValue(),
					&RsAbovePb::get_instance());

      insert_in_container(co_pars, t_par, pb_par);
      auto co_corr =
	define_correlation(pb_q,
			   cob_corr, c_cob_arg.getValue(), m_cob_arg.getValue(),
			   coa_corr, c_coa_arg.getValue(), m_coa_arg.getValue());
      auto bo_corr =
	define_correlation(pb_q,
			   bob_corr, c_bob_arg.getValue(), m_bob_arg.getValue(),
			   boa_corr, c_boa_arg.getValue(), m_boa_arg.getValue());

      insert_in_container(uo_pars, t_par, pb_par, npar("uod", uod_val));
      auto uo_corr =
	define_correlation(pb_q,
			   uob_corr, c_uob_arg.getValue(), m_uob_arg.getValue(),
			   uoa_corr, c_uoa_arg.getValue(), m_uoa_arg.getValue());
      
      auto po_corr = define_correlation(pb_q, &PobBradley::get_instance(),
					&PoaBradley::get_instance());

      auto bw_corr = define_correlation(pb_q, bwb_corr, bwa_corr);

      auto cw_corr = define_correlation(pb_q, cwb_corr, cwa_corr);

      bo_pars.insert(t_par);
      auto bobp = compute(bob_corr, c_bob_arg.getValue(), m_bob_arg.getValue(),
			  check, bo_pars, p_pb, rs_pb);
      
      auto uobp = compute(uob_corr, c_uob_arg.getValue(), m_uob_arg.getValue(),
			  check, uo_pars, p_pb, rs_pb);

      insert_in_container(bo_pars, pb_par, NPAR(bobp));

      uo_pars.insert("uobp", uobp.raw(), &uobp.unit);

      auto pobp = dcompute(&PobBradley::get_instance(), check, po_pars,
			  rs_pb, npar("bob", bobp));

      auto bwbp = dcompute(bwb_corr, check, bw_pars, t_par, npar("p", pb_q));

      insert_in_container(po_pars, pb_par, NPAR(pobp));
      insert_in_container(ug_pars, t_par, tpr_par);
      insert_in_container(bw_pars, t_par, pb_par, NPAR(bwbp));
      cg_pars.insert(tpr_par);
      uw_pars.insert(t_par);
      pw_pars.insert(t_par);
      rsw_pars.insert(t_par);
      cw_pars.insert(t_par);
      cwa_pars.insert(t_par);
      sgo_pars.insert(t_par);
      sgw_pars.insert(t_par);

      size_t n = insert_in_row(row, t_q, pb_q, uod_val);

      size_t i = 0;
      for (auto p_it = p_values.get_it(); p_it.has_curr(); ) // pressure loop
	{
	  Correlation::NamedPar p_par = p_it.get_curr();
	  VtlQuantity p_q = par(p_par);

	  bool pb_row = false; // true if this line concerns to bubble point

	  // WARNING: these predicates must be evaluated exactly in this order
	  if (p_q <= pb_q or (not (i < 2)) or first_p_above_pb) 
	    p_it.next();
	  else 
	    {
	      pb_row = true;
	      p_par = npar("p", ++i == 1 ? pb_q : next_pb_q);
	      p_q = par(p_par);
	      assert(i <= 2);
	    }

	  pressure = p_q.raw();
	  CALL(Ppr, ppr, p_q, adjustedppcm);
	  auto ppr_par = NPAR(ppr);
	  auto rs = compute(rs_corr, check, p_q, rs_pars, p_par);
	  auto rs_par = NPAR(rs);
	  auto co = compute(co_corr, check, p_q, co_pars, p_par);
	  auto co_par = NPAR(co);
	  auto bo = compute(bo_corr, check, p_q, bo_pars, p_par, rs_par, co_par);
	  auto uo = compute(uo_corr, check, p_q, uo_pars, p_par, rs_par);
	  auto po = compute(po_corr, check, p_q, po_pars, p_par, rs_par, co_par,
			    npar("bob", bo));
	  VtlQuantity z;
	  if (p_q <= pb_q)
	    z = dcompute(zfactor_corr, check, ppr_par, tpr_par);
	  auto z_par = NPAR(z);
	  auto cg = dcompute(cg_corr, check, cg_pars, ppr_par, z_par);
	  CALL(Bg, bg, t_q, p_q, z);
	  auto ug = dcompute(ug_corr, check, ug_pars, p_par, ppr_par, z_par);
	  CALL(Pg, pg, yg, t_q, p_q, z);
	  auto rsw = dcompute(rsw_corr, check, rsw_pars, p_par);
	  auto rsw_par = NPAR(rsw);
	  auto cwa = dcompute(cwa_corr, check, cwa_pars, p_par, rsw_par);
	  auto bw = compute(bw_corr, check, p_q, bw_pars, p_par, NPAR(cwa));
	  auto bw_par = NPAR(bw);
	  auto pw = dcompute(pw_corr, check, pw_pars, p_par, bw_par);
	  auto cw = compute(cw_corr, check, p_q, cw_pars, p_par, z_par, 
			    NPAR(bg), rsw_par, bw_par, NPAR(cwa));
	  CALL(PpwSpiveyMN, ppw, t_q, p_q);
	  auto uw = dcompute(uw_corr, check, uw_pars, p_par, NPAR(ppw));
	  auto sgo = dcompute(sgo_corr, check, sgo_pars, p_par);
	  auto sgw = dcompute(sgw_corr, check, sgw_pars, p_par);

	  size_t n = insert_in_row(row, p_q, rs, co, bo, uo, po, z, cg, bg,
				   ug, pg, bw, uw, pw, rsw, cw, sgo, sgw);

	  assert(row.size() == 21);

	  print_row(row, pb_row);
	  row.popn(n);
	}

      row.popn(n);
      remove_from_container(rs_pars, "pb", t_par);
      remove_from_container(co_pars, "pb", t_par);
      remove_from_container(bo_pars, "bobp", "pb", t_par);
      remove_from_container(uo_pars, "uobp", "pb", "uod", t_par);
      remove_from_container(po_pars, "pb", "pobp");
      remove_from_container(ug_pars, t_par, tpr_par);
      remove_from_container(bw_pars, t_par, pb_par, "bwbp");
      sgo_pars.remove(t_par);
      sgw_pars.remove(t_par);
      cg_pars.remove(tpr_par);
      uw_pars.remove(t_par); 
      pw_pars.remove(t_par);
      cw_pars.remove(t_par);
      rsw_pars.remove(t_par);
    }

  if (report_exceptions)
    {
      cout << endl
	   << "Exceptions:" << endl;
      exception_list.for_each([] (const auto & s) { printf(s.c_str()); });
    }

  exit(0);
}

void generate_grid_wetgas()
{
  cout << "grid wetgas option not yet implemented" << endl;
  abort();
  exit(0);
}

void generate_grid_drygas()
{
  set_check(); // Initialization of constant data
  set_api();
  set_rsb();
  set_yg();
  set_tsep();
  set_psep();
  set_h2s_concentration();
  set_co2_concentration();
  set_n2_concentration();
  set_nacl_concentration();
  
  set_ppchc_corr();
  set_tpchc_corr();
  set_ppcm_mixing_corr();
  set_tpcm_mixing_corr();
  set_adjustedppcm_corr();
  set_adjustedtpcm_corr();
  set_zfactor_corr();
  set_cg_corr();
  set_ug_corr();
  set_bwb_corr();
  set_uw_corr();
  set_pw_corr();
  set_rsw_corr();
  set_cwb_corr();
  set_sgw_corr();
  report_exceptions = catch_exceptions.getValue();

  set_t_range();
  set_p_range();

  // Calculation of constants for Z
  auto yghc = dcompute_noexcep(YghcWichertAziz::correlation(), true, NPAR(yg),
			       NPAR(n2_concentration), NPAR(co2_concentration),
			       NPAR(h2s_concentration));
  auto ppchc = dcompute_noexcep(ppchc_corr, true, NPAR(yghc),
				NPAR(n2_concentration), NPAR(co2_concentration),
				NPAR(h2s_concentration));
  auto ppcm = dcompute_noexcep(ppcm_mixing_corr, true, NPAR(ppchc),
			       NPAR(n2_concentration), NPAR(co2_concentration),
			       NPAR(h2s_concentration));		       
  auto tpchc = tpchc_corr->compute(check, yghc);
  auto tpcm = dcompute_noexcep(tpcm_mixing_corr, true, NPAR(tpchc),
			       NPAR(n2_concentration), NPAR(co2_concentration),
			       NPAR(h2s_concentration));
  auto adjustedppcm = dcompute_noexcep(adjustedppcm_corr, true, NPAR(ppcm),
				       NPAR(tpcm), NPAR(co2_concentration),
				       NPAR(h2s_concentration));
  auto adjustedtpcm = dcompute_noexcep(adjustedtpcm_corr, true, NPAR(tpcm),
				       NPAR(co2_concentration),
				       NPAR(h2s_concentration));
  // End calculation constants for z

  // Initialization of correlation parameter lists
  auto ug_pars = load_constant_parameters({ug_corr});
  insert_in_container(ug_pars, npar("tpc", adjustedtpcm),
		      npar("ppc", adjustedppcm));
  auto bwb_pars = load_constant_parameters({bwb_corr});
  auto uw_pars = load_constant_parameters({uw_corr});
  auto pw_pars = load_constant_parameters({pw_corr});
  auto rsw_pars = load_constant_parameters({rsw_corr});
  auto cwb_pars = load_constant_parameters({cwb_corr});
  ParList cg_pars; cg_pars.insert(npar("ppc", ppcm));
  ParList sgw_pars;

  using P = pair<string, const Unit*>;
  print_csv_header(P("t", get<3>(t_values.get_first())),
		   P("p", get<3>(p_values.get_first())),
		   P("zfactor", &Zfactor::get_instance()),
		   P("cg", &cg_corr->unit),
		   P("bg", &Bg::get_instance().unit),
		   P("ug", &ug_corr->unit),
		   P("pg", &Pg::get_instance().unit),
		   P("bwb", &bwb_corr->unit),
		   P("uw", &uw_corr->unit),
		   P("pw", &pw_corr->unit),
		   P("rsw", &rsw_corr->unit),
		   P("cwb", &cwb_corr->unit),
		   P("sgw", &sgw_corr->unit),
		   P("exception", &Unit::null_unit));

  FixedStack<const VtlQuantity*> row(25); // Here are the
					  // values. Ensure that the
					  // insertion order is the
					  // same as for the csv
					  // header temperature loop
  for (auto t_it = t_values.get_it(); t_it.has_curr(); t_it.next()) 
    {
      Correlation::NamedPar t_par = t_it.get_curr();
      VtlQuantity t_q = par(t_par);
      temperature = t_q.raw();
      CALL(Tpr, tpr, t_q, adjustedtpcm);
      auto tpr_par = NPAR(tpr);
  
      insert_in_container(ug_pars, t_par, tpr_par);
      bwb_pars.insert(t_par);
      cg_pars.insert(tpr_par);
      rsw_pars.insert(t_par);
      uw_pars.insert(t_par);
      pw_pars.insert(t_par);
      cwb_pars.insert(t_par);
      sgw_pars.insert(t_par);
      

      size_t n = insert_in_row(row, t_q);

          // pressure loop
      for (auto p_it = p_values.get_it(); p_it.has_curr(); p_it.next())
	{
	  Correlation::NamedPar p_par = p_it.get_curr();
	  VtlQuantity p_q = par(p_par);

	  pressure = p_q.raw();
	  CALL(Ppr, ppr, p_q, adjustedppcm);
	  auto ppr_par = NPAR(ppr);

	  VtlQuantity z = dcompute(zfactor_corr, check, ppr_par, tpr_par);
	  auto z_par = NPAR(z);
	  auto cg = dcompute(cg_corr, check, cg_pars, ppr_par, z_par);
	  CALL(Bg, bg, t_q, p_q, z);
	  auto ug = dcompute(ug_corr, check, ug_pars, p_par, ppr_par, z_par);
	  CALL(Pg, pg, yg, t_q, p_q, z);
	  auto rsw = dcompute(rsw_corr, check, rsw_pars, p_par);
	  auto rsw_par = NPAR(rsw);
	  auto bwb = dcompute(bwb_corr, check, bwb_pars, p_par);
	  auto bw_par = npar("bw", bwb);
	  auto pw = dcompute(pw_corr, check, pw_pars, p_par, bw_par);
	  auto cwb = dcompute(cwb_corr, check, cwb_pars, p_par, z_par, 
			      NPAR(bg), rsw_par, bw_par);
	  CALL(PpwSpiveyMN, ppw, t_q, p_q);
	  auto uw = dcompute(uw_corr, check, uw_pars, p_par, NPAR(ppw));
	  auto sgw = dcompute(sgw_corr, check, sgw_pars, p_par);

	  size_t n = insert_in_row(row, p_q, z, cg, bg, ug, pg, bwb, uw,
				   pw, rsw, cwb, sgw);

	  assert(row.size() == 13);

	  print_row(row);
	  row.popn(n);
	}

      row.popn(n);
      remove_from_container(ug_pars, t_par, tpr_par);
      bwb_pars.remove(t_par);
      sgw_pars.remove(t_par);
      cg_pars.remove(tpr_par);
      uw_pars.remove(t_par); 
      pw_pars.remove(t_par);
      cwb_pars.remove(t_par);
      rsw_pars.remove(t_par);
    }

  if (report_exceptions)
    {
      cout << endl
	   << "Exceptions:" << endl;
      exception_list.for_each([] (const auto & s) { printf(s.c_str()); });
    }

  exit(0);
}

void generate_grid_brine()
{
  cout << "grid brine option not yet implemented" << endl;
  abort();
  exit(0);
}

void generate_grid_gascondensated()
{
  cout << "grid gacondensated option not yet implemented" << endl;
  abort();
  exit(0);
}

AHDispatcher<string, void (*)()>
grid_dispatcher("blackoil", generate_grid_blackoil,
		"wetgas", generate_grid_wetgas,
		"drygas", generate_grid_drygas,
		"brine", generate_grid_brine,
		"gascondensated", generate_grid_gascondensated);

using OptionPtr = DynList<DynList<double>> (*)();

int main(int argc, char *argv[])
{
  cmd.parse(argc, argv);

  if (print_types.getValue())
    print_fluid_types();

  if (grid.isSet())
    grid_dispatcher.run(grid.getValue());

  cout << "No " << grid.getFlag() << " or " << print_types.getFlag()
       << " have been set" << endl;
  abort();
}




