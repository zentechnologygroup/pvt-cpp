
# include <tclap/CmdLine.h>

# include <pvt-correlations.H>

using namespace TCLAP;

DynList<double> test(int argc, char *argv[])
{
  CmdLine cmd(argv[0], ' ', "0");


  cmd.parse(argc, argv);

  DynList<double> ret;

  return ret;
}

int main(int argc, char *argv[])
{
  auto mat = test(argc, argv);
  
  return 0;
}
