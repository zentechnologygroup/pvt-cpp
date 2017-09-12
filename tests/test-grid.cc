
# include <tclap/CmdLine.h>
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
      ZENTHROW(CommandLineError, str + " is not of form \"min max n\"");

    if (n == 0)
      ZENTHROW(CommandLineError, ::to_string(n) + " n cannot be zero");

    if (min > max)
      {
	ostringstream s;
	s << "min value " << min << " greater than max value " << max;
	ZENTHROW(CommandLineError, s.str());
      }

    return *this;
  }

  double step() const noexcept { return (max - min) / (n - 1); }
};

namespace TCLAP
{
  template<> struct ArgTraits<RangeDesc> { typedef StringLike ValueCategory; };
}

ValueArg<RangeDesc> t = { "t", "t", "t range", false, RangeDesc(200, 400, 10),
			  "t min max steps", cmd };

ValueArg<RangeDesc> p = { "p", "p", "p range", false, RangeDesc(400, 800, 10),
			  "p min max steps", cmd };

ValueArg<string> var_name =
  { "v", "var-name", "var name", false, "", "var name", cmd };

SwitchArg print = { "P", "print", "print grid", cmd };

vector<string> output_types = { "R", "csv", "mat" };
ValuesConstraint<string> allowed_output_types = output_types;
ValueArg<string> output = { "", "output", "output type", false,
			    "mat", &allowed_output_types, cmd };

void finish(const string & msg)
{
  cout << msg << endl;
  abort();
}

int main(int argc, char *argv[])
{
  cmd.parse(argc, argv);

  const string & file_name = file.getValue();
  if (not exists_file(file_name))
    finish("file " + file_name + " does not exist");
  ifstream in(file_name);

  PvtGrid grid(in);

  if (print.getValue())
    {
      cout << grid << endl;
      return 0;
    }

  if (not (t.isSet() and p.isSet() and var_name.isSet()))
    return 0;

  const string & name = var_name.getValue();
  const auto & tdesc = t.getValue();
  const auto & pdesc = p.getValue();
  for (double tval = tdesc.min; tval <= tdesc.max; tval += tdesc.step())
    for (double pval = pdesc.min; pval <= pdesc.max; pval += pdesc.step())
      cout << name << "(" << tval << ", " << pval << ") = "
	   << grid(name, Quantity<Fahrenheit>(tval), Quantity<psia>(pval)) << endl;
}

