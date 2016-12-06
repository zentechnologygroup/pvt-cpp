#ifndef UNDERSATURATED_OIL_VISCOSITY_H
#define UNDERSATURATED_OIL_VISCOSITY_H

#include <correlations/correlation.H>

/** UoaKartoatmodjoSchmidt

Depending on the API gravity, the samples can cover three different classes of
crude oils: heavy oils for 10<°API<=22.3, medium oils for 22.3<°API<=31.1, and
light oils for °API>31.1.


*/
class UoaKartoatmodjoSchmidt : public UndersaturatedOilViscosity
{
  Correlation_Singleton(UoaKartoatmodjoSchmidt);

  UoaKartoatmodjoSchmidt()
    : UndersaturatedOilViscosity("UoaKartoatmodjoSchmidt", CP::get_instance(),
                                 0.168, 517.03)
  {
    set_author("Kartoatmodjo & Schmidt");
    set_title("KARTOATMODJO & SCHMIDT CORRELATION FOR HEAVY, MEDIUM AND LIGHT "
              "OILS, CALCULATION OF UNDERSATURATED OIL VISCOSITY");
    add_parameter("uobp", CP::get_instance(), "Oil viscosity at Pb", 0.168,
                  184.86);
    def_parameter_latex_symbol("uobp", "\\mu_{obp}");
    add_parameter("pb", psia::get_instance(), "Bubble point pressure", 24.7,
                  4764.7);
    def_parameter_latex_symbol("pb", "P_b");
    add_parameter("p", psia::get_instance(), "Pressure", 24.7, 6014.7);
    def_parameter_latex_symbol("p", "P");
    add_db("Based on a total of 3588 data points collected from 661 different "
           "crude oil samples.");
    add_db("The first major source was from South East Asia, mainly Indonesia. "
           "The second source was North America, including the offshore area. "
           "The rest came from the Middle East and Latin America.");
    add_ref("kartoatmodjo:1991");
  }

public:
  inline Quantity<CP> impl(const Quantity<CP>& uobp, const Quantity<psia>& pb,
                           const Quantity<psia>& p) const;

  Quantity<CP> operator()(const Quantity<CP>& uobp, const Quantity<psia>& pb,
                          const Quantity<psia>& p) const
  {
    return impl(uobp, pb, p);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& uobp = it.get_curr();
      it.next();
      const auto& pb = it.get_curr();
      it.next();
      const auto& p = it.get_curr();

      return impl(uobp, pb, p);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& uobp = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();
    it.next();
    const auto& p = it.get_curr();

    return impl(uobp, pb, p);
  }
};

extern Quantity<CP> UoaKartoatmodjoSchmidt__correlation__fct(
  const Quantity<CP>& uobp, const Quantity<psia>& pb, const Quantity<psia>& p);
extern double UoaKartoatmodjoSchmidt__correlation__valid__fct(double uobp,
                                                              double pb,
                                                              double p);

/** UoaDeGhettoEtAl


*/
class UoaDeGhettoEtAl : public UndersaturatedOilViscosity
{
  Correlation_Singleton(UoaDeGhettoEtAl);

  UoaDeGhettoEtAl()
    : UndersaturatedOilViscosity("UoaDeGhettoEtAl", CP::get_instance(), 2.4,
                                 354.6)
  {
    set_author("De Ghetto, Paone & Villa");
    set_title("DE GHETTO, PAONE & VILLA CORRELATION FOR EXTRA-HEAVY (MODIFIED "
              "LABEDI CORRELATION) AND HEAVY (MODIFIED KARTOATMODJO & SCHMIDT "
              "CORRELATION) OILS, CALCULATION OF UNDERSATURATED OIL VISCOSITY");
    add_parameter("uobp", CP::get_instance(), "Oil viscosity at Pb", 2.1,
                  295.9);
    def_parameter_latex_symbol("uobp", "\\mu_{obp}");
    add_parameter("p", psia::get_instance(), "Pressure", 1038.49, 7411.54);
    def_parameter_latex_symbol("p", "P");
    add_parameter("pb", psia::get_instance(), "Bubble point pressure", 208.86,
                  4021.96);
    def_parameter_latex_symbol("pb", "P_b");
    add_parameter("uod", CP::get_instance(), "Dead oil viscosity", 7.7, 1386.9);
    def_parameter_latex_symbol("uod", "\\mu_{od}");
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
  inline Quantity<CP> impl(const Quantity<CP>& uobp, const Quantity<psia>& p,
                           const Quantity<psia>& pb, const Quantity<CP>& uod,
                           const Quantity<Api>& api) const;

  Quantity<CP> operator()(const Quantity<CP>& uobp, const Quantity<psia>& p,
                          const Quantity<psia>& pb, const Quantity<CP>& uod,
                          const Quantity<Api>& api) const
  {
    return impl(uobp, p, pb, uod, api);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& uobp = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& pb = it.get_curr();
      it.next();
      const auto& uod = it.get_curr();
      it.next();
      const auto& api = it.get_curr();

      return impl(uobp, p, pb, uod, api);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& uobp = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();
    it.next();
    const auto& uod = it.get_curr();
    it.next();
    const auto& api = it.get_curr();

    return impl(uobp, p, pb, uod, api);
  }
};

extern Quantity<CP> UoaDeGhettoEtAl__correlation__fct(const Quantity<CP>& uobp,
                                                      const Quantity<psia>& p,
                                                      const Quantity<psia>& pb,
                                                      const Quantity<CP>& uod,
                                                      const Quantity<Api>& api);
extern double UoaDeGhettoEtAl__correlation__valid__fct(double uobp, double p,
                                                       double pb, double uod,
                                                       double api);

/** UoaBeal

Beal developed the graphical correlation and Standing presented the equation


*/
class UoaBeal : public UndersaturatedOilViscosity
{
  Correlation_Singleton(UoaBeal);

  UoaBeal()
    : UndersaturatedOilViscosity("UoaBeal", CP::get_instance(), 0.16, 315)
  {
    set_author("Beal");
    set_title("BEAL CORRELATION, CALCULATION OF UNDERSATURATED OIL VISCOSITY");
    add_parameter("uobp", CP::get_instance(), "Oil viscosity at Pb", 0.142,
                  127);
    def_parameter_latex_symbol("uobp", "\\mu_{obp}");
    add_parameter("p", psia::get_instance(), "Pressure", Quantity<psig>(1500),
                  Quantity<psig>(5500));
    def_parameter_latex_symbol("p", "P");
    add_parameter("pb", psia::get_instance(), "Bubble point pressure",
                  Quantity<psig>(125), Quantity<psig>(4120));
    def_parameter_latex_symbol("pb", "P_b");
    add_db("Based on 52 viscosity observations taken from 26 crude oil "
           "samples, representing 20 individual oil pools, 11 of which are in "
           "California.");
    add_db("Half of the observations include viscosity data for the "
           "undersaturated crude oil above the bubble point, and the other "
           "half are viscosity observations at the bubble point.");
    add_ref("beal:1946");
    add_ref("standing:1977");
  }

public:
  inline Quantity<CP> impl(const Quantity<CP>& uobp, const Quantity<psia>& p,
                           const Quantity<psia>& pb) const;

  Quantity<CP> operator()(const Quantity<CP>& uobp, const Quantity<psia>& p,
                          const Quantity<psia>& pb) const
  {
    return impl(uobp, p, pb);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& uobp = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& pb = it.get_curr();

      return impl(uobp, p, pb);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& uobp = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();

    return impl(uobp, p, pb);
  }
};

extern Quantity<CP> UoaBeal__correlation__fct(const Quantity<CP>& uobp,
                                              const Quantity<psia>& p,
                                              const Quantity<psia>& pb);
extern double UoaBeal__correlation__valid__fct(double uobp, double p,
                                               double pb);

/** UoaVasquezBeggs

The development ranges are presented by Bánzer, who does not specify if the
range for viscosity corresponds to uoBubble or uoa.


*/
class UoaVasquezBeggs : public UndersaturatedOilViscosity
{
  Correlation_Singleton(UoaVasquezBeggs);

  UoaVasquezBeggs()
    : UndersaturatedOilViscosity("UoaVasquezBeggs", CP::get_instance(), 0.117,
                                 148)
  {
    set_author("Vazquez & Beggs");
    set_title("VAZQUEZ & BEGGS CORRELATION, CALCULATION OF UNDERSATURATED OIL "
              "VISCOSITY");
    add_parameter("uobp", CP::get_instance(), "Oil viscosity at Pb");
    def_parameter_latex_symbol("uobp", "\\mu_{obp}");
    add_parameter("p", psia::get_instance(), "Pressure", 141, 9515);
    def_parameter_latex_symbol("p", "P");
    add_parameter("pb", psia::get_instance(), "Bubble point pressure");
    def_parameter_latex_symbol("pb", "P_b");
    add_db("Based on more than 600 laboratory PVT analyses from fields all "
           "over the world, including more than 6000 measurements of "
           "undersaturated oil viscosity at various pressures.");
    add_ref("vazquez:1980");
  }

public:
  inline Quantity<CP> impl(const Quantity<CP>& uobp, const Quantity<psia>& p,
                           const Quantity<psia>& pb) const;

  Quantity<CP> operator()(const Quantity<CP>& uobp, const Quantity<psia>& p,
                          const Quantity<psia>& pb) const
  {
    return impl(uobp, p, pb);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& uobp = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& pb = it.get_curr();

      return impl(uobp, p, pb);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& uobp = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();

    return impl(uobp, p, pb);
  }
};

extern Quantity<CP> UoaVasquezBeggs__correlation__fct(const Quantity<CP>& uobp,
                                                      const Quantity<psia>& p,
                                                      const Quantity<psia>& pb);
extern double UoaVasquezBeggs__correlation__valid__fct(double uobp, double p,
                                                       double pb);

/** UoaKhan


*/
class UoaKhan : public UndersaturatedOilViscosity
{
  Correlation_Singleton(UoaKhan);

  UoaKhan()
    : UndersaturatedOilViscosity("UoaKhan", CP::get_instance(), 0.13, 71.0)
  {
    set_author("Khan et al.");
    set_title(
      "KHAN ET AL. CORRELATION, CALCULATION OF UNDERSATURATED OIL VISCOSITY");
    add_parameter("uobp", CP::get_instance(), "Oil viscosity at Pb", 0.13,
                  17.9);
    def_parameter_latex_symbol("uobp", "\\mu_{obp}");
    add_parameter("p", psia::get_instance(), "Pressure", 14.7, 5015);
    def_parameter_latex_symbol("p", "P");
    add_parameter("pb", psia::get_instance(), "Bubble point pressure", 107,
                  4315);
    def_parameter_latex_symbol("pb", "P_b");
    add_db("Based on 75 bottom hole samples taken from 62 Saudi Arabian oil "
           "reservoirs.");
    add_db("A total of 1503 data points were used for oil viscosity above the "
           "bubble point pressure.");
    add_ref("khan:1987");
  }

public:
  inline Quantity<CP> impl(const Quantity<CP>& uobp, const Quantity<psia>& p,
                           const Quantity<psia>& pb) const;

  Quantity<CP> operator()(const Quantity<CP>& uobp, const Quantity<psia>& p,
                          const Quantity<psia>& pb) const
  {
    return impl(uobp, p, pb);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& uobp = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& pb = it.get_curr();

      return impl(uobp, p, pb);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& uobp = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();

    return impl(uobp, p, pb);
  }
};

extern Quantity<CP> UoaKhan__correlation__fct(const Quantity<CP>& uobp,
                                              const Quantity<psia>& p,
                                              const Quantity<psia>& pb);
extern double UoaKhan__correlation__valid__fct(double uobp, double p,
                                               double pb);

/** UoaPetroskyFarshad


*/
class UoaPetroskyFarshad : public UndersaturatedOilViscosity
{
  Correlation_Singleton(UoaPetroskyFarshad);

  UoaPetroskyFarshad()
    : UndersaturatedOilViscosity("UoaPetroskyFarshad", CP::get_instance(),
                                 0.224, 4.09)
  {
    set_author("Petrosky & Farshad");
    set_title("PETROSKY & FARSHAD CORRELATION, CALCULATION OF UNDERSATURATED "
              "OIL VISCOSITY");
    add_parameter("uobp", CP::get_instance(), "Oil viscosity at Pb", 0.211,
                  3.546);
    def_parameter_latex_symbol("uobp", "\\mu_{obp}");
    add_parameter("p", psia::get_instance(), "Pressure", 1600, 10250);
    def_parameter_latex_symbol("p", "P");
    add_parameter("pb", psia::get_instance(), "Bubble point pressure", 1574,
                  9552);
    def_parameter_latex_symbol("pb", "P_b");
    add_db("Based on a set of 404 data points from 126 laboratory PVT analyses "
           "of Gulf of Mexico crude oils. For the ranges of carbon dioxide and "
           "nitrogen concentration in the mixture, 126 data points were "
           "considered.");
    add_db("Fluid samples were obtained from reservoirs located offshore Texas "
           "and Louisiana. Producing areas from Galveston Island, eastward, "
           "through Main Pass are represented.");
    add_ref("petrosky:1995");
  }

public:
  inline Quantity<CP> impl(const Quantity<CP>& uobp, const Quantity<psia>& p,
                           const Quantity<psia>& pb) const;

  Quantity<CP> operator()(const Quantity<CP>& uobp, const Quantity<psia>& p,
                          const Quantity<psia>& pb) const
  {
    return impl(uobp, p, pb);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& uobp = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& pb = it.get_curr();

      return impl(uobp, p, pb);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& uobp = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();

    return impl(uobp, p, pb);
  }
};

extern Quantity<CP> UoaPetroskyFarshad__correlation__fct(
  const Quantity<CP>& uobp, const Quantity<psia>& p, const Quantity<psia>& pb);
extern double UoaPetroskyFarshad__correlation__valid__fct(double uobp, double p,
                                                          double pb);

/** UoaAbedini


*/
class UoaAbedini : public UndersaturatedOilViscosity
{
  Correlation_Singleton(UoaAbedini);

  UoaAbedini()
    : UndersaturatedOilViscosity("UoaAbedini", CP::get_instance(), 0.316,
                                 18.435)
  {
    set_author("Abedini, Abedini & Eslami");
    set_title("ABEDINI, ABEDINI & ESLAMI CORRELATION, CALCULATION OF "
              "UNDERSATURATED OIL VISCOSITY");
    add_parameter("uobp", CP::get_instance(), "Oil viscosity at Pb");
    def_parameter_latex_symbol("uobp", "\\mu_{obp}");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("pb", psia::get_instance(), "Bubble point pressure", 1340,
                  4513);
    def_parameter_latex_symbol("pb", "P_b");
    add_db("Based on PVT experimental data of five sample oils from Iranian "
           "oil reservoirs.");
    add_ref("abedini:2010");
  }

public:
  inline Quantity<CP> impl(const Quantity<CP>& uobp, const Quantity<psia>& p,
                           const Quantity<psia>& pb) const;

  Quantity<CP> operator()(const Quantity<CP>& uobp, const Quantity<psia>& p,
                          const Quantity<psia>& pb) const
  {
    return impl(uobp, p, pb);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& uobp = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& pb = it.get_curr();

      return impl(uobp, p, pb);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& uobp = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();

    return impl(uobp, p, pb);
  }
};

extern Quantity<CP> UoaAbedini__correlation__fct(const Quantity<CP>& uobp,
                                                 const Quantity<psia>& p,
                                                 const Quantity<psia>& pb);
extern double UoaAbedini__correlation__valid__fct(double uobp, double p,
                                                  double pb);

/** UoaPerezML

The correlation could not be verified because the original reference is not
available. Date: September 8 2016.


*/
class UoaPerezML : public UndersaturatedOilViscosity
{
  Correlation_Singleton(UoaPerezML);

  UoaPerezML()
    : UndersaturatedOilViscosity("UoaPerezML", CP::get_instance())
  {
    set_author("Pérez, Heny & Lago");
    set_title("PÉREZ, HENY & LAGO CORRELATION (MODIFIED LABEDI CORRELATION), "
              "CALCULATION OF UNDERSATURATED OIL VISCOSITY");
    set_hidden();
    add_parameter("uobp", CP::get_instance(), "Oil viscosity at Pb");
    def_parameter_latex_symbol("uobp", "\\mu_{obp}");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("pb", psia::get_instance(), "Bubble point pressure", 405,
                  1335);
    def_parameter_latex_symbol("pb", "P_b");
    add_ref("perez:2001");
  }

public:
  inline Quantity<CP> impl(const Quantity<CP>& uobp, const Quantity<psia>& p,
                           const Quantity<psia>& pb) const;

  Quantity<CP> operator()(const Quantity<CP>& uobp, const Quantity<psia>& p,
                          const Quantity<psia>& pb) const
  {
    return impl(uobp, p, pb);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& uobp = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& pb = it.get_curr();

      return impl(uobp, p, pb);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& uobp = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();

    return impl(uobp, p, pb);
  }
};

extern Quantity<CP> UoaPerezML__correlation__fct(const Quantity<CP>& uobp,
                                                 const Quantity<psia>& p,
                                                 const Quantity<psia>& pb);
extern double UoaPerezML__correlation__valid__fct(double uobp, double p,
                                                  double pb);

/** UoaDindorukChristman


*/
class UoaDindorukChristman : public UndersaturatedOilViscosity
{
  Correlation_Singleton(UoaDindorukChristman);

  UoaDindorukChristman()
    : UndersaturatedOilViscosity("UoaDindorukChristman", CP::get_instance(),
                                 0.211, 10.6)
  {
    set_author("Dindoruk & Christman");
    set_title("DINDORUK & CHRISTMAN CORRELATION, CALCULATION OF UNDERSATURATED "
              "OIL VISCOSITY");
    add_parameter("uobp", CP::get_instance(), "Oil viscosity at Pb", 0.161,
                  8.7);
    def_parameter_latex_symbol("uobp", "\\mu_{obp}");
    add_parameter("p", psia::get_instance(), "Pressure", 1128, 22370);
    def_parameter_latex_symbol("p", "P");
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
  inline Quantity<CP> impl(const Quantity<CP>& uobp, const Quantity<psia>& p,
                           const Quantity<psia>& pb,
                           const Quantity<SCF_STB>& rsb) const;

  Quantity<CP> operator()(const Quantity<CP>& uobp, const Quantity<psia>& p,
                          const Quantity<psia>& pb,
                          const Quantity<SCF_STB>& rsb) const
  {
    return impl(uobp, p, pb, rsb);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& uobp = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& pb = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();

      return impl(uobp, p, pb, rsb);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& uobp = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();

    return impl(uobp, p, pb, rsb);
  }
};

extern Quantity<CP> UoaDindorukChristman__correlation__fct(
  const Quantity<CP>& uobp, const Quantity<psia>& p, const Quantity<psia>& pb,
  const Quantity<SCF_STB>& rsb);
extern double UoaDindorukChristman__correlation__valid__fct(double uobp,
                                                            double p, double pb,
                                                            double rsb);

#endif // UNDERSATURATED_OIL_VISCOSITY_H
