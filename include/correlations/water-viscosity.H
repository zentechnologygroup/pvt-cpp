#ifndef WATER_VISCOSITY_H
#define WATER_VISCOSITY_H

#include <correlations/correlation.H>

/** UwMaoDuan

The application range for P is: 1 - 1000 [bar]. P is used to calculate the
density of pure water, which was obtained from the IAPWS-97 international
standard equation of state.

The upper limit of the development range for the pressure is 350 bar and for the
temperature is 573 °K.

The authors indicated that the model can be extrapolated beyond the range of
data to predict the viscosity up to 1000 bar, because the viscosity of the
alkali-chloride solutions increases only very slightly with pressure. The valid
temperature range is from 273 °K to 623 °K.


*/
class UwMaoDuan : public WaterViscosity
{
  Correlation_Singleton(UwMaoDuan);

  UwMaoDuan()
    : WaterViscosity("UwMaoDuan", Paxs::get_instance())
  {
    set_author("Mao & Duan");
    set_title("MAO & DUAN CORRELATION, CALCULATION OF WATER VISCOSITY");
    add_parameter("t", Kelvin::get_instance(), "Temperature", 273, 623);
    def_parameter_latex_symbol("t", "T");
    add_parameter("nacl_concentration", Molality_NaCl::get_instance(),
                  "Dissolved salt concentration", 0, 6);
    def_parameter_latex_symbol("nacl_concentration", "C_{NaCl}");
    add_parameter("ppw", Gr_cm3::get_instance(), "Pure water density");
    def_parameter_latex_symbol("ppw", "\\rho_{pw}");
    add_db("Based on more than 2200 experimental data points of sodium "
           "chloride brines.");
    add_ref("mao:2009");
    add_ref("mcCain:2011");
  }

public:
  inline Quantity<Paxs> impl(const Quantity<Kelvin>& t,
                             const Quantity<Molality_NaCl>& nacl_concentration,
                             const Quantity<Gr_cm3>& ppw) const;

  Quantity<Paxs> operator()(const Quantity<Kelvin>& t,
                            const Quantity<Molality_NaCl>& nacl_concentration,
                            const Quantity<Gr_cm3>& ppw) const
  {
    return impl(t, nacl_concentration, ppw);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& t = it.get_curr();
      it.next();
      const auto& nacl_concentration = it.get_curr();
      it.next();
      const auto& ppw = it.get_curr();

      return impl(t, nacl_concentration, ppw);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& t = it.get_curr();
    it.next();
    const auto& nacl_concentration = it.get_curr();
    it.next();
    const auto& ppw = it.get_curr();

    return impl(t, nacl_concentration, ppw);
  }
};

extern Quantity<Paxs> UwMaoDuan__correlation__fct(
  const Quantity<Kelvin>& t, const Quantity<Molality_NaCl>& nacl_concentration,
  const Quantity<Gr_cm3>& ppw);
extern double UwMaoDuan__correlation__valid__fct(double t,
                                                 double nacl_concentration,
                                                 double ppw);

/** UwVanWingen

The original reference is not available. The correlation was verified by using a
secondary reference: Bánzer (1996). Date: October 6 2016.


*/
class UwVanWingen : public WaterViscosity
{
  Correlation_Singleton(UwVanWingen);

  UwVanWingen()
    : WaterViscosity("UwVanWingen", CP::get_instance(), 0.1, 2)
  {
    set_author("Van Wingen");
    set_title("VAN WINGEN CORRELATION, CALCULATION OF WATER VISCOSITY");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 0, 350);
    def_parameter_latex_symbol("t", "T");
    add_ref("vanWingen:1950");
    add_ref("brill:1974");
  }

public:
  inline Quantity<CP> impl(const Quantity<Fahrenheit>& t) const;

  Quantity<CP> operator()(const Quantity<Fahrenheit>& t) const
  {
    return impl(t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& t = it.get_curr();

      return impl(t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& t = it.get_curr();

    return impl(t);
  }
};

extern Quantity<CP> UwVanWingen__correlation__fct(
  const Quantity<Fahrenheit>& t);
extern double UwVanWingen__correlation__valid__fct(double t);

/** UwMatthewsRussel

The original reference (Meehan, 1980) is not available. The correlation was
verified by using a secondary reference: Bánzer (1996). Date: October 6 2016.

Matthews & Russell (1967) presented the graphical correlation (from Chesnut,
unpublished, Shell Development Co.).

The ranges were taken from the graphical correlation.


*/
class UwMatthewsRussel : public WaterViscosity
{
  Correlation_Singleton(UwMatthewsRussel);

  UwMatthewsRussel()
    : WaterViscosity("UwMatthewsRussel", CP::get_instance(), 0.1, 2.1)
  {
    set_author("Matthews & Russell");
    set_title("MATTHEWS & RUSSELL CORRELATION, CALCULATION OF WATER VISCOSITY");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 40, 400);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure", 14.7, 10000);
    def_parameter_latex_symbol("p", "P");
    add_parameter("nacl_concentration", Dissolved_Salt_Percent::get_instance(),
                  "Dissolved salt concentration", 0, 26);
    def_parameter_latex_symbol("nacl_concentration", "C_{NaCl}");
    add_ref("matthews:1967");
    add_ref("meehan:1980");
  }

public:
  inline Quantity<CP> impl(
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<Dissolved_Salt_Percent>& nacl_concentration) const;

  Quantity<CP> operator()(
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<Dissolved_Salt_Percent>& nacl_concentration) const
  {
    return impl(t, p, nacl_concentration);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& t = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& nacl_concentration = it.get_curr();

      return impl(t, p, nacl_concentration);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& nacl_concentration = it.get_curr();

    return impl(t, p, nacl_concentration);
  }
};

extern Quantity<CP> UwMatthewsRussel__correlation__fct(
  const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
  const Quantity<Dissolved_Salt_Percent>& nacl_concentration);
extern double UwMatthewsRussel__correlation__valid__fct(
  double t, double p, double nacl_concentration);

/** UwMcCain

McCain expressed that the viscosity equation fits the graphical correlation
presented by Matthews & Russell (from Chesnut, unpublished, Shell Development
Co.) to within 5 % at temperatures between 100 and 400 °F and salinities to 26
%.

The equation for the viscosity correction fits data at 86.5 to 167 °F and
pressures below 10000 psia to within 4 %. At pressures between 10000 and 14000
psia, the fit is to within 7 %.


*/
class UwMcCain : public WaterViscosity
{
  Correlation_Singleton(UwMcCain);

  UwMcCain()
    : WaterViscosity("UwMcCain", CP::get_instance())
  {
    set_author("McCain");
    set_title("McCAIN CORRELATION, CALCULATION OF WATER VISCOSITY");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 100, 400);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure", 14.7, 14000);
    def_parameter_latex_symbol("p", "P");
    add_parameter("nacl_concentration", Dissolved_Salt_Percent::get_instance(),
                  "Dissolved salt concentration", 0, 26);
    def_parameter_latex_symbol("nacl_concentration", "C_{NaCl}");
    add_ref("mcCain:1990");
  }

public:
  inline Quantity<CP> impl(
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<Dissolved_Salt_Percent>& nacl_concentration) const;

  Quantity<CP> operator()(
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<Dissolved_Salt_Percent>& nacl_concentration) const
  {
    return impl(t, p, nacl_concentration);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& t = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& nacl_concentration = it.get_curr();

      return impl(t, p, nacl_concentration);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& nacl_concentration = it.get_curr();

    return impl(t, p, nacl_concentration);
  }
};

extern Quantity<CP> UwMcCain__correlation__fct(
  const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
  const Quantity<Dissolved_Salt_Percent>& nacl_concentration);
extern double UwMcCain__correlation__valid__fct(double t, double p,
                                                double nacl_concentration);

/** UwMcCoy

The original reference is not available. The correlation was verified by using a
secondary reference: Bánzer (1996). Date: October 7 2016.


*/
class UwMcCoy : public WaterViscosity
{
  Correlation_Singleton(UwMcCoy);

  UwMcCoy()
    : WaterViscosity("UwMcCoy", CP::get_instance())
  {
    set_author("McCoy");
    set_title("McCOY CORRELATION, CALCULATION OF WATER VISCOSITY");
    add_parameter("t", Kelvin::get_instance(), "Temperature");
    def_parameter_latex_symbol("t", "T");
    add_parameter("nacl_concentration", Dissolved_Salt_Percent::get_instance(),
                  "Dissolved salt concentration");
    def_parameter_latex_symbol("nacl_concentration", "C_{NaCl}");
    add_ref("mcCoy:1983");
  }

public:
  inline Quantity<CP> impl(
    const Quantity<Kelvin>& t,
    const Quantity<Dissolved_Salt_Percent>& nacl_concentration) const;

  Quantity<CP> operator()(
    const Quantity<Kelvin>& t,
    const Quantity<Dissolved_Salt_Percent>& nacl_concentration) const
  {
    return impl(t, nacl_concentration);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& t = it.get_curr();
      it.next();
      const auto& nacl_concentration = it.get_curr();

      return impl(t, nacl_concentration);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& t = it.get_curr();
    it.next();
    const auto& nacl_concentration = it.get_curr();

    return impl(t, nacl_concentration);
  }
};

extern Quantity<CP> UwMcCoy__correlation__fct(
  const Quantity<Kelvin>& t,
  const Quantity<Dissolved_Salt_Percent>& nacl_concentration);
extern double UwMcCoy__correlation__valid__fct(double t,
                                               double nacl_concentration);

#endif // WATER_VISCOSITY_H
