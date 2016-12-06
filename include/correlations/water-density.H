#ifndef WATER_DENSITY_H
#define WATER_DENSITY_H

#include <correlations/correlation.H>

/** PwSpiveyMN

The ranges were established from the intersection of the ranges of the
methane-free water density correlation and the methane solubility correlation.

The correlation was verified by using the original reference and McCain et al.
(2011) as a secondary reference. Date: October 12 2016.


*/
class PwSpiveyMN : public WaterDensity
{
  Correlation_Singleton(PwSpiveyMN);

  PwSpiveyMN()
    : WaterDensity("PwSpiveyMN", Gr_cm3::get_instance())
  {
    set_author("Spivey, McCain & North");
    set_title(
      "SPIVEY, McCAIN & NORTH CORRELATION, CALCULATION OF WATER DENSITY");
    add_parameter("t", Celsius::get_instance(), "Temperature", 0, 275);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", mPascal::get_instance(), "Pressure", 0.9, 200);
    def_parameter_latex_symbol("p", "P");
    add_parameter("nacl_concentration", Molality_NaCl::get_instance(),
                  "Dissolved salt concentration", 0, 5.7);
    def_parameter_latex_symbol("nacl_concentration", "C_{NaCl}");
    add_db("The brine density correlation was obtained by combining the "
           "correlations developed by Spivey, McCain & North (2004), for "
           "density of methane-free sodium chloride brine and solubility of "
           "methane in sodium chloride brines, with published correlations for "
           "specific volume and partial molar volume of methane in brine.");
    add_db("The methane solubility correlation is based on 1039 data points "
           "from a database of volumetric properties of sodium chloride "
           "brine.");
    add_note("The authors developed an empirical correlation for the "
             "solubility of methane in pure water, which is then corrected for "
             "salinity using a modification of Duan's method.");
    add_ref("spivey:2004");
    add_ref("mcCain:2011");
  }

public:
  inline Quantity<Gr_cm3> impl(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p,
    const Quantity<Molality_NaCl>& nacl_concentration) const;

  Quantity<Gr_cm3> operator()(
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

extern Quantity<Gr_cm3> PwSpiveyMN__correlation__fct(
  const Quantity<Celsius>& t, const Quantity<mPascal>& p,
  const Quantity<Molality_NaCl>& nacl_concentration);
extern double PwSpiveyMN__correlation__valid__fct(double t, double p,
                                                  double nacl_concentration);

/** PwSpiveyMNGasFree

The correlation was verified by using the original reference and McCain et al.
(2011) as a secondary reference. Date: October 12 2016.


*/
class PwSpiveyMNGasFree : public WaterDensity
{
  Correlation_Singleton(PwSpiveyMNGasFree);

  PwSpiveyMNGasFree()
    : WaterDensity("PwSpiveyMNGasFree", Gr_cm3::get_instance())
  {
    set_author("Spivey, McCain & North (Methane-Free)");
    set_title("SPIVEY, McCAIN & NORTH CORRELATION, CALCULATION OF METHANE-FREE "
              "WATER DENSITY");
    add_parameter("t", Celsius::get_instance(), "Temperature", 0, 275);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", mPascal::get_instance(), "Pressure", 0.1, 200);
    def_parameter_latex_symbol("p", "P");
    add_parameter("nacl_concentration", Molality_NaCl::get_instance(),
                  "Dissolved salt concentration", 0, 5.7);
    def_parameter_latex_symbol("nacl_concentration", "C_{NaCl}");
    add_db("Based on 1461 data points from a database of volumetric properties "
           "of methane-free sodium chloride brine.");
    add_note("Spivey, McCain & North (2004) used the IAPWS-95 international "
             "standard equation of state for water to determine the "
             "coefficients of the correlation for pure water density.");
    add_ref("spivey:2004");
    add_ref("mcCain:2011");
  }

public:
  inline Quantity<Gr_cm3> impl(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p,
    const Quantity<Molality_NaCl>& nacl_concentration) const;

  Quantity<Gr_cm3> operator()(
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

extern Quantity<Gr_cm3> PwSpiveyMNGasFree__correlation__fct(
  const Quantity<Celsius>& t, const Quantity<mPascal>& p,
  const Quantity<Molality_NaCl>& nacl_concentration);
extern double PwSpiveyMNGasFree__correlation__valid__fct(
  double t, double p, double nacl_concentration);

/** PwMcCain

The range for the dissolved salt concentration was taken from the graph
presented by McCain (1990).

The correlation was verified by using the original reference and Bánzer (1996)
as a secondary reference. Date: October 11 2016.


*/
class PwMcCain : public WaterDensity
{
  Correlation_Singleton(PwMcCain);

  PwMcCain()
    : WaterDensity("PwMcCain", Lb_ft3::get_instance())
  {
    set_author("McCain");
    set_title("McCAIN CORRELATION, CALCULATION OF WATER DENSITY");
    add_parameter("nacl_concentration", Dissolved_Salt_Percent::get_instance(),
                  "Dissolved salt concentration", 0, 26);
    def_parameter_latex_symbol("nacl_concentration", "C_{NaCl}");
    add_parameter("bw", RB_STB::get_instance(),
                  "Water formation volume factor");
    def_parameter_latex_symbol("bw", "B_{w}");
    add_db("The brine density is expressed as a function of the total "
           "percentage of the dissolved solids weight and the water formation "
           "volume factor.");
    add_ref("mcCain:1990");
  }

public:
  inline Quantity<Lb_ft3> impl(
    const Quantity<Dissolved_Salt_Percent>& nacl_concentration,
    const Quantity<RB_STB>& bw) const;

  Quantity<Lb_ft3> operator()(
    const Quantity<Dissolved_Salt_Percent>& nacl_concentration,
    const Quantity<RB_STB>& bw) const
  {
    return impl(nacl_concentration, bw);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& nacl_concentration = it.get_curr();
      it.next();
      const auto& bw = it.get_curr();

      return impl(nacl_concentration, bw);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& nacl_concentration = it.get_curr();
    it.next();
    const auto& bw = it.get_curr();

    return impl(nacl_concentration, bw);
  }
};

extern Quantity<Lb_ft3> PwMcCain__correlation__fct(
  const Quantity<Dissolved_Salt_Percent>& nacl_concentration,
  const Quantity<RB_STB>& bw);
extern double PwMcCain__correlation__valid__fct(double nacl_concentration,
                                                double bw);

/** PpwSpiveyMN

The correlation was verified by using the original reference.


*/
class PpwSpiveyMN : public PureWaterDensity
{
  Correlation_Singleton(PpwSpiveyMN);

  PpwSpiveyMN()
    : PureWaterDensity("PpwSpiveyMN", Gr_cm3::get_instance())
  {
    set_author("Spivey, McCain & North");
    set_title(
      "SPIVEY, McCAIN & NORTH CORRELATION, CALCULATION OF PURE WATER DENSITY");
    add_parameter("t", Celsius::get_instance(), "Temperature", 0, 275);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", mPascal::get_instance(), "Pressure", 0.1, 200);
    def_parameter_latex_symbol("p", "P");
    add_note("Spivey, McCain & North (2004) used the IAPWS-95 international "
             "standard equation of state for water to determine the "
             "coefficients of the correlation for pure water.");
    add_ref("spivey:2004");
  }

public:
  inline Quantity<Gr_cm3> impl(const Quantity<Celsius>& t,
                               const Quantity<mPascal>& p) const;

  Quantity<Gr_cm3> operator()(const Quantity<Celsius>& t,
                              const Quantity<mPascal>& p) const
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

extern Quantity<Gr_cm3> PpwSpiveyMN__correlation__fct(
  const Quantity<Celsius>& t, const Quantity<mPascal>& p);
extern double PpwSpiveyMN__correlation__valid__fct(double t, double p);

#endif // WATER_DENSITY_H
