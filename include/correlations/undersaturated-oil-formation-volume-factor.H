#ifndef UNDERSATURATED_OIL_FORMATION_VOLUME_FACTOR_H
#define UNDERSATURATED_OIL_FORMATION_VOLUME_FACTOR_H

#include <correlations/correlation.H>

/** BoaMcCain

The correlation was verified by using the original reference and Bánzer (1996)
as a secondary reference. Date: October 26 2016


*/
class BoaMcCain : public UndersaturatedOilFormationVolumeFactor
{
  Correlation_Singleton(BoaMcCain);

  BoaMcCain()
    : UndersaturatedOilFormationVolumeFactor("BoaMcCain",
                                             RB_STB::get_instance())
  {
    set_author("McCain");
    set_title("McCAIN CORRELATION, CALCULATION OF UNDERSATURATED OIL FORMATION "
              "VOLUME FACTOR");
    add_parameter("bobp", RB_STB::get_instance(),
                  "Oil formation volume factor at Pb");
    def_parameter_latex_symbol("bobp", "B_{obp}");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("pb", psia::get_instance(), "Bubble point pressure");
    def_parameter_latex_symbol("pb", "P_b");
    add_parameter("co", psia_1::get_instance(),
                  "Oil isothermal compressibility");
    def_parameter_latex_symbol("co", "c_{o}");
    add_db("Based on the equation presented by McCain (1990).");
    add_db("The volume factor at bubble point pressure is adjusted to higher "
           "pressures through the use of the coefficient of isothermal "
           "compresibility.");
    add_note("This correlation determines the oil formation volume factor for "
             "pressures above the bubble point pressure.");
    add_ref("mcCain:1990");
  }

public:
  inline Quantity<RB_STB> impl(const Quantity<RB_STB>& bobp,
                               const Quantity<psia>& p,
                               const Quantity<psia>& pb,
                               const Quantity<psia_1>& co) const;

  Quantity<RB_STB> operator()(const Quantity<RB_STB>& bobp,
                              const Quantity<psia>& p, const Quantity<psia>& pb,
                              const Quantity<psia_1>& co) const
  {
    return impl(bobp, p, pb, co);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& bobp = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& pb = it.get_curr();
      it.next();
      const auto& co = it.get_curr();

      return impl(bobp, p, pb, co);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& bobp = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();
    it.next();
    const auto& co = it.get_curr();

    return impl(bobp, p, pb, co);
  }
};

extern Quantity<RB_STB> BoaMcCain__correlation__fct(
  const Quantity<RB_STB>& bobp, const Quantity<psia>& p,
  const Quantity<psia>& pb, const Quantity<psia_1>& co);
extern double BoaMcCain__correlation__valid__fct(double bobp, double p,
                                                 double pb, double co);

/** BoaDeGhetto

De Ghetto et al. recommend the use of the Vazquez & Beggs correlation for the
estimation of Bo. The values for the coefficients c1, c2, and c3 are selected
only for API<=30.

McCain's correlation for Boa is evaluated with the Coa value obtained from the
De Ghetto, Paone & Villa correlation.


*/
class BoaDeGhetto : public UndersaturatedOilFormationVolumeFactor
{
  Correlation_Singleton(BoaDeGhetto);

  BoaDeGhetto()
    : UndersaturatedOilFormationVolumeFactor("BoaDeGhetto",
                                             RB_STB::get_instance())
  {
    set_author("De Ghetto, Paone & Villa");
    set_title("DE GHETTO, PAONE & VILLA CORRELATION FOR EXTRA-HEAVY AND HEAVY "
              "OILS (VAZQUEZ & BEGGS CORRELATION), CALCULATION OF "
              "UNDERSATURATED OIL FORMATION VOLUME FACTOR");
    set_hidden();
    add_parameter("bobp", RB_STB::get_instance(),
                  "Oil formation volume factor at Pb");
    def_parameter_latex_symbol("bobp", "B_{obp}");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.623,
                  1.517);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("api", Api::get_instance(), "API oil gravity", 6, 22.3);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 17.21,
                  640.25);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 131.4, 250.7);
    def_parameter_latex_symbol("t", "T");
    add_parameter("tsep", Fahrenheit::get_instance(), "Separator temperature",
                  59, 177.8);
    def_parameter_latex_symbol("tsep", "T_{sp}");
    add_parameter("p", psia::get_instance(), "Pressure", 1038.49, 7411.54);
    def_parameter_latex_symbol("p", "P");
    add_parameter("pb", psia::get_instance(), "Bubble point pressure");
    def_parameter_latex_symbol("pb", "P_b");
    add_parameter("psep", psia::get_instance(), "Separator pressure", 14.5,
                  752.2);
    def_parameter_latex_symbol("psep", "P_{sp}");
    add_db("Based on 1200 measured data points of 63 heavy and extra-heavy oil "
           "samples obtained from the Mediterranean Basin, Africa and the "
           "Persian Gulf.");
    add_db("Oil samples have been divided in two different API gravity "
           "classes: extra-heavy oils for °API<=10, heavy oils for "
           "10<°API<=22.3.");
    add_ref("deGhetto:1995");
  }

public:
  inline Quantity<RB_STB> impl(
    const Quantity<RB_STB>& bobp, const Quantity<Sgg>& yg,
    const Quantity<Api>& api, const Quantity<SCF_STB>& rsb,
    const Quantity<Fahrenheit>& t, const Quantity<Fahrenheit>& tsep,
    const Quantity<psia>& p, const Quantity<psia>& pb,
    const Quantity<psia>& psep) const;

  Quantity<RB_STB> operator()(const Quantity<RB_STB>& bobp,
                              const Quantity<Sgg>& yg, const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<Fahrenheit>& tsep,
                              const Quantity<psia>& p, const Quantity<psia>& pb,
                              const Quantity<psia>& psep) const
  {
    return impl(bobp, yg, api, rsb, t, tsep, p, pb, psep);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& bobp = it.get_curr();
      it.next();
      const auto& yg = it.get_curr();
      it.next();
      const auto& api = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();
      it.next();
      const auto& t = it.get_curr();
      it.next();
      const auto& tsep = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& pb = it.get_curr();
      it.next();
      const auto& psep = it.get_curr();

      return impl(bobp, yg, api, rsb, t, tsep, p, pb, psep);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& bobp = it.get_curr();
    it.next();
    const auto& yg = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& tsep = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();
    it.next();
    const auto& psep = it.get_curr();

    return impl(bobp, yg, api, rsb, t, tsep, p, pb, psep);
  }
};

extern Quantity<RB_STB> BoaDeGhetto__correlation__fct(
  const Quantity<RB_STB>& bobp, const Quantity<Sgg>& yg,
  const Quantity<Api>& api, const Quantity<SCF_STB>& rsb,
  const Quantity<Fahrenheit>& t, const Quantity<Fahrenheit>& tsep,
  const Quantity<psia>& p, const Quantity<psia>& pb,
  const Quantity<psia>& psep);
extern double BoaDeGhetto__correlation__valid__fct(double bobp, double yg,
                                                   double api, double rsb,
                                                   double t, double tsep,
                                                   double p, double pb,
                                                   double psep);

/** BoaHanafy

McCain's correlation for Boa is evaluated with the Coa value obtained from the
Hanafy et al. correlation.


*/
class BoaHanafy : public UndersaturatedOilFormationVolumeFactor
{
  Correlation_Singleton(BoaHanafy);

  BoaHanafy()
    : UndersaturatedOilFormationVolumeFactor("BoaHanafy",
                                             RB_STB::get_instance())
  {
    set_author("Hanafy et al.");
    set_title("HANAFY ET AL. CORRELATION, CALCULATION OF UNDERSATURATED OIL "
              "FORMATION VOLUME FACTOR");
    set_hidden();
    add_parameter("bobp", RB_STB::get_instance(),
                  "Oil formation volume factor at Pb");
    def_parameter_latex_symbol("bobp", "B_{obp}");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("pb", psia::get_instance(), "Bubble point pressure");
    def_parameter_latex_symbol("pb", "P_b");
    add_db("Based on experimental PVT data of 324 fluid samples taken from 176 "
           "wells located in 75 fields. This data represents 15 productive "
           "zones of 123 reservoirs distributed along three different regions "
           "of Egypt, including the Gulf of Suez, Western Desert, and Sinai.");
    add_ref("hanafy:1997");
  }

public:
  inline Quantity<RB_STB> impl(const Quantity<RB_STB>& bobp,
                               const Quantity<psia>& p,
                               const Quantity<psia>& pb) const;

  Quantity<RB_STB> operator()(const Quantity<RB_STB>& bobp,
                              const Quantity<psia>& p,
                              const Quantity<psia>& pb) const
  {
    return impl(bobp, p, pb);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& bobp = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& pb = it.get_curr();

      return impl(bobp, p, pb);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& bobp = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();

    return impl(bobp, p, pb);
  }
};

extern Quantity<RB_STB> BoaHanafy__correlation__fct(
  const Quantity<RB_STB>& bobp, const Quantity<psia>& p,
  const Quantity<psia>& pb);
extern double BoaHanafy__correlation__valid__fct(double bobp, double p,
                                                 double pb);

/** BoaKartoatmodjo

McCain's correlation for Boa is evaluated with the Coa value obtained from the
Kartoatmodjo & Schmidt correlation.


*/
class BoaKartoatmodjo : public UndersaturatedOilFormationVolumeFactor
{
  Correlation_Singleton(BoaKartoatmodjo);

  BoaKartoatmodjo()
    : UndersaturatedOilFormationVolumeFactor("BoaKartoatmodjo",
                                             RB_STB::get_instance())
  {
    set_author("Kartoatmodjo & Schmidt");
    set_title("KARTOATMODJO & SCHMIDT CORRELATION, CALCULATION OF "
              "UNDERSATURATED OIL FORMATION VOLUME FACTOR");
    set_hidden();
    add_parameter("bobp", RB_STB::get_instance(),
                  "Oil formation volume factor at Pb");
    def_parameter_latex_symbol("bobp", "B_{obp}");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.4824,
                  1.668);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(59), Quantity<Api>(14.4));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 0,
                  2890);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 75, 320);
    def_parameter_latex_symbol("t", "T");
    add_parameter("tsep", Fahrenheit::get_instance(), "Separator temperature",
                  38, 294);
    def_parameter_latex_symbol("tsep", "T_{sp}");
    add_parameter("p", psia::get_instance(), "Pressure", 24.7, 6014.7);
    def_parameter_latex_symbol("p", "P");
    add_parameter("pb", psia::get_instance(), "Bubble point pressure");
    def_parameter_latex_symbol("pb", "P_b");
    add_parameter("psep", psia::get_instance(), "Separator pressure", 14.7,
                  1414.7);
    def_parameter_latex_symbol("psep", "P_{sp}");
    add_db("Based on a set of 5392 data points, which represent 740 different "
           "crude oil samples.");
    add_db("The data bank was collected from PVT reports and literature. The "
           "first major source was from South East Asia, mainly Indonesia. The "
           "second source was North America, including the offshore area. The "
           "rest came from the Middle East and Latin America.");
    add_ref("kartoatmodjo:1991");
    add_par_synonym("yo", "api", "Api");
  }

public:
  inline Quantity<RB_STB> impl(
    const Quantity<RB_STB>& bobp, const Quantity<Sgg>& yg,
    const Quantity<Sg_do>& yo, const Quantity<SCF_STB>& rsb,
    const Quantity<Fahrenheit>& t, const Quantity<Fahrenheit>& tsep,
    const Quantity<psia>& p, const Quantity<psia>& pb,
    const Quantity<psia>& psep) const;

  Quantity<RB_STB> operator()(const Quantity<RB_STB>& bobp,
                              const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<Fahrenheit>& tsep,
                              const Quantity<psia>& p, const Quantity<psia>& pb,
                              const Quantity<psia>& psep) const
  {
    return impl(bobp, yg, yo, rsb, t, tsep, p, pb, psep);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& bobp = it.get_curr();
      it.next();
      const auto& yg = it.get_curr();
      it.next();
      const auto& yo = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();
      it.next();
      const auto& t = it.get_curr();
      it.next();
      const auto& tsep = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& pb = it.get_curr();
      it.next();
      const auto& psep = it.get_curr();

      return impl(bobp, yg, yo, rsb, t, tsep, p, pb, psep);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& bobp = it.get_curr();
    it.next();
    const auto& yg = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& tsep = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();
    it.next();
    const auto& psep = it.get_curr();

    return impl(bobp, yg, yo, rsb, t, tsep, p, pb, psep);
  }
};

extern Quantity<RB_STB> BoaKartoatmodjo__correlation__fct(
  const Quantity<RB_STB>& bobp, const Quantity<Sgg>& yg,
  const Quantity<Sg_do>& yo, const Quantity<SCF_STB>& rsb,
  const Quantity<Fahrenheit>& t, const Quantity<Fahrenheit>& tsep,
  const Quantity<psia>& p, const Quantity<psia>& pb,
  const Quantity<psia>& psep);
extern double BoaKartoatmodjo__correlation__valid__fct(double bobp, double yg,
                                                       double yo, double rsb,
                                                       double t, double tsep,
                                                       double p, double pb,
                                                       double psep);

/** BoaPetroskyFarshad

McCain's correlation for Boa is evaluated with the Coa value obtained from the
Petrosky & Farshad correlation.


*/
class BoaPetroskyFarshad : public UndersaturatedOilFormationVolumeFactor
{
  Correlation_Singleton(BoaPetroskyFarshad);

  BoaPetroskyFarshad()
    : UndersaturatedOilFormationVolumeFactor("BoaPetroskyFarshad",
                                             RB_STB::get_instance())
  {
    set_author("Petrosky & Farshad");
    set_title("PETROSKY & FARSHAD CORRELATION, CALCULATION OF UNDERSATURATED "
              "OIL FORMATION VOLUME FACTOR");
    set_hidden();
    add_parameter("bobp", RB_STB::get_instance(),
                  "Oil formation volume factor at Pb");
    def_parameter_latex_symbol("bobp", "B_{obp}");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.5781,
                  0.8519);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(45), Quantity<Api>(16.3));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 217,
                  1406);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 114, 288);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure", 1700, 10692);
    def_parameter_latex_symbol("p", "P");
    add_parameter("pb", psia::get_instance(), "Bubble point pressure");
    def_parameter_latex_symbol("pb", "P_b");
    add_db("Based on 81 laboratory PVT analyses. Fluid samples were obtained "
           "from reservoirs located offshore Texas and Louisiana.");
    add_db("Producing areas from Galveston Island, eastward, through Main Pass "
           "are represented.");
    add_db("Many Gulf of Mexico crudes can be characterized as naphthenic or "
           "biodegraded oils.");
    add_ref("petrosky:1993");
    add_par_synonym("yo", "api", "Api");
  }

public:
  inline Quantity<RB_STB> impl(const Quantity<RB_STB>& bobp,
                               const Quantity<Sgg>& yg,
                               const Quantity<Sg_do>& yo,
                               const Quantity<SCF_STB>& rsb,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<psia>& p,
                               const Quantity<psia>& pb) const;

  Quantity<RB_STB> operator()(const Quantity<RB_STB>& bobp,
                              const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<psia>& p,
                              const Quantity<psia>& pb) const
  {
    return impl(bobp, yg, yo, rsb, t, p, pb);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& bobp = it.get_curr();
      it.next();
      const auto& yg = it.get_curr();
      it.next();
      const auto& yo = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();
      it.next();
      const auto& t = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& pb = it.get_curr();

      return impl(bobp, yg, yo, rsb, t, p, pb);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& bobp = it.get_curr();
    it.next();
    const auto& yg = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();

    return impl(bobp, yg, yo, rsb, t, p, pb);
  }
};

extern Quantity<RB_STB> BoaPetroskyFarshad__correlation__fct(
  const Quantity<RB_STB>& bobp, const Quantity<Sgg>& yg,
  const Quantity<Sg_do>& yo, const Quantity<SCF_STB>& rsb,
  const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
  const Quantity<psia>& pb);
extern double BoaPetroskyFarshad__correlation__valid__fct(double bobp,
                                                          double yg, double yo,
                                                          double rsb, double t,
                                                          double p, double pb);

/** BoaVasquezBeggs

McCain's correlation for Boa is evaluated with the Coa value obtained from the
Vazquez & Beggs correlation.


*/
class BoaVasquezBeggs : public UndersaturatedOilFormationVolumeFactor
{
  Correlation_Singleton(BoaVasquezBeggs);

  BoaVasquezBeggs()
    : UndersaturatedOilFormationVolumeFactor("BoaVasquezBeggs",
                                             RB_STB::get_instance())
  {
    set_author("Vazquez & Beggs");
    set_title("VAZQUEZ & BEGGS CORRELATION, CALCULATION OF UNDERSATURATED OIL "
              "FORMATION VOLUME FACTOR");
    set_hidden();
    add_parameter("bobp", RB_STB::get_instance(),
                  "Oil formation volume factor at Pb");
    def_parameter_latex_symbol("bobp", "B_{obp}");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.511,
                  1.351);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("api", Api::get_instance(), "API oil gravity", 15.3, 59.5);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 9.3,
                  2199);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature");
    def_parameter_latex_symbol("t", "T");
    add_parameter("tsep", Fahrenheit::get_instance(), "Separator temperature",
                  76, 150);
    def_parameter_latex_symbol("tsep", "T_{sp}");
    add_parameter("p", psia::get_instance(), "Pressure", 141, 9515);
    def_parameter_latex_symbol("p", "P");
    add_parameter("pb", psia::get_instance(), "Bubble point pressure");
    def_parameter_latex_symbol("pb", "P_b");
    add_parameter("psep", psia::get_instance(), "Separator pressure", 60, 565);
    def_parameter_latex_symbol("psep", "P_{sp}");
    add_db("Based on more than 600 laboratory PVT analyses from fields all "
           "over the world.");
    add_ref("vazquez:1980");
    add_ref("alShammasi:2001");
  }

public:
  inline Quantity<RB_STB> impl(
    const Quantity<RB_STB>& bobp, const Quantity<Sgg>& yg,
    const Quantity<Api>& api, const Quantity<SCF_STB>& rsb,
    const Quantity<Fahrenheit>& t, const Quantity<Fahrenheit>& tsep,
    const Quantity<psia>& p, const Quantity<psia>& pb,
    const Quantity<psia>& psep) const;

  Quantity<RB_STB> operator()(const Quantity<RB_STB>& bobp,
                              const Quantity<Sgg>& yg, const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<Fahrenheit>& tsep,
                              const Quantity<psia>& p, const Quantity<psia>& pb,
                              const Quantity<psia>& psep) const
  {
    return impl(bobp, yg, api, rsb, t, tsep, p, pb, psep);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& bobp = it.get_curr();
      it.next();
      const auto& yg = it.get_curr();
      it.next();
      const auto& api = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();
      it.next();
      const auto& t = it.get_curr();
      it.next();
      const auto& tsep = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& pb = it.get_curr();
      it.next();
      const auto& psep = it.get_curr();

      return impl(bobp, yg, api, rsb, t, tsep, p, pb, psep);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& bobp = it.get_curr();
    it.next();
    const auto& yg = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& tsep = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();
    it.next();
    const auto& psep = it.get_curr();

    return impl(bobp, yg, api, rsb, t, tsep, p, pb, psep);
  }
};

extern Quantity<RB_STB> BoaVasquezBeggs__correlation__fct(
  const Quantity<RB_STB>& bobp, const Quantity<Sgg>& yg,
  const Quantity<Api>& api, const Quantity<SCF_STB>& rsb,
  const Quantity<Fahrenheit>& t, const Quantity<Fahrenheit>& tsep,
  const Quantity<psia>& p, const Quantity<psia>& pb,
  const Quantity<psia>& psep);
extern double BoaVasquezBeggs__correlation__valid__fct(double bobp, double yg,
                                                       double api, double rsb,
                                                       double t, double tsep,
                                                       double p, double pb,
                                                       double psep);

/** BoaPerezML

The correlation could not be verified because the original reference is not
available. Date: August 22 2016.

McCain's correlation for Boa is evaluated with the Coa value obtained from the
Pérez, Heny & Lago correlation.


*/
class BoaPerezML : public UndersaturatedOilFormationVolumeFactor
{
  Correlation_Singleton(BoaPerezML);

  BoaPerezML()
    : UndersaturatedOilFormationVolumeFactor("BoaPerezML",
                                             RB_STB::get_instance())
  {
    set_author("Pérez, Heny & Lago");
    set_title("PÉREZ, HENY & LAGO CORRELATION (EZEKWE-QUERIN-HUMPHREY "
              "CORRELATION USED BY PÉREZ, HENY & LAGO), CALCULATION OF "
              "UNDERSATURATED OIL FORMATION VOLUME FACTOR");
    set_hidden();
    add_parameter("bobp", RB_STB::get_instance(),
                  "Oil formation volume factor at Pb");
    def_parameter_latex_symbol("bobp", "B_{obp}");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.571,
                  0.981);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(12.9), Quantity<Api>(6.4));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 38,
                  121);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 112, 300);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("pb", psia::get_instance(), "Bubble point pressure", 405,
                  1335);
    def_parameter_latex_symbol("pb", "P_b");
    add_note("The correlation could not be verified because the original "
             "reference is not available. Date: August 22 2016.");
    add_ref("perez:2001");
    add_par_synonym("yo", "api", "Api");
  }

public:
  inline Quantity<RB_STB> impl(const Quantity<RB_STB>& bobp,
                               const Quantity<Sgg>& yg,
                               const Quantity<Sg_do>& yo,
                               const Quantity<SCF_STB>& rsb,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<psia>& p,
                               const Quantity<psia>& pb) const;

  Quantity<RB_STB> operator()(const Quantity<RB_STB>& bobp,
                              const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<psia>& p,
                              const Quantity<psia>& pb) const
  {
    return impl(bobp, yg, yo, rsb, t, p, pb);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& bobp = it.get_curr();
      it.next();
      const auto& yg = it.get_curr();
      it.next();
      const auto& yo = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();
      it.next();
      const auto& t = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& pb = it.get_curr();

      return impl(bobp, yg, yo, rsb, t, p, pb);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& bobp = it.get_curr();
    it.next();
    const auto& yg = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();

    return impl(bobp, yg, yo, rsb, t, p, pb);
  }
};

extern Quantity<RB_STB> BoaPerezML__correlation__fct(
  const Quantity<RB_STB>& bobp, const Quantity<Sgg>& yg,
  const Quantity<Sg_do>& yo, const Quantity<SCF_STB>& rsb,
  const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
  const Quantity<psia>& pb);
extern double BoaPerezML__correlation__valid__fct(double bobp, double yg,
                                                  double yo, double rsb,
                                                  double t, double p,
                                                  double pb);

/** BoaMillanArcia

The correlation could not be verified because the original reference is not
available. Date: August 22 2016.

McCain's correlation for Boa is evaluated with the Coa value obtained from the
Millán-Arcia correlation.


*/
class BoaMillanArcia : public UndersaturatedOilFormationVolumeFactor
{
  Correlation_Singleton(BoaMillanArcia);

  BoaMillanArcia()
    : UndersaturatedOilFormationVolumeFactor("BoaMillanArcia",
                                             RB_STB::get_instance())
  {
    set_author("Millán-Arcia");
    set_title("MILLÁN-ARCIA CORRELATION, CALCULATION OF UNDERSATURATED OIL "
              "FORMATION VOLUME FACTOR");
    set_hidden();
    add_parameter("bobp", RB_STB::get_instance(),
                  "Oil formation volume factor at Pb");
    def_parameter_latex_symbol("bobp", "B_{obp}");
    add_parameter("api", Api::get_instance(), "API oil gravity", 9.0, 20.2);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 53,
                  459);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 87, 195);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("pb", psia::get_instance(), "Bubble point pressure", 222.0,
                  3432.7);
    def_parameter_latex_symbol("pb", "P_b");
    add_db("Venezuelan heavy crudes were correlated.");
    add_ref("millan:1984");
    add_ref("perez:2001");
  }

public:
  inline Quantity<RB_STB> impl(const Quantity<RB_STB>& bobp,
                               const Quantity<Api>& api,
                               const Quantity<SCF_STB>& rsb,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<psia>& p,
                               const Quantity<psia>& pb) const;

  Quantity<RB_STB> operator()(const Quantity<RB_STB>& bobp,
                              const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<psia>& p,
                              const Quantity<psia>& pb) const
  {
    return impl(bobp, api, rsb, t, p, pb);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& bobp = it.get_curr();
      it.next();
      const auto& api = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();
      it.next();
      const auto& t = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& pb = it.get_curr();

      return impl(bobp, api, rsb, t, p, pb);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& bobp = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();

    return impl(bobp, api, rsb, t, p, pb);
  }
};

extern Quantity<RB_STB> BoaMillanArcia__correlation__fct(
  const Quantity<RB_STB>& bobp, const Quantity<Api>& api,
  const Quantity<SCF_STB>& rsb, const Quantity<Fahrenheit>& t,
  const Quantity<psia>& p, const Quantity<psia>& pb);
extern double BoaMillanArcia__correlation__valid__fct(double bobp, double api,
                                                      double rsb, double t,
                                                      double p, double pb);

#endif // UNDERSATURATED_OIL_FORMATION_VOLUME_FACTOR_H
