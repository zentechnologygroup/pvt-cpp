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

Declare_Pair_Correlations_Set(ppchc, tpchc, pch_correlations)
Declare_Pair_Correlations_Set(ppcm, tpcm, pcm_correlations)
Declare_Pair_Correlations_Set(adjustedppcm, adjustedtpcm, adjusted_correlations)

struct Zdata
{
  const Quantity<Fahrenheit> t = 0;
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

  string to_string() const
  {
    ostringstream s;
    s << "t = " << t << endl
      << "p =";
    p.for_each([&s] (auto v) { s << " " << v; });
    s << endl
      << "z =";
    z.for_each([&s] (auto v) { s << " " << v; });
    return s.str();
  }

  Zdata(const Json & j)
    : t(Quantity<Fahrenheit>(j["t"].get<double>())),
      p(to_Array(j["p"].get<vector<double>>())),
      z(to_Array(j["z"].get<vector<double>>())) {}
};

struct Ztuner
{
  Quantity<Sgg> yg;
  Quantity<MoleFraction> n2;
  Quantity<MoleFraction> co2;
  Quantity<MoleFraction> h2s;

private:

  DynSetTree<Zdata> zvals;

public:

  //                t,     p,       z
  using T = tuple<double, double, double>;

  Json to_json() const
  {
    Json j;
    j["yg"] = yg.raw();
    j["n2"] = n2.raw();
    j["co2"] = co2.raw();
    j["h2s"] = h2s.raw();
    j["zvals"] =
      to_vector(zvals.maps<Json>([] (auto & z) { return z.to_json(); }));
    return j;
  }

  string to_string() const
  {
    ostringstream s;
    s << "yg = " << yg << endl
      << "n2 = " << n2 << endl
      << "co2 = " << co2 << endl
      << "h2s = " << h2s << endl
      << "zvals:" << endl;
    auto & last = zvals.get_last();
    for (auto it = zvals.get_it(); it.has_curr(); it.next())
      {
	auto & z = it.get_curr();
	s << shift_lines_to_left(z.to_string(), 2);
	if (&z != &last)
	  s << endl;
      }
    return s.str();
  }

  friend ostream & operator << (ostream& out, const Ztuner & z)
  {
    return out << z.to_string();
  }

  Ztuner(istream & in)
  {
    Json j = Json::parse(in);
    yg = Quantity<Sgg>(j["yg"].get<double>());
    n2 = Quantity<MoleFraction>(j["n2"].get<double>());
    co2 = Quantity<MoleFraction>(j["co2"].get<double>());
    h2s = Quantity<MoleFraction>(j["h2s"].get<double>());
    for (const Json & jz : j["zvals"])
      zvals.append(jz);
  }

  Ztuner(istream && in) : Ztuner(in) {}

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

  void
  add_z(const Quantity<Fahrenheit> & t, Array<double> && p, Array<double> && z)
  {
    add_z(Zdata(t, move(p), move(z)));
  }

  using Zcomb = tuple<const Correlation*, PseudoPair,
		      PseudoPair, PseudoPair, const Correlation*, LFit>;

  static const LFit & fit(const Zcomb & z) { return get<5>(z); }
  static double sumsq(const Zcomb & z) { return fit(z).sumsq; }
  static double c(const Zcomb & z) { return fit(z).c; }
  static double m(const Zcomb & z) { return fit(z).m; }
  static DynList<string> zcomb_header()
  {
    return build_dynlist<string>("yghc", "ppchc", "tpchc", "ppcm", "tpcm",
				 "adjustedppcm", "adjustedtpcm", "z",
				 "sumsq", "c", "m");
  }
  
  static DynList<string> zcomb_to_str(const Zcomb & c)
  {
    return build_dynlist<string>(get<0>(c)->name,
				 get<1>(c).first->name,
				 get<1>(c).second->name,
				 get<2>(c).first->name,
				 get<2>(c).second->name,
				 get<3>(c).first->name,
				 get<3>(c).second->name,
				 get<4>(c)->name,
				 ::to_string(get<5>(c).sumsq),
				 ::to_string(get<5>(c).c),
				 ::to_string(get<5>(c).m));
  }

  static Correlation::NamedPar npar(const string & name, double val,
				    const Unit * unit_ptr)
  {
    return Correlation::NamedPar(true, name, val, unit_ptr);
  }

# define NPAR(NAME) npar(#NAME, NAME.raw(), &NAME.unit)

  DynList<string> exception_list;

  mutable DynList<const Correlation*> yghc_corrs = yghc_correlations();
  mutable DynList<PseudoPair> pch_corrs = pch_correlations();
  mutable DynList<PseudoPair> pcm_corrs = pcm_correlations();
  mutable DynList<PseudoPair> adjust_corrs = adjusted_correlations();
  mutable DynList<const Correlation*> z_corrs = zfactor_correlations();

# define Catch_Continue()			\
  catch (exception & e)				\
    {						\
      exception_list.append(e.what());		\
      continue;					\
    }

# define Catch_Break()			\
  catch (exception & e)				\
    {						\
      exception_list.append(e.what());		\
      break;					\
    }

  DynList<Zcomb> solve(bool check_z = false)
  {
    if (zvals.is_empty())
      ZENTHROW(EmptyVarSet, "data set does not contain p-z values");
    
    DynList<Zcomb> ret;
    auto zlab = sort(lab_values(), [] (auto & t1, auto & t2)
		    { return get<1>(t1) < get<1>(t2); });
    auto zx = zlab.maps<double>([] (auto & t) { return get<2>(t); });

    ParList pars;
    insert_in_container(pars, NPAR(yg), NPAR(n2), NPAR(co2), NPAR(h2s));
    for (auto yghc_it = yghc_corrs.get_it(); yghc_it.has_curr(); yghc_it.next())
      {
	const Correlation * yghc_corr = yghc_it.get_curr();
	try
	  {
	    Quantity<Sgg> yghc = yghc_corr->compute_by_names(pars, true);
	    pars.insert("yghc", yghc);
	  }
	Catch_Continue();
	for (auto pch_it = pch_corrs.get_it(); pch_it.has_curr(); pch_it.next())
	  {
	    auto & pch = pch_it.get_curr();
	    auto ppchc_corr = pch.first;
	    auto tpchc_corr = pch.second;
	    try
	      {
		auto ppchc = ppchc_corr->compute_by_names(pars, true);
		auto tpchc = tpchc_corr->compute_by_names(pars, true);
		insert_in_container(pars, NPAR(ppchc), NPAR(tpchc));
	      }
	    Catch_Continue();
	    for (auto pcm_it = pcm_corrs.get_it(); pcm_it.has_curr();
		 pcm_it.next())
	      {
		auto & pcm = pcm_it.get_curr();
		auto ppcm_corr = pcm.first;
		auto tpcm_corr = pcm.second;
		VtlQuantity ppcm = VtlQuantity::null_quantity;
		VtlQuantity tpcm = VtlQuantity::null_quantity;
		try
		  {
		    ppcm = ppcm_corr->compute_by_names(pars, true);
		    tpcm = tpcm_corr->compute_by_names(pars, true);
		    insert_in_container(pars, NPAR(ppcm), NPAR(tpcm));
		  }
		Catch_Continue();
		for (auto ad_it = adjust_corrs.get_it(); ad_it.has_curr();
		     ad_it.next())
		  {
		    auto & ad = ad_it.get_curr();
		    auto adjustedppcm_corr = ad.first;
		    auto adjustedtpcm_corr = ad.second;
		    try
		      {
			ppcm = adjustedppcm_corr->compute_by_names(pars, true);
			tpcm = adjustedtpcm_corr->compute_by_names(pars, true);
			remove_from_container(pars, "ppcm", "tpcm");
			insert_in_container(pars, NPAR(ppcm), NPAR(tpcm));
		      }
		    Catch_Continue();
		    DynList<double> zlist;
		    auto zit = z_corrs.get_it();
		  explore_correlations:
		    while (zit.has_curr())
		      {
			auto zcorr = zit.get_curr(); zit.next();
			for (auto it = zlab.get_it(); it.has_curr(); it.next())
			  {
			    auto & d = it.get_curr();
			    const auto & t = Quantity<Fahrenheit>(get<0>(d));
			    const auto & p = Quantity<psia>(get<1>(d));
			    VtlQuantity tpr = VtlQuantity::null_quantity;
			    VtlQuantity ppr = VtlQuantity::null_quantity;
			    try
			      {
				tpr = Tpr::get_instance().
				  compute({t, tpcm}, true);
				ppr = Ppr::get_instance().
				  compute({p, ppcm}, true);
			      }
			    Catch_Continue();
			    DynList<VtlQuantity> prpars =
			      build_dynlist<VtlQuantity>(tpr, ppr);
			    try
			      {
				auto z = zcorr->compute(prpars, check_z);
				zlist.append(z.raw());
			      }
			    catch (exception & e)
			      {
				exception_list.append(e.what());
				goto explore_correlations; // view next correlation
			      }
			  }
			LFit fit(zx, zlist); 
			zlist.empty();
			ret.append(Zcomb(yghc_corr,
					 make_pair(ppchc_corr, tpchc_corr),
					 make_pair(ppcm_corr, tpcm_corr),
					 make_pair(adjustedppcm_corr,
						   adjustedtpcm_corr),
					 zcorr, fit));
		      }
		    remove_from_container(pars, "ppcm", "tpcm");
		  }
	      }
	    remove_from_container(pars, "ppchc", "tpchc");
	  }
	pars.remove("yghc");
      }

    return ret;
  }

  static DynList<DynList<string>> to_dynlist(const DynList<Zcomb> & l)
  {
    DynList<DynList<string>> ret;
    ret.append(zcomb_header());
    ret.append(l.maps<DynList<string>>([] (auto & c) { return zcomb_to_str(c); }));
    return ret;
  }

  Ztuner() {}
};

# endif
