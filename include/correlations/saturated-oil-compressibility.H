#ifndef SATURATED_OIL_COMPRESSIBILITY_H
#define SATURATED_OIL_COMPRESSIBILITY_H

#include <correlations/correlation.H>

/** CobMcCainEtAl


*/
class CobMcCainEtAl : public SaturatedOilCompressibility
{
  Correlation_Singleton(CobMcCainEtAl);

  CobMcCainEtAl()
    : SaturatedOilCompressibility("CobMcCainEtAl", psia_1::get_instance(),
                                  3.1e-05, 0.0066)
  {
    set_author("McCain, Rollins & Villena");
    set_title("McCAIN, ROLLINS & VILLENA CORRELATION, CALCULATION OF SATURATED "
              "OIL ISOTHERMAL COMPRESSIBILITY");
    add_parameter("api", Api::get_instance(), "API oil gravity", 18, 52);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 15,
                  1947);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("t", Rankine::get_instance(), "Temperature",
                  Quantity<Fahrenheit>(78), Quantity<Fahrenheit>(330));
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure", 500, 5300);
    def_parameter_latex_symbol("p", "P");
    add_parameter("pb", psia::get_instance(), "Bubble point pressure", 763,
                  5300);
    def_parameter_latex_symbol("pb", "P_b");
    add_db(
      "Based on 2500 experimental data obtained from different crude oils.");
    add_db("The equation was developed with data from reservoir-fluid studies "
           "of 260 oil fields worldwide.");
    add_ref("mcCain:1988");
  }

public:
  inline Quantity<psia_1> impl(const Quantity<Api>& api,
                               const Quantity<SCF_STB>& rsb,
                               const Quantity<Rankine>& t,
                               const Quantity<psia>& p,
                               const Quantity<psia>& pb) const;

  Quantity<psia_1> operator()(const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Rankine>& t,
                              const Quantity<psia>& p,
                              const Quantity<psia>& pb) const
  {
    return impl(api, rsb, t, p, pb);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& api = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();
      it.next();
      const auto& t = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& pb = it.get_curr();

      return impl(api, rsb, t, p, pb);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& api = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();

    return impl(api, rsb, t, p, pb);
  }
};

extern Quantity<psia_1> CobMcCainEtAl__correlation__fct(
  const Quantity<Api>& api, const Quantity<SCF_STB>& rsb,
  const Quantity<Rankine>& t, const Quantity<psia>& p,
  const Quantity<psia>& pb);
extern double CobMcCainEtAl__correlation__valid__fct(double api, double rsb,
                                                     double t, double p,
                                                     double pb);

#endif // SATURATED_OIL_COMPRESSIBILITY_H
