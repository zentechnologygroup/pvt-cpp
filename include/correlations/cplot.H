# ifndef CPLOT_H
# define CPLOT_H

# include <correlations/pvt-correlations.H>
# include <correlations/defined-correlation.H>
# include <correlations/correlation-invoker.H>
# include <correlations/uo-min.H>

class Cplot
{
  CorrelationInvoker inv;

public:

  bool check;

  Cplot(const Unit & tunit, const Unit & punit, bool __check)
    : inv(tunit, punit), check(__check) {}

# define CONSTANT_PAR(NAME, __unit)			\
  VtlQuantity NAME;					\
  const Unit * NAME##_unit = &__unit::get_instance();	\
							\
  void set_##NAME(double val, const Unit & unit)	\
  {							\
    if (not unit.is_sibling(*NAME##_unit))		\
      ZENTHROW(InvalidUnit, "unit " + unit.name + " is not for " +	\
	       NAME##_unit->physical_quantity.name);			\
    NAME = VtlQuantity(unit, val);					\
  }									\
									\
  void test_##NAME() const						\
  {									\
    if (NAME.is_null())							\
      ZENTHROW(ValueNotFound, #NAME " value has not been set");		\
  }

  CONSTANT_PAR(api, Api);
  CONSTANT_PAR(rsb, SCF_STB);
  CONSTANT_PAR(yg, Sgg);
  CONSTANT_PAR(tsep, Fahrenheit);
  CONSTANT_PAR(tsep2, Fahrenheit);
  CONSTANT_PAR(psep, psia);
  CONSTANT_PAR(ogr, STB_MMscf);
  CONSTANT_PAR(n2, MolePercent);
  CONSTANT_PAR(co2, MolePercent);
  CONSTANT_PAR(h2s, MolePercent);
  CONSTANT_PAR(nacl, Molality_NaCl);

  /** Given a list of required parameters, this function tests if
      `name` is inside. It this is the case, then `name` is inserted
      in `pars_list`.

      `required` parameter is a list of pairs. Each pair is composed
      by the parameter name and a list of parameter alias.

      This method is intended to be used for constant parameters.
   */
  static void
  test_parameter(const DynList<pair<string, DynList<string>>> & required,
		 const string & name, const VtlQuantity & par,
		 ParList & pars_list)
  {
    if (required.exists([&name] (auto & p) { return p.first == name or
	    p.second.exists([&name] (auto & s) { return s == name; }); }))
      pars_list.insert(name, par);
  }

  ParList load_constant_parameters(const DynList<const Correlation*> & l)
  {
    ParList pars_list;
    auto required_pars = DefinedCorrelation::parameter_list(l);
				  
    test_parameter(required_pars, "api", api, pars_list);
    test_parameter(required_pars, "rsb", rsb, pars_list);
    test_parameter(required_pars, "yg", yg, pars_list);
    test_parameter(required_pars, "tsep", tsep, pars_list);
    test_parameter(required_pars, "psep", psep, pars_list);
    test_parameter(required_pars, "n2", n2, pars_list);
    test_parameter(required_pars, "co2", co2, pars_list);
    test_parameter(required_pars, "h2s", h2s, pars_list);
    test_parameter(required_pars, "nacl", nacl, pars_list);

    return pars_list;
  }
  
  bool are_blackoil_parameters_set() const noexcept
  {
    return api != VtlQuantity::null_quantity and
      rsb != VtlQuantity::null_quantity and
      yg != VtlQuantity::null_quantity and
      tsep != VtlQuantity::null_quantity and
      psep != VtlQuantity::null_quantity;
  }

# define Declare_Correlation_Common(NAME)	\
  void test_##NAME##_corr() const					\
  {									\
    if (NAME##_corr == nullptr)						\
      ZENTHROW(CorrelationNotFound, "Correlation for " #NAME		\
	       " has not been set");					\
  }

  /* Declare a new correlation type. That is, a correlation pointer of
     name NAME_corr and a setter function of name set_NAME().
						
     NAME is the expected target name of correlation and CORR is the
     correlation by default.

     The setter validates that the CORR target name is equal to NAME
     and that the returning unit is sibling.     
  */
# define Declare_Correlation(NAME)					\
  const Correlation * NAME##_corr = nullptr;				\
									\
  Declare_Correlation_Common(NAME);					\
									\
  void set_##NAME(const Correlation & corr)				\
  {									\
    if (corr.target_name() != #NAME)					\
      ZENTHROW(InvalidTargetType, corr.name +				\
	       " is not for " #NAME);					\
    NAME##_corr = &corr;						\
  }									\
									\
  void set_##NAME##_corr(const Correlation * corr_ptr)			\
  {									\
    set_##NAME(*corr_ptr);						\
  }   

# define Declare_Optional_Correlation(NAME, CORR)			\
  const Correlation * NAME##_corr = &CORR::get_instance();		\
									\
  Declare_Correlation_Common(NAME);					\
									\
  void set_##NAME(const Correlation & corr)				\
  {									\
    assert(NAME##_corr);						\
    if (NAME##_corr->target_name() != #NAME)				\
      ZENTHROW(InvalidTargetType, NAME##_corr->name +			\
	       " is not for " #NAME " (probably set with macro)");	\
    if (corr.target_name() != #NAME)					\
      ZENTHROW(InvalidTargetType, corr.name + " is not for " #NAME);	\
    NAME##_corr = &corr;						\
  }									\
									\
  void set_##NAME(const Correlation * corr_ptr)				\
  {									\
    set_##NAME(*corr_ptr);						\
  }
  
# define Declare_Tuned_Correlation(NAME)				\
  Declare_Correlation(NAME);						\
  mutable double c_##NAME = 0;						\
  mutable double m_##NAME = 1;						\
									\
  void set_##NAME(const Correlation & __##NAME##_corr,			\
		  double c, double m) noexcept				\
  {									\
    set_##NAME(__##NAME##_corr);					\
    c_##NAME = c;							\
    m_##NAME = m;							\
  }									\
									\
  void set_##NAME(const Correlation * __##NAME##_corr,			\
		  double c, double m) noexcept				\
  {									\
    set_##NAME(*__##NAME##_corr, c, m);					\
  }

  Declare_Tuned_Correlation(pb);
  Declare_Tuned_Correlation(rs);
  Declare_Tuned_Correlation(bob);
  Declare_Tuned_Correlation(boa);
  Declare_Tuned_Correlation(coa);
  Declare_Tuned_Correlation(uod);
  Declare_Tuned_Correlation(uob);
  Declare_Tuned_Correlation(uoa);
  Declare_Tuned_Correlation(zfactor);

  /* Verify that corr_ptr has the correct target name */
  static void
  verify_correlation(const Correlation * corr_ptr, const string & target_name)
  {
    if (corr_ptr == nullptr)
      ZENTHROW(CorrelationNotFound, "Correlation for " + target_name +
	       " has not been defined");
    if (corr_ptr->target_name() != target_name)
      ZENTHROW(InvalidTargetType,
	       corr_ptr->name + "  is not for" + target_name);
  }

  void blackoil_correlations_are_set() const
  {
    verify_correlation(pb_corr, "pb");
    verify_correlation(rs_corr, "rs");
    verify_correlation(bob_corr, "bob");
    verify_correlation(coa_corr, "coa");
    verify_correlation(boa_corr, "boa");
    verify_correlation(uod_corr, "uod");
    verify_correlation(uob_corr, "uob");
    verify_correlation(uoa_corr, "uoa");
    verify_correlation(zfactor_corr, "zfactor");
  }

  Declare_Optional_Correlation(cob, CobMcCainEtAl);
  Declare_Optional_Correlation(ppchc, PpchcStanding);
  Declare_Optional_Correlation(ppcm_mixing, PpcmKayMixingRule);
  Declare_Optional_Correlation(adjustedppcm, AdjustedppcmWichertAziz);
  Declare_Optional_Correlation(tpchc, TpchcStanding);
  Declare_Optional_Correlation(tpcm_mixing, TpcmKayMixingRule);
  Declare_Optional_Correlation(adjustedtpcm, AdjustedtpcmWichertAziz);
  Declare_Optional_Correlation(cg, CgMattarBA);
  Declare_Optional_Correlation(ug, UgCarrKB);
  Declare_Optional_Correlation(bwb, BwbSpiveyMN);
  Declare_Optional_Correlation(bwa, BwaSpiveyMN);
  Declare_Optional_Correlation(uw, UwMcCain);
  Declare_Optional_Correlation(pw, PwSpiveyMN);
  Declare_Optional_Correlation(cwb, CwbSpiveyMN);
  Declare_Optional_Correlation(cwa, CwaSpiveyMN);
  Declare_Optional_Correlation(rsw, RswSpiveyMN);
  Declare_Optional_Correlation(sgo, SgoBakerSwerdloff);
  Declare_Optional_Correlation(sgw, SgwJenningsNewman);
  
private:
  
  // These vectors are used for computing grid
  Array<double> t_values, p_values; // the units are in inv attribute

  VtlQuantity t_q, p_q;
  double temperature, pressure;

  VtlQuantity bobp, pobp, bwbp;
  NamedPar pb_par, t_par, p_par;

  VtlQuantity yghc, ppchc, ppcm, tpchc, tpcm, adjustedppcm, adjustedtpcm, pb_q;
  VtlQuantity min_uo;

  ParList pb_pars, rs_pars, uod_pars, bo_pars, co_pars, uo_pars, po_pars,
    ug_pars, bw_pars, uw_pars, pw_pars, rsw_pars, cw_pars, cwa_pars, cg_pars,
    sgo_pars, sgw_pars;

public:

  void blackoil_ready()
  {
    blackoil_correlations_are_set();
    test_api();
    test_rsb();
    test_yg();
    test_tsep();
    test_psep();
    test_h2s();
    test_co2();
    test_n2();
    test_nacl();
 
    test_pb_corr();
    test_rs_corr();
    test_bob_corr();
    test_boa_corr();
    test_uod_corr();
    test_cob_corr();
    test_coa_corr();
    test_uob_corr();
    test_uoa_corr();
    test_ppchc_corr();
    test_tpchc_corr();
    test_ppcm_mixing_corr();
    test_tpcm_mixing_corr();
    test_adjustedppcm_corr();
    test_adjustedtpcm_corr();
    test_zfactor_corr();
    test_cg_corr();
    test_ug_corr();
    test_bwb_corr();
    test_bwa_corr();
    test_uw_corr();
    test_pw_corr();
    test_rsw_corr();
    test_cwb_corr();
    test_cwa_corr();
    test_sgo_corr();
    test_sgw_corr();

    /* Calculation of constants for Z */
    yghc = inv.compute_exc(YghcWichertAziz::correlation(), true, NPAR(yg),
			   NPAR(n2), NPAR(co2), NPAR(h2s)); 
    ppchc = inv.compute_exc(ppchc_corr, true, NPAR(yghc), NPAR(n2),
			    NPAR(co2), NPAR(h2s));
    ppcm = inv.compute_exc(ppcm_mixing_corr, true, NPAR(ppchc), NPAR(n2),
			   NPAR(co2), NPAR(h2s));
    tpchc = tpchc_corr->compute(check, yghc);
    tpcm = inv.compute_exc(tpcm_mixing_corr, true, NPAR(tpchc),
			   NPAR(n2), NPAR(co2), NPAR(h2s));
    adjustedppcm = inv.compute_exc(adjustedppcm_corr, true, NPAR(ppcm),
				   NPAR(tpcm), NPAR(co2), NPAR(h2s));
     adjustedtpcm = inv.compute_exc(adjustedtpcm_corr, true, NPAR(tpcm),
				    NPAR(co2), NPAR(h2s));
    /* End calculation constants for z */

     min_uo = min_uo_val();

     /* Initialization of correlation parameter lists */
     pb_pars = load_constant_parameters({pb_corr});
     rs_pars = load_constant_parameters({rs_corr, &RsAbovePb::get_instance()});
     uod_pars = load_constant_parameters({uod_corr});
     bo_pars = load_constant_parameters({bob_corr, boa_corr});
     co_pars = load_constant_parameters({cob_corr, coa_corr});
     uo_pars = load_constant_parameters({uob_corr, uoa_corr});
     po_pars = load_constant_parameters({&PobBradley::get_instance(),
	   &PoaBradley::get_instance()});
     ug_pars = load_constant_parameters({ug_corr});
     insert_in_container(ug_pars, npar("tpc", adjustedtpcm),
			 npar("ppc", adjustedppcm));
     bw_pars = load_constant_parameters({bwb_corr, bwa_corr});
     uw_pars = load_constant_parameters({uw_corr});
     pw_pars = load_constant_parameters({pw_corr});
     rsw_pars = load_constant_parameters({rsw_corr});
     cw_pars = load_constant_parameters({cwb_corr, cwa_corr});
     cwa_pars = load_constant_parameters({cwa_corr});
     cg_pars.insert(npar("ppc", ppcm));
     sgo_pars = load_constant_parameters({sgo_corr});
  }

  DefinedCorrelation rs_corr;

  void blackoil_temperature_calculations()
  {
    
  }


}; 

# endif
