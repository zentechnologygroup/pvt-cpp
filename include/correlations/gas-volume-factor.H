#ifndef GAS_VOLUME_FACTOR_H
#define GAS_VOLUME_FACTOR_H

#include <correlations/correlation.H>

/** Bg

The equation was verified by using secondary references: Bánzer (1996) and
PetroWiki (2016). Date: September 28 2016.


*/
class Bg : public GasVolumeFactor
{
  Correlation_Singleton(Bg);

  Bg()
    : GasVolumeFactor("Bg", RCF_SCF::get_instance())
  {
    set_author("Standard Equation");
    set_title("CALCULATION OF GAS FORMATION VOLUME FACTOR");
    add_parameter("t", Rankine::get_instance(), "Temperature");
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("z", ZFactor::get_instance(), "Gas compressibility factor");
    def_parameter_latex_symbol("z", "Z\\ factor");
    add_db("The calculation is based on the Real Gas Law.");
    add_ref("takacs:2005");
    add_ref("petroWiki:2016");
  }

public:
  inline Quantity<RCF_SCF> impl(const Quantity<Rankine>& t,
                                const Quantity<psia>& p,
                                const Quantity<ZFactor>& z) const;

  Quantity<RCF_SCF> operator()(const Quantity<Rankine>& t,
                               const Quantity<psia>& p,
                               const Quantity<ZFactor>& z) const
  {
    return impl(t, p, z);
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
      const auto& z = it.get_curr();

      return impl(t, p, z);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& z = it.get_curr();

    return impl(t, p, z);
  }
};

extern Quantity<RCF_SCF> Bg__correlation__fct(const Quantity<Rankine>& t,
                                              const Quantity<psia>& p,
                                              const Quantity<ZFactor>& z);
extern double Bg__correlation__valid__fct(double t, double p, double z);

#endif // GAS_VOLUME_FACTOR_H
