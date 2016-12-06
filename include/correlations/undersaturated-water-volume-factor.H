#ifndef UNDERSATURATED_WATER_VOLUME_FACTOR_H
#define UNDERSATURATED_WATER_VOLUME_FACTOR_H

#include <correlations/correlation.H>

/** BwaSpiveyMN

The correlation was verified by using the original reference and McCain et al.
(2011) as a secondary reference. Date: October 18 2016.


*/
class BwaSpiveyMN : public UndersaturatedWaterVolumeFactor
{
  Correlation_Singleton(BwaSpiveyMN);

  BwaSpiveyMN()
    : UndersaturatedWaterVolumeFactor("BwaSpiveyMN", Sm3_Sm3::get_instance())
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
             "formation volume factor at pressures above the bubble point "
             "pressure.");
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

extern Quantity<Sm3_Sm3> BwaSpiveyMN__correlation__fct(
  const Quantity<Celsius>& t, const Quantity<mPascal>& p,
  const Quantity<Molality_NaCl>& nacl_concentration);
extern double BwaSpiveyMN__correlation__valid__fct(double t, double p,
                                                   double nacl_concentration);

/** BwaMcCain

The correlation was verified by using the original reference and Bánzer (1996)
as a secondary reference. Date: October 26 2016.


*/
class BwaMcCain : public UndersaturatedWaterVolumeFactor
{
  Correlation_Singleton(BwaMcCain);

  BwaMcCain()
    : UndersaturatedWaterVolumeFactor("BwaMcCain", RB_STB::get_instance())
  {
    set_author("McCain");
    set_title("McCain CORRELATION, CALCULATION OF WATER FORMATION VOLUME "
              "FACTOR (FOR PRESSURES ABOVE THE BUBBLE POINT)");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("pb", psia::get_instance(), "Bubble point pressure");
    def_parameter_latex_symbol("pb", "P_b");
    add_parameter("bwbp", RB_STB::get_instance(),
                  "Water formation volume factor at Pb");
    def_parameter_latex_symbol("bwbp", "B_{wbp}");
    add_parameter("cw", psia_1::get_instance(),
                  "Water isothermal compressibility");
    def_parameter_latex_symbol("cw", "c_{w}");
    add_db("Based on the equation presented by McCain (1990).");
    add_note("This correlation determines the water formation volume factor "
             "for pressures above the bubble point pressure.");
    add_ref("mcCain:1990");
  }

public:
  inline Quantity<RB_STB> impl(const Quantity<psia>& p,
                               const Quantity<psia>& pb,
                               const Quantity<RB_STB>& bwbp,
                               const Quantity<psia_1>& cw) const;

  Quantity<RB_STB> operator()(const Quantity<psia>& p, const Quantity<psia>& pb,
                              const Quantity<RB_STB>& bwbp,
                              const Quantity<psia_1>& cw) const
  {
    return impl(p, pb, bwbp, cw);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& p = it.get_curr();
      it.next();
      const auto& pb = it.get_curr();
      it.next();
      const auto& bwbp = it.get_curr();
      it.next();
      const auto& cw = it.get_curr();

      return impl(p, pb, bwbp, cw);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();
    it.next();
    const auto& bwbp = it.get_curr();
    it.next();
    const auto& cw = it.get_curr();

    return impl(p, pb, bwbp, cw);
  }
};

extern Quantity<RB_STB> BwaMcCain__correlation__fct(
  const Quantity<psia>& p, const Quantity<psia>& pb,
  const Quantity<RB_STB>& bwbp, const Quantity<psia_1>& cw);
extern double BwaMcCain__correlation__valid__fct(double p, double pb,
                                                 double bwbp, double cw);

#endif // UNDERSATURATED_WATER_VOLUME_FACTOR_H
