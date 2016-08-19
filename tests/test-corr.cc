
# include <tclap/CmdLine.h>

# include <ah-stl-utils.H>
# include <ahFunctional.H>

# include <pvt-correlations.H>

using namespace TCLAP;

void test(int argc, char *argv[])
{
  CmdLine cmd(argv[0], ' ', "0");

  vector<string> correlations;
  Correlation::list().for_each([&correlations] (auto p)
			       { correlations.push_back(p->name); });
  ValuesConstraint<string> allowed = correlations;
  ValueArg<string> correlation = { "c", "correlation", "correlation name",
				   true, "", &allowed};
  cmd.add(correlation);

  SwitchArg print = { "p", "print", "print correlation information", false };
  cmd.add(print);

  UnlabeledMultiArg<double> pars = { "pars", "correlation parameter values",
				     false, "values" };
  cmd.add(pars);

  cmd.parse(argc, argv);

  auto correlation_ptr = Correlation::search_by_name(correlation.getValue());
  if (correlation_ptr == nullptr)
    {
      cout << "Error: correlation name " << correlation.getValue()
	   << " not found" << endl;
      abort();
    }

  if (print.getValue())
    cout << *correlation_ptr << endl
	 << endl;

  auto pars_list =
    zip(correlation_ptr->get_par_types(), vector_to_DynList(pars.getValue())).
   map<VtlQuantity>([] (auto p) { return VtlQuantity(*p.first, p.second); });

  auto ret = correlation_ptr->compute(pars_list);

  auto n = correlation_ptr->get_num_pars();
  cout << correlation_ptr->name << "(";
  pars_list.for_each([&n] (const auto & p)
		     {
		       cout << p;
		       if (--n != 0)
			 cout << ", ";
		     });
  cout << ") = " << ret << endl;


  cout << endl;
}

int main(int argc, char *argv[])
{
  test(argc, argv);
  
  return 0;
}
