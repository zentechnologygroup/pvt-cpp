#ifndef GAS_ISOTHERMAL_COMPRESSIBILITY_H
#define GAS_ISOTHERMAL_COMPRESSIBILITY_H

#include <pvt-exceptions.H>
#include <correlations/correlation.H>

/** CgSarem

    Generated at 2017-10-30 16:49:05 -0400

    Zen Technologies Group


The original reference is not available. The correlation was verified by using a
secondary reference: Bánzer (1996). Date: September 26 2016.

References:

  - sarem:1961

  - standing:1942

@param[in] tpr Gas pseudoreduced temperature. Range in [1.05, 2.95]
@param[in] ppr Gas pseudoreduced pressure. Range in [0.1, 14.9]
@param[in] ppc Gas pseudocritical pressure. Range in [, ]
@param[in] z Gas compressibility factor. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class CgSarem : public GasIsothermalCompressibility {
  Correlation_Singleton(CgSarem);

  CgSarem() : GasIsothermalCompressibility("CgSarem", psia_1::get_instance()) {
    set_author("Sarem");
    set_title(
        "SAREM FIT METHOD, CALCULATION OF GAS ISOTHERMAL COMPRESSIBILITY");
    add_parameter("tpr", PseudoReducedTemperature::get_instance(),
                  "Gas pseudoreduced temperature", 1.05, 2.95);
    def_parameter_latex_symbol("tpr", "T_{pr}");
    add_parameter("ppr", PseudoReducedPressure::get_instance(),
                  "Gas pseudoreduced pressure", 0.1, 14.9);
    def_parameter_latex_symbol("ppr", "P_{pr}");
    add_parameter("ppc", psia::get_instance(), "Gas pseudocritical pressure");
    def_parameter_latex_symbol("ppc", "P_{pc}");
    add_parameter("z", Zfactor::get_instance(), "Gas compressibility factor");
    def_parameter_latex_symbol("z", "Z\\ factor");
    add_db("The correlation is based on the partial derivative of the Z-factor "
           "equation with respect to the pseudoreduced pressure.");
    add_db("The Z-factor equation was obtained by using Legendre polynomials "
           "of up to five degree to fit the Standing-Katz curves.");
    add_ref("sarem:1961");
    add_ref("standing:1942");
  }

public:
  static inline double impl(const double& tpr, const double& ppr,
                            const double& ppc, const double& z) noexcept;

  Quantity<psia_1> call(const Quantity<PseudoReducedTemperature>& tpr,
                        const Quantity<PseudoReducedPressure>& ppr,
                        const Quantity<psia>& ppc,
                        const Quantity<Zfactor>& z) const {
    return Quantity<psia_1>(impl(tpr.raw(), ppr.raw(), ppc.raw(), z.raw()));
  }

  Quantity<psia_1> validate_and_call(
      const Quantity<PseudoReducedTemperature>& tpr,
      const Quantity<PseudoReducedPressure>& ppr, const Quantity<psia>& ppc,
      const Quantity<Zfactor>& z) const {
    if (not(tpr >= 1.05))
      ZENTHROW(OutOfParameterRange, "parameter tpr is less than 1.05");
    if (not(tpr <= 2.95))
      ZENTHROW(OutOfParameterRange, "parameter tpr is greater than 2.95");
    if (not(ppr >= 0.1))
      ZENTHROW(OutOfParameterRange, "parameter ppr is less than 0.1");
    if (not(ppr <= 14.9))
      ZENTHROW(OutOfParameterRange, "parameter ppr is greater than 14.9");

    return call(tpr, ppr, ppc, z);
  }

  Quantity<psia_1> operator()(const Quantity<PseudoReducedTemperature>& tpr,
                              const Quantity<PseudoReducedPressure>& ppr,
                              const Quantity<psia>& ppc,
                              const Quantity<Zfactor>& z) const {
    return Quantity<psia_1>(call(tpr, ppr, ppc, z));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& tpr = it.get_curr();
    it.next();
    const auto& ppr = it.get_curr();
    it.next();
    const auto& ppc = it.get_curr();
    it.next();
    const auto& z = it.get_curr();

    return call(tpr, ppr, ppc, z);
  }
};

extern Quantity<psia_1> CgSarem__correlation__fct(
    const Quantity<PseudoReducedTemperature>& tpr,
    const Quantity<PseudoReducedPressure>& ppr, const Quantity<psia>& ppc,
    const Quantity<Zfactor>& z);
extern double CgSarem__correlation__valid__fct(const double& tpr,
                                               const double& ppr,
                                               const double& ppc,
                                               const double& z);

/** CgHallYarborough

    Generated at 2017-10-30 16:49:05 -0400

    Zen Technologies Group


The original reference is not available. The correlation was verified by using a
secondary reference: Bánzer (1996). Date: September 27 2016.

The application ranges and data bank information was obtained from Takacs
(1989).

References:

  - hall:1973

  - yarborough:1974

  - standing:1942

@param[in] tpr Gas pseudoreduced temperature. Range in [1.2, 3.0]
@param[in] ppr Gas pseudoreduced pressure. Range in [0.1, 24.0]
@param[in] ppc Gas pseudocritical pressure. Range in [, ]
@param[in] z Gas compressibility factor. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class CgHallYarborough : public GasIsothermalCompressibility {
  Correlation_Singleton(CgHallYarborough);

  CgHallYarborough()
      : GasIsothermalCompressibility("CgHallYarborough",
                                     psia_1::get_instance()) {
    set_author("Hall & Yarborough");
    set_title("HALL & YARBOROUGH FIT METHOD, CALCULATION OF GAS ISOTHERMAL "
              "COMPRESSIBILITY");
    add_parameter("tpr", PseudoReducedTemperature::get_instance(),
                  "Gas pseudoreduced temperature", 1.2, 3.0);
    def_parameter_latex_symbol("tpr", "T_{pr}");
    add_parameter("ppr", PseudoReducedPressure::get_instance(),
                  "Gas pseudoreduced pressure", 0.1, 24.0);
    def_parameter_latex_symbol("ppr", "P_{pr}");
    add_parameter("ppc", psia::get_instance(), "Gas pseudocritical pressure");
    def_parameter_latex_symbol("ppc", "P_{pc}");
    add_parameter("z", Zfactor::get_instance(), "Gas compressibility factor");
    def_parameter_latex_symbol("z", "Z\\ factor");
    add_db("The correlation is based on the partial derivative of the Z-factor "
           "equation with respect to the pseudoreduced pressure.");
    add_db("The Z-factor correlation is based on the equation of state of "
           "Starling-Carnahan. The coefficients of the equation were obtained "
           "by fitting the Standing-Katz curves.");
    add_db(
        "The equation is solved by using the Newton-Raphson iteration method.");
    add_ref("hall:1973");
    add_ref("yarborough:1974");
    add_ref("standing:1942");
  }

public:
  static inline double impl(const double& tpr, const double& ppr,
                            const double& ppc, const double& z) noexcept;

  Quantity<psia_1> call(const Quantity<PseudoReducedTemperature>& tpr,
                        const Quantity<PseudoReducedPressure>& ppr,
                        const Quantity<psia>& ppc,
                        const Quantity<Zfactor>& z) const {
    return Quantity<psia_1>(impl(tpr.raw(), ppr.raw(), ppc.raw(), z.raw()));
  }

  Quantity<psia_1> validate_and_call(
      const Quantity<PseudoReducedTemperature>& tpr,
      const Quantity<PseudoReducedPressure>& ppr, const Quantity<psia>& ppc,
      const Quantity<Zfactor>& z) const {
    if (not(tpr >= 1.2))
      ZENTHROW(OutOfParameterRange, "parameter tpr is less than 1.2");
    if (not(tpr <= 3.0))
      ZENTHROW(OutOfParameterRange, "parameter tpr is greater than 3.0");
    if (not(ppr >= 0.1))
      ZENTHROW(OutOfParameterRange, "parameter ppr is less than 0.1");
    if (not(ppr <= 24.0))
      ZENTHROW(OutOfParameterRange, "parameter ppr is greater than 24.0");

    return call(tpr, ppr, ppc, z);
  }

  Quantity<psia_1> operator()(const Quantity<PseudoReducedTemperature>& tpr,
                              const Quantity<PseudoReducedPressure>& ppr,
                              const Quantity<psia>& ppc,
                              const Quantity<Zfactor>& z) const {
    return Quantity<psia_1>(call(tpr, ppr, ppc, z));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& tpr = it.get_curr();
    it.next();
    const auto& ppr = it.get_curr();
    it.next();
    const auto& ppc = it.get_curr();
    it.next();
    const auto& z = it.get_curr();

    return call(tpr, ppr, ppc, z);
  }
};

extern Quantity<psia_1> CgHallYarborough__correlation__fct(
    const Quantity<PseudoReducedTemperature>& tpr,
    const Quantity<PseudoReducedPressure>& ppr, const Quantity<psia>& ppc,
    const Quantity<Zfactor>& z);
extern double CgHallYarborough__correlation__valid__fct(const double& tpr,
                                                        const double& ppr,
                                                        const double& ppc,
                                                        const double& z);

/** CgMattarBA

    Generated at 2017-10-30 16:49:05 -0400

    Zen Technologies Group


The original reference is not available. The correlation was verified by using a
secondary reference: Bánzer (1996). Date: September 27 2016.

The application ranges and data bank information was obtained from Takacs
(1989).

References:

  - mattar:1975

  - dranchuk:1973

  - standing:1942

  - takacs:1989

@param[in] tpr Gas pseudoreduced temperature. Range in [1.05, 3]
@param[in] ppr Gas pseudoreduced pressure. Range in [0.2, 30]
@param[in] ppc Gas pseudocritical pressure. Range in [, ]
@param[in] z Gas compressibility factor. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class CgMattarBA : public GasIsothermalCompressibility {
  Correlation_Singleton(CgMattarBA);

  CgMattarBA()
      : GasIsothermalCompressibility("CgMattarBA", psia_1::get_instance()) {
    set_author("Mattar, Brar & Aziz");
    set_title("MATTAR, BRAR & AZIZ FIT METHOD, CALCULATION OF GAS ISOTHERMAL "
              "COMPRESSIBILITY");
    add_parameter("tpr", PseudoReducedTemperature::get_instance(),
                  "Gas pseudoreduced temperature", 1.05, 3);
    def_parameter_latex_symbol("tpr", "T_{pr}");
    add_parameter("ppr", PseudoReducedPressure::get_instance(),
                  "Gas pseudoreduced pressure", 0.2, 30);
    def_parameter_latex_symbol("ppr", "P_{pr}");
    add_parameter("ppc", psia::get_instance(), "Gas pseudocritical pressure");
    def_parameter_latex_symbol("ppc", "P_{pc}");
    add_parameter("z", Zfactor::get_instance(), "Gas compressibility factor");
    def_parameter_latex_symbol("z", "Z\\ factor");
    add_db("The correlation is based on the partial derivative of the Z-factor "
           "equation with respect to the pseudoreduced pressure.");
    add_db("The Z-factor correlation was obtained by Dranchuk, Purvis & "
           "Robinson, as a result of fitting the Benedict-Webb-Rubin (BWR) "
           "equation of state. 1500 data points from the original "
           "Standing-Katz chart were used.");
    add_ref("mattar:1975");
    add_ref("dranchuk:1973");
    add_ref("standing:1942");
    add_ref("takacs:1989");
  }

public:
  static inline double impl(const double& tpr, const double& ppr,
                            const double& ppc, const double& z) noexcept;

  Quantity<psia_1> call(const Quantity<PseudoReducedTemperature>& tpr,
                        const Quantity<PseudoReducedPressure>& ppr,
                        const Quantity<psia>& ppc,
                        const Quantity<Zfactor>& z) const {
    return Quantity<psia_1>(impl(tpr.raw(), ppr.raw(), ppc.raw(), z.raw()));
  }

  Quantity<psia_1> validate_and_call(
      const Quantity<PseudoReducedTemperature>& tpr,
      const Quantity<PseudoReducedPressure>& ppr, const Quantity<psia>& ppc,
      const Quantity<Zfactor>& z) const {
    if (not(tpr >= 1.05))
      ZENTHROW(OutOfParameterRange, "parameter tpr is less than 1.05");
    if (not(tpr <= 3))
      ZENTHROW(OutOfParameterRange, "parameter tpr is greater than 3");
    if (not(ppr >= 0.2))
      ZENTHROW(OutOfParameterRange, "parameter ppr is less than 0.2");
    if (not(ppr <= 30))
      ZENTHROW(OutOfParameterRange, "parameter ppr is greater than 30");

    return call(tpr, ppr, ppc, z);
  }

  Quantity<psia_1> operator()(const Quantity<PseudoReducedTemperature>& tpr,
                              const Quantity<PseudoReducedPressure>& ppr,
                              const Quantity<psia>& ppc,
                              const Quantity<Zfactor>& z) const {
    return Quantity<psia_1>(call(tpr, ppr, ppc, z));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& tpr = it.get_curr();
    it.next();
    const auto& ppr = it.get_curr();
    it.next();
    const auto& ppc = it.get_curr();
    it.next();
    const auto& z = it.get_curr();

    return call(tpr, ppr, ppc, z);
  }
};

extern Quantity<psia_1> CgMattarBA__correlation__fct(
    const Quantity<PseudoReducedTemperature>& tpr,
    const Quantity<PseudoReducedPressure>& ppr, const Quantity<psia>& ppc,
    const Quantity<Zfactor>& z);
extern double CgMattarBA__correlation__valid__fct(const double& tpr,
                                                  const double& ppr,
                                                  const double& ppc,
                                                  const double& z);

/** CgGopal

    Generated at 2017-10-30 16:49:05 -0400

    Zen Technologies Group


The original reference is not available. The correlation was verified by using a
secondary reference: Bánzer (1996). Date: September 27 2016.

The application ranges and data bank information was obtained from Takacs
(1989).

References:

  - gopal:1977

  - standing:1942

  - takacs:1989

@param[in] tpr Gas pseudoreduced temperature. Range in [1.05, 3]
@param[in] ppr Gas pseudoreduced pressure. Range in [0.2, 15]
@param[in] ppc Gas pseudocritical pressure. Range in [, ]
@param[in] z Gas compressibility factor. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class CgGopal : public GasIsothermalCompressibility {
  Correlation_Singleton(CgGopal);

  CgGopal() : GasIsothermalCompressibility("CgGopal", psia_1::get_instance()) {
    set_author("Gopal");
    set_title(
        "GOPAL FIT METHOD, CALCULATION OF GAS ISOTHERMAL COMPRESSIBILITY");
    add_parameter("tpr", PseudoReducedTemperature::get_instance(),
                  "Gas pseudoreduced temperature", 1.05, 3);
    def_parameter_latex_symbol("tpr", "T_{pr}");
    add_parameter("ppr", PseudoReducedPressure::get_instance(),
                  "Gas pseudoreduced pressure", 0.2, 15);
    def_parameter_latex_symbol("ppr", "P_{pr}");
    add_parameter("ppc", psia::get_instance(), "Gas pseudocritical pressure");
    def_parameter_latex_symbol("ppc", "P_{pc}");
    add_parameter("z", Zfactor::get_instance(), "Gas compressibility factor");
    def_parameter_latex_symbol("z", "Z\\ factor");
    add_db("The correlation is based on the partial derivative of the Z-factor "
           "equations with respect to the pseudoreduced pressure.");
    add_db("The Z-factor equations presented by Gopal were obtained by fitting "
           "the Standing-Katz curves.");
    add_ref("gopal:1977");
    add_ref("standing:1942");
    add_ref("takacs:1989");
  }

public:
  static inline double impl(const double& tpr, const double& ppr,
                            const double& ppc, const double& z) noexcept;

  Quantity<psia_1> call(const Quantity<PseudoReducedTemperature>& tpr,
                        const Quantity<PseudoReducedPressure>& ppr,
                        const Quantity<psia>& ppc,
                        const Quantity<Zfactor>& z) const {
    return Quantity<psia_1>(impl(tpr.raw(), ppr.raw(), ppc.raw(), z.raw()));
  }

  Quantity<psia_1> validate_and_call(
      const Quantity<PseudoReducedTemperature>& tpr,
      const Quantity<PseudoReducedPressure>& ppr, const Quantity<psia>& ppc,
      const Quantity<Zfactor>& z) const {
    if (not(tpr >= 1.05))
      ZENTHROW(OutOfParameterRange, "parameter tpr is less than 1.05");
    if (not(tpr <= 3))
      ZENTHROW(OutOfParameterRange, "parameter tpr is greater than 3");
    if (not(ppr >= 0.2))
      ZENTHROW(OutOfParameterRange, "parameter ppr is less than 0.2");
    if (not(ppr <= 15))
      ZENTHROW(OutOfParameterRange, "parameter ppr is greater than 15");

    return call(tpr, ppr, ppc, z);
  }

  Quantity<psia_1> operator()(const Quantity<PseudoReducedTemperature>& tpr,
                              const Quantity<PseudoReducedPressure>& ppr,
                              const Quantity<psia>& ppc,
                              const Quantity<Zfactor>& z) const {
    return Quantity<psia_1>(call(tpr, ppr, ppc, z));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& tpr = it.get_curr();
    it.next();
    const auto& ppr = it.get_curr();
    it.next();
    const auto& ppc = it.get_curr();
    it.next();
    const auto& z = it.get_curr();

    return call(tpr, ppr, ppc, z);
  }
};

extern Quantity<psia_1> CgGopal__correlation__fct(
    const Quantity<PseudoReducedTemperature>& tpr,
    const Quantity<PseudoReducedPressure>& ppr, const Quantity<psia>& ppc,
    const Quantity<Zfactor>& z);
extern double CgGopal__correlation__valid__fct(const double& tpr,
                                               const double& ppr,
                                               const double& ppc,
                                               const double& z);

/** CgBrillBeggs

    Generated at 2017-10-30 16:49:05 -0400

    Zen Technologies Group


The original reference is not available. The correlation was verified by using a
secondary reference: Bánzer (1996). Date: September 27 2016.

References:

  - brill:1974

  - standing:1942

@param[in] tpr Gas pseudoreduced temperature. Range in [1.2, 2.4]
@param[in] ppr Gas pseudoreduced pressure. Range in [0, 13]
@param[in] ppc Gas pseudocritical pressure. Range in [, ]
@param[in] z Gas compressibility factor. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class CgBrillBeggs : public GasIsothermalCompressibility {
  Correlation_Singleton(CgBrillBeggs);

  CgBrillBeggs()
      : GasIsothermalCompressibility("CgBrillBeggs", psia_1::get_instance()) {
    set_author("Brill & Beggs");
    set_title("BRILL & BEGGS FIT METHOD, CALCULATION OF GAS ISOTHERMAL "
              "COMPRESSIBILITY");
    add_parameter("tpr", PseudoReducedTemperature::get_instance(),
                  "Gas pseudoreduced temperature", 1.2, 2.4);
    def_parameter_latex_symbol("tpr", "T_{pr}");
    add_parameter("ppr", PseudoReducedPressure::get_instance(),
                  "Gas pseudoreduced pressure", 0, 13);
    def_parameter_latex_symbol("ppr", "P_{pr}");
    add_parameter("ppc", psia::get_instance(), "Gas pseudocritical pressure");
    def_parameter_latex_symbol("ppc", "P_{pc}");
    add_parameter("z", Zfactor::get_instance(), "Gas compressibility factor");
    def_parameter_latex_symbol("z", "Z\\ factor");
    add_db("The correlation is based on the partial derivative of the Z-factor "
           "equation with respect to the pseudoreduced pressure.");
    add_db("The Z-factor correlation was obtained by applying adjustment "
           "methods to the Standing-Katz curves.");
    add_ref("brill:1974");
    add_ref("standing:1942");
  }

public:
  static inline double impl(const double& tpr, const double& ppr,
                            const double& ppc, const double& z) noexcept;

  Quantity<psia_1> call(const Quantity<PseudoReducedTemperature>& tpr,
                        const Quantity<PseudoReducedPressure>& ppr,
                        const Quantity<psia>& ppc,
                        const Quantity<Zfactor>& z) const {
    return Quantity<psia_1>(impl(tpr.raw(), ppr.raw(), ppc.raw(), z.raw()));
  }

  Quantity<psia_1> validate_and_call(
      const Quantity<PseudoReducedTemperature>& tpr,
      const Quantity<PseudoReducedPressure>& ppr, const Quantity<psia>& ppc,
      const Quantity<Zfactor>& z) const {
    if (not(tpr >= 1.2))
      ZENTHROW(OutOfParameterRange, "parameter tpr is less than 1.2");
    if (not(tpr <= 2.4))
      ZENTHROW(OutOfParameterRange, "parameter tpr is greater than 2.4");
    if (not(ppr >= 0))
      ZENTHROW(OutOfParameterRange, "parameter ppr is less than 0");
    if (not(ppr <= 13))
      ZENTHROW(OutOfParameterRange, "parameter ppr is greater than 13");

    return call(tpr, ppr, ppc, z);
  }

  Quantity<psia_1> operator()(const Quantity<PseudoReducedTemperature>& tpr,
                              const Quantity<PseudoReducedPressure>& ppr,
                              const Quantity<psia>& ppc,
                              const Quantity<Zfactor>& z) const {
    return Quantity<psia_1>(call(tpr, ppr, ppc, z));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& tpr = it.get_curr();
    it.next();
    const auto& ppr = it.get_curr();
    it.next();
    const auto& ppc = it.get_curr();
    it.next();
    const auto& z = it.get_curr();

    return call(tpr, ppr, ppc, z);
  }
};

extern Quantity<psia_1> CgBrillBeggs__correlation__fct(
    const Quantity<PseudoReducedTemperature>& tpr,
    const Quantity<PseudoReducedPressure>& ppr, const Quantity<psia>& ppc,
    const Quantity<Zfactor>& z);
extern double CgBrillBeggs__correlation__valid__fct(const double& tpr,
                                                    const double& ppr,
                                                    const double& ppc,
                                                    const double& z);

/** CgPapay

    Generated at 2017-10-30 16:49:05 -0400

    Zen Technologies Group


References:

  - papay:1968

  - standing:1942

  - takacs:1989

@param[in] tpr Gas pseudoreduced temperature. Range in [1.2, 3]
@param[in] ppr Gas pseudoreduced pressure. Range in [0.2, 15]
@param[in] ppc Gas pseudocritical pressure. Range in [, ]
@param[in] z Gas compressibility factor. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class CgPapay : public GasIsothermalCompressibility {
  Correlation_Singleton(CgPapay);

  CgPapay() : GasIsothermalCompressibility("CgPapay", psia_1::get_instance()) {
    set_author("Pápay");
    set_title(
        "PÁPAY FIT METHOD, CALCULATION OF GAS ISOTHERMAL COMPRESSIBILITY ");
    add_parameter("tpr", PseudoReducedTemperature::get_instance(),
                  "Gas pseudoreduced temperature", 1.2, 3);
    def_parameter_latex_symbol("tpr", "T_{pr}");
    add_parameter("ppr", PseudoReducedPressure::get_instance(),
                  "Gas pseudoreduced pressure", 0.2, 15);
    def_parameter_latex_symbol("ppr", "P_{pr}");
    add_parameter("ppc", psia::get_instance(), "Gas pseudocritical pressure");
    def_parameter_latex_symbol("ppc", "P_{pc}");
    add_parameter("z", Zfactor::get_instance(), "Gas compressibility factor");
    def_parameter_latex_symbol("z", "Z\\ factor");
    add_db("The correlation is based on the partial derivative of the Z-factor "
           "equation with respect to the pseudoreduced pressure.");
    add_db("The Z-factor equation resulted from applying methods of adjustment "
           "to the Standing-Katz curves.");
    add_ref("papay:1968");
    add_ref("standing:1942");
    add_ref("takacs:1989");
  }

public:
  static inline double impl(const double& tpr, const double& ppr,
                            const double& ppc, const double& z) noexcept;

  Quantity<psia_1> call(const Quantity<PseudoReducedTemperature>& tpr,
                        const Quantity<PseudoReducedPressure>& ppr,
                        const Quantity<psia>& ppc,
                        const Quantity<Zfactor>& z) const {
    return Quantity<psia_1>(impl(tpr.raw(), ppr.raw(), ppc.raw(), z.raw()));
  }

  Quantity<psia_1> validate_and_call(
      const Quantity<PseudoReducedTemperature>& tpr,
      const Quantity<PseudoReducedPressure>& ppr, const Quantity<psia>& ppc,
      const Quantity<Zfactor>& z) const {
    if (not(tpr >= 1.2))
      ZENTHROW(OutOfParameterRange, "parameter tpr is less than 1.2");
    if (not(tpr <= 3))
      ZENTHROW(OutOfParameterRange, "parameter tpr is greater than 3");
    if (not(ppr >= 0.2))
      ZENTHROW(OutOfParameterRange, "parameter ppr is less than 0.2");
    if (not(ppr <= 15))
      ZENTHROW(OutOfParameterRange, "parameter ppr is greater than 15");

    return call(tpr, ppr, ppc, z);
  }

  Quantity<psia_1> operator()(const Quantity<PseudoReducedTemperature>& tpr,
                              const Quantity<PseudoReducedPressure>& ppr,
                              const Quantity<psia>& ppc,
                              const Quantity<Zfactor>& z) const {
    return Quantity<psia_1>(call(tpr, ppr, ppc, z));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& tpr = it.get_curr();
    it.next();
    const auto& ppr = it.get_curr();
    it.next();
    const auto& ppc = it.get_curr();
    it.next();
    const auto& z = it.get_curr();

    return call(tpr, ppr, ppc, z);
  }
};

extern Quantity<psia_1> CgPapay__correlation__fct(
    const Quantity<PseudoReducedTemperature>& tpr,
    const Quantity<PseudoReducedPressure>& ppr, const Quantity<psia>& ppc,
    const Quantity<Zfactor>& z);
extern double CgPapay__correlation__valid__fct(const double& tpr,
                                               const double& ppr,
                                               const double& ppc,
                                               const double& z);

#endif // GAS_ISOTHERMAL_COMPRESSIBILITY_H
