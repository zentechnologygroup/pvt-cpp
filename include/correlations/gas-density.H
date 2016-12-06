#ifndef GAS_DENSITY_H
#define GAS_DENSITY_H

#include <correlations/correlation.H>

/** Pg

The equation was verified by using a secondary reference: Bánzer (1996). Date:
September 28 2016.


*/
class Pg : public GasDensity
{
  Correlation_Singleton(Pg);

  Pg()
    : GasDensity("Pg", Lb_ft3::get_instance())
  {
    set_author("Standard Equation");
    set_title("CALCULATION OF GAS DENSITY");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity");
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("pb", psia::get_instance(), "Bubble point pressure");
    def_parameter_latex_symbol("pb", "P_b");
    add_parameter("t", Rankine::get_instance(), "Temperature");
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("z", ZFactor::get_instance(), "Gas compressibility factor");
    def_parameter_latex_symbol("z", "Z\\ factor");
    add_db("The calculation is based on the Real Gas Law.");
    add_ref("takacs:2005");
  }

public:
  inline Quantity<Lb_ft3> impl(const Quantity<Sgg>& yg,
                               const Quantity<psia>& pb,
                               const Quantity<Rankine>& t,
                               const Quantity<psia>& p,
                               const Quantity<ZFactor>& z) const;

  Quantity<Lb_ft3> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& pb,
                              const Quantity<Rankine>& t,
                              const Quantity<psia>& p,
                              const Quantity<ZFactor>& z) const
  {
    return impl(yg, pb, t, p, z);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& pb = it.get_curr();
      it.next();
      const auto& t = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& z = it.get_curr();

      return impl(yg, pb, t, p, z);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& z = it.get_curr();

    return impl(yg, pb, t, p, z);
  }
};

extern Quantity<Lb_ft3> Pg__correlation__fct(const Quantity<Sgg>& yg,
                                             const Quantity<psia>& pb,
                                             const Quantity<Rankine>& t,
                                             const Quantity<psia>& p,
                                             const Quantity<ZFactor>& z);
extern double Pg__correlation__valid__fct(double yg, double pb, double t,
                                          double p, double z);

#endif // GAS_DENSITY_H
