#ifndef EQUIVALENT_VOLUME_H
#define EQUIVALENT_VOLUME_H

#include <pvt-exceptions.H>
#include <correlations/correlation.H>

/** VeqspMcCain

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group

Calculation of primary separator equivalent volume (for two stages of
separation).
The equivalent volume is the volume of stock-tank gas plus the volume in scf
that would be occupied by a barrel of stock-tank liquid if it were gas.
This equation is not recommended when the total nonhydrocarbon content of the
gas exceeds 25 mol%.

The equation was verified by using the original reference: McCain (1991). Date:
March 06 2017.

References:

  - mcCain:1991

@param[in] tsep Primary separator temperature. Range in [60, 120]
@param[in] psep Primary separator pressure. Range in [100, 700]
@param[in] yg Primary separator gas gravity. Range in [0.6, 0.8]
@param[in] api API condensate gravity. Range in [40, 70]
@return inside [650, 4500]
@ingroup Correlations
*/
class VeqspMcCain : public EquivalentVolume {
  Correlation_Singleton(VeqspMcCain);

  VeqspMcCain()
      : EquivalentVolume("VeqspMcCain", SCF_STB::get_instance(), 650, 4500) {
    set_author("McCain (Primary Separator)");
    set_title("CALCULATION OF EQUIVALENT VOLUME FOR TWO STAGES OF SEPARATION");
    set_hidden_blackoil_grid();
    set_hidden_wetgas_grid();
    set_hidden_drygas_grid();
    add_parameter("tsep", Fahrenheit::get_instance(),
                  "Primary separator temperature", 60, 120);
    def_parameter_latex_symbol("tsep", "T_{sp}");
    add_parameter("psep", psia::get_instance(), "Primary separator pressure",
                  100, 700);
    def_parameter_latex_symbol("psep", "P_{sp}");
    add_parameter("yg", Sgg::get_instance(), "Primary separator gas gravity",
                  0.6, 0.8);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("api", Api::get_instance(), "API condensate gravity", 40, 70);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_note("Calculation of primary separator equivalent volume (for two "
             "stages of separation).");
    add_note("The equivalent volume is the volume of stock-tank gas plus the "
             "volume in scf that would be occupied by a barrel of stock-tank "
             "liquid if it were gas.");
    add_note("This equation is not recommended when the total nonhydrocarbon "
             "content of the gas exceeds 25 mol%.");
    add_ref("mcCain:1991");
  }

public:
  static inline double impl(const double& tsep, const double& psep,
                            const double& yg, const double& api) noexcept;

  Quantity<SCF_STB> call(const Quantity<Fahrenheit>& tsep,
                         const Quantity<psia>& psep, const Quantity<Sgg>& yg,
                         const Quantity<Api>& api) const {
    return Quantity<SCF_STB>(impl(tsep.raw(), psep.raw(), yg.raw(), api.raw()));
  }

  Quantity<SCF_STB> validate_and_call(const Quantity<Fahrenheit>& tsep,
                                      const Quantity<psia>& psep,
                                      const Quantity<Sgg>& yg,
                                      const Quantity<Api>& api) const {
    if (not(tsep >= 60))
      ZENTHROW(OutOfParameterRange, "parameter tsep is less than 60");
    if (not(tsep <= 120))
      ZENTHROW(OutOfParameterRange, "parameter tsep is greater than 120");
    if (not(psep >= 100))
      ZENTHROW(OutOfParameterRange, "parameter psep is less than 100");
    if (not(psep <= 700))
      ZENTHROW(OutOfParameterRange, "parameter psep is greater than 700");
    if (not(yg >= 0.6))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.6");
    if (not(yg <= 0.8))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 0.8");
    if (not(api >= 40))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 40");
    if (not(api <= 70))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 70");

    return call(tsep, psep, yg, api);
  }

  Quantity<SCF_STB> operator()(const Quantity<Fahrenheit>& tsep,
                               const Quantity<psia>& psep,
                               const Quantity<Sgg>& yg,
                               const Quantity<Api>& api) const {
    return Quantity<SCF_STB>(call(tsep, psep, yg, api));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& tsep = it.get_curr();
    it.next();
    const auto& psep = it.get_curr();
    it.next();
    const auto& yg = it.get_curr();
    it.next();
    const auto& api = it.get_curr();

    return call(tsep, psep, yg, api);
  }
};

extern Quantity<SCF_STB> VeqspMcCain__correlation__fct(
    const Quantity<Fahrenheit>& tsep, const Quantity<psia>& psep,
    const Quantity<Sgg>& yg, const Quantity<Api>& api);
extern double VeqspMcCain__correlation__valid__fct(const double& tsep,
                                                   const double& psep,
                                                   const double& yg,
                                                   const double& api);

/** Veqsp2McCain

    Generated at 2017-10-30 16:49:04 -0400

    Zen Technologies Group

Calculation of second separator equivalent volume (for three stages of
separation).
The equivalent volume is the volume of stock-tank gas and second-separator gas,
plus the volume in scf that would be occupied by a barrel of stock-tank liquid
if it were gas.
This equation is not recommended when the total nonhydrocarbon content of the
gas exceeds 25 mol%.

The equation was verified by using the original reference: McCain (1991). Date:
March 06 2017.

References:

  - mcCain:1991

@param[in] tsep Primary separator temperature. Range in [60, 120]
@param[in] tsep2 Second separator temperature. Range in [60, 120]
@param[in] psep Primary separator pressure. Range in [100, 1500]
@param[in] yg Primary separator gas gravity. Range in [0.6, 0.8]
@param[in] api API condensate gravity. Range in [40, 70]
@return inside [550, 5000]
@ingroup Correlations
*/
class Veqsp2McCain : public EquivalentVolume {
  Correlation_Singleton(Veqsp2McCain);

  Veqsp2McCain()
      : EquivalentVolume("Veqsp2McCain", SCF_STB::get_instance(), 550, 5000) {
    set_author("McCain (Second Separator)");
    set_title(
        "CALCULATION OF EQUIVALENT VOLUME FOR THREE STAGES OF SEPARATION");
    set_hidden_blackoil_grid();
    set_hidden_wetgas_grid();
    set_hidden_drygas_grid();
    add_parameter("tsep", Fahrenheit::get_instance(),
                  "Primary separator temperature", 60, 120);
    def_parameter_latex_symbol("tsep", "T_{sp}");
    add_parameter("tsep2", Fahrenheit::get_instance(),
                  "Second separator temperature", 60, 120);
    add_parameter("psep", psia::get_instance(), "Primary separator pressure",
                  100, 1500);
    def_parameter_latex_symbol("psep", "P_{sp}");
    add_parameter("yg", Sgg::get_instance(), "Primary separator gas gravity",
                  0.6, 0.8);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("api", Api::get_instance(), "API condensate gravity", 40, 70);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_note("Calculation of second separator equivalent volume (for three "
             "stages of separation).");
    add_note("The equivalent volume is the volume of stock-tank gas and "
             "second-separator gas, plus the volume in scf that would be "
             "occupied by a barrel of stock-tank liquid if it were gas.");
    add_note("This equation is not recommended when the total nonhydrocarbon "
             "content of the gas exceeds 25 mol%.");
    add_ref("mcCain:1991");
  }

public:
  static inline double impl(const double& tsep, const double& tsep2,
                            const double& psep, const double& yg,
                            const double& api) noexcept;

  Quantity<SCF_STB> call(const Quantity<Fahrenheit>& tsep,
                         const Quantity<Fahrenheit>& tsep2,
                         const Quantity<psia>& psep, const Quantity<Sgg>& yg,
                         const Quantity<Api>& api) const {
    return Quantity<SCF_STB>(
        impl(tsep.raw(), tsep2.raw(), psep.raw(), yg.raw(), api.raw()));
  }

  Quantity<SCF_STB> validate_and_call(const Quantity<Fahrenheit>& tsep,
                                      const Quantity<Fahrenheit>& tsep2,
                                      const Quantity<psia>& psep,
                                      const Quantity<Sgg>& yg,
                                      const Quantity<Api>& api) const {
    if (not(tsep >= 60))
      ZENTHROW(OutOfParameterRange, "parameter tsep is less than 60");
    if (not(tsep <= 120))
      ZENTHROW(OutOfParameterRange, "parameter tsep is greater than 120");
    if (not(tsep2 >= 60))
      ZENTHROW(OutOfParameterRange, "parameter tsep2 is less than 60");
    if (not(tsep2 <= 120))
      ZENTHROW(OutOfParameterRange, "parameter tsep2 is greater than 120");
    if (not(psep >= 100))
      ZENTHROW(OutOfParameterRange, "parameter psep is less than 100");
    if (not(psep <= 1500))
      ZENTHROW(OutOfParameterRange, "parameter psep is greater than 1500");
    if (not(yg >= 0.6))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.6");
    if (not(yg <= 0.8))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 0.8");
    if (not(api >= 40))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 40");
    if (not(api <= 70))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 70");

    return call(tsep, tsep2, psep, yg, api);
  }

  Quantity<SCF_STB> operator()(const Quantity<Fahrenheit>& tsep,
                               const Quantity<Fahrenheit>& tsep2,
                               const Quantity<psia>& psep,
                               const Quantity<Sgg>& yg,
                               const Quantity<Api>& api) const {
    return Quantity<SCF_STB>(call(tsep, tsep2, psep, yg, api));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& tsep = it.get_curr();
    it.next();
    const auto& tsep2 = it.get_curr();
    it.next();
    const auto& psep = it.get_curr();
    it.next();
    const auto& yg = it.get_curr();
    it.next();
    const auto& api = it.get_curr();

    return call(tsep, tsep2, psep, yg, api);
  }
};

extern Quantity<SCF_STB> Veqsp2McCain__correlation__fct(
    const Quantity<Fahrenheit>& tsep, const Quantity<Fahrenheit>& tsep2,
    const Quantity<psia>& psep, const Quantity<Sgg>& yg,
    const Quantity<Api>& api);
extern double Veqsp2McCain__correlation__valid__fct(const double& tsep,
                                                    const double& tsep2,
                                                    const double& psep,
                                                    const double& yg,
                                                    const double& api);

#endif // EQUIVALENT_VOLUME_H
