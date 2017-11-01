#ifndef OIL_INTERFACIAL_TENSION_H
#define OIL_INTERFACIAL_TENSION_H

#include <pvt-exceptions.H>
#include <correlations/correlation.H>

/** SgoBakerSwerdloff

    Generated at 2017-10-30 16:49:06 -0400

    Zen Technologies Group


The correlation was verified by using secondary references (Beggs, 1987 and
Bánzer, 1996) because the original reference is not available. Date: August 30
2016.

References:

  - baker:1956

  - beggs:1987

  - petroWiki:2016:4

@param[in] t Temperature. Range in [, ]
@param[in] api API oil gravity. Range in [4, 75]
@param[in] p Pressure. Range in [14.7, 5000]
@return inside [0, 40]
@ingroup Correlations
*/
class SgoBakerSwerdloff : public OilInterfacialTension {
  Correlation_Singleton(SgoBakerSwerdloff);

  SgoBakerSwerdloff()
      : OilInterfacialTension("SgoBakerSwerdloff", dynes_cm::get_instance(), 0,
                              40) {
    set_author("Baker & Swerdloff");
    set_title("BAKER & SWERDLOFF CORRELATION, CALCULATION OF OIL INTERFACIAL "
              "TENSION");
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
  static inline double impl(const double& t, const double& api,
                            const double& p) noexcept;

  Quantity<dynes_cm> call(const Quantity<Fahrenheit>& t,
                          const Quantity<Api>& api,
                          const Quantity<psia>& p) const {
    return Quantity<dynes_cm>(impl(t.raw(), api.raw(), p.raw()));
  }

  Quantity<dynes_cm> validate_and_call(const Quantity<Fahrenheit>& t,
                                       const Quantity<Api>& api,
                                       const Quantity<psia>& p) const {
    if (not(api >= 4))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 4");
    if (not(api <= 75))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 75");

    return call(t, api, p);
  }

  Quantity<dynes_cm> operator()(const Quantity<Fahrenheit>& t,
                                const Quantity<Api>& api,
                                const Quantity<psia>& p) const {
    return Quantity<dynes_cm>(call(t, api, p));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& t = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& p = it.get_curr();

    return call(t, api, p);
  }
};

extern Quantity<dynes_cm> SgoBakerSwerdloff__correlation__fct(
    const Quantity<Fahrenheit>& t, const Quantity<Api>& api,
    const Quantity<psia>& p);
extern double SgoBakerSwerdloff__correlation__valid__fct(const double& t,
                                                         const double& api,
                                                         const double& p);

#endif // OIL_INTERFACIAL_TENSION_H
