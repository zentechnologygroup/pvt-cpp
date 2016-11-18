
# include <tclap/CmdLine.h>

# include <tpl_dynMapTree.H>

# include <metadata/pvt-analyse.H>
# include <metadata/pvt-calibrate.H>

using namespace std;
using namespace TCLAP;

CmdLine cmd = { "test-calibrate", ' ', "0" };

vector<string> properties =
  { "pb", "rs", "bob", "boa", "bo", "uob", "uoa", "uo", "uod" };
ValuesConstraint<string> allowed = properties;

ValueArg<string> property =
  { "P", "property", "property", true, "", &allowed, cmd };

ValueArg<double> value = { "v", "value", "value for property", false, 0,
			   "value for the given property", cmd };
  
SwitchArg corr_List = { "l", "list", "list matching correlations", cmd };

SwitchArg corr_list = { "L", "List", "list all associated correlations", cmd };

SwitchArg corr_best = { "B", "best", "list best correlations", cmd };
  
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

ValueArg<string> file =
  { "f", "file", "file name", false, "", "file name", cmd };

ValueArg<double> bobp =
  { "", "bobp", "bobp value", false, 0, "bobp value", cmd };
ValueArg<double> uobp =
  { "", "uobp", "uobp value", false, 0, "uobp value", cmd };
ValueArg<double> uod =
  { "", "uod", "uod value", false, 0, "uod value", cmd };

PvtAnalyzer load_pvt_data(istream & input)
{
  return PvtAnalyzer(input);
}

void list_correlations(const DynList<const Correlation*> & l)
{
  l.for_each([] (auto p) { cout << p->call_string() << endl; });
}

void error_msg(const string & msg = "Not yet implemented")
{
  cout << msg << endl;
  abort();
}

void process_pb(PvtAnalyzer & pvt)
{
  if (corr_list.isSet())
    {
      list_correlations(pvt.pb_correlations());
      exit(0);
    }

  if (corr_List.isSet())
    {
      list_correlations(pvt.pb_valid_correlations());
      exit(0);
    }

  if (corr_best.isSet())
    {
      double pb = pvt.get_pb();
      auto l = pvt.pb_best_correlations().maps<DynList<string>>([pb] (auto t)
        {
	  auto per = get<2>(t)/get<1>(t) * 100;
	  return DynList<string>( { get<0>(t)->call_string(),
		to_string(get<1>(t)), to_string(get<2>(t)), to_string(per) });
	});
      l.insert({"Correlation", "Value",
	    "Error respect to pb = " + to_string(pvt.get_pb()), "%"});
      cout << to_string(format_string(l)) << endl;
      exit(0);
    }
}

void process_rs(PvtAnalyzer & pvt)
{
  error_msg();
}

void process_bob(PvtAnalyzer & pvt)
{
  error_msg();
}

void process_boa(PvtAnalyzer & pvt)
{
  error_msg();
}
void process_bo(PvtAnalyzer & pvt)
{
  error_msg();
}
void process_uob(PvtAnalyzer & pvt)
{
  error_msg();
}
void process_uoa(PvtAnalyzer & pvt)
{
  error_msg();
}
void process_uo(PvtAnalyzer & pvt)
{
  error_msg();
}

void process_uod(PvtAnalyzer & pvt)
{
  error_msg();
}

using OptionPtr = void (*)(PvtAnalyzer&);

DynMapTree<string, OptionPtr> dispatch_tbl;

void init_dispatcher()
{
  dispatch_tbl.insert("pb", process_pb);
  dispatch_tbl.insert("rs", process_rs);
  dispatch_tbl.insert("bob", process_bob);
  dispatch_tbl.insert("boa", process_boa);
  dispatch_tbl.insert("bo", process_bo);
  dispatch_tbl.insert("uob", process_uob);
  dispatch_tbl.insert("uoa", process_uoa);
  dispatch_tbl.insert("uo", process_uo);
  dispatch_tbl.insert("uod", process_uod);
}

void dispatch_option(const string & op, PvtAnalyzer & pvt)
{
  auto command = dispatch_tbl.search(op);
  if (command == nullptr)
    error_msg("Option " + op + " not registered");
  (*command->second)(pvt);
}

int main(int argc, char *argv[])
{
  init_dispatcher();

  //cmd.xorAdd(corr_list, corr_List);
  cmd.parse(argc, argv);

  PvtAnalyzer pvt;
  if (file.isSet())
    {
      ifstream input(file.getValue());
      if (not input)
	error_msg("cannot open " + file.getValue() + " file");
      pvt = load_pvt_data(input);
    }
  else
    pvt = load_pvt_data(cin);

  dispatch_option(property.getValue(), pvt);

  return 0;
}
