
# include <tclap/CmdLine.h>

# include <metadata/pvt-analyse.H>
# include <metadata/pvt-calibrate.H>

using namespace std;
using namespace TCLAP;

CmdLine cmd = { "test-calibrate", ' ', "0" };

vector<string> properties = { "pb", "rs", "bob", "boa", "bo",
			      "uob", "uoa", "uo" };
ValuesConstraint<string> allowed = properties;

ValueArg<string> property = { "P", "property", "property", true,
			      "", &allowed, cmd };

ValueArg<double> value = { "v", "value", "value for property", false, 0,
			   "value for the given property", cmd };
  
SwitchArg list = { "l", "list", "list matching correlations", cmd };
  
SwitchArg set_val = { "s", "set-value", "puts given value in data set", cmd };

ValueArg<string> set_corr = { "S", "set-correlation", "set correlation",
			      false, "",
			      "set correlation for the given property", cmd };

SwitchArg calibrate = { "c", "calibrate", "calibrate given correlation", cmd };

ValueArg<string> unit = { "u", "unit", "unit", false, "",
			  "unit for the given value", cmd };

vector<string> sort_values = { "r2", "sumsq", "mse", "distance" };
ValuesConstraint<string> allow_sort = sort_values;
ValueArg<string> sort_type = { "o", "order", "output order type", false,
			       "sumsq", "order type for the output", cmd };

SwitchArg compute = { "C", "compute", "compute correlation output", cmd };

vector<string> out_type = { "csv", "format", "json", "R" };
ValuesConstraint<string> allow_out_type = out_type;
ValueArg<string> output_type = { "t", "output-type", "output type", false,
				 "format", "output style", cmd };

MultiArg<string> corr_names = { "n", "name", "add correlation name", false,
				"add correlation name to computations", cmd };

// TODO definit bobp uobp

PvtAnalyzer load_pvt_data(istream & input)
{
  return PvtAnalyzer(input);
}

void process_pb(PvtAnalyzer & pvt)
{

}

void process_rs(PvtAnalyzer & pvt)
{

}

void process_bob(PvtAnalyzer & pvt)
{

}

void process_boa(PvtAnalyzer & pvt)
{

}
void process_bo(PvtAnalyzer & pvt)
{

}
void process_uob(PvtAnalyzer & pvt)
{

}
void process_uoa(PvtAnalyzer & pvt)
{

}
void process_uo(PvtAnalyzer & pvt)
{

}

int main(int argc, char *argv[])
{
  cmd.parse(argc, argv);

  cout << load_pvt_data(cin).get_data().full_desc() << endl;

  return 0;
}
