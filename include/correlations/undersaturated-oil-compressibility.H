#ifndef UNDERSATURATED_OIL_COMPRESSIBILITY_H
#define UNDERSATURATED_OIL_COMPRESSIBILITY_H

#include <pvt-exceptions.H>
#include <correlations/correlation.H>

/** CoaDeGhetto

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group


The range for Rsb is presented by De Ghetto et al. as the range for Solution
GOR.

References:

  - deGhetto:1995

@param[in] yg Gas specific gravity. Range in [0.623, 1.517]
@param[in] api API oil gravity. Range in [6, 22.3]
@param[in] rsb Solution GOR at Pb. Range in [17.21, 640.25]
@param[in] t Temperature. Range in [131.4, 250.7]
@param[in] tsep Separator temperature. Range in [59, 177.8]
@param[in] p Pressure. Range in [1038.49, 7411.54]
@param[in] psep Separator pressure. Range in [14.5, 752.2]
@return inside [3.02e-06, 4.29e-05]
@ingroup Correlations
*/
class CoaDeGhetto : public UndersaturatedOilCompressibility {
  Correlation_Singleton(CoaDeGhetto);

  CoaDeGhetto()
      : UndersaturatedOilCompressibility("CoaDeGhetto", psia_1::get_instance(),
                                         3.02e-06, 4.29e-05) {
    set_author("De Ghetto, Paone & Villa");
    set_title("DE GHETTO, PAONE & VILLA CORRELATION FOR EXTRA-HEAVY AND HEAVY "
              "OILS (MODIFIED VASQUEZ & BEGGS CORRELATION), CALCULATION OF "
              "UNDERSATURATED OIL ISOTHERMAL COMPRESSIBILITY");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.623,
                  1.517);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("api", Api::get_instance(), "API oil gravity", 6, 22.3);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 17.21,
                  640.25);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 131.4, 250.7);
    def_parameter_latex_symbol("t", "T");
    add_parameter("tsep", Fahrenheit::get_instance(), "Separator temperature",
                  59, 177.8);
    def_parameter_latex_symbol("tsep", "T_{sp}");
    add_parameter("p", psia::get_instance(), "Pressure", 1038.49, 7411.54);
    def_parameter_latex_symbol("p", "P");
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
                            const double& rsb, const double& t,
                            const double& tsep, const double& p,
                            const double& psep) noexcept;

  Quantity<psia_1> call(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                        const Quantity<SCF_STB>& rsb,
                        const Quantity<Fahrenheit>& t,
                        const Quantity<Fahrenheit>& tsep,
                        const Quantity<psia>& p,
                        const Quantity<psia>& psep) const {
    return Quantity<psia_1>(impl(yg.raw(), api.raw(), rsb.raw(), t.raw(),
                                 tsep.raw(), p.raw(), psep.raw()));
  }

  Quantity<psia_1> validate_and_call(const Quantity<Sgg>& yg,
                                     const Quantity<Api>& api,
                                     const Quantity<SCF_STB>& rsb,
                                     const Quantity<Fahrenheit>& t,
                                     const Quantity<Fahrenheit>& tsep,
                                     const Quantity<psia>& p,
                                     const Quantity<psia>& psep) const {
    if (not(yg >= 0.623))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.623");
    if (not(yg <= 1.517))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.517");
    if (not(api >= 6))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 6");
    if (not(api <= 22.3))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 22.3");
    if (not(rsb >= 17.21))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 17.21");
    if (not(rsb <= 640.25))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 640.25");
    if (not(tsep >= 59))
      ZENTHROW(OutOfParameterRange, "parameter tsep is less than 59");
    if (not(tsep <= 177.8))
      ZENTHROW(OutOfParameterRange, "parameter tsep is greater than 177.8");
    if (not(psep >= 14.5))
      ZENTHROW(OutOfParameterRange, "parameter psep is less than 14.5");
    if (not(psep <= 752.2))
      ZENTHROW(OutOfParameterRange, "parameter psep is greater than 752.2");

    return call(yg, api, rsb, t, tsep, p, psep);
  }

  Quantity<psia_1> operator()(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<Fahrenheit>& tsep,
                              const Quantity<psia>& p,
                              const Quantity<psia>& psep) const {
    return Quantity<psia_1>(call(yg, api, rsb, t, tsep, p, psep));
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
    const auto& rsb = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& tsep = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& psep = it.get_curr();

    return call(yg, api, rsb, t, tsep, p, psep);
  }
};

extern Quantity<psia_1> CoaDeGhetto__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<Api>& api,
    const Quantity<SCF_STB>& rsb, const Quantity<Fahrenheit>& t,
    const Quantity<Fahrenheit>& tsep, const Quantity<psia>& p,
    const Quantity<psia>& psep);
extern double CoaDeGhetto__correlation__valid__fct(
    const double& yg, const double& api, const double& rsb, const double& t,
    const double& tsep, const double& p, const double& psep);

/** CoaHanafy

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group


The range for Rsb is presented by Hanafy et al. as the range for Rs.

References:

  - hanafy:1997

@param[in] rsb Solution GOR at Pb. Range in [7, 4272]
@return inside [4.46e-06, 6.268e-05]
@ingroup Correlations
*/
class CoaHanafy : public UndersaturatedOilCompressibility {
  Correlation_Singleton(CoaHanafy);

  CoaHanafy()
      : UndersaturatedOilCompressibility("CoaHanafy", psia_1::get_instance(),
                                         4.46e-06, 6.268e-05) {
    set_author("Hanafy et al.");
    set_title("HANAFY ET AL. CORRELATION, CALCULATION OF UNDERSATURATED OIL "
              "ISOTHERMAL COMPRESSIBILITY");
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

  Quantity<psia_1> call(const Quantity<SCF_STB>& rsb) const {
    return Quantity<psia_1>(impl(rsb.raw()));
  }

  Quantity<psia_1> validate_and_call(const Quantity<SCF_STB>& rsb) const {
    if (not(rsb >= 7))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 7");
    if (not(rsb <= 4272))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 4272");

    return call(rsb);
  }

  Quantity<psia_1> operator()(const Quantity<SCF_STB>& rsb) const {
    return Quantity<psia_1>(call(rsb));
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

extern Quantity<psia_1> CoaHanafy__correlation__fct(
    const Quantity<SCF_STB>& rsb);
extern double CoaHanafy__correlation__valid__fct(const double& rsb);

/** CoaKartoatmodjoSchmidt

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group


The range for Rsb is presented by Kartoatmodjo and Schmidt as the range for Rsf
(flash gas-oil ratio expressed as cubic feet of separator and stock tank gas at
any pressure less than the bubble point pressure in one stock tank barrel oil).

An error was detected in the order of magnitude when testing. The equations
finally implemented were those presented by Kartoatmodjo & Schmidt (1991) in the
program calculations at the end of the paper (SUBRUTINE RSTCOMP), using LOG10
instead of ALOG10. See also Bánzer (1996).

References:

  - kartoatmodjo:1991

  - kartoatmodjo:1994

@param[in] yg Gas specific gravity. Range in [0.4824, 1.668]
@param[in] api API oil gravity. Range in [14.4, 59]
@param[in] rsb Solution GOR at Pb. Range in [0, 2890]
@param[in] t Temperature. Range in [75, 320]
@param[in] tsep Separator temperature. Range in [38, 294]
@param[in] p Pressure. Range in [24.7, 6014.7]
@param[in] psep Separator pressure. Range in [14.7, 1414.7]
@return inside [, ]
@ingroup Correlations
*/
class CoaKartoatmodjoSchmidt : public UndersaturatedOilCompressibility {
  Correlation_Singleton(CoaKartoatmodjoSchmidt);

  CoaKartoatmodjoSchmidt()
      : UndersaturatedOilCompressibility("CoaKartoatmodjoSchmidt",
                                         psia_1::get_instance()) {
    set_author("Kartoatmodjo & Schmidt");
    set_title("KARTOATMODJO & SCHMIDT CORRELATION, CALCULATION OF "
              "UNDERSATURATED OIL ISOTHERMAL COMPRESSIBILITY");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.4824,
                  1.668);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("api", Api::get_instance(), "API oil gravity", 14.4, 59);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 0,
                  2890);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 75, 320);
    def_parameter_latex_symbol("t", "T");
    add_parameter("tsep", Fahrenheit::get_instance(), "Separator temperature",
                  38, 294);
    def_parameter_latex_symbol("tsep", "T_{sp}");
    add_parameter("p", psia::get_instance(), "Pressure", 24.7, 6014.7);
    def_parameter_latex_symbol("p", "P");
    add_parameter("psep", psia::get_instance(), "Separator pressure", 14.7,
                  1414.7);
    def_parameter_latex_symbol("psep", "P_{sp}");
    add_db("Based on a total of 3588 data points collected from 661 different "
           "crude oil samples.");
    add_db("The data bank was collected from PVT reports and literature. The "
           "first major source was from South East Asia, mainly Indonesia. The "
           "second source was North America, including the offshore area. The "
           "rest came from the Middle East and Latin America.");
    add_ref("kartoatmodjo:1991");
    add_ref("kartoatmodjo:1994");
  }

public:
  static inline double impl(const double& yg, const double& api,
                            const double& rsb, const double& t,
                            const double& tsep, const double& p,
                            const double& psep) noexcept;

  Quantity<psia_1> call(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                        const Quantity<SCF_STB>& rsb,
                        const Quantity<Fahrenheit>& t,
                        const Quantity<Fahrenheit>& tsep,
                        const Quantity<psia>& p,
                        const Quantity<psia>& psep) const {
    return Quantity<psia_1>(impl(yg.raw(), api.raw(), rsb.raw(), t.raw(),
                                 tsep.raw(), p.raw(), psep.raw()));
  }

  Quantity<psia_1> validate_and_call(const Quantity<Sgg>& yg,
                                     const Quantity<Api>& api,
                                     const Quantity<SCF_STB>& rsb,
                                     const Quantity<Fahrenheit>& t,
                                     const Quantity<Fahrenheit>& tsep,
                                     const Quantity<psia>& p,
                                     const Quantity<psia>& psep) const {
    if (not(yg >= 0.4824))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.4824");
    if (not(yg <= 1.668))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.668");
    if (not(api >= 14.4))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 14.4");
    if (not(api <= 59))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 59");
    if (not(rsb >= 0))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 0");
    if (not(rsb <= 2890))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 2890");
    if (not(tsep >= 38))
      ZENTHROW(OutOfParameterRange, "parameter tsep is less than 38");
    if (not(tsep <= 294))
      ZENTHROW(OutOfParameterRange, "parameter tsep is greater than 294");
    if (not(psep >= 14.7))
      ZENTHROW(OutOfParameterRange, "parameter psep is less than 14.7");
    if (not(psep <= 1414.7))
      ZENTHROW(OutOfParameterRange, "parameter psep is greater than 1414.7");

    return call(yg, api, rsb, t, tsep, p, psep);
  }

  Quantity<psia_1> operator()(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<Fahrenheit>& tsep,
                              const Quantity<psia>& p,
                              const Quantity<psia>& psep) const {
    return Quantity<psia_1>(call(yg, api, rsb, t, tsep, p, psep));
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
    const auto& rsb = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& tsep = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& psep = it.get_curr();

    return call(yg, api, rsb, t, tsep, p, psep);
  }
};

extern Quantity<psia_1> CoaKartoatmodjoSchmidt__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<Api>& api,
    const Quantity<SCF_STB>& rsb, const Quantity<Fahrenheit>& t,
    const Quantity<Fahrenheit>& tsep, const Quantity<psia>& p,
    const Quantity<psia>& psep);
extern double CoaKartoatmodjoSchmidt__correlation__valid__fct(
    const double& yg, const double& api, const double& rsb, const double& t,
    const double& tsep, const double& p, const double& psep);

/** CoaPetroskyFarshad

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group


The range for Rsb is presented by Petrosky & Farshad as the range for solution
GOR (Rs).

References:

  - petrosky:1993

@param[in] yg Gas specific gravity. Range in [0.5781, 0.8519]
@param[in] api API oil gravity. Range in [16.3, 45]
@param[in] rsb Solution GOR at Pb. Range in [217, 1406]
@param[in] t Temperature. Range in [114, 288]
@param[in] p Pressure. Range in [1700, 10692]
@return inside [3.507e-06, 2.464e-05]
@ingroup Correlations
*/
class CoaPetroskyFarshad : public UndersaturatedOilCompressibility {
  Correlation_Singleton(CoaPetroskyFarshad);

  CoaPetroskyFarshad()
      : UndersaturatedOilCompressibility("CoaPetroskyFarshad",
                                         psia_1::get_instance(), 3.507e-06,
                                         2.464e-05) {
    set_author("Petrosky & Farshad");
    set_title("PETROSKY & FARSHAD CORRELATION, CALCULATION OF UNDERSATURATED "
              "OIL ISOTHERMAL COMPRESSIBILITY");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.5781,
                  0.8519);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("api", Api::get_instance(), "API oil gravity", 16.3, 45);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 217,
                  1406);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 114, 288);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure", 1700, 10692);
    def_parameter_latex_symbol("p", "P");
    add_db("Based on 81 laboratory PVT analyses, using a total of 304 data "
           "points. Fluid samples were obtained from reservoirs located "
           "offshore Texas and Louisiana.");
    add_db("Producing areas from Galveston Island, eastward, through Main Pass "
           "are represented.");
    add_db("Many Gulf of Mexico crudes can be characterized as naphthenic or "
           "biodegraded oils.");
    add_ref("petrosky:1993");
  }

public:
  static inline double impl(const double& yg, const double& api,
                            const double& rsb, const double& t,
                            const double& p) noexcept;

  Quantity<psia_1> call(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                        const Quantity<SCF_STB>& rsb,
                        const Quantity<Fahrenheit>& t,
                        const Quantity<psia>& p) const {
    return Quantity<psia_1>(
        impl(yg.raw(), api.raw(), rsb.raw(), t.raw(), p.raw()));
  }

  Quantity<psia_1> validate_and_call(const Quantity<Sgg>& yg,
                                     const Quantity<Api>& api,
                                     const Quantity<SCF_STB>& rsb,
                                     const Quantity<Fahrenheit>& t,
                                     const Quantity<psia>& p) const {
    if (not(yg >= 0.5781))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.5781");
    if (not(yg <= 0.8519))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 0.8519");
    if (not(api >= 16.3))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 16.3");
    if (not(api <= 45))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 45");
    if (not(rsb >= 217))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 217");
    if (not(rsb <= 1406))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 1406");

    return call(yg, api, rsb, t, p);
  }

  Quantity<psia_1> operator()(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<psia>& p) const {
    return Quantity<psia_1>(call(yg, api, rsb, t, p));
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
    const auto& rsb = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();

    return call(yg, api, rsb, t, p);
  }
};

extern Quantity<psia_1> CoaPetroskyFarshad__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<Api>& api,
    const Quantity<SCF_STB>& rsb, const Quantity<Fahrenheit>& t,
    const Quantity<psia>& p);
extern double CoaPetroskyFarshad__correlation__valid__fct(const double& yg,
                                                          const double& api,
                                                          const double& rsb,
                                                          const double& t,
                                                          const double& p);

/** CoaVasquezBeggs

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group


The development ranges are presented by Bánzer (1996).

References:

  - vazquez:1980

@param[in] yg Gas specific gravity. Range in [0.511, 1.351]
@param[in] api API oil gravity. Range in [15.3, 59.5]
@param[in] rsb Solution GOR at Pb. Range in [9.3, 2199]
@param[in] t Temperature. Range in [, ]
@param[in] tsep Separator temperature. Range in [76, 150]
@param[in] p Pressure. Range in [141, 9515]
@param[in] psep Separator pressure. Range in [60, 565]
@return inside [, ]
@ingroup Correlations
*/
class CoaVasquezBeggs : public UndersaturatedOilCompressibility {
  Correlation_Singleton(CoaVasquezBeggs);

  CoaVasquezBeggs()
      : UndersaturatedOilCompressibility("CoaVasquezBeggs",
                                         psia_1::get_instance()) {
    set_author("Vazquez & Beggs");
    set_title("VAZQUEZ & BEGGS CORRELATION, CALCULATION OF UNDERSATURATED OIL "
              "ISOTHERMAL COMPRESSIBILITY");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.511,
                  1.351);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("api", Api::get_instance(), "API oil gravity", 15.3, 59.5);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 9.3,
                  2199);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature");
    def_parameter_latex_symbol("t", "T");
    add_parameter("tsep", Fahrenheit::get_instance(), "Separator temperature",
                  76, 150);
    def_parameter_latex_symbol("tsep", "T_{sp}");
    add_parameter("p", psia::get_instance(), "Pressure", 141, 9515);
    def_parameter_latex_symbol("p", "P");
    add_parameter("psep", psia::get_instance(), "Separator pressure", 60, 565);
    def_parameter_latex_symbol("psep", "P_{sp}");
    add_db("Based on more than 600 laboratory PVT analyses from fields all "
           "over the world.");
    add_db("The correlation was developed using a total of 4036 data points.");
    add_ref("vazquez:1980");
  }

public:
  static inline double impl(const double& yg, const double& api,
                            const double& rsb, const double& t,
                            const double& tsep, const double& p,
                            const double& psep) noexcept;

  Quantity<psia_1> call(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                        const Quantity<SCF_STB>& rsb,
                        const Quantity<Fahrenheit>& t,
                        const Quantity<Fahrenheit>& tsep,
                        const Quantity<psia>& p,
                        const Quantity<psia>& psep) const {
    return Quantity<psia_1>(impl(yg.raw(), api.raw(), rsb.raw(), t.raw(),
                                 tsep.raw(), p.raw(), psep.raw()));
  }

  Quantity<psia_1> validate_and_call(const Quantity<Sgg>& yg,
                                     const Quantity<Api>& api,
                                     const Quantity<SCF_STB>& rsb,
                                     const Quantity<Fahrenheit>& t,
                                     const Quantity<Fahrenheit>& tsep,
                                     const Quantity<psia>& p,
                                     const Quantity<psia>& psep) const {
    if (not(yg >= 0.511))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.511");
    if (not(yg <= 1.351))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.351");
    if (not(api >= 15.3))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 15.3");
    if (not(api <= 59.5))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 59.5");
    if (not(rsb >= 9.3))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 9.3");
    if (not(rsb <= 2199))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 2199");
    if (not(tsep >= 76))
      ZENTHROW(OutOfParameterRange, "parameter tsep is less than 76");
    if (not(tsep <= 150))
      ZENTHROW(OutOfParameterRange, "parameter tsep is greater than 150");
    if (not(psep >= 60))
      ZENTHROW(OutOfParameterRange, "parameter psep is less than 60");
    if (not(psep <= 565))
      ZENTHROW(OutOfParameterRange, "parameter psep is greater than 565");

    return call(yg, api, rsb, t, tsep, p, psep);
  }

  Quantity<psia_1> operator()(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<Fahrenheit>& tsep,
                              const Quantity<psia>& p,
                              const Quantity<psia>& psep) const {
    return Quantity<psia_1>(call(yg, api, rsb, t, tsep, p, psep));
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
    const auto& rsb = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& tsep = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& psep = it.get_curr();

    return call(yg, api, rsb, t, tsep, p, psep);
  }
};

extern Quantity<psia_1> CoaVasquezBeggs__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<Api>& api,
    const Quantity<SCF_STB>& rsb, const Quantity<Fahrenheit>& t,
    const Quantity<Fahrenheit>& tsep, const Quantity<psia>& p,
    const Quantity<psia>& psep);
extern double CoaVasquezBeggs__correlation__valid__fct(
    const double& yg, const double& api, const double& rsb, const double& t,
    const double& tsep, const double& p, const double& psep);

/** CoaPerezML

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group


The correlation was verified by using the original reference (Pérez, Heny &
Lago, 2001). Date: September 27 2017.

References:

  - perez:2001

@param[in] yg Gas specific gravity. Range in [0.571, 0.981]
@param[in] api API oil gravity. Range in [6.4, 12.9]
@param[in] rsb Solution GOR at Pb. Range in [38, 121]
@param[in] t Temperature. Range in [112, 300]
@param[in] p Pressure. Range in [, ]
@param[in] pb Bubble point pressure. Range in [405, 1335]
@return inside [, ]
@ingroup Correlations
*/
class CoaPerezML : public UndersaturatedOilCompressibility {
  Correlation_Singleton(CoaPerezML);

  CoaPerezML()
      : UndersaturatedOilCompressibility("CoaPerezML", psia_1::get_instance()) {
    set_author("Pérez, Heny & Lago");
    set_title("PÉREZ, HENY & LAGO CORRELATION (MODIFIED PETROSKY & FARSHAD "
              "CORRELATION), CALCULATION OF UNDERSATURATED OIL ISOTHERMAL "
              "COMPRESSIBILITY");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.571,
                  0.981);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("api", Api::get_instance(), "API oil gravity", 6.4, 12.9);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
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
    add_ref("perez:2001");
  }

  inline void precondition(const Quantity<psia>& p,
                           const Quantity<psia>& pb) const;

public:
  static inline double impl(const double& yg, const double& api,
                            const double& rsb, const double& t, const double& p,
                            const double& pb) noexcept;

  Quantity<psia_1> call(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                        const Quantity<SCF_STB>& rsb,
                        const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
                        const Quantity<psia>& pb) const {
    precondition(p, pb);
    return Quantity<psia_1>(
        impl(yg.raw(), api.raw(), rsb.raw(), t.raw(), p.raw(), pb.raw()));
  }

  Quantity<psia_1> validate_and_call(const Quantity<Sgg>& yg,
                                     const Quantity<Api>& api,
                                     const Quantity<SCF_STB>& rsb,
                                     const Quantity<Fahrenheit>& t,
                                     const Quantity<psia>& p,
                                     const Quantity<psia>& pb) const {
    if (not(yg >= 0.571))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.571");
    if (not(yg <= 0.981))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 0.981");
    if (not(api >= 6.4))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 6.4");
    if (not(api <= 12.9))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 12.9");
    if (not(rsb >= 38))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 38");
    if (not(rsb <= 121))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 121");
    if (not(pb >= 405))
      ZENTHROW(OutOfParameterRange, "parameter pb is less than 405");
    if (not(pb <= 1335))
      ZENTHROW(OutOfParameterRange, "parameter pb is greater than 1335");

    return call(yg, api, rsb, t, p, pb);
  }

  Quantity<psia_1> operator()(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<psia>& p,
                              const Quantity<psia>& pb) const {
    return Quantity<psia_1>(call(yg, api, rsb, t, p, pb));
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
    const auto& rsb = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();

    return call(yg, api, rsb, t, p, pb);
  }
};

extern Quantity<psia_1> CoaPerezML__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<Api>& api,
    const Quantity<SCF_STB>& rsb, const Quantity<Fahrenheit>& t,
    const Quantity<psia>& p, const Quantity<psia>& pb);
extern double CoaPerezML__correlation__valid__fct(
    const double& yg, const double& api, const double& rsb, const double& t,
    const double& p, const double& pb);

/** CoaMillanArcia

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group


The correlation could not be verified because the original reference is not
available. Date: August 29 2016.

References:

  - millan:1984

  - perez:2001

@param[in] api API oil gravity. Range in [9.0, 20.2]
@param[in] rsb Solution GOR at Pb. Range in [53, 459]
@param[in] t Temperature. Range in [87, 195]
@param[in] p Pressure. Range in [, ]
@param[in] pb Bubble point pressure. Range in [222.0, 3432.7]
@return inside [, ]
@ingroup Correlations
*/
class CoaMillanArcia : public UndersaturatedOilCompressibility {
  Correlation_Singleton(CoaMillanArcia);

  CoaMillanArcia()
      : UndersaturatedOilCompressibility("CoaMillanArcia",
                                         psia_1::get_instance()) {
    set_author("Millán-Arcia");
    set_title("MILLÁN-ARCIA CORRELATION, CALCULATION OF UNDERSATURATED OIL "
              "ISOTHERMAL COMPRESSIBILITY");
    add_parameter("api", Api::get_instance(), "API oil gravity", 9.0, 20.2);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 53,
                  459);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 87, 195);
    def_parameter_latex_symbol("t", "T");
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
                            const double& t, const double& p,
                            const double& pb) noexcept;

  Quantity<psia_1> call(const Quantity<Api>& api, const Quantity<SCF_STB>& rsb,
                        const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
                        const Quantity<psia>& pb) const {
    precondition(p, pb);
    return Quantity<psia_1>(
        impl(api.raw(), rsb.raw(), t.raw(), p.raw(), pb.raw()));
  }

  Quantity<psia_1> validate_and_call(const Quantity<Api>& api,
                                     const Quantity<SCF_STB>& rsb,
                                     const Quantity<Fahrenheit>& t,
                                     const Quantity<psia>& p,
                                     const Quantity<psia>& pb) const {
    if (not(api >= 9.0))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 9.0");
    if (not(api <= 20.2))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 20.2");
    if (not(rsb >= 53))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 53");
    if (not(rsb <= 459))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 459");
    if (not(pb >= 222.0))
      ZENTHROW(OutOfParameterRange, "parameter pb is less than 222.0");
    if (not(pb <= 3432.7))
      ZENTHROW(OutOfParameterRange, "parameter pb is greater than 3432.7");

    return call(api, rsb, t, p, pb);
  }

  Quantity<psia_1> operator()(const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<psia>& p,
                              const Quantity<psia>& pb) const {
    return Quantity<psia_1>(call(api, rsb, t, p, pb));
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
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();

    return call(api, rsb, t, p, pb);
  }
};

extern Quantity<psia_1> CoaMillanArcia__correlation__fct(
    const Quantity<Api>& api, const Quantity<SCF_STB>& rsb,
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<psia>& pb);
extern double CoaMillanArcia__correlation__valid__fct(const double& api,
                                                      const double& rsb,
                                                      const double& t,
                                                      const double& p,
                                                      const double& pb);

#endif // UNDERSATURATED_OIL_COMPRESSIBILITY_H
