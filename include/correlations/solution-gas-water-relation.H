#ifndef SOLUTION_GAS_WATER_RELATION_H
#define SOLUTION_GAS_WATER_RELATION_H

#include <correlations/correlation.H>

/** RswSpiveyMN

The correlation was verified by using the original reference and McCain et al.
(2011) as a secondary reference. Date: October 17 2016.

The original reference expresses the output unit in Scm3/Scm3


*/
class RswSpiveyMN : public SolutionGasWaterRelation
{
  Correlation_Singleton(RswSpiveyMN);

  RswSpiveyMN()
    : SolutionGasWaterRelation("RswSpiveyMN", Sm3_Sm3::get_instance())
  {
    set_author("Spivey, McCain & North");
    set_title("SPIVEY, McCAIN & NORTH CORRELATION, CALCULATION OF THE SOLUTION "
              "GAS-WATER RATIO");
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
    add_note("The solution gas-water ratio is obtained by dividing the volume "
             "of the gas at standard conditions by the volume of brine at "
             "stock-tank conditions.");
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

extern Quantity<Sm3_Sm3> RswSpiveyMN__correlation__fct(
  const Quantity<Celsius>& t, const Quantity<mPascal>& p,
  const Quantity<Molality_NaCl>& nacl_concentration);
extern double RswSpiveyMN__correlation__valid__fct(double t, double p,
                                                   double nacl_concentration);

/** RswCulbersonMcKetta

The data bank information was obtained from the original reference.

The equation was verified by using McCain (1990) and Bánzer (1996) as secondary
references. Date: October 17 2016.


*/
class RswCulbersonMcKetta : public SolutionGasWaterRelation
{
  Correlation_Singleton(RswCulbersonMcKetta);

  RswCulbersonMcKetta()
    : SolutionGasWaterRelation("RswCulbersonMcKetta", SCF_STB::get_instance())
  {
    set_author("Culberson & McKetta");
    set_title("CULBERSON & McKETTA CORRELATION, CALCULATION OF THE SOLUTION "
              "GAS-WATER RATIO");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 77, 340);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure", 1000, 10000);
    def_parameter_latex_symbol("p", "P");
    add_parameter("nacl_concentration", Dissolved_Salt_Percent::get_instance(),
                  "Dissolved salt concentration", 0, 30);
    def_parameter_latex_symbol("nacl_concentration", "C_{NaCl}");
    add_db("Based on experimental and smoothed data for the solubility of "
           "methane in water for temperatures of 77, 100, 160, 220, 280, and "
           "340 °F at pressures to 10000 psia.");
    add_note("Culberson & McKetta (1951) developed the graphical correlation "
             "and McCain (1990) presented it in a mathematical form.");
    add_ref("culberson:1951");
    add_ref("mcCain:1990");
  }

public:
  inline Quantity<SCF_STB> impl(
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<Dissolved_Salt_Percent>& nacl_concentration) const;

  Quantity<SCF_STB> operator()(
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

extern Quantity<SCF_STB> RswCulbersonMcKetta__correlation__fct(
  const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
  const Quantity<Dissolved_Salt_Percent>& nacl_concentration);
extern double RswCulbersonMcKetta__correlation__valid__fct(
  double t, double p, double nacl_concentration);

/** RswMcCoy

The original reference is not available. The correlation was verified by using
Bánzer (1996) as a secondary reference. Date: October 17 2016.


*/
class RswMcCoy : public SolutionGasWaterRelation
{
  Correlation_Singleton(RswMcCoy);

  RswMcCoy()
    : SolutionGasWaterRelation("RswMcCoy", SCF_STB::get_instance())
  {
    set_author("McCoy");
    set_title("McCOY CORRELATION, CALCULATION OF THE SOLUTION GAS-WATER RATIO");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature");
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("nacl_concentration", Dissolved_Salt_Percent::get_instance(),
                  "Dissolved salt concentration");
    def_parameter_latex_symbol("nacl_concentration", "C_{NaCl}");
    add_ref("mcCoy:1983");
  }

public:
  inline Quantity<SCF_STB> impl(
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<Dissolved_Salt_Percent>& nacl_concentration) const;

  Quantity<SCF_STB> operator()(
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

extern Quantity<SCF_STB> RswMcCoy__correlation__fct(
  const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
  const Quantity<Dissolved_Salt_Percent>& nacl_concentration);
extern double RswMcCoy__correlation__valid__fct(double t, double p,
                                                double nacl_concentration);

#endif // SOLUTION_GAS_WATER_RELATION_H
