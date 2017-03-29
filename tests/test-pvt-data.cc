
# include <istream>

# include <ah-stl-utils.H>

# include <tclap/CmdLine.h>

# include <metadata/pvt-data.H>

using namespace std;
using namespace TCLAP;
using namespace Aleph;

CmdLine cmd = { "calibrate", ' ', "0" };

ValueArg<string> list_corr = { "", "list", "get related correlations", false, "",
			   "get related correlations", cmd };

ValueArg<string> match = { "", "match", "get matching correlations", false, "",
			   "get matching correlations", cmd };

ValueArg<string> apply = { "", "apply", "get applying correlations", false, "",
			   "get applying correlations", cmd };

struct ValuesArg
{
  string target_name;
  const Unit * punit_ptr = nullptr;
  Array<double> p;
  const Unit * unit_ptr = nullptr;
  Array<double> values;

  ValuesArg() {}

  ValuesArg & operator = (const string & str)
  {
    istringstream iss(str);

    if (not (iss >> target_name))
      throw TCLAP::ArgParseException(str + " does not contain target name");
    if (not Correlation::array().exists([this] (auto ptr)
				{ return ptr->target_name() == target_name; }))
      throw TCLAP::ArgParseException(target_name + " is invalid as target name");

    {
      string punit_name;
      if (not (iss >> punit_name))
	throw TCLAP::ArgParseException(str + " does not contain unit name");
      punit_ptr = Unit::search(punit_name);
      if (punit_ptr == nullptr)
	throw TCLAP::ArgParseException(punit_name + " for pressure not found");
      if (&punit_ptr->physical_quantity != &Pressure::get_instance())
	throw TCLAP::ArgParseException(punit_name + " is not for pressure");
    }

    string unit_name;
    if (not (iss >> unit_name))
      throw TCLAP::ArgParseException(str +
				     " second field does not contain unit name");
    unit_ptr = Unit::search(unit_name);
    if (unit_ptr == nullptr)
      throw TCLAP::ArgParseException(unit_name + " does not exist as unit");

    DynList<double> vals;
    size_t n = 0;
    string data;
    for (; iss >> data; ++n)
      {
	if (not is_double(data))
	  throw TCLAP::ArgParseException(data + " is not a double");
	vals.append(atof(data));
      }

    if (n % 2 != 0)
      throw TCLAP::ArgParseException("number of values is not even");

    auto it = vals.get_it();
    for (size_t i = 0; i < n/2; ++i, it.next())
      p.append(it.get_curr());

    for (size_t i = 0; i < n/2; ++i, it.next())
      values.append(it.get_curr());

    return *this;
  }

  friend ostream & operator << (ostream & out, const ValuesArg & a)
  {
    out << a.punit_ptr->name << " " << a.unit_ptr->name;
    zip_for_each([&out] (auto t) { out << " " << get<0>(t) << " " << get<1>(t); },
		 a.p, a.values);
    return out;
  }
};

namespace TCLAP
{
  template <> struct ArgTraits<ValuesArg> { typedef StringLike ValueCategory; };
}

MultiArg<ValuesArg> target = { "", "target", "target name", false, 
			       "property target name", cmd };
ValueArg<double> api = { "", "api", "api", true, 0, "api", cmd };
ValueArg<double> t = { "", "t", "temperature", true, 0,
		       "temperature in Fahrenheit", cmd };
ValueArg<double> pb = { "", "pb", "pb", true, 0, "pb in psia", cmd };
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

SwitchArg print_data = { "", "print", "print loaded data", cmd };

PvtData build_pvt_data()
{
  PvtData data;
  data.add_const("api", api.getValue(), Api::get_instance());
  data.add_const("t", t.getValue(), Fahrenheit::get_instance());
  data.add_const("pb", pb.getValue(), psia::get_instance());
  data.add_const("yg", yg.getValue(), Sgg::get_instance());
  if (tsep.isSet())
    data.add_const("tsep", tsep.getValue(), Fahrenheit::get_instance());
  if (psep.isSet())
    data.add_const("psep", psep.getValue(), psia::get_instance());
  if (h2s.isSet())
    data.add_const("h2s_concentration", h2s.getValue(),
		   MolePercent::get_instance());
  if (co2.isSet())
    data.add_const("co2_concentration", co2.getValue(),
		   MolePercent::get_instance());
  if (n2.isSet())
    data.add_const("n2_concentration", n2.getValue(),
		   MolePercent::get_instance());
  if (nacl.isSet())
    data.add_const("nacl_concentration", nacl.getValue(),
		   Molality_NaCl::get_instance());

  for (auto & a : target.getValue())
    data.add_vector("p", *a.punit_ptr, a.p,
		    a.target_name, *a.unit_ptr, a.values);

  return data;
}

void print(const PvtData & data)
{
  cout << data << endl;
  exit(0);
}

void print_correlations()
{
  Correlation::array().filter([name = list_corr.getValue()] (auto p)
			      {
				return p->target_name() == name;
			      }).
    for_each([] (auto p)
	     {
	       cout << p->name << ":";
	       p->parameters_signature().for_each([] (auto & p)
						  {
						    cout << " " << p;
						  });
	       cout << endl;
	     });

  exit(0);
}

void print_matches(const PvtData & data)
{
  data.matches_with_pars(match.getValue()).for_each([] (auto p)
						    {
						      cout << p->name << endl;
						    });
  exit(0);
}

// TODO: este trabajo puede ir en header separado
void print_apply(const PvtData & data)
{
  auto corr_list = data.can_be_applied(apply.getValue());

  cout << "can be applied for " << apply.getValue() << endl;
  corr_list.for_each([] (auto p) { cout << "  " << p->name << endl; });

  using T = //                 pressure vals,   target vals,   statistics
    tuple<const Correlation*, DynList<double>, DynList<double>, DynList<double>>;

  DynList<T> stats = corr_list.maps<T>([&data] (auto corr_ptr)
    {
      auto vals = data.apply(corr_ptr);
      cout << corr_ptr->name << " samples:";
      data.get_samples(corr_ptr->target_name()).for_each([] (auto v)
    { cout << " " << v;}); cout << endl;
      auto stats = CorrStat(data.get_samples(corr_ptr->target_name())).
        stats_list(Array<double>(vals.second));
      return make_tuple(corr_ptr, move(vals.first), move(vals.second),
			move(stats));
    });

  DynList<DynList<string>> rows = stats.maps<DynList<string>>([] (auto & t)
    {
      DynList<string> ret = build_dynlist<string>(get<0>(t)->name);
      auto stats =
        get<3>(t).template maps<string>([] (auto v) { return ::to_string(v); });
      ret.append(stats);
      return ret;
    });

  DynList<string> header = build_dynlist<string>("Correlation");
  header.append(CorrStat::stats_header());

  rows.insert(header);

  // TODO: opción para tipo de salida

  cout << Aleph::to_string(format_string(rows)) << endl;

  exit(0);
}

int main(int argc, char *argv[])
{
  cmd.parse(argc, argv);

  PvtData pvtdata = build_pvt_data();

  // TODO: posible dispatcher para opcione de print, match y apply

  if (print_data.getValue())
    print(pvtdata);

  if (list_corr.isSet())
    print_correlations();

  if (match.isSet())
    print_matches(pvtdata);

  if (apply.isSet())
    print_apply(pvtdata);
}
