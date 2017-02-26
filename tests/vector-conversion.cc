
# include <istream>

# include <tclap/CmdLine.h>
# include <correlations/correlation.H>

using namespace TCLAP;

CmdLine cmd = { "vector-conversion", ' ', "0" };

ValueArg<string> source = {"S", "source-unit", "source unit", true,
			   "", "source unit", cmd};
ValueArg<string> target = {"T", "target-unit", "target unit", true,
			   "", "target unit", cmd};

UnlabeledMultiArg<double> vals = { "values", "list of values to convert", true,
				   "list of values to convert", cmd };

const Unit * search_unit(const string & name)
{
  auto unit_ptr = Unit::search_by_name(name);
  if (unit_ptr == nullptr)
    {
      unit_ptr = Unit::search_by_symbol(name);
      if (unit_ptr == nullptr)
	{
	  cout << "Unit " << name << " not found" << endl;
	  abort();
	}
    }
  return unit_ptr;
}

void convert()
{
  auto src_unit = search_unit(source.getValue());
  auto tgt_unit = search_unit(target.getValue());
  auto convert_fct = search_conversion(*src_unit, *tgt_unit);

  for (auto v : vals)
    cout << (*convert_fct)(v) << " ";
  cout << endl;
}

int main(int argc, char *argv[])
{
  cmd.parse(argc, argv);
  convert();
}
