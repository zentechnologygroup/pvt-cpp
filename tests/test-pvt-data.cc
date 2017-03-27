
# include <istream>

# include <ah-stl-utils.H>

# include <tclap/CmdLine.h>

# include <metadata/pvt-data.H>

using namespace TCLAP;

CmdLine cmd = { "calibrate", ' ', "0" };

ValueArg<string> match = { "m", "match", "get matching correlations", false, "",
			   "get matching correlations", cmd };

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

    string pdata, vdata;
    for (size_t i = 1; iss >> pdata >> vdata; ++i)
      {
	if (not is_double(pdata))
	  throw TCLAP::ArgParseException(pdata + " for pressure in pair " +
					 to_string(i) + " is not a double");
	if (not is_double(vdata))
	  throw TCLAP::ArgParseException(vdata + " in pair " + to_string(i) +
					 " is not a double");

	p.append(atof(pdata));
	values.append(atof(vdata));
      }

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

int main(int argc, char *argv[])
{
  cmd.parse(argc, argv);

  PvtData pvtdata = build_pvt_data();

  pvtdata.add_const("t", 189, Fahrenheit::get_instance());
  pvtdata.add_const("api", 26, Api::get_instance());
  pvtdata.add_const("pb", 3891.98, psia::get_instance());
  pvtdata.add_const("rsb", 1110, SCF_STB::get_instance());
  pvtdata.add_const("yg", 0.71, Sgg::get_instance());
  pvtdata.add_const("tsep", 90, Fahrenheit::get_instance());
  pvtdata.add_const("psep", 60, psia::get_instance());
  pvtdata.add_vector("p", psia::get_instance(),
		     { 1000, 2000, 3000, 3500, 3602.9, 3891.98 },
		     "bob", RB_STB::get_instance(),
		     { 1.32591, 1.45799, 1.61047, 1.70097, 1.70942, 1.77922 });
  pvtdata.add_vector("p", psia::get_instance(),
		     { 1000, 2000, 3000, 3500, 3602.9, 3891.98 },
		     "rs", SCF_STB::get_instance(),
		     { 287.701, 521.132, 801.125, 967.446, 982.957, 1110 });
  // fin parte temporal

  if (match.isSet())
    {
      pvtdata.matches_with_pars(match.getValue()).for_each([] (auto p)
        {
	  cout << p->name << endl;
	});
      exit(0);
    }

  cout << "bob matches:" << endl;
  pvtdata.matches_with_pars("bob").for_each([] (auto ptr)
    {
      cout << ptr->name << endl;
    });
  cout << endl
       << "bob apply:" << endl;

  pvtdata.can_be_applied("bob").for_each([] (auto ptr)
    {
      cout << ptr->name << endl;
    });
  cout << endl;
}
