#ifndef GAS_VISCOSITY_H
#define GAS_VISCOSITY_H

#include <correlations/correlation.H>

/** UgCarrKB

A linear effect of concentration was assumed to apply over the concentration
range from 0 to 15 mol percent of non-hydrocarbon components.

The equation was verified by using secondary references: Bánzer (1996) and
Standing (1977). Date: September 29 2016.

The development ranges were presented by Bánzer (1996).

The description was obtained from PetroWiki.


*/
class UgCarrKB : public GasViscosity
{
  Correlation_Singleton(UgCarrKB);

  UgCarrKB()
    : GasViscosity("UgCarrKB", CP::get_instance())
  {
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
    add_parameter("n2_concentration", MolePercent::get_instance(),
                  "N2 concentration");
    def_parameter_latex_symbol("n2_concentration", "C_{N_2}");
    add_parameter("co2_concentration", MolePercent::get_instance(),
                  "CO2 concentration");
    def_parameter_latex_symbol("co2_concentration", "C_{CO_2}");
    add_parameter("h2s_concentration", MolePercent::get_instance(),
                  "H2S concentration");
    def_parameter_latex_symbol("h2s_concentration", "C_{H_2S}");
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

public:
  inline Quantity<CP> impl(
    const Quantity<Fahrenheit>& t,
    const Quantity<PseudoReducedTemperature>& tpr,
    const Quantity<PseudoReducedPressure>& ppr, const Quantity<Sgg>& yg,
    const Quantity<MolePercent>& n2_concentration,
    const Quantity<MolePercent>& co2_concentration,
    const Quantity<MolePercent>& h2s_concentration) const;

  Quantity<CP> operator()(const Quantity<Fahrenheit>& t,
                          const Quantity<PseudoReducedTemperature>& tpr,
                          const Quantity<PseudoReducedPressure>& ppr,
                          const Quantity<Sgg>& yg,
                          const Quantity<MolePercent>& n2_concentration,
                          const Quantity<MolePercent>& co2_concentration,
                          const Quantity<MolePercent>& h2s_concentration) const
  {
    return impl(t, tpr, ppr, yg, n2_concentration, co2_concentration,
                h2s_concentration);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& t = it.get_curr();
      it.next();
      const auto& tpr = it.get_curr();
      it.next();
      const auto& ppr = it.get_curr();
      it.next();
      const auto& yg = it.get_curr();
      it.next();
      const auto& n2_concentration = it.get_curr();
      it.next();
      const auto& co2_concentration = it.get_curr();
      it.next();
      const auto& h2s_concentration = it.get_curr();

      return impl(t, tpr, ppr, yg, n2_concentration, co2_concentration,
                  h2s_concentration);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& t = it.get_curr();
    it.next();
    const auto& tpr = it.get_curr();
    it.next();
    const auto& ppr = it.get_curr();
    it.next();
    const auto& yg = it.get_curr();
    it.next();
    const auto& n2_concentration = it.get_curr();
    it.next();
    const auto& co2_concentration = it.get_curr();
    it.next();
    const auto& h2s_concentration = it.get_curr();

    return impl(t, tpr, ppr, yg, n2_concentration, co2_concentration,
                h2s_concentration);
  }
};

extern Quantity<CP> UgCarrKB__correlation__fct(
  const Quantity<Fahrenheit>& t, const Quantity<PseudoReducedTemperature>& tpr,
  const Quantity<PseudoReducedPressure>& ppr, const Quantity<Sgg>& yg,
  const Quantity<MolePercent>& n2_concentration,
  const Quantity<MolePercent>& co2_concentration,
  const Quantity<MolePercent>& h2s_concentration);
extern double UgCarrKB__correlation__valid__fct(double t, double tpr,
                                                double ppr, double yg,
                                                double n2_concentration,
                                                double co2_concentration,
                                                double h2s_concentration);

/** UgLeeGE

The correlation was verified by using the original reference and a secondary
one: Bánzer (1996). Date: October 03 2016.


*/
class UgLeeGE : public GasViscosity
{
  Correlation_Singleton(UgLeeGE);

  UgLeeGE()
    : GasViscosity("UgLeeGE", mP::get_instance())
  {
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
    add_parameter("z", ZFactor::get_instance(), "Gas compressibility factor");
    def_parameter_latex_symbol("z", "Z\\ factor");
    add_db("Based on experimental viscosity and density data of four natural "
           "gases. The samples were furnished by the Atlantic Richfield Co., "
           "the Continental Oil Co. and the Pan American Petroleum Corp.");
    add_ref("lee:1966");
  }

public:
  inline Quantity<mP> impl(const Quantity<Rankine>& t, const Quantity<psia>& p,
                           const Quantity<Sgg>& yg,
                           const Quantity<ZFactor>& z) const;

  Quantity<mP> operator()(const Quantity<Rankine>& t, const Quantity<psia>& p,
                          const Quantity<Sgg>& yg,
                          const Quantity<ZFactor>& z) const
  {
    return impl(t, p, yg, z);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& t = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& yg = it.get_curr();
      it.next();
      const auto& z = it.get_curr();

      return impl(t, p, yg, z);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& yg = it.get_curr();
    it.next();
    const auto& z = it.get_curr();

    return impl(t, p, yg, z);
  }
};

extern Quantity<mP> UgLeeGE__correlation__fct(const Quantity<Rankine>& t,
                                              const Quantity<psia>& p,
                                              const Quantity<Sgg>& yg,
                                              const Quantity<ZFactor>& z);
extern double UgLeeGE__correlation__valid__fct(double t, double p, double yg,
                                               double z);

/** UgDeanStiel

The original reference is not available. The correlation was verified by using a
secondary reference: Gawish & Al-Homadhi (2005). Date: October 03 2016.


*/
class UgDeanStiel : public GasViscosity
{
  Correlation_Singleton(UgDeanStiel);

  UgDeanStiel()
    : GasViscosity("UgDeanStiel", CP::get_instance())
  {
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
    add_parameter("z", ZFactor::get_instance(), "Gas compressibility factor");
    def_parameter_latex_symbol("z", "Z\\ factor");
    add_ref("dean:1958");
    add_ref("gawish:2005");
  }

public:
  inline Quantity<CP> impl(const Quantity<Rankine>& t, const Quantity<psia>& p,
                           const Quantity<Rankine>& tpc,
                           const Quantity<psia>& ppc, const Quantity<Sgg>& yg,
                           const Quantity<ZFactor>& z) const;

  Quantity<CP> operator()(const Quantity<Rankine>& t, const Quantity<psia>& p,
                          const Quantity<Rankine>& tpc,
                          const Quantity<psia>& ppc, const Quantity<Sgg>& yg,
                          const Quantity<ZFactor>& z) const
  {
    return impl(t, p, tpc, ppc, yg, z);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
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

      return impl(t, p, tpc, ppc, yg, z);
    }

    const DynList<VtlQuantity>& pars = __pars;
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

    return impl(t, p, tpc, ppc, yg, z);
  }
};

extern Quantity<CP> UgDeanStiel__correlation__fct(const Quantity<Rankine>& t,
                                                  const Quantity<psia>& p,
                                                  const Quantity<Rankine>& tpc,
                                                  const Quantity<psia>& ppc,
                                                  const Quantity<Sgg>& yg,
                                                  const Quantity<ZFactor>& z);
extern double UgDeanStiel__correlation__valid__fct(double t, double p,
                                                   double tpc, double ppc,
                                                   double yg, double z);

#endif // GAS_VISCOSITY_H
