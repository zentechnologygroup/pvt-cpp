#ifndef GAS_COMPRESSIBILITY_H
#define GAS_COMPRESSIBILITY_H

#include <pvt-exceptions.H>
#include <correlations/correlation.H>

/** ZfactorSarem

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group


The original reference is not available. The correlation was verified by using a
secondary reference: Bánzer (1996). Date: September 22 2016.

References:

  - sarem:1961

  - standing:1942

@param[in] tpr Gas pseudoreduced temperature. Range in [1.05, 2.95]
@param[in] ppr Gas pseudoreduced pressure. Range in [0.1, 14.9]
@return inside [, ]
@ingroup Correlations
*/
class ZfactorSarem : public GasCompressibility {
  Correlation_Singleton(ZfactorSarem);

  ZfactorSarem() : GasCompressibility("ZfactorSarem", Zfactor::get_instance()) {
    set_author("Sarem");
    set_title(
        "SAREM FIT METHOD, CALCULATION OF THE GAS COMPRESSIBILITY FACTOR");
    add_parameter("tpr", PseudoReducedTemperature::get_instance(),
                  "Gas pseudoreduced temperature", 1.05, 2.95);
    def_parameter_latex_symbol("tpr", "T_{pr}");
    add_parameter("ppr", PseudoReducedPressure::get_instance(),
                  "Gas pseudoreduced pressure", 0.1, 14.9);
    def_parameter_latex_symbol("ppr", "P_{pr}");
    add_db("The correlation was obtained by using Legendre polynomials of up "
           "to five degree to fit the Standing-Katz curves for the gas "
           "compressibility factor (Z).");
    add_ref("sarem:1961");
    add_ref("standing:1942");
  }

public:
  static inline double impl(const double& tpr, const double& ppr) noexcept;

  Quantity<Zfactor> call(const Quantity<PseudoReducedTemperature>& tpr,
                         const Quantity<PseudoReducedPressure>& ppr) const {
    return Quantity<Zfactor>(impl(tpr.raw(), ppr.raw()));
  }

  Quantity<Zfactor> validate_and_call(
      const Quantity<PseudoReducedTemperature>& tpr,
      const Quantity<PseudoReducedPressure>& ppr) const {
    if (not(tpr >= 1.05))
      ZENTHROW(OutOfParameterRange, "parameter tpr is less than 1.05");
    if (not(tpr <= 2.95))
      ZENTHROW(OutOfParameterRange, "parameter tpr is greater than 2.95");
    if (not(ppr >= 0.1))
      ZENTHROW(OutOfParameterRange, "parameter ppr is less than 0.1");
    if (not(ppr <= 14.9))
      ZENTHROW(OutOfParameterRange, "parameter ppr is greater than 14.9");

    return call(tpr, ppr);
  }

  Quantity<Zfactor> operator()(
      const Quantity<PseudoReducedTemperature>& tpr,
      const Quantity<PseudoReducedPressure>& ppr) const {
    return Quantity<Zfactor>(call(tpr, ppr));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& tpr = it.get_curr();
    it.next();
    const auto& ppr = it.get_curr();

    return call(tpr, ppr);
  }
};

extern Quantity<Zfactor> ZfactorSarem__correlation__fct(
    const Quantity<PseudoReducedTemperature>& tpr,
    const Quantity<PseudoReducedPressure>& ppr);
extern double ZfactorSarem__correlation__valid__fct(const double& tpr,
                                                    const double& ppr);

/** ZfactorHallYarborough

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group


The original reference is not available. The correlation was verified by using
secondary references: Bánzer (1996) and Whitson & Brulé (2000). Date: September
23 2016.

The application ranges and data bank information was obtained from Takacs
(1989).

References:

  - hall:1973

  - yarborough:1974

  - standing:1942

  - whitson:2000

@param[in] tpr Gas pseudoreduced temperature. Range in [1.2, 3]
@param[in] ppr Gas pseudoreduced pressure. Range in [0.1, 24]
@return inside [, ]
@ingroup Correlations
*/
class ZfactorHallYarborough : public GasCompressibility {
  Correlation_Singleton(ZfactorHallYarborough);

  ZfactorHallYarborough()
      : GasCompressibility("ZfactorHallYarborough", Zfactor::get_instance()) {
    set_author("Hall & Yarborough");
    set_title("HALL & YARBOROUGH FIT METHOD, CALCULATION OF THE GAS "
              "COMPRESSIBILITY FACTOR");
    add_parameter("tpr", PseudoReducedTemperature::get_instance(),
                  "Gas pseudoreduced temperature", 1.2, 3);
    def_parameter_latex_symbol("tpr", "T_{pr}");
    add_parameter("ppr", PseudoReducedPressure::get_instance(),
                  "Gas pseudoreduced pressure", 0.1, 24);
    def_parameter_latex_symbol("ppr", "P_{pr}");
    add_db("Based on the equation of state of Starling-Carnahan for the "
           "calculation of Z-factor. The coefficients of the equation were "
           "obtained by fitting the Standing-Katz curves.");
    add_db(
        "The equation is solved by using the Newton-Raphson iteration method.");
    add_ref("hall:1973");
    add_ref("yarborough:1974");
    add_ref("standing:1942");
    add_ref("whitson:2000");
  }

public:
  static inline double impl(const double& tpr, const double& ppr) noexcept;

  Quantity<Zfactor> call(const Quantity<PseudoReducedTemperature>& tpr,
                         const Quantity<PseudoReducedPressure>& ppr) const {
    return Quantity<Zfactor>(impl(tpr.raw(), ppr.raw()));
  }

  Quantity<Zfactor> validate_and_call(
      const Quantity<PseudoReducedTemperature>& tpr,
      const Quantity<PseudoReducedPressure>& ppr) const {
    if (not(tpr >= 1.2))
      ZENTHROW(OutOfParameterRange, "parameter tpr is less than 1.2");
    if (not(tpr <= 3))
      ZENTHROW(OutOfParameterRange, "parameter tpr is greater than 3");
    if (not(ppr >= 0.1))
      ZENTHROW(OutOfParameterRange, "parameter ppr is less than 0.1");
    if (not(ppr <= 24))
      ZENTHROW(OutOfParameterRange, "parameter ppr is greater than 24");

    return call(tpr, ppr);
  }

  Quantity<Zfactor> operator()(
      const Quantity<PseudoReducedTemperature>& tpr,
      const Quantity<PseudoReducedPressure>& ppr) const {
    return Quantity<Zfactor>(call(tpr, ppr));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& tpr = it.get_curr();
    it.next();
    const auto& ppr = it.get_curr();

    return call(tpr, ppr);
  }
};

extern Quantity<Zfactor> ZfactorHallYarborough__correlation__fct(
    const Quantity<PseudoReducedTemperature>& tpr,
    const Quantity<PseudoReducedPressure>& ppr);
extern double ZfactorHallYarborough__correlation__valid__fct(const double& tpr,
                                                             const double& ppr);

/** ZfactorDranchukPR

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group


The correlation was verified by using the original reference and a secondary
one: Bánzer (1996). Date: September 28 2016.

The application ranges and data bank information was obtained from Takacs
(1989).

References:

  - dranchuk:1973

  - standing:1942

  - takacs:1989

@param[in] tpr Gas pseudoreduced temperature. Range in [1.05, 3]
@param[in] ppr Gas pseudoreduced pressure. Range in [0.2, 30]
@return inside [, ]
@ingroup Correlations
*/
class ZfactorDranchukPR : public GasCompressibility {
  Correlation_Singleton(ZfactorDranchukPR);

  ZfactorDranchukPR()
      : GasCompressibility("ZfactorDranchukPR", Zfactor::get_instance()) {
    set_author("Dranchuk, Purvis & Robinson");
    set_title("DRANCHUK, PURVIS & ROBINSON FIT METHOD, CALCULATION OF THE GAS "
              "COMPRESSIBILITY FACTOR");
    add_parameter("tpr", PseudoReducedTemperature::get_instance(),
                  "Gas pseudoreduced temperature", 1.05, 3);
    def_parameter_latex_symbol("tpr", "T_{pr}");
    add_parameter("ppr", PseudoReducedPressure::get_instance(),
                  "Gas pseudoreduced pressure", 0.2, 30);
    def_parameter_latex_symbol("ppr", "P_{pr}");
    add_db("This method is a result of fitting the Benedict-Webb-Rubin (BWR) "
           "equation of state for the Z-factor. 1500 data points from the "
           "original Standing-Katz chart were used.");
    add_db(
        "The equation is solved by using the Newton-Raphson iteration method.");
    add_ref("dranchuk:1973");
    add_ref("standing:1942");
    add_ref("takacs:1989");
  }

public:
  static inline double impl(const double& tpr, const double& ppr) noexcept;

  Quantity<Zfactor> call(const Quantity<PseudoReducedTemperature>& tpr,
                         const Quantity<PseudoReducedPressure>& ppr) const {
    return Quantity<Zfactor>(impl(tpr.raw(), ppr.raw()));
  }

  Quantity<Zfactor> validate_and_call(
      const Quantity<PseudoReducedTemperature>& tpr,
      const Quantity<PseudoReducedPressure>& ppr) const {
    if (not(tpr >= 1.05))
      ZENTHROW(OutOfParameterRange, "parameter tpr is less than 1.05");
    if (not(tpr <= 3))
      ZENTHROW(OutOfParameterRange, "parameter tpr is greater than 3");
    if (not(ppr >= 0.2))
      ZENTHROW(OutOfParameterRange, "parameter ppr is less than 0.2");
    if (not(ppr <= 30))
      ZENTHROW(OutOfParameterRange, "parameter ppr is greater than 30");

    return call(tpr, ppr);
  }

  Quantity<Zfactor> operator()(
      const Quantity<PseudoReducedTemperature>& tpr,
      const Quantity<PseudoReducedPressure>& ppr) const {
    return Quantity<Zfactor>(call(tpr, ppr));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& tpr = it.get_curr();
    it.next();
    const auto& ppr = it.get_curr();

    return call(tpr, ppr);
  }
};

extern Quantity<Zfactor> ZfactorDranchukPR__correlation__fct(
    const Quantity<PseudoReducedTemperature>& tpr,
    const Quantity<PseudoReducedPressure>& ppr);
extern double ZfactorDranchukPR__correlation__valid__fct(const double& tpr,
                                                         const double& ppr);

/** ZfactorDranchukAK

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group

The ranges of application are: 1 <= Tpr <= 3 for 0.2 <= Ppr <= 30 and 0.7 <= Tpr
< 1 for 0 <= Ppr <= 1.

The original reference is not available. The correlation was verified by using a
secondary reference: Bánzer (1996). Date: September 23 2016.

The application ranges and data bank information was obtained from Takacs
(1989).

The lower limit for Ppr (when Tpr's range is from 0.7 to 1.0) was taken from the
development ranges of the correlation presented by Standing & Katz (1942).

References:

  - dranchuk:1975

  - standing:1942

  - takacs:1989

@param[in] tpr Gas pseudoreduced temperature. Range in [0.7, 3]
@param[in] ppr Gas pseudoreduced pressure. Range in [0, 30]
@return inside [, ]
@ingroup Correlations
*/
class ZfactorDranchukAK : public GasCompressibility {
  Correlation_Singleton(ZfactorDranchukAK);

  ZfactorDranchukAK()
      : GasCompressibility("ZfactorDranchukAK", Zfactor::get_instance()) {
    set_author("Dranchuk & Abou-Kassem");
    set_title("DRANCHUK & ABOU-KASSEM FIT METHOD, CALCULATION OF THE GAS "
              "COMPRESSIBILITY FACTOR");
    add_parameter("tpr", PseudoReducedTemperature::get_instance(),
                  "Gas pseudoreduced temperature", 0.7, 3);
    def_parameter_latex_symbol("tpr", "T_{pr}");
    add_parameter("ppr", PseudoReducedPressure::get_instance(),
                  "Gas pseudoreduced pressure", 0, 30);
    def_parameter_latex_symbol("ppr", "P_{pr}");
    add_db("This method is a result of fitting the Starling-Carnahan equation "
           "of state for the Z-factor. 1500 data points from the original "
           "Standing-Katz chart were used.");
    add_db(
        "The equation is solved by using the Newton-Raphson iteration method.");
    add_note("The ranges of application are: 1 <= Tpr <= 3 for 0.2 <= Ppr <= "
             "30 and 0.7 <= Tpr < 1 for 0 <= Ppr <= 1.");
    add_ref("dranchuk:1975");
    add_ref("standing:1942");
    add_ref("takacs:1989");
  }

public:
  static inline double impl(const double& tpr, const double& ppr) noexcept;

  Quantity<Zfactor> call(const Quantity<PseudoReducedTemperature>& tpr,
                         const Quantity<PseudoReducedPressure>& ppr) const {
    return Quantity<Zfactor>(impl(tpr.raw(), ppr.raw()));
  }

  Quantity<Zfactor> validate_and_call(
      const Quantity<PseudoReducedTemperature>& tpr,
      const Quantity<PseudoReducedPressure>& ppr) const {
    if (not(tpr >= 0.7))
      ZENTHROW(OutOfParameterRange, "parameter tpr is less than 0.7");
    if (not(tpr <= 3))
      ZENTHROW(OutOfParameterRange, "parameter tpr is greater than 3");
    if (not(ppr >= 0))
      ZENTHROW(OutOfParameterRange, "parameter ppr is less than 0");
    if (not(ppr <= 30))
      ZENTHROW(OutOfParameterRange, "parameter ppr is greater than 30");

    return call(tpr, ppr);
  }

  Quantity<Zfactor> operator()(
      const Quantity<PseudoReducedTemperature>& tpr,
      const Quantity<PseudoReducedPressure>& ppr) const {
    return Quantity<Zfactor>(call(tpr, ppr));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& tpr = it.get_curr();
    it.next();
    const auto& ppr = it.get_curr();

    return call(tpr, ppr);
  }
};

extern Quantity<Zfactor> ZfactorDranchukAK__correlation__fct(
    const Quantity<PseudoReducedTemperature>& tpr,
    const Quantity<PseudoReducedPressure>& ppr);
extern double ZfactorDranchukAK__correlation__valid__fct(const double& tpr,
                                                         const double& ppr);

/** ZfactorGopal

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group


The original reference is not available. The correlation was verified by using a
secondary reference: Bánzer (1996). Date: September 23 2016.

The application ranges and data bank information was obtained from Takacs
(1989).

References:

  - gopal:1977

  - standing:1942

  - takacs:1989

@param[in] tpr Gas pseudoreduced temperature. Range in [1.05, 3]
@param[in] ppr Gas pseudoreduced pressure. Range in [0.2, 15]
@return inside [, ]
@ingroup Correlations
*/
class ZfactorGopal : public GasCompressibility {
  Correlation_Singleton(ZfactorGopal);

  ZfactorGopal() : GasCompressibility("ZfactorGopal", Zfactor::get_instance()) {
    set_author("Gopal");
    set_title(
        "GOPAL FIT METHOD, CALCULATION OF THE GAS COMPRESSIBILITY FACTOR");
    add_parameter("tpr", PseudoReducedTemperature::get_instance(),
                  "Gas pseudoreduced temperature", 1.05, 3);
    def_parameter_latex_symbol("tpr", "T_{pr}");
    add_parameter("ppr", PseudoReducedPressure::get_instance(),
                  "Gas pseudoreduced pressure", 0.2, 15);
    def_parameter_latex_symbol("ppr", "P_{pr}");
    add_db("Gopal presented a non-iterative method for calculating the "
           "Z-factor. The coefficients of the equations were obtained by "
           "fitting the Standing-Katz curves.");
    add_ref("gopal:1977");
    add_ref("standing:1942");
    add_ref("takacs:1989");
  }

public:
  static inline double impl(const double& tpr, const double& ppr) noexcept;

  Quantity<Zfactor> call(const Quantity<PseudoReducedTemperature>& tpr,
                         const Quantity<PseudoReducedPressure>& ppr) const {
    return Quantity<Zfactor>(impl(tpr.raw(), ppr.raw()));
  }

  Quantity<Zfactor> validate_and_call(
      const Quantity<PseudoReducedTemperature>& tpr,
      const Quantity<PseudoReducedPressure>& ppr) const {
    if (not(tpr >= 1.05))
      ZENTHROW(OutOfParameterRange, "parameter tpr is less than 1.05");
    if (not(tpr <= 3))
      ZENTHROW(OutOfParameterRange, "parameter tpr is greater than 3");
    if (not(ppr >= 0.2))
      ZENTHROW(OutOfParameterRange, "parameter ppr is less than 0.2");
    if (not(ppr <= 15))
      ZENTHROW(OutOfParameterRange, "parameter ppr is greater than 15");

    return call(tpr, ppr);
  }

  Quantity<Zfactor> operator()(
      const Quantity<PseudoReducedTemperature>& tpr,
      const Quantity<PseudoReducedPressure>& ppr) const {
    return Quantity<Zfactor>(call(tpr, ppr));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& tpr = it.get_curr();
    it.next();
    const auto& ppr = it.get_curr();

    return call(tpr, ppr);
  }
};

extern Quantity<Zfactor> ZfactorGopal__correlation__fct(
    const Quantity<PseudoReducedTemperature>& tpr,
    const Quantity<PseudoReducedPressure>& ppr);
extern double ZfactorGopal__correlation__valid__fct(const double& tpr,
                                                    const double& ppr);

/** ZfactorBrillBeggs

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group


The original reference is not available. The correlation was verified by using a
secondary reference: Bánzer (1996). Date: September 26 2016.

References:

  - brill:1974

  - standing:1942

@param[in] tpr Gas pseudoreduced temperature. Range in [1.2, 2.4]
@param[in] ppr Gas pseudoreduced pressure. Range in [0, 13]
@return inside [, ]
@ingroup Correlations
*/
class ZfactorBrillBeggs : public GasCompressibility {
  Correlation_Singleton(ZfactorBrillBeggs);

  ZfactorBrillBeggs()
      : GasCompressibility("ZfactorBrillBeggs", Zfactor::get_instance()) {
    set_author("Brill & Beggs");
    set_title("BRILL & BEGGS FIT METHOD, CALCULATION OF THE GAS "
              "COMPRESSIBILITY FACTOR");
    add_parameter("tpr", PseudoReducedTemperature::get_instance(),
                  "Gas pseudoreduced temperature", 1.2, 2.4);
    def_parameter_latex_symbol("tpr", "T_{pr}");
    add_parameter("ppr", PseudoReducedPressure::get_instance(),
                  "Gas pseudoreduced pressure", 0, 13);
    def_parameter_latex_symbol("ppr", "P_{pr}");
    add_db("The equation was obtained by applying adjustment methods to the "
           "Standing-Katz curves for the gas compressibility factor (Z).");
    add_ref("brill:1974");
    add_ref("standing:1942");
  }

public:
  static inline double impl(const double& tpr, const double& ppr) noexcept;

  Quantity<Zfactor> call(const Quantity<PseudoReducedTemperature>& tpr,
                         const Quantity<PseudoReducedPressure>& ppr) const {
    return Quantity<Zfactor>(impl(tpr.raw(), ppr.raw()));
  }

  Quantity<Zfactor> validate_and_call(
      const Quantity<PseudoReducedTemperature>& tpr,
      const Quantity<PseudoReducedPressure>& ppr) const {
    if (not(tpr >= 1.2))
      ZENTHROW(OutOfParameterRange, "parameter tpr is less than 1.2");
    if (not(tpr <= 2.4))
      ZENTHROW(OutOfParameterRange, "parameter tpr is greater than 2.4");
    if (not(ppr >= 0))
      ZENTHROW(OutOfParameterRange, "parameter ppr is less than 0");
    if (not(ppr <= 13))
      ZENTHROW(OutOfParameterRange, "parameter ppr is greater than 13");

    return call(tpr, ppr);
  }

  Quantity<Zfactor> operator()(
      const Quantity<PseudoReducedTemperature>& tpr,
      const Quantity<PseudoReducedPressure>& ppr) const {
    return Quantity<Zfactor>(call(tpr, ppr));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& tpr = it.get_curr();
    it.next();
    const auto& ppr = it.get_curr();

    return call(tpr, ppr);
  }
};

extern Quantity<Zfactor> ZfactorBrillBeggs__correlation__fct(
    const Quantity<PseudoReducedTemperature>& tpr,
    const Quantity<PseudoReducedPressure>& ppr);
extern double ZfactorBrillBeggs__correlation__valid__fct(const double& tpr,
                                                         const double& ppr);

/** ZfactorPapay

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group


The original reference is not available. The correlation was verified by using
secondary references: Bánzer (1996) and Takacs (1989). Date: September 23 2016.

The data bank information was obtained from Takacs (1989).

The application ranges were presented by Bánzer (1996).

References:

  - papay:1968

  - standing:1942

  - takacs:1989

@param[in] tpr Gas pseudoreduced temperature. Range in [1.2, 3]
@param[in] ppr Gas pseudoreduced pressure. Range in [0.2, 15]
@return inside [, ]
@ingroup Correlations
*/
class ZfactorPapay : public GasCompressibility {
  Correlation_Singleton(ZfactorPapay);

  ZfactorPapay() : GasCompressibility("ZfactorPapay", Zfactor::get_instance()) {
    set_author("Pápay");
    set_title(
        "PÁPAY FIT METHOD, CALCULATION OF THE GAS COMPRESSIBILITY FACTOR");
    add_parameter("tpr", PseudoReducedTemperature::get_instance(),
                  "Gas pseudoreduced temperature", 1.2, 3);
    def_parameter_latex_symbol("tpr", "T_{pr}");
    add_parameter("ppr", PseudoReducedPressure::get_instance(),
                  "Gas pseudoreduced pressure", 0.2, 15);
    def_parameter_latex_symbol("ppr", "P_{pr}");
    add_db("Pápay presented an equation for the calculation of Z-factors, "
           "resulting from applying methods of adjustment to the Standing-Katz "
           "curves. The equation is widely used in Hungary.");
    add_ref("papay:1968");
    add_ref("standing:1942");
    add_ref("takacs:1989");
  }

public:
  static inline double impl(const double& tpr, const double& ppr) noexcept;

  Quantity<Zfactor> call(const Quantity<PseudoReducedTemperature>& tpr,
                         const Quantity<PseudoReducedPressure>& ppr) const {
    return Quantity<Zfactor>(impl(tpr.raw(), ppr.raw()));
  }

  Quantity<Zfactor> validate_and_call(
      const Quantity<PseudoReducedTemperature>& tpr,
      const Quantity<PseudoReducedPressure>& ppr) const {
    if (not(tpr >= 1.2))
      ZENTHROW(OutOfParameterRange, "parameter tpr is less than 1.2");
    if (not(tpr <= 3))
      ZENTHROW(OutOfParameterRange, "parameter tpr is greater than 3");
    if (not(ppr >= 0.2))
      ZENTHROW(OutOfParameterRange, "parameter ppr is less than 0.2");
    if (not(ppr <= 15))
      ZENTHROW(OutOfParameterRange, "parameter ppr is greater than 15");

    return call(tpr, ppr);
  }

  Quantity<Zfactor> operator()(
      const Quantity<PseudoReducedTemperature>& tpr,
      const Quantity<PseudoReducedPressure>& ppr) const {
    return Quantity<Zfactor>(call(tpr, ppr));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& tpr = it.get_curr();
    it.next();
    const auto& ppr = it.get_curr();

    return call(tpr, ppr);
  }
};

extern Quantity<Zfactor> ZfactorPapay__correlation__fct(
    const Quantity<PseudoReducedTemperature>& tpr,
    const Quantity<PseudoReducedPressure>& ppr);
extern double ZfactorPapay__correlation__valid__fct(const double& tpr,
                                                    const double& ppr);

#endif // GAS_COMPRESSIBILITY_H
