
# include <ah-string-utils.H>
# include <metadata/empirical-data.H>

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
  e.matching_correlations("Uob").for_each([] (auto p)
	     {
	       cout << "    " << p->name;
	       p->names().for_each([] (const auto & s) { cout << " " << s; });
	       cout << endl;
	     });

  cout << "Correlation matching data set parameters and its values" << endl;
  e.valid_correlations("Uoa").for_each([] (auto p)
	     {
	       cout << "    " << p->name;
	       p->names().for_each([] (const auto & s) { cout << " " << s; });
	       cout << endl;
	     });


  cout << "****************************************************************"
       << endl;

  auto rs_ptr = Correlation::search_by_name("RsMillanArcia");

  assert(e.can_be_applied("Uob", rs_ptr) and e.can_be_applied("Uob", rs_ptr));

  rs_ptr->names_and_synonyms().for_each([] (const auto & l)
    {
      l.for_each([] (const auto & s) { cout << s << " "; });
      cout << endl;
    });

  if (e.fits_parameter_ranges("Uob", rs_ptr)) 
    cout << "Empirical data fits the correlation ranges" << endl;
  else
    {
      cout << "Empirical data does not fit the correlation ranges" << endl
	   << "Here the correlation pars:" << endl
	   << *rs_ptr << endl
	   << endl
	   << "And here the empirical parameter out of range" << endl;
      auto novalid = e.invalid_parameters_values("Uob", rs_ptr); 
      for (auto it = novalid.get_it(); it.has_curr(); it.next())
	{
	  auto p = it.get_curr();
	  cout << "    " << p.first;
	  p.second.for_each([] (auto v) { cout << " " << v; });
	  cout << endl;
	}
      cout << endl;
    }

  auto mat = e.compute_mat("Uob", rs_ptr, false); 
  auto smat = get<1>(mat).maps<DynList<string>>([] (const auto & row)
    {
      return row.template maps<string>([] (auto v) { return to_string(v); });
    });
  zip_for_each([] (auto t) { get<0>(t).append(to_string(get<1>(t))); },
	       smat, get<2>(mat));
  // smat.mutable_for_each([y = get<1>(mat)] (auto & row)
  // 			{ row.append(to_string(

  auto sign = rs_ptr->parameters_signature();
  sign.append(rs_ptr->target_name());
  smat.insert(sign);

  cout << "Matrix = " << endl
       << to_string(format_string(smat));

  auto bo_correlations = Correlation::array().filter([] (auto ptr)
    {
      return ptr->target_name() == "bo";
    });
  bo_correlations.for_each([] (auto ptr) { cout << ptr->name << endl; });

  bo_correlations.for_each([&e] (auto ptr)
    {
      cout << ptr->name << ":";
      auto p = e.matching_names(0, ptr);
      cout << " "; p.first.for_each([] (const auto & s) { cout << " " << s; });
      cout << ","; p.second.for_each([] (const auto & s) { cout << " " << s; });
      cout << endl
	   << endl;
    });

  //cout << e.to_json() << endl;
  cout << e.full_desc() << endl;

}
