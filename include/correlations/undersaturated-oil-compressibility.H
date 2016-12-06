#ifndef UNDERSATURATED_OIL_COMPRESSIBILITY_H
#define UNDERSATURATED_OIL_COMPRESSIBILITY_H

#include <correlations/correlation.H>

/** CoaDeGhetto

The range for Rsb is presented by De Ghetto et al. as the range for Solution
GOR.


*/
class CoaDeGhetto : public UndersaturatedOilCompressibility
{
  Correlation_Singleton(CoaDeGhetto);

  CoaDeGhetto()
    : UndersaturatedOilCompressibility("CoaDeGhetto", psia_1::get_instance(),
                                       3.02e-06, 4.29e-05)
  {
    set_author("De Ghetto, Paone & Villa");
    set_title("DE GHETTO, PAONE & VILLA CORRELATION FOR EXTRA-HEAVY AND HEAVY "
              "OILS (MODIFIED VASQUEZ & BEGGS CORRELATION), CALCULATION OF "
              "UNDERSATURATED OIL ISOTHERMAL COMPRESSIBILITY");
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
  inline Quantity<psia_1> impl(const Quantity<Sgg>& yg,
                               const Quantity<Api>& api,
                               const Quantity<SCF_STB>& rsb,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<Fahrenheit>& tsep,
                               const Quantity<psia>& p,
                               const Quantity<psia>& psep) const;

  Quantity<psia_1> operator()(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<Fahrenheit>& tsep,
                              const Quantity<psia>& p,
                              const Quantity<psia>& psep) const
  {
    return impl(yg, api, rsb, t, tsep, p, psep);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
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
      const auto& psep = it.get_curr();

      return impl(yg, api, rsb, t, tsep, p, psep);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
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
    const auto& psep = it.get_curr();

    return impl(yg, api, rsb, t, tsep, p, psep);
  }
};

extern Quantity<psia_1> CoaDeGhetto__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<Api>& api,
  const Quantity<SCF_STB>& rsb, const Quantity<Fahrenheit>& t,
  const Quantity<Fahrenheit>& tsep, const Quantity<psia>& p,
  const Quantity<psia>& psep);
extern double CoaDeGhetto__correlation__valid__fct(double yg, double api,
                                                   double rsb, double t,
                                                   double tsep, double p,
                                                   double psep);

/** CoaHanafy

The range for Rsb is presented by Hanafy et al. as the range for Rs.


*/
class CoaHanafy : public UndersaturatedOilCompressibility
{
  Correlation_Singleton(CoaHanafy);

  CoaHanafy()
    : UndersaturatedOilCompressibility("CoaHanafy", psia_1::get_instance(),
                                       4.46e-06, 6.268e-05)
  {
    set_author("Hanafy et al.");
    set_title("HANAFY ET AL. CORRELATION, CALCULATION OF UNDERSATURATED OIL "
              "ISOTHERMAL COMPRESSIBILITY");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 7,
                  4272);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_db("Based on experimental PVT data of 324 fluid samples taken from 176 "
           "wells located in 75 fields. This data represents 15 productive "
           "zones of 123 reservoirs distributed along three different regions "
           "of Egypt, including the Gulf of Suez, Western Desert, and Sinai.");
    add_ref("hanafy:1997");
  }

public:
  inline Quantity<psia_1> impl(const Quantity<SCF_STB>& rsb) const;

  Quantity<psia_1> operator()(const Quantity<SCF_STB>& rsb) const
  {
    return impl(rsb);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& rsb = it.get_curr();

      return impl(rsb);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& rsb = it.get_curr();

    return impl(rsb);
  }
};

extern Quantity<psia_1> CoaHanafy__correlation__fct(
  const Quantity<SCF_STB>& rsb);
extern double CoaHanafy__correlation__valid__fct(double rsb);

/** CoaKartoatmodjoSchmidt

The range for Rsb is presented by Kartoatmodjo and Schmidt as the range for Rsf
(flash gas-oil ratio expressed as cubic feet of separator and stock tank gas at
any pressure less than the bubble point pressure in one stock tank barrel oil).

In the original reference, the pressure is a multiplicative variable. However,
an error was detected in the order of magnitude when testing (in the equation
the pressure value must divide instead of multiply). See Bánzer (1996).


*/
class CoaKartoatmodjoSchmidt : public UndersaturatedOilCompressibility
{
  Correlation_Singleton(CoaKartoatmodjoSchmidt);

  CoaKartoatmodjoSchmidt()
    : UndersaturatedOilCompressibility("CoaKartoatmodjoSchmidt",
                                       psia_1::get_instance())
  {
    set_author("Kartoatmodjo & Schmidt");
    set_title("KARTOATMODJO & SCHMIDT CORRELATION, CALCULATION OF "
              "UNDERSATURATED OIL ISOTHERMAL COMPRESSIBILITY");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.4824,
                  1.668);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("api", Api::get_instance(), "API oil gravity", 14.4, 59);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
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
    add_parameter("psep", psia::get_instance(), "Separator pressure", 14.7,
                  1414.7);
    def_parameter_latex_symbol("psep", "P_{sp}");
    add_db("Based on a total of 3588 data points collected from 661 different "
           "crude oil samples.");
    add_db("The data bank was collected from PVT reports and literature. The "
           "first major source was from South East Asia, mainly Indonesia. The "
           "second source was North America, including the offshore area. The "
           "rest came from the Middle East and Latin America.");
    add_ref("kartoatmodjo:1991");
    add_ref("kartoatmodjo:1994");
  }

public:
  inline Quantity<psia_1> impl(const Quantity<Sgg>& yg,
                               const Quantity<Api>& api,
                               const Quantity<SCF_STB>& rsb,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<Fahrenheit>& tsep,
                               const Quantity<psia>& p,
                               const Quantity<psia>& psep) const;

  Quantity<psia_1> operator()(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<Fahrenheit>& tsep,
                              const Quantity<psia>& p,
                              const Quantity<psia>& psep) const
  {
    return impl(yg, api, rsb, t, tsep, p, psep);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
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
      const auto& psep = it.get_curr();

      return impl(yg, api, rsb, t, tsep, p, psep);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
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
    const auto& psep = it.get_curr();

    return impl(yg, api, rsb, t, tsep, p, psep);
  }
};

extern Quantity<psia_1> CoaKartoatmodjoSchmidt__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<Api>& api,
  const Quantity<SCF_STB>& rsb, const Quantity<Fahrenheit>& t,
  const Quantity<Fahrenheit>& tsep, const Quantity<psia>& p,
  const Quantity<psia>& psep);
extern double CoaKartoatmodjoSchmidt__correlation__valid__fct(
  double yg, double api, double rsb, double t, double tsep, double p,
  double psep);

/** CoaPetroskyFarshad

The range for Rsb is presented by Petrosky & Farshad as the range for solution
GOR (Rs).


*/
class CoaPetroskyFarshad : public UndersaturatedOilCompressibility
{
  Correlation_Singleton(CoaPetroskyFarshad);

  CoaPetroskyFarshad()
    : UndersaturatedOilCompressibility(
        "CoaPetroskyFarshad", psia_1::get_instance(), 3.507e-06, 2.464e-05)
  {
    set_author("Petrosky & Farshad");
    set_title("PETROSKY & FARSHAD CORRELATION, CALCULATION OF UNDERSATURATED "
              "OIL ISOTHERMAL COMPRESSIBILITY");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.5781,
                  0.8519);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("api", Api::get_instance(), "API oil gravity", 16.3, 45);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 217,
                  1406);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 114, 288);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure", 1700, 10692);
    def_parameter_latex_symbol("p", "P");
    add_db("Based on 81 laboratory PVT analyses, using a total of 304 data "
           "points. Fluid samples were obtained from reservoirs located "
           "offshore Texas and Louisiana.");
    add_db("Producing areas from Galveston Island, eastward, through Main Pass "
           "are represented.");
    add_db("Many Gulf of Mexico crudes can be characterized as naphthenic or "
           "biodegraded oils.");
    add_ref("petrosky:1993");
  }

public:
  inline Quantity<psia_1> impl(const Quantity<Sgg>& yg,
                               const Quantity<Api>& api,
                               const Quantity<SCF_STB>& rsb,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<psia>& p) const;

  Quantity<psia_1> operator()(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<psia>& p) const
  {
    return impl(yg, api, rsb, t, p);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& api = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();
      it.next();
      const auto& t = it.get_curr();
      it.next();
      const auto& p = it.get_curr();

      return impl(yg, api, rsb, t, p);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();

    return impl(yg, api, rsb, t, p);
  }
};

extern Quantity<psia_1> CoaPetroskyFarshad__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<Api>& api,
  const Quantity<SCF_STB>& rsb, const Quantity<Fahrenheit>& t,
  const Quantity<psia>& p);
extern double CoaPetroskyFarshad__correlation__valid__fct(double yg, double api,
                                                          double rsb, double t,
                                                          double p);

/** CoaVasquezBeggs

The development ranges are presented by Bánzer (1996).


*/
class CoaVasquezBeggs : public UndersaturatedOilCompressibility
{
  Correlation_Singleton(CoaVasquezBeggs);

  CoaVasquezBeggs()
    : UndersaturatedOilCompressibility("CoaVasquezBeggs",
                                       psia_1::get_instance())
  {
    set_author("Vazquez & Beggs");
    set_title("VAZQUEZ & BEGGS CORRELATION, CALCULATION OF UNDERSATURATED OIL "
              "ISOTHERMAL COMPRESSIBILITY");
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
    add_parameter("psep", psia::get_instance(), "Separator pressure", 60, 565);
    def_parameter_latex_symbol("psep", "P_{sp}");
    add_db("Based on more than 600 laboratory PVT analyses from fields all "
           "over the world.");
    add_db("The correlation was developed using a total of 4036 data points.");
    add_ref("vazquez:1980");
  }

public:
  inline Quantity<psia_1> impl(const Quantity<Sgg>& yg,
                               const Quantity<Api>& api,
                               const Quantity<SCF_STB>& rsb,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<Fahrenheit>& tsep,
                               const Quantity<psia>& p,
                               const Quantity<psia>& psep) const;

  Quantity<psia_1> operator()(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<Fahrenheit>& tsep,
                              const Quantity<psia>& p,
                              const Quantity<psia>& psep) const
  {
    return impl(yg, api, rsb, t, tsep, p, psep);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
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
      const auto& psep = it.get_curr();

      return impl(yg, api, rsb, t, tsep, p, psep);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
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
    const auto& psep = it.get_curr();

    return impl(yg, api, rsb, t, tsep, p, psep);
  }
};

extern Quantity<psia_1> CoaVasquezBeggs__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<Api>& api,
  const Quantity<SCF_STB>& rsb, const Quantity<Fahrenheit>& t,
  const Quantity<Fahrenheit>& tsep, const Quantity<psia>& p,
  const Quantity<psia>& psep);
extern double CoaVasquezBeggs__correlation__valid__fct(double yg, double api,
                                                       double rsb, double t,
                                                       double tsep, double p,
                                                       double psep);

/** CoaPerezML

The correlation could not be verified because the original reference is not
available. Date: August 29 2016.


*/
class CoaPerezML : public UndersaturatedOilCompressibility
{
  Correlation_Singleton(CoaPerezML);

  CoaPerezML()
    : UndersaturatedOilCompressibility("CoaPerezML", psia_1::get_instance())
  {
    set_author("Pérez, Heny & Lago");
    set_title("PÉREZ, HENY & LAGO CORRELATION (MODIFIED PETROSKY & FARSHAD "
              "CORRELATION), CALCULATION OF UNDERSATURATED OIL ISOTHERMAL "
              "COMPRESSIBILITY");
    set_hidden();
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.571,
                  0.981);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("api", Api::get_instance(), "API oil gravity", 6.4, 12.9);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
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
    add_ref("perez:2001");
  }

public:
  inline Quantity<psia_1> impl(const Quantity<Sgg>& yg,
                               const Quantity<Api>& api,
                               const Quantity<SCF_STB>& rsb,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<psia>& p,
                               const Quantity<psia>& pb) const;

  Quantity<psia_1> operator()(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<psia>& p,
                              const Quantity<psia>& pb) const
  {
    return impl(yg, api, rsb, t, p, pb);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
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

      return impl(yg, api, rsb, t, p, pb);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
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

    return impl(yg, api, rsb, t, p, pb);
  }
};

extern Quantity<psia_1> CoaPerezML__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<Api>& api,
  const Quantity<SCF_STB>& rsb, const Quantity<Fahrenheit>& t,
  const Quantity<psia>& p, const Quantity<psia>& pb);
extern double CoaPerezML__correlation__valid__fct(double yg, double api,
                                                  double rsb, double t,
                                                  double p, double pb);

/** CoaMillanArcia

The correlation could not be verified because the original reference is not
available. Date: August 29 2016.


*/
class CoaMillanArcia : public UndersaturatedOilCompressibility
{
  Correlation_Singleton(CoaMillanArcia);

  CoaMillanArcia()
    : UndersaturatedOilCompressibility("CoaMillanArcia", psia_1::get_instance())
  {
    set_author("Millán-Arcia");
    set_title("MILLÁN-ARCIA CORRELATION, CALCULATION OF UNDERSATURATED OIL "
              "ISOTHERMAL COMPRESSIBILITY");
    set_hidden();
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
  inline Quantity<psia_1> impl(const Quantity<Api>& api,
                               const Quantity<SCF_STB>& rsb,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<psia>& p,
                               const Quantity<psia>& pb) const;

  Quantity<psia_1> operator()(const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<psia>& p,
                              const Quantity<psia>& pb) const
  {
    return impl(api, rsb, t, p, pb);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& api = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();
      it.next();
      const auto& t = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& pb = it.get_curr();

      return impl(api, rsb, t, p, pb);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& api = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();

    return impl(api, rsb, t, p, pb);
  }
};

extern Quantity<psia_1> CoaMillanArcia__correlation__fct(
  const Quantity<Api>& api, const Quantity<SCF_STB>& rsb,
  const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
  const Quantity<psia>& pb);
extern double CoaMillanArcia__correlation__valid__fct(double api, double rsb,
                                                      double t, double p,
                                                      double pb);

#endif // UNDERSATURATED_OIL_COMPRESSIBILITY_H
