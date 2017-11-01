#ifndef UNDERSATURATED_WATER_VOLUME_FACTOR_H
#define UNDERSATURATED_WATER_VOLUME_FACTOR_H

#include <pvt-exceptions.H>
#include <correlations/correlation.H>

/** BwaSpiveyMN

    Generated at 2017-10-30 16:49:10 -0400

    Zen Technologies Group

The formation volume factor is found by dividing the volume at reservoir
conditions by the volume at stock tank conditions.
This correlation provides an accurate and consistent estimate of formation
volume factor at pressures above the bubble point pressure.

The correlation was verified by using the original reference and McCain et al.
(2011) as a secondary reference. Date: October 18 2016.

References:

  - spivey:2004

  - mcCain:2011

@param[in] t Temperature. Range in [0, 275]
@param[in] p Pressure. Range in [0.1, 200]
@param[in] nacl Dissolved salt concentration. Range in [0, 5.7]
@return inside [, ]
@ingroup Correlations
*/
class BwaSpiveyMN : public UndersaturatedWaterVolumeFactor {
  Correlation_Singleton(BwaSpiveyMN);

  BwaSpiveyMN()
      : UndersaturatedWaterVolumeFactor("BwaSpiveyMN",
                                        Rm3_Sm3::get_instance()) {
    set_author("Spivey, McCain & North");
    set_title("SPIVEY, McCAIN & NORTH CORRELATION, CALCULATION OF WATER "
              "FORMATION VOLUME FACTOR ");
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
    add_note("The formation volume factor is found by dividing the volume at "
             "reservoir conditions by the volume at stock tank conditions.");
    add_note("This correlation provides an accurate and consistent estimate of "
             "formation volume factor at pressures above the bubble point "
             "pressure.");
    add_ref("spivey:2004");
    add_ref("mcCain:2011");
  }

  inline void precondition(const Quantity<Celsius>& t,
                           const Quantity<mPascal>& p) const;

public:
  static inline double impl(const double& t, const double& p,
                            const double& nacl) noexcept;

  Quantity<Rm3_Sm3> call(const Quantity<Celsius>& t, const Quantity<mPascal>& p,
                         const Quantity<Molality_NaCl>& nacl) const {
    precondition(t, p);
    return Quantity<Rm3_Sm3>(impl(t.raw(), p.raw(), nacl.raw()));
  }

  Quantity<Rm3_Sm3> validate_and_call(
      const Quantity<Celsius>& t, const Quantity<mPascal>& p,
      const Quantity<Molality_NaCl>& nacl) const {
    if (not(nacl >= 0))
      ZENTHROW(OutOfParameterRange, "parameter nacl is less than 0");
    if (not(nacl <= 5.7))
      ZENTHROW(OutOfParameterRange, "parameter nacl is greater than 5.7");

    return call(t, p, nacl);
  }

  Quantity<Rm3_Sm3> operator()(const Quantity<Celsius>& t,
                               const Quantity<mPascal>& p,
                               const Quantity<Molality_NaCl>& nacl) const {
    return Quantity<Rm3_Sm3>(call(t, p, nacl));
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

extern Quantity<Rm3_Sm3> BwaSpiveyMN__correlation__fct(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p,
    const Quantity<Molality_NaCl>& nacl);
extern double BwaSpiveyMN__correlation__valid__fct(const double& t,
                                                   const double& p,
                                                   const double& nacl);

/** BwaMcCain

    Generated at 2017-10-30 16:49:10 -0400

    Zen Technologies Group

This correlation determines the water formation volume factor for pressures
above the bubble point pressure.

The correlation was verified by using the original reference and Bánzer (1996)
as a secondary reference. Date: October 26 2016.

References:

  - mcCain:1990

@param[in] p Pressure. Range in [, ]
@param[in] pb Bubble point pressure. Range in [, ]
@param[in] bwbp Water formation volume factor at Pb. Range in [, ]
@param[in] cwa Undersaturated Water isothermal compressibility. Range in
[2.2e-06, 3.8e-06]
@return inside [, ]
@ingroup Correlations
*/
class BwaMcCain : public UndersaturatedWaterVolumeFactor {
  Correlation_Singleton(BwaMcCain);

  BwaMcCain()
      : UndersaturatedWaterVolumeFactor("BwaMcCain", RB_STB::get_instance()) {
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
    add_parameter("cwa", psia_1::get_instance(),
                  "Undersaturated Water isothermal compressibility", 2.2e-06,
                  3.8e-06);
    def_parameter_latex_symbol("cwa", "c_{wa}");
    add_db("Based on the equation presented by McCain (1990).");
    add_note("This correlation determines the water formation volume factor "
             "for pressures above the bubble point pressure.");
    add_ref("mcCain:1990");
  }

  inline void precondition(const Quantity<psia>& p,
                           const Quantity<psia>& pb) const;

public:
  static inline double impl(const double& p, const double& pb,
                            const double& bwbp, const double& cwa) noexcept;

  Quantity<RB_STB> call(const Quantity<psia>& p, const Quantity<psia>& pb,
                        const Quantity<RB_STB>& bwbp,
                        const Quantity<psia_1>& cwa) const {
    precondition(p, pb);
    return Quantity<RB_STB>(impl(p.raw(), pb.raw(), bwbp.raw(), cwa.raw()));
  }

  Quantity<RB_STB> validate_and_call(const Quantity<psia>& p,
                                     const Quantity<psia>& pb,
                                     const Quantity<RB_STB>& bwbp,
                                     const Quantity<psia_1>& cwa) const {
    if (not(cwa >= 2.2e-06))
      ZENTHROW(OutOfParameterRange, "parameter cwa is less than 2.2e-06");
    if (not(cwa <= 3.8e-06))
      ZENTHROW(OutOfParameterRange, "parameter cwa is greater than 3.8e-06");

    return call(p, pb, bwbp, cwa);
  }

  Quantity<RB_STB> operator()(const Quantity<psia>& p, const Quantity<psia>& pb,
                              const Quantity<RB_STB>& bwbp,
                              const Quantity<psia_1>& cwa) const {
    return Quantity<RB_STB>(call(p, pb, bwbp, cwa));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();
    it.next();
    const auto& bwbp = it.get_curr();
    it.next();
    const auto& cwa = it.get_curr();

    return call(p, pb, bwbp, cwa);
  }
};

extern Quantity<RB_STB> BwaMcCain__correlation__fct(
    const Quantity<psia>& p, const Quantity<psia>& pb,
    const Quantity<RB_STB>& bwbp, const Quantity<psia_1>& cwa);
extern double BwaMcCain__correlation__valid__fct(const double& p,
                                                 const double& pb,
                                                 const double& bwbp,
                                                 const double& cwa);

#endif // UNDERSATURATED_WATER_VOLUME_FACTOR_H
