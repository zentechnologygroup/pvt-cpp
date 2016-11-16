
# include <tuple>
# include <iostream>

# include <tclap/CmdLine.h>

# include <metadata/pvt-analyse.H>

using namespace std;
using namespace TCLAP;

int main(int argc, char *argv[])
{
  CmdLine cmd = { argv[0], ' ', "0" };

  ValueArg<string> json_file = { "f", "json-file", "json file", false,
				 "data.json", "json file name", cmd };

  ValueArg<double> bobp = { "b", "bobp", "bo at bubble point", false,
			    0.0, "bo at bubble point", cmd };

  ValueArg<string> bobp_unit = { "u", "bobp-unit", "unit used for bobp", false,
				 "", "unit symbol used for bobp", cmd };

  cmd.parse(argc, argv);

  ifstream input(json_file.getValue());
  if (not input)
    {
      cout << "cannot open " << json_file.getValue() << endl;
      return 0;
    }

  PvtAnalyzer pvt(input);

  auto bob_vals = pvt.get_data().search_variable("Below Pb", "bob");
  if (not get<0>(bob_vals))
    {
      cout << "The data set does not contain the bob variable" << endl;
      abort();
    }

  auto bob_val_unit = get<2>(bob_vals);

  double last_bob_val = get<1>(bob_vals).get_last();

  if (bobp_unit.isSet())
    {
      auto unit_ptr = Unit::search_by_symbol(bobp_unit.getValue());
      if (unit_ptr == nullptr)
	{
	  cout << "Unit symbol " << bobp_unit.getValue() << " not found"
	       << endl;
	  abort();
	}

      if (not unit_ptr->is_sibling(*bob_val_unit))
	{
	  cout << "Defined unit for bobp " << unit_ptr->name
	       << " is not physically related to defined unit for bob "
	       << bob_val_unit->name << endl;
	  abort();
	}

      bob_val_unit = unit_ptr;
    }

  if (bobp.isSet())
    pvt.get_data().def_const("bobp", bobp.getValue(), bob_val_unit);
  else
    pvt.get_data().def_const("bobp", last_bob_val, bob_val_unit);

  pvt.check_data();

  cout << pvt.get_data().full_desc() << endl
       << endl
       << "Uod correlations:" << endl;
  pvt.uod_correlations().for_each([] (auto p)
				  {
				    cout << "  " << p->call_string() << endl;
				  });
  cout << endl
       << "valid Uod corelations:" << endl;
  pvt.uod_valid_correlations().for_each([] (auto p)
    { 
      cout << "  " << p->call_string() << endl;
    });
  cout << endl
       << "Uob complete correlations:" << endl;
  pvt.uob_correlations().for_each([] (auto p)
    { 
      cout << "  " << p->call_string() << endl;
    });
  cout << endl
     << "Uob valid correlations:" << endl;
  pvt.uob_valid_correlations().for_each([] (auto p)
    { 
      cout << "  " << p->call_string() << endl;
    });
  cout << endl
       << "Uob incomplete correlations:" << endl;
  pvt.uob_incomplete_correlations().for_each([&] (auto p)
    {
      cout << "  " << p.first->call_string() << endl;
      p.second.for_each([&] (auto p)
      {
	cout << "    " << p.first << endl;
	p.second.for_each([&] (auto p)
          {
	    cout << "      " << p->call_string() << " "
		 << pvt.get_data().can_be_applied(p) << endl;
	  });
      });
      cout << endl;
    });

  auto fits = sort(pvt.uob_correlations_lfits(), [] (auto t1, auto t2)
		   //		   { return get<3>(t1) > get<3>(t2); }); // r2
		   //{ return get<4>(t1) < get<4>(t2); }); // mse
		   { return get<6>(t1).sumsq < get<6>(t2).sumsq; });

  auto fit_list = fits.maps<DynList<string>>([] (auto t)
    {
      CorrStat::LFit lfit = get<6>(t);
      DynList<string> ret = { get<0>(t)->name,
			      get<1>(t) ? get<1>(t)->name : "null",
			      to_string(get<3>(t)), to_string(get<4>(t)),
			      to_string(get<5>(t)), to_string(lfit.c),
			      to_string(lfit.m), to_string(lfit.sumsq) };
      return ret;      
    });
  fit_list.insert({"uob", "uod", "r2", "mse", "sigma", "c", "m", "sumsq"});
  cout << to_string(format_string(fit_list)) << endl;

  auto p = pvt.get_data().values(0, "p");
  auto uob = pvt.get_data().values(0, "uob");

  bool uod_set = false;
  auto best_uod = get<1>(fits.get_first());
  if (best_uod)
    {
      pvt.get_data().def_const("uod", pvt.get_data().compute(best_uod),
			       &best_uod->unit);
      uod_set = true;
    }

  auto uod = pvt.get_data().find_const("uod");
  cout << "uod = " << VtlQuantity(*get<2>(uod), get<1>(uod)) << endl
       << "by " << best_uod->name << endl;
  
  auto best_corr = get<0>(fits.get_first());

  auto best_uob_vals = pvt.get_data().compute(0, best_corr);
  auto tuned_best_uob_vals =
    pvt.get_data().tuned_compute(0, best_corr, get<6>(fits.get_first()).c,
				 get<6>(fits.get_first()).m);

  cout << Rvector("P", p) << endl
       << Rvector("uob", uob) << endl
       << Rvector(best_corr->name, best_uob_vals) << endl
       << Rvector("tuned." + best_corr->name, tuned_best_uob_vals) << endl;
			   
  if (uod_set)
    pvt.get_data().remove_last_const("uod");

}

