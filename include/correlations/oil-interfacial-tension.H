#ifndef OIL_INTERFACIAL_TENSION_H
#define OIL_INTERFACIAL_TENSION_H

#include <correlations/correlation.H>

/** SgoBakerSwerdloff

The correlation was verified by using secondary references (Beggs, 1987 and
Bánzer, 1996) because the original reference is not available. Date: August 30
2016.


*/
class SgoBakerSwerdloff : public OilInterfacialTension
{
  Correlation_Singleton(SgoBakerSwerdloff);

  SgoBakerSwerdloff()
    : OilInterfacialTension("SgoBakerSwerdloff", dynes_cm::get_instance(), 0,
                            40)
  {
    set_author("Baker & Swerdloff");
    set_title(
      "BAKER & SWERDLOFF CORRELATION, CALCULATION OF OIL INTERFACIAL TENSION");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature");
    def_parameter_latex_symbol("t", "T");
    add_parameter("api", Api::get_instance(), "API oil gravity", 4, 75);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("p", psia::get_instance(), "Pressure", 14.7, 5000);
    def_parameter_latex_symbol("p", "P");
    add_ref("baker:1956");
    add_ref("beggs:1987");
    add_ref("petroWiki:2016:4");
  }

public:
  inline Quantity<dynes_cm> impl(const Quantity<Fahrenheit>& t,
                                 const Quantity<Api>& api,
                                 const Quantity<psia>& p) const;

  Quantity<dynes_cm> operator()(const Quantity<Fahrenheit>& t,
                                const Quantity<Api>& api,
                                const Quantity<psia>& p) const
  {
    return impl(t, api, p);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& t = it.get_curr();
      it.next();
      const auto& api = it.get_curr();
      it.next();
      const auto& p = it.get_curr();

      return impl(t, api, p);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& t = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& p = it.get_curr();

    return impl(t, api, p);
  }
};

extern Quantity<dynes_cm> SgoBakerSwerdloff__correlation__fct(
  const Quantity<Fahrenheit>& t, const Quantity<Api>& api,
  const Quantity<psia>& p);
extern double SgoBakerSwerdloff__correlation__valid__fct(double t, double api,
                                                         double p);

#endif // OIL_INTERFACIAL_TENSION_H
