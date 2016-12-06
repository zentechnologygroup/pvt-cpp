#ifndef SATURATED_OIL_DENSITY_H
#define SATURATED_OIL_DENSITY_H

#include <correlations/correlation.H>

/** PobBradley


*/
class PobBradley : public SaturatedOilDensity
{
  Correlation_Singleton(PobBradley);

  PobBradley()
    : SaturatedOilDensity("PobBradley", Lb_ft3::get_instance())
  {
    set_author("Standard Equation");
    set_title("CALCULATION OF SATURATED OIL DENSITY");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity");
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR");
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("bob", RB_STB::get_instance(),
                  "Saturated oil formation volume factor");
    def_parameter_latex_symbol("bob", "B_{ob}");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity");
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_ref("beggs:1987");
  }

public:
  inline Quantity<Lb_ft3> impl(const Quantity<Sgg>& yg,
                               const Quantity<SCF_STB>& rs,
                               const Quantity<RB_STB>& bob,
                               const Quantity<Sg_do>& yo) const;

  Quantity<Lb_ft3> operator()(const Quantity<Sgg>& yg,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<RB_STB>& bob,
                              const Quantity<Sg_do>& yo) const
  {
    return impl(yg, rs, bob, yo);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& rs = it.get_curr();
      it.next();
      const auto& bob = it.get_curr();
      it.next();
      const auto& yo = it.get_curr();

      return impl(yg, rs, bob, yo);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& bob = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();

    return impl(yg, rs, bob, yo);
  }
};

extern Quantity<Lb_ft3> PobBradley__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rs,
  const Quantity<RB_STB>& bob, const Quantity<Sg_do>& yo);
extern double PobBradley__correlation__valid__fct(double yg, double rs,
                                                  double bob, double yo);

#endif // SATURATED_OIL_DENSITY_H
