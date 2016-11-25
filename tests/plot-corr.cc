
# include <tclap/CmdLine.h>

# include <correlations/defined-correlation.H>

using namespace std;
using namespace TCLAP;

void error_msg(const string & msg)
{
  cout << msg << endl;
  abort();
}

CmdLine cmd = { "plot-corr", ' ', "0" };

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
			    "correlation for pb in psia", cmd };
const Correlation * pb_corr = nullptr;
DynList<Correlation::NamedPar> pb_pars = { yg_par, rsb_par, api_par };
void set_pb()
{
  pb_corr = Correlation::search_by_name(pb_arg.getValue());
  if (pb_corr == nullptr)
    error_msg(pb_arg.getValue() + " correlation not found");

  if (pb_corr->target_name() != "pb")
    error_msg(pb_arg.getValue() + " correlation is not for pb");

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
					     c_pb_arg.getValue(), 0);
  pb_pars.remove_first();
  return make_tuple(true, "pb", ret.raw(), &ret.unit);
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

ValueArg<RangeDesc> t_range =
  { "", "t", "range spec for temperature", true, RangeDesc(),
    "range spec \"min max n\" for temperature", cmd };
DynList<Correlation::NamedPar> t_values;
void set_t_range()
{
  const auto & range = t_range.getValue();
  const auto & step = range.step();
  for (double val = range.min; val < range.max; val += step)
    t_values.append(make_tuple(true, "t", val, &Fahrenheit::get_instance()));
}

ValueArg<RangeDesc> p_range =
  { "", "p", "range spec for pressure", true, RangeDesc(),
    "range spec \"min max n\" for pressure", cmd };
DynList<Correlation::NamedPar> p_values;
void set_p_range()
{
  const auto & range = p_range.getValue();
  const auto & step = range.step();
  for (double val = range.min; val < range.max; val += step)
    t_values.append(make_tuple(true, "p", val, &psia::get_instance()));
}

ValueArg<double> cb = { "", "cb", "c for below range", false, 0,
			"c for below range", cmd };

ValueArg<double> mb_arg = { "", "mb", "m for below range", false, 0,
			    "m for below range", cmd };

ValueArg<double> ca_arg = { "", "ca", "m for above range", false, 0,
			    "c for above range", cmd };

ValueArg<double> ma_arg = { "", "ma", "m for above range", false, 0,
			    "c for above range", cmd };

string target_name;

ValueArg<string> below_corr_arg = { "", "below", "below correlation name", true, 
				    "", "below correlation name", cmd };
const Correlation * below_corr_ptr = nullptr;
void set_below_corr()
{
  below_corr_ptr = Correlation::search_by_name(below_corr_arg.getValue());
  if (below_corr_ptr == nullptr)
    error_msg(below_corr_arg.getValue() + " not found");
  target_name = below_corr_ptr->target_name();
  if (target_name != "rs" and target_name != "bo" and target_name != "uo")
    error_msg(below_corr_arg.getValue() + " must be rs, bo or uo family");
}

ValueArg<string> above_corr_arg = { "", "above", "above correlation name", false,
				    "", "above correlation name", cmd };
DynList<Correlation::ParByName> const_parameters;

int main(int argc, char *argv[])
{
  cmd.parse(argc, argv);

  set_api();
  set_rsb();
  set_yg();
  set_tsep();
  set_psep();
  set_t_range();
  set_p_range();
}
