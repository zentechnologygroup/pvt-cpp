#ifndef GAS_DENSITY_H
#define GAS_DENSITY_H

#include <pvt-exceptions.H>
#include <correlations/correlation.H>

/** Pg

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group


The equation was verified by using secondary references: Ahmed (2010) and Bánzer
(1996). Date: September 28 2016.

References:

  - takacs:2005

  - ahmed:2010

@param[in] yg Gas specific gravity. Range in [, ]
@param[in] t Temperature. Range in [, ]
@param[in] p Pressure. Range in [, ]
@param[in] z Gas compressibility factor. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class Pg : public GasDensity {
  Correlation_Singleton(Pg);

  Pg() : GasDensity("Pg", Lb_ft3::get_instance()) {
    set_author("Standard Equation");
    set_title("CALCULATION OF GAS DENSITY");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity");
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("t", Rankine::get_instance(), "Temperature");
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("z", Zfactor::get_instance(), "Gas compressibility factor");
    def_parameter_latex_symbol("z", "Z\\ factor");
    add_db("The calculation is based on the Real Gas Law.");
    add_ref("takacs:2005");
    add_ref("ahmed:2010");
  }

public:
  static inline double impl(const double& yg, const double& t, const double& p,
                            const double& z) noexcept;

  Quantity<Lb_ft3> call(const Quantity<Sgg>& yg, const Quantity<Rankine>& t,
                        const Quantity<psia>& p,
                        const Quantity<Zfactor>& z) const {
    return Quantity<Lb_ft3>(impl(yg.raw(), t.raw(), p.raw(), z.raw()));
  }

  Quantity<Lb_ft3> validate_and_call(const Quantity<Sgg>& yg,
                                     const Quantity<Rankine>& t,
                                     const Quantity<psia>& p,
                                     const Quantity<Zfactor>& z) const {

    return call(yg, t, p, z);
  }

  Quantity<Lb_ft3> operator()(const Quantity<Sgg>& yg,
                              const Quantity<Rankine>& t,
                              const Quantity<psia>& p,
                              const Quantity<Zfactor>& z) const {
    return Quantity<Lb_ft3>(call(yg, t, p, z));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& z = it.get_curr();

    return call(yg, t, p, z);
  }
};

extern Quantity<Lb_ft3> Pg__correlation__fct(const Quantity<Sgg>& yg,
                                             const Quantity<Rankine>& t,
                                             const Quantity<psia>& p,
                                             const Quantity<Zfactor>& z);
extern double Pg__correlation__valid__fct(const double& yg, const double& t,
                                          const double& p, const double& z);

#endif // GAS_DENSITY_H
