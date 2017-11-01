#ifndef PSEUDO_REDUCED_RATIOS_H
#define PSEUDO_REDUCED_RATIOS_H

#include <pvt-exceptions.H>
#include <correlations/correlation.H>

/** Tpr

    Generated at 2017-10-30 16:49:06 -0400

    Zen Technologies Group

The temperature is expressed as a ratio of its critical value.

References:

  - ahmed:1989

@param[in] t Temperature. Range in [, ]
@param[in] tpc Gas pseudocritical temperature. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class Tpr : public GasPseudoreducedTemperature {
  Correlation_Singleton(Tpr);

  Tpr()
      : GasPseudoreducedTemperature("Tpr",
                                    PseudoReducedTemperature::get_instance()) {
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
  static inline double impl(const double& t, const double& tpc) noexcept;

  Quantity<PseudoReducedTemperature> call(const Quantity<Rankine>& t,
                                          const Quantity<Rankine>& tpc) const {
    return Quantity<PseudoReducedTemperature>(impl(t.raw(), tpc.raw()));
  }

  Quantity<PseudoReducedTemperature> validate_and_call(
      const Quantity<Rankine>& t, const Quantity<Rankine>& tpc) const {

    return call(t, tpc);
  }

  Quantity<PseudoReducedTemperature> operator()(
      const Quantity<Rankine>& t, const Quantity<Rankine>& tpc) const {
    return Quantity<PseudoReducedTemperature>(call(t, tpc));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& t = it.get_curr();
    it.next();
    const auto& tpc = it.get_curr();

    return call(t, tpc);
  }
};

extern Quantity<PseudoReducedTemperature> Tpr__correlation__fct(
    const Quantity<Rankine>& t, const Quantity<Rankine>& tpc);
extern double Tpr__correlation__valid__fct(const double& t, const double& tpc);

/** Ppr

    Generated at 2017-10-30 16:49:06 -0400

    Zen Technologies Group

The pressure is expressed as a ratio of its critical value.

References:

  - ahmed:1989

@param[in] p Pressure. Range in [, ]
@param[in] ppc Gas pseudocritical pressure. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class Ppr : public GasPseudoreducedPressure {
  Correlation_Singleton(Ppr);

  Ppr()
      : GasPseudoreducedPressure("Ppr", PseudoReducedPressure::get_instance()) {
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
  static inline double impl(const double& p, const double& ppc) noexcept;

  Quantity<PseudoReducedPressure> call(const Quantity<psia>& p,
                                       const Quantity<psia>& ppc) const {
    return Quantity<PseudoReducedPressure>(impl(p.raw(), ppc.raw()));
  }

  Quantity<PseudoReducedPressure> validate_and_call(
      const Quantity<psia>& p, const Quantity<psia>& ppc) const {

    return call(p, ppc);
  }

  Quantity<PseudoReducedPressure> operator()(const Quantity<psia>& p,
                                             const Quantity<psia>& ppc) const {
    return Quantity<PseudoReducedPressure>(call(p, ppc));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& p = it.get_curr();
    it.next();
    const auto& ppc = it.get_curr();

    return call(p, ppc);
  }
};

extern Quantity<PseudoReducedPressure> Ppr__correlation__fct(
    const Quantity<psia>& p, const Quantity<psia>& ppc);
extern double Ppr__correlation__valid__fct(const double& p, const double& ppc);

#endif // PSEUDO_REDUCED_RATIOS_H
