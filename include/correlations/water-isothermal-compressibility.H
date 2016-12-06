#ifndef WATER_ISOTHERMAL_COMPRESSIBILITY_H
#define WATER_ISOTHERMAL_COMPRESSIBILITY_H

#include <correlations/correlation.H>

/** CwSpiveyMN

The correlation was verified by using the original reference and McCain et al.
(2011) as a secondary reference. Date: October 19 2016.


*/
class CwSpiveyMN : public WaterIsothermalCompressibility
{
  Correlation_Singleton(CwSpiveyMN);

  CwSpiveyMN()
    : WaterIsothermalCompressibility("CwSpiveyMN", mPa_1::get_instance())
  {
    set_author("Spivey, McCain & North");
    set_title("SPIVEY, McCAIN & NORTH, CALCULATION OF WATER ISOTHERMAL "
              "COMPRESSIBILITY");
    add_parameter("t", Celsius::get_instance(), "Temperature", 0, 275);
    add_parameter("p", mPascal::get_instance(), "Pressure", 0.1, 200);
    add_parameter("pb", mPascal::get_instance(), "Bubble point pressure");
    add_parameter("nacl_concentration", Molality_NaCl::get_instance(),
                  "Dissolved salt concentration", 0, 5.7);
    add_parameter("z", ZFactor::get_instance(), "Gas compressibility factor");
    add_db("The volumetric properties of oilfield brines were obtained by "
           "combining the correlations developed by Spivey et al., for density "
           "of methane-free sodium chloride brine and solubility of methane in "
           "sodium chloride brines, with published correlations for specific "
           "volume and partial molar volume of methane in brine.");
    add_note("This correlation provides an accurate and consistent estimate of "
             "the coefficient of isothermal compressibility at pressures below "
             "and above the bubble point pressure.");
    add_ref("spivey:2004");
    add_ref("mcCain:2011");
  }

public:
  inline Quantity<mPa_1> impl(const Quantity<Celsius>& t,
                              const Quantity<mPascal>& p,
                              const Quantity<mPascal>& pb,
                              const Quantity<Molality_NaCl>& nacl_concentration,
                              const Quantity<ZFactor>& z) const;

  Quantity<mPa_1> operator()(const Quantity<Celsius>& t,
                             const Quantity<mPascal>& p,
                             const Quantity<mPascal>& pb,
                             const Quantity<Molality_NaCl>& nacl_concentration,
                             const Quantity<ZFactor>& z) const
  {
    return impl(t, p, pb, nacl_concentration, z);
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
      const auto& pb = it.get_curr();
      it.next();
      const auto& nacl_concentration = it.get_curr();
      it.next();
      const auto& z = it.get_curr();

      return impl(t, p, pb, nacl_concentration, z);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();
    it.next();
    const auto& nacl_concentration = it.get_curr();
    it.next();
    const auto& z = it.get_curr();

    return impl(t, p, pb, nacl_concentration, z);
  }
};

extern Quantity<mPa_1> CwSpiveyMN__correlation__fct(
  const Quantity<Celsius>& t, const Quantity<mPascal>& p,
  const Quantity<mPascal>& pb,
  const Quantity<Molality_NaCl>& nacl_concentration,
  const Quantity<ZFactor>& z);
extern double CwSpiveyMN__correlation__valid__fct(double t, double p, double pb,
                                                  double nacl_concentration,
                                                  double z);

/** CwDodsonStanding

The original reference (Meehan, 1980) is not available. The correlation was
verified by using Bánzer (1996) as a secondary reference. Date: October 21 2016.


*/
class CwDodsonStanding : public WaterIsothermalCompressibility
{
  Correlation_Singleton(CwDodsonStanding);

  CwDodsonStanding()
    : WaterIsothermalCompressibility("CwDodsonStanding", psia_1::get_instance())
  {
    set_author("Dodson & Standing");
    set_title("DODSON & STANDING CORRELATION, CALCULATION OF WATER ISOTHERMAL "
              "COMPRESSIBILITY (FOR PRESSURES ABOVE THE BUBBLE POINT)");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 100, 250);
    add_parameter("p", psia::get_instance(), "Pressure", 500, 5000);
    add_parameter("rsw", SCF_STB::get_instance(), "Solution gas-water ratio");
    add_parameter("nacl_concentration", Dissolved_Salt_Percent::get_instance(),
                  "Dissolved salt concentration", 0.08, 3.5);
    add_db("Based on PVT and solubility data for mixtures of natural gas and "
           "water, and for mixtures of natural gas and two typical oil-field "
           "brines.");
    add_note("This correlation is for calculating the isothermal "
             "compressibility of water at pressures above the bubble point.");
    add_ref("dodson:1944");
    add_ref("banzer:1996");
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

extern Quantity<psia_1> CwDodsonStanding__correlation__fct(
  const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
  const Quantity<SCF_STB>& rsw,
  const Quantity<Dissolved_Salt_Percent>& nacl_concentration);
extern double CwDodsonStanding__correlation__valid__fct(
  double t, double p, double rsw, double nacl_concentration);

/** CwOsif

The correlation was verified by using the original reference and Bánzer (1996)
as a secondary reference. Date: October 20 2016.

A difference was found between the conversion of units suggested by Bánzer
(1996) and the conversion generally expressed in the petroleum unit converters.

Pure water density at oilfield standard conditions (60 °F and 14.7 psia):
http://www.engineeringtoolbox.com/water-properties-d_1258.html


*/
class CwOsif : public WaterIsothermalCompressibility
{
  Correlation_Singleton(CwOsif);

  CwOsif()
    : WaterIsothermalCompressibility("CwOsif", psia_1::get_instance())
  {
    set_author("Osif");
    set_title("OSIF CORRELATION, CALCULATION OF WATER ISOTHERMAL "
              "COMPRESSIBILITY (FOR PRESSURES ABOVE THE BUBBLE POINT)");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 200, 270);
    add_parameter("p", psia::get_instance(), "Pressure", 1000, 20000);
    add_parameter("nacl_concentration", CgL::get_instance(),
                  "Dissolved salt concentration", 0, 200);
    add_db("Based on 420 observations.");
    add_note("This correlation is for calculating the isothermal "
             "compressibility of water at pressures above the bubble point.");
    add_ref("osif:1988");
    add_ref("banzer:1996");
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

extern Quantity<psia_1> CwOsif__correlation__fct(
  const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
  const Quantity<CgL>& nacl_concentration);
extern double CwOsif__correlation__valid__fct(double t, double p,
                                              double nacl_concentration);

/** CwMcCain

The development ranges for the temperature and pressure were taken from the
graph presented by Ramey (1964). The development range for the salt
concentration was taken from the graph suggested by McCain (1990).

The correlation was verified by using the original reference and Bánzer (1996)
as a secondary reference. Date: October 21 2016.


*/
class CwMcCain : public WaterIsothermalCompressibility
{
  Correlation_Singleton(CwMcCain);

  CwMcCain()
    : WaterIsothermalCompressibility("CwMcCain", psia_1::get_instance())
  {
    set_author("Not defined");
    set_title("McCAIN CORRELATION, CALCULATION OF WATER ISOTHERMAL "
              "COMPRESSIBILITY (FOR PRESSURES BELOW THE BUBBLE POINT)");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 100, 250);
    add_parameter("p", psia::get_instance(), "Pressure", 500, 5000);
    add_parameter("nacl_concentration", Dissolved_Salt_Percent::get_instance(),
                  "Dissolved salt concentration", 0, 30);
    add_parameter("bg", RB_SCF::get_instance(), "Gas formation volume factor");
    add_parameter("bw", RB_STB::get_instance(),
                  "Water formation volume factor");
    add_parameter("cwa", psia_1::get_instance(),
                  "Water isothermal compressibility above Pb");
    add_db("Based on differentiated gas-in-water solubility data. Ramey (1964) "
           "presented the pressure differential of gas in solution for a "
           "single natural gas containing 88.51 % methane and 6.02 % ethane.");
    add_note("This correlation is for calculating the isothermal "
             "compressibility of water at pressures below or equal to the "
             "bubble point.");
    add_note("Ramey (1964) developed the graphical correlation for the "
             "derivative of the solution gas-water ratio with respect to the "
             "pressure, and McCain (1990) presented it in a mathematical "
             "form.");
    add_note(
      "Cwa is estimated at P and T as if pressure were above bubble point.");
  }

public:
  inline Quantity<psia_1> impl(
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<Dissolved_Salt_Percent>& nacl_concentration,
    const Quantity<RB_SCF>& bg, const Quantity<RB_STB>& bw,
    const Quantity<psia_1>& cwa) const;

  Quantity<psia_1> operator()(
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<Dissolved_Salt_Percent>& nacl_concentration,
    const Quantity<RB_SCF>& bg, const Quantity<RB_STB>& bw,
    const Quantity<psia_1>& cwa) const
  {
    return impl(t, p, nacl_concentration, bg, bw, cwa);
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
      it.next();
      const auto& bg = it.get_curr();
      it.next();
      const auto& bw = it.get_curr();
      it.next();
      const auto& cwa = it.get_curr();

      return impl(t, p, nacl_concentration, bg, bw, cwa);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& nacl_concentration = it.get_curr();
    it.next();
    const auto& bg = it.get_curr();
    it.next();
    const auto& bw = it.get_curr();
    it.next();
    const auto& cwa = it.get_curr();

    return impl(t, p, nacl_concentration, bg, bw, cwa);
  }
};

extern Quantity<psia_1> CwMcCain__correlation__fct(
  const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
  const Quantity<Dissolved_Salt_Percent>& nacl_concentration,
  const Quantity<RB_SCF>& bg, const Quantity<RB_STB>& bw,
  const Quantity<psia_1>& cwa);
extern double CwMcCain__correlation__valid__fct(double t, double p,
                                                double nacl_concentration,
                                                double bg, double bw,
                                                double cwa);

#endif // WATER_ISOTHERMAL_COMPRESSIBILITY_H
