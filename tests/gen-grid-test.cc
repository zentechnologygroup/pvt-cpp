
# include <iostream>

# include <ah-comb.H>
# include <ah-dispatcher.H>

# include <tclap-utils.H>
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

SwitchArg print = { "P", "print", "print grid", cmd };

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

DynList<DynList<double>> process_gen_arg()
{
  if (not gen_arg.isSet())
    return DynList<DynList<double>>();

  return gen_vals();
}

ostream & output_grid(const DynList<string> & names,
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

struct ValsDesc
{
  double maxp = numeric_limits<double>::min();
  double minp = numeric_limits<double>::max();
  double maxv = numeric_limits<double>::min();
  double minv = numeric_limits<double>::max();

  size_t n = 0;
  DynList<double> t;
  DynList<double> p;
  DynList<double> v;
  DynList<double> vg;
};

ValsDesc convert_to_valsdesc(const DynList<DynList<double>> & vals,
			     const PvtGrid & grid)
{
  size_t vi = grid.is_valid() ? grid.property_index("v") : 0;
  ValsDesc d;
  for (auto it = vals.get_it(); it.has_curr(); it.next())
    {
      auto & l = it.get_curr();
      auto & t = l.get_first();
      auto & p = l.nth(1);
      auto & v = l.get_last();
      d.maxp = max(d.maxp, p);
      d.minp = min(d.minp, p);
      d.maxv = max(d.maxv, v);
      d.minv = min(d.minv, v);
      ++d.n;
      d.t.append(t);
      d.p.append(p);
      d.v.append(v);
      if (not grid.is_valid())
	continue;
      
      const VtlQuantity temp(Fahrenheit::get_instance(), t);
      const VtlQuantity pres(psia::get_instance(), p);
      d.vg.append(grid.compute(vi, temp, pres).raw());
    }

  return d;
}

DynList<DynList<string>> to_dynlist(const ValsDesc & d)
{
  DynList<DynList<string>> rows;

  DynList<string> header =
    build_dynlist<string>("t " + Fahrenheit::get_instance().name,
			  "p " + psia::get_instance().name,
			  "v " + TestUnit::get_instance().name); 
  if (not d.vg.is_empty())
    {
      header.append("vg");
      rows = zip_maps<DynList<string>>([] (auto t)
        {
	  return build_dynlist<string>
	  (to_string(get<0>(t)), to_string(get<1>(t)),
           to_string(get<2>(t)), to_string(get<3>(t)));
	}, d.t, d.p, d.v, d.vg);
    }
  else
    rows = zip_maps<DynList<string>>([] (auto t)
      {
	return build_dynlist<string>
	(to_string(get<0>(t)), to_string(get<1>(t)), to_string(get<2>(t)));
      }, d.t, d.p, d.v);

  rows.insert(header);

  return rows;
}

void process_print_grid(const ValsDesc & d)
{
  static auto print_mat = [] (const ValsDesc & d)
    {
      cout << to_string(format_string(to_dynlist(d)));
    };
  static auto print_csv = [] (const ValsDesc & d)
    {
      cout << to_string(format_string_csv(to_dynlist(d)));
    };
  static auto print_R = [] (const ValsDesc & d)
    { //          temp    p, v and eventually vg
      DynMapTree<double, DynList<DynList<double>>> temps;
      if (d.vg.is_empty())
	for (auto it = zip_it(d.t, d.p, d.v); it.has_curr(); it.next())
	  {
	    auto t = it.get_curr();
	    temps[get<0>(t)].append(build_dynlist<double>(get<1>(t), get<2>(t)));
	  }
      else
	for (auto it = zip_it(d.t, d.p, d.v, d.vg); it.has_curr(); it.next())
	  {
	    auto t = it.get_curr();
	    temps[get<0>(t)].append(build_dynlist<double>(get<1>(t), get<2>(t),
							  get<3>(t)));
	  }

      bool has_grid_values = false;
      DynList<string> pnames, vnames, vgnames;
      ostringstream s;
      for (auto it = temps.get_it(); it.has_curr(); it.next())
	{
	  auto & p = it.get_curr();
	  const string tname = to_string(int(p.first));
	  const string pname = "p_" + tname;
	  const string vname = "v_" + tname;
	  const string vgname = "vg_" + tname;
	  pnames.append(pname);
	  vnames.append(vname);
	  vgnames.append(vgname);
	  auto pv = transpose(p.second);
	  auto lit = pv.get_it();
	  s << Rvector(pname, lit.get_curr()) << endl; lit.next();
	  s << Rvector(vname, lit.get_curr()) << endl; lit.next();
	  if (lit.has_curr())
	    {
	      s << Rvector(vgname, pv.get_last()) << endl;
	      has_grid_values = true;
	    }
	}
      s << "plot(0, type=\"n\", xlim=c(" << d.minp << "," << d.maxp
      << "), ylim=c(" << d.minv << "," << d.maxv << "))" << endl;

      size_t pch = 1;
      size_t col = 1;
      DynList<string> colnames;
      DynList<int> colors;
      DynList<string> ltys;
      DynList<string> pchs;
      for (auto it = temps.get_it(); it.has_curr(); it.next(), col++)
	{
	  auto & pp = it.get_curr();
	  const string tname = to_string(int(pp.first));
	  const string pname = "p_" + tname;
	  const string vname = "v_" + tname;
	  const string vgname = "vg_" + tname;
	  s << "lines(" << pname << "," << vname << ",col=" << col << ")"
	    << endl;
	  if (has_grid_values) 
	    s << "points(" << pname << "," << vgname << ",pch=" << pch
	      << ", col=" << col << ")" << endl;
	  colnames.append("\"" + tname + "\"");
	  colors.append(col);
	  pchs.append("NA");
	  ltys.append("1");
	}
      s << Rvector("cnames", colnames) << endl
        << Rvector("cols", colors) << endl
        << Rvector("pchs", pchs) << endl
        << Rvector("ltys", ltys) << endl
        << "legend(\"topright\", legend=cnames, col=cols, pch=pchs, lty=ltys)"
        << endl;

      cout << s.str() << endl;
      execute_R_script(s.str());
    };

  static const AHDispatcher<string, void (*)(const ValsDesc&)>
    dispatcher("mat", print_mat, "csv", print_csv, "R", print_R);

  if (not print.isSet())
    return;

  dispatcher.run(output.getValue(), d);
}

PvtGrid load_grid()
{
  if (not file.isSet())
    return PvtGrid();

  const string & file_name = file.getValue();

  if (not exists_file(file_name))
    error_msg("file " + file_name +  " doesn't exist");

  ifstream in(file_name);

  return PvtGrid(in);
}

int main(int argc, char *argv[])
{
  cmd.parse(argc, argv);

  auto vals = process_gen_arg();

  auto grid = load_grid();

  process_print_grid(convert_to_valsdesc(vals, grid));
}
