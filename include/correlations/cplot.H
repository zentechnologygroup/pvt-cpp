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
  
# define Declare_Parameter(name, ref_unit)			\
  mutable VtlQuantity name = VtlQuantity::null_quantity;	\
  mutable Correlation::NamedPar name##_par;			\
  void set_##name(double val, const Unit & unit)		\
  {								\
    name = VtlQuantity(ref_unit, VtlQuantity(unit, val));	\
    name##_par = npar(#name, name);				\
  }

# define Declare_Default_Parameter(name, val, ref_unit)		\
  mutable VtlQuantity name = { ref_unit, val };			\
  mutable Correlation::NamedPar name##_par;			\
  void set_##name(double value, const Unit & unit)		\
  {								\
    name = VtlQuantity(ref_unit, VtlQuantity(unit, value));	\
    name##_par = npar(#name, name);				\
  }

  Declare_Parameter(api, Api::get_instance());
  Declare_Parameter(rsb, SCF_STB::get_instance());
  Declare_Parameter(yg, Sgg::get_instance());
  Declare_Parameter(tsep, Fahrenheit::get_instance());
  Declare_Parameter(tsep2, Fahrenheit::get_instance());
  Declare_Parameter(psep, psia::get_instance());
  Declare_Default_Parameter(nacl, 0, Molality_NaCl::get_instance());
  Declare_Default_Parameter(n2, 0, MolePercent::get_instance());
  Declare_Default_Parameter(co2, 0, MolePercent::get_instance());
  Declare_Default_Parameter(h2s, 0, MolePercent::get_instance());

  bool are_blackoil_bparameters_set() const noexcept
  {
    return api != VtlQuantity::null_quantity and
      rsb != VtlQuantity::null_quantity and
      yg != VtlQuantity::null_quantity and
      tsep != VtlQuantity::null_quantity and
      psep != VtlQuantity::null_quantity;
  }

# define Declare_Tuned_Correlation(name)	\
  const Correlation * name##_corr = nullptr;	\
  mutable double c_##name = 0;			\
  mutable double m_##name = 1;			\
						\
  void set_##name(const Correlation * __##name##_corr,			\
		  double c, double m) noexcept				\
  {									\
    name##_corr = __##name##_corr;					\
    c_##name = c;							\
    m_##name = m;							\
  }

# define Declare_Correlation(NAME, CORR)				\
  const Correlation * NAME##_corr = &CORR::get_instance();		\
  void set_##NAME(const Correlation * corr_ptr)			\
  {									\
    const string target_name = CORR::get_instance().target_name();	\
    if (NAME##_corr->target_name() != target_name)			\
      ZENTHROW(CorrelationNotFound, NAME##_corr->name +			\
	       " is not for " + target_name);				\
    NAME##_corr = corr_ptr;						\
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

  static void
  verify_corr(const Correlation * corr_ptr, const string & target_name)
  {
    if (corr_ptr == nullptr)
      ZENTHROW(CorrelationNotFound,
	       "correlation for " + target_name + " is not set");
    if (corr_ptr->target_name() != target_name)
      ZENTHROW(CorrelationNotFound,
	       corr_ptr->name + "  is not for" + target_name);
  }

  void blackoil_correlations_are_set() const
  {
    verify_corr(pb_corr, "pb");
    verify_corr(rs_corr, "rs");
    verify_corr(bob_corr, "bob");
    verify_corr(coa_corr, "coa");
    verify_corr(boa_corr, "boa");
    verify_corr(uod_corr, "uod");
    verify_corr(uob_corr, "uob");
    verify_corr(uoa_corr, "uoa");
    verify_corr(zfactor_corr, "zfactor");
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
