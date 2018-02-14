# ifndef CPLOT_H
# define CPLOT_H

# include <correlations/pvt-correlations.H>

struct Cplot
{
  /*********** The following attributes define fluid properties */

# define CONSTANT_PAR(__name, __unit)			\
  VtlQuantity __name;					\
  const Unit * __name##_unit = &__unit::get_instance();	\
							\
  void set_##__name(double val, const Unit & unit)	\
  {							\
    if (not unit.is_sibling(*__name##_unit))		\
      ZENTHROW(UnitNotFound, "unit " + unit.name + " is not for " +	\
	       __name##_unit->physical_quantity.name);			\
    __name = VtlQuantity(unit, val);					\
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
# define TEST_PARAMETER(required, name)					\
  if ((required.exists([&name](auto & p) { return p.first == name or	\
	    p.second.exists([&name](auto & s) { return s == name; }); }))) \
    pars_list.insert(name, value, unit_ptr)

  /**************** end of fluid properties parameters */

  // macro that constructs a parameter by name with name par from a VtlQuantity
# define NPAR(par) npar(#par, par)
  
  bool are_blackoil_parameters_set() const noexcept
  {
    return api != VtlQuantity::null_quantity and
      rsb != VtlQuantity::null_quantity and
      yg != VtlQuantity::null_quantity and
      tsep != VtlQuantity::null_quantity and
      psep != VtlQuantity::null_quantity;
  }

  /* Declare a new correlation type. That is, a correlation pointer of
     name NAME_corr and a setter function of name set_NAME().
						
     NAME is the expected target name of correlation and CORR is the
     correlation by default.

     The setter validates that the CORR target name is equal to NAME
     and that the returning unit is sibling.
     
  */
# define Declare_Correlation(NAME, CORR)				\
  const Correlation * NAME##_corr = &CORR::get_instance();		\
  void set_##NAME(const Correlation & corr)				\
  {									\
    assert(NAME##_corr);						\
    if (NAME##_corr->target_name() != #NAME)				\
      ZENTHROW(InvalidTargetType, NAME##_corr->name +			\
	       " is not for " #NAME " (probably set with macro)");	\
    if (corr.target_name() != #NAME)					\
      ZENTHROW(InvalidTargetType, corr.name +				\
	       " is not for " #NAME);					\
    if (not CORR::get_instance().unit.is_sibling(corr.unit))		\
      ZENTHROW(InvalidUnit, corr.name +					\
	       " unit is not sibling of " +				\
	       CORR::get_instance().unit.name);				\
    NAME##_corr = &corr;						\
  }									\
  									\
  void set_##NAME(const Correlation * corr_ptr)				\
  {									\
    set_##NAME(*corr_ptr);						\
  }
  
# define Declare_Tuned_Correlation(NAME, CORR)				\
  Declare_Correlation(NAME, CORR);					\
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

  Declare_Tuned_Correlation(pb, PbAlMarhoun);
  Declare_Tuned_Correlation(rs, RsAlMarhoun);
  Declare_Tuned_Correlation(bob, BobAlShammasi);
  Declare_Tuned_Correlation(boa, BoaMcCain);
  Declare_Tuned_Correlation(coa, CoaDeGhetto);
  Declare_Tuned_Correlation(uod, UodBeal);
  Declare_Tuned_Correlation(uob, UobBeggsRobinson);
  Declare_Tuned_Correlation(uoa, UoaAbedini);
  Declare_Tuned_Correlation(zfactor, ZfactorDranchukAK);

private:

  /* Verify that corr_ptr has the correct target name */
  static void
  verify_correlation(const Correlation & corr, const string & target_name)
  {
    if (corr.target_name() != target_name)
      ZENTHROW(InvalidTargetType,
	       corr.name + "  is not for" + target_name);
  }

  static void
  verify_correlation(const Correlation * corr_ptr, const string & target_name)
  {
    verify_correlation(*corr_ptr, target_name);
  }

public:

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

  Declare_Correlation(cob, CobMcCainEtAl);
  Declare_Correlation(ppchc, PpchcStanding);
  Declare_Correlation(ppcm_mixing, PpcmKayMixingRule);
  Declare_Correlation(adjustedppcm, AdjustedppcmWichertAziz);
  Declare_Correlation(tpchc, TpchcStanding);
  Declare_Correlation(tpcm_mixing, TpcmKayMixingRule);
  Declare_Correlation(adjustedtpcm, AdjustedtpcmWichertAziz);
  Declare_Correlation(cg, CgMattarBA);
  Declare_Correlation(ug, UgCarrKB);
  Declare_Correlation(bwb, BwbSpiveyMN);
  Declare_Correlation(bwa, BwaSpiveyMN);
  Declare_Correlation(uw, UwMcCain);
  Declare_Correlation(pw, PwSpiveyMN);
  Declare_Correlation(cwb, CwbSpiveyMN);
  Declare_Correlation(cwa, CwaSpiveyMN);
  Declare_Correlation(rsw, RswSpiveyMN);
  Declare_Correlation(sgo, SgoBakerSwerdloff);
  Declare_Correlation(sgw, SgwJenningsNewman);

  void init_blackoil()
  {
    blackoil_correlations_are_set();
  }
  
}; 

# endif
