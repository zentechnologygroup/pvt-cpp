#ifndef GAS_VISCOSITY_H
#define GAS_VISCOSITY_H

#include <pvt-exceptions.H>
#include <correlations/correlation.H>

/** UgCarrKB

    Generated at 2017-10-30 16:49:05 -0400

    Zen Technologies Group

Carr et al. (1954) presented the graphs of the correlation. Dempsey (1965)
expressed them in a mathematical form.

A linear effect of concentration was assumed to apply over the concentration
range from 0 to 15 mol percent of non-hydrocarbon components.

The equation was verified by using secondary references: Bánzer (1996) and
Standing (1977). Date: September 29 2016.

The development ranges were presented by Bánzer (1996).

The description was obtained from PetroWiki.

References:

  - carr:1954

  - dempsey:1965

  - standing:1977

  - petroWiki:2016:2

@param[in] t Temperature. Range in [100, 300]
@param[in] tpr Gas pseudoreduced temperature. Range in [1.2, 3]
@param[in] ppr Gas pseudoreduced pressure. Range in [1, 20]
@param[in] yg Gas specific gravity. Range in [0.55, 1.55]
@param[in] n2 N2 concentration. Range in [, ]
@param[in] co2 CO2 concentration. Range in [, ]
@param[in] h2s H2S concentration. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class UgCarrKB : public GasViscosity {
  Correlation_Singleton(UgCarrKB);

  UgCarrKB() : GasViscosity("UgCarrKB", CP::get_instance()) {
    set_author("Carr, Kobayashi & Burrows");
    set_title(
        "CARR, KOBAYASHI & BURROWS CORRELATION, CALCULATION OF GAS VISCOSITY");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 100, 300);
    def_parameter_latex_symbol("t", "T");
    add_parameter("tpr", PseudoReducedTemperature::get_instance(),
                  "Gas pseudoreduced temperature", 1.2, 3);
    def_parameter_latex_symbol("tpr", "T_{pr}");
    add_parameter("ppr", PseudoReducedPressure::get_instance(),
                  "Gas pseudoreduced pressure", 1, 20);
    def_parameter_latex_symbol("ppr", "P_{pr}");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.55,
                  1.55);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("n2", MoleFraction::get_instance(), "N2 concentration");
    add_parameter("co2", MoleFraction::get_instance(), "CO2 concentration");
    add_parameter("h2s", MoleFraction::get_instance(), "H2S concentration");
    add_db("Carr, Kobayashi & Burrows (1954) developed charts that are the "
           "most widely used for calculating the viscosity of natural gas from "
           "the pseudoreduced temperature and pressure.");
    add_note("Carr et al. (1954) presented the graphs of the correlation. "
             "Dempsey (1965) expressed them in a mathematical form.");
    add_ref("carr:1954");
    add_ref("dempsey:1965");
    add_ref("standing:1977");
    add_ref("petroWiki:2016:2");
  }

  inline void precondition(const Quantity<MoleFraction>& n2,
                           const Quantity<MoleFraction>& co2,
                           const Quantity<MoleFraction>& h2s) const;

public:
  static inline double impl(const double& t, const double& tpr,
                            const double& ppr, const double& yg,
                            const double& n2, const double& co2,
                            const double& h2s) noexcept;

  Quantity<CP> call(const Quantity<Fahrenheit>& t,
                    const Quantity<PseudoReducedTemperature>& tpr,
                    const Quantity<PseudoReducedPressure>& ppr,
                    const Quantity<Sgg>& yg, const Quantity<MoleFraction>& n2,
                    const Quantity<MoleFraction>& co2,
                    const Quantity<MoleFraction>& h2s) const {
    precondition(n2, co2, h2s);
    return Quantity<CP>(impl(t.raw(), tpr.raw(), ppr.raw(), yg.raw(), n2.raw(),
                             co2.raw(), h2s.raw()));
  }

  Quantity<CP> validate_and_call(const Quantity<Fahrenheit>& t,
                                 const Quantity<PseudoReducedTemperature>& tpr,
                                 const Quantity<PseudoReducedPressure>& ppr,
                                 const Quantity<Sgg>& yg,
                                 const Quantity<MoleFraction>& n2,
                                 const Quantity<MoleFraction>& co2,
                                 const Quantity<MoleFraction>& h2s) const {
    if (not(tpr >= 1.2))
      ZENTHROW(OutOfParameterRange, "parameter tpr is less than 1.2");
    if (not(tpr <= 3))
      ZENTHROW(OutOfParameterRange, "parameter tpr is greater than 3");
    if (not(ppr >= 1))
      ZENTHROW(OutOfParameterRange, "parameter ppr is less than 1");
    if (not(ppr <= 20))
      ZENTHROW(OutOfParameterRange, "parameter ppr is greater than 20");
    if (not(yg >= 0.55))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.55");
    if (not(yg <= 1.55))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.55");

    return call(t, tpr, ppr, yg, n2, co2, h2s);
  }

  Quantity<CP> operator()(const Quantity<Fahrenheit>& t,
                          const Quantity<PseudoReducedTemperature>& tpr,
                          const Quantity<PseudoReducedPressure>& ppr,
                          const Quantity<Sgg>& yg,
                          const Quantity<MoleFraction>& n2,
                          const Quantity<MoleFraction>& co2,
                          const Quantity<MoleFraction>& h2s) const {
    return Quantity<CP>(call(t, tpr, ppr, yg, n2, co2, h2s));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& t = it.get_curr();
    it.next();
    const auto& tpr = it.get_curr();
    it.next();
    const auto& ppr = it.get_curr();
    it.next();
    const auto& yg = it.get_curr();
    it.next();
    const auto& n2 = it.get_curr();
    it.next();
    const auto& co2 = it.get_curr();
    it.next();
    const auto& h2s = it.get_curr();

    return call(t, tpr, ppr, yg, n2, co2, h2s);
  }
};

extern Quantity<CP> UgCarrKB__correlation__fct(
    const Quantity<Fahrenheit>& t,
    const Quantity<PseudoReducedTemperature>& tpr,
    const Quantity<PseudoReducedPressure>& ppr, const Quantity<Sgg>& yg,
    const Quantity<MoleFraction>& n2, const Quantity<MoleFraction>& co2,
    const Quantity<MoleFraction>& h2s);
extern double UgCarrKB__correlation__valid__fct(
    const double& t, const double& tpr, const double& ppr, const double& yg,
    const double& n2, const double& co2, const double& h2s);

/** UgLeeGE

    Generated at 2017-10-30 16:49:05 -0400

    Zen Technologies Group


The correlation was verified by using the original reference and a secondary
one: Bánzer (1996). Date: October 03 2016.

References:

  - lee:1966

@param[in] t Temperature. Range in [Quantity<Fahrenheit>(100),
Quantity<Fahrenheit>(340)]
@param[in] p Pressure. Range in [100, 8000]
@param[in] yg Gas specific gravity. Range in [, ]
@param[in] z Gas compressibility factor. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class UgLeeGE : public GasViscosity {
  Correlation_Singleton(UgLeeGE);

  UgLeeGE() : GasViscosity("UgLeeGE", mP::get_instance()) {
    set_author("Lee, Gonzalez & Eakin");
    set_title(
        "LEE, GONZALEZ & EAKIN CORRELATION, CALCULATION OF GAS VISCOSITY");
    add_parameter("t", Rankine::get_instance(), "Temperature",
                  Quantity<Fahrenheit>(100), Quantity<Fahrenheit>(340));
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure", 100, 8000);
    def_parameter_latex_symbol("p", "P");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity");
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("z", Zfactor::get_instance(), "Gas compressibility factor");
    def_parameter_latex_symbol("z", "Z\\ factor");
    add_db("Based on experimental viscosity and density data of four natural "
           "gases. The samples were furnished by the Atlantic Richfield Co., "
           "the Continental Oil Co. and the Pan American Petroleum Corp.");
    add_ref("lee:1966");
  }

public:
  static inline double impl(const double& t, const double& p, const double& yg,
                            const double& z) noexcept;

  Quantity<mP> call(const Quantity<Rankine>& t, const Quantity<psia>& p,
                    const Quantity<Sgg>& yg, const Quantity<Zfactor>& z) const {
    return Quantity<mP>(impl(t.raw(), p.raw(), yg.raw(), z.raw()));
  }

  Quantity<mP> validate_and_call(const Quantity<Rankine>& t,
                                 const Quantity<psia>& p,
                                 const Quantity<Sgg>& yg,
                                 const Quantity<Zfactor>& z) const {

    return call(t, p, yg, z);
  }

  Quantity<mP> operator()(const Quantity<Rankine>& t, const Quantity<psia>& p,
                          const Quantity<Sgg>& yg,
                          const Quantity<Zfactor>& z) const {
    return Quantity<mP>(call(t, p, yg, z));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& yg = it.get_curr();
    it.next();
    const auto& z = it.get_curr();

    return call(t, p, yg, z);
  }
};

extern Quantity<mP> UgLeeGE__correlation__fct(const Quantity<Rankine>& t,
                                              const Quantity<psia>& p,
                                              const Quantity<Sgg>& yg,
                                              const Quantity<Zfactor>& z);
extern double UgLeeGE__correlation__valid__fct(const double& t, const double& p,
                                               const double& yg,
                                               const double& z);

/** UgDeanStiel

    Generated at 2017-10-30 16:49:05 -0400

    Zen Technologies Group


The original reference is not available. The correlation was verified by using a
secondary reference: Gawish & Al-Homadhi (2005). Date: October 03 2016.

References:

  - dean:1958

  - gawish:2005

@param[in] t Temperature. Range in [, ]
@param[in] p Pressure. Range in [, ]
@param[in] tpc Gas pseudocritical temperature. Range in [, ]
@param[in] ppc Gas pseudocritical pressure. Range in [, ]
@param[in] yg Gas specific gravity. Range in [, ]
@param[in] z Gas compressibility factor. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class UgDeanStiel : public GasViscosity {
  Correlation_Singleton(UgDeanStiel);

  UgDeanStiel() : GasViscosity("UgDeanStiel", CP::get_instance()) {
    set_author("Dean & Stiel");
    set_title("DEAN & STIEL CORRELATION, CALCULATION OF GAS VISCOSITY");
    add_parameter("t", Rankine::get_instance(), "Temperature");
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("tpc", Rankine::get_instance(),
                  "Gas pseudocritical temperature");
    def_parameter_latex_symbol("tpc", "T_{pc}");
    add_parameter("ppc", psia::get_instance(), "Gas pseudocritical pressure");
    def_parameter_latex_symbol("ppc", "P_{pc}");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity");
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("z", Zfactor::get_instance(), "Gas compressibility factor");
    def_parameter_latex_symbol("z", "Z\\ factor");
    add_ref("dean:1958");
    add_ref("gawish:2005");
  }

public:
  static inline double impl(const double& t, const double& p, const double& tpc,
                            const double& ppc, const double& yg,
                            const double& z) noexcept;

  Quantity<CP> call(const Quantity<Rankine>& t, const Quantity<psia>& p,
                    const Quantity<Rankine>& tpc, const Quantity<psia>& ppc,
                    const Quantity<Sgg>& yg, const Quantity<Zfactor>& z) const {
    return Quantity<CP>(
        impl(t.raw(), p.raw(), tpc.raw(), ppc.raw(), yg.raw(), z.raw()));
  }

  Quantity<CP> validate_and_call(const Quantity<Rankine>& t,
                                 const Quantity<psia>& p,
                                 const Quantity<Rankine>& tpc,
                                 const Quantity<psia>& ppc,
                                 const Quantity<Sgg>& yg,
                                 const Quantity<Zfactor>& z) const {

    return call(t, p, tpc, ppc, yg, z);
  }

  Quantity<CP> operator()(const Quantity<Rankine>& t, const Quantity<psia>& p,
                          const Quantity<Rankine>& tpc,
                          const Quantity<psia>& ppc, const Quantity<Sgg>& yg,
                          const Quantity<Zfactor>& z) const {
    return Quantity<CP>(call(t, p, tpc, ppc, yg, z));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& tpc = it.get_curr();
    it.next();
    const auto& ppc = it.get_curr();
    it.next();
    const auto& yg = it.get_curr();
    it.next();
    const auto& z = it.get_curr();

    return call(t, p, tpc, ppc, yg, z);
  }
};

extern Quantity<CP> UgDeanStiel__correlation__fct(const Quantity<Rankine>& t,
                                                  const Quantity<psia>& p,
                                                  const Quantity<Rankine>& tpc,
                                                  const Quantity<psia>& ppc,
                                                  const Quantity<Sgg>& yg,
                                                  const Quantity<Zfactor>& z);
extern double UgDeanStiel__correlation__valid__fct(
    const double& t, const double& p, const double& tpc, const double& ppc,
    const double& yg, const double& z);

#endif // GAS_VISCOSITY_H
