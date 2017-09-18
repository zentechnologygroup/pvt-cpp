
# include <iostream>
# include <tclap-utils.H>
# include <utils.H>
# include <pvt-grid-compute.H>
# include <pvt-units.H>
# include <dummy-unit.H>

using namespace TCLAP;

auto & iref = UnitsInstancer::init();

double
eq(double k, double c0, double c1, double c2, double c3, double c4,
   double t, double x)
{
  return k*t + c0 + c1+x + c2*pow2(x) + c3*pow3(x) + c4*sin(x);
}

namespace TCLAP
{
  template<> struct ArgTraits<RangeDesc> { typedef StringLike ValueCategory; };
  template<> struct ArgTraits<Values> { typedef StringLike ValueCategory; };
}

CmdLine cmd = { "gen-grid-test", ' ', "0.0" };

ValueArg<RangeDesc> tr = { "t", "t", "t", false,
			   RangeDesc(&Fahrenheit::get_instance()),
			   "t min max n", cmd };
ValueArg<RangeDesc> pr = { "p", "p", "p", false,
			   RangeDesc(&psia::get_instance()),
			   "p min max n", cmd };

ValueArg<Values> tv = { "", "tv", "t", false,
			Values(&Fahrenheit::get_instance()), "t vals", cmd };
ValueArg<Values> pv = { "", "pv", "p", false, Values(&psia::get_instance()),
			"p vals", cmd };

ValueArg<double> k = { "", "k", "k", false, 2, "k", cmd };
ValueArg<double> c0 = { "", "c0", "c0", false, 1, "c0", cmd };
ValueArg<double> c1 = { "", "c1", "c1", false, 1e-1, "c1", cmd };
ValueArg<double> c2 = { "", "c2", "c2", false, 1e-4, "c2", cmd };
ValueArg<double> c3 = { "", "c3", "c3", false, 1e-7, "c3", cmd };
ValueArg<double> c4 = { "", "c4", "c4", false, 1e-1, "c4", cmd };

SwitchArg gen_arg = { "g", "gen-grid", "generate testing grid", cmd };
ValueArg<string> file = { "f", "file", "file name", false, "", "file name", cmd };

vector<string> output_types = { "R", "csv", "mat" };
ValuesConstraint<string> allowed_output_types = output_types;
ValueArg<string> output = { "", "output", "output type", false,
			    "mat", &allowed_output_types, cmd };

DynList<DynList<double>> gen_vals()
{
  double k = ::k.getValue(), c0 = ::c0.getValue(), c1 = ::c1.getValue(),
    c2 = ::c2.getValue(), c3 = ::c3.getValue(), c4 = ::c4.getValue();
  DynList<DynList<double>> ret;
  const DynList<double> tvals =
    tr.isSet() ? tr.getValue().values() : tv.getValue().vals;
  const DynList<double> pvals =
    pr.isSet() ? pr.getValue().values() : pv.getValue().vals;
  for (auto it = tvals.get_it(); it.has_curr(); it.next())
    {
      const double & t = it.get_curr();
      for (auto it = pvals.get_it(); it.has_curr(); it.next())
	{
	  auto & p = it.get_curr();
	  ret.append(build_dynlist<double>(t, p,
					   eq(k, c0, c1, c2, c3, c4, t, p)));
	}
    }
  return ret;
}

ostream & gen_grid(const DynList<string> & names,
		   const DynList<const Unit *> & units,
		   const DynList<DynList<double>> & vals,
		   ostream & out)
{
  assert(names.size() == units.size());
  out << join(zip_maps<string>([] (auto t)
			       { return get<0>(t) + " " + get<1>(t)->name; },
			       names, units), ",") << endl;
  for (auto it = vals.get_it(); it.has_curr(); it.next())
    {
      join(it.get_curr(), ",", out);
      out << endl;
    }

  return out;
}

DynList<DynList<string>> convert_to_string(const DynList<DynList<double>> & vals)
{
  DynList<string> header =
    build_dynlist<string>("t " + Fahrenheit::get_instance().name,
			  "p " + psia::get_instance().name,
			  "v " + TestUnit::get_instance().name);
  auto rows = vals.maps<DynList<string>>([] (auto & l)
    { return l.template maps<string>([] (double v) { return to_str(v); }); });
  rows.insert(header);
  return rows;
}

void process_gen_grid()
{
  static auto mat = [] (const DynList<DynList<double>> & vals)
    {
      cout << to_string(format_string(convert_to_string(vals))) << endl;
    };
  static auto csv = [] (const DynList<DynList<double>> & vals)
    {
      cout << to_string(format_string_csv(convert_to_string(vals))) << endl;
    };
  auto vals = gen_vals();
}

PvtGrid load_grid(istream & in)
{
  return PvtGrid(in);
}

int main(int argc, char *argv[])
{
  cmd.parse(argc, argv);

  stringstream s;
  gen_grid({"t", "p", "c"}, { &Fahrenheit::get_instance(), &psia::get_instance(),
	&TestUnit::get_instance()}, gen_vals(), s);

  cout << s.str() << endl;

  auto grid = load_grid(s);

  cout << grid << endl;
}
