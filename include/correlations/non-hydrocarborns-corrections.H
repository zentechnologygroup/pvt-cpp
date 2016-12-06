#ifndef NON_HYDROCAHORNS_CORRECTIONS_H
#define NON_HYDROCAHORNS_CORRECTIONS_H

#include <correlations/correlation.H>

/** YgHCWichertAziz

The original reference is not available. The correlation was verified by using
secondary references: Standing (1977) and Bánzer (1996). Date: September 12
2016.


*/
class YgHCWichertAziz : public HydrocarbonGasSpecificGravity
{
  Correlation_Singleton(YgHCWichertAziz);

  YgHCWichertAziz()
    : HydrocarbonGasSpecificGravity("YgHCWichertAziz", Sgg::get_instance(),
                                    0.55)
  {
    set_author("Wichert & Aziz (Gas Gravity Correction)");
    set_title("METHOD OF WICHERT & AZIZ FOR THE CALCULATION OF THE GAS GRAVITY "
              "OF THE HYDROCARBON PORTION");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity");
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("n2_concentration", MoleFraction::get_instance(),
                  "N2 concentration");
    def_parameter_latex_symbol("n2_concentration", "C_{N_2}");
    add_parameter("co2_concentration", MoleFraction::get_instance(),
                  "CO2 concentration");
    def_parameter_latex_symbol("co2_concentration", "C_{CO_2}");
    add_parameter("h2s_concentration", MoleFraction::get_instance(),
                  "H2S concentration");
    def_parameter_latex_symbol("h2s_concentration", "C_{H_2S}");
    add_note("Method of Wichert & Aziz for the calculation of the gas gravity "
             "of the hydrocarbon portion.");
    add_ref("wichert:1972");
    add_ref("standing:1977");
  }

public:
  inline Quantity<Sgg> impl(
    const Quantity<Sgg>& yg, const Quantity<MoleFraction>& n2_concentration,
    const Quantity<MoleFraction>& co2_concentration,
    const Quantity<MoleFraction>& h2s_concentration) const;

  Quantity<Sgg> operator()(
    const Quantity<Sgg>& yg, const Quantity<MoleFraction>& n2_concentration,
    const Quantity<MoleFraction>& co2_concentration,
    const Quantity<MoleFraction>& h2s_concentration) const
  {
    return impl(yg, n2_concentration, co2_concentration, h2s_concentration);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& n2_concentration = it.get_curr();
      it.next();
      const auto& co2_concentration = it.get_curr();
      it.next();
      const auto& h2s_concentration = it.get_curr();

      return impl(yg, n2_concentration, co2_concentration, h2s_concentration);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& n2_concentration = it.get_curr();
    it.next();
    const auto& co2_concentration = it.get_curr();
    it.next();
    const auto& h2s_concentration = it.get_curr();

    return impl(yg, n2_concentration, co2_concentration, h2s_concentration);
  }
};

extern Quantity<Sgg> YgHCWichertAziz__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<MoleFraction>& n2_concentration,
  const Quantity<MoleFraction>& co2_concentration,
  const Quantity<MoleFraction>& h2s_concentration);
extern double YgHCWichertAziz__correlation__valid__fct(
  double yg, double n2_concentration, double co2_concentration,
  double h2s_concentration);

/** PscMKayMixingRule

The original reference is not available. The correlation was verified by using
secondary references: Whitson & Brulé (2000), Standing (1977) and Bánzer (1996).
Date: September 12 2016.


*/
class PscMKayMixingRule : public GasPseudocriticalPressure
{
  Correlation_Singleton(PscMKayMixingRule);

  PscMKayMixingRule()
    : GasPseudocriticalPressure("PscMKayMixingRule", psia::get_instance())
  {
    set_author("Kay (Mixing Rule)");
    set_title("KAY'S MIXING RULE, CALCULATION OF PSEUDOCRITICAL PRESSURE OF "
              "THE WHOLE GAS MIXTURE");
    add_parameter("ppchc", psia::get_instance(),
                  "Gas pseudocritical pressure of the hydrocarbon portion");
    def_parameter_latex_symbol("ppchc", "P_{pcHC}");
    add_parameter("n2_concentration", MoleFraction::get_instance(),
                  "N2 concentration");
    def_parameter_latex_symbol("n2_concentration", "C_{N_2}");
    add_parameter("co2_concentration", MoleFraction::get_instance(),
                  "CO2 concentration");
    def_parameter_latex_symbol("co2_concentration", "C_{CO_2}");
    add_parameter("h2s_concentration", MoleFraction::get_instance(),
                  "H2S concentration");
    def_parameter_latex_symbol("h2s_concentration", "C_{H_2S}");
    add_note("The value of the pseudocritical pressure of the gas hydrocarbon "
             "portion is adjusted for nonhydrocarbon content on the basis of "
             "Kay's mixing rule.");
    add_ref("kay:1936");
    add_ref("whitson:2000");
    add_ref("standing:1977");
  }

public:
  inline Quantity<psia> impl(
    const Quantity<psia>& ppchc, const Quantity<MoleFraction>& n2_concentration,
    const Quantity<MoleFraction>& co2_concentration,
    const Quantity<MoleFraction>& h2s_concentration) const;

  Quantity<psia> operator()(
    const Quantity<psia>& ppchc, const Quantity<MoleFraction>& n2_concentration,
    const Quantity<MoleFraction>& co2_concentration,
    const Quantity<MoleFraction>& h2s_concentration) const
  {
    return impl(ppchc, n2_concentration, co2_concentration, h2s_concentration);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& ppchc = it.get_curr();
      it.next();
      const auto& n2_concentration = it.get_curr();
      it.next();
      const auto& co2_concentration = it.get_curr();
      it.next();
      const auto& h2s_concentration = it.get_curr();

      return impl(ppchc, n2_concentration, co2_concentration,
                  h2s_concentration);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& ppchc = it.get_curr();
    it.next();
    const auto& n2_concentration = it.get_curr();
    it.next();
    const auto& co2_concentration = it.get_curr();
    it.next();
    const auto& h2s_concentration = it.get_curr();

    return impl(ppchc, n2_concentration, co2_concentration, h2s_concentration);
  }
};

extern Quantity<psia> PscMKayMixingRule__correlation__fct(
  const Quantity<psia>& ppchc, const Quantity<MoleFraction>& n2_concentration,
  const Quantity<MoleFraction>& co2_concentration,
  const Quantity<MoleFraction>& h2s_concentration);
extern double PscMKayMixingRule__correlation__valid__fct(
  double ppchc, double n2_concentration, double co2_concentration,
  double h2s_concentration);

/** AdjustedPscMWichertAziz

The original reference is not available. The correlation was verified by using a
secondary reference: Standing (1977). Date: September 12 2016.

The development ranges were taken from PetroWiki
(http://petrowiki.org/Real_gases).


*/
class AdjustedPscMWichertAziz : public GasPseudocriticalPressure
{
  Correlation_Singleton(AdjustedPscMWichertAziz);

  AdjustedPscMWichertAziz()
    : GasPseudocriticalPressure("AdjustedPscMWichertAziz", psia::get_instance())
  {
    set_author("Wichert & Aziz (Mixture Correction)");
    set_title("WICHERT & AZIZ CORRELATION, CALCULATION OF PSEUDOCRITICAL "
              "PRESSURE OF THE WHOLE GAS MIXTURE");
    add_parameter("ppcm", psia::get_instance(),
                  "Gas pseudocritical pressure of the mixture", 154, 7026);
    def_parameter_latex_symbol("ppcm", "P_{pc}");
    add_parameter("tpcm", Fahrenheit::get_instance(),
                  "Gas pseudocritical temperature of the mixture", 40, 300);
    def_parameter_latex_symbol("tpcm", "T_{pc}");
    add_parameter("co2_concentration", MoleFraction::get_instance(),
                  "CO2 concentration", Quantity<MolePercent>(0),
                  Quantity<MolePercent>(54.4));
    def_parameter_latex_symbol("co2_concentration", "C_{CO_2}");
    add_parameter("h2s_concentration", MoleFraction::get_instance(),
                  "H2S concentration", Quantity<MolePercent>(0),
                  Quantity<MolePercent>(73.8));
    def_parameter_latex_symbol("h2s_concentration", "C_{H_2S}");
    add_note("The pseudocritical pressure of the mixture is adjusted by using "
             "the Wichert & Aziz correlation when the gas contains significant "
             "fractions of acid gases, specifically carbon dioxide and "
             "hydrogen sulfide.");
    add_ref("wichert:1972");
    add_ref("standing:1977");
  }

public:
  inline Quantity<psia> impl(
    const Quantity<psia>& ppcm, const Quantity<Fahrenheit>& tpcm,
    const Quantity<MoleFraction>& co2_concentration,
    const Quantity<MoleFraction>& h2s_concentration) const;

  Quantity<psia> operator()(
    const Quantity<psia>& ppcm, const Quantity<Fahrenheit>& tpcm,
    const Quantity<MoleFraction>& co2_concentration,
    const Quantity<MoleFraction>& h2s_concentration) const
  {
    return impl(ppcm, tpcm, co2_concentration, h2s_concentration);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& ppcm = it.get_curr();
      it.next();
      const auto& tpcm = it.get_curr();
      it.next();
      const auto& co2_concentration = it.get_curr();
      it.next();
      const auto& h2s_concentration = it.get_curr();

      return impl(ppcm, tpcm, co2_concentration, h2s_concentration);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& ppcm = it.get_curr();
    it.next();
    const auto& tpcm = it.get_curr();
    it.next();
    const auto& co2_concentration = it.get_curr();
    it.next();
    const auto& h2s_concentration = it.get_curr();

    return impl(ppcm, tpcm, co2_concentration, h2s_concentration);
  }
};

extern Quantity<psia> AdjustedPscMWichertAziz__correlation__fct(
  const Quantity<psia>& ppcm, const Quantity<Fahrenheit>& tpcm,
  const Quantity<MoleFraction>& co2_concentration,
  const Quantity<MoleFraction>& h2s_concentration);
extern double AdjustedPscMWichertAziz__correlation__valid__fct(
  double ppcm, double tpcm, double co2_concentration, double h2s_concentration);

/** TscMKayMixingRule

The original reference is not available. The correlation was verified by using
secondary references: Whitson & Brulé (2000), Standing (1977) and Bánzer (1996).
Date: September 12 2016.


*/
class TscMKayMixingRule : public GasPseudocriticalTemperature
{
  Correlation_Singleton(TscMKayMixingRule);

  TscMKayMixingRule()
    : GasPseudocriticalTemperature("TscMKayMixingRule", Rankine::get_instance())
  {
    set_author("Kay (Mixing Rule)");
    set_title("KAY'S MIXING RULE, CALCULATION OF PSEUDOCRITICAL TEMPERATURE OF "
              "THE WHOLE GAS MIXTURE");
    add_parameter("tpchc", Rankine::get_instance(),
                  "Gas pseudocritical temperature of the hydrocarbon portion");
    def_parameter_latex_symbol("tpchc", "T_{pcHC}");
    add_parameter("n2_concentration", MoleFraction::get_instance(),
                  "N2 concentration");
    def_parameter_latex_symbol("n2_concentration", "C_{N_2}");
    add_parameter("co2_concentration", MoleFraction::get_instance(),
                  "CO2 concentration");
    def_parameter_latex_symbol("co2_concentration", "C_{CO_2}");
    add_parameter("h2s_concentration", MoleFraction::get_instance(),
                  "H2S concentration");
    def_parameter_latex_symbol("h2s_concentration", "C_{H_2S}");
    add_note("The value of the pseudocritical temperature of the gas "
             "hydrocarbon portion is adjusted for nonhydrocarbon content on "
             "the basis of Kay's mixing rule.");
    add_ref("kay:1936");
    add_ref("whitson:2000");
    add_ref("standing:1977");
  }

public:
  inline Quantity<Rankine> impl(
    const Quantity<Rankine>& tpchc,
    const Quantity<MoleFraction>& n2_concentration,
    const Quantity<MoleFraction>& co2_concentration,
    const Quantity<MoleFraction>& h2s_concentration) const;

  Quantity<Rankine> operator()(
    const Quantity<Rankine>& tpchc,
    const Quantity<MoleFraction>& n2_concentration,
    const Quantity<MoleFraction>& co2_concentration,
    const Quantity<MoleFraction>& h2s_concentration) const
  {
    return impl(tpchc, n2_concentration, co2_concentration, h2s_concentration);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& tpchc = it.get_curr();
      it.next();
      const auto& n2_concentration = it.get_curr();
      it.next();
      const auto& co2_concentration = it.get_curr();
      it.next();
      const auto& h2s_concentration = it.get_curr();

      return impl(tpchc, n2_concentration, co2_concentration,
                  h2s_concentration);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& tpchc = it.get_curr();
    it.next();
    const auto& n2_concentration = it.get_curr();
    it.next();
    const auto& co2_concentration = it.get_curr();
    it.next();
    const auto& h2s_concentration = it.get_curr();

    return impl(tpchc, n2_concentration, co2_concentration, h2s_concentration);
  }
};

extern Quantity<Rankine> TscMKayMixingRule__correlation__fct(
  const Quantity<Rankine>& tpchc,
  const Quantity<MoleFraction>& n2_concentration,
  const Quantity<MoleFraction>& co2_concentration,
  const Quantity<MoleFraction>& h2s_concentration);
extern double TscMKayMixingRule__correlation__valid__fct(
  double tpchc, double n2_concentration, double co2_concentration,
  double h2s_concentration);

/** AdjustedTscMWichertAziz

The original reference is not available. The correlation was verified by using a
secondary reference: Standing (1977). Date: September 12 2016.

The development ranges were taken from PetroWiki
(http://petrowiki.org/Real_gases).


*/
class AdjustedTscMWichertAziz : public GasPseudocriticalTemperature
{
  Correlation_Singleton(AdjustedTscMWichertAziz);

  AdjustedTscMWichertAziz()
    : GasPseudocriticalTemperature("AdjustedTscMWichertAziz",
                                   Rankine::get_instance())
  {
    set_author("Wichert & Aziz (Mixture Correction)");
    set_title("WICHERT & AZIZ CORRELATION, CALCULATION OF PSEUDOCRITICAL "
              "TEMPERATURE OF THE WHOLE GAS MIXTURE");
    add_parameter("tpcm", Rankine::get_instance(),
                  "Gas pseudocritical temperature of the mixture",
                  Quantity<Fahrenheit>(40), Quantity<Fahrenheit>(300));
    def_parameter_latex_symbol("tpcm", "T_{pc}");
    add_parameter("co2_concentration", MoleFraction::get_instance(),
                  "CO2 concentration", Quantity<MolePercent>(0),
                  Quantity<MolePercent>(54.4));
    def_parameter_latex_symbol("co2_concentration", "C_{CO_2}");
    add_parameter("h2s_concentration", MoleFraction::get_instance(),
                  "H2S concentration", Quantity<MolePercent>(0),
                  Quantity<MolePercent>(73.8));
    def_parameter_latex_symbol("h2s_concentration", "C_{H_2S}");
    add_note("The pseudocritical temperature of the mixture is adjusted by "
             "using the Wichert & Aziz correlation when the gas contains "
             "significant fractions of acid gases, specifically carbon dioxide "
             "and hydrogen sulfide.");
    add_ref("wichert:1972");
    add_ref("standing:1977");
  }

public:
  inline Quantity<Rankine> impl(
    const Quantity<Rankine>& tpcm,
    const Quantity<MoleFraction>& co2_concentration,
    const Quantity<MoleFraction>& h2s_concentration) const;

  Quantity<Rankine> operator()(
    const Quantity<Rankine>& tpcm,
    const Quantity<MoleFraction>& co2_concentration,
    const Quantity<MoleFraction>& h2s_concentration) const
  {
    return impl(tpcm, co2_concentration, h2s_concentration);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& tpcm = it.get_curr();
      it.next();
      const auto& co2_concentration = it.get_curr();
      it.next();
      const auto& h2s_concentration = it.get_curr();

      return impl(tpcm, co2_concentration, h2s_concentration);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& tpcm = it.get_curr();
    it.next();
    const auto& co2_concentration = it.get_curr();
    it.next();
    const auto& h2s_concentration = it.get_curr();

    return impl(tpcm, co2_concentration, h2s_concentration);
  }
};

extern Quantity<Rankine> AdjustedTscMWichertAziz__correlation__fct(
  const Quantity<Rankine>& tpcm,
  const Quantity<MoleFraction>& co2_concentration,
  const Quantity<MoleFraction>& h2s_concentration);
extern double AdjustedTscMWichertAziz__correlation__valid__fct(
  double tpcm, double co2_concentration, double h2s_concentration);

#endif // NON_HYDROCAHORNS_CORRECTIONS_H
