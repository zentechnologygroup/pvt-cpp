#ifndef UNDERSATURATED_WATER_ISOTHERMAL_COMPRESSIBILITY_H
#define UNDERSATURATED_WATER_ISOTHERMAL_COMPRESSIBILITY_H

#include <pvt-exceptions.H>
#include <correlations/correlation.H>

/** CwaSpiveyMN

    Generated at 2017-10-30 16:49:09 -0400

    Zen Technologies Group

This correlation provides an accurate and consistent estimate of the coefficient
of isothermal compressibility at pressures above the bubble point.

The correlation was verified by using the original reference and McCain et al.
(2011) as a secondary reference. Date: October 19 2016.

References:

  - spivey:2004

  - mcCain:2011

@param[in] t Temperature. Range in [0, 275]
@param[in] p Pressure. Range in [0.1, 200]
@param[in] nacl Dissolved salt concentration. Range in [0, 5.7]
@return inside [, ]
@ingroup Correlations
*/
class CwaSpiveyMN : public UndersaturatedWaterIsothermalCompressibility {
  Correlation_Singleton(CwaSpiveyMN);

  CwaSpiveyMN()
      : UndersaturatedWaterIsothermalCompressibility("CwaSpiveyMN",
                                                     mPa_1::get_instance()) {
    set_author("Spivey, McCain & North");
    set_title("SPIVEY, McCAIN & NORTH, CALCULATION OF WATER ISOTHERMAL "
              "COMPRESSIBILITY (FOR PRESSURES ABOVE THE BUBBLE POINT)");
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
    add_note("This correlation provides an accurate and consistent estimate of "
             "the coefficient of isothermal compressibility at pressures above "
             "the bubble point.");
    add_ref("spivey:2004");
    add_ref("mcCain:2011");
  }

  inline void precondition(const Quantity<Celsius>& t,
                           const Quantity<mPascal>& p) const;

public:
  static inline double impl(const double& t, const double& p,
                            const double& nacl) noexcept;

  Quantity<mPa_1> call(const Quantity<Celsius>& t, const Quantity<mPascal>& p,
                       const Quantity<Molality_NaCl>& nacl) const {
    precondition(t, p);
    return Quantity<mPa_1>(impl(t.raw(), p.raw(), nacl.raw()));
  }

  Quantity<mPa_1> validate_and_call(const Quantity<Celsius>& t,
                                    const Quantity<mPascal>& p,
                                    const Quantity<Molality_NaCl>& nacl) const {
    if (not(nacl >= 0))
      ZENTHROW(OutOfParameterRange, "parameter nacl is less than 0");
    if (not(nacl <= 5.7))
      ZENTHROW(OutOfParameterRange, "parameter nacl is greater than 5.7");

    return call(t, p, nacl);
  }

  Quantity<mPa_1> operator()(const Quantity<Celsius>& t,
                             const Quantity<mPascal>& p,
                             const Quantity<Molality_NaCl>& nacl) const {
    return Quantity<mPa_1>(call(t, p, nacl));
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

extern Quantity<mPa_1> CwaSpiveyMN__correlation__fct(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p,
    const Quantity<Molality_NaCl>& nacl);
extern double CwaSpiveyMN__correlation__valid__fct(const double& t,
                                                   const double& p,
                                                   const double& nacl);

/** CwaDodsonStanding

    Generated at 2017-10-30 16:49:09 -0400

    Zen Technologies Group

This correlation is for calculating the isothermal compressibility of water at
pressures above the bubble point.

The original reference (Meehan, 1980) is not available. The correlation was
verified by using Bánzer (1996) as a secondary reference. Date: October 21 2016.

References:

  - dodson:1944

  - meehan:1980

  - numbere:1977

@param[in] t Temperature. Range in [100, 250]
@param[in] p Pressure. Range in [500, 5000]
@param[in] rsw Solution gas-water ratio. Range in [, ]
@param[in] nacl Dissolved salt concentration. Range in [0.08, 3.5]
@return inside [, ]
@ingroup Correlations
*/
class CwaDodsonStanding : public UndersaturatedWaterIsothermalCompressibility {
  Correlation_Singleton(CwaDodsonStanding);

  CwaDodsonStanding()
      : UndersaturatedWaterIsothermalCompressibility("CwaDodsonStanding",
                                                     psia_1::get_instance()) {
    set_author("Dodson & Standing");
    set_title("DODSON & STANDING CORRELATION, CALCULATION OF WATER ISOTHERMAL "
              "COMPRESSIBILITY (FOR PRESSURES ABOVE THE BUBBLE POINT)");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 100, 250);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure", 500, 5000);
    def_parameter_latex_symbol("p", "P");
    add_parameter("rsw", SCF_STB::get_instance(), "Solution gas-water ratio");
    def_parameter_latex_symbol("rsw", "R_{sw}");
    add_parameter("nacl", Dissolved_Salt_Percent::get_instance(),
                  "Dissolved salt concentration", 0.08, 3.5);
    add_db("Based on PVT and solubility data for mixtures of natural gas and "
           "water, and for mixtures of natural gas and two typical oil-field "
           "brines.");
    add_note("This correlation is for calculating the isothermal "
             "compressibility of water at pressures above the bubble point.");
    add_ref("dodson:1944");
    add_ref("meehan:1980");
    add_ref("numbere:1977");
  }

public:
  static inline double impl(const double& t, const double& p, const double& rsw,
                            const double& nacl) noexcept;

  Quantity<psia_1> call(const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
                        const Quantity<SCF_STB>& rsw,
                        const Quantity<Dissolved_Salt_Percent>& nacl) const {
    return Quantity<psia_1>(impl(t.raw(), p.raw(), rsw.raw(), nacl.raw()));
  }

  Quantity<psia_1> validate_and_call(
      const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
      const Quantity<SCF_STB>& rsw,
      const Quantity<Dissolved_Salt_Percent>& nacl) const {
    if (not(nacl >= 0.08))
      ZENTHROW(OutOfParameterRange, "parameter nacl is less than 0.08");
    if (not(nacl <= 3.5))
      ZENTHROW(OutOfParameterRange, "parameter nacl is greater than 3.5");

    return call(t, p, rsw, nacl);
  }

  Quantity<psia_1> operator()(
      const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
      const Quantity<SCF_STB>& rsw,
      const Quantity<Dissolved_Salt_Percent>& nacl) const {
    return Quantity<psia_1>(call(t, p, rsw, nacl));
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
    const auto& rsw = it.get_curr();
    it.next();
    const auto& nacl = it.get_curr();

    return call(t, p, rsw, nacl);
  }
};

extern Quantity<psia_1> CwaDodsonStanding__correlation__fct(
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<SCF_STB>& rsw, const Quantity<Dissolved_Salt_Percent>& nacl);
extern double CwaDodsonStanding__correlation__valid__fct(const double& t,
                                                         const double& p,
                                                         const double& rsw,
                                                         const double& nacl);

/** CwaOsif

    Generated at 2017-10-30 16:49:09 -0400

    Zen Technologies Group

This correlation is for calculating the isothermal compressibility of water at
pressures above the bubble point.

The correlation was verified by using the original reference and Bánzer (1996)
as a secondary reference. Date: October 20 2016.

A difference was found between the conversion of units suggested by Bánzer
(1996) and the conversion generally expressed in the petroleum unit converters.

Pure water density at oilfield standard conditions (60 °F and 14.7 psia):
http://www.engineeringtoolbox.com/water-properties-d_1258.html

References:

  - osif:1988

@param[in] t Temperature. Range in [200, 270]
@param[in] p Pressure. Range in [1000, 20000]
@param[in] nacl Dissolved salt concentration. Range in [0, 200]
@return inside [, ]
@ingroup Correlations
*/
class CwaOsif : public UndersaturatedWaterIsothermalCompressibility {
  Correlation_Singleton(CwaOsif);

  CwaOsif()
      : UndersaturatedWaterIsothermalCompressibility("CwaOsif",
                                                     psia_1::get_instance()) {
    set_author("Osif");
    set_title("OSIF CORRELATION, CALCULATION OF WATER ISOTHERMAL "
              "COMPRESSIBILITY (FOR PRESSURES ABOVE THE BUBBLE POINT)");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 200, 270);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure", 1000, 20000);
    def_parameter_latex_symbol("p", "P");
    add_parameter("nacl", CgL::get_instance(), "Dissolved salt concentration",
                  0, 200);
    add_db("Based on 420 observations.");
    add_note("This correlation is for calculating the isothermal "
             "compressibility of water at pressures above the bubble point.");
    add_ref("osif:1988");
  }

public:
  static inline double impl(const double& t, const double& p,
                            const double& nacl) noexcept;

  Quantity<psia_1> call(const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
                        const Quantity<CgL>& nacl) const {
    return Quantity<psia_1>(impl(t.raw(), p.raw(), nacl.raw()));
  }

  Quantity<psia_1> validate_and_call(const Quantity<Fahrenheit>& t,
                                     const Quantity<psia>& p,
                                     const Quantity<CgL>& nacl) const {
    if (not(nacl >= 0))
      ZENTHROW(OutOfParameterRange, "parameter nacl is less than 0");
    if (not(nacl <= 200))
      ZENTHROW(OutOfParameterRange, "parameter nacl is greater than 200");

    return call(t, p, nacl);
  }

  Quantity<psia_1> operator()(const Quantity<Fahrenheit>& t,
                              const Quantity<psia>& p,
                              const Quantity<CgL>& nacl) const {
    return Quantity<psia_1>(call(t, p, nacl));
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

extern Quantity<psia_1> CwaOsif__correlation__fct(const Quantity<Fahrenheit>& t,
                                                  const Quantity<psia>& p,
                                                  const Quantity<CgL>& nacl);
extern double CwaOsif__correlation__valid__fct(const double& t, const double& p,
                                               const double& nacl);

#endif // UNDERSATURATED_WATER_ISOTHERMAL_COMPRESSIBILITY_H
