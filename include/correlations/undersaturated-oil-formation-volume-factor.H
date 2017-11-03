#ifndef UNDERSATURATED_OIL_FORMATION_VOLUME_FACTOR_H
#define UNDERSATURATED_OIL_FORMATION_VOLUME_FACTOR_H

#include <pvt-exceptions.H>
#include <correlations/correlation.H>

/** BoaMcCain

    Generated at 2017-10-30 16:49:09 -0400

    Zen Technologies Group

This correlation determines the oil formation volume factor for pressures above
the bubble point pressure.

The correlation was verified by using the original reference and Bánzer (1996)
as a secondary reference. Date: October 26 2016

References:

  - mcCain:1990

@param[in] bobp Oil formation volume factor at Pb. Range in [, ]
@param[in] p Pressure. Range in [, ]
@param[in] pb Bubble point pressure. Range in [, ]
@param[in] coa Oil isothermal compressibility. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class BoaMcCain : public UndersaturatedOilVolumeFactor {
  Correlation_Singleton(BoaMcCain);

  BoaMcCain()
      : UndersaturatedOilVolumeFactor("BoaMcCain", RB_STB::get_instance()) {
    set_author("McCain");
    set_title("McCAIN CORRELATION, CALCULATION OF UNDERSATURATED OIL FORMATION "
              "VOLUME FACTOR");
    add_parameter("bobp", RB_STB::get_instance(),
                  "Oil formation volume factor at Pb");
    def_parameter_latex_symbol("bobp", "B_{obp}");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("pb", psia::get_instance(), "Bubble point pressure");
    def_parameter_latex_symbol("pb", "P_b");
    add_parameter("coa", psia_1::get_instance(),
                  "Oil isothermal compressibility");
    def_parameter_latex_symbol("coa", "c_{oa}");
    add_db("Based on the equation presented by McCain (1990).");
    add_db("The volume factor at bubble point pressure is adjusted to higher "
           "pressures through the use of the coefficient of isothermal "
           "compresibility.");
    add_note("This correlation determines the oil formation volume factor for "
             "pressures above the bubble point pressure.");
    add_ref("mcCain:1990");
  }

  inline void precondition(const Quantity<psia>& p,
                           const Quantity<psia>& pb) const;

public:
  static inline double impl(const double& bobp, const double& p,
                            const double& pb, const double& coa) noexcept;

  Quantity<RB_STB> call(const Quantity<RB_STB>& bobp, const Quantity<psia>& p,
                        const Quantity<psia>& pb,
                        const Quantity<psia_1>& coa) const {
    precondition(p, pb);
    return Quantity<RB_STB>(impl(bobp.raw(), p.raw(), pb.raw(), coa.raw()));
  }

  Quantity<RB_STB> validate_and_call(const Quantity<RB_STB>& bobp,
                                     const Quantity<psia>& p,
                                     const Quantity<psia>& pb,
                                     const Quantity<psia_1>& coa) const {

    return call(bobp, p, pb, coa);
  }

  Quantity<RB_STB> operator()(const Quantity<RB_STB>& bobp,
                              const Quantity<psia>& p, const Quantity<psia>& pb,
                              const Quantity<psia_1>& coa) const {
    return Quantity<RB_STB>(call(bobp, p, pb, coa));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& bobp = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();
    it.next();
    const auto& coa = it.get_curr();

    return call(bobp, p, pb, coa);
  }
};

extern Quantity<RB_STB> BoaMcCain__correlation__fct(
    const Quantity<RB_STB>& bobp, const Quantity<psia>& p,
    const Quantity<psia>& pb, const Quantity<psia_1>& coa);
extern double BoaMcCain__correlation__valid__fct(const double& bobp,
                                                 const double& p,
                                                 const double& pb,
                                                 const double& coa);

/** BoaDeGhetto

    Generated at 2017-10-30 16:49:09 -0400

    Zen Technologies Group


De Ghetto et al. recommend the use of the Vazquez & Beggs correlation for the
estimation of Bo. The values for the coefficients c1, c2, and c3 are selected
only for API<=30.

McCain's correlation for Boa is evaluated with the Coa value obtained from the
De Ghetto, Paone & Villa correlation.

References:

  - deGhetto:1995

@param[in] bobp Oil formation volume factor at Pb. Range in [, ]
@param[in] yg Gas specific gravity. Range in [0.623, 1.517]
@param[in] api API oil gravity. Range in [6, 22.3]
@param[in] rsb Solution GOR at Pb. Range in [17.21, 640.25]
@param[in] t Temperature. Range in [131.4, 250.7]
@param[in] tsep Separator temperature. Range in [59, 177.8]
@param[in] p Pressure. Range in [1038.49, 7411.54]
@param[in] pb Bubble point pressure. Range in [, ]
@param[in] psep Separator pressure. Range in [14.5, 752.2]
@return inside [, ]
@ingroup Correlations
*/
class BoaDeGhetto : public UndersaturatedOilVolumeFactor {
  Correlation_Singleton(BoaDeGhetto);

  BoaDeGhetto()
      : UndersaturatedOilVolumeFactor("BoaDeGhetto", RB_STB::get_instance()) {
    set_author("De Ghetto, Paone & Villa");
    set_title("DE GHETTO, PAONE & VILLA CORRELATION FOR EXTRA-HEAVY AND HEAVY "
              "OILS (VAZQUEZ & BEGGS CORRELATION), CALCULATION OF "
              "UNDERSATURATED OIL FORMATION VOLUME FACTOR");
    set_hidden();
    add_parameter("bobp", RB_STB::get_instance(),
                  "Oil formation volume factor at Pb");
    def_parameter_latex_symbol("bobp", "B_{obp}");
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
    add_parameter("pb", psia::get_instance(), "Bubble point pressure");
    def_parameter_latex_symbol("pb", "P_b");
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

  inline void precondition(const Quantity<psia>& p,
                           const Quantity<psia>& pb) const;

public:
  static inline double impl(const double& bobp, const double& yg,
                            const double& api, const double& rsb,
                            const double& t, const double& tsep,
                            const double& p, const double& pb,
                            const double& psep) noexcept;

  Quantity<RB_STB> call(const Quantity<RB_STB>& bobp, const Quantity<Sgg>& yg,
                        const Quantity<Api>& api, const Quantity<SCF_STB>& rsb,
                        const Quantity<Fahrenheit>& t,
                        const Quantity<Fahrenheit>& tsep,
                        const Quantity<psia>& p, const Quantity<psia>& pb,
                        const Quantity<psia>& psep) const {
    precondition(p, pb);
    return Quantity<RB_STB>(impl(bobp.raw(), yg.raw(), api.raw(), rsb.raw(),
                                 t.raw(), tsep.raw(), p.raw(), pb.raw(),
                                 psep.raw()));
  }

  Quantity<RB_STB> validate_and_call(
      const Quantity<RB_STB>& bobp, const Quantity<Sgg>& yg,
      const Quantity<Api>& api, const Quantity<SCF_STB>& rsb,
      const Quantity<Fahrenheit>& t, const Quantity<Fahrenheit>& tsep,
      const Quantity<psia>& p, const Quantity<psia>& pb,
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

    return call(bobp, yg, api, rsb, t, tsep, p, pb, psep);
  }

  Quantity<RB_STB> operator()(const Quantity<RB_STB>& bobp,
                              const Quantity<Sgg>& yg, const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<Fahrenheit>& tsep,
                              const Quantity<psia>& p, const Quantity<psia>& pb,
                              const Quantity<psia>& psep) const {
    return Quantity<RB_STB>(call(bobp, yg, api, rsb, t, tsep, p, pb, psep));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& bobp = it.get_curr();
    it.next();
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
    const auto& pb = it.get_curr();
    it.next();
    const auto& psep = it.get_curr();

    return call(bobp, yg, api, rsb, t, tsep, p, pb, psep);
  }
};

extern Quantity<RB_STB> BoaDeGhetto__correlation__fct(
    const Quantity<RB_STB>& bobp, const Quantity<Sgg>& yg,
    const Quantity<Api>& api, const Quantity<SCF_STB>& rsb,
    const Quantity<Fahrenheit>& t, const Quantity<Fahrenheit>& tsep,
    const Quantity<psia>& p, const Quantity<psia>& pb,
    const Quantity<psia>& psep);
extern double BoaDeGhetto__correlation__valid__fct(
    const double& bobp, const double& yg, const double& api, const double& rsb,
    const double& t, const double& tsep, const double& p, const double& pb,
    const double& psep);

/** BoaHanafy

    Generated at 2017-10-30 16:49:09 -0400

    Zen Technologies Group


McCain's correlation for Boa is evaluated with the Coa value obtained from the
Hanafy et al. correlation.

References:

  - hanafy:1997

@param[in] bobp Oil formation volume factor at Pb. Range in [, ]
@param[in] p Pressure. Range in [, ]
@param[in] pb Bubble point pressure. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class BoaHanafy : public UndersaturatedOilVolumeFactor {
  Correlation_Singleton(BoaHanafy);

  BoaHanafy()
      : UndersaturatedOilVolumeFactor("BoaHanafy", RB_STB::get_instance()) {
    set_author("Hanafy et al.");
    set_title("HANAFY ET AL. CORRELATION, CALCULATION OF UNDERSATURATED OIL "
              "FORMATION VOLUME FACTOR");
    set_hidden();
    add_parameter("bobp", RB_STB::get_instance(),
                  "Oil formation volume factor at Pb");
    def_parameter_latex_symbol("bobp", "B_{obp}");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("pb", psia::get_instance(), "Bubble point pressure");
    def_parameter_latex_symbol("pb", "P_b");
    add_db("Based on experimental PVT data of 324 fluid samples taken from 176 "
           "wells located in 75 fields. This data represents 15 productive "
           "zones of 123 reservoirs distributed along three different regions "
           "of Egypt, including the Gulf of Suez, Western Desert, and Sinai.");
    add_ref("hanafy:1997");
  }

  inline void precondition(const Quantity<psia>& p,
                           const Quantity<psia>& pb) const;

public:
  static inline double impl(const double& bobp, const double& p,
                            const double& pb) noexcept;

  Quantity<RB_STB> call(const Quantity<RB_STB>& bobp, const Quantity<psia>& p,
                        const Quantity<psia>& pb) const {
    precondition(p, pb);
    return Quantity<RB_STB>(impl(bobp.raw(), p.raw(), pb.raw()));
  }

  Quantity<RB_STB> validate_and_call(const Quantity<RB_STB>& bobp,
                                     const Quantity<psia>& p,
                                     const Quantity<psia>& pb) const {

    return call(bobp, p, pb);
  }

  Quantity<RB_STB> operator()(const Quantity<RB_STB>& bobp,
                              const Quantity<psia>& p,
                              const Quantity<psia>& pb) const {
    return Quantity<RB_STB>(call(bobp, p, pb));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& bobp = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();

    return call(bobp, p, pb);
  }
};

extern Quantity<RB_STB> BoaHanafy__correlation__fct(
    const Quantity<RB_STB>& bobp, const Quantity<psia>& p,
    const Quantity<psia>& pb);
extern double BoaHanafy__correlation__valid__fct(const double& bobp,
                                                 const double& p,
                                                 const double& pb);

/** BoaKartoatmodjoSchmidt

    Generated at 2017-10-30 16:49:09 -0400

    Zen Technologies Group


McCain's correlation for Boa is evaluated with the Coa value obtained from the
Kartoatmodjo & Schmidt correlation.

References:

  - kartoatmodjo:1991

@param[in] bobp Oil formation volume factor at Pb. Range in [, ]
@param[in] yg Gas specific gravity. Range in [0.4824, 1.668]
@param[in] yo Oil specific gravity. Range in [Quantity<Api>(59),
Quantity<Api>(14.4)]
@param[in] rsb Solution GOR at Pb. Range in [0, 2890]
@param[in] t Temperature. Range in [75, 320]
@param[in] tsep Separator temperature. Range in [38, 294]
@param[in] p Pressure. Range in [24.7, 6014.7]
@param[in] pb Bubble point pressure. Range in [, ]
@param[in] psep Separator pressure. Range in [14.7, 1414.7]
@return inside [, ]
@ingroup Correlations
*/
class BoaKartoatmodjoSchmidt : public UndersaturatedOilVolumeFactor {
  Correlation_Singleton(BoaKartoatmodjoSchmidt);

  BoaKartoatmodjoSchmidt()
      : UndersaturatedOilVolumeFactor("BoaKartoatmodjoSchmidt",
                                      RB_STB::get_instance()) {
    set_author("Kartoatmodjo & Schmidt");
    set_title("KARTOATMODJO & SCHMIDT CORRELATION, CALCULATION OF "
              "UNDERSATURATED OIL FORMATION VOLUME FACTOR");
    set_hidden();
    add_parameter("bobp", RB_STB::get_instance(),
                  "Oil formation volume factor at Pb");
    def_parameter_latex_symbol("bobp", "B_{obp}");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.4824,
                  1.668);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(59), Quantity<Api>(14.4));
    def_parameter_latex_symbol("yo", "\\gamma_o");
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
    add_parameter("pb", psia::get_instance(), "Bubble point pressure");
    def_parameter_latex_symbol("pb", "P_b");
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

  inline void precondition(const Quantity<psia>& p,
                           const Quantity<psia>& pb) const;

public:
  static inline double impl(const double& bobp, const double& yg,
                            const double& yo, const double& rsb,
                            const double& t, const double& tsep,
                            const double& p, const double& pb,
                            const double& psep) noexcept;

  Quantity<RB_STB> call(const Quantity<RB_STB>& bobp, const Quantity<Sgg>& yg,
                        const Quantity<Sg_do>& yo, const Quantity<SCF_STB>& rsb,
                        const Quantity<Fahrenheit>& t,
                        const Quantity<Fahrenheit>& tsep,
                        const Quantity<psia>& p, const Quantity<psia>& pb,
                        const Quantity<psia>& psep) const {
    precondition(p, pb);
    return Quantity<RB_STB>(impl(bobp.raw(), yg.raw(), yo.raw(), rsb.raw(),
                                 t.raw(), tsep.raw(), p.raw(), pb.raw(),
                                 psep.raw()));
  }

  Quantity<RB_STB> validate_and_call(
      const Quantity<RB_STB>& bobp, const Quantity<Sgg>& yg,
      const Quantity<Sg_do>& yo, const Quantity<SCF_STB>& rsb,
      const Quantity<Fahrenheit>& t, const Quantity<Fahrenheit>& tsep,
      const Quantity<psia>& p, const Quantity<psia>& pb,
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

    return call(bobp, yg, yo, rsb, t, tsep, p, pb, psep);
  }

  Quantity<RB_STB> operator()(const Quantity<RB_STB>& bobp,
                              const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<Fahrenheit>& tsep,
                              const Quantity<psia>& p, const Quantity<psia>& pb,
                              const Quantity<psia>& psep) const {
    return Quantity<RB_STB>(call(bobp, yg, yo, rsb, t, tsep, p, pb, psep));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& bobp = it.get_curr();
    it.next();
    const auto& yg = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& tsep = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();
    it.next();
    const auto& psep = it.get_curr();

    return call(bobp, yg, yo, rsb, t, tsep, p, pb, psep);
  }
};

extern Quantity<RB_STB> BoaKartoatmodjoSchmidt__correlation__fct(
    const Quantity<RB_STB>& bobp, const Quantity<Sgg>& yg,
    const Quantity<Sg_do>& yo, const Quantity<SCF_STB>& rsb,
    const Quantity<Fahrenheit>& t, const Quantity<Fahrenheit>& tsep,
    const Quantity<psia>& p, const Quantity<psia>& pb,
    const Quantity<psia>& psep);
extern double BoaKartoatmodjoSchmidt__correlation__valid__fct(
    const double& bobp, const double& yg, const double& yo, const double& rsb,
    const double& t, const double& tsep, const double& p, const double& pb,
    const double& psep);

/** BoaPetroskyFarshad

    Generated at 2017-10-30 16:49:09 -0400

    Zen Technologies Group


McCain's correlation for Boa is evaluated with the Coa value obtained from the
Petrosky & Farshad correlation.

References:

  - petrosky:1993

@param[in] bobp Oil formation volume factor at Pb. Range in [, ]
@param[in] yg Gas specific gravity. Range in [0.5781, 0.8519]
@param[in] yo Oil specific gravity. Range in [Quantity<Api>(45),
Quantity<Api>(16.3)]
@param[in] rsb Solution GOR at Pb. Range in [217, 1406]
@param[in] t Temperature. Range in [114, 288]
@param[in] p Pressure. Range in [1700, 10692]
@param[in] pb Bubble point pressure. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class BoaPetroskyFarshad : public UndersaturatedOilVolumeFactor {
  Correlation_Singleton(BoaPetroskyFarshad);

  BoaPetroskyFarshad()
      : UndersaturatedOilVolumeFactor("BoaPetroskyFarshad",
                                      RB_STB::get_instance()) {
    set_author("Petrosky & Farshad");
    set_title("PETROSKY & FARSHAD CORRELATION, CALCULATION OF UNDERSATURATED "
              "OIL FORMATION VOLUME FACTOR");
    set_hidden();
    add_parameter("bobp", RB_STB::get_instance(),
                  "Oil formation volume factor at Pb");
    def_parameter_latex_symbol("bobp", "B_{obp}");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.5781,
                  0.8519);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(45), Quantity<Api>(16.3));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 217,
                  1406);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 114, 288);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure", 1700, 10692);
    def_parameter_latex_symbol("p", "P");
    add_parameter("pb", psia::get_instance(), "Bubble point pressure");
    def_parameter_latex_symbol("pb", "P_b");
    add_db("Based on 81 laboratory PVT analyses. Fluid samples were obtained "
           "from reservoirs located offshore Texas and Louisiana.");
    add_db("Producing areas from Galveston Island, eastward, through Main Pass "
           "are represented.");
    add_db("Many Gulf of Mexico crudes can be characterized as naphthenic or "
           "biodegraded oils.");
    add_ref("petrosky:1993");
    add_par_synonym("yo", "api", "Api");
  }

  inline void precondition(const Quantity<psia>& p,
                           const Quantity<psia>& pb) const;

public:
  static inline double impl(const double& bobp, const double& yg,
                            const double& yo, const double& rsb,
                            const double& t, const double& p,
                            const double& pb) noexcept;

  Quantity<RB_STB> call(const Quantity<RB_STB>& bobp, const Quantity<Sgg>& yg,
                        const Quantity<Sg_do>& yo, const Quantity<SCF_STB>& rsb,
                        const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
                        const Quantity<psia>& pb) const {
    precondition(p, pb);
    return Quantity<RB_STB>(impl(bobp.raw(), yg.raw(), yo.raw(), rsb.raw(),
                                 t.raw(), p.raw(), pb.raw()));
  }

  Quantity<RB_STB> validate_and_call(const Quantity<RB_STB>& bobp,
                                     const Quantity<Sgg>& yg,
                                     const Quantity<Sg_do>& yo,
                                     const Quantity<SCF_STB>& rsb,
                                     const Quantity<Fahrenheit>& t,
                                     const Quantity<psia>& p,
                                     const Quantity<psia>& pb) const {
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
    if (not(rsb >= 217))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 217");
    if (not(rsb <= 1406))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 1406");

    return call(bobp, yg, yo, rsb, t, p, pb);
  }

  Quantity<RB_STB> operator()(const Quantity<RB_STB>& bobp,
                              const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<psia>& p,
                              const Quantity<psia>& pb) const {
    return Quantity<RB_STB>(call(bobp, yg, yo, rsb, t, p, pb));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& bobp = it.get_curr();
    it.next();
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

    return call(bobp, yg, yo, rsb, t, p, pb);
  }
};

extern Quantity<RB_STB> BoaPetroskyFarshad__correlation__fct(
    const Quantity<RB_STB>& bobp, const Quantity<Sgg>& yg,
    const Quantity<Sg_do>& yo, const Quantity<SCF_STB>& rsb,
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<psia>& pb);
extern double BoaPetroskyFarshad__correlation__valid__fct(
    const double& bobp, const double& yg, const double& yo, const double& rsb,
    const double& t, const double& p, const double& pb);

/** BoaVasquezBeggs

    Generated at 2017-10-30 16:49:09 -0400

    Zen Technologies Group


McCain's correlation for Boa is evaluated with the Coa value obtained from the
Vazquez & Beggs correlation.

References:

  - vazquez:1980

  - alShammasi:2001

@param[in] bobp Oil formation volume factor at Pb. Range in [, ]
@param[in] yg Gas specific gravity. Range in [0.511, 1.351]
@param[in] api API oil gravity. Range in [15.3, 59.5]
@param[in] rsb Solution GOR at Pb. Range in [9.3, 2199]
@param[in] t Temperature. Range in [, ]
@param[in] tsep Separator temperature. Range in [76, 150]
@param[in] p Pressure. Range in [141, 9515]
@param[in] pb Bubble point pressure. Range in [, ]
@param[in] psep Separator pressure. Range in [60, 565]
@return inside [, ]
@ingroup Correlations
*/
class BoaVasquezBeggs : public UndersaturatedOilVolumeFactor {
  Correlation_Singleton(BoaVasquezBeggs);

  BoaVasquezBeggs()
      : UndersaturatedOilVolumeFactor("BoaVasquezBeggs",
                                      RB_STB::get_instance()) {
    set_author("Vazquez & Beggs");
    set_title("VAZQUEZ & BEGGS CORRELATION, CALCULATION OF UNDERSATURATED OIL "
              "FORMATION VOLUME FACTOR");
    set_hidden();
    add_parameter("bobp", RB_STB::get_instance(),
                  "Oil formation volume factor at Pb");
    def_parameter_latex_symbol("bobp", "B_{obp}");
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
    add_parameter("pb", psia::get_instance(), "Bubble point pressure");
    def_parameter_latex_symbol("pb", "P_b");
    add_parameter("psep", psia::get_instance(), "Separator pressure", 60, 565);
    def_parameter_latex_symbol("psep", "P_{sp}");
    add_db("Based on more than 600 laboratory PVT analyses from fields all "
           "over the world.");
    add_ref("vazquez:1980");
    add_ref("alShammasi:2001");
  }

  inline void precondition(const Quantity<psia>& p,
                           const Quantity<psia>& pb) const;

public:
  static inline double impl(const double& bobp, const double& yg,
                            const double& api, const double& rsb,
                            const double& t, const double& tsep,
                            const double& p, const double& pb,
                            const double& psep) noexcept;

  Quantity<RB_STB> call(const Quantity<RB_STB>& bobp, const Quantity<Sgg>& yg,
                        const Quantity<Api>& api, const Quantity<SCF_STB>& rsb,
                        const Quantity<Fahrenheit>& t,
                        const Quantity<Fahrenheit>& tsep,
                        const Quantity<psia>& p, const Quantity<psia>& pb,
                        const Quantity<psia>& psep) const {
    precondition(p, pb);
    return Quantity<RB_STB>(impl(bobp.raw(), yg.raw(), api.raw(), rsb.raw(),
                                 t.raw(), tsep.raw(), p.raw(), pb.raw(),
                                 psep.raw()));
  }

  Quantity<RB_STB> validate_and_call(
      const Quantity<RB_STB>& bobp, const Quantity<Sgg>& yg,
      const Quantity<Api>& api, const Quantity<SCF_STB>& rsb,
      const Quantity<Fahrenheit>& t, const Quantity<Fahrenheit>& tsep,
      const Quantity<psia>& p, const Quantity<psia>& pb,
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

    return call(bobp, yg, api, rsb, t, tsep, p, pb, psep);
  }

  Quantity<RB_STB> operator()(const Quantity<RB_STB>& bobp,
                              const Quantity<Sgg>& yg, const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<Fahrenheit>& tsep,
                              const Quantity<psia>& p, const Quantity<psia>& pb,
                              const Quantity<psia>& psep) const {
    return Quantity<RB_STB>(call(bobp, yg, api, rsb, t, tsep, p, pb, psep));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& bobp = it.get_curr();
    it.next();
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
    const auto& pb = it.get_curr();
    it.next();
    const auto& psep = it.get_curr();

    return call(bobp, yg, api, rsb, t, tsep, p, pb, psep);
  }
};

extern Quantity<RB_STB> BoaVasquezBeggs__correlation__fct(
    const Quantity<RB_STB>& bobp, const Quantity<Sgg>& yg,
    const Quantity<Api>& api, const Quantity<SCF_STB>& rsb,
    const Quantity<Fahrenheit>& t, const Quantity<Fahrenheit>& tsep,
    const Quantity<psia>& p, const Quantity<psia>& pb,
    const Quantity<psia>& psep);
extern double BoaVasquezBeggs__correlation__valid__fct(
    const double& bobp, const double& yg, const double& api, const double& rsb,
    const double& t, const double& tsep, const double& p, const double& pb,
    const double& psep);

/** BoaPerezML

    Generated at 2017-10-30 16:49:09 -0400

    Zen Technologies Group

The correlation could not be verified because the original reference is not
available. Date: August 22 2016.

The correlation could not be verified because the original reference is not
available. Date: August 22 2016.

McCain's correlation for Boa is evaluated with the Coa value obtained from the
Pérez, Heny & Lago correlation.

References:

  - perez:2001

@param[in] bobp Oil formation volume factor at Pb. Range in [, ]
@param[in] yg Gas specific gravity. Range in [0.571, 0.981]
@param[in] yo Oil specific gravity. Range in [Quantity<Api>(12.9),
Quantity<Api>(6.4)]
@param[in] rsb Solution GOR at Pb. Range in [38, 121]
@param[in] t Temperature. Range in [112, 300]
@param[in] p Pressure. Range in [, ]
@param[in] pb Bubble point pressure. Range in [405, 1335]
@return inside [, ]
@ingroup Correlations
*/
class BoaPerezML : public UndersaturatedOilVolumeFactor {
  Correlation_Singleton(BoaPerezML);

  BoaPerezML()
      : UndersaturatedOilVolumeFactor("BoaPerezML", RB_STB::get_instance()) {
    set_author("Pérez, Heny & Lago");
    set_title("PÉREZ, HENY & LAGO CORRELATION (EZEKWE-QUERIN-HUMPHREY "
              "CORRELATION USED BY PÉREZ, HENY & LAGO), CALCULATION OF "
              "UNDERSATURATED OIL FORMATION VOLUME FACTOR");
    set_hidden();
    add_parameter("bobp", RB_STB::get_instance(),
                  "Oil formation volume factor at Pb");
    def_parameter_latex_symbol("bobp", "B_{obp}");
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
    add_note("The correlation could not be verified because the original "
             "reference is not available. Date: August 22 2016.");
    add_ref("perez:2001");
    add_par_synonym("yo", "api", "Api");
  }

  inline void precondition(const Quantity<psia>& p,
                           const Quantity<psia>& pb) const;

public:
  static inline double impl(const double& bobp, const double& yg,
                            const double& yo, const double& rsb,
                            const double& t, const double& p,
                            const double& pb) noexcept;

  Quantity<RB_STB> call(const Quantity<RB_STB>& bobp, const Quantity<Sgg>& yg,
                        const Quantity<Sg_do>& yo, const Quantity<SCF_STB>& rsb,
                        const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
                        const Quantity<psia>& pb) const {
    precondition(p, pb);
    return Quantity<RB_STB>(impl(bobp.raw(), yg.raw(), yo.raw(), rsb.raw(),
                                 t.raw(), p.raw(), pb.raw()));
  }

  Quantity<RB_STB> validate_and_call(const Quantity<RB_STB>& bobp,
                                     const Quantity<Sgg>& yg,
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

    return call(bobp, yg, yo, rsb, t, p, pb);
  }

  Quantity<RB_STB> operator()(const Quantity<RB_STB>& bobp,
                              const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<psia>& p,
                              const Quantity<psia>& pb) const {
    return Quantity<RB_STB>(call(bobp, yg, yo, rsb, t, p, pb));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& bobp = it.get_curr();
    it.next();
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

    return call(bobp, yg, yo, rsb, t, p, pb);
  }
};

extern Quantity<RB_STB> BoaPerezML__correlation__fct(
    const Quantity<RB_STB>& bobp, const Quantity<Sgg>& yg,
    const Quantity<Sg_do>& yo, const Quantity<SCF_STB>& rsb,
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<psia>& pb);
extern double BoaPerezML__correlation__valid__fct(
    const double& bobp, const double& yg, const double& yo, const double& rsb,
    const double& t, const double& p, const double& pb);

/** BoaMillanArcia

    Generated at 2017-10-30 16:49:09 -0400

    Zen Technologies Group


The correlation could not be verified because the original reference is not
available. Date: August 22 2016.

McCain's correlation for Boa is evaluated with the Coa value obtained from the
Millán-Arcia correlation.

References:

  - millan:1984

  - perez:2001

@param[in] bobp Oil formation volume factor at Pb. Range in [, ]
@param[in] api API oil gravity. Range in [9.0, 20.2]
@param[in] rsb Solution GOR at Pb. Range in [53, 459]
@param[in] t Temperature. Range in [87, 195]
@param[in] p Pressure. Range in [, ]
@param[in] pb Bubble point pressure. Range in [222.0, 3432.7]
@return inside [, ]
@ingroup Correlations
*/
class BoaMillanArcia : public UndersaturatedOilVolumeFactor {
  Correlation_Singleton(BoaMillanArcia);

  BoaMillanArcia()
      : UndersaturatedOilVolumeFactor("BoaMillanArcia",
                                      RB_STB::get_instance()) {
    set_author("Millán-Arcia");
    set_title("MILLÁN-ARCIA CORRELATION, CALCULATION OF UNDERSATURATED OIL "
              "FORMATION VOLUME FACTOR");
    set_hidden();
    add_parameter("bobp", RB_STB::get_instance(),
                  "Oil formation volume factor at Pb");
    def_parameter_latex_symbol("bobp", "B_{obp}");
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
  static inline double impl(const double& bobp, const double& api,
                            const double& rsb, const double& t, const double& p,
                            const double& pb) noexcept;

  Quantity<RB_STB> call(const Quantity<RB_STB>& bobp, const Quantity<Api>& api,
                        const Quantity<SCF_STB>& rsb,
                        const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
                        const Quantity<psia>& pb) const {
    precondition(p, pb);
    return Quantity<RB_STB>(
        impl(bobp.raw(), api.raw(), rsb.raw(), t.raw(), p.raw(), pb.raw()));
  }

  Quantity<RB_STB> validate_and_call(const Quantity<RB_STB>& bobp,
                                     const Quantity<Api>& api,
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

    return call(bobp, api, rsb, t, p, pb);
  }

  Quantity<RB_STB> operator()(const Quantity<RB_STB>& bobp,
                              const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<psia>& p,
                              const Quantity<psia>& pb) const {
    return Quantity<RB_STB>(call(bobp, api, rsb, t, p, pb));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& bobp = it.get_curr();
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

    return call(bobp, api, rsb, t, p, pb);
  }
};

extern Quantity<RB_STB> BoaMillanArcia__correlation__fct(
    const Quantity<RB_STB>& bobp, const Quantity<Api>& api,
    const Quantity<SCF_STB>& rsb, const Quantity<Fahrenheit>& t,
    const Quantity<psia>& p, const Quantity<psia>& pb);
extern double BoaMillanArcia__correlation__valid__fct(
    const double& bobp, const double& api, const double& rsb, const double& t,
    const double& p, const double& pb);

#endif // UNDERSATURATED_OIL_FORMATION_VOLUME_FACTOR_H
