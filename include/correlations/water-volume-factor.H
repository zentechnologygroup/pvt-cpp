#ifndef WATER_VOLUME_FACTOR_H
#define WATER_VOLUME_FACTOR_H

#include <correlations/correlation.H>

/** BwSpiveyMN


*/
class BwSpiveyMN : public WaterVolumeFactor
{
  Correlation_Singleton(BwSpiveyMN);

  BwSpiveyMN()
    : WaterVolumeFactor("BwSpiveyMN", RB_STB::get_instance())
  {
    set_author("Spivey et al.");
    add_parameter("t", Celsius::get_instance(), "Aqui Neylith",
                  Quantity<Fahrenheit>(32), Quantity<Fahrenheit>(530));
    add_parameter("p", mPascal::get_instance(), "Aqui Neylith",
                  Quantity<psia>(14.5), Quantity<psia>(29000));
    add_parameter("s", Dissolved_Salt_Percent::get_instance(), "Aqui Neylith",
                  0, 26);
  }

public:
  inline Quantity<RB_STB> impl(const Quantity<Celsius>& t,
                               const Quantity<mPascal>& p,
                               const Quantity<Dissolved_Salt_Percent>& s) const;

  Quantity<RB_STB> operator()(const Quantity<Celsius>& t,
                              const Quantity<mPascal>& p,
                              const Quantity<Dissolved_Salt_Percent>& s) const
  {
    return impl(t, p, s);
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
      const auto& s = it.get_curr();

      return impl(t, p, s);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& s = it.get_curr();

    return impl(t, p, s);
  }
};

extern Quantity<RB_STB> BwSpiveyMN__correlation__fct(
  const Quantity<Celsius>& t, const Quantity<mPascal>& p,
  const Quantity<Dissolved_Salt_Percent>& s);
extern double BwSpiveyMN__correlation__valid__fct(double t, double p, double s);

/** BwMcCain


*/
class BwMcCain : public WaterVolumeFactor
{
  Correlation_Singleton(BwMcCain);

  BwMcCain()
    : WaterVolumeFactor("BwMcCain", RB_STB::get_instance())
  {
    set_author("McCain");
    add_parameter("t", Fahrenheit::get_instance(), "Aqui Neylith", 70, 250);
    add_parameter("p", psia::get_instance(), "Aqui Neylith", 1000, 5000);
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

extern Quantity<RB_STB> BwMcCain__correlation__fct(
  const Quantity<Fahrenheit>& t, const Quantity<psia>& p);
extern double BwMcCain__correlation__valid__fct(double t, double p);

/** BwMcCoy


*/
class BwMcCoy : public WaterVolumeFactor
{
  Correlation_Singleton(BwMcCoy);

  BwMcCoy()
    : WaterVolumeFactor("BwMcCoy", RB_STB::get_instance())
  {
    set_author("McCoy");
    add_parameter("t", Fahrenheit::get_instance(), "Aqui Neylith");
    add_parameter("p", psia::get_instance(), "Aqui Neylith");
    add_parameter("pb", psia::get_instance(), "Aqui Neylith");
    add_parameter("s", Dissolved_Salt_Percent::get_instance(), "Aqui Neylith");
    add_parameter("cw", psia_1::get_instance(), "Aqui Neylith");
  }

public:
  inline Quantity<RB_STB> impl(const Quantity<Fahrenheit>& t,
                               const Quantity<psia>& p,
                               const Quantity<psia>& pb,
                               const Quantity<Dissolved_Salt_Percent>& s,
                               const Quantity<psia_1>& cw) const;

  Quantity<RB_STB> operator()(const Quantity<Fahrenheit>& t,
                              const Quantity<psia>& p, const Quantity<psia>& pb,
                              const Quantity<Dissolved_Salt_Percent>& s,
                              const Quantity<psia_1>& cw) const
  {
    return impl(t, p, pb, s, cw);
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
      const auto& pb = it.get_curr();
      it.next();
      const auto& s = it.get_curr();
      it.next();
      const auto& cw = it.get_curr();

      return impl(t, p, pb, s, cw);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();
    it.next();
    const auto& s = it.get_curr();
    it.next();
    const auto& cw = it.get_curr();

    return impl(t, p, pb, s, cw);
  }
};

extern Quantity<RB_STB> BwMcCoy__correlation__fct(
  const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
  const Quantity<psia>& pb, const Quantity<Dissolved_Salt_Percent>& s,
  const Quantity<psia_1>& cw);
extern double BwMcCoy__correlation__valid__fct(double t, double p, double pb,
                                               double s, double cw);

#endif // WATER_VOLUME_FACTOR_H
