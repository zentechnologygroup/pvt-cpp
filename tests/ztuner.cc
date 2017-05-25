
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
  Array<double> p;
  Array<double> z;

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

CmdLine cmd = { "ztuner", ' ', "0" };		 

# define Declare_Arg(NAME)			\
  ValueArg<double> NAME##_arg = { "", #NAME, #NAME, 0, false, #NAME, cmd }; \
									\
  const Correlation * NAME##_corr = nullptr;				\
  const Unit * NAME##_unit = nullptr

Declare_Arg(yg);
Declare_Arg(co2);
Declare_Arg(n2);
Declare_Arg(h2s);

MultiArg<PZArg> zvalues = { "", "z", "z", false,
			    "t tunit punit p-list z-list", cmd };

void process_z()
{
  for (auto & z : zvalues.getValue())
    {
      cout << z << endl;
    }
}

int main(int argc, char *argv[])
{
  cmd.parse(argc, argv);

  process_z();
}

