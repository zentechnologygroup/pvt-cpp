#ifndef UNDERSATURATED_WATER_ISOTHERMAL_COMPRESSIBILITY_H
#define UNDERSATURATED_WATER_ISOTHERMAL_COMPRESSIBILITY_H

#include <correlations/correlation.H>

/** CwaSpiveyMN

The correlation was verified by using the original reference and McCain et al.
(2011) as a secondary reference. Date: October 19 2016.


*/
class CwaSpiveyMN : public UndersaturatedWaterIsothermalCompressibility
{
  Correlation_Singleton(CwaSpiveyMN);

  CwaSpiveyMN()
    : UndersaturatedWaterIsothermalCompressibility("CwaSpiveyMN",
                                                   mPa_1::get_instance())
  {
    set_author("Spivey, McCain & North");
    set_title("SPIVEY, McCAIN & NORTH, CALCULATION OF WATER ISOTHERMAL "
              "COMPRESSIBILITY (FOR PRESSURES ABOVE THE BUBBLE POINT)");
    add_parameter("t", Celsius::get_instance(), "Temperature", 0, 275);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", mPascal::get_instance(), "Pressure", 0.1, 200);
    def_parameter_latex_symbol("p", "P");
    add_parameter("nacl_concentration", Molality_NaCl::get_instance(),
                  "Dissolved salt concentration", 0, 5.7);
    def_parameter_latex_symbol("nacl_concentration", "C_{NaCl}");
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

public:
  inline Quantity<mPa_1> impl(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p,
    const Quantity<Molality_NaCl>& nacl_concentration) const;

  Quantity<mPa_1> operator()(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p,
    const Quantity<Molality_NaCl>& nacl_concentration) const
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

extern Quantity<mPa_1> CwaSpiveyMN__correlation__fct(
  const Quantity<Celsius>& t, const Quantity<mPascal>& p,
  const Quantity<Molality_NaCl>& nacl_concentration);
extern double CwaSpiveyMN__correlation__valid__fct(double t, double p,
                                                   double nacl_concentration);

/** CwaDodsonStanding

The original reference (Meehan, 1980) is not available. The correlation was
verified by using Bánzer (1996) as a secondary reference. Date: October 21 2016.


*/
class CwaDodsonStanding : public UndersaturatedWaterIsothermalCompressibility
{
  Correlation_Singleton(CwaDodsonStanding);

  CwaDodsonStanding()
    : UndersaturatedWaterIsothermalCompressibility("CwaDodsonStanding",
                                                   psia_1::get_instance())
  {
    set_author("Dodson & Standing");
    set_title("DODSON & STANDING CORRELATION, CALCULATION OF WATER ISOTHERMAL "
              "COMPRESSIBILITY (FOR PRESSURES ABOVE THE BUBBLE POINT)");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 100, 250);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure", 500, 5000);
    def_parameter_latex_symbol("p", "P");
    add_parameter("rsw", SCF_STB::get_instance(), "Solution gas-water ratio");
    def_parameter_latex_symbol("rsw", "R_{sw}");
    add_parameter("nacl_concentration", Dissolved_Salt_Percent::get_instance(),
                  "Dissolved salt concentration", 0.08, 3.5);
    def_parameter_latex_symbol("nacl_concentration", "C_{NaCl}");
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
  inline Quantity<psia_1> impl(
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<SCF_STB>& rsw,
    const Quantity<Dissolved_Salt_Percent>& nacl_concentration) const;

  Quantity<psia_1> operator()(
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<SCF_STB>& rsw,
    const Quantity<Dissolved_Salt_Percent>& nacl_concentration) const
  {
    return impl(t, p, rsw, nacl_concentration);
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
      const auto& rsw = it.get_curr();
      it.next();
      const auto& nacl_concentration = it.get_curr();

      return impl(t, p, rsw, nacl_concentration);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& rsw = it.get_curr();
    it.next();
    const auto& nacl_concentration = it.get_curr();

    return impl(t, p, rsw, nacl_concentration);
  }
};

extern Quantity<psia_1> CwaDodsonStanding__correlation__fct(
  const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
  const Quantity<SCF_STB>& rsw,
  const Quantity<Dissolved_Salt_Percent>& nacl_concentration);
extern double CwaDodsonStanding__correlation__valid__fct(
  double t, double p, double rsw, double nacl_concentration);

/** CwaOsif

The correlation was verified by using the original reference and Bánzer (1996)
as a secondary reference. Date: October 20 2016.

A difference was found between the conversion of units suggested by Bánzer
(1996) and the conversion generally expressed in the petroleum unit converters.

Pure water density at oilfield standard conditions (60 °F and 14.7 psia):
http://www.engineeringtoolbox.com/water-properties-d_1258.html


*/
class CwaOsif : public UndersaturatedWaterIsothermalCompressibility
{
  Correlation_Singleton(CwaOsif);

  CwaOsif()
    : UndersaturatedWaterIsothermalCompressibility("CwaOsif",
                                                   psia_1::get_instance())
  {
    set_author("Osif");
    set_title("OSIF CORRELATION, CALCULATION OF WATER ISOTHERMAL "
              "COMPRESSIBILITY (FOR PRESSURES ABOVE THE BUBBLE POINT)");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 200, 270);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure", 1000, 20000);
    def_parameter_latex_symbol("p", "P");
    add_parameter("nacl_concentration", CgL::get_instance(),
                  "Dissolved salt concentration", 0, 200);
    def_parameter_latex_symbol("nacl_concentration", "C_{NaCl}");
    add_db("Based on 420 observations.");
    add_note("This correlation is for calculating the isothermal "
             "compressibility of water at pressures above the bubble point.");
    add_ref("osif:1988");
  }

public:
  inline Quantity<psia_1> impl(const Quantity<Fahrenheit>& t,
                               const Quantity<psia>& p,
                               const Quantity<CgL>& nacl_concentration) const;

  Quantity<psia_1> operator()(const Quantity<Fahrenheit>& t,
                              const Quantity<psia>& p,
                              const Quantity<CgL>& nacl_concentration) const
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

extern Quantity<psia_1> CwaOsif__correlation__fct(
  const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
  const Quantity<CgL>& nacl_concentration);
extern double CwaOsif__correlation__valid__fct(double t, double p,
                                               double nacl_concentration);

#endif // UNDERSATURATED_WATER_ISOTHERMAL_COMPRESSIBILITY_H
