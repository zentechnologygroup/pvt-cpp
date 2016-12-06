#ifndef SATURATED_WATER_ISOTHERMAL_COMPRESSIBILITY_H
#define SATURATED_WATER_ISOTHERMAL_COMPRESSIBILITY_H

#include <correlations/correlation.H>

/** CwbSpiveyMN

The correlation was verified by using the original reference and McCain et al.
(2011) as a secondary reference. Date: October 19 2016.


*/
class CwbSpiveyMN : public SaturatedWaterIsothermalCompressibility
{
  Correlation_Singleton(CwbSpiveyMN);

  CwbSpiveyMN()
    : SaturatedWaterIsothermalCompressibility("CwbSpiveyMN",
                                              mPa_1::get_instance())
  {
    set_author("Spivey, McCain & North");
    set_title("SPIVEY, McCAIN & NORTH, CALCULATION OF WATER ISOTHERMAL "
              "COMPRESSIBILITY (FOR PRESSURES BELOW THE BUBBLE POINT)");
    add_parameter("t", Celsius::get_instance(), "Temperature", 0, 275);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", mPascal::get_instance(), "Pressure", 0.1, 200);
    def_parameter_latex_symbol("p", "P");
    add_parameter("nacl_concentration", Molality_NaCl::get_instance(),
                  "Dissolved salt concentration", 0, 5.7);
    def_parameter_latex_symbol("nacl_concentration", "C_{NaCl}");
    add_parameter("z", ZFactor::get_instance(), "Gas compressibility factor");
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

public:
  inline Quantity<mPa_1> impl(const Quantity<Celsius>& t,
                              const Quantity<mPascal>& p,
                              const Quantity<Molality_NaCl>& nacl_concentration,
                              const Quantity<ZFactor>& z) const;

  Quantity<mPa_1> operator()(const Quantity<Celsius>& t,
                             const Quantity<mPascal>& p,
                             const Quantity<Molality_NaCl>& nacl_concentration,
                             const Quantity<ZFactor>& z) const
  {
    return impl(t, p, nacl_concentration, z);
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
      const auto& z = it.get_curr();

      return impl(t, p, nacl_concentration, z);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& nacl_concentration = it.get_curr();
    it.next();
    const auto& z = it.get_curr();

    return impl(t, p, nacl_concentration, z);
  }
};

extern Quantity<mPa_1> CwbSpiveyMN__correlation__fct(
  const Quantity<Celsius>& t, const Quantity<mPascal>& p,
  const Quantity<Molality_NaCl>& nacl_concentration,
  const Quantity<ZFactor>& z);
extern double CwbSpiveyMN__correlation__valid__fct(double t, double p,
                                                   double nacl_concentration,
                                                   double z);

/** CwbMcCain

The development ranges for the temperature and pressure were taken from the
graph presented by Ramey (1964). The development range for the salt
concentration was taken from the graph suggested by McCain (1990).

The correlation was verified by using the original reference and Bánzer (1996)
as a secondary reference. Date: October 21 2016.


*/
class CwbMcCain : public SaturatedWaterIsothermalCompressibility
{
  Correlation_Singleton(CwbMcCain);

  CwbMcCain()
    : SaturatedWaterIsothermalCompressibility("CwbMcCain",
                                              psia_1::get_instance())
  {
    set_author("McCain");
    set_title("McCAIN CORRELATION, CALCULATION OF WATER ISOTHERMAL "
              "COMPRESSIBILITY (FOR PRESSURES BELOW THE BUBBLE POINT)");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 100, 250);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure", 500, 5000);
    def_parameter_latex_symbol("p", "P");
    add_parameter("nacl_concentration", Dissolved_Salt_Percent::get_instance(),
                  "Dissolved salt concentration", 0, 30);
    def_parameter_latex_symbol("nacl_concentration", "C_{NaCl}");
    add_parameter("bg", RB_SCF::get_instance(), "Gas formation volume factor");
    def_parameter_latex_symbol("bg", "B_{g}");
    add_parameter("bw", RB_STB::get_instance(),
                  "Water formation volume factor");
    def_parameter_latex_symbol("bw", "B_{w}");
    add_parameter("cwa", psia_1::get_instance(),
                  "Water isothermal compressibility above Pb");
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

extern Quantity<psia_1> CwbMcCain__correlation__fct(
  const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
  const Quantity<Dissolved_Salt_Percent>& nacl_concentration,
  const Quantity<RB_SCF>& bg, const Quantity<RB_STB>& bw,
  const Quantity<psia_1>& cwa);
extern double CwbMcCain__correlation__valid__fct(double t, double p,
                                                 double nacl_concentration,
                                                 double bg, double bw,
                                                 double cwa);

#endif // SATURATED_WATER_ISOTHERMAL_COMPRESSIBILITY_H
