#ifndef SATURATED_WATER_VOLUME_FACTOR_H
#define SATURATED_WATER_VOLUME_FACTOR_H

#include <pvt-exceptions.H>
#include <correlations/correlation.H>

/** BwbSpiveyMN

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group

The formation volume factor is found by dividing the volume at reservoir
conditions by the volume at stock tank conditions.
This correlation provides an accurate and consistent estimate of formation
volume factor at pressures below or equal to the bubble point pressure.

The correlation was verified by using the original reference and McCain et al.
(2011) as a secondary reference. Date: October 18 2016.

References:

  - spivey:2004

  - mcCain:2011

@param[in] t Temperature. Range in [0, 275]
@param[in] p Pressure. Range in [0.1, 200]
@param[in] nacl Dissolved salt concentration. Range in [0, 5.7]
@return inside [, ]
@ingroup Correlations
*/
class BwbSpiveyMN : public SaturatedWaterVolumeFactor {
  Correlation_Singleton(BwbSpiveyMN);

  BwbSpiveyMN()
      : SaturatedWaterVolumeFactor("BwbSpiveyMN", Rm3_Sm3::get_instance()) {
    set_author("Spivey, McCain & North");
    set_title("SPIVEY, McCAIN & NORTH CORRELATION, CALCULATION OF WATER "
              "FORMATION VOLUME FACTOR ");
    add_parameter("t", Celsius::get_instance(), "Temperature", 0, 275);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", mPascal::get_instance(), "Pressure", 0.1, 200);
    def_parameter_latex_symbol("p", "P");
    add_parameter("nacl", Molality_NaCl::get_instance(),
                  "Dissolved salt concentration", 0, 5.7);
    add_db("The volumetric properties of oilfield brines were obtained by "
           "combining the correlations developed by Spivey, McCain & North "
           "(2004), for density of methane-free sodium chloride brine and "
           "solubility of methane in sodium chloride brines, with published "
           "correlations for specific volume and partial molar volume of "
           "methane in brine.");
    add_note("The formation volume factor is found by dividing the volume at "
             "reservoir conditions by the volume at stock tank conditions.");
    add_note("This correlation provides an accurate and consistent estimate of "
             "formation volume factor at pressures below or equal to the "
             "bubble point pressure.");
    add_ref("spivey:2004");
    add_ref("mcCain:2011");
  }

  inline void precondition(const Quantity<Celsius>& t,
                           const Quantity<mPascal>& p) const;

public:
  static inline double impl(const double& t, const double& p,
                            const double& nacl) noexcept;

  Quantity<Rm3_Sm3> call(const Quantity<Celsius>& t, const Quantity<mPascal>& p,
                         const Quantity<Molality_NaCl>& nacl) const {
    precondition(t, p);
    return Quantity<Rm3_Sm3>(impl(t.raw(), p.raw(), nacl.raw()));
  }

  Quantity<Rm3_Sm3> validate_and_call(
      const Quantity<Celsius>& t, const Quantity<mPascal>& p,
      const Quantity<Molality_NaCl>& nacl) const {
    if (not(nacl >= 0))
      ZENTHROW(OutOfParameterRange, "parameter nacl is less than 0");
    if (not(nacl <= 5.7))
      ZENTHROW(OutOfParameterRange, "parameter nacl is greater than 5.7");

    return call(t, p, nacl);
  }

  Quantity<Rm3_Sm3> operator()(const Quantity<Celsius>& t,
                               const Quantity<mPascal>& p,
                               const Quantity<Molality_NaCl>& nacl) const {
    return Quantity<Rm3_Sm3>(call(t, p, nacl));
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
    const auto& nacl = it.get_curr();

    return call(t, p, nacl);
  }
};

extern Quantity<Rm3_Sm3> BwbSpiveyMN__correlation__fct(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p,
    const Quantity<Molality_NaCl>& nacl);
extern double BwbSpiveyMN__correlation__valid__fct(const double& t,
                                                   const double& p,
                                                   const double& nacl);

/** BwbMcCain

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group

This correlation determines the water formation volume factor for pressures
below or equal to bubble point pressure, and it is valid for oilfield waters
with widely varying brine concentrations.

The correlation was verified by using the original reference and Bánzer (1996)
as a secondary reference. Date: October 18 2016.

References:

  - mcCain:1990

@param[in] t Temperature. Range in [90, 260]
@param[in] p Pressure. Range in [1000, 5000]
@return inside [, ]
@ingroup Correlations
*/
class BwbMcCain : public SaturatedWaterVolumeFactor {
  Correlation_Singleton(BwbMcCain);

  BwbMcCain()
      : SaturatedWaterVolumeFactor("BwbMcCain", RB_STB::get_instance()) {
    set_author("McCain");
    set_title("McCain CORRELATION, CALCULATION OF WATER FORMATION VOLUME "
              "FACTOR (FOR PRESSURES BELOW OR EQUAL TO THE BUBBLE POINT)");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 90, 260);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure", 1000, 5000);
    def_parameter_latex_symbol("p", "P");
    add_db("Based on graphs, presented by McCain (1990), of the change in "
           "volume as a function of reservoir temperature and pressure.");
    add_note("This correlation determines the water formation volume factor "
             "for pressures below or equal to bubble point pressure, and it is "
             "valid for oilfield waters with widely varying brine "
             "concentrations.");
    add_ref("mcCain:1990");
  }

public:
  static inline double impl(const double& t, const double& p) noexcept;

  Quantity<RB_STB> call(const Quantity<Fahrenheit>& t,
                        const Quantity<psia>& p) const {
    return Quantity<RB_STB>(impl(t.raw(), p.raw()));
  }

  Quantity<RB_STB> validate_and_call(const Quantity<Fahrenheit>& t,
                                     const Quantity<psia>& p) const {

    return call(t, p);
  }

  Quantity<RB_STB> operator()(const Quantity<Fahrenheit>& t,
                              const Quantity<psia>& p) const {
    return Quantity<RB_STB>(call(t, p));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();

    return call(t, p);
  }
};

extern Quantity<RB_STB> BwbMcCain__correlation__fct(
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p);
extern double BwbMcCain__correlation__valid__fct(const double& t,
                                                 const double& p);

/** BwbMcCoy

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group

This correlation determines the formation volume factor for gas-saturated pure
water, for pressures below or equal to bubble point pressure.

The original reference is not available. The correlation was verified by using
Bánzer (1996) as a secondary reference. Date: October 18 2016.

References:

  - mcCoy:1983

@param[in] t Temperature. Range in [, ]
@param[in] p Pressure. Range in [, ]
@param[in] nacl Dissolved salt concentration. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class BwbMcCoy : public SaturatedWaterVolumeFactor {
  Correlation_Singleton(BwbMcCoy);

  BwbMcCoy() : SaturatedWaterVolumeFactor("BwbMcCoy", RB_STB::get_instance()) {
    set_author("McCoy");
    set_title("McCOY CORRELATION, CALCULATION OF WATER FORMATION VOLUME FACTOR "
              "(FOR PRESSURES BELOW OR EQUAL TO THE BUBBLE POINT)");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature");
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("nacl", Dissolved_Salt_Percent::get_instance(),
                  "Dissolved salt concentration");
    add_note("This correlation determines the formation volume factor for "
             "gas-saturated pure water, for pressures below or equal to bubble "
             "point pressure.");
    add_ref("mcCoy:1983");
  }

public:
  static inline double impl(const double& t, const double& p,
                            const double& nacl) noexcept;

  Quantity<RB_STB> call(const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
                        const Quantity<Dissolved_Salt_Percent>& nacl) const {
    return Quantity<RB_STB>(impl(t.raw(), p.raw(), nacl.raw()));
  }

  Quantity<RB_STB> validate_and_call(
      const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
      const Quantity<Dissolved_Salt_Percent>& nacl) const {

    return call(t, p, nacl);
  }

  Quantity<RB_STB> operator()(
      const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
      const Quantity<Dissolved_Salt_Percent>& nacl) const {
    return Quantity<RB_STB>(call(t, p, nacl));
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
    const auto& nacl = it.get_curr();

    return call(t, p, nacl);
  }
};

extern Quantity<RB_STB> BwbMcCoy__correlation__fct(
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<Dissolved_Salt_Percent>& nacl);
extern double BwbMcCoy__correlation__valid__fct(const double& t,
                                                const double& p,
                                                const double& nacl);

#endif // SATURATED_WATER_VOLUME_FACTOR_H
