#ifndef WET_GAS_SPECIFIC_GRAVITY_H
#define WET_GAS_SPECIFIC_GRAVITY_H

#include <pvt-exceptions.H>
#include <correlations/correlation.H>

/** YwgrMcCain

    Generated at 2017-10-30 16:49:10 -0400

    Zen Technologies Group

Calculation of reservoir wet gas specific gravity.
Gas mixtures that produce condensate at surface conditions may exist as a
single-phase gas in the reservoir and production tubing. If reservoir/wellstream
properties are desired at conditions where the mixture is single-phase,
surface-gas and condensate properties must be converted to reservoir/wellstream
specific gravity. This gravity should be used to estimate pseudocritical
properties.
This equation is not recommended when the total nonhydrocarbon content of the
gas exceeds 25 mol%.

Producing GOR is the inverse of Condensate gas ratio.

The equation was verified by using the original reference: McCain (1991). Date:
March 06 2017.

References:

  - mcCain:1991

@param[in] yg Primary separator gas gravity. Range in [0.6, 0.8]
@param[in] yo Condensate gravity. Range in [Quantity<Api>(70),
Quantity<Api>(40)]
@param[in] rsp1 Primary separator producing GOR. Range in [, ]
@param[in] gpa Additional gas produced. Range in [10, 4000]
@param[in] veq Equivalent volume. Range in [550, 5000]
@return inside [0.8, 1.55]
@ingroup Correlations
*/
class YwgrMcCain : public WetGasSpecificGravity {
  Correlation_Singleton(YwgrMcCain);

  YwgrMcCain()
      : WetGasSpecificGravity("YwgrMcCain", Sgg::get_instance(), 0.8, 1.55) {
    set_author("McCain (Wet Gas Gravity Correction)");
    set_title("CALCULATION OF RESERVOIR WET GAS SPECIFIC GRAVITY");
    set_hidden_blackoil_grid();
    set_hidden_wetgas_grid();
    set_hidden_drygas_grid();
    add_parameter("yg", Sgg::get_instance(), "Primary separator gas gravity",
                  0.6, 0.8);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("yo", Sg_do::get_instance(), "Condensate gravity",
                  Quantity<Api>(70), Quantity<Api>(40));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("rsp1", SCF_STB::get_instance(),
                  "Primary separator producing GOR");
    def_parameter_latex_symbol("rsp1", "R_{s-sp1}");
    add_parameter("gpa", SCF_STB::get_instance(), "Additional gas produced", 10,
                  4000);
    def_parameter_latex_symbol("gpa", "G_{pa}");
    add_parameter("veq", SCF_STB::get_instance(), "Equivalent volume", 550,
                  5000);
    def_parameter_latex_symbol("veq", "V_{eq}");
    add_note("Calculation of reservoir wet gas specific gravity.");
    add_note("Gas mixtures that produce condensate at surface conditions may "
             "exist as a single-phase gas in the reservoir and production "
             "tubing. If reservoir/wellstream properties are desired at "
             "conditions where the mixture is single-phase, surface-gas and "
             "condensate properties must be converted to reservoir/wellstream "
             "specific gravity. This gravity should be used to estimate "
             "pseudocritical properties.");
    add_note("This equation is not recommended when the total nonhydrocarbon "
             "content of the gas exceeds 25 mol%.");
    add_ref("mcCain:1991");
    add_par_synonym("yo", "api", "api");
  }

public:
  static inline double impl(const double& yg, const double& yo,
                            const double& rsp1, const double& gpa,
                            const double& veq) noexcept;

  Quantity<Sgg> call(const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
                     const Quantity<SCF_STB>& rsp1,
                     const Quantity<SCF_STB>& gpa,
                     const Quantity<SCF_STB>& veq) const {
    return Quantity<Sgg>(
        impl(yg.raw(), yo.raw(), rsp1.raw(), gpa.raw(), veq.raw()));
  }

  Quantity<Sgg> validate_and_call(const Quantity<Sgg>& yg,
                                  const Quantity<Sg_do>& yo,
                                  const Quantity<SCF_STB>& rsp1,
                                  const Quantity<SCF_STB>& gpa,
                                  const Quantity<SCF_STB>& veq) const {
    if (not(yg >= 0.6))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.6");
    if (not(yg <= 0.8))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 0.8");
    if (not(yo >= Quantity<Api>(70)))
      ZENTHROW(OutOfParameterRange,
               "parameter yo is less than Quantity<Api>(70)");
    if (not(yo <= Quantity<Api>(40)))
      ZENTHROW(OutOfParameterRange,
               "parameter yo is greater than Quantity<Api>(40)");
    if (not(gpa >= 10))
      ZENTHROW(OutOfParameterRange, "parameter gpa is less than 10");
    if (not(gpa <= 4000))
      ZENTHROW(OutOfParameterRange, "parameter gpa is greater than 4000");
    if (not(veq >= 550))
      ZENTHROW(OutOfParameterRange, "parameter veq is less than 550");
    if (not(veq <= 5000))
      ZENTHROW(OutOfParameterRange, "parameter veq is greater than 5000");

    return call(yg, yo, rsp1, gpa, veq);
  }

  Quantity<Sgg> operator()(const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
                           const Quantity<SCF_STB>& rsp1,
                           const Quantity<SCF_STB>& gpa,
                           const Quantity<SCF_STB>& veq) const {
    return Quantity<Sgg>(call(yg, yo, rsp1, gpa, veq));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& rsp1 = it.get_curr();
    it.next();
    const auto& gpa = it.get_curr();
    it.next();
    const auto& veq = it.get_curr();

    return call(yg, yo, rsp1, gpa, veq);
  }
};

extern Quantity<Sgg> YwgrMcCain__correlation__fct(const Quantity<Sgg>& yg,
                                                  const Quantity<Sg_do>& yo,
                                                  const Quantity<SCF_STB>& rsp1,
                                                  const Quantity<SCF_STB>& gpa,
                                                  const Quantity<SCF_STB>& veq);
extern double YwgrMcCain__correlation__valid__fct(const double& yg,
                                                  const double& yo,
                                                  const double& rsp1,
                                                  const double& gpa,
                                                  const double& veq);

#endif // WET_GAS_SPECIFIC_GRAVITY_H
