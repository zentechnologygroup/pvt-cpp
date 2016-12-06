#ifndef SATURATED_WATER_VOLUME_FACTOR_H
#define SATURATED_WATER_VOLUME_FACTOR_H

#include <correlations/correlation.H>

/** BwbSpiveyMN

The correlation was verified by using the original reference and McCain et al.
(2011) as a secondary reference. Date: October 18 2016.


*/
class BwbSpiveyMN : public SaturatedWaterVolumeFactor
{
  Correlation_Singleton(BwbSpiveyMN);

  BwbSpiveyMN()
    : SaturatedWaterVolumeFactor("BwbSpiveyMN", Sm3_Sm3::get_instance())
  {
    set_author("Spivey, McCain & North");
    set_title("SPIVEY, McCAIN & NORTH CORRELATION, CALCULATION OF WATER "
              "FORMATION VOLUME FACTOR ");
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
    add_note("The formation volume factor is found by dividing the volume at "
             "reservoir conditions by the volume at stock tank conditions.");
    add_note("This correlation provides an accurate and consistent estimate of "
             "formation volume factor at pressures below or equal to the "
             "bubble point pressure.");
    add_ref("spivey:2004");
    add_ref("mcCain:2011");
  }

public:
  inline Quantity<Sm3_Sm3> impl(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p,
    const Quantity<Molality_NaCl>& nacl_concentration) const;

  Quantity<Sm3_Sm3> operator()(
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

extern Quantity<Sm3_Sm3> BwbSpiveyMN__correlation__fct(
  const Quantity<Celsius>& t, const Quantity<mPascal>& p,
  const Quantity<Molality_NaCl>& nacl_concentration);
extern double BwbSpiveyMN__correlation__valid__fct(double t, double p,
                                                   double nacl_concentration);

/** BwbMcCain

The correlation was verified by using the original reference and Bánzer (1996)
as a secondary reference. Date: October 18 2016.


*/
class BwbMcCain : public SaturatedWaterVolumeFactor
{
  Correlation_Singleton(BwbMcCain);

  BwbMcCain()
    : SaturatedWaterVolumeFactor("BwbMcCain", RB_STB::get_instance())
  {
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
  inline Quantity<RB_STB> impl(const Quantity<Fahrenheit>& t,
                               const Quantity<psia>& p) const;

  Quantity<RB_STB> operator()(const Quantity<Fahrenheit>& t,
                              const Quantity<psia>& p) const
  {
    return impl(t, p);
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

      return impl(t, p);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();

    return impl(t, p);
  }
};

extern Quantity<RB_STB> BwbMcCain__correlation__fct(
  const Quantity<Fahrenheit>& t, const Quantity<psia>& p);
extern double BwbMcCain__correlation__valid__fct(double t, double p);

/** BwbMcCoy

The original reference is not available. The correlation was verified by using
Bánzer (1996) as a secondary reference. Date: October 18 2016.


*/
class BwbMcCoy : public SaturatedWaterVolumeFactor
{
  Correlation_Singleton(BwbMcCoy);

  BwbMcCoy()
    : SaturatedWaterVolumeFactor("BwbMcCoy", RB_STB::get_instance())
  {
    set_author("McCoy");
    set_title("McCOY CORRELATION, CALCULATION OF WATER FORMATION VOLUME FACTOR "
              "(FOR PRESSURES BELOW OR EQUAL TO THE BUBBLE POINT)");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature");
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("nacl_concentration", Dissolved_Salt_Percent::get_instance(),
                  "Dissolved salt concentration");
    def_parameter_latex_symbol("nacl_concentration", "C_{NaCl}");
    add_note("This correlation determines the formation volume factor for "
             "gas-saturated pure water, for pressures below or equal to bubble "
             "point pressure.");
    add_ref("mcCoy:1983");
  }

public:
  inline Quantity<RB_STB> impl(
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<Dissolved_Salt_Percent>& nacl_concentration) const;

  Quantity<RB_STB> operator()(
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

extern Quantity<RB_STB> BwbMcCoy__correlation__fct(
  const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
  const Quantity<Dissolved_Salt_Percent>& nacl_concentration);
extern double BwbMcCoy__correlation__valid__fct(double t, double p,
                                                double nacl_concentration);

#endif // SATURATED_WATER_VOLUME_FACTOR_H
