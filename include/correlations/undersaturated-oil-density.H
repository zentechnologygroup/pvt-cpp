#ifndef UNDERSATURATED_OIL_DENSITY_H
#define UNDERSATURATED_OIL_DENSITY_H

#include <correlations/correlation.H>

/** PoaBradley


*/
class PoaBradley : public UndersaturatedOilDensity
{
  Correlation_Singleton(PoaBradley);

  PoaBradley()
    : UndersaturatedOilDensity("PoaBradley", Lb_ft3::get_instance())
  {
    set_author("Standard Equation");
    set_title("CALCULATION OF UNDERSATURATED OIL DENSITY");
    add_parameter("pobp", Lb_ft3::get_instance(), "Oil density at Pb");
    def_parameter_latex_symbol("pobp", "\\rho_{obp}");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("pb", psia::get_instance(), "Bubble point pressure");
    def_parameter_latex_symbol("pb", "P_b");
    add_parameter("co", psia_1::get_instance(),
                  "Oil isothermal compressibility");
    def_parameter_latex_symbol("co", "c_{o}");
    add_ref("beggs:1987");
  }

public:
  inline Quantity<Lb_ft3> impl(const Quantity<Lb_ft3>& pobp,
                               const Quantity<psia>& p,
                               const Quantity<psia>& pb,
                               const Quantity<psia_1>& co) const;

  Quantity<Lb_ft3> operator()(const Quantity<Lb_ft3>& pobp,
                              const Quantity<psia>& p, const Quantity<psia>& pb,
                              const Quantity<psia_1>& co) const
  {
    return impl(pobp, p, pb, co);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& pobp = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& pb = it.get_curr();
      it.next();
      const auto& co = it.get_curr();

      return impl(pobp, p, pb, co);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& pobp = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();
    it.next();
    const auto& co = it.get_curr();

    return impl(pobp, p, pb, co);
  }
};

extern Quantity<Lb_ft3> PoaBradley__correlation__fct(
  const Quantity<Lb_ft3>& pobp, const Quantity<psia>& p,
  const Quantity<psia>& pb, const Quantity<psia_1>& co);
extern double PoaBradley__correlation__valid__fct(double pobp, double p,
                                                  double pb, double co);

#endif // UNDERSATURATED_OIL_DENSITY_H
