#ifndef NON_HYDROCAHORNS_CORRECTIONS_H
#define NON_HYDROCAHORNS_CORRECTIONS_H

#include <pvt-exceptions.H>
#include <correlations/correlation.H>

/** YghcWichertAziz

    Generated at 2017-10-30 16:49:06 -0400

    Zen Technologies Group

Method of Wichert & Aziz for the calculation of the gas gravity of the
hydrocarbon portion.

The original reference is not available. The correlation was verified by using
secondary references: Standing (1977) and Bánzer (1996). Date: September 12
2016.

References:

  - wichert:1972

  - standing:1977

@param[in] yg Gas specific gravity. Range in [, ]
@param[in] n2 N2 concentration. Range in [, ]
@param[in] co2 CO2 concentration. Range in [, ]
@param[in] h2s H2S concentration. Range in [, ]
@return inside [0.55, ]
@ingroup Correlations
*/
class YghcWichertAziz : public HydrocarbonGasSpecificGravity {
  Correlation_Singleton(YghcWichertAziz);

  YghcWichertAziz()
      : HydrocarbonGasSpecificGravity("YghcWichertAziz", Sgg::get_instance(),
                                      0.55) {
    set_author("Wichert & Aziz (Gas Gravity Correction)");
    set_title("METHOD OF WICHERT & AZIZ FOR THE CALCULATION OF THE GAS GRAVITY "
              "OF THE HYDROCARBON PORTION");
    set_hidden_blackoil_grid();
    set_hidden_wetgas_grid();
    set_hidden_drygas_grid();
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity");
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("n2", MoleFraction::get_instance(), "N2 concentration");
    add_parameter("co2", MoleFraction::get_instance(), "CO2 concentration");
    add_parameter("h2s", MoleFraction::get_instance(), "H2S concentration");
    add_note("Method of Wichert & Aziz for the calculation of the gas gravity "
             "of the hydrocarbon portion.");
    add_ref("wichert:1972");
    add_ref("standing:1977");
  }

  inline void precondition(const Quantity<MoleFraction>& n2,
                           const Quantity<MoleFraction>& co2,
                           const Quantity<MoleFraction>& h2s) const;

public:
  static inline double impl(const double& yg, const double& n2,
                            const double& co2, const double& h2s) noexcept;

  Quantity<Sgg> call(const Quantity<Sgg>& yg, const Quantity<MoleFraction>& n2,
                     const Quantity<MoleFraction>& co2,
                     const Quantity<MoleFraction>& h2s) const {
    precondition(n2, co2, h2s);
    return Quantity<Sgg>(impl(yg.raw(), n2.raw(), co2.raw(), h2s.raw()));
  }

  Quantity<Sgg> validate_and_call(const Quantity<Sgg>& yg,
                                  const Quantity<MoleFraction>& n2,
                                  const Quantity<MoleFraction>& co2,
                                  const Quantity<MoleFraction>& h2s) const {

    return call(yg, n2, co2, h2s);
  }

  Quantity<Sgg> operator()(const Quantity<Sgg>& yg,
                           const Quantity<MoleFraction>& n2,
                           const Quantity<MoleFraction>& co2,
                           const Quantity<MoleFraction>& h2s) const {
    return Quantity<Sgg>(call(yg, n2, co2, h2s));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& n2 = it.get_curr();
    it.next();
    const auto& co2 = it.get_curr();
    it.next();
    const auto& h2s = it.get_curr();

    return call(yg, n2, co2, h2s);
  }
};

extern Quantity<Sgg> YghcWichertAziz__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<MoleFraction>& n2,
    const Quantity<MoleFraction>& co2, const Quantity<MoleFraction>& h2s);
extern double YghcWichertAziz__correlation__valid__fct(const double& yg,
                                                       const double& n2,
                                                       const double& co2,
                                                       const double& h2s);

/** PpcmKayMixingRule

    Generated at 2017-10-30 16:49:06 -0400

    Zen Technologies Group

The value of the pseudocritical pressure of the gas hydrocarbon portion is
adjusted for nonhydrocarbon content on the basis of Kay's mixing rule.

The original reference is not available. The correlation was verified by using
secondary references: Whitson & Brulé (2000), Standing (1977) and Bánzer (1996).
Date: September 12 2016.

References:

  - kay:1936

  - whitson:2000

  - standing:1977

@param[in] ppchc Gas pseudocritical pressure of the hydrocarbon portion. Range
in [, ]
@param[in] n2 N2 concentration. Range in [, ]
@param[in] co2 CO2 concentration. Range in [, ]
@param[in] h2s H2S concentration. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class PpcmKayMixingRule : public MixtureGasPseudocriticalPressure {
  Correlation_Singleton(PpcmKayMixingRule);

  PpcmKayMixingRule()
      : MixtureGasPseudocriticalPressure("PpcmKayMixingRule",
                                         psia::get_instance()) {
    set_author("Kay (Mixing Rule)");
    set_title("KAY'S MIXING RULE, CALCULATION OF PSEUDOCRITICAL PRESSURE OF "
              "THE WHOLE GAS MIXTURE");
    set_hidden_blackoil_grid();
    set_hidden_wetgas_grid();
    set_hidden_drygas_grid();
    add_parameter("ppchc", psia::get_instance(),
                  "Gas pseudocritical pressure of the hydrocarbon portion");
    def_parameter_latex_symbol("ppchc", "P_{pcHC}");
    add_parameter("n2", MoleFraction::get_instance(), "N2 concentration");
    add_parameter("co2", MoleFraction::get_instance(), "CO2 concentration");
    add_parameter("h2s", MoleFraction::get_instance(), "H2S concentration");
    add_note("The value of the pseudocritical pressure of the gas hydrocarbon "
             "portion is adjusted for nonhydrocarbon content on the basis of "
             "Kay's mixing rule.");
    add_ref("kay:1936");
    add_ref("whitson:2000");
    add_ref("standing:1977");
  }

  inline void precondition(const Quantity<MoleFraction>& n2,
                           const Quantity<MoleFraction>& co2,
                           const Quantity<MoleFraction>& h2s) const;

public:
  static inline double impl(const double& ppchc, const double& n2,
                            const double& co2, const double& h2s) noexcept;

  Quantity<psia> call(const Quantity<psia>& ppchc,
                      const Quantity<MoleFraction>& n2,
                      const Quantity<MoleFraction>& co2,
                      const Quantity<MoleFraction>& h2s) const {
    precondition(n2, co2, h2s);
    return Quantity<psia>(impl(ppchc.raw(), n2.raw(), co2.raw(), h2s.raw()));
  }

  Quantity<psia> validate_and_call(const Quantity<psia>& ppchc,
                                   const Quantity<MoleFraction>& n2,
                                   const Quantity<MoleFraction>& co2,
                                   const Quantity<MoleFraction>& h2s) const {

    return call(ppchc, n2, co2, h2s);
  }

  Quantity<psia> operator()(const Quantity<psia>& ppchc,
                            const Quantity<MoleFraction>& n2,
                            const Quantity<MoleFraction>& co2,
                            const Quantity<MoleFraction>& h2s) const {
    return Quantity<psia>(call(ppchc, n2, co2, h2s));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& ppchc = it.get_curr();
    it.next();
    const auto& n2 = it.get_curr();
    it.next();
    const auto& co2 = it.get_curr();
    it.next();
    const auto& h2s = it.get_curr();

    return call(ppchc, n2, co2, h2s);
  }
};

extern Quantity<psia> PpcmKayMixingRule__correlation__fct(
    const Quantity<psia>& ppchc, const Quantity<MoleFraction>& n2,
    const Quantity<MoleFraction>& co2, const Quantity<MoleFraction>& h2s);
extern double PpcmKayMixingRule__correlation__valid__fct(const double& ppchc,
                                                         const double& n2,
                                                         const double& co2,
                                                         const double& h2s);

/** AdjustedppcmWichertAziz

    Generated at 2017-10-30 16:49:06 -0400

    Zen Technologies Group

The pseudocritical pressure of the mixture is adjusted by using the Wichert &
Aziz correlation when the gas contains significant fractions of acid gases,
specifically carbon dioxide and hydrogen sulfide.
The authors indicate that the correlation has an average absolute error of 0.97
% over these ranges: 154 psia < p < 7,026 psia and 40 °F < T < 300 °F.

The original reference is not available. The correlation was verified by using a
secondary reference: Standing (1977). Date: September 12 2016.

The development ranges were taken from PetroWiki
(http://petrowiki.org/Real_gases).

References:

  - wichert:1972

  - standing:1977

  - petroWiki:2016:6

@param[in] ppcm Gas pseudocritical pressure of the mixture. Range in [, ]
@param[in] tpcm Gas pseudocritical temperature of the mixture. Range in [, ]
@param[in] co2 CO2 concentration. Range in [Quantity<MolePercent>(0),
Quantity<MolePercent>(54.4)]
@param[in] h2s H2S concentration. Range in [Quantity<MolePercent>(0),
Quantity<MolePercent>(73.8)]
@return inside [, ]
@ingroup Correlations
*/
class AdjustedppcmWichertAziz : public MixtureGasPseudocriticalPressure {
  Correlation_Singleton(AdjustedppcmWichertAziz);

  AdjustedppcmWichertAziz()
      : MixtureGasPseudocriticalPressure("AdjustedppcmWichertAziz",
                                         psia::get_instance()) {
    set_author("Wichert & Aziz (Mixture Correction)");
    set_title("WICHERT & AZIZ CORRELATION, CALCULATION OF PSEUDOCRITICAL "
              "PRESSURE OF THE WHOLE GAS MIXTURE");
    set_hidden_blackoil_grid();
    set_hidden_wetgas_grid();
    set_hidden_drygas_grid();
    add_parameter("ppcm", psia::get_instance(),
                  "Gas pseudocritical pressure of the mixture");
    def_parameter_latex_symbol("ppcm", "P_{pc}");
    add_parameter("tpcm", Rankine::get_instance(),
                  "Gas pseudocritical temperature of the mixture");
    def_parameter_latex_symbol("tpcm", "T_{pc}");
    add_parameter("co2", MoleFraction::get_instance(), "CO2 concentration",
                  Quantity<MolePercent>(0), Quantity<MolePercent>(54.4));
    add_parameter("h2s", MoleFraction::get_instance(), "H2S concentration",
                  Quantity<MolePercent>(0), Quantity<MolePercent>(73.8));
    add_note("The pseudocritical pressure of the mixture is adjusted by using "
             "the Wichert & Aziz correlation when the gas contains significant "
             "fractions of acid gases, specifically carbon dioxide and "
             "hydrogen sulfide.");
    add_note("The authors indicate that the correlation has an average "
             "absolute error of 0.97 % over these ranges: 154 psia < p < 7,026 "
             "psia and 40 °F < T < 300 °F.");
    add_ref("wichert:1972");
    add_ref("standing:1977");
    add_ref("petroWiki:2016:6");
  }

  inline void precondition(const Quantity<MoleFraction>& co2,
                           const Quantity<MoleFraction>& h2s) const;

public:
  static inline double impl(const double& ppcm, const double& tpcm,
                            const double& co2, const double& h2s) noexcept;

  Quantity<psia> call(const Quantity<psia>& ppcm, const Quantity<Rankine>& tpcm,
                      const Quantity<MoleFraction>& co2,
                      const Quantity<MoleFraction>& h2s) const {
    precondition(co2, h2s);
    return Quantity<psia>(impl(ppcm.raw(), tpcm.raw(), co2.raw(), h2s.raw()));
  }

  Quantity<psia> validate_and_call(const Quantity<psia>& ppcm,
                                   const Quantity<Rankine>& tpcm,
                                   const Quantity<MoleFraction>& co2,
                                   const Quantity<MoleFraction>& h2s) const {
    if (not(co2 >= Quantity<MolePercent>(0)))
      ZENTHROW(OutOfParameterRange,
               "parameter co2 is less than Quantity<MolePercent>(0)");
    if (not(co2 <= Quantity<MolePercent>(54.4)))
      ZENTHROW(OutOfParameterRange,
               "parameter co2 is greater than Quantity<MolePercent>(54.4)");
    if (not(h2s >= Quantity<MolePercent>(0)))
      ZENTHROW(OutOfParameterRange,
               "parameter h2s is less than Quantity<MolePercent>(0)");
    if (not(h2s <= Quantity<MolePercent>(73.8)))
      ZENTHROW(OutOfParameterRange,
               "parameter h2s is greater than Quantity<MolePercent>(73.8)");

    return call(ppcm, tpcm, co2, h2s);
  }

  Quantity<psia> operator()(const Quantity<psia>& ppcm,
                            const Quantity<Rankine>& tpcm,
                            const Quantity<MoleFraction>& co2,
                            const Quantity<MoleFraction>& h2s) const {
    return Quantity<psia>(call(ppcm, tpcm, co2, h2s));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& ppcm = it.get_curr();
    it.next();
    const auto& tpcm = it.get_curr();
    it.next();
    const auto& co2 = it.get_curr();
    it.next();
    const auto& h2s = it.get_curr();

    return call(ppcm, tpcm, co2, h2s);
  }
};

extern Quantity<psia> AdjustedppcmWichertAziz__correlation__fct(
    const Quantity<psia>& ppcm, const Quantity<Rankine>& tpcm,
    const Quantity<MoleFraction>& co2, const Quantity<MoleFraction>& h2s);
extern double AdjustedppcmWichertAziz__correlation__valid__fct(
    const double& ppcm, const double& tpcm, const double& co2,
    const double& h2s);

/** TpcmKayMixingRule

    Generated at 2017-10-30 16:49:06 -0400

    Zen Technologies Group

The value of the pseudocritical temperature of the gas hydrocarbon portion is
adjusted for nonhydrocarbon content on the basis of Kay's mixing rule.

The original reference is not available. The correlation was verified by using
secondary references: Whitson & Brulé (2000), Standing (1977) and Bánzer (1996).
Date: September 12 2016.

References:

  - kay:1936

  - whitson:2000

  - standing:1977

@param[in] tpchc Gas pseudocritical temperature of the hydrocarbon portion.
Range in [, ]
@param[in] n2 N2 concentration. Range in [, ]
@param[in] co2 CO2 concentration. Range in [, ]
@param[in] h2s H2S concentration. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class TpcmKayMixingRule : public MixtureGasPseudocriticalTemperature {
  Correlation_Singleton(TpcmKayMixingRule);

  TpcmKayMixingRule()
      : MixtureGasPseudocriticalTemperature("TpcmKayMixingRule",
                                            Rankine::get_instance()) {
    set_author("Kay (Mixing Rule)");
    set_title("KAY'S MIXING RULE, CALCULATION OF PSEUDOCRITICAL TEMPERATURE OF "
              "THE WHOLE GAS MIXTURE");
    set_hidden_blackoil_grid();
    set_hidden_wetgas_grid();
    set_hidden_drygas_grid();
    add_parameter("tpchc", Rankine::get_instance(),
                  "Gas pseudocritical temperature of the hydrocarbon portion");
    def_parameter_latex_symbol("tpchc", "T_{pcHC}");
    add_parameter("n2", MoleFraction::get_instance(), "N2 concentration");
    add_parameter("co2", MoleFraction::get_instance(), "CO2 concentration");
    add_parameter("h2s", MoleFraction::get_instance(), "H2S concentration");
    add_note("The value of the pseudocritical temperature of the gas "
             "hydrocarbon portion is adjusted for nonhydrocarbon content on "
             "the basis of Kay's mixing rule.");
    add_ref("kay:1936");
    add_ref("whitson:2000");
    add_ref("standing:1977");
  }

  inline void precondition(const Quantity<MoleFraction>& n2,
                           const Quantity<MoleFraction>& co2,
                           const Quantity<MoleFraction>& h2s) const;

public:
  static inline double impl(const double& tpchc, const double& n2,
                            const double& co2, const double& h2s) noexcept;

  Quantity<Rankine> call(const Quantity<Rankine>& tpchc,
                         const Quantity<MoleFraction>& n2,
                         const Quantity<MoleFraction>& co2,
                         const Quantity<MoleFraction>& h2s) const {
    precondition(n2, co2, h2s);
    return Quantity<Rankine>(impl(tpchc.raw(), n2.raw(), co2.raw(), h2s.raw()));
  }

  Quantity<Rankine> validate_and_call(const Quantity<Rankine>& tpchc,
                                      const Quantity<MoleFraction>& n2,
                                      const Quantity<MoleFraction>& co2,
                                      const Quantity<MoleFraction>& h2s) const {

    return call(tpchc, n2, co2, h2s);
  }

  Quantity<Rankine> operator()(const Quantity<Rankine>& tpchc,
                               const Quantity<MoleFraction>& n2,
                               const Quantity<MoleFraction>& co2,
                               const Quantity<MoleFraction>& h2s) const {
    return Quantity<Rankine>(call(tpchc, n2, co2, h2s));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& tpchc = it.get_curr();
    it.next();
    const auto& n2 = it.get_curr();
    it.next();
    const auto& co2 = it.get_curr();
    it.next();
    const auto& h2s = it.get_curr();

    return call(tpchc, n2, co2, h2s);
  }
};

extern Quantity<Rankine> TpcmKayMixingRule__correlation__fct(
    const Quantity<Rankine>& tpchc, const Quantity<MoleFraction>& n2,
    const Quantity<MoleFraction>& co2, const Quantity<MoleFraction>& h2s);
extern double TpcmKayMixingRule__correlation__valid__fct(const double& tpchc,
                                                         const double& n2,
                                                         const double& co2,
                                                         const double& h2s);

/** AdjustedtpcmWichertAziz

    Generated at 2017-10-30 16:49:06 -0400

    Zen Technologies Group

The pseudocritical temperature of the mixture is adjusted by using the Wichert &
Aziz correlation when the gas contains significant fractions of acid gases,
specifically carbon dioxide and hydrogen sulfide.
The authors indicate that the correlation has an average absolute error of 0.97
% over this range: 40 °F < T < 300 °F.

The original reference is not available. The correlation was verified by using a
secondary reference: Standing (1977). Date: September 12 2016.

The development ranges were taken from PetroWiki
(http://petrowiki.org/Real_gases).

References:

  - wichert:1972

  - standing:1977

  - petroWiki:2016:6

@param[in] tpcm Gas pseudocritical temperature of the mixture. Range in [, ]
@param[in] co2 CO2 concentration. Range in [Quantity<MolePercent>(0),
Quantity<MolePercent>(54.4)]
@param[in] h2s H2S concentration. Range in [Quantity<MolePercent>(0),
Quantity<MolePercent>(73.8)]
@return inside [, ]
@ingroup Correlations
*/
class AdjustedtpcmWichertAziz : public MixtureGasPseudocriticalTemperature {
  Correlation_Singleton(AdjustedtpcmWichertAziz);

  AdjustedtpcmWichertAziz()
      : MixtureGasPseudocriticalTemperature("AdjustedtpcmWichertAziz",
                                            Rankine::get_instance()) {
    set_author("Wichert & Aziz (Mixture Correction)");
    set_title("WICHERT & AZIZ CORRELATION, CALCULATION OF PSEUDOCRITICAL "
              "TEMPERATURE OF THE WHOLE GAS MIXTURE");
    set_hidden_blackoil_grid();
    set_hidden_wetgas_grid();
    set_hidden_drygas_grid();
    add_parameter("tpcm", Rankine::get_instance(),
                  "Gas pseudocritical temperature of the mixture");
    def_parameter_latex_symbol("tpcm", "T_{pc}");
    add_parameter("co2", MoleFraction::get_instance(), "CO2 concentration",
                  Quantity<MolePercent>(0), Quantity<MolePercent>(54.4));
    add_parameter("h2s", MoleFraction::get_instance(), "H2S concentration",
                  Quantity<MolePercent>(0), Quantity<MolePercent>(73.8));
    add_note("The pseudocritical temperature of the mixture is adjusted by "
             "using the Wichert & Aziz correlation when the gas contains "
             "significant fractions of acid gases, specifically carbon dioxide "
             "and hydrogen sulfide.");
    add_note("The authors indicate that the correlation has an average "
             "absolute error of 0.97 % over this range: 40 °F < T < 300 °F.");
    add_ref("wichert:1972");
    add_ref("standing:1977");
    add_ref("petroWiki:2016:6");
  }

  inline void precondition(const Quantity<MoleFraction>& co2,
                           const Quantity<MoleFraction>& h2s) const;

public:
  static inline double impl(const double& tpcm, const double& co2,
                            const double& h2s) noexcept;

  Quantity<Rankine> call(const Quantity<Rankine>& tpcm,
                         const Quantity<MoleFraction>& co2,
                         const Quantity<MoleFraction>& h2s) const {
    precondition(co2, h2s);
    return Quantity<Rankine>(impl(tpcm.raw(), co2.raw(), h2s.raw()));
  }

  Quantity<Rankine> validate_and_call(const Quantity<Rankine>& tpcm,
                                      const Quantity<MoleFraction>& co2,
                                      const Quantity<MoleFraction>& h2s) const {
    if (not(co2 >= Quantity<MolePercent>(0)))
      ZENTHROW(OutOfParameterRange,
               "parameter co2 is less than Quantity<MolePercent>(0)");
    if (not(co2 <= Quantity<MolePercent>(54.4)))
      ZENTHROW(OutOfParameterRange,
               "parameter co2 is greater than Quantity<MolePercent>(54.4)");
    if (not(h2s >= Quantity<MolePercent>(0)))
      ZENTHROW(OutOfParameterRange,
               "parameter h2s is less than Quantity<MolePercent>(0)");
    if (not(h2s <= Quantity<MolePercent>(73.8)))
      ZENTHROW(OutOfParameterRange,
               "parameter h2s is greater than Quantity<MolePercent>(73.8)");

    return call(tpcm, co2, h2s);
  }

  Quantity<Rankine> operator()(const Quantity<Rankine>& tpcm,
                               const Quantity<MoleFraction>& co2,
                               const Quantity<MoleFraction>& h2s) const {
    return Quantity<Rankine>(call(tpcm, co2, h2s));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& tpcm = it.get_curr();
    it.next();
    const auto& co2 = it.get_curr();
    it.next();
    const auto& h2s = it.get_curr();

    return call(tpcm, co2, h2s);
  }
};

extern Quantity<Rankine> AdjustedtpcmWichertAziz__correlation__fct(
    const Quantity<Rankine>& tpcm, const Quantity<MoleFraction>& co2,
    const Quantity<MoleFraction>& h2s);
extern double AdjustedtpcmWichertAziz__correlation__valid__fct(
    const double& tpcm, const double& co2, const double& h2s);

#endif // NON_HYDROCAHORNS_CORRECTIONS_H
