
# include <metadata/correlation-analyzer.H>

int main()
{
  ifstream file("data.csv");

  EmpiricalData e;

  e.def_const("t", 189, "degF");
  e.def_const("api", 26, "api");
  e.def_const("pb", 3891.98, "psia");
  e.def_const("rsb", 1110, "SCF_STB");
  e.def_const("yg", 0.71, "Sgg");
  e.def_const("tsep", 90, "degF");
  e.def_const("psep", 60, "psia");

  e.add_samples(file);

  cout << e.to_string() << endl;

  CorrelationAnalyser m(e);

  m.target_var = "pb";

  cout << "Data names: ";
  e.names().for_each([] (const auto & s) { cout << " " << s; });
  cout << endl
       << endl;

  cout << "Correlation matching return name:" << endl;
  m.target_correlations().for_each([] (auto cptr)
    {
      cout << cptr->name << ":";
      cptr->names().for_each([] (const auto & name) { cout << " " << name; });
      cout << endl;
    });
  cout << endl;

  cout << "Correlations matching data set parameters: " << endl;
  e.matching_correlations().for_each([] (auto p)
	     {
	       cout << "    " << p->name;
	       p->names().for_each([] (const auto & s) { cout << " " << s; });
	       cout << endl;
	     });


  cout << "****************************************************************"
       << endl;

  auto ptr = Correlation::search_by_name("PbAlShammasi");
  ptr->names_and_synonyms().for_each([] (const auto & l)
    {
      l.for_each([] (const auto & s) { cout << s << " "; });
      cout << endl;
    });

  auto vals = e.execute_correlation(ptr);
}
