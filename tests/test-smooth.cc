
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
  const string & file_name = file.getValue();
  if (not exists_file(file_name))
    error_msg("file " + file_name + " does not exist");
  ifstream in(file_name);
  return SmoothGrid(PvtGrid(in));
}


int main(int argc, char *argv[])
{
  SmoothGrid sgrid = load_grid();
}
