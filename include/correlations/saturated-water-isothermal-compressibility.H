#ifndef SATURATED_WATER_ISOTHERMAL_COMPRESSIBILITY_H
#define SATURATED_WATER_ISOTHERMAL_COMPRESSIBILITY_H

#include <pvt-exceptions.H>
#include <correlations/correlation.H>

/** CwbSpiveyMN

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group

This correlation provides an accurate and consistent estimate of the coefficient
of isothermal compressibility at pressures below the bubble point.

The correlation was verified by using the original reference and McCain et al.
(2011) as a secondary reference. Date: October 19 2016.

References:

  - spivey:2004

  - mcCain:2011

@param[in] t Temperature. Range in [0, 275]
@param[in] p Pressure. Range in [0.1, 200]
@param[in] nacl Dissolved salt concentration. Range in [0, 5.7]
@param[in] z Gas compressibility factor. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class CwbSpiveyMN : public SaturatedWaterIsothermalCompressibility {
  Correlation_Singleton(CwbSpiveyMN);

  CwbSpiveyMN()
      : SaturatedWaterIsothermalCompressibility("CwbSpiveyMN",
                                                mPa_1::get_instance()) {
    set_author("Spivey, McCain & North");
    set_title("SPIVEY, McCAIN & NORTH, CALCULATION OF WATER ISOTHERMAL "
              "COMPRESSIBILITY (FOR PRESSURES BELOW THE BUBBLE POINT)");
    add_parameter("t", Celsius::get_instance(), "Temperature", 0, 275);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", mPascal::get_instance(), "Pressure", 0.1, 200);
    def_parameter_latex_symbol("p", "P");
    add_parameter("nacl", Molality_NaCl::get_instance(),
                  "Dissolved salt concentration", 0, 5.7);
    add_parameter("z", Zfactor::get_instance(), "Gas compressibility factor");
    def_parameter_latex_symbol("z", "Z\\ factor");
    add_db("The volumetric properties of oilfield brines were obtained by "
           "combining the correlations developed by Spivey, McCain & North "
           "(2004), for density of methane-free sodium chloride brine and "
           "solubility of methane in sodium chloride brines, with published "
           "correlations for specific volume and partial molar volume of "
           "methane in brine.");
    add_note("This correlation provides an accurate and consistent estimate of "
             "the coefficient of isothermal compressibility at pressures below "
             "the bubble point.");
    add_ref("spivey:2004");
    add_ref("mcCain:2011");
  }

  inline void precondition(const Quantity<Celsius>& t,
                           const Quantity<mPascal>& p) const;

public:
  static inline double impl(const double& t, const double& p,
                            const double& nacl, const double& z) noexcept;

  Quantity<mPa_1> call(const Quantity<Celsius>& t, const Quantity<mPascal>& p,
                       const Quantity<Molality_NaCl>& nacl,
                       const Quantity<Zfactor>& z) const {
    precondition(t, p);
    return Quantity<mPa_1>(impl(t.raw(), p.raw(), nacl.raw(), z.raw()));
  }

  Quantity<mPa_1> validate_and_call(const Quantity<Celsius>& t,
                                    const Quantity<mPascal>& p,
                                    const Quantity<Molality_NaCl>& nacl,
                                    const Quantity<Zfactor>& z) const {
    if (not(nacl >= 0))
      ZENTHROW(OutOfParameterRange, "parameter nacl is less than 0");
    if (not(nacl <= 5.7))
      ZENTHROW(OutOfParameterRange, "parameter nacl is greater than 5.7");

    return call(t, p, nacl, z);
  }

  Quantity<mPa_1> operator()(const Quantity<Celsius>& t,
                             const Quantity<mPascal>& p,
                             const Quantity<Molality_NaCl>& nacl,
                             const Quantity<Zfactor>& z) const {
    return Quantity<mPa_1>(call(t, p, nacl, z));
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
    it.next();
    const auto& z = it.get_curr();

    return call(t, p, nacl, z);
  }
};

extern Quantity<mPa_1> CwbSpiveyMN__correlation__fct(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p,
    const Quantity<Molality_NaCl>& nacl, const Quantity<Zfactor>& z);
extern double CwbSpiveyMN__correlation__valid__fct(const double& t,
                                                   const double& p,
                                                   const double& nacl,
                                                   const double& z);

/** CwbMcCain

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group

This correlation is for calculating the isothermal compressibility of water at
pressures below or equal to the bubble point.
The undersaturated water isothermal compressibility is estimated at P and T as
if pressure were above bubble point.
Ramey (1964) developed the graphical correlation for the derivative of the
solution gas-water ratio with respect to the pressure, and McCain (1990)
presented it in a mathematical form.

The development ranges for the temperature and pressure were taken from the
graph presented by Ramey (1964). The development range for the salt
concentration was taken from the graph suggested by McCain (1990).

The correlation was verified by using the original reference and Bánzer (1996)
as a secondary reference. Date: October 21 2016.

References:

  - mcCain:1990

  - ramey:1964

@param[in] t Temperature. Range in [100, 250]
@param[in] p Pressure. Range in [500, 5000]
@param[in] nacl Dissolved salt concentration. Range in [0, 30]
@param[in] bg Gas formation volume factor. Range in [, ]
@param[in] bw Water formation volume factor. Range in [, ]
@param[in] cwa Undersaturated water isothermal compressibility. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class CwbMcCain : public SaturatedWaterIsothermalCompressibility {
  Correlation_Singleton(CwbMcCain);

  CwbMcCain()
      : SaturatedWaterIsothermalCompressibility("CwbMcCain",
                                                psia_1::get_instance()) {
    set_author("McCain");
    set_title("McCAIN CORRELATION, CALCULATION OF WATER ISOTHERMAL "
              "COMPRESSIBILITY (FOR PRESSURES BELOW THE BUBBLE POINT)");
    set_hidden_wetgas_grid();
    set_hidden_drygas_grid();
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 100, 250);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure", 500, 5000);
    def_parameter_latex_symbol("p", "P");
    add_parameter("nacl", Dissolved_Salt_Percent::get_instance(),
                  "Dissolved salt concentration", 0, 30);
    add_parameter("bg", RB_SCF::get_instance(), "Gas formation volume factor");
    def_parameter_latex_symbol("bg", "B_{g}");
    add_parameter("bw", RB_STB::get_instance(),
                  "Water formation volume factor");
    def_parameter_latex_symbol("bw", "B_{w}");
    add_parameter("cwa", psia_1::get_instance(),
                  "Undersaturated water isothermal compressibility");
    def_parameter_latex_symbol("cwa", "c_{wa}");
    add_db("Based on differentiated gas-in-water solubility data. Ramey (1964) "
           "presented the pressure differential of gas in solution for a "
           "single natural gas containing 88.51 % methane and 6.02 % ethane.");
    add_note("This correlation is for calculating the isothermal "
             "compressibility of water at pressures below or equal to the "
             "bubble point.");
    add_note("The undersaturated water isothermal compressibility is estimated "
             "at P and T as if pressure were above bubble point.");
    add_note("Ramey (1964) developed the graphical correlation for the "
             "derivative of the solution gas-water ratio with respect to the "
             "pressure, and McCain (1990) presented it in a mathematical "
             "form.");
    add_ref("mcCain:1990");
    add_ref("ramey:1964");
  }

public:
  static inline double impl(const double& t, const double& p,
                            const double& nacl, const double& bg,
                            const double& bw, const double& cwa) noexcept;

  Quantity<psia_1> call(const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
                        const Quantity<Dissolved_Salt_Percent>& nacl,
                        const Quantity<RB_SCF>& bg, const Quantity<RB_STB>& bw,
                        const Quantity<psia_1>& cwa) const {
    return Quantity<psia_1>(
        impl(t.raw(), p.raw(), nacl.raw(), bg.raw(), bw.raw(), cwa.raw()));
  }

  Quantity<psia_1> validate_and_call(
      const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
      const Quantity<Dissolved_Salt_Percent>& nacl, const Quantity<RB_SCF>& bg,
      const Quantity<RB_STB>& bw, const Quantity<psia_1>& cwa) const {
    if (not(nacl >= 0))
      ZENTHROW(OutOfParameterRange, "parameter nacl is less than 0");
    if (not(nacl <= 30))
      ZENTHROW(OutOfParameterRange, "parameter nacl is greater than 30");

    return call(t, p, nacl, bg, bw, cwa);
  }

  Quantity<psia_1> operator()(const Quantity<Fahrenheit>& t,
                              const Quantity<psia>& p,
                              const Quantity<Dissolved_Salt_Percent>& nacl,
                              const Quantity<RB_SCF>& bg,
                              const Quantity<RB_STB>& bw,
                              const Quantity<psia_1>& cwa) const {
    return Quantity<psia_1>(call(t, p, nacl, bg, bw, cwa));
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
    it.next();
    const auto& bg = it.get_curr();
    it.next();
    const auto& bw = it.get_curr();
    it.next();
    const auto& cwa = it.get_curr();

    return call(t, p, nacl, bg, bw, cwa);
  }
};

extern Quantity<psia_1> CwbMcCain__correlation__fct(
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<Dissolved_Salt_Percent>& nacl, const Quantity<RB_SCF>& bg,
    const Quantity<RB_STB>& bw, const Quantity<psia_1>& cwa);
extern double CwbMcCain__correlation__valid__fct(
    const double& t, const double& p, const double& nacl, const double& bg,
    const double& bw, const double& cwa);

/** CwbMcCainOsif

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group

This correlation is for calculating the isothermal compressibility of water at
pressures below or equal to the bubble point.
McCain's correlation is evaluated with the undersaturated water isothermal
compressibility value obtained from the Osif correlation, which is estimated at
P and T as if pressure were above bubble point.
Ramey (1964) developed the graphical correlation for the derivative of the
solution gas-water ratio with respect to the pressure, and McCain (1990)
presented it in a mathematical form.

The development ranges for the temperature and pressure were taken from the
graph presented by Ramey (1964). The development range for the salt
concentration was taken from the graph suggested by McCain (1990).

The correlation was verified by using the original reference and Bánzer (1996)
as a secondary reference. Date: October 21 2016.

References:

  - mcCain:1990

  - ramey:1964

  - osif:1988

@param[in] t Temperature. Range in [100, 250]
@param[in] p Pressure. Range in [500, 5000]
@param[in] nacl Dissolved salt concentration. Range in [0, 30]
@param[in] bg Gas formation volume factor. Range in [, ]
@param[in] bw Water formation volume factor. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class CwbMcCainOsif : public SaturatedWaterIsothermalCompressibility {
  Correlation_Singleton(CwbMcCainOsif);

  CwbMcCainOsif()
      : SaturatedWaterIsothermalCompressibility("CwbMcCainOsif",
                                                psia_1::get_instance()) {
    set_author("McCain & Osif");
    set_title("McCAIN & OSIF CORRELATION, CALCULATION OF WATER ISOTHERMAL "
              "COMPRESSIBILITY (FOR PRESSURES BELOW THE BUBBLE POINT)");
    set_hidden();
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 100, 250);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure", 500, 5000);
    def_parameter_latex_symbol("p", "P");
    add_parameter("nacl", Dissolved_Salt_Percent::get_instance(),
                  "Dissolved salt concentration", 0, 30);
    add_parameter("bg", RB_SCF::get_instance(), "Gas formation volume factor");
    def_parameter_latex_symbol("bg", "B_{g}");
    add_parameter("bw", RB_STB::get_instance(),
                  "Water formation volume factor");
    def_parameter_latex_symbol("bw", "B_{w}");
    add_db("Based on differentiated gas-in-water solubility data. Ramey (1964) "
           "presented the pressure differential of gas in solution for a "
           "single natural gas containing 88.51 % methane and 6.02 % ethane.");
    add_note("This correlation is for calculating the isothermal "
             "compressibility of water at pressures below or equal to the "
             "bubble point.");
    add_note("McCain's correlation is evaluated with the undersaturated water "
             "isothermal compressibility value obtained from the Osif "
             "correlation, which is estimated at P and T as if pressure were "
             "above bubble point.");
    add_note("Ramey (1964) developed the graphical correlation for the "
             "derivative of the solution gas-water ratio with respect to the "
             "pressure, and McCain (1990) presented it in a mathematical "
             "form.");
    add_ref("mcCain:1990");
    add_ref("ramey:1964");
    add_ref("osif:1988");
  }

public:
  static inline double impl(const double& t, const double& p,
                            const double& nacl, const double& bg,
                            const double& bw) noexcept;

  Quantity<psia_1> call(const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
                        const Quantity<Dissolved_Salt_Percent>& nacl,
                        const Quantity<RB_SCF>& bg,
                        const Quantity<RB_STB>& bw) const {
    return Quantity<psia_1>(
        impl(t.raw(), p.raw(), nacl.raw(), bg.raw(), bw.raw()));
  }

  Quantity<psia_1> validate_and_call(
      const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
      const Quantity<Dissolved_Salt_Percent>& nacl, const Quantity<RB_SCF>& bg,
      const Quantity<RB_STB>& bw) const {
    if (not(nacl >= 0))
      ZENTHROW(OutOfParameterRange, "parameter nacl is less than 0");
    if (not(nacl <= 30))
      ZENTHROW(OutOfParameterRange, "parameter nacl is greater than 30");

    return call(t, p, nacl, bg, bw);
  }

  Quantity<psia_1> operator()(const Quantity<Fahrenheit>& t,
                              const Quantity<psia>& p,
                              const Quantity<Dissolved_Salt_Percent>& nacl,
                              const Quantity<RB_SCF>& bg,
                              const Quantity<RB_STB>& bw) const {
    return Quantity<psia_1>(call(t, p, nacl, bg, bw));
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
    it.next();
    const auto& bg = it.get_curr();
    it.next();
    const auto& bw = it.get_curr();

    return call(t, p, nacl, bg, bw);
  }
};

extern Quantity<psia_1> CwbMcCainOsif__correlation__fct(
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<Dissolved_Salt_Percent>& nacl, const Quantity<RB_SCF>& bg,
    const Quantity<RB_STB>& bw);
extern double CwbMcCainOsif__correlation__valid__fct(const double& t,
                                                     const double& p,
                                                     const double& nacl,
                                                     const double& bg,
                                                     const double& bw);

#endif // SATURATED_WATER_ISOTHERMAL_COMPRESSIBILITY_H
