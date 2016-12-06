#ifndef WATER_INTERFACIAL_TENSION_H
#define WATER_INTERFACIAL_TENSION_H

#include <correlations/correlation.H>

/** SgwJenningsNewman

The data bank and development ranges were obtained from the original reference
and a secondary one: McCain et al. (1990).

Jennings & Newman (1971) presented a graphical correlation with a pressure range
up to 12000 psia. McCain et al. (1990) presented the correlation in a
mathematical form, indicating that it should not be used for pressures above
8000 psia.

The correlation was verified by using Bánzer (1996) and McCain et al. (1990) as
secondary references. Date: October 13 2016.


*/
class SgwJenningsNewman : public WaterInterfacialTension
{
  Correlation_Singleton(SgwJenningsNewman);

  SgwJenningsNewman()
    : WaterInterfacialTension("SgwJenningsNewman", dynes_cm::get_instance())
  {
    set_author("Jennings & Newman");
    set_title("JENNINGS & NEWMAN CORRELATION, CALCULATION OF GAS-WATER "
              "INTERFACIAL TENSION");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 74, 350);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure", 14.7, 8000);
    def_parameter_latex_symbol("p", "P");
    add_db("Based on measurements of interfacial tension of water against "
           "mixtures of methane and normal decane, under reservoir conditions "
           "of temperature and pressure.");
    add_db("The interfacial tension data were obtained by the pendent drop "
           "method. Seven systems covering the methane-decane composition "
           "range from 100 percent decane to 100 percent methane were studied "
           "at three temperatures and 11 pressure intervals in the "
           "single-phase, hydrocarbon region.");
    add_ref("jennings:1971");
    add_ref("mcCain:1990");
  }

public:
  inline Quantity<dynes_cm> impl(const Quantity<Fahrenheit>& t,
                                 const Quantity<psia>& p) const;

  Quantity<dynes_cm> operator()(const Quantity<Fahrenheit>& t,
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

extern Quantity<dynes_cm> SgwJenningsNewman__correlation__fct(
  const Quantity<Fahrenheit>& t, const Quantity<psia>& p);
extern double SgwJenningsNewman__correlation__valid__fct(double t, double p);

#endif // WATER_INTERFACIAL_TENSION_H
