# ifndef Z_CALIBRATE_H
# define Z_CALIBRATE_H

# include <ah-string-utils.H>
# include <tpl_array.H>
# include <tpl_dynSetTree.H>
# include <lfit.H>
# include <correlations/pvt-correlations.H>
# include <metadata/metadata-exceptions.H>

using PseudoPair = pair<const Correlation*, const Correlation*>;

# define Declare_Correlations_Set(NAME)			\
  DynList<const Correlation*> NAME##_correlations()	\
  {							\
    return Correlation::array().filter([] (auto p)			\
				       { return p->target_name() == #NAME; }); \
  }

Declare_Correlations_Set(yghc);
Declare_Correlations_Set(zfactor)

# define Declare_Pair_Correlations_Set(PNAME, TNAME, fct_name)	\
  DynList<PseudoPair> fct_name()					\
  {									\
    auto pcorrs =							\
      Correlation::array().filter([] (auto ptr)				\
				  {					\
				    return ptr->target_name() == #PNAME; \
				  });					\
    auto tcorrs =							\
      Correlation::array().filter([] (auto ptr)				\
				  {					\
				    return ptr->target_name() == #TNAME; \
				  });					\
									\
    in_place_sort(pcorrs, [] (auto ptr1, auto ptr2)			\
		  { return ptr1->name < ptr2->name; });			\
    in_place_sort(tcorrs, [] (auto ptr1, auto ptr2)			\
		  { return ptr1->name < ptr2->name; });			\
									\
    auto ret = zip(pcorrs, tcorrs);					\
    if (not ret.all([] (auto & p)					\
		    {							\
		      return p.first->author_name() == p.second->author_name();\
		    }))							\
      ZENTHROW(MismatchAuthors,						\
	       "Pseudo critical correlations does not have the same author"); \
									\
    return ret;								\
}

Declare_Pair_Correlations_Set(ppchc, tpchc, pseudo_correlations)
Declare_Pair_Correlations_Set(adjustedppcm, adjustedtpcm, adjusted_correlations)

struct ZData
{
  const Quantity<Fahrenheit> t;
  const Array<double> p; // psia
  const Array<double> z;

  ZData(const Quantity<Fahrenheit> & t, Array<double> && p, Array<double> && z)
    : t(t), p(move(p)), z(move(z)) {}

  bool operator < (const ZData & z) const { return t.raw() < z.t.raw(); }
};

struct Ztuner
{
  const Quantity<Sgg> yg;
  const Quantity<MoleFraction> n2;
  const Quantity<MoleFraction> co2;
  const Quantity<MoleFraction> h2s;
  DynSetTree<ZData> zvals;
  const DynList<const Correlation*> yghc_corrs = yghc_correlations();
  const DynList<PseudoPair> pseudo_corrs = pseudo_correlations();
  const DynList<PseudoPair> adjust_corrs = adjusted_correlations();
  const DynList<const Correlation*> z_corrs = zfactor_correlations();

  Ztuner(const Quantity<Sgg> yg, const Quantity<MoleFraction> n2,
	 const Quantity<MoleFraction> co2, const Quantity<MoleFraction> h2s)
    : yg(yg), n2(n2), co2(co2), h2s(h2s) {}

  void add_z(ZData && z)
  {
    if (zvals.has(z))
      ZENTHROW(DuplicatedVarName, "duplicated z for t = " + z.t.to_string());
    
    zvals.append(move(z));
  }

  using Zcomb = tuple<const Correlation*, PseudoPair,
		      PseudoPair, const Correlation*, LFit>;

  static Correlation::NamedPar npar(const string & name, double val,
				    const Unit * unit_ptr)
  {
    return Correlation::NamedPar(true, name, val, unit_ptr);
  }

# define NPAR(NAME) npar(#NAME, NAME.raw(), &NAME.unit)

  DynList<Zcomb> solve(bool check)
  {
    ParList pars;
    insert_in_container(pars, NPAR(yg), NPAR(n2), NPAR(co2), NPAR(h2s));
    for (auto yghc_it = yghc_corrs.get_it(); yghc_it.has_curr(); yghc_it.next())
      {
	const Correlation * yghc_corr = yghc_it.get_curr();
	Quantity<Sgg> yghc = yghc_corr->compute_by_names(pars, check);
	pars.insert("ygch", yghc);
	
	for (auto ps_it = pseudo_corrs.get_it(); ps_it.has_curr(); ps_it.next())
	  {
	    auto & ps = ps_it.get_curr();
	    auto ppchc_corr = ps.first;
	    auto tpchc_corr = ps.second;
	    auto ppchc = ppchc_corr->compute_by_names(pars, check);
	    auto tpchc = tpchc_corr->compute_by_names(pars, check);
	    insert_in_container(pars, NPAR(ppchc), NPAR(tpchc));
	    
	    for (auto ad_it = adjust_corrs.get_it(); ad_it.has_curr();
		 ad_it.next())
	      {
		auto & ad = ad_it.get_curr();
		auto adjustedppcm_corr = ad.first;
		auto adjustedtpcm_corr = ad.second;
		auto adjustedppcm =
		  adjustedppcm_corr->compute_by_names(pars, check);
		auto adjustedtpcm =
		  adjustedtpcm_corr->compute_by_names(pars, check);
		insert_in_container(pars, NPAR(adjustedppcm), NPAR(adjustedtpcm));

		DynList<tuple<double, double, double>> z;
		for (auto zit = z_corrs.get_it(); zit.has_curr(); zit.next())
		  {
		    auto zcorr = zit.get_curr();
		    for (auto it = zvals.get_it(); it.has_curr(); it.next())
		      {
			auto & d = it.get_curr();
			auto tpr =
			  Tpr::get_instance().compute({d.t, adjustedtpcm}, check);
			auto ppr = d.p.maps<PseudoReducedPressure>
			  ([&adjustedtpcm, &d, check] (auto v)
			   {
			     return Ppr::get_instance().
			       compute({Quantity<psia>(v), adjustedtpcm}, check);
			   });
			auto z = ppr.maps<double>([&tpr, check, zcorr] (auto & ppr)
                          {
			    return zcorr->compute({tpr, ppr}, check);
			  });
			zip_for_each([t = d.t.raw()] (auto v)
				     {
				       return make_tuple(t, get<0>(v),
							 get<1>(v));
				     }, d.p, z);
		      }
		  }
		
		remove_from_container(pars, "adjustedppcm", "adjustedtpcm");
	      }

	    remove_from_container(pars, "ppchc", "tpchc");
	  }
	pars.remove("yghc");
      }
  }
  
};

# endif
