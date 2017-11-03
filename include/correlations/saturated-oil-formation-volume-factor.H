#ifndef SATURATED_OIL_FORMATION_VOLUME_FACTOR_H
#define SATURATED_OIL_FORMATION_VOLUME_FACTOR_H

#include <pvt-exceptions.H>
#include <correlations/correlation.H>

/** BobAlmarhoun

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group


References:

  - alMarhoun:1988

@param[in] yg Gas specific gravity. Range in [0.752, 1.367]
@param[in] yo Oil specific gravity. Range in [Quantity<Api>(44.6),
Quantity<Api>(19.4)]
@param[in] rs Solution GOR. Range in [26, 1602]
@param[in] t Temperature. Range in [Quantity<Fahrenheit>(74),
Quantity<Fahrenheit>(240)]
@return inside [1.032, 1.997]
@ingroup Correlations
*/
class BobAlmarhoun : public SaturatedOilVolumeFactor {
  Correlation_Singleton(BobAlmarhoun);

  BobAlmarhoun()
      : SaturatedOilVolumeFactor("BobAlmarhoun", RB_STB::get_instance(), 1.032,
                                 1.997) {
    set_author("Al-Marhoun");
    set_title("AL-MARHOUN CORRELATION, CALCULATION OF SATURATED OIL FORMATION "
              "VOLUME FACTOR");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.752,
                  1.367);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(44.6), Quantity<Api>(19.4));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 26, 1602);
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("t", Rankine::get_instance(), "Temperature",
                  Quantity<Fahrenheit>(74), Quantity<Fahrenheit>(240));
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 69 bottomhole fluid samples from 69 Middle East oil "
           "reservoirs.");
    add_ref("alMarhoun:1988");
    add_par_synonym("yo", "api", "Api");
  }

public:
  static inline double impl(const double& yg, const double& yo,
                            const double& rs, const double& t) noexcept;

  Quantity<RB_STB> call(const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
                        const Quantity<SCF_STB>& rs,
                        const Quantity<Rankine>& t) const {
    return Quantity<RB_STB>(impl(yg.raw(), yo.raw(), rs.raw(), t.raw()));
  }

  Quantity<RB_STB> validate_and_call(const Quantity<Sgg>& yg,
                                     const Quantity<Sg_do>& yo,
                                     const Quantity<SCF_STB>& rs,
                                     const Quantity<Rankine>& t) const {
    if (not(yg >= 0.752))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.752");
    if (not(yg <= 1.367))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.367");
    if (not(yo >= Quantity<Api>(44.6)))
      ZENTHROW(OutOfParameterRange,
               "parameter yo is less than Quantity<Api>(44.6)");
    if (not(yo <= Quantity<Api>(19.4)))
      ZENTHROW(OutOfParameterRange,
               "parameter yo is greater than Quantity<Api>(19.4)");
    if (not(rs >= 26))
      ZENTHROW(OutOfParameterRange, "parameter rs is less than 26");
    if (not(rs <= 1602))
      ZENTHROW(OutOfParameterRange, "parameter rs is greater than 1602");

    return call(yg, yo, rs, t);
  }

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<Rankine>& t) const {
    return Quantity<RB_STB>(call(yg, yo, rs, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, yo, rs, t);
  }
};

extern Quantity<RB_STB> BobAlmarhoun__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
    const Quantity<SCF_STB>& rs, const Quantity<Rankine>& t);
extern double BobAlmarhoun__correlation__valid__fct(const double& yg,
                                                    const double& yo,
                                                    const double& rs,
                                                    const double& t);

/** BobAlShammasi

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group


References:

  - alShammasi:1999

@param[in] yg Gas specific gravity. Range in [0.51, 3.44]
@param[in] yo Oil specific gravity. Range in [Quantity<Api>(63.7),
Quantity<Api>(6)]
@param[in] rs Solution GOR. Range in [6, 3298.6]
@param[in] t Temperature. Range in [74, 341.6]
@return inside [1.02, 2.916]
@ingroup Correlations
*/
class BobAlShammasi : public SaturatedOilVolumeFactor {
  Correlation_Singleton(BobAlShammasi);

  BobAlShammasi()
      : SaturatedOilVolumeFactor("BobAlShammasi", RB_STB::get_instance(), 1.02,
                                 2.916) {
    set_author("Al-Shammasi");
    set_title("Al-SHAMMASI CORRELATION, CALCULATION OF SATURATED OIL FORMATION "
              "VOLUME FACTOR");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.51,
                  3.44);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(63.7), Quantity<Api>(6));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 6, 3298.6);
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 74, 341.6);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on a total of 1709 data sets (1661 data sets from 13 "
           "different published literature papers and 48 data sets originated "
           "from Kuwait reservoirs from unpublished sources).");
    add_db("The data available after excluding the duplicates and the repeated "
           "data sets for formation volume factor are 1345 data sets.");
    add_ref("alShammasi:1999");
    add_par_synonym("yo", "api", "Api");
  }

public:
  static inline double impl(const double& yg, const double& yo,
                            const double& rs, const double& t) noexcept;

  Quantity<RB_STB> call(const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
                        const Quantity<SCF_STB>& rs,
                        const Quantity<Fahrenheit>& t) const {
    return Quantity<RB_STB>(impl(yg.raw(), yo.raw(), rs.raw(), t.raw()));
  }

  Quantity<RB_STB> validate_and_call(const Quantity<Sgg>& yg,
                                     const Quantity<Sg_do>& yo,
                                     const Quantity<SCF_STB>& rs,
                                     const Quantity<Fahrenheit>& t) const {
    if (not(yg >= 0.51))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.51");
    if (not(yg <= 3.44))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 3.44");
    if (not(yo >= Quantity<Api>(63.7)))
      ZENTHROW(OutOfParameterRange,
               "parameter yo is less than Quantity<Api>(63.7)");
    if (not(yo <= Quantity<Api>(6)))
      ZENTHROW(OutOfParameterRange,
               "parameter yo is greater than Quantity<Api>(6)");
    if (not(rs >= 6))
      ZENTHROW(OutOfParameterRange, "parameter rs is less than 6");
    if (not(rs <= 3298.6))
      ZENTHROW(OutOfParameterRange, "parameter rs is greater than 3298.6");

    return call(yg, yo, rs, t);
  }

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<Fahrenheit>& t) const {
    return Quantity<RB_STB>(call(yg, yo, rs, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, yo, rs, t);
  }
};

extern Quantity<RB_STB> BobAlShammasi__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
    const Quantity<SCF_STB>& rs, const Quantity<Fahrenheit>& t);
extern double BobAlShammasi__correlation__valid__fct(const double& yg,
                                                     const double& yo,
                                                     const double& rs,
                                                     const double& t);

/** BobCegarra

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group

Cegarra didn't present a correlation for Bo. Al-Shammasi's correlation is used
to calculate Bob because of the similarity of his development ranges with
Cegarra's.

References:

  - cegarra:2007

  - alShammasi:1999

@param[in] yg Gas specific gravity. Range in [0.654, 1.337]
@param[in] yo Oil specific gravity. Range in [Quantity<Api>(41.9),
Quantity<Api>(22)]
@param[in] rs Solution GOR. Range in [135, 4290]
@param[in] t Temperature. Range in [117, 354]
@return inside [, ]
@ingroup Correlations
*/
class BobCegarra : public SaturatedOilVolumeFactor {
  Correlation_Singleton(BobCegarra);

  BobCegarra()
      : SaturatedOilVolumeFactor("BobCegarra", RB_STB::get_instance()) {
    set_author("Cegarra");
    set_title("CEGARRA CORRELATION (AL-SHAMMASI CORRELATION APPLIED FOR "
              "CEGARRA), CALCULATION OF SATURATED OIL FORMATION VOLUME FACTOR");
    set_hidden_calc();
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.654,
                  1.337);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(41.9), Quantity<Api>(22));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 135, 4290);
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 117, 354);
    def_parameter_latex_symbol("t", "T");
    add_note("Cegarra didn't present a correlation for Bo. Al-Shammasi's "
             "correlation is used to calculate Bob because of the similarity "
             "of his development ranges with Cegarra's.");
    add_ref("cegarra:2007");
    add_ref("alShammasi:1999");
    add_par_synonym("yo", "api", "Api");
  }

public:
  static inline double impl(const double& yg, const double& yo,
                            const double& rs, const double& t) noexcept;

  Quantity<RB_STB> call(const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
                        const Quantity<SCF_STB>& rs,
                        const Quantity<Fahrenheit>& t) const {
    return Quantity<RB_STB>(impl(yg.raw(), yo.raw(), rs.raw(), t.raw()));
  }

  Quantity<RB_STB> validate_and_call(const Quantity<Sgg>& yg,
                                     const Quantity<Sg_do>& yo,
                                     const Quantity<SCF_STB>& rs,
                                     const Quantity<Fahrenheit>& t) const {
    if (not(yg >= 0.654))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.654");
    if (not(yg <= 1.337))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.337");
    if (not(yo >= Quantity<Api>(41.9)))
      ZENTHROW(OutOfParameterRange,
               "parameter yo is less than Quantity<Api>(41.9)");
    if (not(yo <= Quantity<Api>(22)))
      ZENTHROW(OutOfParameterRange,
               "parameter yo is greater than Quantity<Api>(22)");
    if (not(rs >= 135))
      ZENTHROW(OutOfParameterRange, "parameter rs is less than 135");
    if (not(rs <= 4290))
      ZENTHROW(OutOfParameterRange, "parameter rs is greater than 4290");

    return call(yg, yo, rs, t);
  }

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<Fahrenheit>& t) const {
    return Quantity<RB_STB>(call(yg, yo, rs, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, yo, rs, t);
  }
};

extern Quantity<RB_STB> BobCegarra__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
    const Quantity<SCF_STB>& rs, const Quantity<Fahrenheit>& t);
extern double BobCegarra__correlation__valid__fct(const double& yg,
                                                  const double& yo,
                                                  const double& rs,
                                                  const double& t);

/** BobDeGhetto

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group


De Ghetto et al. recommend the use of the Vazquez & Beggs correlation for the
estimation of Bo. The values for the coefficients c1, c2, and c3 are selected
only for API<=30.

References:

  - deGhetto:1995

@param[in] yg Gas specific gravity. Range in [0.623, 1.517]
@param[in] api API oil gravity. Range in [6, 22.3]
@param[in] rs Solution GOR. Range in [17.21, 640.25]
@param[in] t Temperature. Range in [131.4, 250.7]
@param[in] tsep Separator temperature. Range in [59, 177.8]
@param[in] psep Separator pressure. Range in [14.5, 752.2]
@return inside [1.057, 1.362]
@ingroup Correlations
*/
class BobDeGhetto : public SaturatedOilVolumeFactor {
  Correlation_Singleton(BobDeGhetto);

  BobDeGhetto()
      : SaturatedOilVolumeFactor("BobDeGhetto", RB_STB::get_instance(), 1.057,
                                 1.362) {
    set_author("De Ghetto, Paone & Villa");
    set_title("DE GHETTO, PAONE & VILLA CORRELATION FOR EXTRA-HEAVY AND HEAVY "
              "OILS (VAZQUEZ & BEGGS CORRELATION), CALCULATION OF SATURATED "
              "OIL FORMATION VOLUME FACTOR");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.623,
                  1.517);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("api", Api::get_instance(), "API oil gravity", 6, 22.3);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 17.21, 640.25);
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 131.4, 250.7);
    def_parameter_latex_symbol("t", "T");
    add_parameter("tsep", Fahrenheit::get_instance(), "Separator temperature",
                  59, 177.8);
    def_parameter_latex_symbol("tsep", "T_{sp}");
    add_parameter("psep", psia::get_instance(), "Separator pressure", 14.5,
                  752.2);
    def_parameter_latex_symbol("psep", "P_{sp}");
    add_db("Based on 1200 measured data points of 63 heavy and extra-heavy oil "
           "samples obtained from the Mediterranean Basin, Africa and the "
           "Persian Gulf.");
    add_db("Oil samples have been divided in two different API gravity "
           "classes: extra-heavy oils for °API<=10, heavy oils for "
           "10<°API<=22.3.");
    add_ref("deGhetto:1995");
  }

public:
  static inline double impl(const double& yg, const double& api,
                            const double& rs, const double& t,
                            const double& tsep, const double& psep) noexcept;

  Quantity<RB_STB> call(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                        const Quantity<SCF_STB>& rs,
                        const Quantity<Fahrenheit>& t,
                        const Quantity<Fahrenheit>& tsep,
                        const Quantity<psia>& psep) const {
    return Quantity<RB_STB>(
        impl(yg.raw(), api.raw(), rs.raw(), t.raw(), tsep.raw(), psep.raw()));
  }

  Quantity<RB_STB> validate_and_call(const Quantity<Sgg>& yg,
                                     const Quantity<Api>& api,
                                     const Quantity<SCF_STB>& rs,
                                     const Quantity<Fahrenheit>& t,
                                     const Quantity<Fahrenheit>& tsep,
                                     const Quantity<psia>& psep) const {
    if (not(yg >= 0.623))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.623");
    if (not(yg <= 1.517))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.517");
    if (not(api >= 6))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 6");
    if (not(api <= 22.3))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 22.3");
    if (not(rs >= 17.21))
      ZENTHROW(OutOfParameterRange, "parameter rs is less than 17.21");
    if (not(rs <= 640.25))
      ZENTHROW(OutOfParameterRange, "parameter rs is greater than 640.25");
    if (not(tsep >= 59))
      ZENTHROW(OutOfParameterRange, "parameter tsep is less than 59");
    if (not(tsep <= 177.8))
      ZENTHROW(OutOfParameterRange, "parameter tsep is greater than 177.8");
    if (not(psep >= 14.5))
      ZENTHROW(OutOfParameterRange, "parameter psep is less than 14.5");
    if (not(psep <= 752.2))
      ZENTHROW(OutOfParameterRange, "parameter psep is greater than 752.2");

    return call(yg, api, rs, t, tsep, psep);
  }

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<Fahrenheit>& tsep,
                              const Quantity<psia>& psep) const {
    return Quantity<RB_STB>(call(yg, api, rs, t, tsep, psep));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& tsep = it.get_curr();
    it.next();
    const auto& psep = it.get_curr();

    return call(yg, api, rs, t, tsep, psep);
  }
};

extern Quantity<RB_STB> BobDeGhetto__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<Api>& api,
    const Quantity<SCF_STB>& rs, const Quantity<Fahrenheit>& t,
    const Quantity<Fahrenheit>& tsep, const Quantity<psia>& psep);
extern double BobDeGhetto__correlation__valid__fct(
    const double& yg, const double& api, const double& rs, const double& t,
    const double& tsep, const double& psep);

/** BobDindorukChristman

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group


The range assigned for Rs was taken from the range expressed by the authors for
Rsb.

References:

  - dindoruk:2004

@param[in] yg Gas specific gravity. Range in [0.6017, 1.027]
@param[in] api API oil gravity. Range in [14.7, 40]
@param[in] rs Solution GOR. Range in [133, 3050]
@param[in] t Temperature. Range in [117, 276]
@param[in] tsep Separator temperature. Range in [, ]
@return inside [1.0845, 2.7381]
@ingroup Correlations
*/
class BobDindorukChristman : public SaturatedOilVolumeFactor {
  Correlation_Singleton(BobDindorukChristman);

  BobDindorukChristman()
      : SaturatedOilVolumeFactor("BobDindorukChristman", RB_STB::get_instance(),
                                 1.0845, 2.7381) {
    set_author("Dindoruk & Christman");
    set_title("DINDORUK & CHRISTMAN CORRELATION, CALCULATION OF SATURATED OIL "
              "FORMATION VOLUME FACTOR");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.6017,
                  1.027);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("api", Api::get_instance(), "API oil gravity", 14.7, 40);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 133, 3050);
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 117, 276);
    def_parameter_latex_symbol("t", "T");
    add_parameter("tsep", Fahrenheit::get_instance(), "Separator temperature");
    def_parameter_latex_symbol("tsep", "T_{sp}");
    add_db("Based on approximately 100 PVT reports from the Gulf of Mexico.");
    add_ref("dindoruk:2004");
  }

public:
  static inline double impl(const double& yg, const double& api,
                            const double& rs, const double& t,
                            const double& tsep) noexcept;

  Quantity<RB_STB> call(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                        const Quantity<SCF_STB>& rs,
                        const Quantity<Fahrenheit>& t,
                        const Quantity<Fahrenheit>& tsep) const {
    return Quantity<RB_STB>(
        impl(yg.raw(), api.raw(), rs.raw(), t.raw(), tsep.raw()));
  }

  Quantity<RB_STB> validate_and_call(const Quantity<Sgg>& yg,
                                     const Quantity<Api>& api,
                                     const Quantity<SCF_STB>& rs,
                                     const Quantity<Fahrenheit>& t,
                                     const Quantity<Fahrenheit>& tsep) const {
    if (not(yg >= 0.6017))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.6017");
    if (not(yg <= 1.027))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.027");
    if (not(api >= 14.7))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 14.7");
    if (not(api <= 40))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 40");
    if (not(rs >= 133))
      ZENTHROW(OutOfParameterRange, "parameter rs is less than 133");
    if (not(rs <= 3050))
      ZENTHROW(OutOfParameterRange, "parameter rs is greater than 3050");

    return call(yg, api, rs, t, tsep);
  }

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<Fahrenheit>& tsep) const {
    return Quantity<RB_STB>(call(yg, api, rs, t, tsep));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& tsep = it.get_curr();

    return call(yg, api, rs, t, tsep);
  }
};

extern Quantity<RB_STB> BobDindorukChristman__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<Api>& api,
    const Quantity<SCF_STB>& rs, const Quantity<Fahrenheit>& t,
    const Quantity<Fahrenheit>& tsep);
extern double BobDindorukChristman__correlation__valid__fct(const double& yg,
                                                            const double& api,
                                                            const double& rs,
                                                            const double& t,
                                                            const double& tsep);

/** BobDoklaOsman

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group


The range assigned for Rs was taken from the range expressed by the authors for
Rsb.

References:

  - dokla:1992

@param[in] yg Gas specific gravity. Range in [0.789, 1.29]
@param[in] yo Oil specific gravity. Range in [0.8236, 0.886]
@param[in] rs Solution GOR. Range in [181, 2266]
@param[in] t Temperature. Range in [Quantity<Fahrenheit>(190),
Quantity<Fahrenheit>(275)]
@return inside [1.216, 2.493]
@ingroup Correlations
*/
class BobDoklaOsman : public SaturatedOilVolumeFactor {
  Correlation_Singleton(BobDoklaOsman);

  BobDoklaOsman()
      : SaturatedOilVolumeFactor("BobDoklaOsman", RB_STB::get_instance(), 1.216,
                                 2.493) {
    set_author("Dokla & Osman");
    set_title("DOKLA & OSMAN CORRELATION, CALCULATION OF SATURATED OIL "
              "FORMATION VOLUME FACTOR");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.789,
                  1.29);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity", 0.8236,
                  0.886);
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 181, 2266);
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("t", Rankine::get_instance(), "Temperature",
                  Quantity<Fahrenheit>(190), Quantity<Fahrenheit>(275));
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 51 bottomhole samples from UAE reservoirs.");
    add_ref("dokla:1992");
    add_par_synonym("yo", "api", "Api");
  }

public:
  static inline double impl(const double& yg, const double& yo,
                            const double& rs, const double& t) noexcept;

  Quantity<RB_STB> call(const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
                        const Quantity<SCF_STB>& rs,
                        const Quantity<Rankine>& t) const {
    return Quantity<RB_STB>(impl(yg.raw(), yo.raw(), rs.raw(), t.raw()));
  }

  Quantity<RB_STB> validate_and_call(const Quantity<Sgg>& yg,
                                     const Quantity<Sg_do>& yo,
                                     const Quantity<SCF_STB>& rs,
                                     const Quantity<Rankine>& t) const {
    if (not(yg >= 0.789))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.789");
    if (not(yg <= 1.29))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.29");
    if (not(yo >= 0.8236))
      ZENTHROW(OutOfParameterRange, "parameter yo is less than 0.8236");
    if (not(yo <= 0.886))
      ZENTHROW(OutOfParameterRange, "parameter yo is greater than 0.886");
    if (not(rs >= 181))
      ZENTHROW(OutOfParameterRange, "parameter rs is less than 181");
    if (not(rs <= 2266))
      ZENTHROW(OutOfParameterRange, "parameter rs is greater than 2266");

    return call(yg, yo, rs, t);
  }

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<Rankine>& t) const {
    return Quantity<RB_STB>(call(yg, yo, rs, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, yo, rs, t);
  }
};

extern Quantity<RB_STB> BobDoklaOsman__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
    const Quantity<SCF_STB>& rs, const Quantity<Rankine>& t);
extern double BobDoklaOsman__correlation__valid__fct(const double& yg,
                                                     const double& yo,
                                                     const double& rs,
                                                     const double& t);

/** BobGlaso

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group


References:

  - glaso:1980

@param[in] yg Gas specific gravity. Range in [0.65, 1.276]
@param[in] yo Oil specific gravity. Range in [Quantity<Api>(48.1),
Quantity<Api>(22.3)]
@param[in] rs Solution GOR. Range in [90, 2637]
@param[in] t Temperature. Range in [80, 280]
@return inside [1.087, 2.588]
@ingroup Correlations
*/
class BobGlaso : public SaturatedOilVolumeFactor {
  Correlation_Singleton(BobGlaso);

  BobGlaso()
      : SaturatedOilVolumeFactor("BobGlaso", RB_STB::get_instance(), 1.087,
                                 2.588) {
    set_author("Glaso");
    set_title("GLASO CORRELATION, CALCULATION OF SATURATED OIL FORMATION "
              "VOLUME FACTOR");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.65,
                  1.276);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(48.1), Quantity<Api>(22.3));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 90, 2637);
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 80, 280);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 26 samples from the North Sea (collected from wells in "
           "the region 56 to 62°N) and 19 samples from the Middle East, "
           "Algeria, and several areas in the U.S.");
    add_ref("glaso:1980");
    add_par_synonym("yo", "api", "Api");
  }

public:
  static inline double impl(const double& yg, const double& yo,
                            const double& rs, const double& t) noexcept;

  Quantity<RB_STB> call(const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
                        const Quantity<SCF_STB>& rs,
                        const Quantity<Fahrenheit>& t) const {
    return Quantity<RB_STB>(impl(yg.raw(), yo.raw(), rs.raw(), t.raw()));
  }

  Quantity<RB_STB> validate_and_call(const Quantity<Sgg>& yg,
                                     const Quantity<Sg_do>& yo,
                                     const Quantity<SCF_STB>& rs,
                                     const Quantity<Fahrenheit>& t) const {
    if (not(yg >= 0.65))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.65");
    if (not(yg <= 1.276))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.276");
    if (not(yo >= Quantity<Api>(48.1)))
      ZENTHROW(OutOfParameterRange,
               "parameter yo is less than Quantity<Api>(48.1)");
    if (not(yo <= Quantity<Api>(22.3)))
      ZENTHROW(OutOfParameterRange,
               "parameter yo is greater than Quantity<Api>(22.3)");
    if (not(rs >= 90))
      ZENTHROW(OutOfParameterRange, "parameter rs is less than 90");
    if (not(rs <= 2637))
      ZENTHROW(OutOfParameterRange, "parameter rs is greater than 2637");

    return call(yg, yo, rs, t);
  }

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<Fahrenheit>& t) const {
    return Quantity<RB_STB>(call(yg, yo, rs, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, yo, rs, t);
  }
};

extern Quantity<RB_STB> BobGlaso__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
    const Quantity<SCF_STB>& rs, const Quantity<Fahrenheit>& t);
extern double BobGlaso__correlation__valid__fct(const double& yg,
                                                const double& yo,
                                                const double& rs,
                                                const double& t);

/** BobHanafy

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group


References:

  - hanafy:1997

@param[in] rs Solution GOR. Range in [7, 4272]
@return inside [1.032, 4.35]
@ingroup Correlations
*/
class BobHanafy : public SaturatedOilVolumeFactor {
  Correlation_Singleton(BobHanafy);

  BobHanafy()
      : SaturatedOilVolumeFactor("BobHanafy", RB_STB::get_instance(), 1.032,
                                 4.35) {
    set_author("Hanafy et al.");
    set_title("HANAFY ET AL. CORRELATION, CALCULATION OF SATURATED OIL "
              "FORMATION VOLUME FACTOR");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 7, 4272);
    def_parameter_latex_symbol("rs", "R_s");
    add_db("Based on experimental PVT data of 324 fluid samples taken from 176 "
           "wells located in 75 fields. This data represents 15 productive "
           "zones of 123 reservoirs distributed along three different regions "
           "of Egypt, including the Gulf of Suez, Western Desert, and Sinai.");
    add_ref("hanafy:1997");
  }

public:
  static inline double impl(const double& rs) noexcept;

  Quantity<RB_STB> call(const Quantity<SCF_STB>& rs) const {
    return Quantity<RB_STB>(impl(rs.raw()));
  }

  Quantity<RB_STB> validate_and_call(const Quantity<SCF_STB>& rs) const {
    if (not(rs >= 7))
      ZENTHROW(OutOfParameterRange, "parameter rs is less than 7");
    if (not(rs <= 4272))
      ZENTHROW(OutOfParameterRange, "parameter rs is greater than 4272");

    return call(rs);
  }

  Quantity<RB_STB> operator()(const Quantity<SCF_STB>& rs) const {
    return Quantity<RB_STB>(call(rs));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& rs = it.get_curr();

    return call(rs);
  }
};

extern Quantity<RB_STB> BobHanafy__correlation__fct(
    const Quantity<SCF_STB>& rs);
extern double BobHanafy__correlation__valid__fct(const double& rs);

/** BobKartoatmodjo

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group


References:

  - kartoatmodjo:1991

@param[in] yg Gas specific gravity. Range in [0.4824, 1.668]
@param[in] yo Oil specific gravity. Range in [Quantity<Api>(59),
Quantity<Api>(14.4)]
@param[in] rs Solution GOR. Range in [0, 2890]
@param[in] t Temperature. Range in [75, 320]
@param[in] tsep Separator temperature. Range in [38, 294]
@param[in] psep Separator pressure. Range in [14.7, 1414.7]
@return inside [1.007, 2.144]
@ingroup Correlations
*/
class BobKartoatmodjo : public SaturatedOilVolumeFactor {
  Correlation_Singleton(BobKartoatmodjo);

  BobKartoatmodjo()
      : SaturatedOilVolumeFactor("BobKartoatmodjo", RB_STB::get_instance(),
                                 1.007, 2.144) {
    set_author("Kartoatmodjo & Schmidt");
    set_title("KARTOATMODJO & SCHMIDT CORRELATION, CALCULATION OF SATURATED "
              "OIL FORMATION VOLUME FACTOR");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.4824,
                  1.668);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(59), Quantity<Api>(14.4));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 0, 2890);
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 75, 320);
    def_parameter_latex_symbol("t", "T");
    add_parameter("tsep", Fahrenheit::get_instance(), "Separator temperature",
                  38, 294);
    def_parameter_latex_symbol("tsep", "T_{sp}");
    add_parameter("psep", psia::get_instance(), "Separator pressure", 14.7,
                  1414.7);
    def_parameter_latex_symbol("psep", "P_{sp}");
    add_db("Based on a set of 5392 data points, which represent 740 different "
           "crude oil samples.");
    add_db("The data bank was collected from PVT reports and literature. The "
           "first major source was from South East Asia, mainly Indonesia. The "
           "second source was North America, including the offshore area. The "
           "rest came from the Middle East and Latin America.");
    add_ref("kartoatmodjo:1991");
    add_par_synonym("yo", "api", "Api");
  }

public:
  static inline double impl(const double& yg, const double& yo,
                            const double& rs, const double& t,
                            const double& tsep, const double& psep) noexcept;

  Quantity<RB_STB> call(const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
                        const Quantity<SCF_STB>& rs,
                        const Quantity<Fahrenheit>& t,
                        const Quantity<Fahrenheit>& tsep,
                        const Quantity<psia>& psep) const {
    return Quantity<RB_STB>(
        impl(yg.raw(), yo.raw(), rs.raw(), t.raw(), tsep.raw(), psep.raw()));
  }

  Quantity<RB_STB> validate_and_call(const Quantity<Sgg>& yg,
                                     const Quantity<Sg_do>& yo,
                                     const Quantity<SCF_STB>& rs,
                                     const Quantity<Fahrenheit>& t,
                                     const Quantity<Fahrenheit>& tsep,
                                     const Quantity<psia>& psep) const {
    if (not(yg >= 0.4824))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.4824");
    if (not(yg <= 1.668))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.668");
    if (not(yo >= Quantity<Api>(59)))
      ZENTHROW(OutOfParameterRange,
               "parameter yo is less than Quantity<Api>(59)");
    if (not(yo <= Quantity<Api>(14.4)))
      ZENTHROW(OutOfParameterRange,
               "parameter yo is greater than Quantity<Api>(14.4)");
    if (not(rs >= 0))
      ZENTHROW(OutOfParameterRange, "parameter rs is less than 0");
    if (not(rs <= 2890))
      ZENTHROW(OutOfParameterRange, "parameter rs is greater than 2890");
    if (not(tsep >= 38))
      ZENTHROW(OutOfParameterRange, "parameter tsep is less than 38");
    if (not(tsep <= 294))
      ZENTHROW(OutOfParameterRange, "parameter tsep is greater than 294");
    if (not(psep >= 14.7))
      ZENTHROW(OutOfParameterRange, "parameter psep is less than 14.7");
    if (not(psep <= 1414.7))
      ZENTHROW(OutOfParameterRange, "parameter psep is greater than 1414.7");

    return call(yg, yo, rs, t, tsep, psep);
  }

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<Fahrenheit>& tsep,
                              const Quantity<psia>& psep) const {
    return Quantity<RB_STB>(call(yg, yo, rs, t, tsep, psep));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& tsep = it.get_curr();
    it.next();
    const auto& psep = it.get_curr();

    return call(yg, yo, rs, t, tsep, psep);
  }
};

extern Quantity<RB_STB> BobKartoatmodjo__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
    const Quantity<SCF_STB>& rs, const Quantity<Fahrenheit>& t,
    const Quantity<Fahrenheit>& tsep, const Quantity<psia>& psep);
extern double BobKartoatmodjo__correlation__valid__fct(
    const double& yg, const double& yo, const double& rs, const double& t,
    const double& tsep, const double& psep);

/** BobStanding

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group


The range assigned for Rs was taken from the range expressed by the author for
Rsb.

References:

  - standing:1947

  - alShammasi:2001

@param[in] yg Gas specific gravity. Range in [0.59, 0.95]
@param[in] yo Oil specific gravity. Range in [Quantity<Api>(63.8),
Quantity<Api>(16.5)]
@param[in] rs Solution GOR. Range in [20, 1425]
@param[in] t Temperature. Range in [100, 258]
@return inside [1.024, 2.15]
@ingroup Correlations
*/
class BobStanding : public SaturatedOilVolumeFactor {
  Correlation_Singleton(BobStanding);

  BobStanding()
      : SaturatedOilVolumeFactor("BobStanding", RB_STB::get_instance(), 1.024,
                                 2.15) {
    set_author("Standing");
    set_title("STANDING CORRELATION, CALCULATION OF SATURATED OIL FORMATION "
              "VOLUME FACTOR");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.59,
                  0.95);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(63.8), Quantity<Api>(16.5));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 20, 1425);
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 100, 258);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 105 experimentally determined bubble point pressures from "
           "22 different Californian crude-oil-natural-gas mixtures.");
    add_ref("standing:1947");
    add_ref("alShammasi:2001");
    add_par_synonym("yo", "api", "Api");
  }

public:
  static inline double impl(const double& yg, const double& yo,
                            const double& rs, const double& t) noexcept;

  Quantity<RB_STB> call(const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
                        const Quantity<SCF_STB>& rs,
                        const Quantity<Fahrenheit>& t) const {
    return Quantity<RB_STB>(impl(yg.raw(), yo.raw(), rs.raw(), t.raw()));
  }

  Quantity<RB_STB> validate_and_call(const Quantity<Sgg>& yg,
                                     const Quantity<Sg_do>& yo,
                                     const Quantity<SCF_STB>& rs,
                                     const Quantity<Fahrenheit>& t) const {
    if (not(yg >= 0.59))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.59");
    if (not(yg <= 0.95))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 0.95");
    if (not(yo >= Quantity<Api>(63.8)))
      ZENTHROW(OutOfParameterRange,
               "parameter yo is less than Quantity<Api>(63.8)");
    if (not(yo <= Quantity<Api>(16.5)))
      ZENTHROW(OutOfParameterRange,
               "parameter yo is greater than Quantity<Api>(16.5)");
    if (not(rs >= 20))
      ZENTHROW(OutOfParameterRange, "parameter rs is less than 20");
    if (not(rs <= 1425))
      ZENTHROW(OutOfParameterRange, "parameter rs is greater than 1425");

    return call(yg, yo, rs, t);
  }

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<Fahrenheit>& t) const {
    return Quantity<RB_STB>(call(yg, yo, rs, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, yo, rs, t);
  }
};

extern Quantity<RB_STB> BobStanding__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
    const Quantity<SCF_STB>& rs, const Quantity<Fahrenheit>& t);
extern double BobStanding__correlation__valid__fct(const double& yg,
                                                   const double& yo,
                                                   const double& rs,
                                                   const double& t);

/** BobLasater

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group

Lasater didn't present a correlation for Bo. Standing's correlation is used to
calculate Bob because of the similarity of his data bank and development ranges
with Lasater's.

The range assigned for Rs was taken from the range expressed by the author for
Rsb.

References:

  - lasater:1958

  - standing:1947

  - beggs:1987

@param[in] yg Gas specific gravity. Range in [0.574, 1.223]
@param[in] yo Oil specific gravity. Range in [Quantity<Api>(51.1),
Quantity<Api>(17.9)]
@param[in] rs Solution GOR. Range in [3, 2905]
@param[in] t Temperature. Range in [82, 272]
@return inside [, ]
@ingroup Correlations
*/
class BobLasater : public SaturatedOilVolumeFactor {
  Correlation_Singleton(BobLasater);

  BobLasater()
      : SaturatedOilVolumeFactor("BobLasater", RB_STB::get_instance()) {
    set_author("Lasater");
    set_title("LASATER CORRELATION (STANDING CORRELATION APPLIED FOR LASATER), "
              "CALCULATION OF SATURATED OIL FORMATION VOLUME FACTOR");
    set_hidden_calc();
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.574,
                  1.223);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(51.1), Quantity<Api>(17.9));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 3, 2905);
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 82, 272);
    def_parameter_latex_symbol("t", "T");
    add_db("Lasater correlation is based on data obtained from Canada, Western "
           "and Mid-Continental United States, and South America.");
    add_db("Standing correlation for formation volumes of bubble point liquids "
           "is based on data collected from 22 different Californian "
           "crude-oil-natural-gas mixtures (Western United States).");
    add_note("Lasater didn't present a correlation for Bo. Standing's "
             "correlation is used to calculate Bob because of the similarity "
             "of his data bank and development ranges with Lasater's.");
    add_ref("lasater:1958");
    add_ref("standing:1947");
    add_ref("beggs:1987");
    add_par_synonym("yo", "api", "Api");
  }

public:
  static inline double impl(const double& yg, const double& yo,
                            const double& rs, const double& t) noexcept;

  Quantity<RB_STB> call(const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
                        const Quantity<SCF_STB>& rs,
                        const Quantity<Fahrenheit>& t) const {
    return Quantity<RB_STB>(impl(yg.raw(), yo.raw(), rs.raw(), t.raw()));
  }

  Quantity<RB_STB> validate_and_call(const Quantity<Sgg>& yg,
                                     const Quantity<Sg_do>& yo,
                                     const Quantity<SCF_STB>& rs,
                                     const Quantity<Fahrenheit>& t) const {
    if (not(yg >= 0.574))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.574");
    if (not(yg <= 1.223))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.223");
    if (not(yo >= Quantity<Api>(51.1)))
      ZENTHROW(OutOfParameterRange,
               "parameter yo is less than Quantity<Api>(51.1)");
    if (not(yo <= Quantity<Api>(17.9)))
      ZENTHROW(OutOfParameterRange,
               "parameter yo is greater than Quantity<Api>(17.9)");
    if (not(rs >= 3))
      ZENTHROW(OutOfParameterRange, "parameter rs is less than 3");
    if (not(rs <= 2905))
      ZENTHROW(OutOfParameterRange, "parameter rs is greater than 2905");

    return call(yg, yo, rs, t);
  }

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<Fahrenheit>& t) const {
    return Quantity<RB_STB>(call(yg, yo, rs, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, yo, rs, t);
  }
};

extern Quantity<RB_STB> BobLasater__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
    const Quantity<SCF_STB>& rs, const Quantity<Fahrenheit>& t);
extern double BobLasater__correlation__valid__fct(const double& yg,
                                                  const double& yo,
                                                  const double& rs,
                                                  const double& t);

/** BobPetroskyFarshad

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group


References:

  - petrosky:1993

@param[in] yg Gas specific gravity. Range in [0.5781, 0.8519]
@param[in] yo Oil specific gravity. Range in [Quantity<Api>(45),
Quantity<Api>(16.3)]
@param[in] rs Solution GOR. Range in [217, 1406]
@param[in] t Temperature. Range in [114, 288]
@return inside [1.1178, 1.6229]
@ingroup Correlations
*/
class BobPetroskyFarshad : public SaturatedOilVolumeFactor {
  Correlation_Singleton(BobPetroskyFarshad);

  BobPetroskyFarshad()
      : SaturatedOilVolumeFactor("BobPetroskyFarshad", RB_STB::get_instance(),
                                 1.1178, 1.6229) {
    set_author("Petrosky & Farshad");
    set_title("PETROSKY & FARSHAD CORRELATION, CALCULATION OF SATURATED OIL "
              "FORMATION VOLUME FACTOR");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.5781,
                  0.8519);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(45), Quantity<Api>(16.3));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 217, 1406);
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 114, 288);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 81 laboratory PVT analyses. Fluid samples were obtained "
           "from reservoirs located offshore Texas and Louisiana.");
    add_db("Producing areas from Galveston Island, eastward, through Main Pass "
           "are represented.");
    add_db("Many Gulf of Mexico crudes can be characterized as naphthenic or "
           "biodegraded oils.");
    add_ref("petrosky:1993");
    add_par_synonym("yo", "api", "Api");
  }

public:
  static inline double impl(const double& yg, const double& yo,
                            const double& rs, const double& t) noexcept;

  Quantity<RB_STB> call(const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
                        const Quantity<SCF_STB>& rs,
                        const Quantity<Fahrenheit>& t) const {
    return Quantity<RB_STB>(impl(yg.raw(), yo.raw(), rs.raw(), t.raw()));
  }

  Quantity<RB_STB> validate_and_call(const Quantity<Sgg>& yg,
                                     const Quantity<Sg_do>& yo,
                                     const Quantity<SCF_STB>& rs,
                                     const Quantity<Fahrenheit>& t) const {
    if (not(yg >= 0.5781))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.5781");
    if (not(yg <= 0.8519))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 0.8519");
    if (not(yo >= Quantity<Api>(45)))
      ZENTHROW(OutOfParameterRange,
               "parameter yo is less than Quantity<Api>(45)");
    if (not(yo <= Quantity<Api>(16.3)))
      ZENTHROW(OutOfParameterRange,
               "parameter yo is greater than Quantity<Api>(16.3)");
    if (not(rs >= 217))
      ZENTHROW(OutOfParameterRange, "parameter rs is less than 217");
    if (not(rs <= 1406))
      ZENTHROW(OutOfParameterRange, "parameter rs is greater than 1406");

    return call(yg, yo, rs, t);
  }

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<Fahrenheit>& t) const {
    return Quantity<RB_STB>(call(yg, yo, rs, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, yo, rs, t);
  }
};

extern Quantity<RB_STB> BobPetroskyFarshad__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
    const Quantity<SCF_STB>& rs, const Quantity<Fahrenheit>& t);
extern double BobPetroskyFarshad__correlation__valid__fct(const double& yg,
                                                          const double& yo,
                                                          const double& rs,
                                                          const double& t);

/** BobTotalCFP

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group


References:

  - totalCFP:1983

  - banzer:1996

@param[in] yg Gas specific gravity. Range in [, ]
@param[in] api API oil gravity. Range in [, ]
@param[in] rs Solution GOR. Range in [, ]
@param[in] t Temperature. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class BobTotalCFP : public SaturatedOilVolumeFactor {
  Correlation_Singleton(BobTotalCFP);

  BobTotalCFP()
      : SaturatedOilVolumeFactor("BobTotalCFP", RB_STB::get_instance()) {
    set_author("Total CFP");
    set_title("TOTAL COMPAGNIE FRANÇAISE DES PÉTROLES (CFP) CORRELATION, "
              "CALCULATION OF SATURATED OIL FORMATION VOLUME FACTOR");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity");
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("api", Api::get_instance(), "API oil gravity");
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR");
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature");
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 336 PVT analyses of crude oil from eastern Venezuela.");
    add_ref("totalCFP:1983");
    add_ref("banzer:1996");
  }

public:
  static inline double impl(const double& yg, const double& api,
                            const double& rs, const double& t) noexcept;

  Quantity<RB_STB> call(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                        const Quantity<SCF_STB>& rs,
                        const Quantity<Fahrenheit>& t) const {
    return Quantity<RB_STB>(impl(yg.raw(), api.raw(), rs.raw(), t.raw()));
  }

  Quantity<RB_STB> validate_and_call(const Quantity<Sgg>& yg,
                                     const Quantity<Api>& api,
                                     const Quantity<SCF_STB>& rs,
                                     const Quantity<Fahrenheit>& t) const {

    return call(yg, api, rs, t);
  }

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<Fahrenheit>& t) const {
    return Quantity<RB_STB>(call(yg, api, rs, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, api, rs, t);
  }
};

extern Quantity<RB_STB> BobTotalCFP__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<Api>& api,
    const Quantity<SCF_STB>& rs, const Quantity<Fahrenheit>& t);
extern double BobTotalCFP__correlation__valid__fct(const double& yg,
                                                   const double& api,
                                                   const double& rs,
                                                   const double& t);

/** BobVasquezBeggs

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group


References:

  - vazquez:1980

  - alShammasi:2001

@param[in] yg Gas specific gravity. Range in [0.511, 1.35]
@param[in] api API oil gravity. Range in [15.3, 59.3]
@param[in] rs Solution GOR. Range in [0, 2199]
@param[in] t Temperature. Range in [75, 294]
@param[in] tsep Separator temperature. Range in [76, 150]
@param[in] psep Separator pressure. Range in [60, 565]
@return inside [1.028, 2.226]
@ingroup Correlations
*/
class BobVasquezBeggs : public SaturatedOilVolumeFactor {
  Correlation_Singleton(BobVasquezBeggs);

  BobVasquezBeggs()
      : SaturatedOilVolumeFactor("BobVasquezBeggs", RB_STB::get_instance(),
                                 1.028, 2.226) {
    set_author("Vazquez & Beggs");
    set_title("VAZQUEZ & BEGGS CORRELATION, CALCULATION OF SATURATED OIL "
              "FORMATION VOLUME FACTOR");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.511,
                  1.35);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("api", Api::get_instance(), "API oil gravity", 15.3, 59.3);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 0, 2199);
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 75, 294);
    def_parameter_latex_symbol("t", "T");
    add_parameter("tsep", Fahrenheit::get_instance(), "Separator temperature",
                  76, 150);
    def_parameter_latex_symbol("tsep", "T_{sp}");
    add_parameter("psep", psia::get_instance(), "Separator pressure", 60, 565);
    def_parameter_latex_symbol("psep", "P_{sp}");
    add_db("Based on more than 600 laboratory PVT analyses from fields all "
           "over the world.");
    add_ref("vazquez:1980");
    add_ref("alShammasi:2001");
  }

public:
  static inline double impl(const double& yg, const double& api,
                            const double& rs, const double& t,
                            const double& tsep, const double& psep) noexcept;

  Quantity<RB_STB> call(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                        const Quantity<SCF_STB>& rs,
                        const Quantity<Fahrenheit>& t,
                        const Quantity<Fahrenheit>& tsep,
                        const Quantity<psia>& psep) const {
    return Quantity<RB_STB>(
        impl(yg.raw(), api.raw(), rs.raw(), t.raw(), tsep.raw(), psep.raw()));
  }

  Quantity<RB_STB> validate_and_call(const Quantity<Sgg>& yg,
                                     const Quantity<Api>& api,
                                     const Quantity<SCF_STB>& rs,
                                     const Quantity<Fahrenheit>& t,
                                     const Quantity<Fahrenheit>& tsep,
                                     const Quantity<psia>& psep) const {
    if (not(yg >= 0.511))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.511");
    if (not(yg <= 1.35))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.35");
    if (not(api >= 15.3))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 15.3");
    if (not(api <= 59.3))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 59.3");
    if (not(rs >= 0))
      ZENTHROW(OutOfParameterRange, "parameter rs is less than 0");
    if (not(rs <= 2199))
      ZENTHROW(OutOfParameterRange, "parameter rs is greater than 2199");
    if (not(tsep >= 76))
      ZENTHROW(OutOfParameterRange, "parameter tsep is less than 76");
    if (not(tsep <= 150))
      ZENTHROW(OutOfParameterRange, "parameter tsep is greater than 150");
    if (not(psep >= 60))
      ZENTHROW(OutOfParameterRange, "parameter psep is less than 60");
    if (not(psep <= 565))
      ZENTHROW(OutOfParameterRange, "parameter psep is greater than 565");

    return call(yg, api, rs, t, tsep, psep);
  }

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<Fahrenheit>& tsep,
                              const Quantity<psia>& psep) const {
    return Quantity<RB_STB>(call(yg, api, rs, t, tsep, psep));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& tsep = it.get_curr();
    it.next();
    const auto& psep = it.get_curr();

    return call(yg, api, rs, t, tsep, psep);
  }
};

extern Quantity<RB_STB> BobVasquezBeggs__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<Api>& api,
    const Quantity<SCF_STB>& rs, const Quantity<Fahrenheit>& t,
    const Quantity<Fahrenheit>& tsep, const Quantity<psia>& psep);
extern double BobVasquezBeggs__correlation__valid__fct(
    const double& yg, const double& api, const double& rs, const double& t,
    const double& tsep, const double& psep);

/** BobPerezML

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group

Ezekwe, Querin & Humphrey's correlation is used to calculate the oil formation
volume factor at bubble point pressure.

The correlation was verified by using the original reference (Pérez, Heny &
Lago, 2001). Date: October 02 2017.

References:

  - perez:2001

@param[in] yg Gas specific gravity. Range in [0.571, 0.981]
@param[in] yo Oil specific gravity. Range in [Quantity<Api>(12.9),
Quantity<Api>(6.4)]
@param[in] rsb Solution GOR at Pb. Range in [38, 121]
@param[in] t Temperature. Range in [112, 300]
@param[in] p Pressure. Range in [, ]
@param[in] pb Bubble point pressure. Range in [405, 1335]
@return inside [1.032, 1.183]
@ingroup Correlations
*/
class BobPerezML : public SaturatedOilVolumeFactor {
  Correlation_Singleton(BobPerezML);

  BobPerezML()
      : SaturatedOilVolumeFactor("BobPerezML", RB_STB::get_instance(), 1.032,
                                 1.183) {
    set_author("Pérez, Heny & Lago");
    set_title("PÉREZ, HENY & LAGO CORRELATION, CALCULATION OF SATURATED OIL "
              "FORMATION VOLUME FACTOR");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.571,
                  0.981);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(12.9), Quantity<Api>(6.4));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 38,
                  121);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 112, 300);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("pb", psia::get_instance(), "Bubble point pressure", 405,
                  1335);
    def_parameter_latex_symbol("pb", "P_b");
    add_db("Based on 35 laboratory PVT analyses of extra-heavy oil samples "
           "from the Orinoco Oil Belt, specifically: 16 from Hamaca, 14 from "
           "Zuata, three from Cerro Negro, one from Melones, and one from "
           "Boscán.");
    add_note("Ezekwe, Querin & Humphrey's correlation is used to calculate the "
             "oil formation volume factor at bubble point pressure.");
    add_ref("perez:2001");
    add_par_synonym("yo", "api", "Api");
  }

  inline void precondition(const Quantity<psia>& p,
                           const Quantity<psia>& pb) const;

public:
  static inline double impl(const double& yg, const double& yo,
                            const double& rsb, const double& t, const double& p,
                            const double& pb) noexcept;

  Quantity<RB_STB> call(const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
                        const Quantity<SCF_STB>& rsb,
                        const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
                        const Quantity<psia>& pb) const {
    precondition(p, pb);
    return Quantity<RB_STB>(
        impl(yg.raw(), yo.raw(), rsb.raw(), t.raw(), p.raw(), pb.raw()));
  }

  Quantity<RB_STB> validate_and_call(const Quantity<Sgg>& yg,
                                     const Quantity<Sg_do>& yo,
                                     const Quantity<SCF_STB>& rsb,
                                     const Quantity<Fahrenheit>& t,
                                     const Quantity<psia>& p,
                                     const Quantity<psia>& pb) const {
    if (not(yg >= 0.571))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.571");
    if (not(yg <= 0.981))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 0.981");
    if (not(yo >= Quantity<Api>(12.9)))
      ZENTHROW(OutOfParameterRange,
               "parameter yo is less than Quantity<Api>(12.9)");
    if (not(yo <= Quantity<Api>(6.4)))
      ZENTHROW(OutOfParameterRange,
               "parameter yo is greater than Quantity<Api>(6.4)");
    if (not(rsb >= 38))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 38");
    if (not(rsb <= 121))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 121");
    if (not(pb >= 405))
      ZENTHROW(OutOfParameterRange, "parameter pb is less than 405");
    if (not(pb <= 1335))
      ZENTHROW(OutOfParameterRange, "parameter pb is greater than 1335");

    return call(yg, yo, rsb, t, p, pb);
  }

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<psia>& p,
                              const Quantity<psia>& pb) const {
    return Quantity<RB_STB>(call(yg, yo, rsb, t, p, pb));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();

    return call(yg, yo, rsb, t, p, pb);
  }
};

extern Quantity<RB_STB> BobPerezML__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
    const Quantity<SCF_STB>& rsb, const Quantity<Fahrenheit>& t,
    const Quantity<psia>& p, const Quantity<psia>& pb);
extern double BobPerezML__correlation__valid__fct(
    const double& yg, const double& yo, const double& rsb, const double& t,
    const double& p, const double& pb);

/** BobVelardeMcCain

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group


The solution gas-oil ratios in the database are total gas-oil ratios (obtained
by adding the solution gas-oil ratios at the separator and the stock tank).

References:

  - velarde:1997

@param[in] yg Gas specific gravity. Range in [0.561, 1.101]
@param[in] yo Oil specific gravity. Range in [Quantity<Api>(53.4),
Quantity<Api>(11.6)]
@param[in] rs Solution GOR. Range in [, ]
@param[in] rsb Solution GOR at Pb. Range in [102, 1808]
@param[in] t Temperature. Range in [70, 307]
@param[in] p Pressure. Range in [, ]
@return inside [1.04, 2.082]
@ingroup Correlations
*/
class BobVelardeMcCain : public SaturatedOilVolumeFactor {
  Correlation_Singleton(BobVelardeMcCain);

  BobVelardeMcCain()
      : SaturatedOilVolumeFactor("BobVelardeMcCain", RB_STB::get_instance(),
                                 1.04, 2.082) {
    set_author("Velarde, Blasingame & McCain");
    set_title("VELARDE, BLASINGAME & McCAIN CORRELATION, CALCULATION OF "
              "SATURATED OIL FORMATION VOLUME FACTOR");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.561,
                  1.101);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(53.4), Quantity<Api>(11.6));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR");
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 102,
                  1808);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 70, 307);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_db("Based on 195 PVT reports, including 2097 data sets.");
    add_ref("velarde:1997");
    add_par_synonym("yo", "api", "Api");
  }

public:
  static inline double impl(const double& yg, const double& yo,
                            const double& rs, const double& rsb,
                            const double& t, const double& p) noexcept;

  Quantity<RB_STB> call(const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
                        const Quantity<SCF_STB>& rs,
                        const Quantity<SCF_STB>& rsb,
                        const Quantity<Fahrenheit>& t,
                        const Quantity<psia>& p) const {
    return Quantity<RB_STB>(
        impl(yg.raw(), yo.raw(), rs.raw(), rsb.raw(), t.raw(), p.raw()));
  }

  Quantity<RB_STB> validate_and_call(const Quantity<Sgg>& yg,
                                     const Quantity<Sg_do>& yo,
                                     const Quantity<SCF_STB>& rs,
                                     const Quantity<SCF_STB>& rsb,
                                     const Quantity<Fahrenheit>& t,
                                     const Quantity<psia>& p) const {
    if (not(yg >= 0.561))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.561");
    if (not(yg <= 1.101))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.101");
    if (not(yo >= Quantity<Api>(53.4)))
      ZENTHROW(OutOfParameterRange,
               "parameter yo is less than Quantity<Api>(53.4)");
    if (not(yo <= Quantity<Api>(11.6)))
      ZENTHROW(OutOfParameterRange,
               "parameter yo is greater than Quantity<Api>(11.6)");
    if (not(rsb >= 102))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 102");
    if (not(rsb <= 1808))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 1808");

    return call(yg, yo, rs, rsb, t, p);
  }

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<psia>& p) const {
    return Quantity<RB_STB>(call(yg, yo, rs, rsb, t, p));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();

    return call(yg, yo, rs, rsb, t, p);
  }
};

extern Quantity<RB_STB> BobVelardeMcCain__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
    const Quantity<SCF_STB>& rs, const Quantity<SCF_STB>& rsb,
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p);
extern double BobVelardeMcCain__correlation__valid__fct(
    const double& yg, const double& yo, const double& rs, const double& rsb,
    const double& t, const double& p);

/** BobMillanArcia

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group


The correlation could not be verified because the original reference is not
available. Date: August 22 2016.

References:

  - millan:1984

  - perez:2001

@param[in] api API oil gravity. Range in [9, 20.2]
@param[in] rsb Solution GOR at Pb. Range in [, ]
@param[in] p Pressure. Range in [, ]
@param[in] pb Bubble point pressure. Range in [222.0, 3432.7]
@return inside [1.014, 1.248]
@ingroup Correlations
*/
class BobMillanArcia : public SaturatedOilVolumeFactor {
  Correlation_Singleton(BobMillanArcia);

  BobMillanArcia()
      : SaturatedOilVolumeFactor("BobMillanArcia", RB_STB::get_instance(),
                                 1.014, 1.248) {
    set_author("Millán-Arcia");
    set_title("MILLÁN-ARCIA CORRELATION, CALCULATION OF SATURATED OIL "
              "FORMATION VOLUME FACTOR");
    add_parameter("api", Api::get_instance(), "API oil gravity", 9, 20.2);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb");
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("pb", psia::get_instance(), "Bubble point pressure", 222.0,
                  3432.7);
    def_parameter_latex_symbol("pb", "P_b");
    add_db("Venezuelan heavy crudes were correlated.");
    add_ref("millan:1984");
    add_ref("perez:2001");
  }

  inline void precondition(const Quantity<psia>& p,
                           const Quantity<psia>& pb) const;

public:
  static inline double impl(const double& api, const double& rsb,
                            const double& p, const double& pb) noexcept;

  Quantity<RB_STB> call(const Quantity<Api>& api, const Quantity<SCF_STB>& rsb,
                        const Quantity<psia>& p,
                        const Quantity<psia>& pb) const {
    precondition(p, pb);
    return Quantity<RB_STB>(impl(api.raw(), rsb.raw(), p.raw(), pb.raw()));
  }

  Quantity<RB_STB> validate_and_call(const Quantity<Api>& api,
                                     const Quantity<SCF_STB>& rsb,
                                     const Quantity<psia>& p,
                                     const Quantity<psia>& pb) const {
    if (not(api >= 9))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 9");
    if (not(api <= 20.2))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 20.2");
    if (not(pb >= 222.0))
      ZENTHROW(OutOfParameterRange, "parameter pb is less than 222.0");
    if (not(pb <= 3432.7))
      ZENTHROW(OutOfParameterRange, "parameter pb is greater than 3432.7");

    return call(api, rsb, p, pb);
  }

  Quantity<RB_STB> operator()(const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<psia>& p,
                              const Quantity<psia>& pb) const {
    return Quantity<RB_STB>(call(api, rsb, p, pb));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& api = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();

    return call(api, rsb, p, pb);
  }
};

extern Quantity<RB_STB> BobMillanArcia__correlation__fct(
    const Quantity<Api>& api, const Quantity<SCF_STB>& rsb,
    const Quantity<psia>& p, const Quantity<psia>& pb);
extern double BobMillanArcia__correlation__valid__fct(const double& api,
                                                      const double& rsb,
                                                      const double& p,
                                                      const double& pb);

/** BobManucciRosales

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group


The correlation could not be verified because the original reference is not
available. Date: August 22 2016.

References:

  - manucci:1968

  - perez:2001

@param[in] api API oil gravity. Range in [14, 40]
@param[in] rs Solution GOR. Range in [180, 2500]
@param[in] t Temperature. Range in [165, 304]
@return inside [1.05, 2.4]
@ingroup Correlations
*/
class BobManucciRosales : public SaturatedOilVolumeFactor {
  Correlation_Singleton(BobManucciRosales);

  BobManucciRosales()
      : SaturatedOilVolumeFactor("BobManucciRosales", RB_STB::get_instance(),
                                 1.05, 2.4) {
    set_author("Manucci & Rosales");
    set_title("MANUCCI & ROSALES CORRELATION, CALCULATION OF SATURATED OIL "
              "FORMATION VOLUME FACTOR");
    add_parameter("api", Api::get_instance(), "API oil gravity", 14, 40);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 180, 2500);
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 165, 304);
    def_parameter_latex_symbol("t", "T");
    add_db("Crude oils of eastern Venezuela were correlated, by using "
           "differential separation data from laboratory PVT analysis.");
    add_ref("manucci:1968");
    add_ref("perez:2001");
  }

public:
  static inline double impl(const double& api, const double& rs,
                            const double& t) noexcept;

  Quantity<RB_STB> call(const Quantity<Api>& api, const Quantity<SCF_STB>& rs,
                        const Quantity<Fahrenheit>& t) const {
    return Quantity<RB_STB>(impl(api.raw(), rs.raw(), t.raw()));
  }

  Quantity<RB_STB> validate_and_call(const Quantity<Api>& api,
                                     const Quantity<SCF_STB>& rs,
                                     const Quantity<Fahrenheit>& t) const {
    if (not(api >= 14))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 14");
    if (not(api <= 40))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 40");
    if (not(rs >= 180))
      ZENTHROW(OutOfParameterRange, "parameter rs is less than 180");
    if (not(rs <= 2500))
      ZENTHROW(OutOfParameterRange, "parameter rs is greater than 2500");

    return call(api, rs, t);
  }

  Quantity<RB_STB> operator()(const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<Fahrenheit>& t) const {
    return Quantity<RB_STB>(call(api, rs, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& api = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(api, rs, t);
  }
};

extern Quantity<RB_STB> BobManucciRosales__correlation__fct(
    const Quantity<Api>& api, const Quantity<SCF_STB>& rs,
    const Quantity<Fahrenheit>& t);
extern double BobManucciRosales__correlation__valid__fct(const double& api,
                                                         const double& rs,
                                                         const double& t);

/** BobLopezCR

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group


The correlation was verified by using the original reference. Date: April 27
2017.

References:

  - lopez:2014

@param[in] yg Gas specific gravity. Range in [0.5086, 0.75]
@param[in] api API oil gravity. Range in [7.4, 14.5]
@param[in] rs Solution GOR. Range in [12, 110]
@param[in] t Temperature. Range in [124, 220]
@return inside [1.0298, 1.1659]
@ingroup Correlations
*/
class BobLopezCR : public SaturatedOilVolumeFactor {
  Correlation_Singleton(BobLopezCR);

  BobLopezCR()
      : SaturatedOilVolumeFactor("BobLopezCR", RB_STB::get_instance(), 1.0298,
                                 1.1659) {
    set_author("López, Castillo & Rojas");
    set_title("LÓPEZ, CASTILLO & ROJAS CORRELATION, CALCULATION OF SATURATED "
              "OIL FORMATION VOLUME FACTOR");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.5086,
                  0.75);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("api", Api::get_instance(), "API oil gravity", 7.4, 14.5);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 12, 110);
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 124, 220);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 48 PVT tests from saturated oil reservoirs located in "
           "Bare and Arecuna fields, from the Ayacucho Area of the Faja "
           "Petrolífera del Orinoco.");
    add_ref("lopez:2014");
  }

public:
  static inline double impl(const double& yg, const double& api,
                            const double& rs, const double& t) noexcept;

  Quantity<RB_STB> call(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                        const Quantity<SCF_STB>& rs,
                        const Quantity<Fahrenheit>& t) const {
    return Quantity<RB_STB>(impl(yg.raw(), api.raw(), rs.raw(), t.raw()));
  }

  Quantity<RB_STB> validate_and_call(const Quantity<Sgg>& yg,
                                     const Quantity<Api>& api,
                                     const Quantity<SCF_STB>& rs,
                                     const Quantity<Fahrenheit>& t) const {
    if (not(yg >= 0.5086))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.5086");
    if (not(yg <= 0.75))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 0.75");
    if (not(api >= 7.4))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 7.4");
    if (not(api <= 14.5))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 14.5");
    if (not(rs >= 12))
      ZENTHROW(OutOfParameterRange, "parameter rs is less than 12");
    if (not(rs <= 110))
      ZENTHROW(OutOfParameterRange, "parameter rs is greater than 110");

    return call(yg, api, rs, t);
  }

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<Fahrenheit>& t) const {
    return Quantity<RB_STB>(call(yg, api, rs, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, api, rs, t);
  }
};

extern Quantity<RB_STB> BobLopezCR__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<Api>& api,
    const Quantity<SCF_STB>& rs, const Quantity<Fahrenheit>& t);
extern double BobLopezCR__correlation__valid__fct(const double& yg,
                                                  const double& api,
                                                  const double& rs,
                                                  const double& t);

/** BobSalazar

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group

Salazar (2008) presented the correlation as model 1.

The correlation was verified by using the original reference. Date: April 28
2017.

References:

  - salazar:2008

@param[in] yg Gas specific gravity. Range in [0.584, 1.142]
@param[in] api API oil gravity. Range in [7.1, 18.9]
@param[in] p Pressure. Range in [, ]
@return inside [1.02, 1.1]
@ingroup Correlations
*/
class BobSalazar : public SaturatedOilVolumeFactor {
  Correlation_Singleton(BobSalazar);

  BobSalazar()
      : SaturatedOilVolumeFactor("BobSalazar", RB_STB::get_instance(), 1.02,
                                 1.1) {
    set_author("Salazar");
    set_title("SALAZAR CORRELATION, CALCULATION OF SATURATED OIL FORMATION "
              "VOLUME FACTOR");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.584,
                  1.142);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("api", Api::get_instance(), "API oil gravity", 7.1, 18.9);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_db("Based on 136 PVT reports from different areas of the Faja "
           "Petrolífera del Orinoco: 60 from Ayacucho, 48 from Junín, 26 from "
           "Carabobo, and 2 from Boyacá.");
    add_note("Salazar (2008) presented the correlation as model 1.");
    add_ref("salazar:2008");
  }

public:
  static inline double impl(const double& yg, const double& api,
                            const double& p) noexcept;

  Quantity<RB_STB> call(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                        const Quantity<psia>& p) const {
    return Quantity<RB_STB>(impl(yg.raw(), api.raw(), p.raw()));
  }

  Quantity<RB_STB> validate_and_call(const Quantity<Sgg>& yg,
                                     const Quantity<Api>& api,
                                     const Quantity<psia>& p) const {
    if (not(yg >= 0.584))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.584");
    if (not(yg <= 1.142))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.142");
    if (not(api >= 7.1))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 7.1");
    if (not(api <= 18.9))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 18.9");

    return call(yg, api, p);
  }

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                              const Quantity<psia>& p) const {
    return Quantity<RB_STB>(call(yg, api, p));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& p = it.get_curr();

    return call(yg, api, p);
  }
};

extern Quantity<RB_STB> BobSalazar__correlation__fct(const Quantity<Sgg>& yg,
                                                     const Quantity<Api>& api,
                                                     const Quantity<psia>& p);
extern double BobSalazar__correlation__valid__fct(const double& yg,
                                                  const double& api,
                                                  const double& p);

#endif // SATURATED_OIL_FORMATION_VOLUME_FACTOR_H
