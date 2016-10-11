
# include <correlations/correlation-stats.H>
# include <metadata/empirical-data.H>

int main()
{
  ifstream file("data.csv");

  EmpiricalData e;

  e.def_const("t", "degF");
  e.def_const("pb", "psia");

  e.add_samples(file);

  e.add_sample({"p = 10", "rs = 10", "bo = 28", "uo = 20"});

  cout << e.to_string() << endl;

  return 0;
}
