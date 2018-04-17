
# include <tclap/CmdLine.h>
# include <ah-comb.H>
# include <ah-dispatcher.H>
# include <ah-zip.H>
# include <pvt-units.H>
# include <pvt-grid-compute.H>

using namespace TCLAP;
using namespace std;
using namespace Aleph;

CmdLine cmd = { "test-grid", ' ', "0.0" };

ValueArg<string> file =
  { "f", "file", "file name", true, "", "file name", cmd };

struct RangeDesc
{
  double min = 0, max = 0;
  size_t n = 1; // num of steps

  RangeDesc(double min, double max, size_t n) : min(min), max(max), n(n) {}
  RangeDesc() {}

  RangeDesc & operator = (const string & str)
  {
    istringstream iss(str);
    if (not (iss >> min >> max >> n))
      ALEPHTHROW(CommandLineError, str + " is not of form \"min max n\"");

    if (n == 0)
      ALEPHTHROW(CommandLineError, ::to_string(n) + " n cannot be zero");

    if (min > max)
      {
	ostringstream s;
	s << "min value " << min << " greater than max value " << max;
	ALEPHTHROW(CommandLineError, s.str());
      }

    return *this;
  }

  double step() const noexcept { return (max - min) / (n - 1); }
};

struct ArrayDesc
{
  DynList<double> values;
  size_t n = 0;

  ArrayDesc & operator = (const string & str)
  {
    string data;
    istringstream iss(str);

    for (;iss >> data; ++n)
      {
	if (not is_double(data))
	  ALEPHTHROW(CommandLineError, data + " is not a double");

	values.append(atof(data));
      }

    if (values.is_empty())
      ALEPHTHROW(CommandLineError, "cannot read array");

    in_place_sort(values);
 
    return *this;
  }
};

namespace TCLAP
{
  template<> struct ArgTraits<RangeDesc> { typedef StringLike ValueCategory; };
  template<> struct ArgTraits<ArrayDesc> { typedef StringLike ValueCategory; };
}

ValueArg<RangeDesc> t = { "t", "t", "t range", false, RangeDesc(200, 400, 10),
			  "t min max steps", cmd };

ValueArg<RangeDesc> p = { "p", "p", "p range", false, RangeDesc(400, 800, 10),
			  "p min max steps", cmd };

ValueArg<ArrayDesc> t_array = { "", "t_array", "t values", false, ArrayDesc(),
				"t values", cmd };

ValueArg<ArrayDesc> p_array = { "", "p_array", "p values", false, ArrayDesc(),
				"p values", cmd };

ValueArg<string> var_name =
  { "v", "var-name", "var name", false, "", "var name", cmd };

SwitchArg print = { "P", "print", "print grid", cmd };

vector<string> output_types = { "R", "csv", "mat" };
ValuesConstraint<string> allowed_output_types = output_types;
ValueArg<string> output = { "", "output", "output type", false,
			    "mat", &allowed_output_types, cmd };

void process_output(const string & name, const DynList<DynList<double>> & l)
{
  static auto mapl = [] (const string & name, DynList<DynList<double>> & l)
    {
      auto out = l.maps<DynList<string>>([] (auto & l)
      {
	return l.template maps<string>([] (auto v) { return to_string(v); });
      });
      out.insert(build_dynlist<string>("t", "p", name)); // header
      return out;
    };
  
  static auto mat = [] (const string & name, DynList<DynList<double>> & l)
    {
      cout << to_string(format_string(mapl(name, l))) << endl;
    };

  static auto csv = [] (const string & name, DynList<DynList<double>> & l)
    {
      cout << to_string(format_string_csv(mapl(name, l))) << endl;
    };

  static auto R = [] (const string & name, DynList<DynList<double>> & l)
    { //          t        p and value
      DynMapTree<double, DynList<DynList<double>>> temps;
      for (auto it = l.get_it(); it.has_curr(); it.next())
	{
	  auto & curr = it.get_curr();
	  temps[curr.remove_first()].append(move(curr));
	}

      DynList<string> pnames, vnames, tnames;
      double pmin = numeric_limits<double>::max();
      double pmax = numeric_limits<double>::min();
      double vmin = numeric_limits<double>::max();
      double vmax = numeric_limits<double>::min();
      ostringstream s;
      for (auto it = temps.get_it(); it.has_curr(); it.next())
	{
	  auto & curr = it.get_curr();
	  const double & t = curr.first;
	  const string tname = to_string(long(t));
	  auto vals = transpose(curr.second);
	  const string pname = "p_" + tname;
	  const string vname = name + "_" + tname;
	  tnames.append(tname);
	  pnames.append(pname);
	  vnames.append(vname);
	  const DynList<double> & pvals = vals.get_first();
	  const DynList<double> & vvals = vals.get_last();
	  s << Rvector(pname, pvals) << endl
	    << Rvector(vname, vvals) << endl;
	  pmin = pvals.foldl(pmin, [] (auto a, auto v) { return min(a, v); });
	  pmax = pvals.foldl(pmax, [] (auto a, auto v) { return max(a, v); });
	  vmin = vvals.foldl(vmin, [] (auto a, auto v) { return min(a, v); });
	  vmax = vvals.foldl(vmax, [] (auto a, auto v) { return max(a, v); });
	}

      s << "plot(0, type=\"n\", xlim=c(" << pmin << "," << pmax << "), ylim=c("
        << vmin << "," << vmax << "))" << endl;

      // TODO: añadir puntos experimentales si los hay en forma de points

      size_t col = 1;
      DynList<string> colnames;
      DynList<int> colors;
      for (auto it = zip_it(tnames, pnames, vnames); it.has_curr();
	   it.next(), ++col)
	{
	  auto t = it.get_curr();
	  const string & tname = get<0>(t);
	  const string & pname = get<1>(t);
	  const string & vname = get<2>(t);
	  s << "lines(" << pname << "," << vname << ",col=" << col << ")"
		<< endl;
	  colnames.append("\"" + tname + "\"");
	  colors.append(col);
	}

      s << Rvector("cnames", colnames) << endl
        << Rvector("cols", colors) << endl
        << "legend(\"topright\", legend=cnames, col=cols, lty=1)" << endl;

      execute_R_script(s.str());
    };

  static
    AHDispatcher<string, void (*)(const string &, DynList<DynList<double>>&)>
    dispatcher("mat", mat, "csv", csv, "R", R);

  dispatcher.run(output.getValue(), name, l);
}

DynList<double> tvals, pvals;

# define SET_LIST(NAME)							\
  DynList<double> & set_##NAME##_vals()					\
  {									\
    if (NAME.isSet() and NAME##_array.isSet())				\
      ALEPHTHROW(CommandLineError, #NAME " and " #NAME			\
	       "_array option are exclusive");				\
									\
    if (not NAME.isSet() and not NAME##_array.isSet())			\
      ALEPHTHROW(CommandLineError, #NAME " or " #NAME			\
	       "_array option must be set");				\
									\
    if (NAME.isSet())							\
      {									\
	const RangeDesc & d = NAME.getValue();				\
	for (double v = d.min, step = d.step(); v <= d.max; v += step)	\
	  NAME##vals.append(v);						\
      }									\
    else								\
      NAME##vals = NAME##_array.getValue().values;			\
    return NAME##vals;							\
  }

SET_LIST(t);
SET_LIST(p);

int main(int argc, char *argv[])
{
  cmd.parse(argc, argv);

  const string & file_name = file.getValue();
  if (not exists_file(file_name))
    error_msg("file " + file_name + " does not exist");
  ifstream in(file_name);

  PvtGrid grid(in);

  if (print.getValue())
    {
      cout << grid << endl;
      return 0;
    }

  if (not var_name.isSet())
    ALEPHTHROW(CommandLineError, "Property flag -v NAME is mandatory");

  DynList<DynList<double>> l;
  const string & name = var_name.getValue();

  set_t_vals();
  set_p_vals();

  for (double tval : tvals)
    for (double pval : pvals)
      l.append(build_dynlist<double>(tval, pval,
				     grid(name, Quantity<Fahrenheit>(tval),
					  Quantity<psia>(pval)).raw()));

  process_output(name, l);
}

