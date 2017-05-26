
# include <fstream>

# include <ah-string-utils.H>
# include <tclap/CmdLine.h>

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

  static void read_and_validate_unit(const PhysicalQuantity & pq,
				     istringstream & iss, const Unit *& unit_ptr)
  {
    string data;
    if (not (iss >> data))
      ZENTHROW(CommandLineError, "cannot read unit for " + pq.name);

    unit_ptr = Unit::search(data);
    if (unit_ptr == nullptr)
      ZENTHROW(CommandLineError, "unit " + data + " not found");

    if (&unit_ptr->physical_quantity != &pq)
      ZENTHROW(CommandLineError, "unit " + data + " is not for " + pq.name);
  }

  static double read_double(istringstream & iss)
  {
    string data;
    iss >> data;
    if (not is_double(data))
      ZENTHROW(CommandLineError, "read value " + data + " is not a double");
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
      ZENTHROW(CommandLineError, "Number of values " + to_string(n) +
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
      ZENTHROW(CommandLineError, "pressure values are not monotone");

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

namespace TCLAP
{
  template <> struct ArgTraits<PZArg> { typedef StringLike ValueCategory; };
}

static const DynSetTree<string> valid = { "yg", "co2", "n2", "h2s" };

struct ArgUnit
{
  string name;
  string unit_name;

  ArgUnit & operator = (const string & str)
  {
    istringstream iss(str);
    if (not (iss >> name >> unit_name))
      ZENTHROW(CommandLineError, str + " is not a pair par-name unit");

    if (not valid.contains(name))
      ZENTHROW(CommandLineError, name + " is an invalid parameter name");

    return *this;
  }

  ArgUnit() {}

  friend ostream& operator << (ostream &os, const ArgUnit & a) 
  {
    return os << a.name << " " << a.unit_name;
  }
};

namespace TCLAP
{
  template<> struct ArgTraits<ArgUnit> { typedef StringLike ValueCategory; };
}

CmdLine cmd = { "ztuner", ' ', "0" };		 

# define Declare_Arg(NAME, u)						\
  ValueArg<double> NAME##_arg = { "", #NAME, #NAME, 0, false, #NAME, cmd }; \
  const Unit * NAME##_unit = &u

Declare_Arg(yg, Sgg::get_instance());
Declare_Arg(co2, MoleFraction::get_instance());
Declare_Arg(n2, MoleFraction::get_instance());
Declare_Arg(h2s, MoleFraction::get_instance());

MultiArg<PZArg> zvalues = { "", "z", "z", false,
			    "t tunit punit p-list z-list", cmd };

ValueArg<string> fname = { "f", "file", "file name", false, "",
			   "file name", cmd };

SwitchArg save = { "s", "save", "save json", cmd };

MultiArg<ArgUnit> unit = { "", "unit", "change unit of input data", false,
			   "unit \"par-name unit\"", cmd };

// Checks whether the parameter par_name has a change of
// unity. ref_unit is the default unit of the parameter. If there was
// no change specification for par_name, then returns ref_unit
void test_par_unit_change(const string & par_name, const Unit *& unit_ptr)
{
  if (not valid.contains(par_name))
    {
      cout << "for option --unit " << par_name << ": invalid parameter name"
	   << endl;
      abort();
    }
 
  auto & pq = unit_ptr->physical_quantity;
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
	break;
      }
}

# define Test_Unit(NAME) test_par_unit_change(#NAME, NAME##_unit)

Ztuner process_input()
{
  Test_Unit(yg);
  Test_Unit(n2);
  Test_Unit(co2);
  Test_Unit(h2s);

  Ztuner ret(VtlQuantity(*yg_unit, yg_arg.getValue()),
	     VtlQuantity(*n2_unit, n2_arg.getValue()),
	     VtlQuantity(*co2_unit, co2_arg.getValue()),
	     VtlQuantity(*h2s_unit, h2s_arg.getValue()));

  for (auto & z : zvalues.getValue())
    ret.add_z(VtlQuantity(*z.tunit_ptr, z.t), move(z.p), move(z.z));

  return ret;
}

int main(int argc, char *argv[])
{
  cmd.parse(argc, argv);

  Ztuner data;

  if (fname.isSet())
    {
      const string & file_name = fname.getValue();
      if (not exists_file(file_name))
	ZENTHROW(CommandLineError, "file with name " + file_name + " not found");
      data = Ztuner(ifstream(file_name));
    }
  else
    {
      data = process_input();
      if (save.getValue())
	{
	  if (not fname.isSet())
	    ZENTHROW(CommandLineError,
		     "for save option file name has not been set");
	  ofstream out(fname.getValue());
	  out << data.to_json().dump(2) << endl;
	}
    }

  auto l = data.solve(true);
  auto s = Ztuner::to_dynlist(l);
      
  cout << to_string(format_string(s)) << endl;v

}
