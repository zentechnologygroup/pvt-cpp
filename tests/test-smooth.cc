
# include <tclap/CmdLine.h>
# include <correlations/smooth-grid.H>

using namespace TCLAP;
using namespace std;
using namespace Aleph;

CmdLine cmd = { "test-grid", ' ', "0.0" };

ValueArg<string> file =
  { "f", "file", "file name", true, "", "file name", cmd };

SmoothGrid load_grid()
{
  if (not file.isSet())
    ZENTHROW(CommandLineError, "csv file is not set");
  const string & file_name = file.getValue();
  if (not exists_file(file_name))
    error_msg("file " + file_name + " does not exist");
  ifstream in(file_name);
  return SmoothGrid(PvtGrid(in));
}


int main(int argc, char *argv[])
{
  cmd.parse(argc, argv);

  SmoothGrid sgrid = load_grid();

  sgrid.grid.pb_table.for_each([] (auto p)
			       { cout << p.first << " " << p.second << endl; });
}
