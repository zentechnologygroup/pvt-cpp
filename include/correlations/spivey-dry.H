#ifndef SPIVEY_DRY_H
#define SPIVEY_DRY_H

#include <pvt-exceptions.H>
#include <correlations/correlation.H>

/** PvSpiveyMN

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group


The correlation was verified by using the original reference and McCain et al.
(2011) as a secondary reference. Date: October 19 2016.

References:

  - spivey:2004

  - mcCain:2011

@param[in] t Temperature. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class PvSpiveyMN : public VaporPressure {
  Correlation_Singleton(PvSpiveyMN);

  PvSpiveyMN() : VaporPressure("PvSpiveyMN", mPascal::get_instance()) {
    set_author("Spivey, McCain & North");
    set_title("SPIVEY, McCAIN & NORTH, CALCULATION OF VAPOR PRESSURE");
    set_hidden_blackoil_grid();
    set_hidden_wetgas_grid();
    set_hidden_drygas_grid();
    add_parameter("t", Kelvin::get_instance(), "Temperature");
    def_parameter_latex_symbol("t", "T");
    add_db("Vapor pressure of pure water, calculated from the IAWPS-95 "
           "formulation.");
    add_ref("spivey:2004");
    add_ref("mcCain:2011");
  }

public:
  static inline double impl(const double& t) noexcept;

  Quantity<mPascal> call(const Quantity<Kelvin>& t) const {
    return Quantity<mPascal>(impl(t.raw()));
  }

  Quantity<mPascal> validate_and_call(const Quantity<Kelvin>& t) const {

    return call(t);
  }

  Quantity<mPascal> operator()(const Quantity<Kelvin>& t) const {
    return Quantity<mPascal>(call(t));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& t = it.get_curr();

    return call(t);
  }
};

extern Quantity<mPascal> PvSpiveyMN__correlation__fct(
    const Quantity<Kelvin>& t);
extern double PvSpiveyMN__correlation__valid__fct(const double& t);

/** MwSpiveyMN

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group


The correlation was verified by using the original reference and McCain et al.
(2011) as a secondary reference. Date: October 19 2016.

References:

  - spivey:2004

  - mcCain:2011

@param[in] t Temperature. Range in [, ]
@param[in] p Pressure. Range in [, ]
@param[in] pv Vapor pressure. Range in [, ]
@param[in] nacl Dissolved salt concentration. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class MwSpiveyMN : public TmpCorrelation {
  Correlation_Singleton(MwSpiveyMN);

  MwSpiveyMN() : TmpCorrelation("MwSpiveyMN", Molality_NaCl::get_instance()) {
    set_author("Spivey, McCain & North");
    set_title("SPIVEY, McCAIN & NORTH, CALCULATION OF SOLUBILITY OF METHANE IN "
              "BRINE");
    set_hidden();
    add_parameter("t", Celsius::get_instance(), "Temperature");
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", mPascal::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("pv", mPascal::get_instance(), "Vapor pressure");
    def_parameter_latex_symbol("pv", "P_{v}");
    add_parameter("nacl", Molality_NaCl::get_instance(),
                  "Dissolved salt concentration");
    add_db("Calculation of solubility of methane in brine.");
    add_ref("spivey:2004");
    add_ref("mcCain:2011");
  }

public:
  static inline double impl(const double& t, const double& p, const double& pv,
                            const double& nacl) noexcept;

  Quantity<Molality_NaCl> call(const Quantity<Celsius>& t,
                               const Quantity<mPascal>& p,
                               const Quantity<mPascal>& pv,
                               const Quantity<Molality_NaCl>& nacl) const {
    return Quantity<Molality_NaCl>(
        impl(t.raw(), p.raw(), pv.raw(), nacl.raw()));
  }

  Quantity<Molality_NaCl> validate_and_call(
      const Quantity<Celsius>& t, const Quantity<mPascal>& p,
      const Quantity<mPascal>& pv, const Quantity<Molality_NaCl>& nacl) const {

    return call(t, p, pv, nacl);
  }

  Quantity<Molality_NaCl> operator()(
      const Quantity<Celsius>& t, const Quantity<mPascal>& p,
      const Quantity<mPascal>& pv, const Quantity<Molality_NaCl>& nacl) const {
    return Quantity<Molality_NaCl>(call(t, p, pv, nacl));
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
    const auto& pv = it.get_curr();
    it.next();
    const auto& nacl = it.get_curr();

    return call(t, p, pv, nacl);
  }
};

extern Quantity<Molality_NaCl> MwSpiveyMN__correlation__fct(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p,
    const Quantity<mPascal>& pv, const Quantity<Molality_NaCl>& nacl);
extern double MwSpiveyMN__correlation__valid__fct(const double& t,
                                                  const double& p,
                                                  const double& pv,
                                                  const double& nacl);

/** CgfwSpiveyMN

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group


The correlation was verified by using the original reference and McCain et al.
(2011) as a secondary reference. Date: October 19 2016.

References:

  - spivey:2004

  - mcCain:2011

@param[in] t Temperature. Range in [, ]
@param[in] p Pressure. Range in [, ]
@param[in] nacl Dissolved salt concentration. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class CgfwSpiveyMN : public TmpCorrelation {
  Correlation_Singleton(CgfwSpiveyMN);

  CgfwSpiveyMN() : TmpCorrelation("CgfwSpiveyMN", mPa_1::get_instance()) {
    set_author("Spivey, McCain & North");
    set_title("SPIVEY, McCAIN & NORTH, CALCULATION OF COMPRESSIBILITY OF "
              "METHANE-FREE BRINE");
    set_hidden();
    add_parameter("t", Celsius::get_instance(), "Temperature");
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", mPascal::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("nacl", Molality_NaCl::get_instance(),
                  "Dissolved salt concentration");
    add_db("Calculation of compressibility of methane-free brine.");
    add_ref("spivey:2004");
    add_ref("mcCain:2011");
  }

public:
  static inline double impl(const double& t, const double& p,
                            const double& nacl) noexcept;

  Quantity<mPa_1> call(const Quantity<Celsius>& t, const Quantity<mPascal>& p,
                       const Quantity<Molality_NaCl>& nacl) const {
    return Quantity<mPa_1>(impl(t.raw(), p.raw(), nacl.raw()));
  }

  Quantity<mPa_1> validate_and_call(const Quantity<Celsius>& t,
                                    const Quantity<mPascal>& p,
                                    const Quantity<Molality_NaCl>& nacl) const {

    return call(t, p, nacl);
  }

  Quantity<mPa_1> operator()(const Quantity<Celsius>& t,
                             const Quantity<mPascal>& p,
                             const Quantity<Molality_NaCl>& nacl) const {
    return Quantity<mPa_1>(call(t, p, nacl));
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

extern Quantity<mPa_1> CgfwSpiveyMN__correlation__fct(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p,
    const Quantity<Molality_NaCl>& nacl);
extern double CgfwSpiveyMN__correlation__valid__fct(const double& t,
                                                    const double& p,
                                                    const double& nacl);

/** BwSpiveyMNDry

    Generated at 2017-10-30 16:49:08 -0400

    Zen Technologies Group

The formation volume factor is found by dividing the volume at reservoir
conditions by the volume at stock tank conditions.
This correlation provides an accurate and consistent estimate of formation
volume factor at pressures above and below the bubble point pressure.

The correlation was verified by using the original reference and McCain et al.
(2011) as a secondary reference. Date: October 18 2016.

References:

  - spivey:2004

  - mcCain:2011

@param[in] t Temperature. Range in [0, 275]
@param[in] p Pressure. Range in [0.1, 200]
@param[in] pv Vapor pressure. Range in [0, 200]
@param[in] nacl Dissolved salt concentration. Range in [0, 5.7]
@return inside [, ]
@ingroup Correlations
*/
class BwSpiveyMNDry : public TmpCorrelation {
  Correlation_Singleton(BwSpiveyMNDry);

  BwSpiveyMNDry() : TmpCorrelation("BwSpiveyMNDry", Rm3_Sm3::get_instance()) {
    set_author("Spivey, McCain & North");
    set_title("SPIVEY, McCAIN & NORTH CORRELATION, CALCULATION OF WATER "
              "FORMATION VOLUME FACTOR ");
    set_hidden();
    add_parameter("t", Celsius::get_instance(), "Temperature", 0, 275);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", mPascal::get_instance(), "Pressure", 0.1, 200);
    def_parameter_latex_symbol("p", "P");
    add_parameter("pv", mPascal::get_instance(), "Vapor pressure", 0, 200);
    def_parameter_latex_symbol("pv", "P_{v}");
    add_parameter("nacl", Molality_NaCl::get_instance(),
                  "Dissolved salt concentration", 0, 5.7);
    add_db("The volumetric properties of oilfield brines were obtained by "
           "combining the correlations developed by Spivey, McCain & North "
           "(2004), for density of methane-free sodium chloride brine and "
           "solubility of methane in sodium chloride brines, with published "
           "correlations for specific volume and partial molar volume of "
           "methane in brine.");
    add_note("The formation volume factor is found by dividing the volume at "
             "reservoir conditions by the volume at stock tank conditions.");
    add_note("This correlation provides an accurate and consistent estimate of "
             "formation volume factor at pressures above and below the bubble "
             "point pressure.");
    add_ref("spivey:2004");
    add_ref("mcCain:2011");
  }

  inline void precondition(const Quantity<mPascal>& p,
                           const Quantity<mPascal>& pv) const;

public:
  static inline double impl(const double& t, const double& p, const double& pv,
                            const double& nacl) noexcept;

  Quantity<Rm3_Sm3> call(const Quantity<Celsius>& t, const Quantity<mPascal>& p,
                         const Quantity<mPascal>& pv,
                         const Quantity<Molality_NaCl>& nacl) const {
    precondition(p, pv);
    return Quantity<Rm3_Sm3>(impl(t.raw(), p.raw(), pv.raw(), nacl.raw()));
  }

  Quantity<Rm3_Sm3> validate_and_call(
      const Quantity<Celsius>& t, const Quantity<mPascal>& p,
      const Quantity<mPascal>& pv, const Quantity<Molality_NaCl>& nacl) const {
    if (not(pv >= 0))
      ZENTHROW(OutOfParameterRange, "parameter pv is less than 0");
    if (not(pv <= 200))
      ZENTHROW(OutOfParameterRange, "parameter pv is greater than 200");
    if (not(nacl >= 0))
      ZENTHROW(OutOfParameterRange, "parameter nacl is less than 0");
    if (not(nacl <= 5.7))
      ZENTHROW(OutOfParameterRange, "parameter nacl is greater than 5.7");

    return call(t, p, pv, nacl);
  }

  Quantity<Rm3_Sm3> operator()(const Quantity<Celsius>& t,
                               const Quantity<mPascal>& p,
                               const Quantity<mPascal>& pv,
                               const Quantity<Molality_NaCl>& nacl) const {
    return Quantity<Rm3_Sm3>(call(t, p, pv, nacl));
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
    const auto& pv = it.get_curr();
    it.next();
    const auto& nacl = it.get_curr();

    return call(t, p, pv, nacl);
  }
};

extern Quantity<Rm3_Sm3> BwSpiveyMNDry__correlation__fct(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p,
    const Quantity<mPascal>& pv, const Quantity<Molality_NaCl>& nacl);
extern double BwSpiveyMNDry__correlation__valid__fct(const double& t,
                                                     const double& p,
                                                     const double& pv,
                                                     const double& nacl);

#endif // SPIVEY_DRY_H
