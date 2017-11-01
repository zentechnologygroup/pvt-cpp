#ifndef SOLUTION_GAS_WATER_RELATION_H
#define SOLUTION_GAS_WATER_RELATION_H

#include <pvt-exceptions.H>
#include <correlations/correlation.H>

/** RswSpiveyMN

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group

The solution gas-water ratio is obtained by dividing the volume of the gas at
standard conditions by the volume of brine at stock-tank conditions.

The correlation was verified by using the original reference and McCain et al.
(2011) as a secondary reference. Date: October 17 2016.

The original reference expresses the output unit in Scm3/Scm3

References:

  - spivey:2004

  - mcCain:2011

@param[in] t Temperature. Range in [0, 275]
@param[in] p Pressure. Range in [0.1, 200]
@param[in] nacl Dissolved salt concentration. Range in [0, 5.7]
@return inside [, ]
@ingroup Correlations
*/
class RswSpiveyMN : public SolutionGasWaterRelation {
  Correlation_Singleton(RswSpiveyMN);

  RswSpiveyMN()
      : SolutionGasWaterRelation("RswSpiveyMN", Sm3_Sm3::get_instance()) {
    set_author("Spivey, McCain & North");
    set_title("SPIVEY, McCAIN & NORTH CORRELATION, CALCULATION OF THE SOLUTION "
              "GAS-WATER RATIO");
    add_parameter("t", Celsius::get_instance(), "Temperature", 0, 275);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", mPascal::get_instance(), "Pressure", 0.1, 200);
    def_parameter_latex_symbol("p", "P");
    add_parameter("nacl", Molality_NaCl::get_instance(),
                  "Dissolved salt concentration", 0, 5.7);
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

  inline void precondition(const Quantity<Celsius>& t,
                           const Quantity<mPascal>& p) const;

public:
  static inline double impl(const double& t, const double& p,
                            const double& nacl) noexcept;

  Quantity<Sm3_Sm3> call(const Quantity<Celsius>& t, const Quantity<mPascal>& p,
                         const Quantity<Molality_NaCl>& nacl) const {
    precondition(t, p);
    return Quantity<Sm3_Sm3>(impl(t.raw(), p.raw(), nacl.raw()));
  }

  Quantity<Sm3_Sm3> validate_and_call(
      const Quantity<Celsius>& t, const Quantity<mPascal>& p,
      const Quantity<Molality_NaCl>& nacl) const {
    if (not(nacl >= 0))
      ZENTHROW(OutOfParameterRange, "parameter nacl is less than 0");
    if (not(nacl <= 5.7))
      ZENTHROW(OutOfParameterRange, "parameter nacl is greater than 5.7");

    return call(t, p, nacl);
  }

  Quantity<Sm3_Sm3> operator()(const Quantity<Celsius>& t,
                               const Quantity<mPascal>& p,
                               const Quantity<Molality_NaCl>& nacl) const {
    return Quantity<Sm3_Sm3>(call(t, p, nacl));
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

extern Quantity<Sm3_Sm3> RswSpiveyMN__correlation__fct(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p,
    const Quantity<Molality_NaCl>& nacl);
extern double RswSpiveyMN__correlation__valid__fct(const double& t,
                                                   const double& p,
                                                   const double& nacl);

/** RswCulbersonMcKetta

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group

Culberson & McKetta (1951) developed the graphical correlation and McCain (1990)
presented it in a mathematical form.

The data bank information was obtained from the original reference.

The equation was verified by using McCain (1990) and Bánzer (1996) as secondary
references. Date: October 17 2016.

References:

  - culberson:1951

  - mcCain:1990

@param[in] t Temperature. Range in [77, 340]
@param[in] p Pressure. Range in [1000, 10000]
@param[in] nacl Dissolved salt concentration. Range in [0, 30]
@return inside [, ]
@ingroup Correlations
*/
class RswCulbersonMcKetta : public SolutionGasWaterRelation {
  Correlation_Singleton(RswCulbersonMcKetta);

  RswCulbersonMcKetta()
      : SolutionGasWaterRelation("RswCulbersonMcKetta",
                                 SCF_STB::get_instance()) {
    set_author("Culberson & McKetta");
    set_title("CULBERSON & McKETTA CORRELATION, CALCULATION OF THE SOLUTION "
              "GAS-WATER RATIO");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 77, 340);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure", 1000, 10000);
    def_parameter_latex_symbol("p", "P");
    add_parameter("nacl", Dissolved_Salt_Percent::get_instance(),
                  "Dissolved salt concentration", 0, 30);
    add_db("Based on experimental and smoothed data for the solubility of "
           "methane in water for temperatures of 77, 100, 160, 220, 280, and "
           "340 °F at pressures to 10000 psia.");
    add_note("Culberson & McKetta (1951) developed the graphical correlation "
             "and McCain (1990) presented it in a mathematical form.");
    add_ref("culberson:1951");
    add_ref("mcCain:1990");
  }

public:
  static inline double impl(const double& t, const double& p,
                            const double& nacl) noexcept;

  Quantity<SCF_STB> call(const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
                         const Quantity<Dissolved_Salt_Percent>& nacl) const {
    return Quantity<SCF_STB>(impl(t.raw(), p.raw(), nacl.raw()));
  }

  Quantity<SCF_STB> validate_and_call(
      const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
      const Quantity<Dissolved_Salt_Percent>& nacl) const {
    if (not(nacl >= 0))
      ZENTHROW(OutOfParameterRange, "parameter nacl is less than 0");
    if (not(nacl <= 30))
      ZENTHROW(OutOfParameterRange, "parameter nacl is greater than 30");

    return call(t, p, nacl);
  }

  Quantity<SCF_STB> operator()(
      const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
      const Quantity<Dissolved_Salt_Percent>& nacl) const {
    return Quantity<SCF_STB>(call(t, p, nacl));
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

extern Quantity<SCF_STB> RswCulbersonMcKetta__correlation__fct(
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<Dissolved_Salt_Percent>& nacl);
extern double RswCulbersonMcKetta__correlation__valid__fct(const double& t,
                                                           const double& p,
                                                           const double& nacl);

/** RswMcCoy

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group


The original reference is not available. The correlation was verified by using
Bánzer (1996) as a secondary reference. Date: October 17 2016.

References:

  - mcCoy:1983

@param[in] t Temperature. Range in [, ]
@param[in] p Pressure. Range in [, ]
@param[in] nacl Dissolved salt concentration. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class RswMcCoy : public SolutionGasWaterRelation {
  Correlation_Singleton(RswMcCoy);

  RswMcCoy() : SolutionGasWaterRelation("RswMcCoy", SCF_STB::get_instance()) {
    set_author("McCoy");
    set_title("McCOY CORRELATION, CALCULATION OF THE SOLUTION GAS-WATER RATIO");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature");
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("nacl", Dissolved_Salt_Percent::get_instance(),
                  "Dissolved salt concentration");
    add_ref("mcCoy:1983");
  }

public:
  static inline double impl(const double& t, const double& p,
                            const double& nacl) noexcept;

  Quantity<SCF_STB> call(const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
                         const Quantity<Dissolved_Salt_Percent>& nacl) const {
    return Quantity<SCF_STB>(impl(t.raw(), p.raw(), nacl.raw()));
  }

  Quantity<SCF_STB> validate_and_call(
      const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
      const Quantity<Dissolved_Salt_Percent>& nacl) const {

    return call(t, p, nacl);
  }

  Quantity<SCF_STB> operator()(
      const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
      const Quantity<Dissolved_Salt_Percent>& nacl) const {
    return Quantity<SCF_STB>(call(t, p, nacl));
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

extern Quantity<SCF_STB> RswMcCoy__correlation__fct(
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<Dissolved_Salt_Percent>& nacl);
extern double RswMcCoy__correlation__valid__fct(const double& t,
                                                const double& p,
                                                const double& nacl);

#endif // SOLUTION_GAS_WATER_RELATION_H
