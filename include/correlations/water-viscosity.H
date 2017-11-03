#ifndef WATER_VISCOSITY_H
#define WATER_VISCOSITY_H

#include <pvt-exceptions.H>
#include <correlations/correlation.H>

/** UwMaoDuan

    Generated at 2017-10-30 16:49:10 -0400

    Zen Technologies Group


The application range for P is: 1 - 1000 [bar]. P is used to calculate the
density of pure water, which was obtained from the IAPWS-97 international
standard equation of state.

The upper limit of the development range for the pressure is 350 bar and for the
temperature is 573 °K.

The authors indicated that the model can be extrapolated beyond the range of
data to predict the viscosity up to 1000 bar, because the viscosity of the
alkali-chloride solutions increases only very slightly with pressure. The valid
temperature range is from 273 °K to 623 °K.

References:

  - mao:2009

  - mcCain:2011

@param[in] t Temperature. Range in [273, 623]
@param[in] nacl Dissolved salt concentration. Range in [0, 6]
@param[in] ppw Pure water density. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class UwMaoDuan : public WaterViscosity {
  Correlation_Singleton(UwMaoDuan);

  UwMaoDuan() : WaterViscosity("UwMaoDuan", Paxs::get_instance()) {
    set_author("Mao & Duan");
    set_title("MAO & DUAN CORRELATION, CALCULATION OF WATER VISCOSITY");
    add_parameter("t", Kelvin::get_instance(), "Temperature", 273, 623);
    def_parameter_latex_symbol("t", "T");
    add_parameter("nacl", Molality_NaCl::get_instance(),
                  "Dissolved salt concentration", 0, 6);
    add_parameter("ppw", Gr_cm3::get_instance(), "Pure water density");
    def_parameter_latex_symbol("ppw", "\\rho_{pw}");
    add_db("Based on more than 2200 experimental data points of sodium "
           "chloride brines.");
    add_ref("mao:2009");
    add_ref("mcCain:2011");
  }

public:
  static inline double impl(const double& t, const double& nacl,
                            const double& ppw) noexcept;

  Quantity<Paxs> call(const Quantity<Kelvin>& t,
                      const Quantity<Molality_NaCl>& nacl,
                      const Quantity<Gr_cm3>& ppw) const {
    return Quantity<Paxs>(impl(t.raw(), nacl.raw(), ppw.raw()));
  }

  Quantity<Paxs> validate_and_call(const Quantity<Kelvin>& t,
                                   const Quantity<Molality_NaCl>& nacl,
                                   const Quantity<Gr_cm3>& ppw) const {
    if (not(nacl >= 0))
      ZENTHROW(OutOfParameterRange, "parameter nacl is less than 0");
    if (not(nacl <= 6))
      ZENTHROW(OutOfParameterRange, "parameter nacl is greater than 6");

    return call(t, nacl, ppw);
  }

  Quantity<Paxs> operator()(const Quantity<Kelvin>& t,
                            const Quantity<Molality_NaCl>& nacl,
                            const Quantity<Gr_cm3>& ppw) const {
    return Quantity<Paxs>(call(t, nacl, ppw));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& t = it.get_curr();
    it.next();
    const auto& nacl = it.get_curr();
    it.next();
    const auto& ppw = it.get_curr();

    return call(t, nacl, ppw);
  }
};

extern Quantity<Paxs> UwMaoDuan__correlation__fct(
    const Quantity<Kelvin>& t, const Quantity<Molality_NaCl>& nacl,
    const Quantity<Gr_cm3>& ppw);
extern double UwMaoDuan__correlation__valid__fct(const double& t,
                                                 const double& nacl,
                                                 const double& ppw);

/** UwVanWingen

    Generated at 2017-10-30 16:49:10 -0400

    Zen Technologies Group


The original reference is not available. The correlation was verified by using a
secondary reference: Bánzer (1996). Date: October 6 2016.

References:

  - vanWingen:1950

  - brill:1974

@param[in] t Temperature. Range in [0, 350]
@return inside [0.1, 2]
@ingroup Correlations
*/
class UwVanWingen : public WaterViscosity {
  Correlation_Singleton(UwVanWingen);

  UwVanWingen() : WaterViscosity("UwVanWingen", CP::get_instance(), 0.1, 2) {
    set_author("Van Wingen");
    set_title("VAN WINGEN CORRELATION, CALCULATION OF WATER VISCOSITY");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 0, 350);
    def_parameter_latex_symbol("t", "T");
    add_ref("vanWingen:1950");
    add_ref("brill:1974");
  }

public:
  static inline double impl(const double& t) noexcept;

  Quantity<CP> call(const Quantity<Fahrenheit>& t) const {
    return Quantity<CP>(impl(t.raw()));
  }

  Quantity<CP> validate_and_call(const Quantity<Fahrenheit>& t) const {

    return call(t);
  }

  Quantity<CP> operator()(const Quantity<Fahrenheit>& t) const {
    return Quantity<CP>(call(t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& t = it.get_curr();

    return call(t);
  }
};

extern Quantity<CP> UwVanWingen__correlation__fct(
    const Quantity<Fahrenheit>& t);
extern double UwVanWingen__correlation__valid__fct(const double& t);

/** UwMatthewsRussel

    Generated at 2017-10-30 16:49:10 -0400

    Zen Technologies Group


The original reference (Meehan, 1980) is not available. The correlation was
verified by using a secondary reference: Bánzer (1996). Date: October 6 2016.

Matthews & Russell (1967) presented the graphical correlation (from Chesnut,
unpublished, Shell Development Co.).

The ranges were taken from the graphical correlation.

References:

  - matthews:1967

  - meehan:1980

@param[in] t Temperature. Range in [40, 400]
@param[in] p Pressure. Range in [14.7, 10000]
@param[in] nacl Dissolved salt concentration. Range in [0, 26]
@return inside [0.1, 2.1]
@ingroup Correlations
*/
class UwMatthewsRussel : public WaterViscosity {
  Correlation_Singleton(UwMatthewsRussel);

  UwMatthewsRussel()
      : WaterViscosity("UwMatthewsRussel", CP::get_instance(), 0.1, 2.1) {
    set_author("Matthews & Russell");
    set_title("MATTHEWS & RUSSELL CORRELATION, CALCULATION OF WATER VISCOSITY");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 40, 400);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure", 14.7, 10000);
    def_parameter_latex_symbol("p", "P");
    add_parameter("nacl", Dissolved_Salt_Percent::get_instance(),
                  "Dissolved salt concentration", 0, 26);
    add_ref("matthews:1967");
    add_ref("meehan:1980");
  }

public:
  static inline double impl(const double& t, const double& p,
                            const double& nacl) noexcept;

  Quantity<CP> call(const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
                    const Quantity<Dissolved_Salt_Percent>& nacl) const {
    return Quantity<CP>(impl(t.raw(), p.raw(), nacl.raw()));
  }

  Quantity<CP> validate_and_call(
      const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
      const Quantity<Dissolved_Salt_Percent>& nacl) const {
    if (not(nacl >= 0))
      ZENTHROW(OutOfParameterRange, "parameter nacl is less than 0");
    if (not(nacl <= 26))
      ZENTHROW(OutOfParameterRange, "parameter nacl is greater than 26");

    return call(t, p, nacl);
  }

  Quantity<CP> operator()(const Quantity<Fahrenheit>& t,
                          const Quantity<psia>& p,
                          const Quantity<Dissolved_Salt_Percent>& nacl) const {
    return Quantity<CP>(call(t, p, nacl));
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

extern Quantity<CP> UwMatthewsRussel__correlation__fct(
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<Dissolved_Salt_Percent>& nacl);
extern double UwMatthewsRussel__correlation__valid__fct(const double& t,
                                                        const double& p,
                                                        const double& nacl);

/** UwMcCain

    Generated at 2017-10-30 16:49:10 -0400

    Zen Technologies Group


McCain expressed that the viscosity equation fits the graphical correlation
presented by Matthews & Russell (from Chesnut, unpublished, Shell Development
Co.) to within 5 % at temperatures between 100 and 400 °F and salinities to 26
%.

The equation for the viscosity correction fits data at 86.5 to 167 °F and
pressures below 10000 psia to within 4 %. At pressures between 10000 and 14000
psia, the fit is to within 7 %.

References:

  - mcCain:1990

@param[in] t Temperature. Range in [100, 400]
@param[in] p Pressure. Range in [14.7, 14000]
@param[in] nacl Dissolved salt concentration. Range in [0, 26]
@return inside [, ]
@ingroup Correlations
*/
class UwMcCain : public WaterViscosity {
  Correlation_Singleton(UwMcCain);

  UwMcCain() : WaterViscosity("UwMcCain", CP::get_instance()) {
    set_author("McCain");
    set_title("McCAIN CORRELATION, CALCULATION OF WATER VISCOSITY");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 100, 400);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure", 14.7, 14000);
    def_parameter_latex_symbol("p", "P");
    add_parameter("nacl", Dissolved_Salt_Percent::get_instance(),
                  "Dissolved salt concentration", 0, 26);
    add_ref("mcCain:1990");
  }

public:
  static inline double impl(const double& t, const double& p,
                            const double& nacl) noexcept;

  Quantity<CP> call(const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
                    const Quantity<Dissolved_Salt_Percent>& nacl) const {
    return Quantity<CP>(impl(t.raw(), p.raw(), nacl.raw()));
  }

  Quantity<CP> validate_and_call(
      const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
      const Quantity<Dissolved_Salt_Percent>& nacl) const {
    if (not(nacl >= 0))
      ZENTHROW(OutOfParameterRange, "parameter nacl is less than 0");
    if (not(nacl <= 26))
      ZENTHROW(OutOfParameterRange, "parameter nacl is greater than 26");

    return call(t, p, nacl);
  }

  Quantity<CP> operator()(const Quantity<Fahrenheit>& t,
                          const Quantity<psia>& p,
                          const Quantity<Dissolved_Salt_Percent>& nacl) const {
    return Quantity<CP>(call(t, p, nacl));
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

extern Quantity<CP> UwMcCain__correlation__fct(
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<Dissolved_Salt_Percent>& nacl);
extern double UwMcCain__correlation__valid__fct(const double& t,
                                                const double& p,
                                                const double& nacl);

/** UwMcCoy

    Generated at 2017-10-30 16:49:10 -0400

    Zen Technologies Group


The original reference is not available. The correlation was verified by using a
secondary reference: Bánzer (1996). Date: October 7 2016.

References:

  - mcCoy:1983

@param[in] t Temperature. Range in [, ]
@param[in] nacl Dissolved salt concentration. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class UwMcCoy : public WaterViscosity {
  Correlation_Singleton(UwMcCoy);

  UwMcCoy() : WaterViscosity("UwMcCoy", CP::get_instance()) {
    set_author("McCoy");
    set_title("McCOY CORRELATION, CALCULATION OF WATER VISCOSITY");
    add_parameter("t", Kelvin::get_instance(), "Temperature");
    def_parameter_latex_symbol("t", "T");
    add_parameter("nacl", Dissolved_Salt_Percent::get_instance(),
                  "Dissolved salt concentration");
    add_ref("mcCoy:1983");
  }

public:
  static inline double impl(const double& t, const double& nacl) noexcept;

  Quantity<CP> call(const Quantity<Kelvin>& t,
                    const Quantity<Dissolved_Salt_Percent>& nacl) const {
    return Quantity<CP>(impl(t.raw(), nacl.raw()));
  }

  Quantity<CP> validate_and_call(
      const Quantity<Kelvin>& t,
      const Quantity<Dissolved_Salt_Percent>& nacl) const {

    return call(t, nacl);
  }

  Quantity<CP> operator()(const Quantity<Kelvin>& t,
                          const Quantity<Dissolved_Salt_Percent>& nacl) const {
    return Quantity<CP>(call(t, nacl));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& t = it.get_curr();
    it.next();
    const auto& nacl = it.get_curr();

    return call(t, nacl);
  }
};

extern Quantity<CP> UwMcCoy__correlation__fct(
    const Quantity<Kelvin>& t, const Quantity<Dissolved_Salt_Percent>& nacl);
extern double UwMcCoy__correlation__valid__fct(const double& t,
                                               const double& nacl);

#endif // WATER_VISCOSITY_H
