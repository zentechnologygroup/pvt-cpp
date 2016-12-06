#ifndef SPIVEY_DRY_H
#define SPIVEY_DRY_H

#include <correlations/correlation.H>

/** PvSpiveyMN

The correlation was verified by using the original reference and McCain et al.
(2011) as a secondary reference. Date: October 19 2016.


*/
class PvSpiveyMN : public TmpCorrelation
{
  Correlation_Singleton(PvSpiveyMN);

  PvSpiveyMN()
    : TmpCorrelation("PvSpiveyMN", mPascal::get_instance())
  {
    set_author("Spivey, McCain & North");
    set_title("SPIVEY, McCAIN & NORTH, CALCULATION OF VAPOR PRESSURE");
    set_hidden();
    add_parameter("t", Kelvin::get_instance(), "Temperature");
    def_parameter_latex_symbol("t", "T");
    add_db("Vapor pressure of pure water, calculated from the IAWPS-95 "
           "formulation.");
    add_ref("spivey:2004");
    add_ref("mcCain:2011");
  }

public:
  inline Quantity<mPascal> impl(const Quantity<Kelvin>& t) const;

  Quantity<mPascal> operator()(const Quantity<Kelvin>& t) const
  {
    return impl(t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& t = it.get_curr();

      return impl(t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& t = it.get_curr();

    return impl(t);
  }
};

extern Quantity<mPascal> PvSpiveyMN__correlation__fct(
  const Quantity<Kelvin>& t);
extern double PvSpiveyMN__correlation__valid__fct(double t);

/** MwSpiveyMN

The correlation was verified by using the original reference and McCain et al.
(2011) as a secondary reference. Date: October 19 2016.


*/
class MwSpiveyMN : public TmpCorrelation
{
  Correlation_Singleton(MwSpiveyMN);

  MwSpiveyMN()
    : TmpCorrelation("MwSpiveyMN", Molality_NaCl::get_instance())
  {
    set_author("Spivey, McCain & North");
    set_title(
      "SPIVEY, McCAIN & NORTH, CALCULATION OF SOLUBILITY OF METHANE IN BRINE");
    set_hidden();
    add_parameter("t", Celsius::get_instance(), "Temperature");
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", mPascal::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("pv", mPascal::get_instance(), "Vapor pressure");
    def_parameter_latex_symbol("pv", "P_{v}");
    add_parameter("nacl_concentration", Molality_NaCl::get_instance(),
                  "Dissolved salt concentration");
    def_parameter_latex_symbol("nacl_concentration", "C_{NaCl}");
    add_db("Calculation of solubility of methane in brine.");
    add_ref("spivey:2004");
    add_ref("mcCain:2011");
  }

public:
  inline Quantity<Molality_NaCl> impl(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p,
    const Quantity<mPascal>& pv,
    const Quantity<Molality_NaCl>& nacl_concentration) const;

  Quantity<Molality_NaCl> operator()(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p,
    const Quantity<mPascal>& pv,
    const Quantity<Molality_NaCl>& nacl_concentration) const
  {
    return impl(t, p, pv, nacl_concentration);
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
      const auto& pv = it.get_curr();
      it.next();
      const auto& nacl_concentration = it.get_curr();

      return impl(t, p, pv, nacl_concentration);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pv = it.get_curr();
    it.next();
    const auto& nacl_concentration = it.get_curr();

    return impl(t, p, pv, nacl_concentration);
  }
};

extern Quantity<Molality_NaCl> MwSpiveyMN__correlation__fct(
  const Quantity<Celsius>& t, const Quantity<mPascal>& p,
  const Quantity<mPascal>& pv,
  const Quantity<Molality_NaCl>& nacl_concentration);
extern double MwSpiveyMN__correlation__valid__fct(double t, double p, double pv,
                                                  double nacl_concentration);

/** CgfwSpiveyMN

The correlation was verified by using the original reference and McCain et al.
(2011) as a secondary reference. Date: October 19 2016.


*/
class CgfwSpiveyMN : public TmpCorrelation
{
  Correlation_Singleton(CgfwSpiveyMN);

  CgfwSpiveyMN()
    : TmpCorrelation("CgfwSpiveyMN", mPa_1::get_instance())
  {
    set_author("Spivey, McCain & North");
    set_title("SPIVEY, McCAIN & NORTH, CALCULATION OF COMPRESSIBILITY OF "
              "METHANE-FREE BRINE");
    set_hidden();
    add_parameter("t", Celsius::get_instance(), "Temperature");
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", mPascal::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("nacl_concentration", Molality_NaCl::get_instance(),
                  "Dissolved salt concentration");
    def_parameter_latex_symbol("nacl_concentration", "C_{NaCl}");
    add_db("Calculation of compressibility of methane-free brine.");
    add_ref("spivey:2004");
    add_ref("mcCain:2011");
  }

public:
  inline Quantity<mPa_1> impl(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p,
    const Quantity<Molality_NaCl>& nacl_concentration) const;

  Quantity<mPa_1> operator()(
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

extern Quantity<mPa_1> CgfwSpiveyMN__correlation__fct(
  const Quantity<Celsius>& t, const Quantity<mPascal>& p,
  const Quantity<Molality_NaCl>& nacl_concentration);
extern double CgfwSpiveyMN__correlation__valid__fct(double t, double p,
                                                    double nacl_concentration);

/** BwSpiveyMNDry

The correlation was verified by using the original reference and McCain et al.
(2011) as a secondary reference. Date: October 18 2016.


*/
class BwSpiveyMNDry : public TmpCorrelation
{
  Correlation_Singleton(BwSpiveyMNDry);

  BwSpiveyMNDry()
    : TmpCorrelation("BwSpiveyMNDry", Sm3_Sm3::get_instance())
  {
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
             "formation volume factor at pressures above and below the bubble "
             "point pressure.");
    add_ref("spivey:2004");
    add_ref("mcCain:2011");
  }

public:
  inline Quantity<Sm3_Sm3> impl(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p,
    const Quantity<mPascal>& pv,
    const Quantity<Molality_NaCl>& nacl_concentration) const;

  Quantity<Sm3_Sm3> operator()(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p,
    const Quantity<mPascal>& pv,
    const Quantity<Molality_NaCl>& nacl_concentration) const
  {
    return impl(t, p, pv, nacl_concentration);
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
      const auto& pv = it.get_curr();
      it.next();
      const auto& nacl_concentration = it.get_curr();

      return impl(t, p, pv, nacl_concentration);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pv = it.get_curr();
    it.next();
    const auto& nacl_concentration = it.get_curr();

    return impl(t, p, pv, nacl_concentration);
  }
};

extern Quantity<Sm3_Sm3> BwSpiveyMNDry__correlation__fct(
  const Quantity<Celsius>& t, const Quantity<mPascal>& p,
  const Quantity<mPascal>& pv,
  const Quantity<Molality_NaCl>& nacl_concentration);
extern double BwSpiveyMNDry__correlation__valid__fct(double t, double p,
                                                     double pv,
                                                     double nacl_concentration);

#endif // SPIVEY_DRY_H
