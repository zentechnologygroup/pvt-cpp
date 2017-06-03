
# include <tclap/CmdLine.h>
# include <pvt-grid-compute.H>

using namespace TCLAP;
using namespace std;
using namespace Aleph;

CmdLine cmd = { "test-grid", ' ', "0.0" };

ValueArg<string> file =
  { "f", "file", "file name", true, "", "file name", cmd };

int main(int argc, char *argv[])
{
  cmd.parse(argc, argv);

  const string & file_name = file.getValue();
  if (not exists_file(file_name))
    
}

