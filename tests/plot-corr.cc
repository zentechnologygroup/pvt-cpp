
# include <tclap/CmdLine.h>

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
  cout << "p step == " << step << endl;
  for (double val = range.min; val < range.max; val += step)
    p_values.append(make_tuple(true, "p", val, &psia::get_instance()));
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
  below_corr_ptr = Correlation::search_by_name(below_corr_arg.getValue());
  if (below_corr_ptr == nullptr)
    error_msg(below_corr_arg.getValue() + " below correlation not found");
  target_name = below_corr_ptr->target_name();
  if (target_name != "rs" and target_name != "bob" and target_name != "uob")
    error_msg(below_corr_arg.getValue() +
	      " below correlation has an invalid target name"
	      " (must be rs, bob or uob family)");
}

ValueArg<string> above_corr_arg = { "", "above", "above correlation name", false,
				    "", "above correlation name", cmd };
const Correlation * above_corr_ptr = nullptr;
void set_above_corr()
{
  if (target_name != "rs" and not above_corr_arg.isSet())
    error_msg("Above correlation has not been specified");
  else
    {
      above_corr_ptr = &RsAbovePb::get_instance();
      return;
    }
  
  above_corr_ptr = Correlation::search_by_name(above_corr_arg.getValue());
  if (above_corr_ptr == nullptr)
    error_msg(above_corr_arg.getValue() + " above correlation not found");
  
  const string above_target_name = above_corr_ptr->target_name();
  if (above_target_name == "boa")
    if (target_name != "bob")
      error_msg("Above target " + above_target_name + " cannot be combined with "
		"below correlation " + below_corr_ptr->name);
    else
      return;

  if (above_target_name != "uoa" and target_name == "uob")
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
			       "p", &allow_sort, cmd };

enum class SortType { Pressure, Temperature, Value, Undefined };

SortType get_sort_type(const string & type)
{
  if (type == "p") return SortType::Pressure;
  if (type == "t") return SortType::Temperature;
  if (type == "value") return SortType::Value;
  error_msg("Invalid sort type");
  return SortType::Undefined;
}

DefinedCorrelation define_correlation(const double pb_val)
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

/// target, p, t
DynList<DynList<double>> generate_values()
{
  cout << "Pc corr = " << pb_corr->name << endl;

  DynList<Correlation::NamedPar> pars_list;

  { 
    auto t_par = t_values.get_first();
    pb_pars.insert(t_par);
    auto pb_val =
      pb_corr->tuned_compute_by_names(pb_pars, c_pb_arg.getValue(), 1, false);
    pb_pars.remove_first();
    auto required_pars = define_correlation(pb_val.raw()).parameter_list();
    test_parameter(required_pars, api_par, pars_list);
    test_parameter(required_pars, rsb_par, pars_list);
    test_parameter(required_pars, yg_par, pars_list);
    test_parameter(required_pars, tsep_par, pars_list);
    test_parameter(required_pars, psep_par, pars_list);
    test_parameter(required_pars, n2_concentration_par, pars_list);
    test_parameter(required_pars, co2_concentration_par, pars_list);
    test_parameter(required_pars, h2s_concentration_par, pars_list);
  }

  DynList<DynList<double>> vals; /// target, p, t
  DynList<double> row;

  for (auto t_it = t_values.get_it(); t_it.has_curr(); t_it.next())
    {
      auto t_par = t_it.get_curr();
      pb_pars.insert(t_par);
      pars_list.insert(t_par);
      row.insert(get<2>(t_par));

      auto pb_val =
	pb_corr->tuned_compute_by_names(pb_pars, c_pb_arg.getValue(), 1, false);

      pars_list.insert(make_tuple(true, "pb", pb_val.raw(), &pb_val.unit));

      auto def_corr = define_correlation(pb_val.raw());

      for (auto p_it = p_values.get_it(); p_it.has_curr(); p_it.next())
	{
	  auto p_par = p_it.get_curr();
	  pars_list.insert(p_par);
	  row.insert(get<2>(p_par));

	  pars_list.for_each([] (auto p)
			     {
			       cout << get<1>(p) << " = " << get<2>(p) << " ";
			     });
	  cout << endl;

	  auto val = def_corr.compute_by_names(pars_list, false);
	  row.insert(val);
	  vals.append(row);
	  row.remove_first(); // val
	  
	  row.remove_first(); // p_par
	  pars_list.remove_first(); // p_par
	}

      row.remove_first(); // t_par

      pars_list.remove_first(); // pb_val
      pars_list.remove_first(); // t_par

      pb_pars.remove_first(); // remove t_par
    }

  return vals;
}

void sort(DynList<DynList<double>> & vals, SortType type)
{
  switch (type)
    {
    case SortType::Pressure:
      in_place_sort(vals, [] (const auto & row1, const auto & row2)
		    {
		      return row1.nth(1) < row2.nth(1);
		    });
      break;
    case SortType::Temperature:
      in_place_sort(vals, [] (const auto & row1, const auto & row2)
		    {
		      return row1.get_last() < row2.get_last();
		    });
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


int main(int argc, char *argv[])
{
  cmd.parse(argc, argv);

  set_api();
  set_rsb();
  set_yg();
  set_tsep();
  set_psep();
  set_pb();
  set_t_range();
  set_p_range();
  set_below_corr();
  set_above_corr();

  auto vals = generate_values();

  sort(vals, get_sort_type(sort_type.getValue()));

  cout << mat_format(vals) << endl;
}
