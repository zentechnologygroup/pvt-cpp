#ifndef SATURATED_OIL_VISCOSITY_H
#define SATURATED_OIL_VISCOSITY_H

#include <pvt-exceptions.H>
#include <correlations/correlation.H>

/** UobBeggsRobinson

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group


References:

  - beggs:1975

@param[in] uod Dead oil viscosity. Range in [, ]
@param[in] rs Solution GOR. Range in [20, 2070]
@return inside [, ]
@ingroup Correlations
*/
class UobBeggsRobinson : public SaturatedOilViscosity {
  Correlation_Singleton(UobBeggsRobinson);

  UobBeggsRobinson()
      : SaturatedOilViscosity("UobBeggsRobinson", CP::get_instance()) {
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
  static inline double impl(const double& uod, const double& rs) noexcept;

  Quantity<CP> call(const Quantity<CP>& uod,
                    const Quantity<SCF_STB>& rs) const {
    return Quantity<CP>(impl(uod.raw(), rs.raw()));
  }

  Quantity<CP> validate_and_call(const Quantity<CP>& uod,
                                 const Quantity<SCF_STB>& rs) const {
    if (not(rs >= 20))
      ZENTHROW(OutOfParameterRange, "parameter rs is less than 20");
    if (not(rs <= 2070))
      ZENTHROW(OutOfParameterRange, "parameter rs is greater than 2070");

    return call(uod, rs);
  }

  Quantity<CP> operator()(const Quantity<CP>& uod,
                          const Quantity<SCF_STB>& rs) const {
    return Quantity<CP>(call(uod, rs));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& uod = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();

    return call(uod, rs);
  }
};

extern Quantity<CP> UobBeggsRobinson__correlation__fct(
    const Quantity<CP>& uod, const Quantity<SCF_STB>& rs);
extern double UobBeggsRobinson__correlation__valid__fct(const double& uod,
                                                        const double& rs);

/** UobChewConnally

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group

Chew & Connally presented the graphical correlation and the general form of the
equation.

The values of A and b of the equation are presented by Bánzer (1996) and Ahmed
(2010).

References:

  - chew:1959

  - ahmed:2010

@param[in] uod Dead oil viscosity. Range in [0.377, 100]
@param[in] rs Solution GOR. Range in [0, 1600]
@return inside [0.1, 100]
@ingroup Correlations
*/
class UobChewConnally : public SaturatedOilViscosity {
  Correlation_Singleton(UobChewConnally);

  UobChewConnally()
      : SaturatedOilViscosity("UobChewConnally", CP::get_instance(), 0.1, 100) {
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
  static inline double impl(const double& uod, const double& rs) noexcept;

  Quantity<CP> call(const Quantity<CP>& uod,
                    const Quantity<SCF_STB>& rs) const {
    return Quantity<CP>(impl(uod.raw(), rs.raw()));
  }

  Quantity<CP> validate_and_call(const Quantity<CP>& uod,
                                 const Quantity<SCF_STB>& rs) const {
    if (not(uod >= 0.377))
      ZENTHROW(OutOfParameterRange, "parameter uod is less than 0.377");
    if (not(uod <= 100))
      ZENTHROW(OutOfParameterRange, "parameter uod is greater than 100");
    if (not(rs >= 0))
      ZENTHROW(OutOfParameterRange, "parameter rs is less than 0");
    if (not(rs <= 1600))
      ZENTHROW(OutOfParameterRange, "parameter rs is greater than 1600");

    return call(uod, rs);
  }

  Quantity<CP> operator()(const Quantity<CP>& uod,
                          const Quantity<SCF_STB>& rs) const {
    return Quantity<CP>(call(uod, rs));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& uod = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();

    return call(uod, rs);
  }
};

extern Quantity<CP> UobChewConnally__correlation__fct(
    const Quantity<CP>& uod, const Quantity<SCF_STB>& rs);
extern double UobChewConnally__correlation__valid__fct(const double& uod,
                                                       const double& rs);

/** UobKhan

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group


References:

  - khan:1987

@param[in] rsb Solution GOR at Pb. Range in [24, 1901]
@param[in] yo Oil specific gravity. Range in [Quantity<Api>(44.6),
Quantity<Api>(14.3)]
@param[in] yg Gas specific gravity. Range in [0.752, 1.367]
@param[in] t Temperature. Range in [75, 240]
@param[in] p Pressure. Range in [14.7, 5015]
@param[in] pb Bubble point pressure. Range in [107, 4315]
@return inside [0.13, 77.4]
@ingroup Correlations
*/
class UobKhan : public SaturatedOilViscosity {
  Correlation_Singleton(UobKhan);

  UobKhan() : SaturatedOilViscosity("UobKhan", CP::get_instance(), 0.13, 77.4) {
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

  inline void precondition(const Quantity<psia>& p,
                           const Quantity<psia>& pb) const;

public:
  static inline double impl(const double& rsb, const double& yo,
                            const double& yg, const double& t, const double& p,
                            const double& pb) noexcept;

  Quantity<CP> call(const Quantity<SCF_STB>& rsb, const Quantity<Sg_do>& yo,
                    const Quantity<Sgg>& yg, const Quantity<Fahrenheit>& t,
                    const Quantity<psia>& p, const Quantity<psia>& pb) const {
    precondition(p, pb);
    return Quantity<CP>(
        impl(rsb.raw(), yo.raw(), yg.raw(), t.raw(), p.raw(), pb.raw()));
  }

  Quantity<CP> validate_and_call(const Quantity<SCF_STB>& rsb,
                                 const Quantity<Sg_do>& yo,
                                 const Quantity<Sgg>& yg,
                                 const Quantity<Fahrenheit>& t,
                                 const Quantity<psia>& p,
                                 const Quantity<psia>& pb) const {
    if (not(rsb >= 24))
      ZENTHROW(OutOfParameterRange, "parameter rsb is less than 24");
    if (not(rsb <= 1901))
      ZENTHROW(OutOfParameterRange, "parameter rsb is greater than 1901");
    if (not(yo >= Quantity<Api>(44.6)))
      ZENTHROW(OutOfParameterRange,
               "parameter yo is less than Quantity<Api>(44.6)");
    if (not(yo <= Quantity<Api>(14.3)))
      ZENTHROW(OutOfParameterRange,
               "parameter yo is greater than Quantity<Api>(14.3)");
    if (not(yg >= 0.752))
      ZENTHROW(OutOfParameterRange, "parameter yg is less than 0.752");
    if (not(yg <= 1.367))
      ZENTHROW(OutOfParameterRange, "parameter yg is greater than 1.367");
    if (not(pb >= 107))
      ZENTHROW(OutOfParameterRange, "parameter pb is less than 107");
    if (not(pb <= 4315))
      ZENTHROW(OutOfParameterRange, "parameter pb is greater than 4315");

    return call(rsb, yo, yg, t, p, pb);
  }

  Quantity<CP> operator()(const Quantity<SCF_STB>& rsb,
                          const Quantity<Sg_do>& yo, const Quantity<Sgg>& yg,
                          const Quantity<Fahrenheit>& t,
                          const Quantity<psia>& p,
                          const Quantity<psia>& pb) const {
    return Quantity<CP>(call(rsb, yo, yg, t, p, pb));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

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

    return call(rsb, yo, yg, t, p, pb);
  }
};

extern Quantity<CP> UobKhan__correlation__fct(const Quantity<SCF_STB>& rsb,
                                              const Quantity<Sg_do>& yo,
                                              const Quantity<Sgg>& yg,
                                              const Quantity<Fahrenheit>& t,
                                              const Quantity<psia>& p,
                                              const Quantity<psia>& pb);
extern double UobKhan__correlation__valid__fct(const double& rsb,
                                               const double& yo,
                                               const double& yg,
                                               const double& t, const double& p,
                                               const double& pb);

/** UobKartoatmodjoSchmidt

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group


Depending on the API gravity, the samples can cover three different classes of
crude oils: heavy oils for 10<°API<=22.3, medium oils for 22.3<°API<=31.1, and
light oils for °API>31.1.

References:

  - kartoatmodjo:1991

@param[in] uod Dead oil viscosity. Range in [0.506, 682.0]
@param[in] rs Solution GOR. Range in [0, 2890]
@return inside [0.097, 586]
@ingroup Correlations
*/
class UobKartoatmodjoSchmidt : public SaturatedOilViscosity {
  Correlation_Singleton(UobKartoatmodjoSchmidt);

  UobKartoatmodjoSchmidt()
      : SaturatedOilViscosity("UobKartoatmodjoSchmidt", CP::get_instance(),
                              0.097, 586) {
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
  static inline double impl(const double& uod, const double& rs) noexcept;

  Quantity<CP> call(const Quantity<CP>& uod,
                    const Quantity<SCF_STB>& rs) const {
    return Quantity<CP>(impl(uod.raw(), rs.raw()));
  }

  Quantity<CP> validate_and_call(const Quantity<CP>& uod,
                                 const Quantity<SCF_STB>& rs) const {
    if (not(uod >= 0.506))
      ZENTHROW(OutOfParameterRange, "parameter uod is less than 0.506");
    if (not(uod <= 682.0))
      ZENTHROW(OutOfParameterRange, "parameter uod is greater than 682.0");
    if (not(rs >= 0))
      ZENTHROW(OutOfParameterRange, "parameter rs is less than 0");
    if (not(rs <= 2890))
      ZENTHROW(OutOfParameterRange, "parameter rs is greater than 2890");

    return call(uod, rs);
  }

  Quantity<CP> operator()(const Quantity<CP>& uod,
                          const Quantity<SCF_STB>& rs) const {
    return Quantity<CP>(call(uod, rs));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& uod = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();

    return call(uod, rs);
  }
};

extern Quantity<CP> UobKartoatmodjoSchmidt__correlation__fct(
    const Quantity<CP>& uod, const Quantity<SCF_STB>& rs);
extern double UobKartoatmodjoSchmidt__correlation__valid__fct(const double& uod,
                                                              const double& rs);

/** UobPetroskyFarshad

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group


References:

  - petrosky:1995

@param[in] uod Dead oil viscosity. Range in [0.725, 11.69]
@param[in] rs Solution GOR. Range in [21, 1885]
@return inside [0.211, 7.403]
@ingroup Correlations
*/
class UobPetroskyFarshad : public SaturatedOilViscosity {
  Correlation_Singleton(UobPetroskyFarshad);

  UobPetroskyFarshad()
      : SaturatedOilViscosity("UobPetroskyFarshad", CP::get_instance(), 0.211,
                              7.403) {
    set_author("Petrosky & Farshad");
    set_title("PETROSKY & FARSHAD CORRELATION, CALCULATION OF SATURATED OIL "
              "VISCOSITY");
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
  static inline double impl(const double& uod, const double& rs) noexcept;

  Quantity<CP> call(const Quantity<CP>& uod,
                    const Quantity<SCF_STB>& rs) const {
    return Quantity<CP>(impl(uod.raw(), rs.raw()));
  }

  Quantity<CP> validate_and_call(const Quantity<CP>& uod,
                                 const Quantity<SCF_STB>& rs) const {
    if (not(uod >= 0.725))
      ZENTHROW(OutOfParameterRange, "parameter uod is less than 0.725");
    if (not(uod <= 11.69))
      ZENTHROW(OutOfParameterRange, "parameter uod is greater than 11.69");
    if (not(rs >= 21))
      ZENTHROW(OutOfParameterRange, "parameter rs is less than 21");
    if (not(rs <= 1885))
      ZENTHROW(OutOfParameterRange, "parameter rs is greater than 1885");

    return call(uod, rs);
  }

  Quantity<CP> operator()(const Quantity<CP>& uod,
                          const Quantity<SCF_STB>& rs) const {
    return Quantity<CP>(call(uod, rs));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& uod = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();

    return call(uod, rs);
  }
};

extern Quantity<CP> UobPetroskyFarshad__correlation__fct(
    const Quantity<CP>& uod, const Quantity<SCF_STB>& rs);
extern double UobPetroskyFarshad__correlation__valid__fct(const double& uod,
                                                          const double& rs);

/** UobPerezML

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group

Pérez, Heny & Lago correlation, calculation of saturated oil viscosity.

The correlation was verified by using the original reference (Pérez, Heny &
Lago, 2001). Date: October 02 2017.

References:

  - perez:2001

@param[in] uod Dead oil viscosity. Range in [, ]
@param[in] rs Solution GOR. Range in [38, 121]
@return inside [, ]
@ingroup Correlations
*/
class UobPerezML : public SaturatedOilViscosity {
  Correlation_Singleton(UobPerezML);

  UobPerezML() : SaturatedOilViscosity("UobPerezML", CP::get_instance()) {
    set_author("Pérez, Heny & Lago");
    set_title("PÉREZ, HENY & LAGO CORRELATION, CALCULATION OF SATURATED OIL "
              "VISCOSITY");
    add_parameter("uod", CP::get_instance(), "Dead oil viscosity");
    def_parameter_latex_symbol("uod", "\\mu_{od}");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 38, 121);
    def_parameter_latex_symbol("rs", "R_s");
    add_note("Pérez, Heny & Lago correlation, calculation of saturated oil "
             "viscosity.");
    add_ref("perez:2001");
  }

public:
  static inline double impl(const double& uod, const double& rs) noexcept;

  Quantity<CP> call(const Quantity<CP>& uod,
                    const Quantity<SCF_STB>& rs) const {
    return Quantity<CP>(impl(uod.raw(), rs.raw()));
  }

  Quantity<CP> validate_and_call(const Quantity<CP>& uod,
                                 const Quantity<SCF_STB>& rs) const {
    if (not(rs >= 38))
      ZENTHROW(OutOfParameterRange, "parameter rs is less than 38");
    if (not(rs <= 121))
      ZENTHROW(OutOfParameterRange, "parameter rs is greater than 121");

    return call(uod, rs);
  }

  Quantity<CP> operator()(const Quantity<CP>& uod,
                          const Quantity<SCF_STB>& rs) const {
    return Quantity<CP>(call(uod, rs));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& uod = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();

    return call(uod, rs);
  }
};

extern Quantity<CP> UobPerezML__correlation__fct(const Quantity<CP>& uod,
                                                 const Quantity<SCF_STB>& rs);
extern double UobPerezML__correlation__valid__fct(const double& uod,
                                                  const double& rs);

/** UobGilFonseca

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group


The correlation could not be verified because the original and secondary
references are not available. Date: September 6 2016.

References:

  - perez:2001

@param[in] uod Dead oil viscosity. Range in [, ]
@param[in] rs Solution GOR. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class UobGilFonseca : public SaturatedOilViscosity {
  Correlation_Singleton(UobGilFonseca);

  UobGilFonseca() : SaturatedOilViscosity("UobGilFonseca", CP::get_instance()) {
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
  static inline double impl(const double& uod, const double& rs) noexcept;

  Quantity<CP> call(const Quantity<CP>& uod,
                    const Quantity<SCF_STB>& rs) const {
    return Quantity<CP>(impl(uod.raw(), rs.raw()));
  }

  Quantity<CP> validate_and_call(const Quantity<CP>& uod,
                                 const Quantity<SCF_STB>& rs) const {

    return call(uod, rs);
  }

  Quantity<CP> operator()(const Quantity<CP>& uod,
                          const Quantity<SCF_STB>& rs) const {
    return Quantity<CP>(call(uod, rs));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& uod = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();

    return call(uod, rs);
  }
};

extern Quantity<CP> UobGilFonseca__correlation__fct(
    const Quantity<CP>& uod, const Quantity<SCF_STB>& rs);
extern double UobGilFonseca__correlation__valid__fct(const double& uod,
                                                     const double& rs);

/** UobDeGhettoEtAl

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group


References:

  - deGhetto:1995

@param[in] uod Dead oil viscosity. Range in [7.7, 1386.9]
@param[in] rs Solution GOR. Range in [17.21, 640.25]
@param[in] api API oil gravity. Range in [6, 22.3]
@return inside [2.1, 295.9]
@ingroup Correlations
*/
class UobDeGhettoEtAl : public SaturatedOilViscosity {
  Correlation_Singleton(UobDeGhettoEtAl);

  UobDeGhettoEtAl()
      : SaturatedOilViscosity("UobDeGhettoEtAl", CP::get_instance(), 2.1,
                              295.9) {
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
  static inline double impl(const double& uod, const double& rs,
                            const double& api) noexcept;

  Quantity<CP> call(const Quantity<CP>& uod, const Quantity<SCF_STB>& rs,
                    const Quantity<Api>& api) const {
    return Quantity<CP>(impl(uod.raw(), rs.raw(), api.raw()));
  }

  Quantity<CP> validate_and_call(const Quantity<CP>& uod,
                                 const Quantity<SCF_STB>& rs,
                                 const Quantity<Api>& api) const {
    if (not(uod >= 7.7))
      ZENTHROW(OutOfParameterRange, "parameter uod is less than 7.7");
    if (not(uod <= 1386.9))
      ZENTHROW(OutOfParameterRange, "parameter uod is greater than 1386.9");
    if (not(rs >= 17.21))
      ZENTHROW(OutOfParameterRange, "parameter rs is less than 17.21");
    if (not(rs <= 640.25))
      ZENTHROW(OutOfParameterRange, "parameter rs is greater than 640.25");
    if (not(api >= 6))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 6");
    if (not(api <= 22.3))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 22.3");

    return call(uod, rs, api);
  }

  Quantity<CP> operator()(const Quantity<CP>& uod, const Quantity<SCF_STB>& rs,
                          const Quantity<Api>& api) const {
    return Quantity<CP>(call(uod, rs, api));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& uod = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& api = it.get_curr();

    return call(uod, rs, api);
  }
};

extern Quantity<CP> UobDeGhettoEtAl__correlation__fct(
    const Quantity<CP>& uod, const Quantity<SCF_STB>& rs,
    const Quantity<Api>& api);
extern double UobDeGhettoEtAl__correlation__valid__fct(const double& uod,
                                                       const double& rs,
                                                       const double& api);

/** UobDindorukChristman

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group


The range assigned for Rs was taken from the range expressed by the author for
Rsb.

References:

  - dindoruk:2004

@param[in] uod Dead oil viscosity. Range in [0.896, 62.63]
@param[in] rs Solution GOR. Range in [133, 3050]
@return inside [0.161, 8.7]
@ingroup Correlations
*/
class UobDindorukChristman : public SaturatedOilViscosity {
  Correlation_Singleton(UobDindorukChristman);

  UobDindorukChristman()
      : SaturatedOilViscosity("UobDindorukChristman", CP::get_instance(), 0.161,
                              8.7) {
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
  static inline double impl(const double& uod, const double& rs) noexcept;

  Quantity<CP> call(const Quantity<CP>& uod,
                    const Quantity<SCF_STB>& rs) const {
    return Quantity<CP>(impl(uod.raw(), rs.raw()));
  }

  Quantity<CP> validate_and_call(const Quantity<CP>& uod,
                                 const Quantity<SCF_STB>& rs) const {
    if (not(uod >= 0.896))
      ZENTHROW(OutOfParameterRange, "parameter uod is less than 0.896");
    if (not(uod <= 62.63))
      ZENTHROW(OutOfParameterRange, "parameter uod is greater than 62.63");
    if (not(rs >= 133))
      ZENTHROW(OutOfParameterRange, "parameter rs is less than 133");
    if (not(rs <= 3050))
      ZENTHROW(OutOfParameterRange, "parameter rs is greater than 3050");

    return call(uod, rs);
  }

  Quantity<CP> operator()(const Quantity<CP>& uod,
                          const Quantity<SCF_STB>& rs) const {
    return Quantity<CP>(call(uod, rs));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& uod = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();

    return call(uod, rs);
  }
};

extern Quantity<CP> UobDindorukChristman__correlation__fct(
    const Quantity<CP>& uod, const Quantity<SCF_STB>& rs);
extern double UobDindorukChristman__correlation__valid__fct(const double& uod,
                                                            const double& rs);

/** UobLopezCR

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group


The correlation was verified by using the original reference. Date: April 27
2017.

References:

  - lopez:2005

  - lopez:2014

@param[in] rs Solution GOR. Range in [10, 115]
@param[in] api API oil gravity. Range in [9.1, 14.5]
@param[in] t Temperature. Range in [130, 142]
@param[in] p Pressure. Range in [115, 1250]
@return inside [100, 1800]
@ingroup Correlations
*/
class UobLopezCR : public SaturatedOilViscosity {
  Correlation_Singleton(UobLopezCR);

  UobLopezCR()
      : SaturatedOilViscosity("UobLopezCR", CP::get_instance(), 100, 1800) {
    set_author("López, Castillo & Rojas");
    set_title("LÓPEZ, CASTILLO & ROJAS CORRELATION, CALCULATION OF SATURATED "
              "OIL VISCOSITY FOR LOWER TEMPERATURES");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 10, 115);
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("api", Api::get_instance(), "API oil gravity", 9.1, 14.5);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 130, 142);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure", 115, 1250);
    def_parameter_latex_symbol("p", "P");
    add_db("Based on 48 PVT tests from saturated oil reservoirs located in "
           "Bare and Arecuna fields, from the Ayacucho Area of the Faja "
           "Petrolífera del Orinoco.");
    add_ref("lopez:2005");
    add_ref("lopez:2014");
  }

public:
  static inline double impl(const double& rs, const double& api,
                            const double& t, const double& p) noexcept;

  Quantity<CP> call(const Quantity<SCF_STB>& rs, const Quantity<Api>& api,
                    const Quantity<Fahrenheit>& t,
                    const Quantity<psia>& p) const {
    return Quantity<CP>(impl(rs.raw(), api.raw(), t.raw(), p.raw()));
  }

  Quantity<CP> validate_and_call(const Quantity<SCF_STB>& rs,
                                 const Quantity<Api>& api,
                                 const Quantity<Fahrenheit>& t,
                                 const Quantity<psia>& p) const {
    if (not(rs >= 10))
      ZENTHROW(OutOfParameterRange, "parameter rs is less than 10");
    if (not(rs <= 115))
      ZENTHROW(OutOfParameterRange, "parameter rs is greater than 115");
    if (not(api >= 9.1))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 9.1");
    if (not(api <= 14.5))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 14.5");

    return call(rs, api, t, p);
  }

  Quantity<CP> operator()(const Quantity<SCF_STB>& rs, const Quantity<Api>& api,
                          const Quantity<Fahrenheit>& t,
                          const Quantity<psia>& p) const {
    return Quantity<CP>(call(rs, api, t, p));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& rs = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();

    return call(rs, api, t, p);
  }
};

extern Quantity<CP> UobLopezCR__correlation__fct(const Quantity<SCF_STB>& rs,
                                                 const Quantity<Api>& api,
                                                 const Quantity<Fahrenheit>& t,
                                                 const Quantity<psia>& p);
extern double UobLopezCR__correlation__valid__fct(const double& rs,
                                                  const double& api,
                                                  const double& t,
                                                  const double& p);

/** UobLopezCRHigherT

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group


The correlation was verified by using the original reference. Date: April 27
2017.

It was decided to eliminate this correlation because it exhibits non physical
trends with respect to temperature and its resulting viscosity values are much
lower than those reported by the authors.

References:

  - lopez:2005

  - lopez:2014

@param[in] rs Solution GOR. Range in [16, 94]
@param[in] api API oil gravity. Range in [7.7, 10.3]
@param[in] t Temperature. Range in [154, 220]
@param[in] p Pressure. Range in [215, 1215]
@return inside [100, 1125]
@ingroup Correlations
*/
class UobLopezCRHigherT : public SaturatedOilViscosity {
  Correlation_Singleton(UobLopezCRHigherT);

  UobLopezCRHigherT()
      : SaturatedOilViscosity("UobLopezCRHigherT", CP::get_instance(), 100,
                              1125) {
    set_author("López, Castillo & Rojas (Higher T)");
    set_title("LÓPEZ, CASTILLO & ROJAS CORRELATION, CALCULATION OF SATURATED "
              "OIL VISCOSITY FOR HIGHER TEMPERATURES");
    set_hidden();
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 16, 94);
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("api", Api::get_instance(), "API oil gravity", 7.7, 10.3);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 154, 220);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure", 215, 1215);
    def_parameter_latex_symbol("p", "P");
    add_db("Based on 48 PVT tests from saturated oil reservoirs located in "
           "Bare and Arecuna fields, from the Ayacucho Area of the Faja "
           "Petrolífera del Orinoco.");
    add_ref("lopez:2005");
    add_ref("lopez:2014");
  }

public:
  static inline double impl(const double& rs, const double& api,
                            const double& t, const double& p) noexcept;

  Quantity<CP> call(const Quantity<SCF_STB>& rs, const Quantity<Api>& api,
                    const Quantity<Fahrenheit>& t,
                    const Quantity<psia>& p) const {
    return Quantity<CP>(impl(rs.raw(), api.raw(), t.raw(), p.raw()));
  }

  Quantity<CP> validate_and_call(const Quantity<SCF_STB>& rs,
                                 const Quantity<Api>& api,
                                 const Quantity<Fahrenheit>& t,
                                 const Quantity<psia>& p) const {
    if (not(rs >= 16))
      ZENTHROW(OutOfParameterRange, "parameter rs is less than 16");
    if (not(rs <= 94))
      ZENTHROW(OutOfParameterRange, "parameter rs is greater than 94");
    if (not(api >= 7.7))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 7.7");
    if (not(api <= 10.3))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 10.3");

    return call(rs, api, t, p);
  }

  Quantity<CP> operator()(const Quantity<SCF_STB>& rs, const Quantity<Api>& api,
                          const Quantity<Fahrenheit>& t,
                          const Quantity<psia>& p) const {
    return Quantity<CP>(call(rs, api, t, p));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& rs = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();

    return call(rs, api, t, p);
  }
};

extern Quantity<CP> UobLopezCRHigherT__correlation__fct(
    const Quantity<SCF_STB>& rs, const Quantity<Api>& api,
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p);
extern double UobLopezCRHigherT__correlation__valid__fct(const double& rs,
                                                         const double& api,
                                                         const double& t,
                                                         const double& p);

/** UobSalazar

    Generated at 2017-10-30 16:49:07 -0400

    Zen Technologies Group


The correlation was verified by using the original reference. Date: May 2 2017.

References:

  - salazar:2008

@param[in] bob Saturated oil volume factor. Range in [1.0255, 1.0837]
@param[in] api API oil gravity. Range in [7.8, 14.9]
@param[in] t Temperature. Range in [118, 150]
@param[in] p Pressure. Range in [115, 1815]
@return inside [109, 5040]
@ingroup Correlations
*/
class UobSalazar : public SaturatedOilViscosity {
  Correlation_Singleton(UobSalazar);

  UobSalazar()
      : SaturatedOilViscosity("UobSalazar", CP::get_instance(), 109, 5040) {
    set_author("Salazar");
    set_title("SALAZAR CORRELATION, CALCULATION OF SATURATED OIL VISCOSITY");
    add_parameter("bob", RB_STB::get_instance(), "Saturated oil volume factor",
                  1.0255, 1.0837);
    def_parameter_latex_symbol("bob", "B_{ob}");
    add_parameter("api", Api::get_instance(), "API oil gravity", 7.8, 14.9);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 118, 150);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure", 115, 1815);
    def_parameter_latex_symbol("p", "P");
    add_db("Based on 136 PVT reports from different areas of the Faja "
           "Petrolífera del Orinoco: 60 from Ayacucho, 48 from Junín, 26 from "
           "Carabobo, and 2 from Boyacá.");
    add_ref("salazar:2008");
  }

public:
  static inline double impl(const double& bob, const double& api,
                            const double& t, const double& p) noexcept;

  Quantity<CP> call(const Quantity<RB_STB>& bob, const Quantity<Api>& api,
                    const Quantity<Fahrenheit>& t,
                    const Quantity<psia>& p) const {
    return Quantity<CP>(impl(bob.raw(), api.raw(), t.raw(), p.raw()));
  }

  Quantity<CP> validate_and_call(const Quantity<RB_STB>& bob,
                                 const Quantity<Api>& api,
                                 const Quantity<Fahrenheit>& t,
                                 const Quantity<psia>& p) const {
    if (not(bob >= 1.0255))
      ZENTHROW(OutOfParameterRange, "parameter bob is less than 1.0255");
    if (not(bob <= 1.0837))
      ZENTHROW(OutOfParameterRange, "parameter bob is greater than 1.0837");
    if (not(api >= 7.8))
      ZENTHROW(OutOfParameterRange, "parameter api is less than 7.8");
    if (not(api <= 14.9))
      ZENTHROW(OutOfParameterRange, "parameter api is greater than 14.9");

    return call(bob, api, t, p);
  }

  Quantity<CP> operator()(const Quantity<RB_STB>& bob, const Quantity<Api>& api,
                          const Quantity<Fahrenheit>& t,
                          const Quantity<psia>& p) const {
    return Quantity<CP>(call(bob, api, t, p));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& bob = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();

    return call(bob, api, t, p);
  }
};

extern Quantity<CP> UobSalazar__correlation__fct(const Quantity<RB_STB>& bob,
                                                 const Quantity<Api>& api,
                                                 const Quantity<Fahrenheit>& t,
                                                 const Quantity<psia>& p);
extern double UobSalazar__correlation__valid__fct(const double& bob,
                                                  const double& api,
                                                  const double& t,
                                                  const double& p);

#endif // SATURATED_OIL_VISCOSITY_H
