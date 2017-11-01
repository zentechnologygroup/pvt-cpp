#ifndef GAS_VOLUME_FACTOR_H
#define GAS_VOLUME_FACTOR_H

#include <pvt-exceptions.H>
#include <correlations/correlation.H>

/** Bg

    Generated at 2017-10-30 16:49:06 -0400

    Zen Technologies Group


The equation was verified by using secondary references: Bánzer (1996) and
PetroWiki (2016). Date: September 28 2016.

References:

  - takacs:2005

  - petroWiki:2016

@param[in] t Temperature. Range in [, ]
@param[in] p Pressure. Range in [, ]
@param[in] z Gas compressibility factor. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class Bg : public GasVolumeFactor {
  Correlation_Singleton(Bg);

  Bg() : GasVolumeFactor("Bg", RCF_SCF::get_instance()) {
    set_author("Standard Equation");
    set_title("CALCULATION OF GAS FORMATION VOLUME FACTOR");
    add_parameter("t", Rankine::get_instance(), "Temperature");
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("z", Zfactor::get_instance(), "Gas compressibility factor");
    def_parameter_latex_symbol("z", "Z\\ factor");
    add_db("The calculation is based on the Real Gas Law.");
    add_ref("takacs:2005");
    add_ref("petroWiki:2016");
  }

public:
  static inline double impl(const double& t, const double& p,
                            const double& z) noexcept;

  Quantity<RCF_SCF> call(const Quantity<Rankine>& t, const Quantity<psia>& p,
                         const Quantity<Zfactor>& z) const {
    return Quantity<RCF_SCF>(impl(t.raw(), p.raw(), z.raw()));
  }

  Quantity<RCF_SCF> validate_and_call(const Quantity<Rankine>& t,
                                      const Quantity<psia>& p,
                                      const Quantity<Zfactor>& z) const {

    return call(t, p, z);
  }

  Quantity<RCF_SCF> operator()(const Quantity<Rankine>& t,
                               const Quantity<psia>& p,
                               const Quantity<Zfactor>& z) const {
    return Quantity<RCF_SCF>(call(t, p, z));
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
    const auto& z = it.get_curr();

    return call(t, p, z);
  }
};

extern Quantity<RCF_SCF> Bg__correlation__fct(const Quantity<Rankine>& t,
                                              const Quantity<psia>& p,
                                              const Quantity<Zfactor>& z);
extern double Bg__correlation__valid__fct(const double& t, const double& p,
                                          const double& z);

#endif // GAS_VOLUME_FACTOR_H
