# ifndef Z_CALIBRATE_H
# define Z_CALIBRATE_H

# include <ah-string-utils.H>
# include <tpl_array.H>
# include <tpl_dynSetTree.H>
# include <json.hpp>
# include <lfit.H>
# include <correlations/pvt-correlations.H>
# include <metadata/metadata-exceptions.H>

using Json = nlohmann::json;
using PseudoPair = pair<const Correlation*, const Correlation*>;

# define Declare_Correlations_Set(NAME)					\
  DynList<const Correlation*> NAME##_correlations()			\
  {									\
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

struct Zdata
{
  const Quantity<Fahrenheit> t = VtlQuantity::null_quantity;
  const Array<double> p; // psia
  const Array<double> z;

  Zdata(const Quantity<Fahrenheit> & t, Array<double> && p, Array<double> && z)
    : t(t), p(move(p)), z(move(z)) {}

  Zdata() {}

  bool operator < (const Zdata & z) const { return t.raw() < z.t.raw(); }

  Json to_json() const
  {
    Json j;
    j["t"] = t.raw();
    j["p"] = to_vector(p);
    j["z"] = to_vector(z);
    return j;
  }

  Zdata(const Json & j)
    : t(Quantity<Fahrenheit>(j["t"].get<double>())),
      p(to_Array(j["p"].get<vector<double>>())),
      z(to_Array(j["z"].get<vector<double>>())) {}
};

struct Ztuner
{
  mutable Quantity<Sgg> yg;
  mutable Quantity<MoleFraction> n2;
  mutable Quantity<MoleFraction> co2;
  mutable Quantity<MoleFraction> h2s;
  DynSetTree<Zdata> zvals;
  mutable DynList<const Correlation*> yghc_corrs = yghc_correlations();
  mutable DynList<PseudoPair> pseudo_corrs = pseudo_correlations();
  mutable DynList<PseudoPair> adjust_corrs = adjusted_correlations();
  mutable DynList<const Correlation*> z_corrs = zfactor_correlations();

  //                t,     p,       z
  using T = tuple<double, double, double>;

  Json to_json() const
  {
    Json j;
    j["yg"] = yg.raw();
    j["n2"] = n2.raw();
    j["co2"] = co2.raw();
    j["h2s"] = h2s.raw();
    j["zvals"] = to_vector(zvals.maps<Json>([] (auto & z) { return z.to_json(); }));
    return j;
  }

  Ztuner(iostream & in)
  {
    Json j = Json::parse(in);
    yg = Quantity<Sgg>(j["yg"].get<double>());
    n2 = Quantity<MoleFraction>(j["n2"].get<double>());
    co2 = Quantity<MoleFraction>(j["co2"].get<double>());
    h2s = Quantity<MoleFraction>(j["h2s"].get<double>());
    for (const Json & jz : j["zvals"])
      zvals.append(jz);
  }

  DynList<T> lab_values() const
  {
    DynList<T> ret;
    zvals.for_each([&ret] (auto & d)
      {
	for (auto it = zip_it(d.p, d.z); it.has_curr(); it.next())
	  {
	    auto p = it.get_curr();
	    ret.append(T(d.t.raw(), get<0>(p), get<1>(p)));
	  }
      });
    return ret;
  }

  Ztuner(const Quantity<Sgg> yg, const Quantity<MoleFraction> n2,
	 const Quantity<MoleFraction> co2, const Quantity<MoleFraction> h2s)
    : yg(yg), n2(n2), co2(co2), h2s(h2s) {}

  void add_z(Zdata && z)
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
    DynList<Zcomb> ret;
    auto zlab = sort(lab_values(), [] (auto & t1, auto & t2)
		    { return get<1>(t1) < get<1>(t2); });
    auto zx = zlab.maps<double>([] (auto & t) { return get<2>(t); });

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

		DynList<double> zlist;
		for (auto zit = z_corrs.get_it(); zit.has_curr(); zit.next())
		  {
		    auto zcorr = zit.get_curr();
		    for (auto it = zlab.get_it(); it.has_curr(); it.next())
		      {
			auto & d = it.get_curr();
			const auto & t = Quantity<Fahrenheit>(get<0>(d));
			const auto & p = Quantity<psia>(get<1>(d));
			auto tpr =
			  Tpr::get_instance().compute({t, adjustedtpcm}, check);
			auto ppr =
			  Ppr::get_instance().compute({p, adjustedtpcm}, check);
			DynList<VtlQuantity> prpars =
			  build_dynlist<VtlQuantity>(tpr, ppr);
			auto z = zcorr->compute(prpars, check);
			zlist.append(z.raw());
		      }
		    // here we have in zlist the computed values for
		    // the correlation. Now we perform a linear fit
		    LFit fit(zx, zlist);
		    ret.append(Zcomb(zcorr, make_pair(ppchc_corr, tpchc_corr),
				     make_pair(adjustedppcm_corr,
					       adjustedtpcm_corr),
				     zcorr, fit));
		  }
		
		remove_from_container(pars, "adjustedppcm", "adjustedtpcm");
	      }

	    remove_from_container(pars, "ppchc", "tpchc");
	  }
	pars.remove("yghc");
      }

    return ret;
  }  
};

# endif
