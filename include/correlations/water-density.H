#ifndef WATER_DENSITY_H
#define WATER_DENSITY_H

#include <pvt-exceptions.H>
#include <correlations/correlation.H>

/** PwSpiveyMN

    Generated at 2017-10-30 16:49:10 -0400

    Zen Technologies Group

The authors developed an empirical correlation for the solubility of methane in
pure water, which is then corrected for salinity using a modification of Duan's
method.

The ranges were established from the intersection of the ranges of the
methane-free water density correlation and the methane solubility correlation.

The correlation was verified by using the original reference and McCain et al.
(2011) as a secondary reference. Date: October 12 2016.

References:

  - spivey:2004

  - mcCain:2011

@param[in] t Temperature. Range in [0, 275]
@param[in] p Pressure. Range in [0.9, 200]
@param[in] nacl Dissolved salt concentration. Range in [0, 5.7]
@return inside [, ]
@ingroup Correlations
*/
class PwSpiveyMN : public WaterDensity {
  Correlation_Singleton(PwSpiveyMN);

  PwSpiveyMN() : WaterDensity("PwSpiveyMN", Gr_cm3::get_instance()) {
    set_author("Spivey, McCain & North");
    set_title(
        "SPIVEY, McCAIN & NORTH CORRELATION, CALCULATION OF WATER DENSITY");
    add_parameter("t", Celsius::get_instance(), "Temperature", 0, 275);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", mPascal::get_instance(), "Pressure", 0.9, 200);
    def_parameter_latex_symbol("p", "P");
    add_parameter("nacl", Molality_NaCl::get_instance(),
                  "Dissolved salt concentration", 0, 5.7);
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

  inline void precondition(const Quantity<Celsius>& t,
                           const Quantity<mPascal>& p) const;

public:
  static inline double impl(const double& t, const double& p,
                            const double& nacl) noexcept;

  Quantity<Gr_cm3> call(const Quantity<Celsius>& t, const Quantity<mPascal>& p,
                        const Quantity<Molality_NaCl>& nacl) const {
    precondition(t, p);
    return Quantity<Gr_cm3>(impl(t.raw(), p.raw(), nacl.raw()));
  }

  Quantity<Gr_cm3> validate_and_call(
      const Quantity<Celsius>& t, const Quantity<mPascal>& p,
      const Quantity<Molality_NaCl>& nacl) const {
    if (not(nacl >= 0))
      ZENTHROW(OutOfParameterRange, "parameter nacl is less than 0");
    if (not(nacl <= 5.7))
      ZENTHROW(OutOfParameterRange, "parameter nacl is greater than 5.7");

    return call(t, p, nacl);
  }

  Quantity<Gr_cm3> operator()(const Quantity<Celsius>& t,
                              const Quantity<mPascal>& p,
                              const Quantity<Molality_NaCl>& nacl) const {
    return Quantity<Gr_cm3>(call(t, p, nacl));
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

extern Quantity<Gr_cm3> PwSpiveyMN__correlation__fct(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p,
    const Quantity<Molality_NaCl>& nacl);
extern double PwSpiveyMN__correlation__valid__fct(const double& t,
                                                  const double& p,
                                                  const double& nacl);

/** PwSpiveyMNGasFree

    Generated at 2017-10-30 16:49:10 -0400

    Zen Technologies Group

Spivey, McCain & North (2004) used the IAPWS-95 international standard equation
of state for water to determine the coefficients of the correlation for pure
water density.

The correlation was verified by using the original reference and McCain et al.
(2011) as a secondary reference. Date: October 12 2016.

References:

  - spivey:2004

  - mcCain:2011

@param[in] t Temperature. Range in [0, 275]
@param[in] p Pressure. Range in [0.1, 200]
@param[in] nacl Dissolved salt concentration. Range in [0, 5.7]
@return inside [, ]
@ingroup Correlations
*/
class PwSpiveyMNGasFree : public WaterDensity {
  Correlation_Singleton(PwSpiveyMNGasFree);

  PwSpiveyMNGasFree()
      : WaterDensity("PwSpiveyMNGasFree", Gr_cm3::get_instance()) {
    set_author("Spivey, McCain & North (Methane-Free)");
    set_title("SPIVEY, McCAIN & NORTH CORRELATION, CALCULATION OF METHANE-FREE "
              "WATER DENSITY");
    set_hidden_blackoil_grid();
    set_hidden_wetgas_grid();
    set_hidden_drygas_grid();
    add_parameter("t", Celsius::get_instance(), "Temperature", 0, 275);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", mPascal::get_instance(), "Pressure", 0.1, 200);
    def_parameter_latex_symbol("p", "P");
    add_parameter("nacl", Molality_NaCl::get_instance(),
                  "Dissolved salt concentration", 0, 5.7);
    add_db("Based on 1461 data points from a database of volumetric properties "
           "of methane-free sodium chloride brine.");
    add_note("Spivey, McCain & North (2004) used the IAPWS-95 international "
             "standard equation of state for water to determine the "
             "coefficients of the correlation for pure water density.");
    add_ref("spivey:2004");
    add_ref("mcCain:2011");
  }

public:
  static inline double impl(const double& t, const double& p,
                            const double& nacl) noexcept;

  Quantity<Gr_cm3> call(const Quantity<Celsius>& t, const Quantity<mPascal>& p,
                        const Quantity<Molality_NaCl>& nacl) const {
    return Quantity<Gr_cm3>(impl(t.raw(), p.raw(), nacl.raw()));
  }

  Quantity<Gr_cm3> validate_and_call(
      const Quantity<Celsius>& t, const Quantity<mPascal>& p,
      const Quantity<Molality_NaCl>& nacl) const {
    if (not(nacl >= 0))
      ZENTHROW(OutOfParameterRange, "parameter nacl is less than 0");
    if (not(nacl <= 5.7))
      ZENTHROW(OutOfParameterRange, "parameter nacl is greater than 5.7");

    return call(t, p, nacl);
  }

  Quantity<Gr_cm3> operator()(const Quantity<Celsius>& t,
                              const Quantity<mPascal>& p,
                              const Quantity<Molality_NaCl>& nacl) const {
    return Quantity<Gr_cm3>(call(t, p, nacl));
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

extern Quantity<Gr_cm3> PwSpiveyMNGasFree__correlation__fct(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p,
    const Quantity<Molality_NaCl>& nacl);
extern double PwSpiveyMNGasFree__correlation__valid__fct(const double& t,
                                                         const double& p,
                                                         const double& nacl);

/** PwMcCain

    Generated at 2017-10-30 16:49:10 -0400

    Zen Technologies Group


The range for the dissolved salt concentration was taken from the graph
presented by McCain (1990).

The correlation was verified by using the original reference and Bánzer (1996)
as a secondary reference. Date: October 11 2016.

References:

  - mcCain:1990

@param[in] nacl Dissolved salt concentration. Range in [0, 26]
@param[in] bw Water formation volume factor. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class PwMcCain : public WaterDensity {
  Correlation_Singleton(PwMcCain);

  PwMcCain() : WaterDensity("PwMcCain", Lb_ft3::get_instance()) {
    set_author("McCain");
    set_title("McCAIN CORRELATION, CALCULATION OF WATER DENSITY");
    add_parameter("nacl", Dissolved_Salt_Percent::get_instance(),
                  "Dissolved salt concentration", 0, 26);
    add_parameter("bw", RB_STB::get_instance(),
                  "Water formation volume factor");
    def_parameter_latex_symbol("bw", "B_{w}");
    add_db("The brine density is expressed as a function of the total "
           "percentage of the dissolved solids weight and the water formation "
           "volume factor.");
    add_ref("mcCain:1990");
  }

public:
  static inline double impl(const double& nacl, const double& bw) noexcept;

  Quantity<Lb_ft3> call(const Quantity<Dissolved_Salt_Percent>& nacl,
                        const Quantity<RB_STB>& bw) const {
    return Quantity<Lb_ft3>(impl(nacl.raw(), bw.raw()));
  }

  Quantity<Lb_ft3> validate_and_call(
      const Quantity<Dissolved_Salt_Percent>& nacl,
      const Quantity<RB_STB>& bw) const {
    if (not(nacl >= 0))
      ZENTHROW(OutOfParameterRange, "parameter nacl is less than 0");
    if (not(nacl <= 26))
      ZENTHROW(OutOfParameterRange, "parameter nacl is greater than 26");

    return call(nacl, bw);
  }

  Quantity<Lb_ft3> operator()(const Quantity<Dissolved_Salt_Percent>& nacl,
                              const Quantity<RB_STB>& bw) const {
    return Quantity<Lb_ft3>(call(nacl, bw));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& nacl = it.get_curr();
    it.next();
    const auto& bw = it.get_curr();

    return call(nacl, bw);
  }
};

extern Quantity<Lb_ft3> PwMcCain__correlation__fct(
    const Quantity<Dissolved_Salt_Percent>& nacl, const Quantity<RB_STB>& bw);
extern double PwMcCain__correlation__valid__fct(const double& nacl,
                                                const double& bw);

/** PpwSpiveyMN

    Generated at 2017-10-30 16:49:10 -0400

    Zen Technologies Group

Spivey, McCain & North (2004) used the IAPWS-95 international standard equation
of state for water to determine the coefficients of the correlation for pure
water.

The correlation was verified by using the original reference.

References:

  - spivey:2004

@param[in] t Temperature. Range in [0, 275]
@param[in] p Pressure. Range in [0.1, 200]
@return inside [, ]
@ingroup Correlations
*/
class PpwSpiveyMN : public PureWaterDensity {
  Correlation_Singleton(PpwSpiveyMN);

  PpwSpiveyMN() : PureWaterDensity("PpwSpiveyMN", Gr_cm3::get_instance()) {
    set_author("Spivey, McCain & North");
    set_title("SPIVEY, McCAIN & NORTH CORRELATION, CALCULATION OF PURE WATER "
              "DENSITY");
    set_hidden_blackoil_grid();
    set_hidden_wetgas_grid();
    set_hidden_drygas_grid();
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
  static inline double impl(const double& t, const double& p) noexcept;

  Quantity<Gr_cm3> call(const Quantity<Celsius>& t,
                        const Quantity<mPascal>& p) const {
    return Quantity<Gr_cm3>(impl(t.raw(), p.raw()));
  }

  Quantity<Gr_cm3> validate_and_call(const Quantity<Celsius>& t,
                                     const Quantity<mPascal>& p) const {

    return call(t, p);
  }

  Quantity<Gr_cm3> operator()(const Quantity<Celsius>& t,
                              const Quantity<mPascal>& p) const {
    return Quantity<Gr_cm3>(call(t, p));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();

    return call(t, p);
  }
};

extern Quantity<Gr_cm3> PpwSpiveyMN__correlation__fct(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p);
extern double PpwSpiveyMN__correlation__valid__fct(const double& t,
                                                   const double& p);

#endif // WATER_DENSITY_H
