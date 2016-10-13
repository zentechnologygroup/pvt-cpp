
# include <metadata/correlation-analyzer.H>

int main()
{
  ifstream file("data.csv");

  EmpiricalData e;

  e.def_const("t", 160, "degF");
  e.def_const("pb", 1200, "psia");

  e.add_samples(file);

  e.add_sample({"p = 10", "rs = 10", "bo = 28", "uo = 20"});

  cout << e.full_desc() << endl;

  CorrelationMatcher m(e);

  m.target_var = "pb";

  m.target_correlations().for_each([] (auto p) { cout << *p << endl; });

  return 0;
}
