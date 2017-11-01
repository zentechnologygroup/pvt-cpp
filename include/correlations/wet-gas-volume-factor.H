#ifndef WET_GAS_VOLUME_FACTOR_H
#define WET_GAS_VOLUME_FACTOR_H

#include <pvt-exceptions.H>
#include <correlations/correlation.H>

/** Bwg

    Generated at 2017-10-30 16:49:11 -0400

    Zen Technologies Group

The formation volume factor of a wet gas is defined as the volume of reservoir
gas required to produce one stock-tank barrel of liquid at the surface.

The equation was verified by using as reference: McCain (1990). Date: March 06
2017.

References:

  - mcCain:1990

@param[in] t Temperature. Range in [, ]
@param[in] p Pressure. Range in [, ]
@param[in] z Gas compressibility factor. Range in [, ]
@param[in] rsp1 Primary separator producing GOR. Range in [, ]
@param[in] veq Equivalent volume. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class Bwg : public WetGasVolumeFactor {
  Correlation_Singleton(Bwg);

  Bwg() : WetGasVolumeFactor("Bwg", RCFGas_STB::get_instance()) {
    set_author("Standard Equation");
    set_title("CALCULATION OF WET GAS FORMATION VOLUME FACTOR");
    add_parameter("t", Rankine::get_instance(), "Temperature");
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("z", Zfactor::get_instance(), "Gas compressibility factor");
    def_parameter_latex_symbol("z", "Z\\ factor");
    add_parameter("rsp1", SCF_STB::get_instance(),
                  "Primary separator producing GOR");
    def_parameter_latex_symbol("rsp1", "R_{s-sp1}");
    add_parameter("veq", SCF_STB::get_instance(), "Equivalent volume");
    def_parameter_latex_symbol("veq", "V_{eq}");
    add_note("The formation volume factor of a wet gas is defined as the "
             "volume of reservoir gas required to produce one stock-tank "
             "barrel of liquid at the surface.");
    add_ref("mcCain:1990");
  }

public:
  static inline double impl(const double& t, const double& p, const double& z,
                            const double& rsp1, const double& veq) noexcept;

  Quantity<RCFGas_STB> call(const Quantity<Rankine>& t, const Quantity<psia>& p,
                            const Quantity<Zfactor>& z,
                            const Quantity<SCF_STB>& rsp1,
                            const Quantity<SCF_STB>& veq) const {
    return Quantity<RCFGas_STB>(
        impl(t.raw(), p.raw(), z.raw(), rsp1.raw(), veq.raw()));
  }

  Quantity<RCFGas_STB> validate_and_call(const Quantity<Rankine>& t,
                                         const Quantity<psia>& p,
                                         const Quantity<Zfactor>& z,
                                         const Quantity<SCF_STB>& rsp1,
                                         const Quantity<SCF_STB>& veq) const {

    return call(t, p, z, rsp1, veq);
  }

  Quantity<RCFGas_STB> operator()(const Quantity<Rankine>& t,
                                  const Quantity<psia>& p,
                                  const Quantity<Zfactor>& z,
                                  const Quantity<SCF_STB>& rsp1,
                                  const Quantity<SCF_STB>& veq) const {
    return Quantity<RCFGas_STB>(call(t, p, z, rsp1, veq));
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
    it.next();
    const auto& rsp1 = it.get_curr();
    it.next();
    const auto& veq = it.get_curr();

    return call(t, p, z, rsp1, veq);
  }
};

extern Quantity<RCFGas_STB> Bwg__correlation__fct(const Quantity<Rankine>& t,
                                                  const Quantity<psia>& p,
                                                  const Quantity<Zfactor>& z,
                                                  const Quantity<SCF_STB>& rsp1,
                                                  const Quantity<SCF_STB>& veq);
extern double Bwg__correlation__valid__fct(const double& t, const double& p,
                                           const double& z, const double& rsp1,
                                           const double& veq);

#endif // WET_GAS_VOLUME_FACTOR_H
