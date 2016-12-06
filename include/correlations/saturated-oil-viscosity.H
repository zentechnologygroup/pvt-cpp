#ifndef SATURATED_OIL_VISCOSITY_H
#define SATURATED_OIL_VISCOSITY_H

#include <correlations/correlation.H>

/** UobBeggsRobinson


*/
class UobBeggsRobinson : public SaturatedOilViscosity
{
  Correlation_Singleton(UobBeggsRobinson);

  UobBeggsRobinson()
    : SaturatedOilViscosity("UobBeggsRobinson", CP::get_instance())
  {
    set_author("Beggs & Robinson");
    set_title(
      "BEGGS & ROBINSON CORRELATION, CALCULATION OF SATURATED OIL VISCOSITY");
    add_parameter("uod", CP::get_instance(), "Dead oil viscosity");
    def_parameter_latex_symbol("uod", "\\mu_{od}");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 20, 2070);
    def_parameter_latex_symbol("rs", "R_s");
    add_db("Based on 2073 live oil observations. Beggs & Robinson developed "
           "correlations for dead and live oil viscosities with samples "
           "obtained from 600 oil systems.");
    add_ref("beggs:1975");
  }

public:
  inline Quantity<CP> impl(const Quantity<CP>& uod,
                           const Quantity<SCF_STB>& rs) const;

  Quantity<CP> operator()(const Quantity<CP>& uod,
                          const Quantity<SCF_STB>& rs) const
  {
    return impl(uod, rs);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& uod = it.get_curr();
      it.next();
      const auto& rs = it.get_curr();

      return impl(uod, rs);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& uod = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();

    return impl(uod, rs);
  }
};

extern Quantity<CP> UobBeggsRobinson__correlation__fct(
  const Quantity<CP>& uod, const Quantity<SCF_STB>& rs);
extern double UobBeggsRobinson__correlation__valid__fct(double uod, double rs);

/** UobChewConnally

The values of A and b of the equation are presented by Bánzer (1996) and Ahmed
(2010).


*/
class UobChewConnally : public SaturatedOilViscosity
{
  Correlation_Singleton(UobChewConnally);

  UobChewConnally()
    : SaturatedOilViscosity("UobChewConnally", CP::get_instance(), 0.1, 100)
  {
    set_author("Chew & Connally");
    set_title(
      "CHEW & CONNALLY CORRELATION, CALCULATION OF SATURATED OIL VISCOSITY");
    add_parameter("uod", CP::get_instance(), "Dead oil viscosity", 0.377, 100);
    def_parameter_latex_symbol("uod", "\\mu_{od}");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 0, 1600);
    def_parameter_latex_symbol("rs", "R_s");
    add_db("Based on 457 crude oil samples obtained from all the important "
           "producing areas of the U.S., as well as from Canada and South "
           "America.");
    add_note("Chew & Connally presented the graphical correlation and the "
             "general form of the equation.");
    add_ref("chew:1959");
    add_ref("ahmed:2010");
  }

public:
  inline Quantity<CP> impl(const Quantity<CP>& uod,
                           const Quantity<SCF_STB>& rs) const;

  Quantity<CP> operator()(const Quantity<CP>& uod,
                          const Quantity<SCF_STB>& rs) const
  {
    return impl(uod, rs);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& uod = it.get_curr();
      it.next();
      const auto& rs = it.get_curr();

      return impl(uod, rs);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& uod = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();

    return impl(uod, rs);
  }
};

extern Quantity<CP> UobChewConnally__correlation__fct(
  const Quantity<CP>& uod, const Quantity<SCF_STB>& rs);
extern double UobChewConnally__correlation__valid__fct(double uod, double rs);

/** UobKhan


*/
class UobKhan : public SaturatedOilViscosity
{
  Correlation_Singleton(UobKhan);

  UobKhan()
    : SaturatedOilViscosity("UobKhan", CP::get_instance(), 0.13, 77.4)
  {
    set_author("Khan et al.");
    set_title(
      "KHAN ET AL. CORRELATION, CALCULATION OF SATURATED OIL VISCOSITY");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 24,
                  1901);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(44.6), Quantity<Api>(14.3));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.752,
                  1.367);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 75, 240);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure", 14.7, 5015);
    def_parameter_latex_symbol("p", "P");
    add_parameter("pb", psia::get_instance(), "Bubble point pressure", 107,
                  4315);
    def_parameter_latex_symbol("pb", "P_b");
    add_db("Based on 75 bottom hole samples taken from 62 Saudi Arabian oil "
           "reservoirs. A total of 150 data points were used for bubble point "
           "oil viscosity in 1691 for oil viscosity below the bubble point "
           "pressure.");
    add_ref("khan:1987");
    add_par_synonym("yo", "api", "api");
  }

public:
  inline Quantity<CP> impl(const Quantity<SCF_STB>& rsb,
                           const Quantity<Sg_do>& yo, const Quantity<Sgg>& yg,
                           const Quantity<Fahrenheit>& t,
                           const Quantity<psia>& p,
                           const Quantity<psia>& pb) const;

  Quantity<CP> operator()(const Quantity<SCF_STB>& rsb,
                          const Quantity<Sg_do>& yo, const Quantity<Sgg>& yg,
                          const Quantity<Fahrenheit>& t,
                          const Quantity<psia>& p,
                          const Quantity<psia>& pb) const
  {
    return impl(rsb, yo, yg, t, p, pb);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& rsb = it.get_curr();
      it.next();
      const auto& yo = it.get_curr();
      it.next();
      const auto& yg = it.get_curr();
      it.next();
      const auto& t = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& pb = it.get_curr();

      return impl(rsb, yo, yg, t, p, pb);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& yg = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();

    return impl(rsb, yo, yg, t, p, pb);
  }
};

extern Quantity<CP> UobKhan__correlation__fct(const Quantity<SCF_STB>& rsb,
                                              const Quantity<Sg_do>& yo,
                                              const Quantity<Sgg>& yg,
                                              const Quantity<Fahrenheit>& t,
                                              const Quantity<psia>& p,
                                              const Quantity<psia>& pb);
extern double UobKhan__correlation__valid__fct(double rsb, double yo, double yg,
                                               double t, double p, double pb);

/** UobKartoatmodjoSchmidt

Depending on the API gravity, the samples can cover three different classes of
crude oils: heavy oils for 10<°API<=22.3, medium oils for 22.3<°API<=31.1, and
light oils for °API>31.1.


*/
class UobKartoatmodjoSchmidt : public SaturatedOilViscosity
{
  Correlation_Singleton(UobKartoatmodjoSchmidt);

  UobKartoatmodjoSchmidt()
    : SaturatedOilViscosity("UobKartoatmodjoSchmidt", CP::get_instance(), 0.097,
                            586)
  {
    set_author("Kartoatmodjo & Schmidt");
    set_title("KARTOATMODJO & SCHMIDT CORRELATION FOR HEAVY, MEDIUM AND LIGHT "
              "OILS, CALCULATION OF SATURATED OIL VISCOSITY");
    add_parameter("uod", CP::get_instance(), "Dead oil viscosity", 0.506,
                  682.0);
    def_parameter_latex_symbol("uod", "\\mu_{od}");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 0, 2890);
    def_parameter_latex_symbol("rs", "R_s");
    add_db("Based on 5321 data points of heavy, medium and light oil samples. "
           "The data bank was collected from PVT reports and literature.");
    add_db("The first major source was from South East Asia, mainly Indonesia. "
           "The second source was North America, including the offshore area. "
           "The rest came from the Middle East and Latin America.");
    add_ref("kartoatmodjo:1991");
  }

public:
  inline Quantity<CP> impl(const Quantity<CP>& uod,
                           const Quantity<SCF_STB>& rs) const;

  Quantity<CP> operator()(const Quantity<CP>& uod,
                          const Quantity<SCF_STB>& rs) const
  {
    return impl(uod, rs);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& uod = it.get_curr();
      it.next();
      const auto& rs = it.get_curr();

      return impl(uod, rs);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& uod = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();

    return impl(uod, rs);
  }
};

extern Quantity<CP> UobKartoatmodjoSchmidt__correlation__fct(
  const Quantity<CP>& uod, const Quantity<SCF_STB>& rs);
extern double UobKartoatmodjoSchmidt__correlation__valid__fct(double uod,
                                                              double rs);

/** UobPetroskyFarshad


*/
class UobPetroskyFarshad : public SaturatedOilViscosity
{
  Correlation_Singleton(UobPetroskyFarshad);

  UobPetroskyFarshad()
    : SaturatedOilViscosity("UobPetroskyFarshad", CP::get_instance(), 0.211,
                            7.403)
  {
    set_author("Petrosky & Farshad");
    set_title(
      "PETROSKY & FARSHAD CORRELATION, CALCULATION OF SATURATED OIL VISCOSITY");
    add_parameter("uod", CP::get_instance(), "Dead oil viscosity", 0.725,
                  11.69);
    def_parameter_latex_symbol("uod", "\\mu_{od}");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 21, 1885);
    def_parameter_latex_symbol("rs", "R_s");
    add_db("Based on a set of 864 data points from 126 laboratory PVT analyses "
           "of Gulf of Mexico crude oils. For the ranges of carbon dioxide and "
           "nitrogen concentration in the mixture, 88 data points were "
           "considered.");
    add_db("Fluid samples were obtained from reservoirs located offshore Texas "
           "and Louisiana. Producing areas from Galveston Island, eastward, "
           "through Main Pass are represented.");
    add_ref("petrosky:1995");
  }

public:
  inline Quantity<CP> impl(const Quantity<CP>& uod,
                           const Quantity<SCF_STB>& rs) const;

  Quantity<CP> operator()(const Quantity<CP>& uod,
                          const Quantity<SCF_STB>& rs) const
  {
    return impl(uod, rs);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& uod = it.get_curr();
      it.next();
      const auto& rs = it.get_curr();

      return impl(uod, rs);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& uod = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();

    return impl(uod, rs);
  }
};

extern Quantity<CP> UobPetroskyFarshad__correlation__fct(
  const Quantity<CP>& uod, const Quantity<SCF_STB>& rs);
extern double UobPetroskyFarshad__correlation__valid__fct(double uod,
                                                          double rs);

/** UobPerezML

The correlation could not be verified because the original reference is not
available. Date: September 6 2016.


*/
class UobPerezML : public SaturatedOilViscosity
{
  Correlation_Singleton(UobPerezML);

  UobPerezML()
    : SaturatedOilViscosity("UobPerezML", CP::get_instance())
  {
    set_author("Pérez, Heny & Lago");
    set_title(
      "PÉREZ, HENY & LAGO CORRELATION, CALCULATION OF SATURATED OIL VISCOSITY");
    set_hidden();
    add_parameter("uod", CP::get_instance(), "Dead oil viscosity");
    def_parameter_latex_symbol("uod", "\\mu_{od}");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 38, 121);
    def_parameter_latex_symbol("rs", "R_s");
    add_ref("perez:2001");
  }

public:
  inline Quantity<CP> impl(const Quantity<CP>& uod,
                           const Quantity<SCF_STB>& rs) const;

  Quantity<CP> operator()(const Quantity<CP>& uod,
                          const Quantity<SCF_STB>& rs) const
  {
    return impl(uod, rs);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& uod = it.get_curr();
      it.next();
      const auto& rs = it.get_curr();

      return impl(uod, rs);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& uod = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();

    return impl(uod, rs);
  }
};

extern Quantity<CP> UobPerezML__correlation__fct(const Quantity<CP>& uod,
                                                 const Quantity<SCF_STB>& rs);
extern double UobPerezML__correlation__valid__fct(double uod, double rs);

/** UobGilFonseca

The correlation could not be verified because the original and secondary
references are not available. Date: September 6 2016.


*/
class UobGilFonseca : public SaturatedOilViscosity
{
  Correlation_Singleton(UobGilFonseca);

  UobGilFonseca()
    : SaturatedOilViscosity("UobGilFonseca", CP::get_instance())
  {
    set_author("Gil & Fonseca");
    set_title(
      "GIL & FONSECA CORRELATION, CALCULATION OF SATURATED OIL VISCOSITY");
    set_hidden();
    add_parameter("uod", CP::get_instance(), "Dead oil viscosity");
    def_parameter_latex_symbol("uod", "\\mu_{od}");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR");
    def_parameter_latex_symbol("rs", "R_s");
    add_ref("perez:2001");
  }

public:
  inline Quantity<CP> impl(const Quantity<CP>& uod,
                           const Quantity<SCF_STB>& rs) const;

  Quantity<CP> operator()(const Quantity<CP>& uod,
                          const Quantity<SCF_STB>& rs) const
  {
    return impl(uod, rs);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& uod = it.get_curr();
      it.next();
      const auto& rs = it.get_curr();

      return impl(uod, rs);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& uod = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();

    return impl(uod, rs);
  }
};

extern Quantity<CP> UobGilFonseca__correlation__fct(
  const Quantity<CP>& uod, const Quantity<SCF_STB>& rs);
extern double UobGilFonseca__correlation__valid__fct(double uod, double rs);

/** UobDeGhettoEtAl


*/
class UobDeGhettoEtAl : public SaturatedOilViscosity
{
  Correlation_Singleton(UobDeGhettoEtAl);

  UobDeGhettoEtAl()
    : SaturatedOilViscosity("UobDeGhettoEtAl", CP::get_instance(), 2.1, 295.9)
  {
    set_author("De Ghetto, Paone & Villa");
    set_title("DE GHETTO, PAONE & VILLA CORRELATION FOR EXTRA-HEAVY AND HEAVY "
              "OILS (MODIFIED KARTOATMODJO CORRELATION), CALCULATION OF "
              "SATURATED OIL VISCOSITY");
    add_parameter("uod", CP::get_instance(), "Dead oil viscosity", 7.7, 1386.9);
    def_parameter_latex_symbol("uod", "\\mu_{od}");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 17.21, 640.25);
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("api", Api::get_instance(), "API oil gravity", 6, 22.3);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_db("Based on 1200 measured data points of 63 heavy and extra-heavy oil "
           "samples obtained from the Mediterranean Basin, Africa and the "
           "Persian Gulf.");
    add_db("Oil samples have been divided in two different API gravity "
           "classes: extra-heavy oils for °API<=10, heavy oils for "
           "10<°API<=22.3.");
    add_ref("deGhetto:1995");
  }

public:
  inline Quantity<CP> impl(const Quantity<CP>& uod, const Quantity<SCF_STB>& rs,
                           const Quantity<Api>& api) const;

  Quantity<CP> operator()(const Quantity<CP>& uod, const Quantity<SCF_STB>& rs,
                          const Quantity<Api>& api) const
  {
    return impl(uod, rs, api);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& uod = it.get_curr();
      it.next();
      const auto& rs = it.get_curr();
      it.next();
      const auto& api = it.get_curr();

      return impl(uod, rs, api);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& uod = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& api = it.get_curr();

    return impl(uod, rs, api);
  }
};

extern Quantity<CP> UobDeGhettoEtAl__correlation__fct(
  const Quantity<CP>& uod, const Quantity<SCF_STB>& rs,
  const Quantity<Api>& api);
extern double UobDeGhettoEtAl__correlation__valid__fct(double uod, double rs,
                                                       double api);

/** UobDindorukChristman

The range assigned for Rs was taken from the range expressed by the author for
Rsb.


*/
class UobDindorukChristman : public SaturatedOilViscosity
{
  Correlation_Singleton(UobDindorukChristman);

  UobDindorukChristman()
    : SaturatedOilViscosity("UobDindorukChristman", CP::get_instance(), 0.161,
                            8.7)
  {
    set_author("Dindoruk & Christman");
    set_title("DINDORUK & CHRISTMAN CORRELATION, CALCULATION OF SATURATED OIL "
              "VISCOSITY");
    add_parameter("uod", CP::get_instance(), "Dead oil viscosity", 0.896,
                  62.63);
    def_parameter_latex_symbol("uod", "\\mu_{od}");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 133, 3050);
    def_parameter_latex_symbol("rs", "R_s");
    add_db("Based on more than 90 PVT reports from the Gulf of Mexico.");
    add_ref("dindoruk:2004");
  }

public:
  inline Quantity<CP> impl(const Quantity<CP>& uod,
                           const Quantity<SCF_STB>& rs) const;

  Quantity<CP> operator()(const Quantity<CP>& uod,
                          const Quantity<SCF_STB>& rs) const
  {
    return impl(uod, rs);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& uod = it.get_curr();
      it.next();
      const auto& rs = it.get_curr();

      return impl(uod, rs);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& uod = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();

    return impl(uod, rs);
  }
};

extern Quantity<CP> UobDindorukChristman__correlation__fct(
  const Quantity<CP>& uod, const Quantity<SCF_STB>& rs);
extern double UobDindorukChristman__correlation__valid__fct(double uod,
                                                            double rs);

#endif // SATURATED_OIL_VISCOSITY_H
