#ifndef BUBBLE_POINT_PRESSURE_H
#define BUBBLE_POINT_PRESSURE_H

#include <pvt-exceptions.H>
#include <correlations/correlation.H>

/** PbAlMarhoun

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group


References:

  - alMarhoun:1988

@param[in] yg Gas specific gravity. Range in [0.752, 1.367]
@param[in] yo Oil specific gravity. Range in [Quantity<Api>(44.6),
Quantity<Api>(19.4)]
@param[in] rsb Solution GOR at Pb. Range in [26, 1602]
@param[in] t Temperature. Range in [Quantity<Fahrenheit>(74),
Quantity<Fahrenheit>(240)]
@return inside [130, 3573]
@ingroup Correlations
*/
class PbAlMarhoun : public BubblePointPressure {
  Correlation_Singleton(PbAlMarhoun);

  PbAlMarhoun()
      : BubblePointPressure("PbAlMarhoun", psia::get_instance(), 130, 3573) {
    set_author("Al-Marhoun");
    set_title("AL-MARHOUN CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.752,
                  1.367);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(44.6), Quantity<Api>(19.4));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 26,
                  1602);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("t", Rankine::get_instance(), "Temperature",
                  Quantity<Fahrenheit>(74), Quantity<Fahrenheit>(240));
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 69 bottomhole fluid samples from 69 Middle East oil "
           "reservoirs.");
    add_ref("alMarhoun:1988");
    add_par_synonym("yo", "api", "api");
  }

public:
  static inline double impl(const double& yg, const double& yo,
                            const double& rsb, const double& t) noexcept;

  Quantity<psia> call(const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
                      const Quantity<SCF_STB>& rsb,
                      const Quantity<Rankine>& t) const {
    return Quantity<psia>(impl(yg.raw(), yo.raw(), rsb.raw(), t.raw()));
  }

  Quantity<psia> validate_and_call(const Quantity<Sgg>& yg,
                                   const Quantity<Sg_do>& yo,
                                   const Quantity<SCF_STB>& rsb,
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
    if (not(rsb >= 26))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 26");
    if (not(rsb <= 1602))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 1602");

    return call(yg, yo, rsb, t);
  }

  Quantity<psia> operator()(const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Rankine>& t) const {
    return Quantity<psia>(call(yg, yo, rsb, t));
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

    return call(yg, yo, rsb, t);
  }
};

extern Quantity<psia> PbAlMarhoun__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
    const Quantity<SCF_STB>& rsb, const Quantity<Rankine>& t);
extern double PbAlMarhoun__correlation__valid__fct(const double& yg,
                                                   const double& yo,
                                                   const double& rsb,
                                                   const double& t);

/** PbAlShammasi

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group


References:

  - alShammasi:1999

@param[in] yg Gas specific gravity. Range in [0.51, 3.44]
@param[in] yo Oil specific gravity. Range in [Quantity<Api>(63.7),
Quantity<Api>(6)]
@param[in] rsb Solution GOR at Pb. Range in [6, 3298.6]
@param[in] t Temperature. Range in [74, 341.6]
@return inside [31.7, 7127]
@ingroup Correlations
*/
class PbAlShammasi : public BubblePointPressure {
  Correlation_Singleton(PbAlShammasi);

  PbAlShammasi()
      : BubblePointPressure("PbAlShammasi", psia::get_instance(), 31.7, 7127) {
    set_author("Al-Shammasi");
    set_title("AL-SHAMMASI CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.51,
                  3.44);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(63.7), Quantity<Api>(6));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 6,
                  3298.6);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 74, 341.6);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on a total of 1709 data sets (1661 data sets from 13 "
           "different published literature papers and 48 data sets originated "
           "from Kuwait reservoirs from unpublished sources).");
    add_ref("alShammasi:1999");
    add_par_synonym("yo", "api", "api");
  }

public:
  static inline double impl(const double& yg, const double& yo,
                            const double& rsb, const double& t) noexcept;

  Quantity<psia> call(const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
                      const Quantity<SCF_STB>& rsb,
                      const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(impl(yg.raw(), yo.raw(), rsb.raw(), t.raw()));
  }

  Quantity<psia> validate_and_call(const Quantity<Sgg>& yg,
                                   const Quantity<Sg_do>& yo,
                                   const Quantity<SCF_STB>& rsb,
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
    if (not(rsb >= 6))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 6");
    if (not(rsb <= 3298.6))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 3298.6");

    return call(yg, yo, rsb, t);
  }

  Quantity<psia> operator()(const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(call(yg, yo, rsb, t));
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

    return call(yg, yo, rsb, t);
  }
};

extern Quantity<psia> PbAlShammasi__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
    const Quantity<SCF_STB>& rsb, const Quantity<Fahrenheit>& t);
extern double PbAlShammasi__correlation__valid__fct(const double& yg,
                                                    const double& yo,
                                                    const double& rsb,
                                                    const double& t);

/** PbDeGhettoDetachedFromRs

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group


De Ghetto, Paone & Villa use Yg as the average specific gravity of total surface
gases, while the original correlation (Standing) uses it as the gravity of
dissolved gas. Since Standing doesn't specify if Yg is the gravity of total
surface gases or of the separator, it is assumed that the data is from a one
stage separation with average gas gravity.

It was decided to eliminate this correlation.

References:

  - deGhetto:1995

  - standing:1947

@param[in] yg Gas specific gravity. Range in [0.623, 1.517]
@param[in] rsb Solution GOR at Pb. Range in [17.21, 640.25]
@param[in] api API oil gravity. Range in [6, 22.3]
@param[in] t Temperature. Range in [131.4, 250.7]
@return inside [208.86, 4021.96]
@ingroup Correlations
*/
class PbDeGhettoDetachedFromRs : public BubblePointPressure {
  Correlation_Singleton(PbDeGhettoDetachedFromRs);

  PbDeGhettoDetachedFromRs()
      : BubblePointPressure("PbDeGhettoDetachedFromRs", psia::get_instance(),
                            208.86, 4021.96) {
    set_author("De Ghetto, Paone & Villa");
    set_title("DE GHETTO, PAONE & VILLA CORRELATION FOR EXTRA-HEAVY AND HEAVY "
              "OILS (MODIFIED STANDING CORRELATION), CALCULATION OF BUBBLE "
              "POINT PRESSURE");
    set_hidden();
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.623,
                  1.517);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 17.21,
                  640.25);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("api", Api::get_instance(), "API oil gravity", 6, 22.3);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 131.4, 250.7);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 1200 measured data points of 63 heavy and extra-heavy oil "
           "samples obtained from the Mediterranean Basin, Africa and the "
           "Persian Gulf.");
    add_db("Oil samples have been divided in two different API gravity "
           "classes: extra-heavy oils for °API<=10, heavy oils for "
           "10<°API<=22.3.");
    add_ref("deGhetto:1995");
    add_ref("standing:1947");
  }

public:
  static inline double impl(const double& yg, const double& rsb,
                            const double& api, const double& t) noexcept;

  Quantity<psia> call(const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
                      const Quantity<Api>& api,
                      const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(impl(yg.raw(), rsb.raw(), api.raw(), t.raw()));
  }

  Quantity<psia> validate_and_call(const Quantity<Sgg>& yg,
                                   const Quantity<SCF_STB>& rsb,
                                   const Quantity<Api>& api,
                                   const Quantity<Fahrenheit>& t) const {
    if (not(yg >= 0.623))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.623");
    if (not(yg <= 1.517))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.517");
    if (not(rsb >= 17.21))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 17.21");
    if (not(rsb <= 640.25))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 640.25");
    if (not(api >= 6))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 6");
    if (not(api <= 22.3))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 22.3");

    return call(yg, rsb, api, t);
  }

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(call(yg, rsb, api, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, rsb, api, t);
  }
};

extern Quantity<psia> PbDeGhettoDetachedFromRs__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t);
extern double PbDeGhettoDetachedFromRs__correlation__valid__fct(
    const double& yg, const double& rsb, const double& api, const double& t);

/** PbDeGhettoEtAl

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group

Solution gas oil ratio (Rs) correlations are reversed to solve for the bubble
point pressure (a modified Vazquez & Beggs correlation for heavy oils and a
modified Standing correlation for extra-heavy oils).

De Ghetto, Paone & Villa use Yg as the average specific gravity of total surface
gases, while the original correlation (Standing) uses it as the gravity of
dissolved gas. Since Standing doesn't specify if Yg is the gravity of total
surface gases or of the separator, it is assumed that the data is from a one
stage separation with average gas gravity.

References:

  - deGhetto:1995

  - standing:1947

  - fekete:2012

@param[in] yg Gas specific gravity. Range in [0.623, 1.517]
@param[in] rsb Solution GOR at Pb. Range in [17.21, 640.25]
@param[in] api API oil gravity. Range in [6, 22.3]
@param[in] t Temperature. Range in [131.4, 250.7]
@param[in] tsep Separator temperature. Range in [59, 177.8]
@param[in] psep Separator pressure. Range in [14.5, 752.2]
@return inside [208.86, 4021.96]
@ingroup Correlations
*/
class PbDeGhettoEtAl : public BubblePointPressure {
  Correlation_Singleton(PbDeGhettoEtAl);

  PbDeGhettoEtAl()
      : BubblePointPressure("PbDeGhettoEtAl", psia::get_instance(), 208.86,
                            4021.96) {
    set_author("De Ghetto, Paone & Villa");
    set_title("DE GHETTO, PAONE & VILLA CORRELATION FOR EXTRA-HEAVY (MODIFIED "
              "STANDING CORRELATION) AND HEAVY (MODIFIED VAZQUEZ & BEGGS "
              "CORRELATION) OILS, CALCULATION OF BUBBLE POINT PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.623,
                  1.517);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 17.21,
                  640.25);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("api", Api::get_instance(), "API oil gravity", 6, 22.3);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
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
    add_note("Solution gas oil ratio (Rs) correlations are reversed to solve "
             "for the bubble point pressure (a modified Vazquez & Beggs "
             "correlation for heavy oils and a modified Standing correlation "
             "for extra-heavy oils).");
    add_ref("deGhetto:1995");
    add_ref("standing:1947");
    add_ref("fekete:2012");
  }

public:
  static inline double impl(const double& yg, const double& rsb,
                            const double& api, const double& t,
                            const double& tsep, const double& psep) noexcept;

  Quantity<psia> call(const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
                      const Quantity<Api>& api, const Quantity<Fahrenheit>& t,
                      const Quantity<Fahrenheit>& tsep,
                      const Quantity<psia>& psep) const {
    return Quantity<psia>(
        impl(yg.raw(), rsb.raw(), api.raw(), t.raw(), tsep.raw(), psep.raw()));
  }

  Quantity<psia> validate_and_call(const Quantity<Sgg>& yg,
                                   const Quantity<SCF_STB>& rsb,
                                   const Quantity<Api>& api,
                                   const Quantity<Fahrenheit>& t,
                                   const Quantity<Fahrenheit>& tsep,
                                   const Quantity<psia>& psep) const {
    if (not(yg >= 0.623))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.623");
    if (not(yg <= 1.517))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.517");
    if (not(rsb >= 17.21))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 17.21");
    if (not(rsb <= 640.25))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 640.25");
    if (not(api >= 6))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 6");
    if (not(api <= 22.3))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 22.3");
    if (not(tsep >= 59))
      ZENTHROW(OutOfParameterRange, "parameter tsep is less than 59");
    if (not(tsep <= 177.8))
      ZENTHROW(OutOfParameterRange, "parameter tsep is greater than 177.8");
    if (not(psep >= 14.5))
      ZENTHROW(OutOfParameterRange, "parameter psep is less than 14.5");
    if (not(psep <= 752.2))
      ZENTHROW(OutOfParameterRange, "parameter psep is greater than 752.2");

    return call(yg, rsb, api, t, tsep, psep);
  }

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t,
                            const Quantity<Fahrenheit>& tsep,
                            const Quantity<psia>& psep) const {
    return Quantity<psia>(call(yg, rsb, api, t, tsep, psep));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& tsep = it.get_curr();
    it.next();
    const auto& psep = it.get_curr();

    return call(yg, rsb, api, t, tsep, psep);
  }
};

extern Quantity<psia> PbDeGhettoEtAl__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t,
    const Quantity<Fahrenheit>& tsep, const Quantity<psia>& psep);
extern double PbDeGhettoEtAl__correlation__valid__fct(
    const double& yg, const double& rsb, const double& api, const double& t,
    const double& tsep, const double& psep);

/** PbDindorukChristmanDetachedFromRs

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group


It was decided to eliminate this correlation.

References:

  - dindoruk:2004

@param[in] yg Gas specific gravity. Range in [0.623, 1.517]
@param[in] rsb Solution GOR at Pb. Range in [133, 3050]
@param[in] api API oil gravity. Range in [14.7, 40]
@param[in] t Temperature. Range in [117, 276]
@return inside [926, 12230]
@ingroup Correlations
*/
class PbDindorukChristmanDetachedFromRs : public BubblePointPressure {
  Correlation_Singleton(PbDindorukChristmanDetachedFromRs);

  PbDindorukChristmanDetachedFromRs()
      : BubblePointPressure("PbDindorukChristmanDetachedFromRs",
                            psia::get_instance(), 926, 12230) {
    set_author("Dindoruk & Christman");
    set_title("DINDORUK & CHRISTMAN CORRELATION, CALCULATION OF BUBBLE POINT "
              "PRESSURE");
    set_hidden();
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.623,
                  1.517);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 133,
                  3050);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("api", Api::get_instance(), "API oil gravity", 14.7, 40);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 117, 276);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on more than 100 PVT reports from the Gulf of Mexico.");
    add_ref("dindoruk:2004");
  }

public:
  static inline double impl(const double& yg, const double& rsb,
                            const double& api, const double& t) noexcept;

  Quantity<psia> call(const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
                      const Quantity<Api>& api,
                      const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(impl(yg.raw(), rsb.raw(), api.raw(), t.raw()));
  }

  Quantity<psia> validate_and_call(const Quantity<Sgg>& yg,
                                   const Quantity<SCF_STB>& rsb,
                                   const Quantity<Api>& api,
                                   const Quantity<Fahrenheit>& t) const {
    if (not(yg >= 0.623))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.623");
    if (not(yg <= 1.517))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.517");
    if (not(rsb >= 133))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 133");
    if (not(rsb <= 3050))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 3050");
    if (not(api >= 14.7))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 14.7");
    if (not(api <= 40))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 40");

    return call(yg, rsb, api, t);
  }

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(call(yg, rsb, api, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, rsb, api, t);
  }
};

extern Quantity<psia> PbDindorukChristmanDetachedFromRs__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t);
extern double PbDindorukChristmanDetachedFromRs__correlation__valid__fct(
    const double& yg, const double& rsb, const double& api, const double& t);

/** PbDindorukChristman

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group

Dindoruk & Christman presented separate equations for solution gas oil ratio
(Rs) and bubble point pressure (Pb). Both equations provide nearly identical
results for low GOR systems. For higher GOR systems (greater than 2,000
scf/STB), their Rs equation provides more realistic results; therefore, the
correlation for Rs is used to calculate Pb through the Newton-Raphson numerical
root-finding method.

References:

  - dindoruk:2004

  - petroWiki:2016:7

@param[in] yg Gas specific gravity. Range in [0.623, 1.517]
@param[in] rsb Solution GOR at Pb. Range in [133, 3050]
@param[in] api API oil gravity. Range in [14.7, 40]
@param[in] t Temperature. Range in [117, 276]
@return inside [926, 12230]
@ingroup Correlations
*/
class PbDindorukChristman : public BubblePointPressure {
  Correlation_Singleton(PbDindorukChristman);

  PbDindorukChristman()
      : BubblePointPressure("PbDindorukChristman", psia::get_instance(), 926,
                            12230) {
    set_author("Dindoruk & Christman");
    set_title("DINDORUK & CHRISTMAN CORRELATION, CALCULATION OF BUBBLE POINT "
              "PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.623,
                  1.517);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 133,
                  3050);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("api", Api::get_instance(), "API oil gravity", 14.7, 40);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 117, 276);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on more than 100 PVT reports from the Gulf of Mexico.");
    add_note("Dindoruk & Christman presented separate equations for solution "
             "gas oil ratio (Rs) and bubble point pressure (Pb). Both "
             "equations provide nearly identical results for low GOR systems. "
             "For higher GOR systems (greater than 2,000 scf/STB), their Rs "
             "equation provides more realistic results; therefore, the "
             "correlation for Rs is used to calculate Pb through the "
             "Newton-Raphson numerical root-finding method.");
    add_ref("dindoruk:2004");
    add_ref("petroWiki:2016:7");
  }

public:
  static inline double impl(const double& yg, const double& rsb,
                            const double& api, const double& t) noexcept;

  Quantity<psia> call(const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
                      const Quantity<Api>& api,
                      const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(impl(yg.raw(), rsb.raw(), api.raw(), t.raw()));
  }

  Quantity<psia> validate_and_call(const Quantity<Sgg>& yg,
                                   const Quantity<SCF_STB>& rsb,
                                   const Quantity<Api>& api,
                                   const Quantity<Fahrenheit>& t) const {
    if (not(yg >= 0.623))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.623");
    if (not(yg <= 1.517))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.517");
    if (not(rsb >= 133))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 133");
    if (not(rsb <= 3050))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 3050");
    if (not(api >= 14.7))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 14.7");
    if (not(api <= 40))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 40");

    return call(yg, rsb, api, t);
  }

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(call(yg, rsb, api, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, rsb, api, t);
  }
};

extern Quantity<psia> PbDindorukChristman__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t);
extern double PbDindorukChristman__correlation__valid__fct(const double& yg,
                                                           const double& rsb,
                                                           const double& api,
                                                           const double& t);

/** PbDoklaOsman

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group


References:

  - dokla:1992

@param[in] yg Gas specific gravity. Range in [0.798, 1.29]
@param[in] rsb Solution GOR at Pb. Range in [181, 2266]
@param[in] yo Oil specific gravity. Range in [0.8236, 0.886]
@param[in] t Temperature. Range in [Quantity<Fahrenheit>(190),
Quantity<Fahrenheit>(275)]
@return inside [590, 4640]
@ingroup Correlations
*/
class PbDoklaOsman : public BubblePointPressure {
  Correlation_Singleton(PbDoklaOsman);

  PbDoklaOsman()
      : BubblePointPressure("PbDoklaOsman", psia::get_instance(), 590, 4640) {
    set_author("Dokla & Osman");
    set_title(
        "DOKLA & OSMAN CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.798,
                  1.29);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 181,
                  2266);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity", 0.8236,
                  0.886);
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("t", Rankine::get_instance(), "Temperature",
                  Quantity<Fahrenheit>(190), Quantity<Fahrenheit>(275));
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 51 bottomhole samples from UAE reservoirs.");
    add_ref("dokla:1992");
    add_par_synonym("yo", "api", "api");
  }

public:
  static inline double impl(const double& yg, const double& rsb,
                            const double& yo, const double& t) noexcept;

  Quantity<psia> call(const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
                      const Quantity<Sg_do>& yo,
                      const Quantity<Rankine>& t) const {
    return Quantity<psia>(impl(yg.raw(), rsb.raw(), yo.raw(), t.raw()));
  }

  Quantity<psia> validate_and_call(const Quantity<Sgg>& yg,
                                   const Quantity<SCF_STB>& rsb,
                                   const Quantity<Sg_do>& yo,
                                   const Quantity<Rankine>& t) const {
    if (not(yg >= 0.798))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.798");
    if (not(yg <= 1.29))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.29");
    if (not(rsb >= 181))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 181");
    if (not(rsb <= 2266))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 2266");
    if (not(yo >= 0.8236))
      ZENTHROW(OutOfParameterRange, "parameter yo is less than 0.8236");
    if (not(yo <= 0.886))
      ZENTHROW(OutOfParameterRange, "parameter yo is greater than 0.886");

    return call(yg, rsb, yo, t);
  }

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Sg_do>& yo,
                            const Quantity<Rankine>& t) const {
    return Quantity<psia>(call(yg, rsb, yo, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, rsb, yo, t);
  }
};

extern Quantity<psia> PbDoklaOsman__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Sg_do>& yo, const Quantity<Rankine>& t);
extern double PbDoklaOsman__correlation__valid__fct(const double& yg,
                                                    const double& rsb,
                                                    const double& yo,
                                                    const double& t);

/** PbHanafyDetachedFromRs

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group


References:

  - hanafy:1997

@param[in] rsb Solution GOR at Pb. Range in [7, 4272]
@return inside [36, 5003]
@ingroup Correlations
*/
class PbHanafyDetachedFromRs : public BubblePointPressure {
  Correlation_Singleton(PbHanafyDetachedFromRs);

  PbHanafyDetachedFromRs()
      : BubblePointPressure("PbHanafyDetachedFromRs", psia::get_instance(), 36,
                            5003) {
    set_author("Hanafy et al.");
    set_title(
        "HANAFY ET AL. CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE");
    set_hidden();
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 7,
                  4272);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_db("Based on experimental PVT data of 324 fluid samples taken from 176 "
           "wells located in 75 fields. This data represents 15 productive "
           "zones of 123 reservoirs distributed along three different regions "
           "of Egypt, including the Gulf of Suez, Western Desert, and Sinai.");
    add_ref("hanafy:1997");
  }

public:
  static inline double impl(const double& rsb) noexcept;

  Quantity<psia> call(const Quantity<SCF_STB>& rsb) const {
    return Quantity<psia>(impl(rsb.raw()));
  }

  Quantity<psia> validate_and_call(const Quantity<SCF_STB>& rsb) const {
    if (not(rsb >= 7))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 7");
    if (not(rsb <= 4272))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 4272");

    return call(rsb);
  }

  Quantity<psia> operator()(const Quantity<SCF_STB>& rsb) const {
    return Quantity<psia>(call(rsb));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& rsb = it.get_curr();

    return call(rsb);
  }
};

extern Quantity<psia> PbHanafyDetachedFromRs__correlation__fct(
    const Quantity<SCF_STB>& rsb);
extern double PbHanafyDetachedFromRs__correlation__valid__fct(
    const double& rsb);

/** PbHanafy

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group


References:

  - hanafy:1997

@param[in] rsb Solution GOR at Pb. Range in [7, 4272]
@return inside [36, 5003]
@ingroup Correlations
*/
class PbHanafy : public BubblePointPressure {
  Correlation_Singleton(PbHanafy);

  PbHanafy() : BubblePointPressure("PbHanafy", psia::get_instance(), 36, 5003) {
    set_author("Hanafy et al.");
    set_title(
        "HANAFY ET AL. CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 7,
                  4272);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_db("Based on experimental PVT data of 324 fluid samples taken from 176 "
           "wells located in 75 fields. This data represents 15 productive "
           "zones of 123 reservoirs distributed along three different regions "
           "of Egypt, including the Gulf of Suez, Western Desert, and Sinai.");
    add_ref("hanafy:1997");
  }

public:
  static inline double impl(const double& rsb) noexcept;

  Quantity<psia> call(const Quantity<SCF_STB>& rsb) const {
    return Quantity<psia>(impl(rsb.raw()));
  }

  Quantity<psia> validate_and_call(const Quantity<SCF_STB>& rsb) const {
    if (not(rsb >= 7))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 7");
    if (not(rsb <= 4272))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 4272");

    return call(rsb);
  }

  Quantity<psia> operator()(const Quantity<SCF_STB>& rsb) const {
    return Quantity<psia>(call(rsb));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& rsb = it.get_curr();

    return call(rsb);
  }
};

extern Quantity<psia> PbHanafy__correlation__fct(const Quantity<SCF_STB>& rsb);
extern double PbHanafy__correlation__valid__fct(const double& rsb);

/** PbKartoatmodjoSchmidt

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group


References:

  - kartoatmodjo:1991

@param[in] yg Gas specific gravity. Range in [0.4824, 1.668]
@param[in] rsb Solution GOR at Pb. Range in [0, 2890]
@param[in] api API oil gravity. Range in [14.4, 59]
@param[in] t Temperature. Range in [75, 320]
@param[in] tsep Separator temperature. Range in [38, 294]
@param[in] psep Separator pressure. Range in [14.7, 1414.7]
@return inside [14.7, 6054.7]
@ingroup Correlations
*/
class PbKartoatmodjoSchmidt : public BubblePointPressure {
  Correlation_Singleton(PbKartoatmodjoSchmidt);

  PbKartoatmodjoSchmidt()
      : BubblePointPressure("PbKartoatmodjoSchmidt", psia::get_instance(), 14.7,
                            6054.7) {
    set_author("Kartoatmodjo & Schmidt");
    set_title("KARTOATMODJO & SCHMIDT CORRELATION, CALCULATION OF BUBBLE POINT "
              "PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.4824,
                  1.668);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 0,
                  2890);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("api", Api::get_instance(), "API oil gravity", 14.4, 59);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
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
  }

public:
  static inline double impl(const double& yg, const double& rsb,
                            const double& api, const double& t,
                            const double& tsep, const double& psep) noexcept;

  Quantity<psia> call(const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
                      const Quantity<Api>& api, const Quantity<Fahrenheit>& t,
                      const Quantity<Fahrenheit>& tsep,
                      const Quantity<psia>& psep) const {
    return Quantity<psia>(
        impl(yg.raw(), rsb.raw(), api.raw(), t.raw(), tsep.raw(), psep.raw()));
  }

  Quantity<psia> validate_and_call(const Quantity<Sgg>& yg,
                                   const Quantity<SCF_STB>& rsb,
                                   const Quantity<Api>& api,
                                   const Quantity<Fahrenheit>& t,
                                   const Quantity<Fahrenheit>& tsep,
                                   const Quantity<psia>& psep) const {
    if (not(yg >= 0.4824))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.4824");
    if (not(yg <= 1.668))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.668");
    if (not(rsb >= 0))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 0");
    if (not(rsb <= 2890))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 2890");
    if (not(api >= 14.4))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 14.4");
    if (not(api <= 59))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 59");
    if (not(tsep >= 38))
      ZENTHROW(OutOfParameterRange, "parameter tsep is less than 38");
    if (not(tsep <= 294))
      ZENTHROW(OutOfParameterRange, "parameter tsep is greater than 294");
    if (not(psep >= 14.7))
      ZENTHROW(OutOfParameterRange, "parameter psep is less than 14.7");
    if (not(psep <= 1414.7))
      ZENTHROW(OutOfParameterRange, "parameter psep is greater than 1414.7");

    return call(yg, rsb, api, t, tsep, psep);
  }

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t,
                            const Quantity<Fahrenheit>& tsep,
                            const Quantity<psia>& psep) const {
    return Quantity<psia>(call(yg, rsb, api, t, tsep, psep));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& tsep = it.get_curr();
    it.next();
    const auto& psep = it.get_curr();

    return call(yg, rsb, api, t, tsep, psep);
  }
};

extern Quantity<psia> PbKartoatmodjoSchmidt__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t,
    const Quantity<Fahrenheit>& tsep, const Quantity<psia>& psep);
extern double PbKartoatmodjoSchmidt__correlation__valid__fct(
    const double& yg, const double& rsb, const double& api, const double& t,
    const double& tsep, const double& psep);

/** PbLasater

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group


References:

  - lasater:1958

  - beggs:1987

@param[in] yg Gas specific gravity. Range in [0.574, 1.223]
@param[in] rsb Solution GOR at Pb. Range in [3, 2905]
@param[in] yo Oil specific gravity. Range in [Quantity<Api>(51.1),
Quantity<Api>(17.9)]
@param[in] t Temperature. Range in [82, 272]
@return inside [48, 5780]
@ingroup Correlations
*/
class PbLasater : public BubblePointPressure {
  Correlation_Singleton(PbLasater);

  PbLasater()
      : BubblePointPressure("PbLasater", psia::get_instance(), 48, 5780) {
    set_author("Lasater");
    set_title("LASATER CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.574,
                  1.223);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 3,
                  2905);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(51.1), Quantity<Api>(17.9));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 82, 272);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 158 experimentally measured bubble point pressures of 137 "
           "independent systems.");
    add_db("The data were obtained from Canada, Western and Mid-Continental "
           "United States, and South America.");
    add_ref("lasater:1958");
    add_ref("beggs:1987");
    add_par_synonym("yo", "api", "Api");
  }

public:
  static inline double impl(const double& yg, const double& rsb,
                            const double& yo, const double& t) noexcept;

  Quantity<psia> call(const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
                      const Quantity<Sg_do>& yo,
                      const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(impl(yg.raw(), rsb.raw(), yo.raw(), t.raw()));
  }

  Quantity<psia> validate_and_call(const Quantity<Sgg>& yg,
                                   const Quantity<SCF_STB>& rsb,
                                   const Quantity<Sg_do>& yo,
                                   const Quantity<Fahrenheit>& t) const {
    if (not(yg >= 0.574))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.574");
    if (not(yg <= 1.223))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.223");
    if (not(rsb >= 3))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 3");
    if (not(rsb <= 2905))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 2905");
    if (not(yo >= Quantity<Api>(51.1)))
      ZENTHROW(OutOfParameterRange,
               "parameter yo is less than Quantity<Api>(51.1)");
    if (not(yo <= Quantity<Api>(17.9)))
      ZENTHROW(OutOfParameterRange,
               "parameter yo is greater than Quantity<Api>(17.9)");

    return call(yg, rsb, yo, t);
  }

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Sg_do>& yo,
                            const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(call(yg, rsb, yo, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, rsb, yo, t);
  }
};

extern Quantity<psia> PbLasater__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Sg_do>& yo, const Quantity<Fahrenheit>& t);
extern double PbLasater__correlation__valid__fct(const double& yg,
                                                 const double& rsb,
                                                 const double& yo,
                                                 const double& t);

/** PbPetroskyFarshad

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group


References:

  - petrosky:1993

@param[in] yg Gas specific gravity. Range in [0.5781, 0.8519]
@param[in] rsb Solution GOR at Pb. Range in [217, 1406]
@param[in] api API oil gravity. Range in [16.3, 45]
@param[in] t Temperature. Range in [114, 288]
@return inside [1574, 6523]
@ingroup Correlations
*/
class PbPetroskyFarshad : public BubblePointPressure {
  Correlation_Singleton(PbPetroskyFarshad);

  PbPetroskyFarshad()
      : BubblePointPressure("PbPetroskyFarshad", psia::get_instance(), 1574,
                            6523) {
    set_author("Petrosky & Farshad");
    set_title(
        "PETROSKY & FARSHAD CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.5781,
                  0.8519);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 217,
                  1406);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("api", Api::get_instance(), "API oil gravity", 16.3, 45);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 114, 288);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 81 laboratory PVT analyses.");
    add_db("Fluid samples were obtained from reservoirs located offshore Texas "
           "and Louisiana. Producing areas from Galveston Island, eastward, "
           "through Main Pass are represented.");
    add_ref("petrosky:1993");
  }

public:
  static inline double impl(const double& yg, const double& rsb,
                            const double& api, const double& t) noexcept;

  Quantity<psia> call(const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
                      const Quantity<Api>& api,
                      const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(impl(yg.raw(), rsb.raw(), api.raw(), t.raw()));
  }

  Quantity<psia> validate_and_call(const Quantity<Sgg>& yg,
                                   const Quantity<SCF_STB>& rsb,
                                   const Quantity<Api>& api,
                                   const Quantity<Fahrenheit>& t) const {
    if (not(yg >= 0.5781))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.5781");
    if (not(yg <= 0.8519))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 0.8519");
    if (not(rsb >= 217))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 217");
    if (not(rsb <= 1406))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 1406");
    if (not(api >= 16.3))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 16.3");
    if (not(api <= 45))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 45");

    return call(yg, rsb, api, t);
  }

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(call(yg, rsb, api, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, rsb, api, t);
  }
};

extern Quantity<psia> PbPetroskyFarshad__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t);
extern double PbPetroskyFarshad__correlation__valid__fct(const double& yg,
                                                         const double& rsb,
                                                         const double& api,
                                                         const double& t);

/** PbStanding

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group


References:

  - standing:1947

  - alShammasi:2001

@param[in] yg Gas specific gravity. Range in [0.59, 0.95]
@param[in] rsb Solution GOR at Pb. Range in [20, 1425]
@param[in] api API oil gravity. Range in [16.5, 63.8]
@param[in] t Temperature. Range in [100, 258]
@return inside [130, 7000]
@ingroup Correlations
*/
class PbStanding : public BubblePointPressure {
  Correlation_Singleton(PbStanding);

  PbStanding()
      : BubblePointPressure("PbStanding", psia::get_instance(), 130, 7000) {
    set_author("Standing");
    set_title("STANDING CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.59,
                  0.95);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 20,
                  1425);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("api", Api::get_instance(), "API oil gravity", 16.5, 63.8);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 100, 258);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 105 experimentally determined bubble point pressures from "
           "22 different Californian crude-oil-natural-gas mixtures.");
    add_ref("standing:1947");
    add_ref("alShammasi:2001");
  }

public:
  static inline double impl(const double& yg, const double& rsb,
                            const double& api, const double& t) noexcept;

  Quantity<psia> call(const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
                      const Quantity<Api>& api,
                      const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(impl(yg.raw(), rsb.raw(), api.raw(), t.raw()));
  }

  Quantity<psia> validate_and_call(const Quantity<Sgg>& yg,
                                   const Quantity<SCF_STB>& rsb,
                                   const Quantity<Api>& api,
                                   const Quantity<Fahrenheit>& t) const {
    if (not(yg >= 0.59))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.59");
    if (not(yg <= 0.95))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 0.95");
    if (not(rsb >= 20))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 20");
    if (not(rsb <= 1425))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 1425");
    if (not(api >= 16.5))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 16.5");
    if (not(api <= 63.8))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 63.8");

    return call(yg, rsb, api, t);
  }

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(call(yg, rsb, api, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, rsb, api, t);
  }
};

extern Quantity<psia> PbStanding__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t);
extern double PbStanding__correlation__valid__fct(const double& yg,
                                                  const double& rsb,
                                                  const double& api,
                                                  const double& t);

/** PbTotalCFP

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group


References:

  - totalCFP:1983

  - banzer:1996

@param[in] yg Gas specific gravity. Range in [, ]
@param[in] rsb Solution GOR at Pb. Range in [, ]
@param[in] api API oil gravity. Range in [Api::get_instance().min_val, 45]
@param[in] t Temperature. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class PbTotalCFP : public BubblePointPressure {
  Correlation_Singleton(PbTotalCFP);

  PbTotalCFP() : BubblePointPressure("PbTotalCFP", psia::get_instance()) {
    set_author("Total CFP");
    set_title("TOTAL COMPAGNIE FRANÇAISE DES PÉTROLES (CFP) CORRELATION, "
              "CALCULATION OF BUBBLE POINT PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity");
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb");
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("api", Api::get_instance(), "API oil gravity",
                  Api::get_instance().min_val, 45);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature");
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 336 PVT analyses of crude oil from eastern Venezuela.");
    add_ref("totalCFP:1983");
    add_ref("banzer:1996");
  }

public:
  static inline double impl(const double& yg, const double& rsb,
                            const double& api, const double& t) noexcept;

  Quantity<psia> call(const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
                      const Quantity<Api>& api,
                      const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(impl(yg.raw(), rsb.raw(), api.raw(), t.raw()));
  }

  Quantity<psia> validate_and_call(const Quantity<Sgg>& yg,
                                   const Quantity<SCF_STB>& rsb,
                                   const Quantity<Api>& api,
                                   const Quantity<Fahrenheit>& t) const {
    if (not(api >= Api::get_instance().min_val))
      ZENTHROW(OutOfParameterRange,
               "parameter api is less than Api::get_instance().min_val");
    if (not(api <= 45))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 45");

    return call(yg, rsb, api, t);
  }

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(call(yg, rsb, api, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, rsb, api, t);
  }
};

extern Quantity<psia> PbTotalCFP__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t);
extern double PbTotalCFP__correlation__valid__fct(const double& yg,
                                                  const double& rsb,
                                                  const double& api,
                                                  const double& t);

/** PbVasquezBeggs

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group


The gas gravity is corrected if it resulted from a first-stage separation at a
pressure other than 100 psig (approximately 114.7 psia).

References:

  - vazquez:1980

  - beggs:1987

  - alShammasi:2001

@param[in] yg Gas specific gravity. Range in [0.511, 1.35]
@param[in] rsb Solution GOR at Pb. Range in [0, 2199]
@param[in] api API oil gravity. Range in [15.3, 59.3]
@param[in] t Temperature. Range in [75, 294]
@param[in] tsep Separator temperature. Range in [76, 150]
@param[in] psep Separator pressure. Range in [20, 565]
@return inside [15, 6055]
@ingroup Correlations
*/
class PbVasquezBeggs : public BubblePointPressure {
  Correlation_Singleton(PbVasquezBeggs);

  PbVasquezBeggs()
      : BubblePointPressure("PbVasquezBeggs", psia::get_instance(), 15, 6055) {
    set_author("Vazquez & Beggs");
    set_title(
        "VAZQUEZ & BEGGS CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.511,
                  1.35);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 0,
                  2199);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("api", Api::get_instance(), "API oil gravity", 15.3, 59.3);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 75, 294);
    def_parameter_latex_symbol("t", "T");
    add_parameter("tsep", Fahrenheit::get_instance(), "Separator temperature",
                  76, 150);
    def_parameter_latex_symbol("tsep", "T_{sp}");
    add_parameter("psep", psia::get_instance(), "Separator pressure", 20, 565);
    def_parameter_latex_symbol("psep", "P_{sp}");
    add_db("Based on more than 600 laboratory PVT analyses from fields all "
           "over the world.");
    add_ref("vazquez:1980");
    add_ref("beggs:1987");
    add_ref("alShammasi:2001");
  }

public:
  static inline double impl(const double& yg, const double& rsb,
                            const double& api, const double& t,
                            const double& tsep, const double& psep) noexcept;

  Quantity<psia> call(const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
                      const Quantity<Api>& api, const Quantity<Fahrenheit>& t,
                      const Quantity<Fahrenheit>& tsep,
                      const Quantity<psia>& psep) const {
    return Quantity<psia>(
        impl(yg.raw(), rsb.raw(), api.raw(), t.raw(), tsep.raw(), psep.raw()));
  }

  Quantity<psia> validate_and_call(const Quantity<Sgg>& yg,
                                   const Quantity<SCF_STB>& rsb,
                                   const Quantity<Api>& api,
                                   const Quantity<Fahrenheit>& t,
                                   const Quantity<Fahrenheit>& tsep,
                                   const Quantity<psia>& psep) const {
    if (not(yg >= 0.511))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.511");
    if (not(yg <= 1.35))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.35");
    if (not(rsb >= 0))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 0");
    if (not(rsb <= 2199))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 2199");
    if (not(api >= 15.3))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 15.3");
    if (not(api <= 59.3))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 59.3");
    if (not(tsep >= 76))
      ZENTHROW(OutOfParameterRange, "parameter tsep is less than 76");
    if (not(tsep <= 150))
      ZENTHROW(OutOfParameterRange, "parameter tsep is greater than 150");
    if (not(psep >= 20))
      ZENTHROW(OutOfParameterRange, "parameter psep is less than 20");
    if (not(psep <= 565))
      ZENTHROW(OutOfParameterRange, "parameter psep is greater than 565");

    return call(yg, rsb, api, t, tsep, psep);
  }

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t,
                            const Quantity<Fahrenheit>& tsep,
                            const Quantity<psia>& psep) const {
    return Quantity<psia>(call(yg, rsb, api, t, tsep, psep));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& tsep = it.get_curr();
    it.next();
    const auto& psep = it.get_curr();

    return call(yg, rsb, api, t, tsep, psep);
  }
};

extern Quantity<psia> PbVasquezBeggs__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t,
    const Quantity<Fahrenheit>& tsep, const Quantity<psia>& psep);
extern double PbVasquezBeggs__correlation__valid__fct(
    const double& yg, const double& rsb, const double& api, const double& t,
    const double& tsep, const double& psep);

/** PbVelarde

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group


References:

  - velarde:1997

@param[in] yg Gas specific gravity. Range in [0.556, 1.367]
@param[in] rsb Solution GOR at Pb. Range in [10, 1870]
@param[in] api API oil gravity. Range in [12, 55]
@param[in] t Temperature. Range in [74, 327]
@return inside [70, 6700]
@ingroup Correlations
*/
class PbVelarde : public BubblePointPressure {
  Correlation_Singleton(PbVelarde);

  PbVelarde()
      : BubblePointPressure("PbVelarde", psia::get_instance(), 70, 6700) {
    set_author("Velarde, Blasingame & McCain");
    set_title("VELARDE, BLASINGAME & McCAIN CORRELATION, CALCULATION OF BUBBLE "
              "POINT PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.556,
                  1.367);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 10,
                  1870);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("api", Api::get_instance(), "API oil gravity", 12, 55);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 74, 327);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 728 laboratory measured bubble point pressures.");
    add_ref("velarde:1997");
  }

public:
  static inline double impl(const double& yg, const double& rsb,
                            const double& api, const double& t) noexcept;

  Quantity<psia> call(const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
                      const Quantity<Api>& api,
                      const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(impl(yg.raw(), rsb.raw(), api.raw(), t.raw()));
  }

  Quantity<psia> validate_and_call(const Quantity<Sgg>& yg,
                                   const Quantity<SCF_STB>& rsb,
                                   const Quantity<Api>& api,
                                   const Quantity<Fahrenheit>& t) const {
    if (not(yg >= 0.556))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.556");
    if (not(yg <= 1.367))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.367");
    if (not(rsb >= 10))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 10");
    if (not(rsb <= 1870))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 1870");
    if (not(api >= 12))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 12");
    if (not(api <= 55))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 55");

    return call(yg, rsb, api, t);
  }

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(call(yg, rsb, api, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, rsb, api, t);
  }
};

extern Quantity<psia> PbVelarde__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t);
extern double PbVelarde__correlation__valid__fct(const double& yg,
                                                 const double& rsb,
                                                 const double& api,
                                                 const double& t);

/** PbCegarra

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group


The correlation could not be verified because the original reference is not
available. Date: August 10 2016.

References:

  - cegarra:2007

  - standing:1947

@param[in] yg Gas specific gravity. Range in [0.654, 1.337]
@param[in] rsb Solution GOR at Pb. Range in [135, 4290]
@param[in] api API oil gravity. Range in [22, 41.9]
@param[in] t Temperature. Range in [117, 354]
@return inside [735, 7303]
@ingroup Correlations
*/
class PbCegarra : public BubblePointPressure {
  Correlation_Singleton(PbCegarra);

  PbCegarra()
      : BubblePointPressure("PbCegarra", psia::get_instance(), 735, 7303) {
    set_author("Cegarra");
    set_title("CEGARRA CORRELATION (MODIFIED STANDING CORRELATION), "
              "CALCULATION OF BUBBLE POINT PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.654,
                  1.337);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 135,
                  4290);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("api", Api::get_instance(), "API oil gravity", 22, 41.9);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 117, 354);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 375 consistent PVT analyses. The data bank was compiled "
           "from 8394 fields of eastern Venezuela subdivided into 5192 fields "
           "of light oil and 3202 fields of medium oil. The correlation takes "
           "the functional form of Standing.");
    add_ref("cegarra:2007");
    add_ref("standing:1947");
  }

public:
  static inline double impl(const double& yg, const double& rsb,
                            const double& api, const double& t) noexcept;

  Quantity<psia> call(const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
                      const Quantity<Api>& api,
                      const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(impl(yg.raw(), rsb.raw(), api.raw(), t.raw()));
  }

  Quantity<psia> validate_and_call(const Quantity<Sgg>& yg,
                                   const Quantity<SCF_STB>& rsb,
                                   const Quantity<Api>& api,
                                   const Quantity<Fahrenheit>& t) const {
    if (not(yg >= 0.654))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.654");
    if (not(yg <= 1.337))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.337");
    if (not(rsb >= 135))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 135");
    if (not(rsb <= 4290))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 4290");
    if (not(api >= 22))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 22");
    if (not(api <= 41.9))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 41.9");

    return call(yg, rsb, api, t);
  }

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(call(yg, rsb, api, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, rsb, api, t);
  }
};

extern Quantity<psia> PbCegarra__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t);
extern double PbCegarra__correlation__valid__fct(const double& yg,
                                                 const double& rsb,
                                                 const double& api,
                                                 const double& t);

/** PbPerezML

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group

Pérez, Heny & Lago correlation (modified Total CFP correlation), calculation of
bubble point pressure.

The correlation could not be verified because the original reference is not
available. Date: August 10 2016.

References:

  - perez:2001

@param[in] yg Gas specific gravity. Range in [0.571, 0.981]
@param[in] rsb Solution GOR at Pb. Range in [38, 121]
@param[in] api API oil gravity. Range in [6.4, 12.9]
@param[in] t Temperature. Range in [112, 300]
@return inside [405, 1335]
@ingroup Correlations
*/
class PbPerezML : public BubblePointPressure {
  Correlation_Singleton(PbPerezML);

  PbPerezML()
      : BubblePointPressure("PbPerezML", psia::get_instance(), 405, 1335) {
    set_author("Pérez, Heny & Lago");
    set_title("PÉREZ, HENY & LAGO CORRELATION (MODIFIED TOTAL CFP "
              "CORRELATION), CALCULATION OF BUBBLE POINT PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.571,
                  0.981);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 38,
                  121);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("api", Api::get_instance(), "API oil gravity", 6.4, 12.9);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 112, 300);
    def_parameter_latex_symbol("t", "T");
    add_note("Pérez, Heny & Lago correlation (modified Total CFP correlation), "
             "calculation of bubble point pressure.");
    add_ref("perez:2001");
  }

public:
  static inline double impl(const double& yg, const double& rsb,
                            const double& api, const double& t) noexcept;

  Quantity<psia> call(const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
                      const Quantity<Api>& api,
                      const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(impl(yg.raw(), rsb.raw(), api.raw(), t.raw()));
  }

  Quantity<psia> validate_and_call(const Quantity<Sgg>& yg,
                                   const Quantity<SCF_STB>& rsb,
                                   const Quantity<Api>& api,
                                   const Quantity<Fahrenheit>& t) const {
    if (not(yg >= 0.571))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.571");
    if (not(yg <= 0.981))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 0.981");
    if (not(rsb >= 38))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 38");
    if (not(rsb <= 121))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 121");
    if (not(api >= 6.4))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 6.4");
    if (not(api <= 12.9))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 12.9");

    return call(yg, rsb, api, t);
  }

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(call(yg, rsb, api, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, rsb, api, t);
  }
};

extern Quantity<psia> PbPerezML__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t);
extern double PbPerezML__correlation__valid__fct(const double& yg,
                                                 const double& rsb,
                                                 const double& api,
                                                 const double& t);

/** PbMillanArcia

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group


The correlation was verified by using a secondary reference (Salazar, 2008).
Date: July 27 2017.

References:

  - millan:1984

  - perez:2001

@param[in] yg Gas specific gravity. Range in [, ]
@param[in] rsb Solution GOR at Pb. Range in [53, 459]
@param[in] api API oil gravity. Range in [9, 20.2]
@param[in] t Temperature. Range in [87, 195]
@return inside [222, 3432.7]
@ingroup Correlations
*/
class PbMillanArcia : public BubblePointPressure {
  Correlation_Singleton(PbMillanArcia);

  PbMillanArcia()
      : BubblePointPressure("PbMillanArcia", psia::get_instance(), 222,
                            3432.7) {
    set_author("Millán-Arcia");
    set_title("MILLÁN-ARCIA CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity");
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 53,
                  459);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("api", Api::get_instance(), "API oil gravity", 9, 20.2);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 87, 195);
    def_parameter_latex_symbol("t", "T");
    add_db("Venezuelan heavy crudes were correlated.");
    add_ref("millan:1984");
    add_ref("perez:2001");
  }

public:
  static inline double impl(const double& yg, const double& rsb,
                            const double& api, const double& t) noexcept;

  Quantity<psia> call(const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
                      const Quantity<Api>& api,
                      const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(impl(yg.raw(), rsb.raw(), api.raw(), t.raw()));
  }

  Quantity<psia> validate_and_call(const Quantity<Sgg>& yg,
                                   const Quantity<SCF_STB>& rsb,
                                   const Quantity<Api>& api,
                                   const Quantity<Fahrenheit>& t) const {
    if (not(rsb >= 53))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 53");
    if (not(rsb <= 459))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 459");
    if (not(api >= 9))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 9");
    if (not(api <= 20.2))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 20.2");

    return call(yg, rsb, api, t);
  }

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(call(yg, rsb, api, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, rsb, api, t);
  }
};

extern Quantity<psia> PbMillanArcia__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t);
extern double PbMillanArcia__correlation__valid__fct(const double& yg,
                                                     const double& rsb,
                                                     const double& api,
                                                     const double& t);

/** PbManucciRosales

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group


The correlation could not be verified because the original reference is not
available. Date: August 10 2016.

References:

  - manucci:1968

  - perez:2001

@param[in] yg Gas specific gravity. Range in [, ]
@param[in] rsb Solution GOR at Pb. Range in [180, 2500]
@param[in] api API oil gravity. Range in [14, 40]
@param[in] t Temperature. Range in [165, 304]
@return inside [1230, 5035]
@ingroup Correlations
*/
class PbManucciRosales : public BubblePointPressure {
  Correlation_Singleton(PbManucciRosales);

  PbManucciRosales()
      : BubblePointPressure("PbManucciRosales", psia::get_instance(), 1230,
                            5035) {
    set_author("Manucci & Rosales");
    set_title(
        "MANUCCI & ROSALES CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity");
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 180,
                  2500);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("api", Api::get_instance(), "API oil gravity", 14, 40);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 165, 304);
    def_parameter_latex_symbol("t", "T");
    add_db("Crude oils of eastern Venezuela were correlated, by using "
           "differential separation data from laboratory PVT analysis.");
    add_ref("manucci:1968");
    add_ref("perez:2001");
  }

public:
  static inline double impl(const double& yg, const double& rsb,
                            const double& api, const double& t) noexcept;

  Quantity<psia> call(const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
                      const Quantity<Api>& api,
                      const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(impl(yg.raw(), rsb.raw(), api.raw(), t.raw()));
  }

  Quantity<psia> validate_and_call(const Quantity<Sgg>& yg,
                                   const Quantity<SCF_STB>& rsb,
                                   const Quantity<Api>& api,
                                   const Quantity<Fahrenheit>& t) const {
    if (not(rsb >= 180))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 180");
    if (not(rsb <= 2500))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 2500");
    if (not(api >= 14))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 14");
    if (not(api <= 40))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 40");

    return call(yg, rsb, api, t);
  }

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(call(yg, rsb, api, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, rsb, api, t);
  }
};

extern Quantity<psia> PbManucciRosales__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t);
extern double PbManucciRosales__correlation__valid__fct(const double& yg,
                                                        const double& rsb,
                                                        const double& api,
                                                        const double& t);

/** PbGlaso

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group


References:

  - glaso:1980

@param[in] yg Gas specific gravity. Range in [0.65, 1.276]
@param[in] rsb Solution GOR at Pb. Range in [90, 2637]
@param[in] api API oil gravity. Range in [22.3, 48.1]
@param[in] t Temperature. Range in [80, 280]
@param[in] n2 N2 concentration. Range in [Quantity<MolePercent>(0),
Quantity<MolePercent>(26)]
@param[in] co2 CO2 concentration. Range in [Quantity<MolePercent>(0),
Quantity<MolePercent>(26)]
@param[in] h2s H2S concentration. Range in [Quantity<MolePercent>(0),
Quantity<MolePercent>(50)]
@return inside [135.304, 7112.3]
@ingroup Correlations
*/
class PbGlaso : public BubblePointPressure {
  Correlation_Singleton(PbGlaso);

  PbGlaso()
      : BubblePointPressure("PbGlaso", psia::get_instance(), 135.304, 7112.3) {
    set_author("Glaso");
    set_title("GLASO CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.65,
                  1.276);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 90,
                  2637);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("api", Api::get_instance(), "API oil gravity", 22.3, 48.1);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 80, 280);
    def_parameter_latex_symbol("t", "T");
    add_parameter("n2", MoleFraction::get_instance(), "N2 concentration",
                  Quantity<MolePercent>(0), Quantity<MolePercent>(26));
    add_parameter("co2", MoleFraction::get_instance(), "CO2 concentration",
                  Quantity<MolePercent>(0), Quantity<MolePercent>(26));
    add_parameter("h2s", MoleFraction::get_instance(), "H2S concentration",
                  Quantity<MolePercent>(0), Quantity<MolePercent>(50));
    add_db("Based on 26 samples from the North Sea (collected from wells in "
           "the region 56 to 62°N) and 19 samples from the Middle East, "
           "Algeria, and several areas in the U.S.");
    add_db("The equation was derived from laboratory data, exclusively "
           "sampling North Sea oils. However, the author claims that the "
           "correlation should be valid for all types of gas/oil mixtures "
           "after correcting for non-hydrocarbons in the surface gases and "
           "paraffinicity of the oil.");
    add_ref("glaso:1980");
  }

  inline void precondition(const Quantity<MoleFraction>& n2,
                           const Quantity<MoleFraction>& co2,
                           const Quantity<MoleFraction>& h2s) const;

public:
  static inline double impl(const double& yg, const double& rsb,
                            const double& api, const double& t,
                            const double& n2, const double& co2,
                            const double& h2s) noexcept;

  Quantity<psia> call(const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
                      const Quantity<Api>& api, const Quantity<Fahrenheit>& t,
                      const Quantity<MoleFraction>& n2,
                      const Quantity<MoleFraction>& co2,
                      const Quantity<MoleFraction>& h2s) const {
    precondition(n2, co2, h2s);
    return Quantity<psia>(impl(yg.raw(), rsb.raw(), api.raw(), t.raw(),
                               n2.raw(), co2.raw(), h2s.raw()));
  }

  Quantity<psia> validate_and_call(const Quantity<Sgg>& yg,
                                   const Quantity<SCF_STB>& rsb,
                                   const Quantity<Api>& api,
                                   const Quantity<Fahrenheit>& t,
                                   const Quantity<MoleFraction>& n2,
                                   const Quantity<MoleFraction>& co2,
                                   const Quantity<MoleFraction>& h2s) const {
    if (not(yg >= 0.65))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.65");
    if (not(yg <= 1.276))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.276");
    if (not(rsb >= 90))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 90");
    if (not(rsb <= 2637))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 2637");
    if (not(api >= 22.3))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 22.3");
    if (not(api <= 48.1))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 48.1");
    if (not(n2 >= Quantity<MolePercent>(0)))
      ZENTHROW(OutOfParameterRange,
               "parameter n2 is less than Quantity<MolePercent>(0)");
    if (not(n2 <= Quantity<MolePercent>(26)))
      ZENTHROW(OutOfParameterRange,
               "parameter n2 is greater than Quantity<MolePercent>(26)");
    if (not(co2 >= Quantity<MolePercent>(0)))
      ZENTHROW(OutOfParameterRange,
               "parameter co2 is less than Quantity<MolePercent>(0)");
    if (not(co2 <= Quantity<MolePercent>(26)))
      ZENTHROW(OutOfParameterRange,
               "parameter co2 is greater than Quantity<MolePercent>(26)");
    if (not(h2s >= Quantity<MolePercent>(0)))
      ZENTHROW(OutOfParameterRange,
               "parameter h2s is less than Quantity<MolePercent>(0)");
    if (not(h2s <= Quantity<MolePercent>(50)))
      ZENTHROW(OutOfParameterRange,
               "parameter h2s is greater than Quantity<MolePercent>(50)");

    return call(yg, rsb, api, t, n2, co2, h2s);
  }

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t,
                            const Quantity<MoleFraction>& n2,
                            const Quantity<MoleFraction>& co2,
                            const Quantity<MoleFraction>& h2s) const {
    return Quantity<psia>(call(yg, rsb, api, t, n2, co2, h2s));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& n2 = it.get_curr();
    it.next();
    const auto& co2 = it.get_curr();
    it.next();
    const auto& h2s = it.get_curr();

    return call(yg, rsb, api, t, n2, co2, h2s);
  }
};

extern Quantity<psia> PbGlaso__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t,
    const Quantity<MoleFraction>& n2, const Quantity<MoleFraction>& co2,
    const Quantity<MoleFraction>& h2s);
extern double PbGlaso__correlation__valid__fct(
    const double& yg, const double& rsb, const double& api, const double& t,
    const double& n2, const double& co2, const double& h2s);

/** PbLopezCR

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group

López, Castillo & Rojas presented separate equations for solution gas oil ratio
(Rs) and bubble point pressure (Pb). Their Rs equation provides a higher
determination coefficient and a lower arithmetic average error; therefore, the
correlation for Rs is used to calculate Pb by rearranging the equation.

The correlation was verified by using the original reference. Date: April 27
2017.

References:

  - lopez:2014

@param[in] yg Gas specific gravity. Range in [0.5086, 0.75]
@param[in] rsb Solution GOR at Pb. Range in [0, 120]
@param[in] api API oil gravity. Range in [7.4, 14.5]
@param[in] t Temperature. Range in [124, 220]
@return inside [100, 1315]
@ingroup Correlations
*/
class PbLopezCR : public BubblePointPressure {
  Correlation_Singleton(PbLopezCR);

  PbLopezCR()
      : BubblePointPressure("PbLopezCR", psia::get_instance(), 100, 1315) {
    set_author("López, Castillo & Rojas");
    set_title("LÓPEZ, CASTILLO & ROJAS CORRELATION, CALCULATION OF BUBBLE "
              "POINT PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.5086,
                  0.75);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 0, 120);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("api", Api::get_instance(), "API oil gravity", 7.4, 14.5);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 124, 220);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 48 PVT tests from saturated oil reservoirs located in "
           "Bare and Arecuna fields, from the Ayacucho Area of the Faja "
           "Petrolífera del Orinoco.");
    add_note("López, Castillo & Rojas presented separate equations for "
             "solution gas oil ratio (Rs) and bubble point pressure (Pb). "
             "Their Rs equation provides a higher determination coefficient "
             "and a lower arithmetic average error; therefore, the correlation "
             "for Rs is used to calculate Pb by rearranging the equation.");
    add_ref("lopez:2014");
  }

public:
  static inline double impl(const double& yg, const double& rsb,
                            const double& api, const double& t) noexcept;

  Quantity<psia> call(const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
                      const Quantity<Api>& api,
                      const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(impl(yg.raw(), rsb.raw(), api.raw(), t.raw()));
  }

  Quantity<psia> validate_and_call(const Quantity<Sgg>& yg,
                                   const Quantity<SCF_STB>& rsb,
                                   const Quantity<Api>& api,
                                   const Quantity<Fahrenheit>& t) const {
    if (not(yg >= 0.5086))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.5086");
    if (not(yg <= 0.75))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 0.75");
    if (not(rsb >= 0))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 0");
    if (not(rsb <= 120))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 120");
    if (not(api >= 7.4))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 7.4");
    if (not(api <= 14.5))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 14.5");

    return call(yg, rsb, api, t);
  }

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(call(yg, rsb, api, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, rsb, api, t);
  }
};

extern Quantity<psia> PbLopezCR__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t);
extern double PbLopezCR__correlation__valid__fct(const double& yg,
                                                 const double& rsb,
                                                 const double& api,
                                                 const double& t);

/** PbLopezCRDetachedFromRs

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group


The correlation was verified by using the original reference. Date: April 27
2017.

References:

  - lopez:2014

@param[in] yg Gas specific gravity. Range in [0.5086, 0.75]
@param[in] rsb Solution GOR at Pb. Range in [12, 110]
@param[in] api API oil gravity. Range in [7.4, 14.5]
@param[in] t Temperature. Range in [124, 220]
@return inside [100, 1315]
@ingroup Correlations
*/
class PbLopezCRDetachedFromRs : public BubblePointPressure {
  Correlation_Singleton(PbLopezCRDetachedFromRs);

  PbLopezCRDetachedFromRs()
      : BubblePointPressure("PbLopezCRDetachedFromRs", psia::get_instance(),
                            100, 1315) {
    set_author("López, Castillo & Rojas");
    set_title("LÓPEZ, CASTILLO & ROJAS CORRELATION, CALCULATION OF BUBBLE "
              "POINT PRESSURE");
    set_hidden();
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.5086,
                  0.75);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 12,
                  110);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("api", Api::get_instance(), "API oil gravity", 7.4, 14.5);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 124, 220);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 48 PVT tests from saturated oil reservoirs located in "
           "Bare and Arecuna fields, from the Ayacucho Area of the Faja "
           "Petrolífera del Orinoco.");
    add_ref("lopez:2014");
  }

public:
  static inline double impl(const double& yg, const double& rsb,
                            const double& api, const double& t) noexcept;

  Quantity<psia> call(const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
                      const Quantity<Api>& api,
                      const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(impl(yg.raw(), rsb.raw(), api.raw(), t.raw()));
  }

  Quantity<psia> validate_and_call(const Quantity<Sgg>& yg,
                                   const Quantity<SCF_STB>& rsb,
                                   const Quantity<Api>& api,
                                   const Quantity<Fahrenheit>& t) const {
    if (not(yg >= 0.5086))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.5086");
    if (not(yg <= 0.75))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 0.75");
    if (not(rsb >= 12))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 12");
    if (not(rsb <= 110))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 110");
    if (not(api >= 7.4))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 7.4");
    if (not(api <= 14.5))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 14.5");

    return call(yg, rsb, api, t);
  }

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(call(yg, rsb, api, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, rsb, api, t);
  }
};

extern Quantity<psia> PbLopezCRDetachedFromRs__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t);
extern double PbLopezCRDetachedFromRs__correlation__valid__fct(
    const double& yg, const double& rsb, const double& api, const double& t);

/** PbSalazar

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group

Salazar (2008) presented the correlation as model 1.

The correlation was verified by using the original reference. Date: April 28
2017.

References:

  - salazar:2008

@param[in] yg Gas specific gravity. Range in [0.601, 1.107]
@param[in] rsb Solution GOR at Pb. Range in [26, 149]
@param[in] api API oil gravity. Range in [7.1, 18.9]
@param[in] t Temperature. Range in [108, 169]
@return inside [253, 1564]
@ingroup Correlations
*/
class PbSalazar : public BubblePointPressure {
  Correlation_Singleton(PbSalazar);

  PbSalazar()
      : BubblePointPressure("PbSalazar", psia::get_instance(), 253, 1564) {
    set_author("Salazar");
    set_title("SALAZAR CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.601,
                  1.107);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 26,
                  149);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("api", Api::get_instance(), "API oil gravity", 7.1, 18.9);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 108, 169);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 136 PVT reports from different areas of the Faja "
           "Petrolífera del Orinoco: 60 from Ayacucho, 48 from Junín, 26 from "
           "Carabobo, and 2 from Boyacá.");
    add_note("Salazar (2008) presented the correlation as model 1.");
    add_ref("salazar:2008");
  }

public:
  static inline double impl(const double& yg, const double& rsb,
                            const double& api, const double& t) noexcept;

  Quantity<psia> call(const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
                      const Quantity<Api>& api,
                      const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(impl(yg.raw(), rsb.raw(), api.raw(), t.raw()));
  }

  Quantity<psia> validate_and_call(const Quantity<Sgg>& yg,
                                   const Quantity<SCF_STB>& rsb,
                                   const Quantity<Api>& api,
                                   const Quantity<Fahrenheit>& t) const {
    if (not(yg >= 0.601))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.601");
    if (not(yg <= 1.107))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.107");
    if (not(rsb >= 26))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 26");
    if (not(rsb <= 149))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 149");
    if (not(api >= 7.1))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 7.1");
    if (not(api <= 18.9))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 18.9");

    return call(yg, rsb, api, t);
  }

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t) const {
    return Quantity<psia>(call(yg, rsb, api, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, rsb, api, t);
  }
};

extern Quantity<psia> PbSalazar__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t);
extern double PbSalazar__correlation__valid__fct(const double& yg,
                                                 const double& rsb,
                                                 const double& api,
                                                 const double& t);

#endif // BUBBLE_POINT_PRESSURE_H
