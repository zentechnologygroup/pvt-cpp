#ifndef DEAD_OIL_VISCOSITY_H
#define DEAD_OIL_VISCOSITY_H

#include <correlations/correlation.H>

/** UodBeal


*/
class UodBeal : public DeadOilViscosity
{
  Correlation_Singleton(UodBeal);

  UodBeal()
    : DeadOilViscosity("UodBeal", CP::get_instance(), 0.865, 1550)
  {
    set_author("Beal");
    set_title("BEAL CORRELATION, CALCULATION OF DEAD OIL VISCOSITY");
    add_parameter("api", Api::get_instance(), "API oil gravity", 10.1, 52.5);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 100, 220);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 98 samples of gas-free crude oil measured at temperatures "
           "above 100 °F and 655 samples measured at 100 °F obtained from 492 "
           "different oil fields, of which 358 are in the United States.");
    add_note("Beal (1946) presented a graphical correlation showing the "
             "effects of both oil gravity and temperature on dead-oil "
             "viscosity. The equation is presented by Standing (1977).");
    add_ref("beal:1946");
    add_ref("standing:1977");
  }

public:
  inline Quantity<CP> impl(const Quantity<Api>& api,
                           const Quantity<Fahrenheit>& t) const;

  Quantity<CP> operator()(const Quantity<Api>& api,
                          const Quantity<Fahrenheit>& t) const
  {
    return impl(api, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(api, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(api, t);
  }
};

extern Quantity<CP> UodBeal__correlation__fct(const Quantity<Api>& api,
                                              const Quantity<Fahrenheit>& t);
extern double UodBeal__correlation__valid__fct(double api, double t);

/** UodBeggsRobinson


*/
class UodBeggsRobinson : public DeadOilViscosity
{
  Correlation_Singleton(UodBeggsRobinson);

  UodBeggsRobinson()
    : DeadOilViscosity("UodBeggsRobinson", CP::get_instance())
  {
    set_author("Beggs & Robinson");
    set_title(
      "BEGGS & ROBINSON CORRELATION, CALCULATION OF DEAD OIL VISCOSITY");
    add_parameter("api", Api::get_instance(), "API oil gravity", 16, 58);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 70, 295);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 460 dead oil observations. Beggs & Robinson (1975) "
           "developed correlations for dead and live oil viscosities with "
           "samples obtained from 600 oil systems.");
    add_ref("beggs:1975");
  }

public:
  inline Quantity<CP> impl(const Quantity<Api>& api,
                           const Quantity<Fahrenheit>& t) const;

  Quantity<CP> operator()(const Quantity<Api>& api,
                          const Quantity<Fahrenheit>& t) const
  {
    return impl(api, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(api, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(api, t);
  }
};

extern Quantity<CP> UodBeggsRobinson__correlation__fct(
  const Quantity<Api>& api, const Quantity<Fahrenheit>& t);
extern double UodBeggsRobinson__correlation__valid__fct(double api, double t);

/** UodEgbogahNg


*/
class UodEgbogahNg : public DeadOilViscosity
{
  Correlation_Singleton(UodEgbogahNg);

  UodEgbogahNg()
    : DeadOilViscosity("UodEgbogahNg", CP::get_instance())
  {
    set_author("Ng & Egbogah");
    set_title("NG & EGBOGAH CORRELATION (MODIFIED BEGGS & ROBINSON "
              "CORRELATION), CALCULATION OF DEAD OIL VISCOSITY");
    add_parameter("api", Api::get_instance(), "API oil gravity", 5, 58);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature",
                  Quantity<Celsius>(15), Quantity<Celsius>(80));
    def_parameter_latex_symbol("t", "T");
    add_db("Based on viscosity data from the Reservoir Fluids Analysis "
           "Laboratory of AGAT Engineering Utd. The total number of oil "
           "systems used is 394.");
    add_ref("ng:1983");
  }

public:
  inline Quantity<CP> impl(const Quantity<Api>& api,
                           const Quantity<Fahrenheit>& t) const;

  Quantity<CP> operator()(const Quantity<Api>& api,
                          const Quantity<Fahrenheit>& t) const
  {
    return impl(api, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(api, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(api, t);
  }
};

extern Quantity<CP> UodEgbogahNg__correlation__fct(
  const Quantity<Api>& api, const Quantity<Fahrenheit>& t);
extern double UodEgbogahNg__correlation__valid__fct(double api, double t);

/** UodGlaso

The ranges are taken from the curves presented by Glaso.


*/
class UodGlaso : public DeadOilViscosity
{
  Correlation_Singleton(UodGlaso);

  UodGlaso()
    : DeadOilViscosity("UodGlaso", CP::get_instance(), 0.2, 80)
  {
    set_author("Glaso");
    set_title("GLASO CORRELATION, CALCULATION OF DEAD OIL VISCOSITY");
    add_parameter("api", Api::get_instance(), "API oil gravity", 20, 48);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 50, 300);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 26 samples from the North Sea (collected from wells in "
           "the region 56 to 62°N).");
    add_db("The relation between dead oil viscosity, API gravity, and "
           "temperature, was developed specifically for North Sea oils (for "
           "oils with a paraffinicity characterization factor of 11.9), which "
           "have a factor almost equal to California oils, as well as for oil "
           "samples from fields in various regions of the world.");
    add_ref("glaso:1980");
  }

public:
  inline Quantity<CP> impl(const Quantity<Api>& api,
                           const Quantity<Fahrenheit>& t) const;

  Quantity<CP> operator()(const Quantity<Api>& api,
                          const Quantity<Fahrenheit>& t) const
  {
    return impl(api, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(api, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(api, t);
  }
};

extern Quantity<CP> UodGlaso__correlation__fct(const Quantity<Api>& api,
                                               const Quantity<Fahrenheit>& t);
extern double UodGlaso__correlation__valid__fct(double api, double t);

/** UodKartoatmodjoSchmidt


*/
class UodKartoatmodjoSchmidt : public DeadOilViscosity
{
  Correlation_Singleton(UodKartoatmodjoSchmidt);

  UodKartoatmodjoSchmidt()
    : DeadOilViscosity("UodKartoatmodjoSchmidt", CP::get_instance(), 0.506, 682)
  {
    set_author("Kartoatmodjo & Schmidt");
    set_title(
      "KARTOATMODJO & SCHMIDT CORRELATION, CALCULATION OF DEAD OIL VISCOSITY");
    add_parameter("api", Api::get_instance(), "API oil gravity", 14.4, 59);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 80, 320);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on a total of 661 different crude oil samples. The data bank "
           "was collected from PVT reports and literature.");
    add_db("The first major source was from South East Asia, mainly Indonesia. "
           "The second source was North America, including the offshore area. "
           "The rest came from the Middle East and Latin America.");
    add_ref("kartoatmodjo:1991");
  }

public:
  inline Quantity<CP> impl(const Quantity<Api>& api,
                           const Quantity<Fahrenheit>& t) const;

  Quantity<CP> operator()(const Quantity<Api>& api,
                          const Quantity<Fahrenheit>& t) const
  {
    return impl(api, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(api, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(api, t);
  }
};

extern Quantity<CP> UodKartoatmodjoSchmidt__correlation__fct(
  const Quantity<Api>& api, const Quantity<Fahrenheit>& t);
extern double UodKartoatmodjoSchmidt__correlation__valid__fct(double api,
                                                              double t);

/** UodSattarinEtAl

Sattarin confirmed that the correlation has an error in the published article.
In order to correct it, -5.9836 was replaced by -5.8936. Furthermore, all the
decimals indicated by Sattarin were used, whereas in the article the numbers
were rounded.

All crude oil viscosities were measured at temperatures 10, 20, and 40 °C.


*/
class UodSattarinEtAl : public DeadOilViscosity
{
  Correlation_Singleton(UodSattarinEtAl);

  UodSattarinEtAl()
    : DeadOilViscosity("UodSattarinEtAl", CP::get_instance(), 2, 570)
  {
    set_author("Sattarin et al.");
    set_title("SATTARIN ET AL. CORRELATION, CALCULATION OF DEAD OIL VISCOSITY");
    add_parameter("api", Api::get_instance(), "API oil gravity", 17, 45);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Kelvin::get_instance(), "Temperature",
                  Quantity<Celsius>(10), Quantity<Celsius>(40));
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 438 data points from Iranian off and on-shore dead crude "
           "oil samples, collected in Crude Oil Evaluation department at RIPI. "
           "Standard test method of ASTM D-445 was used for kinematic "
           "viscosity measurements.");
    add_db("Data were divided into two major groups, depending on the API "
           "gravity. The first group included 85 Iranian heavy crude oil "
           "(17<=°API<28) data-points and the second one contained 353 Iranian "
           "light crude oil (28<=°API<45) data-points.");
    add_ref("sattarin:2007");
  }

public:
  inline Quantity<CP> impl(const Quantity<Api>& api,
                           const Quantity<Kelvin>& t) const;

  Quantity<CP> operator()(const Quantity<Api>& api,
                          const Quantity<Kelvin>& t) const
  {
    return impl(api, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(api, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(api, t);
  }
};

extern Quantity<CP> UodSattarinEtAl__correlation__fct(
  const Quantity<Api>& api, const Quantity<Kelvin>& t);
extern double UodSattarinEtAl__correlation__valid__fct(double api, double t);

/** UodNaseri


*/
class UodNaseri : public DeadOilViscosity
{
  Correlation_Singleton(UodNaseri);

  UodNaseri()
    : DeadOilViscosity("UodNaseri", CP::get_instance(), 0.75, 54)
  {
    set_author("Naseri, Nikazar & Mousavi");
    set_title("NASERI, NIKAZAR & MOUSAVI CORRELATION, CALCULATION OF DEAD OIL "
              "VISCOSITY");
    add_parameter("api", Api::get_instance(), "API oil gravity", 17, 44);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 105, 295);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on PVT experimental data of 472 series of Iranian oil "
           "reservoirs. About 250 series of PVT and viscosity data have been "
           "used in developing a new empirical correlation.");
    add_db("Validity and accuracy of the proposed correlations (dead oil "
           "viscosity, saturated and undersaturated viscosity) have been "
           "checked by their application to 222 samples.");
    add_ref("naseri:2005");
  }

public:
  inline Quantity<CP> impl(const Quantity<Api>& api,
                           const Quantity<Fahrenheit>& t) const;

  Quantity<CP> operator()(const Quantity<Api>& api,
                          const Quantity<Fahrenheit>& t) const
  {
    return impl(api, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(api, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(api, t);
  }
};

extern Quantity<CP> UodNaseri__correlation__fct(const Quantity<Api>& api,
                                                const Quantity<Fahrenheit>& t);
extern double UodNaseri__correlation__valid__fct(double api, double t);

/** UodPetroskyFarshad

The independent variables were obtained from laboratory differential data. The
API oil gravity is the residual oil gravity.


*/
class UodPetroskyFarshad : public DeadOilViscosity
{
  Correlation_Singleton(UodPetroskyFarshad);

  UodPetroskyFarshad()
    : DeadOilViscosity("UodPetroskyFarshad", CP::get_instance(), 0.725, 10.249)
  {
    set_author("Petrosky & Farshad");
    set_title(
      "PETROSKY & FARSHAD CORRELATION, CALCULATION OF DEAD OIL VISCOSITY");
    add_parameter("api", Api::get_instance(), "API oil gravity", 25.4, 46.1);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 114, 288);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on a set of 118 data points from 126 laboratory PVT analyses "
           "of Gulf of Mexico crude oils.");
    add_db("Fluid samples were obtained from reservoirs located offshore Texas "
           "and Louisiana. Producing areas from Galveston Island, eastward, "
           "through Main Pass are represented.");
    add_ref("petrosky:1995");
  }

public:
  inline Quantity<CP> impl(const Quantity<Api>& api,
                           const Quantity<Fahrenheit>& t) const;

  Quantity<CP> operator()(const Quantity<Api>& api,
                          const Quantity<Fahrenheit>& t) const
  {
    return impl(api, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(api, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(api, t);
  }
};

extern Quantity<CP> UodPetroskyFarshad__correlation__fct(
  const Quantity<Api>& api, const Quantity<Fahrenheit>& t);
extern double UodPetroskyFarshad__correlation__valid__fct(double api, double t);

/** UodDeGhettoEtAl


*/
class UodDeGhettoEtAl : public DeadOilViscosity
{
  Correlation_Singleton(UodDeGhettoEtAl);

  UodDeGhettoEtAl()
    : DeadOilViscosity("UodDeGhettoEtAl", CP::get_instance(), 7.7, 1386.9)
  {
    set_author("De Ghetto, Paone & Villa");
    set_title("DE GHETTO, PAONE & VILLA CORRELATION FOR EXTRA-HEAVY AND HEAVY "
              "OILS (MODIFIED EGBOGAH-JACK CORRELATION), CALCULATION OF DEAD "
              "OIL VISCOSITY");
    add_parameter("api", Api::get_instance(), "API oil gravity", 6, 22.3);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 131.4, 250.7);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 1200 measured data points of 63 heavy and extra-heavy oil "
           "samples obtained from the Mediterranean Basin, Africa and the "
           "Persian Gulf.");
    add_db("Oil samples have been divided in two different API gravity "
           "classes: extra-heavy oils for °API<=10, heavy oils for "
           "10<°API<=22.3.");
    add_ref("deGhetto:1995");
  }

public:
  inline Quantity<CP> impl(const Quantity<Api>& api,
                           const Quantity<Fahrenheit>& t) const;

  Quantity<CP> operator()(const Quantity<Api>& api,
                          const Quantity<Fahrenheit>& t) const
  {
    return impl(api, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(api, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(api, t);
  }
};

extern Quantity<CP> UodDeGhettoEtAl__correlation__fct(
  const Quantity<Api>& api, const Quantity<Fahrenheit>& t);
extern double UodDeGhettoEtAl__correlation__valid__fct(double api, double t);

/** UodPerezML

The correlation could not be verified because the original reference is not
available. Date: September 1 2016.


*/
class UodPerezML : public DeadOilViscosity
{
  Correlation_Singleton(UodPerezML);

  UodPerezML()
    : DeadOilViscosity("UodPerezML", CP::get_instance())
  {
    set_author("Pérez, Heny & Lago");
    set_title("PÉREZ, HENY & LAGO CORRELATION (MODIFIED BEGGS & ROBINSON "
              "CORRELATION), CALCULATION OF DEAD OIL VISCOSITY");
    set_hidden();
    add_parameter("api", Api::get_instance(), "API oil gravity", 6.4, 12.9);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 112, 300);
    def_parameter_latex_symbol("t", "T");
    add_ref("perez:2001");
  }

public:
  inline Quantity<CP> impl(const Quantity<Api>& api,
                           const Quantity<Fahrenheit>& t) const;

  Quantity<CP> operator()(const Quantity<Api>& api,
                          const Quantity<Fahrenheit>& t) const
  {
    return impl(api, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(api, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(api, t);
  }
};

extern Quantity<CP> UodPerezML__correlation__fct(const Quantity<Api>& api,
                                                 const Quantity<Fahrenheit>& t);
extern double UodPerezML__correlation__valid__fct(double api, double t);

/** UodDindorukChristman


*/
class UodDindorukChristman : public DeadOilViscosity
{
  Correlation_Singleton(UodDindorukChristman);

  UodDindorukChristman()
    : DeadOilViscosity("UodDindorukChristman", CP::get_instance(), 0.896, 62.63)
  {
    set_author("Dindoruk & Christman");
    set_title(
      "DINDORUK & CHRISTMAN CORRELATION, CALCULATION OF DEAD OIL VISCOSITY");
    add_parameter("api", Api::get_instance(), "API oil gravity", 17.4, 40);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 121, 276);
    def_parameter_latex_symbol("t", "T");
    add_parameter("pb", psia::get_instance(), "Bubble point pressure", 926,
                  12230);
    def_parameter_latex_symbol("pb", "P_b");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 133,
                  3050);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_db("Based on more than 90 PVT reports from the Gulf of Mexico.");
    add_ref("dindoruk:2004");
  }

public:
  inline Quantity<CP> impl(const Quantity<Api>& api,
                           const Quantity<Fahrenheit>& t,
                           const Quantity<psia>& pb,
                           const Quantity<SCF_STB>& rsb) const;

  Quantity<CP> operator()(const Quantity<Api>& api,
                          const Quantity<Fahrenheit>& t,
                          const Quantity<psia>& pb,
                          const Quantity<SCF_STB>& rsb) const
  {
    return impl(api, t, pb, rsb);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();
      it.next();
      const auto& pb = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();

      return impl(api, t, pb, rsb);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();

    return impl(api, t, pb, rsb);
  }
};

extern Quantity<CP> UodDindorukChristman__correlation__fct(
  const Quantity<Api>& api, const Quantity<Fahrenheit>& t,
  const Quantity<psia>& pb, const Quantity<SCF_STB>& rsb);
extern double UodDindorukChristman__correlation__valid__fct(double api,
                                                            double t, double pb,
                                                            double rsb);

#endif // DEAD_OIL_VISCOSITY_H
