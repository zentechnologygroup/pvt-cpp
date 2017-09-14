
# include <tclap-utils.H>
# include <utils.H>
# include <pvt-grid-compute.H>

using namespace TCLAP;

double
eq(double k, double c0, double c1, double c2, double c3, double c4,
   double t, double x)
{
  return k*t + c0 + c1+x + c2*pow2(x) + c3*pow3(x) + c4*sin(x);
}

CmdLine cmd = { "gen-grid-test", ' ', "0.0" };

MultiArg<double> t = { "t", "t", "t", true, "t", cmd };
ValueArg<double> k = { "", "k", "k", false, 1, "k", cmd };
ValueArg<double> c0 = { "", "c0", "c0", false, 1, "c0", cmd };
ValueArg<double> c1 = { "", "c1", "c1", false, 1, "c1", cmd };
ValueArg<double> c2 = { "", "c2", "c2", false, 1, "c2", cmd };
ValueArg<double> c3 = { "", "c3", "c3", false, 1, "c3", cmd };
ValueArg<double> c4 = { "", "c4", "c4", false, 1, "c4", cmd };

DynList<pair<double, DynList<double>>> gen(double first, double last, size_t n)
{
  assert(n > 1);
  assert(first < last);
  double k = ::k.getValue(), c0 = ::c0.getValue(), c1 = ::c1.getValue(),
    c2 = ::c2.getValue(), c3 = ::c3.getValue(), c4 = ::c4.getValue();
  const double step = (last - first) / (n - 1);
  DynList<pair<double, DynList<double>>> ret;
  DynList<double> vals;
  double x = first;
  for (auto & t : t.getValue())
    {
      for (size_t i = 0; i < n; ++i, x += step)
	vals.append(build_dynlist<double>(x, eq(k, c0, c1, c2, c3, c4, t, x)));
      ret.append(make_pair(t, move(vals)));
    }
  return ret;
}

iostream & gen_grid(const DynList<string> & names,
		   const DynList<const Unit *> & units,
		   const DynList<pair<double, DynList<double>>> & vals,
		   iostream & out)
{
  assert(names.size() == units.size());
  out << join(zip_maps<string>([] (auto t)
			       { return get<0>(t) + " " + get<1>(t)->name; },
			       names, units), " ") << endl;
  for (auto it = vals.get_it(); it.has_curr(); it.next())
    {
      auto & p = it.get_curr();
      const auto & l = p.second;
      out << p.first << ", " << l.get_first() << ", " << l.get_last() << endl;
    }
  return out;
}

PvtGrid load_grid(istream & in)
{
  return PvtGrid(in);
}

namespace TCLAP
{
   template<> struct ArgTraits<RangeDesc> { typedef StringLike ValueCategory; };
}


int main(int argc, char *argv[argc])
{
  cmd.parse(argc, argv);
}
