/** This file contains all the machinery needed for tuning z
   
   Aleph-w Leandro Rabindranath Leon
 */

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
      ALEPHTHROW(MismatchAuthors,						\
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
      z(to_Array(j["z"].get<vector<double>>()))
  {
    // empty
  }
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

  struct Desc
  {
    double t, p, z;
    Desc(double t, double p, double z) : t(t), p(p), z(z) {}
  };

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
    if (zvals.is_empty())
      s << "  empty";
    else
      {
	auto & last = zvals.get_last();
	for (auto it = zvals.get_it(); it.has_curr(); it.next())
	  {
	    auto & z = it.get_curr();
	    s << shift_lines_to_left(z.to_string(), 2);
	    if (&z != &last)
	      s << endl;
	  }
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
      add_z(jz);
  }

  Ztuner(istream && in) : Ztuner(in) {}

  DynList<Desc> lab_values() const
  {
    DynList<Desc> ret;
    zvals.for_each([&ret] (auto & d)
      {
	for (auto it = zip_it(d.p, d.z); it.has_curr(); it.next())
	  {
	    auto p = it.get_curr();
	    ret.append(Desc(d.t.raw(), get<0>(p), get<1>(p)));
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
      ALEPHTHROW(DuplicatedVarName, "duplicated z for t = " + z.t.to_string());
    
    zvals.append(move(z));
  }

  void
  add_z(const Quantity<Fahrenheit> & t, Array<double> && p, Array<double> && z)
  {
    add_z(Zdata(t, move(p), move(z)));
  }

  struct Zcomb
  {
    const Correlation * yghc_corr = nullptr;
    const Correlation * ppchc_corr = nullptr;
    const Correlation * tpchc_corr = nullptr;
    const Correlation * ppcm_corr = nullptr;
    const Correlation * tpcm_corr = nullptr;
    const Correlation * adjustedppcm_corr = nullptr;
    const Correlation * adjustedtpcm_corr = nullptr;
    const Correlation * z_corr = nullptr;
    mutable LFit lfit;
    mutable size_t i = 0;
    Zcomb() {}
    Zcomb(const Correlation * __yghc_corr,
	  const Correlation * __ppchc_corr, const Correlation * __tpchc_corr,
	  const Correlation * __ppcm_corr, const Correlation * __tpcm_corr,
	  const Correlation * __adjustedppcm_corr,
	  const Correlation * __adjustedtpcm_corr,
	  const Correlation * __z_corr, const LFit & __lfit, const size_t __i)
      : yghc_corr(__yghc_corr),
	ppchc_corr(__ppchc_corr), tpchc_corr(__tpchc_corr),
	ppcm_corr(__ppcm_corr), tpcm_corr(__tpcm_corr),
	adjustedppcm_corr(__adjustedppcm_corr),
	adjustedtpcm_corr(__adjustedtpcm_corr), z_corr(__z_corr),
	lfit(__lfit), i(__i) {}
    double c() const noexcept { return lfit.c; }
    double m() const noexcept { return lfit.m; }
    double sumsq() const noexcept { return lfit.sumsq; }
    double r2() const noexcept { return lfit.r2; }
    double sigma() const noexcept { return lfit.sigma; }
    double mse() const noexcept { return lfit.mse; }
    double num() const noexcept { return i; }
  };

  static DynList<string> zcomb_header()
  {
    return build_dynlist<string>("yghc", "ppchc", "tpchc", "ppcm", "tpcm",
				 "adjustedppcm", "adjustedtpcm", "z", "r2",
				 "sumsq", "sigma", "mse", "c", "m", "num");
  }
  
  static DynList<string> zcomb_to_str(const Zcomb & c)
  {
    return build_dynlist<string>(c.yghc_corr->name,
				 c.ppchc_corr->name, c.tpchc_corr->name,
				 c.ppcm_corr->name, c.tpcm_corr->name,
				 c.adjustedppcm_corr->name,
				 c.adjustedtpcm_corr->name,
				 c.z_corr->name, ::to_string(c.r2()),
				 ::to_string(c.sumsq()), ::to_string(c.sigma()),
				 ::to_string(c.mse()), ::to_string(c.c()),
				 ::to_string(c.m()), ::to_string(c.i));
  }

  static Correlation::NamedPar npar(const string & name, double val,
				    const Unit * unit_ptr)
  {
    return Correlation::NamedPar(true, name, val, unit_ptr);
  }

  static Correlation::NamedPar npar(const string & name, const VtlQuantity & q)
  {
    return Correlation::NamedPar(true, name, q.raw(), &q.unit);
  }

# define NPAR(NAME) npar(#NAME, NAME.raw(), &NAME.unit)

  DynList<string> exception_list;

  Array<Zcomb> zcomb_list;

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
      ALEPHTHROW(EmptyVarSet, "data set does not contain p-z values");
    
    auto zlab = sort(lab_values(), [] (auto & d1, auto & d2)
		    { return d1.p < d2.p; });
    auto zx = zlab.maps<double>([] (auto & d) { return d.z; });

    size_t num = 0;
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
			    const auto & t = Quantity<Fahrenheit>(d.t);
			    const auto & p = Quantity<psia>(d.p);
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
			LFit fit(zlist, zx); 
			zlist.empty();
			zcomb_list.append(Zcomb(yghc_corr, ppchc_corr, tpchc_corr,
						ppcm_corr, tpcm_corr,
						adjustedppcm_corr,
						adjustedtpcm_corr,
						zcorr, fit, num++));
		      }
		    remove_from_container(pars, "ppcm", "tpcm");
		  }
	      }
	    remove_from_container(pars, "ppchc", "tpchc");
	  }
	pars.remove("yghc");
      }

        // zcomb_list is an array, so maps convert it in a DynList
    return zcomb_list.maps([] (const Zcomb & z) { return z; });
  }

  //               t,      p vals,          z vals
  DynList<tuple<double, DynList<double>, DynList<double>>> vals() const
  {
    DynList<tuple<double, DynList<double>, DynList<double>>> ret;
    for (auto it = zvals.get_it(); it.has_curr(); it.next())
      {
	const Zdata & zdata = it.get_curr();
	ret.append(make_tuple(zdata.t.raw(), to_dynlist(zdata.p),
	 		      to_dynlist(zdata.z)));
      }
    return ret;
  }

  DynList<string> basic_header() const
  {
    DynList<string> ret;
    for (auto it = zvals.get_it(); it.has_curr(); it.next())
      {
	auto & z = it.get_curr();
	const string t = ::to_string(int(z.t.raw()));
	ret.append(build_dynlist<string>("p." + t, "z." + t));
      }
    return ret;
  }

  //                t,        zvals,         calibrated zvals
  DynList<tuple<double, DynList<double>, DynList<double>>>
	 eval(const Zcomb & z, bool check) const
  {
    ParList pars;
    insert_in_container(pars, NPAR(yg), NPAR(n2), NPAR(co2), NPAR(h2s));
    Quantity<Sgg> yghc = z.yghc_corr->compute_by_names(pars, true);
    pars.insert("yghc", yghc);

    auto ppchc = z.ppchc_corr->compute_by_names(pars, true);
    auto tpchc = z.tpchc_corr->compute_by_names(pars, true);
    insert_in_container(pars, NPAR(ppchc), NPAR(tpchc));

    VtlQuantity ppcm = z.ppcm_corr->compute_by_names(pars, true);
    VtlQuantity tpcm = z.tpcm_corr->compute_by_names(pars, true);
    insert_in_container(pars, NPAR(ppcm), NPAR(tpcm));

    ppcm = z.adjustedppcm_corr->compute_by_names(pars, true);
    tpcm = z.adjustedtpcm_corr->compute_by_names(pars, true);
    remove_from_container(pars, "ppcm", "tpcm");
    insert_in_container(pars, NPAR(ppcm), NPAR(tpcm));

    DynList<tuple<double, DynList<double>, DynList<double>>> ret;

    const double c = z.lfit.c;
    const double m = z.lfit.m;

    for (auto vit = zvals.get_it(); vit.has_curr(); vit.next())
      {
	const Zdata & zdata = vit.get_curr();
	auto tpr = Tpr::get_instance().compute({zdata.t, tpcm}, true);
	DynList<double> zc; // computed values for given z correlation
	for (auto it = zdata.p.get_it(); it.has_curr(); it.next())
	  {
	    auto pp = it.get_curr();
	    Quantity<psia> p(pp);
	    auto ppr = Ppr::get_instance().compute({p, ppcm}, check);
	    auto zval =
	      z.z_corr->compute(build_dynlist<VtlQuantity>(tpr, ppr), check);
	    zc.append(zval.raw());
	  }

	auto zcali = zc.maps([c, m] (auto z) { return c + m*z; });
	auto val = make_tuple(zdata.t.raw(), move(zc), move(zcali));
	ret.append(val);
      }

    return ret;
  }

  static DynList<DynList<string>> zcomb_to_dynlist(const DynList<Zcomb> & l)
  {
    DynList<DynList<string>> ret;
    ret.append(zcomb_header());
    ret.append(l.maps<DynList<string>>([] (auto & c) { return zcomb_to_str(c); }));
    return ret;
  }

  Ztuner() {}
};

# endif
