#ifndef SATURATED_OIL_DENSITY_H
#define SATURATED_OIL_DENSITY_H

#include <pvt-exceptions.H>
#include <correlations/correlation.H>

/** PobBradley

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group


References:

  - beggs:1987

@param[in] yg Gas specific gravity. Range in [, ]
@param[in] rs Solution GOR. Range in [, ]
@param[in] bob Saturated oil formation volume factor. Range in [, ]
@param[in] yo Oil specific gravity. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class PobBradley : public SaturatedOilDensity {
  Correlation_Singleton(PobBradley);

  PobBradley() : SaturatedOilDensity("PobBradley", Lb_ft3::get_instance()) {
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
    add_par_synonym("yo", "api", "api");
  }

public:
  static inline double impl(const double& yg, const double& rs,
                            const double& bob, const double& yo) noexcept;

  Quantity<Lb_ft3> call(const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rs,
                        const Quantity<RB_STB>& bob,
                        const Quantity<Sg_do>& yo) const {
    return Quantity<Lb_ft3>(impl(yg.raw(), rs.raw(), bob.raw(), yo.raw()));
  }

  Quantity<Lb_ft3> validate_and_call(const Quantity<Sgg>& yg,
                                     const Quantity<SCF_STB>& rs,
                                     const Quantity<RB_STB>& bob,
                                     const Quantity<Sg_do>& yo) const {

    return call(yg, rs, bob, yo);
  }

  Quantity<Lb_ft3> operator()(const Quantity<Sgg>& yg,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<RB_STB>& bob,
                              const Quantity<Sg_do>& yo) const {
    return Quantity<Lb_ft3>(call(yg, rs, bob, yo));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& bob = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();

    return call(yg, rs, bob, yo);
  }
};

extern Quantity<Lb_ft3> PobBradley__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rs,
    const Quantity<RB_STB>& bob, const Quantity<Sg_do>& yo);
extern double PobBradley__correlation__valid__fct(const double& yg,
                                                  const double& rs,
                                                  const double& bob,
                                                  const double& yo);

#endif // SATURATED_OIL_DENSITY_H
