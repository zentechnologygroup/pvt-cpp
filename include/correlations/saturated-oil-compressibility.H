#ifndef SATURATED_OIL_COMPRESSIBILITY_H
#define SATURATED_OIL_COMPRESSIBILITY_H

#include <pvt-exceptions.H>
#include <correlations/correlation.H>

/** CobMcCainEtAl

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group


References:

  - mcCain:1988

@param[in] api API oil gravity. Range in [18, 52]
@param[in] rsb Solution GOR at Pb. Range in [15, 1947]
@param[in] t Temperature. Range in [Quantity<Fahrenheit>(78),
Quantity<Fahrenheit>(330)]
@param[in] p Pressure. Range in [500, 5300]
@param[in] pb Bubble point pressure. Range in [763, 5300]
@return inside [3.1e-05, 0.0066]
@ingroup Correlations
*/
class CobMcCainEtAl : public SaturatedOilCompressibility {
  Correlation_Singleton(CobMcCainEtAl);

  CobMcCainEtAl()
      : SaturatedOilCompressibility("CobMcCainEtAl", psia_1::get_instance(),
                                    3.1e-05, 0.0066) {
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

  inline void precondition(const Quantity<psia>& p,
                           const Quantity<psia>& pb) const;

public:
  static inline double impl(const double& api, const double& rsb,
                            const double& t, const double& p,
                            const double& pb) noexcept;

  Quantity<psia_1> call(const Quantity<Api>& api, const Quantity<SCF_STB>& rsb,
                        const Quantity<Rankine>& t, const Quantity<psia>& p,
                        const Quantity<psia>& pb) const {
    precondition(p, pb);
    return Quantity<psia_1>(
        impl(api.raw(), rsb.raw(), t.raw(), p.raw(), pb.raw()));
  }

  Quantity<psia_1> validate_and_call(const Quantity<Api>& api,
                                     const Quantity<SCF_STB>& rsb,
                                     const Quantity<Rankine>& t,
                                     const Quantity<psia>& p,
                                     const Quantity<psia>& pb) const {
    if (not(api >= 18))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 18");
    if (not(api <= 52))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 52");
    if (not(rsb >= 15))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 15");
    if (not(rsb <= 1947))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 1947");
    if (not(pb >= 763))
      ZENTHROW(OutOfParameterRange, "parameter pb is less than 763");
    if (not(pb <= 5300))
      ZENTHROW(OutOfParameterRange, "parameter pb is greater than 5300");

    return call(api, rsb, t, p, pb);
  }

  Quantity<psia_1> operator()(const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Rankine>& t,
                              const Quantity<psia>& p,
                              const Quantity<psia>& pb) const {
    return Quantity<psia_1>(call(api, rsb, t, p, pb));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

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

    return call(api, rsb, t, p, pb);
  }
};

extern Quantity<psia_1> CobMcCainEtAl__correlation__fct(
    const Quantity<Api>& api, const Quantity<SCF_STB>& rsb,
    const Quantity<Rankine>& t, const Quantity<psia>& p,
    const Quantity<psia>& pb);
extern double CobMcCainEtAl__correlation__valid__fct(const double& api,
                                                     const double& rsb,
                                                     const double& t,
                                                     const double& p,
                                                     const double& pb);

#endif // SATURATED_OIL_COMPRESSIBILITY_H
