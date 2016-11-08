
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

  cout << pvt.get_data().full_desc() << endl;

  auto boa_samples = pvt.get_data().values("Above Pb", "boa");
  cout << Rvector("boa", boa_samples) << endl;

  auto boa_corr = Correlation::array().filter([] (auto p)
    { return p->target_name() == "Boa"; });
  cout << "Boa correlations" << endl;
  boa_corr.for_each([] (auto p) { cout << "  " << p->call_string() << endl; });
  
  auto boa_apliable = pvt.boa_correlations();
  if (boa_apliable.is_empty())
    {
      cout << "There is not any boa correlation" << endl;
      return 0;
    }  

  auto boa_valid = pvt.boa_valid_correlations();
  if (boa_valid.is_empty())
    {
      cout << "Valid correlation is is empty" << endl;
      return 0;
    }

  auto c = boa_valid.get_first();
  cout << c->call_string() << endl << endl;

  auto p = pvt.get_data().values("Above Pb", "p");

  cout << Rvector("p", p) << endl;

  cout << "Boa valid correlations:" << endl;
  boa_valid.for_each([] (auto p) { cout << " " << p->call_string() << endl; });
  cout << endl;

  auto boa_fits = sort(pvt.boa_correlations_lfits(),
		       [] (const auto & t1, const auto &t2)
		       {
			 return get<4>(t1).sumsq < get<4>(t2).sumsq;
		       });

  cout << get<4>(boa_fits.get_first()).to_string() << endl
       << pvt.boa_lfit(get<2>(boa_fits.get_first())).to_string() << endl;

  assert(get<4>(boa_fits.get_first()) ==
	 pvt.boa_lfit(get<2>(boa_fits.get_first())));

  boa_fits.for_each([&pvt] (auto c) { cout << pvt.to_string(c) << endl; });

  boa_valid.for_each([&pvt] (auto t)
    {
      cout << Rvector(t->name, pvt.get_data().compute(1, "p", t)) << endl;
    });

  auto boa_lfits_list = pvt.boa_lfits_list(boa_fits);

  cout << pvt.to_R("tuned.", boa_lfits_list) << endl;

  auto p_below = pvt.get_data().values("Below Pb", "p");
  auto best_bob_correlation = &BobStanding::get_instance();
  auto below_fit = pvt.bob_lfit(best_bob_correlation);

  auto p_above = pvt.get_data().values("Above Pb", "p");
  auto best_boa_correlation = get<2>(boa_fits.get_first());
  auto above_fit = pvt.boa_lfit(best_boa_correlation);

  DefinedCorrelation defcorr("p");

  defcorr.add_tuned_correlation(best_bob_correlation,
				p_below.get_first(), p_below.get_last(),
				below_fit.c, below_fit.m);
  defcorr.add_tuned_correlation(best_boa_correlation,
				p_above.get_first(), p_above.get_last(),
				above_fit.c, above_fit.m);

  auto l = pvt.get_data().generate_samples_by_name(best_boa_correlation, 0);
  l.for_each([] (const auto & row)
    {
      row.for_each([] (auto p) { cout << p.first << " = " << p.second << " "; });
      cout << endl;
    });

//   defcorr.add_tuned_correlation(
				
// , double start, double end)

  // boa_fits.for_each([&pvt] (auto t)
  //   {
  //     cout << Rvector(t->name, pvt.get_data().compute(1, "p", t)) << endl;
  //   });

  // cout << pvt.to_string(pvt.best_bob_correlations()) << endl
  //      << endl;

  // pvt.best_bob_correlations().for_each([&pvt] (auto t)
  //   {
  //     cout << Rvector(get<2>(t)->name, pvt.get_data().compute(0, "rs", get<2>(t)))
  // 	   << endl;
  //   });

  // auto bob_lfits = sort(pvt.bob_correlations_lfits(),
  // 			[] (const auto & t1, const auto &t2)
  // 			{
  // 			  return get<4>(t1).sumsq < get<4>(t2).sumsq;
  // 			});

  // cout << to_string(format_string(bob_lfits.maps<DynList<string>>([] (auto t)
  //   {
  //     auto fit = get<4>(t);
  //     return DynList<string>({get<2>(t)->name, to_string(fit.c),
  // 	    to_string(fit.m), to_string(fit.sumsq) });
  //   }))) << endl;

  // auto bob_lfits_list = pvt.bob_lfits_list(bob_lfits);

  // cout << pvt.to_R("tuned.", bob_lfits_list) << endl;

  // cout << pvt.to_R(pvt.get_data().values(0, "bob"),
  //  		   pvt.get_data().values(0, "p"), "p", "Bob",
  // 		   pvt.best_bob_correlations()) << endl;
}

