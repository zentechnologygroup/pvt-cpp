
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
    ALEPHTHROW(CommandLineError, "csv file is not set");
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

  auto vl = sgrid.grid.get_full_property("uo");

  for (auto it = vl.l.get_it(); it.has_curr(); it.next())
    {
      const PvtGrid::Property & prop = it.get_curr();
      sgrid.uo_review(prop);
    }
}
