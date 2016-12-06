#ifndef PSEUDO_REDUCED_RATIOS_H
#define PSEUDO_REDUCED_RATIOS_H

#include <correlations/correlation.H>

/** Tpr


*/
class Tpr : public GasPseudoreducedTemperature
{
  Correlation_Singleton(Tpr);

  Tpr()
    : GasPseudoreducedTemperature("Tpr",
                                  PseudoReducedTemperature::get_instance())
  {
    set_author("Standard Equation");
    set_title("CALCULATION OF THE PSEUDOREDUCED TEMPERATURE");
    add_parameter("t", Rankine::get_instance(), "Temperature");
    def_parameter_latex_symbol("t", "T");
    add_parameter("tpc", Rankine::get_instance(),
                  "Gas pseudocritical temperature");
    def_parameter_latex_symbol("tpc", "T_{pc}");
    add_note("The temperature is expressed as a ratio of its critical value.");
    add_ref("ahmed:1989");
  }

public:
  inline Quantity<PseudoReducedTemperature> impl(
    const Quantity<Rankine>& t, const Quantity<Rankine>& tpc) const;

  Quantity<PseudoReducedTemperature> operator()(
    const Quantity<Rankine>& t, const Quantity<Rankine>& tpc) const
  {
    return impl(t, tpc);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& t = it.get_curr();
      it.next();
      const auto& tpc = it.get_curr();

      return impl(t, tpc);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& t = it.get_curr();
    it.next();
    const auto& tpc = it.get_curr();

    return impl(t, tpc);
  }
};

extern Quantity<PseudoReducedTemperature> Tpr__correlation__fct(
  const Quantity<Rankine>& t, const Quantity<Rankine>& tpc);
extern double Tpr__correlation__valid__fct(double t, double tpc);

/** Ppr


*/
class Ppr : public GasPseudoreducedPressure
{
  Correlation_Singleton(Ppr);

  Ppr()
    : GasPseudoreducedPressure("Ppr", PseudoReducedPressure::get_instance())
  {
    set_author("Standard Equation");
    set_title("CALCULATION OF THE PSEUDOREDUCED PRESSURE");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("ppc", psia::get_instance(), "Gas pseudocritical pressure");
    def_parameter_latex_symbol("ppc", "P_{pc}");
    add_note("The pressure is expressed as a ratio of its critical value.");
    add_ref("ahmed:1989");
  }

public:
  inline Quantity<PseudoReducedPressure> impl(const Quantity<psia>& p,
                                              const Quantity<psia>& ppc) const;

  Quantity<PseudoReducedPressure> operator()(const Quantity<psia>& p,
                                             const Quantity<psia>& ppc) const
  {
    return impl(p, ppc);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& p = it.get_curr();
      it.next();
      const auto& ppc = it.get_curr();

      return impl(p, ppc);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& p = it.get_curr();
    it.next();
    const auto& ppc = it.get_curr();

    return impl(p, ppc);
  }
};

extern Quantity<PseudoReducedPressure> Ppr__correlation__fct(
  const Quantity<psia>& p, const Quantity<psia>& ppc);
extern double Ppr__correlation__valid__fct(double p, double ppc);

#endif // PSEUDO_REDUCED_RATIOS_H
