
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

  cmd.parse(argc, argv);

  ifstream input(json_file.getValue());
  if (not input)
    {
      cout << "cannot open " << json_file.getValue() << endl;
      return 0;
    }

  PvtAnalyzer pvt(input);

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

  auto fits = sort(pvt.uob_correlations_lfits(), [] (auto p1, auto p2)
   {
     const auto & d1 = p1.first;
     const auto & d2 = p2.first;
     return get<3>(get<2>(d1)).sumsq < get<3>(get<2>(d2)).sumsq;
   });

  auto fit_list = fits.maps<DynList<string>>([] (auto p)
    {
      PvtAnalyzer::Desc t = p.first;
      CorrStat::Desc desc = get<2>(t);
      CorrStat::LFit lfit = get<3>(desc);
      DynList<string> ret = { get<0>(t)->name,
			      p.second ? p.second->name : "null",
			      to_string(get<0>(desc)), to_string(get<1>(desc)),
			      to_string(get<2>(desc)), to_string(lfit.c),
			      to_string(lfit.m), to_string(lfit.sumsq) };
      return ret;      
    });
  fit_list.insert({"uob", "uod", "r2", "mse", "sigma", "c", "m", "sumsq"});
  cout << to_string(format_string(fit_list)) << endl;

  auto best_uod = get<1>(fits.get_first());
  if (best_uod)
    {
      pvt.get_data().def_const("uod", pvt.get_data().compute(best_uod),
			       &best_uod->unit);
    }

  auto uod = pvt.get_data().find_const("uod");
  cout << "uod = " << VtlQuantity(*get<2>(uod), get<1>(uod)) << endl
       << "by " << best_uod->name << endl;
  
  auto best_uob_corr = get<0>(fits.get_first().first);
  auto stats = get<2>(fits.get_first().first);
  auto fit_pars = get<3>(stats);

  auto best_uob_vals = pvt.get_data().compute(0, best_uob_corr);
  auto tuned_best_uob_vals =
    pvt.get_data().tuned_compute(0, best_uob_corr, fit_pars.c, fit_pars.m);

  auto uobp = tuned_best_uob_vals.get_last();

  pvt.get_data().def_const("uobp", uobp, &best_uob_corr->unit);

  cout << "All uoa correlations:" << endl;
  Correlation::array().filter([] (auto p) { return p->target_name() == "uoa"; }).
    for_each([] (auto p)
    {
      cout << p->call_string() << endl;
    });

  auto uoa_fits = pvt.uoa_correlations_lfits();

  auto uoa_list = uoa_fits.maps<DynList<string>>([] (auto d)
    {
      CorrStat::Desc desc = get<2>(d);
      CorrStat::LFit lfit = get<3>(desc);
      DynList<string> ret = { get<0>(d)->name,
			      to_string(get<0>(desc)), to_string(get<1>(desc)),
			      to_string(get<2>(desc)), to_string(lfit.c),
			      to_string(lfit.m), to_string(lfit.sumsq) };
      return ret;      
    });
  uoa_list.insert({"uob", "r2", "mse", "sigma", "c", "m", "sumsq"});
  cout << to_string(format_string(uoa_list)) << endl;

  auto best_uoa = uoa_fits.get_first();

  auto best_uoa_corr = get<0>(best_uoa);
  auto best_uoa_fit = get<3>(get<2>(best_uoa));

  auto p_below = pvt.get_data().values(0, "p");
  auto p_above = pvt.get_data().values(1, "p");
  auto uo = to_dynlist(pvt.get_data().values(0, "uob"));
  uo.append(to_dynlist(pvt.get_data().values(1, "uoa")));

  auto p = to_dynlist(p_below);
  p.append(to_dynlist(p_above));
  
  DefinedCorrelation defcorr("p");
  defcorr.add_tuned_correlation(best_uob_corr, p_below.get_first(),
				p_below.get_last(), fit_pars.c, fit_pars.m);
  defcorr.add_tuned_correlation(best_uoa_corr, p_above.get_first(),
				p_above.get_last(),
				best_uoa_fit.c, best_uoa_fit.m);

  auto samples = pvt.get_data().samples_by_name(defcorr, "p", { 0, 1 } );
  auto values = samples.maps<double>([&defcorr] (const auto & pars)
    {
      return defcorr.compute_by_names(pars);
    });

  cout << Rvector("P", p) << endl
       << Rvector("Uo.Lab", uo) << endl
       << Rvector("Uo", values) << endl;

  
}

