#ifndef GAS_COMPRESSIBILITY_H
#define GAS_COMPRESSIBILITY_H

#include <correlations/correlation.H>

/** ZFactorSarem

The original reference is not available. The correlation was verified by using a
secondary reference: Bánzer (1996). Date: September 22 2016.


*/
class ZFactorSarem : public GasCompressibility
{
  Correlation_Singleton(ZFactorSarem);

  ZFactorSarem()
    : GasCompressibility("ZFactorSarem", ZFactor::get_instance())
  {
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
  inline Quantity<ZFactor> impl(
    const Quantity<PseudoReducedTemperature>& tpr,
    const Quantity<PseudoReducedPressure>& ppr) const;

  Quantity<ZFactor> operator()(const Quantity<PseudoReducedTemperature>& tpr,
                               const Quantity<PseudoReducedPressure>& ppr) const
  {
    return impl(tpr, ppr);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& tpr = it.get_curr();
      it.next();
      const auto& ppr = it.get_curr();

      return impl(tpr, ppr);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& tpr = it.get_curr();
    it.next();
    const auto& ppr = it.get_curr();

    return impl(tpr, ppr);
  }
};

extern Quantity<ZFactor> ZFactorSarem__correlation__fct(
  const Quantity<PseudoReducedTemperature>& tpr,
  const Quantity<PseudoReducedPressure>& ppr);
extern double ZFactorSarem__correlation__valid__fct(double tpr, double ppr);

/** ZFactorHallYarborough

The original reference is not available. The correlation was verified by using a
secondary reference: Bánzer (1996). Date: September 23 2016.

The application ranges and data bank information was obtained from Takacs
(1989).


*/
class ZFactorHallYarborough : public GasCompressibility
{
  Correlation_Singleton(ZFactorHallYarborough);

  ZFactorHallYarborough()
    : GasCompressibility("ZFactorHallYarborough", ZFactor::get_instance())
  {
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
  }

public:
  inline Quantity<ZFactor> impl(
    const Quantity<PseudoReducedTemperature>& tpr,
    const Quantity<PseudoReducedPressure>& ppr) const;

  Quantity<ZFactor> operator()(const Quantity<PseudoReducedTemperature>& tpr,
                               const Quantity<PseudoReducedPressure>& ppr) const
  {
    return impl(tpr, ppr);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& tpr = it.get_curr();
      it.next();
      const auto& ppr = it.get_curr();

      return impl(tpr, ppr);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& tpr = it.get_curr();
    it.next();
    const auto& ppr = it.get_curr();

    return impl(tpr, ppr);
  }
};

extern Quantity<ZFactor> ZFactorHallYarborough__correlation__fct(
  const Quantity<PseudoReducedTemperature>& tpr,
  const Quantity<PseudoReducedPressure>& ppr);
extern double ZFactorHallYarborough__correlation__valid__fct(double tpr,
                                                             double ppr);

/** ZFactorDranchukPR

The correlation was verified by using the original reference and a secondary
one: Bánzer (1996). Date: September 28 2016.

The application ranges and data bank information was obtained from Takacs
(1989).


*/
class ZFactorDranchukPR : public GasCompressibility
{
  Correlation_Singleton(ZFactorDranchukPR);

  ZFactorDranchukPR()
    : GasCompressibility("ZFactorDranchukPR", ZFactor::get_instance())
  {
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
  inline Quantity<ZFactor> impl(
    const Quantity<PseudoReducedTemperature>& tpr,
    const Quantity<PseudoReducedPressure>& ppr) const;

  Quantity<ZFactor> operator()(const Quantity<PseudoReducedTemperature>& tpr,
                               const Quantity<PseudoReducedPressure>& ppr) const
  {
    return impl(tpr, ppr);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& tpr = it.get_curr();
      it.next();
      const auto& ppr = it.get_curr();

      return impl(tpr, ppr);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& tpr = it.get_curr();
    it.next();
    const auto& ppr = it.get_curr();

    return impl(tpr, ppr);
  }
};

extern Quantity<ZFactor> ZFactorDranchukPR__correlation__fct(
  const Quantity<PseudoReducedTemperature>& tpr,
  const Quantity<PseudoReducedPressure>& ppr);
extern double ZFactorDranchukPR__correlation__valid__fct(double tpr,
                                                         double ppr);

/** ZFactorDranchukAK

The original reference is not available. The correlation was verified by using a
secondary reference: Bánzer (1996). Date: September 23 2016.

The application ranges and data bank information was obtained from Takacs
(1989).

The lower limit for Ppr (when Tpr's range is from 0.7 to 1.0) was taken from the
development ranges of the correlation presented by Standing & Katz (1942).


*/
class ZFactorDranchukAK : public GasCompressibility
{
  Correlation_Singleton(ZFactorDranchukAK);

  ZFactorDranchukAK()
    : GasCompressibility("ZFactorDranchukAK", ZFactor::get_instance())
  {
    set_author("Dranchuk &  Abou-Kassem");
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
             "30 and 0.7 <= Tpr <= 1 for 0 <= Ppr <= 1.");
    add_ref("dranchuk:1975");
    add_ref("standing:1942");
    add_ref("takacs:1989");
  }

public:
  inline Quantity<ZFactor> impl(
    const Quantity<PseudoReducedTemperature>& tpr,
    const Quantity<PseudoReducedPressure>& ppr) const;

  Quantity<ZFactor> operator()(const Quantity<PseudoReducedTemperature>& tpr,
                               const Quantity<PseudoReducedPressure>& ppr) const
  {
    return impl(tpr, ppr);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& tpr = it.get_curr();
      it.next();
      const auto& ppr = it.get_curr();

      return impl(tpr, ppr);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& tpr = it.get_curr();
    it.next();
    const auto& ppr = it.get_curr();

    return impl(tpr, ppr);
  }
};

extern Quantity<ZFactor> ZFactorDranchukAK__correlation__fct(
  const Quantity<PseudoReducedTemperature>& tpr,
  const Quantity<PseudoReducedPressure>& ppr);
extern double ZFactorDranchukAK__correlation__valid__fct(double tpr,
                                                         double ppr);

/** ZFactorGopal

The original reference is not available. The correlation was verified by using a
secondary reference: Bánzer (1996). Date: September 23 2016.

The application ranges and data bank information was obtained from Takacs
(1989).


*/
class ZFactorGopal : public GasCompressibility
{
  Correlation_Singleton(ZFactorGopal);

  ZFactorGopal()
    : GasCompressibility("ZFactorGopal", ZFactor::get_instance())
  {
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
  inline Quantity<ZFactor> impl(
    const Quantity<PseudoReducedTemperature>& tpr,
    const Quantity<PseudoReducedPressure>& ppr) const;

  Quantity<ZFactor> operator()(const Quantity<PseudoReducedTemperature>& tpr,
                               const Quantity<PseudoReducedPressure>& ppr) const
  {
    return impl(tpr, ppr);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& tpr = it.get_curr();
      it.next();
      const auto& ppr = it.get_curr();

      return impl(tpr, ppr);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& tpr = it.get_curr();
    it.next();
    const auto& ppr = it.get_curr();

    return impl(tpr, ppr);
  }
};

extern Quantity<ZFactor> ZFactorGopal__correlation__fct(
  const Quantity<PseudoReducedTemperature>& tpr,
  const Quantity<PseudoReducedPressure>& ppr);
extern double ZFactorGopal__correlation__valid__fct(double tpr, double ppr);

/** ZFactorBrillBeggs

The original reference is not available. The correlation was verified by using a
secondary reference: Bánzer (1996). Date: September 26 2016.


*/
class ZFactorBrillBeggs : public GasCompressibility
{
  Correlation_Singleton(ZFactorBrillBeggs);

  ZFactorBrillBeggs()
    : GasCompressibility("ZFactorBrillBeggs", ZFactor::get_instance())
  {
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
  inline Quantity<ZFactor> impl(
    const Quantity<PseudoReducedTemperature>& tpr,
    const Quantity<PseudoReducedPressure>& ppr) const;

  Quantity<ZFactor> operator()(const Quantity<PseudoReducedTemperature>& tpr,
                               const Quantity<PseudoReducedPressure>& ppr) const
  {
    return impl(tpr, ppr);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& tpr = it.get_curr();
      it.next();
      const auto& ppr = it.get_curr();

      return impl(tpr, ppr);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& tpr = it.get_curr();
    it.next();
    const auto& ppr = it.get_curr();

    return impl(tpr, ppr);
  }
};

extern Quantity<ZFactor> ZFactorBrillBeggs__correlation__fct(
  const Quantity<PseudoReducedTemperature>& tpr,
  const Quantity<PseudoReducedPressure>& ppr);
extern double ZFactorBrillBeggs__correlation__valid__fct(double tpr,
                                                         double ppr);

/** ZFactorPapay

The original reference is not available. The correlation was verified by using
secondary references: Bánzer (1996) and Takacs (1989). Date: September 23 2016.

The data bank information was obtained from Takacs (1989).

The application ranges were presented by Bánzer (1996).


*/
class ZFactorPapay : public GasCompressibility
{
  Correlation_Singleton(ZFactorPapay);

  ZFactorPapay()
    : GasCompressibility("ZFactorPapay", ZFactor::get_instance())
  {
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
  inline Quantity<ZFactor> impl(
    const Quantity<PseudoReducedTemperature>& tpr,
    const Quantity<PseudoReducedPressure>& ppr) const;

  Quantity<ZFactor> operator()(const Quantity<PseudoReducedTemperature>& tpr,
                               const Quantity<PseudoReducedPressure>& ppr) const
  {
    return impl(tpr, ppr);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& tpr = it.get_curr();
      it.next();
      const auto& ppr = it.get_curr();

      return impl(tpr, ppr);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& tpr = it.get_curr();
    it.next();
    const auto& ppr = it.get_curr();

    return impl(tpr, ppr);
  }
};

extern Quantity<ZFactor> ZFactorPapay__correlation__fct(
  const Quantity<PseudoReducedTemperature>& tpr,
  const Quantity<PseudoReducedPressure>& ppr);
extern double ZFactorPapay__correlation__valid__fct(double tpr, double ppr);

#endif // GAS_COMPRESSIBILITY_H
