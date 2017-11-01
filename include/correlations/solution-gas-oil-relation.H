#ifndef SOLUTION_GAS_OIL_RELATION_H
#define SOLUTION_GAS_OIL_RELATION_H

#include <pvt-exceptions.H>
#include <correlations/correlation.H>

/** RsAlShammasi

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group


References:

  - alShammasi:1999

@param[in] yg Gas specific gravity. Range in [0.51, 3.44]
@param[in] p Pressure. Range in [Quantity<Atmosphere>(1), ]
@param[in] yo Oil specific gravity. Range in [Quantity<Api>(63.7),
Quantity<Api>(6)]
@param[in] t Temperature. Range in [74, 341.6]
@return inside [6, 3298.6]
@ingroup Correlations
*/
class RsAlShammasi : public SolutionGasOilRelation {
  Correlation_Singleton(RsAlShammasi);

  RsAlShammasi()
      : SolutionGasOilRelation("RsAlShammasi", SCF_STB::get_instance(), 6,
                               3298.6) {
    set_author("Al-Shammasi");
    set_title("AL-SHAMMASI CORRELATION, CALCULATION OF SOLUTION GOR (FOR "
              "PRESSURES BELOW THE BUBBLE POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.51,
                  3.44);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure",
                  Quantity<Atmosphere>(1));
    def_parameter_latex_symbol("p", "P");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(63.7), Quantity<Api>(6));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 74, 341.6);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on a total of 1709 data sets (1661 data sets from 13 "
           "different published literature papers and 48 data sets originated "
           "from Kuwait reservoirs from unpublished sources).");
    add_ref("alShammasi:1999");
    add_par_synonym("yo", "api", "Api");
  }

public:
  static inline double impl(const double& yg, const double& p, const double& yo,
                            const double& t) noexcept;

  Quantity<SCF_STB> call(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                         const Quantity<Sg_do>& yo,
                         const Quantity<Fahrenheit>& t) const {
    return Quantity<SCF_STB>(impl(yg.raw(), p.raw(), yo.raw(), t.raw()));
  }

  Quantity<SCF_STB> validate_and_call(const Quantity<Sgg>& yg,
                                      const Quantity<psia>& p,
                                      const Quantity<Sg_do>& yo,
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

    return call(yg, p, yo, t);
  }

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Sg_do>& yo,
                               const Quantity<Fahrenheit>& t) const {
    return Quantity<SCF_STB>(call(yg, p, yo, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, p, yo, t);
  }
};

extern Quantity<SCF_STB> RsAlShammasi__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Sg_do>& yo,
    const Quantity<Fahrenheit>& t);
extern double RsAlShammasi__correlation__valid__fct(const double& yg,
                                                    const double& p,
                                                    const double& yo,
                                                    const double& t);

/** RsAlShammasiCalibration

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group


References:

  - alShammasi:1999

@param[in] yg Gas specific gravity. Range in [0.51, 3.44]
@param[in] p Pressure. Range in [Quantity<Atmosphere>(1), ]
@param[in] yo Oil specific gravity. Range in [Quantity<Api>(63.7),
Quantity<Api>(6)]
@param[in] t Temperature. Range in [74, 341.6]
@param[in] c c. Range in [, ]
@param[in] m m. Range in [, ]
@return inside [6, 3298.6]
@ingroup Correlations
*/
class RsAlShammasiCalibration : public SolutionGasOilRelation {
  Correlation_Singleton(RsAlShammasiCalibration);

  RsAlShammasiCalibration()
      : SolutionGasOilRelation("RsAlShammasiCalibration",
                               SCF_STB::get_instance(), 6, 3298.6) {
    set_author("Al-Shammasi");
    set_title("AL-SHAMMASI CORRELATION, CALCULATION OF SOLUTION GOR (FOR "
              "PRESSURES BELOW THE BUBBLE POINT)");
    set_hidden();
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.51,
                  3.44);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure",
                  Quantity<Atmosphere>(1));
    def_parameter_latex_symbol("p", "P");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(63.7), Quantity<Api>(6));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 74, 341.6);
    def_parameter_latex_symbol("t", "T");
    add_parameter("c", Dim_Less::get_instance(), "c");
    add_parameter("m", Dim_Less::get_instance(), "m");
    add_db("Based on a total of 1709 data sets (1661 data sets from 13 "
           "different published literature papers and 48 data sets originated "
           "from Kuwait reservoirs from unpublished sources).");
    add_ref("alShammasi:1999");
    add_par_synonym("yo", "api", "Api");
  }

public:
  static inline double impl(const double& yg, const double& p, const double& yo,
                            const double& t, const double& c,
                            const double& m) noexcept;

  Quantity<SCF_STB> call(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                         const Quantity<Sg_do>& yo,
                         const Quantity<Fahrenheit>& t,
                         const Quantity<Dim_Less>& c,
                         const Quantity<Dim_Less>& m) const {
    return Quantity<SCF_STB>(
        impl(yg.raw(), p.raw(), yo.raw(), t.raw(), c.raw(), m.raw()));
  }

  Quantity<SCF_STB> validate_and_call(const Quantity<Sgg>& yg,
                                      const Quantity<psia>& p,
                                      const Quantity<Sg_do>& yo,
                                      const Quantity<Fahrenheit>& t,
                                      const Quantity<Dim_Less>& c,
                                      const Quantity<Dim_Less>& m) const {
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

    return call(yg, p, yo, t, c, m);
  }

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Sg_do>& yo,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<Dim_Less>& c,
                               const Quantity<Dim_Less>& m) const {
    return Quantity<SCF_STB>(call(yg, p, yo, t, c, m));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& c = it.get_curr();
    it.next();
    const auto& m = it.get_curr();

    return call(yg, p, yo, t, c, m);
  }
};

extern Quantity<SCF_STB> RsAlShammasiCalibration__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Sg_do>& yo,
    const Quantity<Fahrenheit>& t, const Quantity<Dim_Less>& c,
    const Quantity<Dim_Less>& m);
extern double RsAlShammasiCalibration__correlation__valid__fct(
    const double& yg, const double& p, const double& yo, const double& t,
    const double& c, const double& m);

/** RsAlMarhoun

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group


References:

  - alMarhoun:1988

@param[in] yg Gas specific gravity. Range in [0.752, 1.367]
@param[in] p Pressure. Range in [20, 3573]
@param[in] yo Oil specific gravity. Range in [Quantity<Api>(44.6),
Quantity<Api>(19.4)]
@param[in] t Temperature. Range in [Quantity<Fahrenheit>(74),
Quantity<Fahrenheit>(240)]
@return inside [26, 1602]
@ingroup Correlations
*/
class RsAlMarhoun : public SolutionGasOilRelation {
  Correlation_Singleton(RsAlMarhoun);

  RsAlMarhoun()
      : SolutionGasOilRelation("RsAlMarhoun", SCF_STB::get_instance(), 26,
                               1602) {
    set_author("Al-Marhoun");
    set_title("AL-MARHOUN CORRELATION, CALCULATION OF SOLUTION GOR (FOR "
              "PRESSURES BELOW THE BUBBLE POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.752,
                  1.367);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure", 20, 3573);
    def_parameter_latex_symbol("p", "P");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(44.6), Quantity<Api>(19.4));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("t", Rankine::get_instance(), "Temperature",
                  Quantity<Fahrenheit>(74), Quantity<Fahrenheit>(240));
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 69 bottomhole fluid samples from 69 Middle East oil "
           "reservoirs.");
    add_ref("alMarhoun:1988");
    add_par_synonym("yo", "api", "Api");
  }

public:
  static inline double impl(const double& yg, const double& p, const double& yo,
                            const double& t) noexcept;

  Quantity<SCF_STB> call(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                         const Quantity<Sg_do>& yo,
                         const Quantity<Rankine>& t) const {
    return Quantity<SCF_STB>(impl(yg.raw(), p.raw(), yo.raw(), t.raw()));
  }

  Quantity<SCF_STB> validate_and_call(const Quantity<Sgg>& yg,
                                      const Quantity<psia>& p,
                                      const Quantity<Sg_do>& yo,
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

    return call(yg, p, yo, t);
  }

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Sg_do>& yo,
                               const Quantity<Rankine>& t) const {
    return Quantity<SCF_STB>(call(yg, p, yo, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, p, yo, t);
  }
};

extern Quantity<SCF_STB> RsAlMarhoun__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Sg_do>& yo,
    const Quantity<Rankine>& t);
extern double RsAlMarhoun__correlation__valid__fct(const double& yg,
                                                   const double& p,
                                                   const double& yo,
                                                   const double& t);

/** RsDeGhetto

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group


De Ghetto et al. use Yg as the average specific gravity of total surface gases,
while the original correlation (Standing) uses it as the gravity of dissolved
gas. Since Standing doesn't specify if Yg is the gravity of total surface gases
or of the separator, it is assumed that the data is from a one stage separation
with average gas gravity.

References:

  - deGhetto:1995

  - standing:1947

@param[in] yg Gas specific gravity. Range in [0.623, 1.517]
@param[in] p Pressure. Range in [1038.49, 7411.54]
@param[in] api API oil gravity. Range in [6, 22.3]
@param[in] t Temperature. Range in [131.4, 250.7]
@param[in] tsep Separator temperature. Range in [59, 177.8]
@param[in] psep Separator pressure. Range in [14.5, 752.2]
@return inside [17.21, 640.25]
@ingroup Correlations
*/
class RsDeGhetto : public SolutionGasOilRelation {
  Correlation_Singleton(RsDeGhetto);

  RsDeGhetto()
      : SolutionGasOilRelation("RsDeGhetto", SCF_STB::get_instance(), 17.21,
                               640.25) {
    set_author("De Ghetto, Paone & Villa");
    set_title("DE GHETTO, PAONE & VILLA CORRELATION FOR EXTRA-HEAVY (MODIFIED "
              "STANDING CORRELATION) AND HEAVY (MODIFIED VAZQUEZ & BEGGS "
              "CORRELATION) OILS, CALCULATION OF SOLUTION GOR (FOR PRESSURES "
              "BELOW THE BUBBLE POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.623,
                  1.517);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure", 1038.49, 7411.54);
    def_parameter_latex_symbol("p", "P");
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
           "classes: extra-heavy oils for API<=10, heavy oils for "
           "10<API<=22.3.");
    add_ref("deGhetto:1995");
    add_ref("standing:1947");
  }

public:
  static inline double impl(const double& yg, const double& p,
                            const double& api, const double& t,
                            const double& tsep, const double& psep) noexcept;

  Quantity<SCF_STB> call(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                         const Quantity<Api>& api,
                         const Quantity<Fahrenheit>& t,
                         const Quantity<Fahrenheit>& tsep,
                         const Quantity<psia>& psep) const {
    return Quantity<SCF_STB>(
        impl(yg.raw(), p.raw(), api.raw(), t.raw(), tsep.raw(), psep.raw()));
  }

  Quantity<SCF_STB> validate_and_call(const Quantity<Sgg>& yg,
                                      const Quantity<psia>& p,
                                      const Quantity<Api>& api,
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
    if (not(tsep >= 59))
      ZENTHROW(OutOfParameterRange, "parameter tsep is less than 59");
    if (not(tsep <= 177.8))
      ZENTHROW(OutOfParameterRange, "parameter tsep is greater than 177.8");
    if (not(psep >= 14.5))
      ZENTHROW(OutOfParameterRange, "parameter psep is less than 14.5");
    if (not(psep <= 752.2))
      ZENTHROW(OutOfParameterRange, "parameter psep is greater than 752.2");

    return call(yg, p, api, t, tsep, psep);
  }

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<Fahrenheit>& tsep,
                               const Quantity<psia>& psep) const {
    return Quantity<SCF_STB>(call(yg, p, api, t, tsep, psep));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& tsep = it.get_curr();
    it.next();
    const auto& psep = it.get_curr();

    return call(yg, p, api, t, tsep, psep);
  }
};

extern Quantity<SCF_STB> RsDeGhetto__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
    const Quantity<Fahrenheit>& t, const Quantity<Fahrenheit>& tsep,
    const Quantity<psia>& psep);
extern double RsDeGhetto__correlation__valid__fct(
    const double& yg, const double& p, const double& api, const double& t,
    const double& tsep, const double& psep);

/** RsDindorukChristman

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group


References:

  - dindoruk:2004

@param[in] yg Gas specific gravity. Range in [0.6017, 1.027]
@param[in] p Pressure. Range in [, ]
@param[in] api API oil gravity. Range in [14.7, 40]
@param[in] t Temperature. Range in [117, 276]
@return inside [, ]
@ingroup Correlations
*/
class RsDindorukChristman : public SolutionGasOilRelation {
  Correlation_Singleton(RsDindorukChristman);

  RsDindorukChristman()
      : SolutionGasOilRelation("RsDindorukChristman", SCF_STB::get_instance()) {
    set_author("Dindoruk & Christman");
    set_title("DINDORUK & CHRISTMAN CORRELATION, CALCULATION OF SOLUTION GOR "
              "(FOR PRESSURES BELOW THE BUBBLE POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.6017,
                  1.027);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("api", Api::get_instance(), "API oil gravity", 14.7, 40);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 117, 276);
    def_parameter_latex_symbol("t", "T");
    add_ref("dindoruk:2004");
  }

public:
  static inline double impl(const double& yg, const double& p,
                            const double& api, const double& t) noexcept;

  Quantity<SCF_STB> call(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                         const Quantity<Api>& api,
                         const Quantity<Fahrenheit>& t) const {
    return Quantity<SCF_STB>(impl(yg.raw(), p.raw(), api.raw(), t.raw()));
  }

  Quantity<SCF_STB> validate_and_call(const Quantity<Sgg>& yg,
                                      const Quantity<psia>& p,
                                      const Quantity<Api>& api,
                                      const Quantity<Fahrenheit>& t) const {
    if (not(yg >= 0.6017))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.6017");
    if (not(yg <= 1.027))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.027");
    if (not(api >= 14.7))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 14.7");
    if (not(api <= 40))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 40");

    return call(yg, p, api, t);
  }

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t) const {
    return Quantity<SCF_STB>(call(yg, p, api, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, p, api, t);
  }
};

extern Quantity<SCF_STB> RsDindorukChristman__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
    const Quantity<Fahrenheit>& t);
extern double RsDindorukChristman__correlation__valid__fct(const double& yg,
                                                           const double& p,
                                                           const double& api,
                                                           const double& t);

/** RsDoklaOsman

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group


References:

  - dokla:1992

@param[in] yg Gas specific gravity. Range in [0.798, 1.29]
@param[in] p Pressure. Range in [, ]
@param[in] yo Oil specific gravity. Range in [0.8236, 0.886]
@param[in] t Temperature. Range in [Quantity<Fahrenheit>(190),
Quantity<Fahrenheit>(275)]
@return inside [, ]
@ingroup Correlations
*/
class RsDoklaOsman : public SolutionGasOilRelation {
  Correlation_Singleton(RsDoklaOsman);

  RsDoklaOsman()
      : SolutionGasOilRelation("RsDoklaOsman", SCF_STB::get_instance()) {
    set_author("Dokla & Osman");
    set_title("DOKLA & OSMAN CORRELATION, CALCULATION OF SOLUTION GOR (FOR "
              "PRESSURES BELOW THE BUBBLE POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.798,
                  1.29);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity", 0.8236,
                  0.886);
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("t", Rankine::get_instance(), "Temperature",
                  Quantity<Fahrenheit>(190), Quantity<Fahrenheit>(275));
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 51 bottomhole samples from UAE reservoirs.");
    add_ref("dokla:1992");
    add_par_synonym("yo", "api", "Api");
  }

public:
  static inline double impl(const double& yg, const double& p, const double& yo,
                            const double& t) noexcept;

  Quantity<SCF_STB> call(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                         const Quantity<Sg_do>& yo,
                         const Quantity<Rankine>& t) const {
    return Quantity<SCF_STB>(impl(yg.raw(), p.raw(), yo.raw(), t.raw()));
  }

  Quantity<SCF_STB> validate_and_call(const Quantity<Sgg>& yg,
                                      const Quantity<psia>& p,
                                      const Quantity<Sg_do>& yo,
                                      const Quantity<Rankine>& t) const {
    if (not(yg >= 0.798))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.798");
    if (not(yg <= 1.29))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.29");
    if (not(yo >= 0.8236))
      ZENTHROW(OutOfParameterRange, "parameter yo is less than 0.8236");
    if (not(yo <= 0.886))
      ZENTHROW(OutOfParameterRange, "parameter yo is greater than 0.886");

    return call(yg, p, yo, t);
  }

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Sg_do>& yo,
                               const Quantity<Rankine>& t) const {
    return Quantity<SCF_STB>(call(yg, p, yo, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, p, yo, t);
  }
};

extern Quantity<SCF_STB> RsDoklaOsman__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Sg_do>& yo,
    const Quantity<Rankine>& t);
extern double RsDoklaOsman__correlation__valid__fct(const double& yg,
                                                    const double& p,
                                                    const double& yo,
                                                    const double& t);

/** RsGlaso

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group


P must be strictly greater than zero for the calculation of the logarithm.

References:

  - glaso:1980

@param[in] yg Gas specific gravity. Range in [0.65, 1.276]
@param[in] p Pressure. Range in [, ]
@param[in] api API oil gravity. Range in [22.3, 48.1]
@param[in] t Temperature. Range in [80, 280]
@param[in] n2 N2 concentration. Range in [Quantity<MolePercent>(0),
Quantity<MolePercent>(26)]
@param[in] co2 CO2 concentration. Range in [Quantity<MolePercent>(0),
Quantity<MolePercent>(26)]
@param[in] h2s H2S concentration. Range in [Quantity<MolePercent>(0),
Quantity<MolePercent>(50)]
@return inside [90, 2637]
@ingroup Correlations
*/
class RsGlaso : public SolutionGasOilRelation {
  Correlation_Singleton(RsGlaso);

  RsGlaso()
      : SolutionGasOilRelation("RsGlaso", SCF_STB::get_instance(), 90, 2637) {
    set_author("Glaso");
    set_title("GLASO CORRELATION, CALCULATION OF SOLUTION GOR (FOR PRESSURES "
              "BELOW THE BUBBLE POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.65,
                  1.276);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
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
    add_ref("glaso:1980");
  }

  inline void precondition(const Quantity<MoleFraction>& n2,
                           const Quantity<MoleFraction>& co2,
                           const Quantity<MoleFraction>& h2s) const;

public:
  static inline double impl(const double& yg, const double& p,
                            const double& api, const double& t,
                            const double& n2, const double& co2,
                            const double& h2s) noexcept;

  Quantity<SCF_STB> call(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                         const Quantity<Api>& api,
                         const Quantity<Fahrenheit>& t,
                         const Quantity<MoleFraction>& n2,
                         const Quantity<MoleFraction>& co2,
                         const Quantity<MoleFraction>& h2s) const {
    precondition(n2, co2, h2s);
    return Quantity<SCF_STB>(impl(yg.raw(), p.raw(), api.raw(), t.raw(),
                                  n2.raw(), co2.raw(), h2s.raw()));
  }

  Quantity<SCF_STB> validate_and_call(const Quantity<Sgg>& yg,
                                      const Quantity<psia>& p,
                                      const Quantity<Api>& api,
                                      const Quantity<Fahrenheit>& t,
                                      const Quantity<MoleFraction>& n2,
                                      const Quantity<MoleFraction>& co2,
                                      const Quantity<MoleFraction>& h2s) const {
    if (not(yg >= 0.65))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.65");
    if (not(yg <= 1.276))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.276");
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

    return call(yg, p, api, t, n2, co2, h2s);
  }

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<MoleFraction>& n2,
                               const Quantity<MoleFraction>& co2,
                               const Quantity<MoleFraction>& h2s) const {
    return Quantity<SCF_STB>(call(yg, p, api, t, n2, co2, h2s));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
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

    return call(yg, p, api, t, n2, co2, h2s);
  }
};

extern Quantity<SCF_STB> RsGlaso__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
    const Quantity<Fahrenheit>& t, const Quantity<MoleFraction>& n2,
    const Quantity<MoleFraction>& co2, const Quantity<MoleFraction>& h2s);
extern double RsGlaso__correlation__valid__fct(
    const double& yg, const double& p, const double& api, const double& t,
    const double& n2, const double& co2, const double& h2s);

/** RsHanafy

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group

Rs is determined by rearranging the bubble point pressure correlation to solve
for the solution gas-oil ratio.

References:

  - hanafy:1997

@param[in] p Pressure. Range in [, ]
@return inside [7, 4272]
@ingroup Correlations
*/
class RsHanafy : public SolutionGasOilRelation {
  Correlation_Singleton(RsHanafy);

  RsHanafy()
      : SolutionGasOilRelation("RsHanafy", SCF_STB::get_instance(), 7, 4272) {
    set_author("Hanafy et al.");
    set_title("HANAFY ET AL. CORRELATION, CALCULATION OF SOLUTION GOR (FOR "
              "PRESSURES BELOW THE BUBBLE POINT)");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_db("Based on experimental PVT data of 324 fluid samples taken from 176 "
           "wells located in 75 fields. This data represents 15 productive "
           "zones of 123 reservoirs distributed along three different regions "
           "of Egypt, including the Gulf of Suez, Western Desert, and Sinai.");
    add_note("Rs is determined by rearranging the bubble point pressure "
             "correlation to solve for the solution gas-oil ratio.");
    add_ref("hanafy:1997");
  }

public:
  static inline double impl(const double& p) noexcept;

  Quantity<SCF_STB> call(const Quantity<psia>& p) const {
    return Quantity<SCF_STB>(impl(p.raw()));
  }

  Quantity<SCF_STB> validate_and_call(const Quantity<psia>& p) const {

    return call(p);
  }

  Quantity<SCF_STB> operator()(const Quantity<psia>& p) const {
    return Quantity<SCF_STB>(call(p));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& p = it.get_curr();

    return call(p);
  }
};

extern Quantity<SCF_STB> RsHanafy__correlation__fct(const Quantity<psia>& p);
extern double RsHanafy__correlation__valid__fct(const double& p);

/** RsKartoatmodjoSchmidt

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group


References:

  - kartoatmodjo:1991

@param[in] yg Gas specific gravity. Range in [0.4824, 1.668]
@param[in] p Pressure. Range in [, ]
@param[in] api API oil gravity. Range in [14.4, 59]
@param[in] t Temperature. Range in [75, 320]
@param[in] tsep Separator temperature. Range in [38, 294]
@param[in] psep Separator pressure. Range in [14.7, 1414.7]
@return inside [0, 2890]
@ingroup Correlations
*/
class RsKartoatmodjoSchmidt : public SolutionGasOilRelation {
  Correlation_Singleton(RsKartoatmodjoSchmidt);

  RsKartoatmodjoSchmidt()
      : SolutionGasOilRelation("RsKartoatmodjoSchmidt", SCF_STB::get_instance(),
                               0, 2890) {
    set_author("Kartoatmodjo & Schmidt");
    set_title("KARTOATMODJO & SCHMIDT CORRELATION, CALCULATION OF SOLUTION GOR "
              "(FOR PRESSURES BELOW THE BUBBLE POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.4824,
                  1.668);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
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
  static inline double impl(const double& yg, const double& p,
                            const double& api, const double& t,
                            const double& tsep, const double& psep) noexcept;

  Quantity<SCF_STB> call(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                         const Quantity<Api>& api,
                         const Quantity<Fahrenheit>& t,
                         const Quantity<Fahrenheit>& tsep,
                         const Quantity<psia>& psep) const {
    return Quantity<SCF_STB>(
        impl(yg.raw(), p.raw(), api.raw(), t.raw(), tsep.raw(), psep.raw()));
  }

  Quantity<SCF_STB> validate_and_call(const Quantity<Sgg>& yg,
                                      const Quantity<psia>& p,
                                      const Quantity<Api>& api,
                                      const Quantity<Fahrenheit>& t,
                                      const Quantity<Fahrenheit>& tsep,
                                      const Quantity<psia>& psep) const {
    if (not(yg >= 0.4824))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.4824");
    if (not(yg <= 1.668))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.668");
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

    return call(yg, p, api, t, tsep, psep);
  }

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<Fahrenheit>& tsep,
                               const Quantity<psia>& psep) const {
    return Quantity<SCF_STB>(call(yg, p, api, t, tsep, psep));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& tsep = it.get_curr();
    it.next();
    const auto& psep = it.get_curr();

    return call(yg, p, api, t, tsep, psep);
  }
};

extern Quantity<SCF_STB> RsKartoatmodjoSchmidt__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
    const Quantity<Fahrenheit>& t, const Quantity<Fahrenheit>& tsep,
    const Quantity<psia>& psep);
extern double RsKartoatmodjoSchmidt__correlation__valid__fct(
    const double& yg, const double& p, const double& api, const double& t,
    const double& tsep, const double& psep);

/** RsLasater

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group

This correlation is best suited for oils with gravity up to 30 °API.

References:

  - lasater:1958

  - beggs:1987

  - baker:2015

@param[in] yg Gas specific gravity. Range in [0.574, 1.223]
@param[in] p Pressure. Range in [, ]
@param[in] yo Oil specific gravity. Range in [Quantity<Api>(51.1),
Quantity<Api>(17.9)]
@param[in] t Temperature. Range in [Quantity<Fahrenheit>(82),
Quantity<Fahrenheit>(272)]
@return inside [, ]
@ingroup Correlations
*/
class RsLasater : public SolutionGasOilRelation {
  Correlation_Singleton(RsLasater);

  RsLasater() : SolutionGasOilRelation("RsLasater", SCF_STB::get_instance()) {
    set_author("Lasater");
    set_title("LASATER CORRELATION, CALCULATION OF SOLUTION GOR (FOR PRESSURES "
              "BELOW THE BUBBLE POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.574,
                  1.223);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(51.1), Quantity<Api>(17.9));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("t", Rankine::get_instance(), "Temperature",
                  Quantity<Fahrenheit>(82), Quantity<Fahrenheit>(272));
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 158 experimentally measured bubble point pressures of 137 "
           "independent systems.");
    add_db("The data were obtained from Canada, Western and Mid-Continental "
           "United States, and South America.");
    add_note(
        "This correlation is best suited for oils with gravity up to 30 °API.");
    add_ref("lasater:1958");
    add_ref("beggs:1987");
    add_ref("baker:2015");
    add_par_synonym("yo", "api", "Api");
  }

public:
  static inline double impl(const double& yg, const double& p, const double& yo,
                            const double& t) noexcept;

  Quantity<SCF_STB> call(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                         const Quantity<Sg_do>& yo,
                         const Quantity<Rankine>& t) const {
    return Quantity<SCF_STB>(impl(yg.raw(), p.raw(), yo.raw(), t.raw()));
  }

  Quantity<SCF_STB> validate_and_call(const Quantity<Sgg>& yg,
                                      const Quantity<psia>& p,
                                      const Quantity<Sg_do>& yo,
                                      const Quantity<Rankine>& t) const {
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

    return call(yg, p, yo, t);
  }

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Sg_do>& yo,
                               const Quantity<Rankine>& t) const {
    return Quantity<SCF_STB>(call(yg, p, yo, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, p, yo, t);
  }
};

extern Quantity<SCF_STB> RsLasater__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Sg_do>& yo,
    const Quantity<Rankine>& t);
extern double RsLasater__correlation__valid__fct(const double& yg,
                                                 const double& p,
                                                 const double& yo,
                                                 const double& t);

/** RsPetroskyFarshad

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group


References:

  - petrosky:1993

@param[in] yg Gas specific gravity. Range in [0.5781, 0.8519]
@param[in] p Pressure. Range in [, ]
@param[in] api API oil gravity. Range in [16.3, 45]
@param[in] t Temperature. Range in [114, 288]
@return inside [217, 1406]
@ingroup Correlations
*/
class RsPetroskyFarshad : public SolutionGasOilRelation {
  Correlation_Singleton(RsPetroskyFarshad);

  RsPetroskyFarshad()
      : SolutionGasOilRelation("RsPetroskyFarshad", SCF_STB::get_instance(),
                               217, 1406) {
    set_author("Petrosky & Farshad");
    set_title("PETROSKY & FARSHAD CORRELATION, CALCULATION OF SOLUTION GOR "
              "(FOR PRESSURES BELOW THE BUBBLE POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.5781,
                  0.8519);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
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
  static inline double impl(const double& yg, const double& p,
                            const double& api, const double& t) noexcept;

  Quantity<SCF_STB> call(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                         const Quantity<Api>& api,
                         const Quantity<Fahrenheit>& t) const {
    return Quantity<SCF_STB>(impl(yg.raw(), p.raw(), api.raw(), t.raw()));
  }

  Quantity<SCF_STB> validate_and_call(const Quantity<Sgg>& yg,
                                      const Quantity<psia>& p,
                                      const Quantity<Api>& api,
                                      const Quantity<Fahrenheit>& t) const {
    if (not(yg >= 0.5781))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.5781");
    if (not(yg <= 0.8519))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 0.8519");
    if (not(api >= 16.3))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 16.3");
    if (not(api <= 45))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 45");

    return call(yg, p, api, t);
  }

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t) const {
    return Quantity<SCF_STB>(call(yg, p, api, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, p, api, t);
  }
};

extern Quantity<SCF_STB> RsPetroskyFarshad__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
    const Quantity<Fahrenheit>& t);
extern double RsPetroskyFarshad__correlation__valid__fct(const double& yg,
                                                         const double& p,
                                                         const double& api,
                                                         const double& t);

/** RsStanding

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group


References:

  - standing:1947

@param[in] yg Gas specific gravity. Range in [0.59, 0.95]
@param[in] p Pressure. Range in [, ]
@param[in] api API oil gravity. Range in [16.5, 63.8]
@param[in] t Temperature. Range in [100, 258]
@return inside [20, 1425]
@ingroup Correlations
*/
class RsStanding : public SolutionGasOilRelation {
  Correlation_Singleton(RsStanding);

  RsStanding()
      : SolutionGasOilRelation("RsStanding", SCF_STB::get_instance(), 20,
                               1425) {
    set_author("Standing");
    set_title("STANDING CORRELATION, CALCULATION OF SOLUTION GOR (FOR "
              "PRESSURES BELOW THE BUBBLE POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.59,
                  0.95);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("api", Api::get_instance(), "API oil gravity", 16.5, 63.8);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 100, 258);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 105 experimentally determined bubble point pressures from "
           "22 different Californian crude-oil-natural-gas mixtures.");
    add_ref("standing:1947");
  }

public:
  static inline double impl(const double& yg, const double& p,
                            const double& api, const double& t) noexcept;

  Quantity<SCF_STB> call(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                         const Quantity<Api>& api,
                         const Quantity<Fahrenheit>& t) const {
    return Quantity<SCF_STB>(impl(yg.raw(), p.raw(), api.raw(), t.raw()));
  }

  Quantity<SCF_STB> validate_and_call(const Quantity<Sgg>& yg,
                                      const Quantity<psia>& p,
                                      const Quantity<Api>& api,
                                      const Quantity<Fahrenheit>& t) const {
    if (not(yg >= 0.59))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.59");
    if (not(yg <= 0.95))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 0.95");
    if (not(api >= 16.5))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 16.5");
    if (not(api <= 63.8))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 63.8");

    return call(yg, p, api, t);
  }

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t) const {
    return Quantity<SCF_STB>(call(yg, p, api, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, p, api, t);
  }
};

extern Quantity<SCF_STB> RsStanding__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
    const Quantity<Fahrenheit>& t);
extern double RsStanding__correlation__valid__fct(const double& yg,
                                                  const double& p,
                                                  const double& api,
                                                  const double& t);

/** RsTotalCFPDetachedFromPb

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group


References:

  - totalCFP:1983

  - banzer:1996

@param[in] yg Gas specific gravity. Range in [, ]
@param[in] p Pressure. Range in [, ]
@param[in] api API oil gravity. Range in [Api::get_instance().min_val, 45]
@param[in] t Temperature. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class RsTotalCFPDetachedFromPb : public SolutionGasOilRelation {
  Correlation_Singleton(RsTotalCFPDetachedFromPb);

  RsTotalCFPDetachedFromPb()
      : SolutionGasOilRelation("RsTotalCFPDetachedFromPb",
                               SCF_STB::get_instance()) {
    set_author("Total CFP");
    set_title("TOTAL COMPAGNIE FRANÇAISE DES PÉTROLES (CFP) CORRELATION, "
              "CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE "
              "POINT)");
    set_hidden();
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity");
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
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
  static inline double impl(const double& yg, const double& p,
                            const double& api, const double& t) noexcept;

  Quantity<SCF_STB> call(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                         const Quantity<Api>& api,
                         const Quantity<Fahrenheit>& t) const {
    return Quantity<SCF_STB>(impl(yg.raw(), p.raw(), api.raw(), t.raw()));
  }

  Quantity<SCF_STB> validate_and_call(const Quantity<Sgg>& yg,
                                      const Quantity<psia>& p,
                                      const Quantity<Api>& api,
                                      const Quantity<Fahrenheit>& t) const {
    if (not(api >= Api::get_instance().min_val))
      ZENTHROW(OutOfParameterRange,
               "parameter api is less than Api::get_instance().min_val");
    if (not(api <= 45))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 45");

    return call(yg, p, api, t);
  }

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t) const {
    return Quantity<SCF_STB>(call(yg, p, api, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, p, api, t);
  }
};

extern Quantity<SCF_STB> RsTotalCFPDetachedFromPb__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
    const Quantity<Fahrenheit>& t);
extern double RsTotalCFPDetachedFromPb__correlation__valid__fct(
    const double& yg, const double& p, const double& api, const double& t);

/** RsTotalCFP

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group

Total CFP presented separate equations for solution gas oil ratio (Rs) and
bubble point pressure (Pb). The Pb equation provides more realistic results;
therefore, Rs is determined by rearranging the bubble point pressure correlation
to solve for the solution gas-oil ratio.

References:

  - totalCFP:1983

  - banzer:1996

  - lopez:2014

@param[in] yg Gas specific gravity. Range in [, ]
@param[in] p Pressure. Range in [, ]
@param[in] api API oil gravity. Range in [Api::get_instance().min_val, 45]
@param[in] t Temperature. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class RsTotalCFP : public SolutionGasOilRelation {
  Correlation_Singleton(RsTotalCFP);

  RsTotalCFP() : SolutionGasOilRelation("RsTotalCFP", SCF_STB::get_instance()) {
    set_author("Total CFP");
    set_title("TOTAL COMPAGNIE FRANÇAISE DES PÉTROLES (CFP) CORRELATION, "
              "CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE "
              "POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity");
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("api", Api::get_instance(), "API oil gravity",
                  Api::get_instance().min_val, 45);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature");
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 336 PVT analyses of crude oil from eastern Venezuela.");
    add_note("Total CFP presented separate equations for solution gas oil "
             "ratio (Rs) and bubble point pressure (Pb). The Pb equation "
             "provides more realistic results; therefore, Rs is determined by "
             "rearranging the bubble point pressure correlation to solve for "
             "the solution gas-oil ratio.");
    add_ref("totalCFP:1983");
    add_ref("banzer:1996");
    add_ref("lopez:2014");
  }

public:
  static inline double impl(const double& yg, const double& p,
                            const double& api, const double& t) noexcept;

  Quantity<SCF_STB> call(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                         const Quantity<Api>& api,
                         const Quantity<Fahrenheit>& t) const {
    return Quantity<SCF_STB>(impl(yg.raw(), p.raw(), api.raw(), t.raw()));
  }

  Quantity<SCF_STB> validate_and_call(const Quantity<Sgg>& yg,
                                      const Quantity<psia>& p,
                                      const Quantity<Api>& api,
                                      const Quantity<Fahrenheit>& t) const {
    if (not(api >= Api::get_instance().min_val))
      ZENTHROW(OutOfParameterRange,
               "parameter api is less than Api::get_instance().min_val");
    if (not(api <= 45))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 45");

    return call(yg, p, api, t);
  }

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t) const {
    return Quantity<SCF_STB>(call(yg, p, api, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, p, api, t);
  }
};

extern Quantity<SCF_STB> RsTotalCFP__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
    const Quantity<Fahrenheit>& t);
extern double RsTotalCFP__correlation__valid__fct(const double& yg,
                                                  const double& p,
                                                  const double& api,
                                                  const double& t);

/** RsVasquezBeggs

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group


The gas gravity is corrected if it resulted from a first-stage separation at a
pressure other than 100 psig (approximately 114.7 psia).

References:

  - vazquez:1980

  - alShammasi:2001

@param[in] yg Gas specific gravity. Range in [0.511, 1.35]
@param[in] p Pressure. Range in [, ]
@param[in] api API oil gravity. Range in [15.3, 59.3]
@param[in] t Temperature. Range in [75, 294]
@param[in] tsep Separator temperature. Range in [76, 150]
@param[in] psep Separator pressure. Range in [60, 565]
@return inside [0, 2199]
@ingroup Correlations
*/
class RsVasquezBeggs : public SolutionGasOilRelation {
  Correlation_Singleton(RsVasquezBeggs);

  RsVasquezBeggs()
      : SolutionGasOilRelation("RsVasquezBeggs", SCF_STB::get_instance(), 0,
                               2199) {
    set_author("Vazquez & Beggs");
    set_title("VAZQUEZ & BEGGS CORRELATION, CALCULATION OF SOLUTION GOR (FOR "
              "PRESSURES BELOW THE BUBBLE POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.511,
                  1.35);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("api", Api::get_instance(), "API oil gravity", 15.3, 59.3);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 75, 294);
    def_parameter_latex_symbol("t", "T");
    add_parameter("tsep", Fahrenheit::get_instance(), "Separator temperature",
                  76, 150);
    def_parameter_latex_symbol("tsep", "T_{sp}");
    add_parameter("psep", psia::get_instance(), "Separator pressure", 60, 565);
    def_parameter_latex_symbol("psep", "P_{sp}");
    add_db("Based on more than 600 laboratory PVT analyses from fields all "
           "over the world.");
    add_db("The equation for solution GOR was obtained by regression analysis "
           "using 5008 measured data points.");
    add_ref("vazquez:1980");
    add_ref("alShammasi:2001");
  }

public:
  static inline double impl(const double& yg, const double& p,
                            const double& api, const double& t,
                            const double& tsep, const double& psep) noexcept;

  Quantity<SCF_STB> call(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                         const Quantity<Api>& api,
                         const Quantity<Fahrenheit>& t,
                         const Quantity<Fahrenheit>& tsep,
                         const Quantity<psia>& psep) const {
    return Quantity<SCF_STB>(
        impl(yg.raw(), p.raw(), api.raw(), t.raw(), tsep.raw(), psep.raw()));
  }

  Quantity<SCF_STB> validate_and_call(const Quantity<Sgg>& yg,
                                      const Quantity<psia>& p,
                                      const Quantity<Api>& api,
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
    if (not(tsep >= 76))
      ZENTHROW(OutOfParameterRange, "parameter tsep is less than 76");
    if (not(tsep <= 150))
      ZENTHROW(OutOfParameterRange, "parameter tsep is greater than 150");
    if (not(psep >= 60))
      ZENTHROW(OutOfParameterRange, "parameter psep is less than 60");
    if (not(psep <= 565))
      ZENTHROW(OutOfParameterRange, "parameter psep is greater than 565");

    return call(yg, p, api, t, tsep, psep);
  }

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<Fahrenheit>& tsep,
                               const Quantity<psia>& psep) const {
    return Quantity<SCF_STB>(call(yg, p, api, t, tsep, psep));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& tsep = it.get_curr();
    it.next();
    const auto& psep = it.get_curr();

    return call(yg, p, api, t, tsep, psep);
  }
};

extern Quantity<SCF_STB> RsVasquezBeggs__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
    const Quantity<Fahrenheit>& t, const Quantity<Fahrenheit>& tsep,
    const Quantity<psia>& psep);
extern double RsVasquezBeggs__correlation__valid__fct(
    const double& yg, const double& p, const double& api, const double& t,
    const double& tsep, const double& psep);

/** RsVelarde

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group


References:

  - velarde:1997

@param[in] yg Gas specific gravity. Range in [0.561, 1.101]
@param[in] pb Bubble point pressure. Range in [Quantity<psia>(106),
Quantity<psia>(5312)]
@param[in] p Pressure. Range in [, ]
@param[in] api API oil gravity. Range in [11.6, 53.4]
@param[in] t Temperature. Range in [70, 307]
@param[in] rsb Solution GOR at Pb. Range in [102, 1808]
@return inside [, ]
@ingroup Correlations
*/
class RsVelarde : public SolutionGasOilRelation {
  Correlation_Singleton(RsVelarde);

  RsVelarde() : SolutionGasOilRelation("RsVelarde", SCF_STB::get_instance()) {
    set_author("Velarde, Blasingame & McCain");
    set_title("VELARDE, BLASINGAME & McCAIN CORRELATION, CALCULATION OF "
              "SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.561,
                  1.101);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("pb", psig::get_instance(), "Bubble point pressure",
                  Quantity<psia>(106), Quantity<psia>(5312));
    def_parameter_latex_symbol("pb", "P_b");
    add_parameter("p", psig::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("api", Api::get_instance(), "API oil gravity", 11.6, 53.4);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 70, 307);
    def_parameter_latex_symbol("t", "T");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 102,
                  1808);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_db("Based on 195 PVT reports, including 2097 data sets.");
    add_ref("velarde:1997");
  }

public:
  static inline double impl(const double& yg, const double& pb, const double& p,
                            const double& api, const double& t,
                            const double& rsb) noexcept;

  Quantity<SCF_STB> call(const Quantity<Sgg>& yg, const Quantity<psig>& pb,
                         const Quantity<psig>& p, const Quantity<Api>& api,
                         const Quantity<Fahrenheit>& t,
                         const Quantity<SCF_STB>& rsb) const {
    return Quantity<SCF_STB>(
        impl(yg.raw(), pb.raw(), p.raw(), api.raw(), t.raw(), rsb.raw()));
  }

  Quantity<SCF_STB> validate_and_call(const Quantity<Sgg>& yg,
                                      const Quantity<psig>& pb,
                                      const Quantity<psig>& p,
                                      const Quantity<Api>& api,
                                      const Quantity<Fahrenheit>& t,
                                      const Quantity<SCF_STB>& rsb) const {
    if (not(yg >= 0.561))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.561");
    if (not(yg <= 1.101))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.101");
    if (not(pb >= Quantity<psia>(106)))
      ZENTHROW(OutOfParameterRange,
               "parameter pb is less than Quantity<psia>(106)");
    if (not(pb <= Quantity<psia>(5312)))
      ZENTHROW(OutOfParameterRange,
               "parameter pb is greater than Quantity<psia>(5312)");
    if (not(api >= 11.6))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 11.6");
    if (not(api <= 53.4))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 53.4");
    if (not(rsb >= 102))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 102");
    if (not(rsb <= 1808))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 1808");

    return call(yg, pb, p, api, t, rsb);
  }

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg,
                               const Quantity<psig>& pb,
                               const Quantity<psig>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<SCF_STB>& rsb) const {
    return Quantity<SCF_STB>(call(yg, pb, p, api, t, rsb));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();

    return call(yg, pb, p, api, t, rsb);
  }
};

extern Quantity<SCF_STB> RsVelarde__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psig>& pb, const Quantity<psig>& p,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t,
    const Quantity<SCF_STB>& rsb);
extern double RsVelarde__correlation__valid__fct(
    const double& yg, const double& pb, const double& p, const double& api,
    const double& t, const double& rsb);

/** RsCegarra

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group


The correlation could not be verified because the original reference is not
available. Date: August 16 2016.

References:

  - cegarra:2007

  - standing:1947

@param[in] yg Gas specific gravity. Range in [0.654, 1.337]
@param[in] p Pressure. Range in [, ]
@param[in] api API oil gravity. Range in [22, 41.9]
@param[in] t Temperature. Range in [117, 354]
@return inside [135, 4290]
@ingroup Correlations
*/
class RsCegarra : public SolutionGasOilRelation {
  Correlation_Singleton(RsCegarra);

  RsCegarra()
      : SolutionGasOilRelation("RsCegarra", SCF_STB::get_instance(), 135,
                               4290) {
    set_author("Cegarra");
    set_title("CEGARRA CORRELATION (MODIFIED STANDING CORRELATION), "
              "CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE "
              "POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.654,
                  1.337);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("api", Api::get_instance(), "API oil gravity", 22, 41.9);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 117, 354);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 375 consistent PVT analyses. The data bank was compiled "
           "from 8394 fields of eastern Venezuela subdivided into 5192 fields "
           "of light oil and 3202 fields of medium oil.");
    add_db("The correlation takes the functional form of Standing.");
    add_ref("cegarra:2007");
    add_ref("standing:1947");
  }

public:
  static inline double impl(const double& yg, const double& p,
                            const double& api, const double& t) noexcept;

  Quantity<SCF_STB> call(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                         const Quantity<Api>& api,
                         const Quantity<Fahrenheit>& t) const {
    return Quantity<SCF_STB>(impl(yg.raw(), p.raw(), api.raw(), t.raw()));
  }

  Quantity<SCF_STB> validate_and_call(const Quantity<Sgg>& yg,
                                      const Quantity<psia>& p,
                                      const Quantity<Api>& api,
                                      const Quantity<Fahrenheit>& t) const {
    if (not(yg >= 0.654))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.654");
    if (not(yg <= 1.337))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.337");
    if (not(api >= 22))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 22");
    if (not(api <= 41.9))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 41.9");

    return call(yg, p, api, t);
  }

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t) const {
    return Quantity<SCF_STB>(call(yg, p, api, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, p, api, t);
  }
};

extern Quantity<SCF_STB> RsCegarra__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
    const Quantity<Fahrenheit>& t);
extern double RsCegarra__correlation__valid__fct(const double& yg,
                                                 const double& p,
                                                 const double& api,
                                                 const double& t);

/** RsPerezML

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group


The correlation was verified by using the original reference (Pérez, Heny &
Lago, 2001). Date: October 02 2017.

References:

  - perez:2001

@param[in] pb Bubble point pressure. Range in [405, 1335]
@param[in] p Pressure. Range in [, ]
@param[in] rsb Solution GOR at Pb. Range in [38, 121]
@return inside [38, 121]
@ingroup Correlations
*/
class RsPerezML : public SolutionGasOilRelation {
  Correlation_Singleton(RsPerezML);

  RsPerezML()
      : SolutionGasOilRelation("RsPerezML", SCF_STB::get_instance(), 38, 121) {
    set_author("Pérez, Heny & Lago");
    set_title("PÉREZ, HENY & LAGO CORRELATION (MODIFIED MILLÁN-ARCIA "
              "CORRELATION), CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW "
              "THE BUBBLE POINT)");
    add_parameter("pb", psia::get_instance(), "Bubble point pressure", 405,
                  1335);
    def_parameter_latex_symbol("pb", "P_b");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 38,
                  121);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_db("Based on 35 laboratory PVT analyses of extra-heavy oil samples "
           "from the Orinoco Oil Belt, specifically: 16 from Hamaca, 14 from "
           "Zuata, three from Cerro Negro, one from Melones, and one from "
           "Boscán.");
    add_ref("perez:2001");
  }

public:
  static inline double impl(const double& pb, const double& p,
                            const double& rsb) noexcept;

  Quantity<SCF_STB> call(const Quantity<psia>& pb, const Quantity<psia>& p,
                         const Quantity<SCF_STB>& rsb) const {
    return Quantity<SCF_STB>(impl(pb.raw(), p.raw(), rsb.raw()));
  }

  Quantity<SCF_STB> validate_and_call(const Quantity<psia>& pb,
                                      const Quantity<psia>& p,
                                      const Quantity<SCF_STB>& rsb) const {
    if (not(pb >= 405))
      ZENTHROW(OutOfParameterRange, "parameter pb is less than 405");
    if (not(pb <= 1335))
      ZENTHROW(OutOfParameterRange, "parameter pb is greater than 1335");
    if (not(rsb >= 38))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 38");
    if (not(rsb <= 121))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 121");

    return call(pb, p, rsb);
  }

  Quantity<SCF_STB> operator()(const Quantity<psia>& pb,
                               const Quantity<psia>& p,
                               const Quantity<SCF_STB>& rsb) const {
    return Quantity<SCF_STB>(call(pb, p, rsb));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& pb = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();

    return call(pb, p, rsb);
  }
};

extern Quantity<SCF_STB> RsPerezML__correlation__fct(
    const Quantity<psia>& pb, const Quantity<psia>& p,
    const Quantity<SCF_STB>& rsb);
extern double RsPerezML__correlation__valid__fct(const double& pb,
                                                 const double& p,
                                                 const double& rsb);

/** RsMillanArciaDetachedFromPb

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group


The correlation could not be verified because the original reference is not
available. Date: August 16 2016.

References:

  - millan:1984

  - perez:2001

@param[in] pb Bubble point pressure. Range in [222, 3432.7]
@param[in] p Pressure. Range in [, ]
@param[in] rsb Solution GOR at Pb. Range in [, ]
@return inside [53, 459]
@ingroup Correlations
*/
class RsMillanArciaDetachedFromPb : public SolutionGasOilRelation {
  Correlation_Singleton(RsMillanArciaDetachedFromPb);

  RsMillanArciaDetachedFromPb()
      : SolutionGasOilRelation("RsMillanArciaDetachedFromPb",
                               SCF_STB::get_instance(), 53, 459) {
    set_author("Millán-Arcia");
    set_title("MILLÁN-ARCIA CORRELATION, CALCULATION OF SOLUTION GOR (FOR "
              "PRESSURES BELOW THE BUBBLE POINT)");
    set_hidden();
    add_parameter("pb", psia::get_instance(), "Bubble point pressure", 222,
                  3432.7);
    def_parameter_latex_symbol("pb", "P_b");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb");
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_db("Venezuelan heavy crudes were correlated.");
    add_ref("millan:1984");
    add_ref("perez:2001");
  }

public:
  static inline double impl(const double& pb, const double& p,
                            const double& rsb) noexcept;

  Quantity<SCF_STB> call(const Quantity<psia>& pb, const Quantity<psia>& p,
                         const Quantity<SCF_STB>& rsb) const {
    return Quantity<SCF_STB>(impl(pb.raw(), p.raw(), rsb.raw()));
  }

  Quantity<SCF_STB> validate_and_call(const Quantity<psia>& pb,
                                      const Quantity<psia>& p,
                                      const Quantity<SCF_STB>& rsb) const {
    if (not(pb >= 222))
      ZENTHROW(OutOfParameterRange, "parameter pb is less than 222");
    if (not(pb <= 3432.7))
      ZENTHROW(OutOfParameterRange, "parameter pb is greater than 3432.7");

    return call(pb, p, rsb);
  }

  Quantity<SCF_STB> operator()(const Quantity<psia>& pb,
                               const Quantity<psia>& p,
                               const Quantity<SCF_STB>& rsb) const {
    return Quantity<SCF_STB>(call(pb, p, rsb));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& pb = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();

    return call(pb, p, rsb);
  }
};

extern Quantity<SCF_STB> RsMillanArciaDetachedFromPb__correlation__fct(
    const Quantity<psia>& pb, const Quantity<psia>& p,
    const Quantity<SCF_STB>& rsb);
extern double RsMillanArciaDetachedFromPb__correlation__valid__fct(
    const double& pb, const double& p, const double& rsb);

/** RsMillanArcia

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group

Millán-Arcia presented separate equations for solution gas oil ratio (Rs) and
bubble point pressure (Pb). The correlation for Pb is used to calculate Rs by
rearranging the equation.

The correlation was verified by using a secondary reference (Salazar, 2008).
Date: July 27 2017.

References:

  - millan:1984

  - perez:2001

@param[in] yg Gas specific gravity. Range in [, ]
@param[in] p Pressure. Range in [222, 3432.7]
@param[in] api API oil gravity. Range in [9, 20.2]
@param[in] t Temperature. Range in [87, 195]
@return inside [53, 459]
@ingroup Correlations
*/
class RsMillanArcia : public SolutionGasOilRelation {
  Correlation_Singleton(RsMillanArcia);

  RsMillanArcia()
      : SolutionGasOilRelation("RsMillanArcia", SCF_STB::get_instance(), 53,
                               459) {
    set_author("Millán-Arcia");
    set_title("MILLÁN-ARCIA CORRELATION, CALCULATION OF SOLUTION GOR (FOR "
              "PRESSURES BELOW THE BUBBLE POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity");
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure", 222, 3432.7);
    def_parameter_latex_symbol("p", "P");
    add_parameter("api", Api::get_instance(), "API oil gravity", 9, 20.2);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 87, 195);
    def_parameter_latex_symbol("t", "T");
    add_db("Venezuelan heavy crudes were correlated.");
    add_note("Millán-Arcia presented separate equations for solution gas oil "
             "ratio (Rs) and bubble point pressure (Pb). The correlation for "
             "Pb is used to calculate Rs by rearranging the equation.");
    add_ref("millan:1984");
    add_ref("perez:2001");
  }

public:
  static inline double impl(const double& yg, const double& p,
                            const double& api, const double& t) noexcept;

  Quantity<SCF_STB> call(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                         const Quantity<Api>& api,
                         const Quantity<Fahrenheit>& t) const {
    return Quantity<SCF_STB>(impl(yg.raw(), p.raw(), api.raw(), t.raw()));
  }

  Quantity<SCF_STB> validate_and_call(const Quantity<Sgg>& yg,
                                      const Quantity<psia>& p,
                                      const Quantity<Api>& api,
                                      const Quantity<Fahrenheit>& t) const {
    if (not(api >= 9))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 9");
    if (not(api <= 20.2))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 20.2");

    return call(yg, p, api, t);
  }

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t) const {
    return Quantity<SCF_STB>(call(yg, p, api, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, p, api, t);
  }
};

extern Quantity<SCF_STB> RsMillanArcia__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
    const Quantity<Fahrenheit>& t);
extern double RsMillanArcia__correlation__valid__fct(const double& yg,
                                                     const double& p,
                                                     const double& api,
                                                     const double& t);

/** RsManucciRosales

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group


The correlation could not be verified because the original reference is not
available. Date: August 16 2016.

References:

  - manucci:1968

  - perez:2001

@param[in] yg Gas specific gravity. Range in [, ]
@param[in] p Pressure. Range in [, ]
@param[in] api API oil gravity. Range in [14, 40]
@param[in] t Temperature. Range in [165, 304]
@return inside [180, 2500]
@ingroup Correlations
*/
class RsManucciRosales : public SolutionGasOilRelation {
  Correlation_Singleton(RsManucciRosales);

  RsManucciRosales()
      : SolutionGasOilRelation("RsManucciRosales", SCF_STB::get_instance(), 180,
                               2500) {
    set_author("Manucci & Rosales");
    set_title("MANUCCI & ROSALES CORRELATION, CALCULATION OF SOLUTION GOR (FOR "
              "PRESSURES BELOW THE BUBBLE POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity");
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
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
  static inline double impl(const double& yg, const double& p,
                            const double& api, const double& t) noexcept;

  Quantity<SCF_STB> call(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                         const Quantity<Api>& api,
                         const Quantity<Fahrenheit>& t) const {
    return Quantity<SCF_STB>(impl(yg.raw(), p.raw(), api.raw(), t.raw()));
  }

  Quantity<SCF_STB> validate_and_call(const Quantity<Sgg>& yg,
                                      const Quantity<psia>& p,
                                      const Quantity<Api>& api,
                                      const Quantity<Fahrenheit>& t) const {
    if (not(api >= 14))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 14");
    if (not(api <= 40))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 40");

    return call(yg, p, api, t);
  }

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t) const {
    return Quantity<SCF_STB>(call(yg, p, api, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, p, api, t);
  }
};

extern Quantity<SCF_STB> RsManucciRosales__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
    const Quantity<Fahrenheit>& t);
extern double RsManucciRosales__correlation__valid__fct(const double& yg,
                                                        const double& p,
                                                        const double& api,
                                                        const double& t);

/** RsLopezCR

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group


The correlation was verified by using the original reference. Date: April 27
2017.

References:

  - lopez:2014

@param[in] yg Gas specific gravity. Range in [0.5086, 0.75]
@param[in] p Pressure. Range in [100, 1315]
@param[in] api API oil gravity. Range in [7.4, 14.5]
@param[in] t Temperature. Range in [124, 220]
@return inside [12, 110]
@ingroup Correlations
*/
class RsLopezCR : public SolutionGasOilRelation {
  Correlation_Singleton(RsLopezCR);

  RsLopezCR()
      : SolutionGasOilRelation("RsLopezCR", SCF_STB::get_instance(), 12, 110) {
    set_author("López, Castillo & Rojas");
    set_title("LÓPEZ, CASTILLO & ROJAS CORRELATION, CALCULATION OF SOLUTION "
              "GOR (FOR PRESSURES BELOW THE BUBBLE POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.5086,
                  0.75);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure", 100, 1315);
    def_parameter_latex_symbol("p", "P");
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
  static inline double impl(const double& yg, const double& p,
                            const double& api, const double& t) noexcept;

  Quantity<SCF_STB> call(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                         const Quantity<Api>& api,
                         const Quantity<Fahrenheit>& t) const {
    return Quantity<SCF_STB>(impl(yg.raw(), p.raw(), api.raw(), t.raw()));
  }

  Quantity<SCF_STB> validate_and_call(const Quantity<Sgg>& yg,
                                      const Quantity<psia>& p,
                                      const Quantity<Api>& api,
                                      const Quantity<Fahrenheit>& t) const {
    if (not(yg >= 0.5086))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.5086");
    if (not(yg <= 0.75))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 0.75");
    if (not(api >= 7.4))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 7.4");
    if (not(api <= 14.5))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 14.5");

    return call(yg, p, api, t);
  }

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t) const {
    return Quantity<SCF_STB>(call(yg, p, api, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, p, api, t);
  }
};

extern Quantity<SCF_STB> RsLopezCR__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
    const Quantity<Fahrenheit>& t);
extern double RsLopezCR__correlation__valid__fct(const double& yg,
                                                 const double& p,
                                                 const double& api,
                                                 const double& t);

/** RsSalazar

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group

Salazar presented separate equations for solution gas oil ratio (Rs) and bubble
point pressure (Pb). The best Pb correlation provides a lower average error than
the different ones proposed for Rs; therefore, this correlation is used to
calculate Rs by rearranging the equation.

The correlation was verified by using the original reference. Date: April 28
2017.

References:

  - salazar:2008

@param[in] yg Gas specific gravity. Range in [0.5086, 0.75]
@param[in] p Pressure. Range in [253, 1564]
@param[in] api API oil gravity. Range in [7.1, 18.9]
@param[in] t Temperature. Range in [108, 169]
@return inside [26, 149]
@ingroup Correlations
*/
class RsSalazar : public SolutionGasOilRelation {
  Correlation_Singleton(RsSalazar);

  RsSalazar()
      : SolutionGasOilRelation("RsSalazar", SCF_STB::get_instance(), 26, 149) {
    set_author("Salazar");
    set_title("SALAZAR CORRELATION, CALCULATION OF SOLUTION GOR (FOR PRESSURES "
              "BELOW THE BUBBLE POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.5086,
                  0.75);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure", 253, 1564);
    def_parameter_latex_symbol("p", "P");
    add_parameter("api", Api::get_instance(), "API oil gravity", 7.1, 18.9);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 108, 169);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 136 PVT reports from different areas of the Faja "
           "Petrolífera del Orinoco: 60 from Ayacucho, 48 from Junín, 26 from "
           "Carabobo, and 2 from Boyacá.");
    add_note("Salazar presented separate equations for solution gas oil ratio "
             "(Rs) and bubble point pressure (Pb). The best Pb correlation "
             "provides a lower average error than the different ones proposed "
             "for Rs; therefore, this correlation is used to calculate Rs by "
             "rearranging the equation.");
    add_ref("salazar:2008");
  }

public:
  static inline double impl(const double& yg, const double& p,
                            const double& api, const double& t) noexcept;

  Quantity<SCF_STB> call(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                         const Quantity<Api>& api,
                         const Quantity<Fahrenheit>& t) const {
    return Quantity<SCF_STB>(impl(yg.raw(), p.raw(), api.raw(), t.raw()));
  }

  Quantity<SCF_STB> validate_and_call(const Quantity<Sgg>& yg,
                                      const Quantity<psia>& p,
                                      const Quantity<Api>& api,
                                      const Quantity<Fahrenheit>& t) const {
    if (not(yg >= 0.5086))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.5086");
    if (not(yg <= 0.75))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 0.75");
    if (not(api >= 7.1))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 7.1");
    if (not(api <= 18.9))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 18.9");

    return call(yg, p, api, t);
  }

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t) const {
    return Quantity<SCF_STB>(call(yg, p, api, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, p, api, t);
  }
};

extern Quantity<SCF_STB> RsSalazar__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
    const Quantity<Fahrenheit>& t);
extern double RsSalazar__correlation__valid__fct(const double& yg,
                                                 const double& p,
                                                 const double& api,
                                                 const double& t);

/** RsSalazarDetachedFromPb

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group

Salazar (2008) presented the correlation as model 1.

The correlation was verified by using the original reference. Date: April 28
2017.

References:

  - salazar:2008

@param[in] yg Gas specific gravity. Range in [0.5692, 0.693]
@param[in] p Pressure. Range in [65, 1940]
@param[in] api API oil gravity. Range in [7.8, 14.9]
@param[in] t Temperature. Range in [118, 220]
@return inside [25, 154]
@ingroup Correlations
*/
class RsSalazarDetachedFromPb : public SolutionGasOilRelation {
  Correlation_Singleton(RsSalazarDetachedFromPb);

  RsSalazarDetachedFromPb()
      : SolutionGasOilRelation("RsSalazarDetachedFromPb",
                               SCF_STB::get_instance(), 25, 154) {
    set_author("Salazar");
    set_title("SALAZAR CORRELATION, CALCULATION OF SOLUTION GOR (FOR PRESSURES "
              "BELOW THE BUBBLE POINT)");
    set_hidden();
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.5692,
                  0.693);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure", 65, 1940);
    def_parameter_latex_symbol("p", "P");
    add_parameter("api", Api::get_instance(), "API oil gravity", 7.8, 14.9);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 118, 220);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 136 PVT reports from different areas of the Faja "
           "Petrolífera del Orinoco: 60 from Ayacucho, 48 from Junín, 26 from "
           "Carabobo, and 2 from Boyacá.");
    add_note("Salazar (2008) presented the correlation as model 1.");
    add_ref("salazar:2008");
  }

public:
  static inline double impl(const double& yg, const double& p,
                            const double& api, const double& t) noexcept;

  Quantity<SCF_STB> call(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                         const Quantity<Api>& api,
                         const Quantity<Fahrenheit>& t) const {
    return Quantity<SCF_STB>(impl(yg.raw(), p.raw(), api.raw(), t.raw()));
  }

  Quantity<SCF_STB> validate_and_call(const Quantity<Sgg>& yg,
                                      const Quantity<psia>& p,
                                      const Quantity<Api>& api,
                                      const Quantity<Fahrenheit>& t) const {
    if (not(yg >= 0.5692))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.5692");
    if (not(yg <= 0.693))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 0.693");
    if (not(api >= 7.8))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 7.8");
    if (not(api <= 14.9))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 14.9");

    return call(yg, p, api, t);
  }

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t) const {
    return Quantity<SCF_STB>(call(yg, p, api, t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return call(yg, p, api, t);
  }
};

extern Quantity<SCF_STB> RsSalazarDetachedFromPb__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
    const Quantity<Fahrenheit>& t);
extern double RsSalazarDetachedFromPb__correlation__valid__fct(
    const double& yg, const double& p, const double& api, const double& t);

/** RsAbovePb

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group


References:

@param[in] pb Bubble point pressure. Range in [, ]
@param[in] p Pressure. Range in [, ]
@param[in] rsb Solution GOR at Pb. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class RsAbovePb : public SolutionGasOilRelation {
  Correlation_Singleton(RsAbovePb);

  RsAbovePb() : SolutionGasOilRelation("RsAbovePb", SCF_STB::get_instance()) {
    set_author("Not defined");
    set_title("Faked constant for internal computations above bubble point");
    set_hidden();
    add_parameter("pb", psia::get_instance(), "Bubble point pressure");
    def_parameter_latex_symbol("pb", "P_b");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb");
    def_parameter_latex_symbol("rsb", "R_{sbp}");
  }

  inline void precondition(const Quantity<psia>& p,
                           const Quantity<psia>& pb) const;

public:
  static inline double impl(const double& pb, const double& p,
                            const double& rsb) noexcept;

  Quantity<SCF_STB> call(const Quantity<psia>& pb, const Quantity<psia>& p,
                         const Quantity<SCF_STB>& rsb) const {
    precondition(p, pb);
    return Quantity<SCF_STB>(impl(pb.raw(), p.raw(), rsb.raw()));
  }

  Quantity<SCF_STB> validate_and_call(const Quantity<psia>& pb,
                                      const Quantity<psia>& p,
                                      const Quantity<SCF_STB>& rsb) const {

    return call(pb, p, rsb);
  }

  Quantity<SCF_STB> operator()(const Quantity<psia>& pb,
                               const Quantity<psia>& p,
                               const Quantity<SCF_STB>& rsb) const {
    return Quantity<SCF_STB>(call(pb, p, rsb));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& pb = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();

    return call(pb, p, rsb);
  }
};

extern Quantity<SCF_STB> RsAbovePb__correlation__fct(
    const Quantity<psia>& pb, const Quantity<psia>& p,
    const Quantity<SCF_STB>& rsb);
extern double RsAbovePb__correlation__valid__fct(const double& pb,
                                                 const double& p,
                                                 const double& rsb);

#endif // SOLUTION_GAS_OIL_RELATION_H
