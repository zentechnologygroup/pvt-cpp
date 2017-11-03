#ifndef UNDERSATURATED_OIL_DENSITY_H
#define UNDERSATURATED_OIL_DENSITY_H

#include <pvt-exceptions.H>
#include <correlations/correlation.H>

/** PoaBradley

    Generated at 2017-10-30 16:49:09 -0400

    Zen Technologies Group


References:

  - beggs:1987

@param[in] pobp Oil density at Pb. Range in [, ]
@param[in] p Pressure. Range in [, ]
@param[in] pb Bubble point pressure. Range in [, ]
@param[in] coa Oil isothermal compressibility. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class PoaBradley : public UndersaturatedOilDensity {
  Correlation_Singleton(PoaBradley);

  PoaBradley()
      : UndersaturatedOilDensity("PoaBradley", Lb_ft3::get_instance()) {
    set_author("Standard Equation");
    set_title("CALCULATION OF UNDERSATURATED OIL DENSITY");
    add_parameter("pobp", Lb_ft3::get_instance(), "Oil density at Pb");
    def_parameter_latex_symbol("pobp", "\\rho_{obp}");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("pb", psia::get_instance(), "Bubble point pressure");
    def_parameter_latex_symbol("pb", "P_b");
    add_parameter("coa", psia_1::get_instance(),
                  "Oil isothermal compressibility");
    def_parameter_latex_symbol("coa", "c_{oa}");
    add_db("Calculation of oil density at pressures above the bubble point.");
    add_ref("beggs:1987");
  }

  inline void precondition(const Quantity<psia>& p,
                           const Quantity<psia>& pb) const;

public:
  static inline double impl(const double& pobp, const double& p,
                            const double& pb, const double& coa) noexcept;

  Quantity<Lb_ft3> call(const Quantity<Lb_ft3>& pobp, const Quantity<psia>& p,
                        const Quantity<psia>& pb,
                        const Quantity<psia_1>& coa) const {
    precondition(p, pb);
    return Quantity<Lb_ft3>(impl(pobp.raw(), p.raw(), pb.raw(), coa.raw()));
  }

  Quantity<Lb_ft3> validate_and_call(const Quantity<Lb_ft3>& pobp,
                                     const Quantity<psia>& p,
                                     const Quantity<psia>& pb,
                                     const Quantity<psia_1>& coa) const {

    return call(pobp, p, pb, coa);
  }

  Quantity<Lb_ft3> operator()(const Quantity<Lb_ft3>& pobp,
                              const Quantity<psia>& p, const Quantity<psia>& pb,
                              const Quantity<psia_1>& coa) const {
    return Quantity<Lb_ft3>(call(pobp, p, pb, coa));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& pobp = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();
    it.next();
    const auto& coa = it.get_curr();

    return call(pobp, p, pb, coa);
  }
};

extern Quantity<Lb_ft3> PoaBradley__correlation__fct(
    const Quantity<Lb_ft3>& pobp, const Quantity<psia>& p,
    const Quantity<psia>& pb, const Quantity<psia_1>& coa);
extern double PoaBradley__correlation__valid__fct(const double& pobp,
                                                  const double& p,
                                                  const double& pb,
                                                  const double& coa);

#endif // UNDERSATURATED_OIL_DENSITY_H
