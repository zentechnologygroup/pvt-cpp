
# include <sstream>
# include <iostream>
# include <tclap/CmdLine.h>

# include <metadata/empirical-data.H>

using namespace TCLAP;
using namespace std;
using namespace Aleph;

int main(int argc, char *argv[])
{
  CmdLine cmd(argv[0], ' ', "0");

  ValueArg<string> json_file = { "f", "json-file", "json file", false,
				 "data.json", "json file name", cmd };

  cmd.parse(argc, argv);

  ifstream input(json_file.getValue());
  if (not input)
    {
      cout << "cannot open " << json_file.getValue() << endl;
      return 0;
    }w

  stringstream s;
  s << input.rdbuf();
  
  EmpiricalData e(s.str());

  cout << e.full_desc() << endl;
  
  return 0;
}

