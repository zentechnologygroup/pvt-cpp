/** PVT ztuner

    This is the back end for automatically computing the best zfactor parameters

    Compile and then type
    
        ./ztuner --help

    In order to see all the options

   Aleph-w Leandro Rabindranath Leon
 */
# include <fstream>

# include <ah-string-utils.H>
# include <ahSort.H>
# include <ah-dispatcher.H>
# include <tclap/CmdLine.h>

# include <units.H>

auto & units_instancer = UnitsInstancer::init();

# include <correlations/pvt-correlations.H>
# include <metadata/z-calibrate.H>

using namespace TCLAP;


struct PZArg
{
  const Unit * tunit_ptr = nullptr;
  const Unit * punit_ptr = nullptr;

  double t = 0;
  DynList<double> p;
  DynList<double> z;

  PZArg() {}

  static void
  read_and_validate_unit(const PhysicalQuantity & pq,
			 istringstream & iss, const Unit *& unit_ptr)
  {
    string data;
    if (not (iss >> data))
      ALEPHTHROW(CommandLineError, "cannot read unit for " + pq.name);

    unit_ptr = Unit::search(data);
    if (unit_ptr == nullptr)
      ALEPHTHROW(CommandLineError, "unit " + data + " not found");

    if (&unit_ptr->physical_quantity != &pq)
      ALEPHTHROW(CommandLineError, "unit " + data + " is not for " + pq.name);
  }

  static double read_double(istringstream & iss)
  {
    string data;
    iss >> data;
    if (not is_double(data))
      ALEPHTHROW(CommandLineError, "read value " + data + " is not a double");
    return atof(data);
  }

  PZArg & operator = (const string & str)
  {
    string data;
    istringstream iss(str);

    t = read_double(iss);
    read_and_validate_unit(Temperature::get_instance(), iss, tunit_ptr);
    read_and_validate_unit(Pressure::get_instance(), iss, punit_ptr);

    DynList<double> l;
    size_t n = 0;    
    for (; iss.good(); ++n)
      l.append(read_double(iss));

    if ((n % 2) != 0)
      ALEPHTHROW(CommandLineError, "Number of values " + to_string(n) +
	       " is not even");

    for (size_t i = 0; i < n/2; ++i)
      p.append(l.remove_first());

    for (size_t i = 0; i < n/2; ++i)
      z.append(l.remove_first());

    if (not is_sorted(p))
      {
	p = p.rev();
	z = z.rev();
      }
      
    if (not is_sorted(p) and not is_inversely_sorted(p))
      ALEPHTHROW(CommandLineError, "pressure values are not monotone");

    mutable_unit_convert(*punit_ptr, p, psia::get_instance());

    return *this;
  }

  friend ostream & operator << (ostream & out, const PZArg & a)
  {
    out << "t = " << a.t << " " << a.tunit_ptr->name << endl
	<< "pressure unit = " << a.punit_ptr->name << endl
	<< "p =";
    a.p.for_each([&out] (auto v) { out << " " << v; });
    out << endl
	<< "z =";
    a.z.for_each([&out] (auto v) { out << " " << v; });
    return out;
  }
};

static const DynSetTree<string> valid = { "yg", "co2", "n2", "h2s" };

struct ArgUnit
{
  string name;
  string unit_name;

  ArgUnit & operator = (const string & str)
  {
    istringstream iss(str);
    if (not (iss >> name >> unit_name))
      ALEPHTHROW(CommandLineError, str + " is not a pair par-name unit");

    if (not valid.contains(name))
      ALEPHTHROW(CommandLineError, name + " is an invalid parameter name");

    return *this;
  }

  ArgUnit() {}

  friend ostream& operator << (ostream &os, const ArgUnit & a) 
  {
    return os << a.name << " " << a.unit_name;
  }
};

struct PlotNumbers
{
  size_t n = 0;
  DynList<size_t> numbers;

  PlotNumbers() {}

  PlotNumbers & operator = (const string & str)
  {
    string data;
    istringstream iss(str);
    for (; iss >> data; ++n)
      if (not is_size_t(data))
	ALEPHTHROW(CommandLineError, data + " is not a unsigned integer");
      else
	numbers.append(atol(data));

    return *this;
  }
};

namespace TCLAP
{
  template<> struct ArgTraits<ArgUnit> { typedef StringLike ValueCategory; };
  template <> struct ArgTraits<PZArg> { typedef StringLike ValueCategory; };
  template <> struct ArgTraits<PlotNumbers> { typedef StringLike ValueCategory; };
}

CmdLine cmd = { "ztuner", ' ', "0" };		 

# define Declare_Arg(NAME, v)						\
  ValueArg<double> NAME##_arg = { "", #NAME, #NAME, false, v, #NAME, cmd }; \
  const Unit * NAME##_unit = nullptr;

Declare_Arg(yg, 0.6);
Declare_Arg(co2, 0);
Declare_Arg(n2, 0);
Declare_Arg(h2s, 0);

MultiArg<PZArg> zvalues = { "", "z", "z", false,
			    "t tunit punit p-list z-list", cmd };

ValueArg<string> fname = { "f", "file", "file name", false, "",
			   "file name", cmd };

SwitchArg save = { "s", "save", "save json", cmd };

SwitchArg print = { "p", "print", "print data", cmd };

SwitchArg eol = { "n", "eol", "print end of line", cmd };

MultiArg<ArgUnit> unit = { "", "unit", "change unit of input data", false,
			   "unit \"par-name unit\"", cmd };

vector<string> sort_types = { "sumsq", "c", "m", "sigma", "mse", "r2", "num" };
ValuesConstraint<string> allowed_sort_types = sort_types;
ValueArg<string> sort = { "", "sort", "sort type", false,
			  "num", &allowed_sort_types, cmd };

vector<string> output_types = { "R", "csv", "mat" };
ValuesConstraint<string> allowed_output_types = output_types;
ValueArg<string> output = { "", "output", "output type", false,
			    "mat", &allowed_output_types, cmd };

ValueArg<size_t> precision_arg = { "", "digits", "number of digits in double",
				   false, 17, "number of digits", cmd };
const size_t Max_Precision = 17;

SwitchArg solve = { "S", "solve", "solve z", cmd };

SwitchArg check = { "c", "check", "check z application ranges", cmd };

SwitchArg exceptions = { "e", "exceptions", "prints exceptions", cmd };

ValueArg<PlotNumbers> plot = { "P", "plot", "plot", false, PlotNumbers(),
			       "plot", cmd };

SwitchArg transpose_out = { "", "transpose", "transpose output", cmd };

// Checks whether the parameter par_name has a change of
// unity. ref_unit is the default unit of the parameter. If there was
// no change specification for par_name, then returns ref_unit
const Unit * test_par_unit_change(const string & par_name,
				  const Unit & unit_ref)
{
  if (not valid.contains(par_name))
    {
      cout << "for option --unit " << par_name << ": invalid parameter name"
	   << endl;
      abort();
    }

  const Unit * unit_ptr = &unit_ref;
  auto & pq = unit_ref.physical_quantity;
  for (const auto & par : unit.getValue()) // traverse list of changes
    if (par.name == par_name)
      {
	unit_ptr = Unit::search_by_name(par.unit_name);
	if (unit_ptr == nullptr)
	  {
	    cout << "In unit change for " << par_name << ": unit name "
		 << par.unit_name << " not found" << endl;
	    abort();
	  }

	if (&pq != &unit_ptr->physical_quantity)
	  {
	    cout << "For " << par_name << " unit: physical quantity "
		 << pq.name << " is invalid" << endl;
	    abort();
	  }
	return unit_ptr;
      }
  return unit_ptr;
}

# define Set_Unit(NAME, UNIT)						\
  NAME##_unit = test_par_unit_change(#NAME, UNIT::get_instance());

# define Set_Par(NAME)							\
  if (NAME##_arg.isSet())						\
    data_ptr->NAME = VtlQuantity(*NAME##_unit, NAME##_arg.getValue());

void process_input(unique_ptr<Ztuner> & data_ptr)
{
  Set_Unit(yg, Sgg);
  Set_Unit(n2, MolePercent);
  Set_Unit(co2, MolePercent);
  Set_Unit(h2s, MolePercent);

  if (data_ptr == nullptr)
    data_ptr = unique_ptr<Ztuner>
      (new Ztuner(VtlQuantity(*yg_unit, yg_arg.getValue()),
		  VtlQuantity(*n2_unit, n2_arg.getValue()),
		  VtlQuantity(*co2_unit, co2_arg.getValue()),
		  VtlQuantity(*h2s_unit, h2s_arg.getValue())));

  Set_Par(yg);
  Set_Par(n2);
  Set_Par(co2);
  Set_Par(h2s);

  for (auto & z : zvalues.getValue())
    data_ptr->add_z(VtlQuantity(*z.tunit_ptr, z.t), move(z.p), move(z.z));
}

void terminate_app()
{
  if (eol.getValue())
    cout << endl;
  exit(0);
}

unique_ptr<Ztuner> data;

void process_print()
{
  if (not print.isSet())
    return;

  cout << *data << endl;
  terminate_app();
}

# define Define_Cmp(NAME)						\
  static auto cmp_##NAME = [] (const Ztuner::Zcomb & z1,		\
			       const Ztuner::Zcomb & z2)		\
    {									\
      return z1.NAME() < z2.NAME();					\
    }

# define Define_1_Cmp(NAME)						\
  static auto cmp_##NAME = [] (const Ztuner::Zcomb & z1,		\
			       const Ztuner::Zcomb & z2)		\
    {									\
      return fabs(1 - z1.NAME()) < fabs(1 - z2.NAME());			\
    }

void plot_mat(const DynList<DynList<string>> & m)
{
  if (transpose_out.getValue())
    cout << Aleph::to_string(format_string(transpose(m))) << endl;
  else
    cout << Aleph::to_string(format_string(m)) << endl;
}

void plot_csv(const DynList<DynList<string>> & m)
{
  if (transpose_out.getValue())
    cout << Aleph::to_string(format_string_csv(transpose(m))) << endl;
  else
    cout << Aleph::to_string(format_string_csv(m)) << endl;
}

void plot_R(const DynList<DynList<string>> & m)
{
  auto p = transpose(m).partition([] (auto & col)
		       { return split(col.get_first(), '.').size() < 3; });

  const DynList<DynList<string>> & lab_cols = p.first;
  const DynList<DynList<string>> & corr_cols = p.second;
  ostringstream s;

  double pmax = 0, zmax = 0, pmin = 1e6 , zmin = 1e6;
  for (auto it = lab_cols.get_it(); it.has_curr(); it.next())
    {
      auto & l = it.get_curr();
      if (l.get_first()[0] == 'p')
	{
	  pmax = l.drop(1).foldl(pmax, [] (auto m, auto v)
				 { return max(m, atof(v)); });
	  pmin = l.drop(1).foldl(pmin, [] (auto m, auto v)
				  { return min(m, atof(v)); });
	}
      else
	{
	  zmax = l.drop(1).foldl(zmax, [] (auto m, auto v)
				 { return max(m, atof(v)); });
	  zmin = l.drop(1).foldl(zmin, [] (auto m, auto v)
				 { return min(m, atof(v)); });
	}
      s << Rvector(l) << endl;
    }
  for (auto it = corr_cols.get_it(); it.has_curr(); it.next())
    {
      auto & l = it.get_curr();
      zmax = l.drop(1).foldl(zmax, [] (auto m, auto v)
			     { return max(m, atof(v)); });
      zmin = l.drop(1).foldl(zmin, [] (auto m, auto v)
			     { return min(m, atof(v)); });
      s << Rvector(l) << endl;
    }

  s << "plot(0, type=\"n\", xlim=c(" << pmin << "," << pmax << "), ylim=c("
    << zmin << "," << zmax << "))" << endl;

  size_t pch = 1;
  DynList<string> colnames;
  DynList<int> colors;
  DynList<string> ltys;
  DynList<string> pchs;
  Array<string> pnames;
  size_t n_p = 0;
  for (auto it = lab_cols.get_it(); it.has_curr(); it.next(), ++pch)
    {
      auto & plist = it.get_curr(); it.next();
      auto & zlist = it.get_curr(); 
      const string & pname = plist.get_first();
      pnames.append(pname);
      ++n_p;
      const string & zname = zlist.get_first();
      colors.append(1);
      ltys.append("NA");
      s << "points(" << pname << "," << zname << ",pch=" << pch << ")"
	<< endl;
      colnames.append("\"" + zname + "\"");
      pchs.append(to_string(pch));
    }

  size_t col = 1;
  size_t i = 0;
  for (auto it = corr_cols.get_it(); it.has_curr(); it.next(), ++i, ++col)
    {
      const string & pname = pnames((i/2) % n_p);
      const string & zname = it.get_curr().get_first();
      s << "lines(" << pname << "," << zname << ",col=" << col << ")"
	<< endl;
      colnames.append("\"" + zname + "\"");
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

  execute_R_script(s.str());
}

void process_plot()
{
  assert(plot.isSet());
  assert(not data->zcomb_list.is_empty());
  assert(not plot.getValue().numbers.is_empty());

  const PlotNumbers & numbers = plot.getValue();
  const DynList<size_t> num_list = numbers.numbers;
  if (not numbers.numbers.all([n = data->zcomb_list.size()] (auto i)
			      { return i < n; }))
    ALEPHTHROW(CommandLineError, "Invalid number in plot list");

  DynList<string> header = data->basic_header();
  auto lab_vals = data->vals();
  DynList<DynList<double>> cols;
  for (auto it = lab_vals.get_it(); it.has_curr(); it.next())
    {
      auto curr = it.get_curr();
      cols.append(get<1>(curr));
      cols.append(get<2>(curr));
    }

  const bool check = ::check.getValue();

  for (auto it = num_list.get_it(); it.has_curr(); it.next())
    {
      const auto num = it.get_curr();
      const Ztuner::Zcomb & z = data->zcomb_list(num);
      auto vals = data->eval(z, check);
      for (auto it = vals.get_it(); it.has_curr(); it.next())
	{
	  auto & curr = it.get_curr();
	  const double & t = get<0>(curr);
	  const string tstr = to_string(int(t));
	  const string title = "z." + to_string(num) + "." + tstr;
	  const auto l = build_dynlist<string>(title, title + ".cal");
	  header.append(l); // Version 4.9.2 of gnu c++ has problems
			    // compiling with header.append ({title,
			    // title + ".cal"});
	  cols.append(move(get<1>(curr)));
	  cols.append(move(get<2>(curr)));
	}
    }

  const size_t precision = ::precision_arg.getValue();
  if (precision == 0 or precision > 17)
    error_msg("precision value " + to_string(precision) +
	      " is not inside (0, 17]");

  DynList<DynList<string>> rows =
    transpose(cols).
    maps<DynList<string>>([precision] (const DynList<double> & col)
    {
      return col.maps<string>([precision] (auto v)
        { return to_string(v, precision); }); 
    });
  rows.insert(header);

  static AHDispatcher<string, void (*)(const DynList<DynList<string>>&)>
    dispatcher("mat", plot_mat, "csv", plot_csv, "R", plot_R);
				       
  dispatcher.run(output.getValue(), rows);
}

void process_solve()
{
  Define_Cmp(sumsq);
  Define_Cmp(c);
  Define_1_Cmp(m);
  Define_Cmp(mse);
  Define_Cmp(sigma);
  Define_Cmp(num);
  Define_1_Cmp(r2);

  static DynMapTree<string, bool (*)(const Ztuner::Zcomb&, const Ztuner::Zcomb&)>
    cmp = { {"sumsq", cmp_sumsq}, {"c", cmp_c}, {"m", cmp_m}, {"num", cmp_num},
	    {"r2", cmp_r2}, {"sigma", cmp_sigma}, {"mse", cmp_mse} };
  static auto format_mat = [] (const DynList<Ztuner::Zcomb> & l)
    {
      return to_string(format_string(Ztuner::zcomb_to_dynlist(l)));
    };
  static auto format_csv = [] (const DynList<Ztuner::Zcomb> & l)
    {
      return to_string(format_string_csv(Ztuner::zcomb_to_dynlist(l)));
    };
  static auto format_R = [] (const DynList<Ztuner::Zcomb> &) -> string
    {
      return "R format is incompatible for solve option";
    };  
  static AHDispatcher<string, string (*)(const DynList<Ztuner::Zcomb>&)>
    dispatcher("mat", format_mat, "csv", format_csv, "R", format_R);

  if (not solve.isSet())
    return;

  auto l = data->solve(check.getValue());
		       
  if (exceptions.getValue())
    data->exception_list.for_each([] (auto & s) { cout << s << endl; });
  else if (plot.isSet())
    process_plot();
  else
    cout << dispatcher.run(output.getValue(),
			   Aleph::sort(l, cmp.find(::sort.getValue())));
  terminate_app();
}

void test_load_file()
{
  if (not fname.isSet())
    return;
  
  ifstream in(fname.getValue());
  if (in)
    {
      try
	{
	  data = unique_ptr<Ztuner>(new Ztuner(in));
	}
      catch (exception & e)
	{
	  if (not save.getValue())
	    ALEPHTHROW(InvalidJson, "reading json: " + string(e.what()));
	}
    }
}

int main(int argc, char *argv[])
{
  cmd.parse(argc, argv);

  test_load_file();

  process_input(data);
  if (save.getValue())
    {
      if (not fname.isSet())
	ALEPHTHROW(CommandLineError,
		 "for save option file name has not been set");
      ofstream out(fname.getValue());
      out << data->to_json().dump(2) << endl;
       if (out.bad())
	ALEPHTHROW(CommandLineError, "cannot write to " + fname.getValue() +
		 " file");
    }

  process_print();
  process_solve();  
}
