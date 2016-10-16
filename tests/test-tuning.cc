
# include <ah-string-utils.H>
# include <metadata/correlation-analyzer.H>

int main()
{
  EmpiricalData e;

  e.def_const("t", 189, "degF");
  e.def_const("api", 26, "api");
  e.def_const("pb", 3891.98, "psia");
  e.def_const("rsb", 1110, "SCF_STB");
  e.def_const("yg", 0.71, "Sgg");
  e.def_const("tsep", 90, "degF");
  e.def_const("psep", 60, "psia");

  e.def_var_set("Uob", "Points below the bubble point");
  e.def_var_set("Uoa", "Points above the bubble point");

  {
    ifstream file("data-b.csv");
    e.add_samples("Uob", file);
  }

  {
    ifstream file("data-a.csv");
    e.add_samples("Uoa", file);
  }

  cout << e.to_string() << endl;

  CorrelationAnalyser m(e);

  m.target_var = "pb";

  cout << "Data names: ";
  e.names().for_each([] (const auto & s) { cout << " " << s; });
  cout << endl
       << endl;

  // cout << "Correlation matching return name:" << endl;
  // m.target_correlations().for_each([] (auto cptr)
  //   {
  //     cout << cptr->name << ":";
  //     cptr->names().for_each([] (const auto & name) { cout << " " << name; });
  //     cout << endl;
  //   });
  // cout << endl;

  cout << "Correlations matching data set parameters: " << endl;
  e.matching_correlations().for_each([] (auto p)
	     {
	       cout << "    " << p->name;
	       p->names().for_each([] (const auto & s) { cout << " " << s; });
	       cout << endl;
	     });

  cout << "Correlation matching data set parameters and its values" << endl;
  e.valid_correlations().for_each([] (auto p)
	     {
	       cout << "    " << p->name;
	       p->names().for_each([] (const auto & s) { cout << " " << s; });
	       cout << endl;
	     });


  cout << "****************************************************************"
       << endl;

  auto ptr = Correlation::search_by_name("RsMillanArcia");

  assert(e.can_be_applied(ptr));
  assert(not e.can_be_applied(Correlation::search_by_name("ZFactorGopal")));

  ptr->names_and_synonyms().for_each([] (const auto & l)
    {
      l.for_each([] (const auto & s) { cout << s << " "; });
      cout << endl;
    });

  if (e.fits_parameter_ranges(ptr))
    cout << "Empirical data fits the correlation ranges" << endl;
  else
    {
      cout << "Empirical data does not fit the correlation ranges" << endl
	   << "Here the correlation pars:" << endl
	   << *ptr << endl
	   << endl
	   << "And here the empirical parameter out of range" << endl;
      auto novalid = e.invalid_parameters_values(ptr);
      for (auto it = novalid.get_it(); it.has_curr(); it.next())
	{
	  auto p = it.get_curr();
	  cout << "    " << p.first;
	  p.second.for_each([] (auto v) { cout << " " << v; });
	  cout << endl;
	}
      cout << endl;
    }

  auto mat = e.compute_mat(ptr, false);
  auto smat = mat.map<DynList<string>>([] (const auto & l)
    {
      return
      l.template map<string>([] (const auto & v) { return ::to_string(v); });
    });

  auto sign = ptr->parameters_signature();
  sign.append(ptr->target_name());
  smat.insert(sign);

  auto r = format_string(smat);

  cout << to_string(r) << endl;
}
