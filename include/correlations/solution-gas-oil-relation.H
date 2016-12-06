#ifndef SOLUTION_GAS_OIL_RELATION_H
#define SOLUTION_GAS_OIL_RELATION_H

#include <correlations/correlation.H>

/** RsAlShammasi


*/
class RsAlShammasi : public SolutionGasOilRelation
{
  Correlation_Singleton(RsAlShammasi);

  RsAlShammasi()
    : SolutionGasOilRelation("RsAlShammasi", SCF_STB::get_instance(), 6, 3298.6)
  {
    set_author("Al-Shammasi");
    set_title("AL-SHAMMASI CORRELATION, CALCULATION OF SOLUTION GOR (FOR "
              "PRESSURES BELOW THE BUBBLE POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.51,
                  3.44);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure",
                  Quantity<Atmosphere>(1));
    def_parameter_latex_symbol("p", "P");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(63.7), Quantity<Api>(6));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 74, 341.6);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on a total of 1709 data sets (1661 data sets from 13 "
           "different published literature papers and 48 data sets originated "
           "from Kuwait reservoirs from unpublished sources).");
    add_ref("alShammasi:1999");
    add_par_synonym("yo", "api", "Api");
  }

public:
  inline Quantity<SCF_STB> impl(const Quantity<Sgg>& yg,
                                const Quantity<psia>& p,
                                const Quantity<Sg_do>& yo,
                                const Quantity<Fahrenheit>& t) const;

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Sg_do>& yo,
                               const Quantity<Fahrenheit>& t) const
  {
    return impl(yg, p, yo, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& yo = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(yg, p, yo, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(yg, p, yo, t);
  }
};

extern Quantity<SCF_STB> RsAlShammasi__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Sg_do>& yo,
  const Quantity<Fahrenheit>& t);
extern double RsAlShammasi__correlation__valid__fct(double yg, double p,
                                                    double yo, double t);

/** RsAlMarhoun


*/
class RsAlMarhoun : public SolutionGasOilRelation
{
  Correlation_Singleton(RsAlMarhoun);

  RsAlMarhoun()
    : SolutionGasOilRelation("RsAlMarhoun", SCF_STB::get_instance(), 26, 1602)
  {
    set_author("Al-Marhoun");
    set_title("AL-MARHOUN CORRELATION, CALCULATION OF SOLUTION GOR (FOR "
              "PRESSURES BELOW THE BUBBLE POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.752,
                  1.367);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure", 20, 3573);
    def_parameter_latex_symbol("p", "P");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(44.6), Quantity<Api>(19.4));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("t", Rankine::get_instance(), "Temperature",
                  Quantity<Fahrenheit>(74), Quantity<Fahrenheit>(240));
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 69 bottomhole fluid samples from 69 Middle East oil "
           "reservoirs.");
    add_ref("alMarhoun:1988");
    add_par_synonym("yo", "api", "Api");
  }

public:
  inline Quantity<SCF_STB> impl(const Quantity<Sgg>& yg,
                                const Quantity<psia>& p,
                                const Quantity<Sg_do>& yo,
                                const Quantity<Rankine>& t) const;

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Sg_do>& yo,
                               const Quantity<Rankine>& t) const
  {
    return impl(yg, p, yo, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& yo = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(yg, p, yo, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(yg, p, yo, t);
  }
};

extern Quantity<SCF_STB> RsAlMarhoun__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Sg_do>& yo,
  const Quantity<Rankine>& t);
extern double RsAlMarhoun__correlation__valid__fct(double yg, double p,
                                                   double yo, double t);

/** RsDeGhetto

De Ghetto et al. use Yg as the average specific gravity of total surface gases,
while the original correlation (Standing) uses it as the gravity of dissolved
gas. Since Standing doesn't specify if Yg is the gravity of total surface gases
or of the separator, it is assumed that the data is from a one stage separation
with average gas gravity.


*/
class RsDeGhetto : public SolutionGasOilRelation
{
  Correlation_Singleton(RsDeGhetto);

  RsDeGhetto()
    : SolutionGasOilRelation("RsDeGhetto", SCF_STB::get_instance(), 17.21,
                             640.25)
  {
    set_author("De Ghetto, Paone & Villa");
    set_title("DE GHETTO, PAONE & VILLA CORRELATION FOR EXTRA-HEAVY (MODIFIED "
              "STANDING CORRELATION) AND HEAVY (MODIFIED VAZQUEZ & BEGGS "
              "CORRELATION) OILS, CALCULATION OF SOLUTION GOR (FOR PRESSURES "
              "BELOW THE BUBBLE POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.623,
                  1.517);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure", 1038.49, 7411.54);
    def_parameter_latex_symbol("p", "P");
    add_parameter("api", Api::get_instance(), "API oil gravity", 6, 22.3);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 131.4, 250.7);
    def_parameter_latex_symbol("t", "T");
    add_parameter("tsep", Fahrenheit::get_instance(), "Separator temperature",
                  59, 177.8);
    def_parameter_latex_symbol("tsep", "T_{sp}");
    add_parameter("psep", psia::get_instance(), "Separator pressure", 14.5,
                  752.2);
    def_parameter_latex_symbol("psep", "P_{sp}");
    add_db("Based on 1200 measured data points of 63 heavy and extra-heavy oil "
           "samples obtained from the Mediterranean Basin, Africa and the "
           "Persian Gulf.");
    add_db("Oil samples have been divided in two different API gravity "
           "classes: extra-heavy oils for API<=10, heavy oils for "
           "10<API<=22.3.");
    add_ref("deGhetto:1995");
    add_ref("standing:1947");
  }

public:
  inline Quantity<SCF_STB> impl(const Quantity<Sgg>& yg,
                                const Quantity<psia>& p,
                                const Quantity<Api>& api,
                                const Quantity<Fahrenheit>& t,
                                const Quantity<Fahrenheit>& tsep,
                                const Quantity<psia>& psep) const;

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<Fahrenheit>& tsep,
                               const Quantity<psia>& psep) const
  {
    return impl(yg, p, api, t, tsep, psep);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();
      it.next();
      const auto& tsep = it.get_curr();
      it.next();
      const auto& psep = it.get_curr();

      return impl(yg, p, api, t, tsep, psep);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& tsep = it.get_curr();
    it.next();
    const auto& psep = it.get_curr();

    return impl(yg, p, api, t, tsep, psep);
  }
};

extern Quantity<SCF_STB> RsDeGhetto__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
  const Quantity<Fahrenheit>& t, const Quantity<Fahrenheit>& tsep,
  const Quantity<psia>& psep);
extern double RsDeGhetto__correlation__valid__fct(double yg, double p,
                                                  double api, double t,
                                                  double tsep, double psep);

/** RsDindorukChristman


*/
class RsDindorukChristman : public SolutionGasOilRelation
{
  Correlation_Singleton(RsDindorukChristman);

  RsDindorukChristman()
    : SolutionGasOilRelation("RsDindorukChristman", SCF_STB::get_instance())
  {
    set_author("Dindoruk & Christman");
    set_title("DINDORUK & CHRISTMAN CORRELATION, CALCULATION OF SOLUTION GOR "
              "(FOR PRESSURES BELOW THE BUBBLE POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.6017,
                  1.027);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("api", Api::get_instance(), "API oil gravity", 14.7, 40);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 117, 276);
    def_parameter_latex_symbol("t", "T");
    add_ref("dindoruk:2004");
  }

public:
  inline Quantity<SCF_STB> impl(const Quantity<Sgg>& yg,
                                const Quantity<psia>& p,
                                const Quantity<Api>& api,
                                const Quantity<Fahrenheit>& t) const;

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t) const
  {
    return impl(yg, p, api, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(yg, p, api, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(yg, p, api, t);
  }
};

extern Quantity<SCF_STB> RsDindorukChristman__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
  const Quantity<Fahrenheit>& t);
extern double RsDindorukChristman__correlation__valid__fct(double yg, double p,
                                                           double api,
                                                           double t);

/** RsDoklaOsman


*/
class RsDoklaOsman : public SolutionGasOilRelation
{
  Correlation_Singleton(RsDoklaOsman);

  RsDoklaOsman()
    : SolutionGasOilRelation("RsDoklaOsman", SCF_STB::get_instance())
  {
    set_author("Dokla & Osman");
    set_title("DOKLA & OSMAN CORRELATION, CALCULATION OF SOLUTION GOR (FOR "
              "PRESSURES BELOW THE BUBBLE POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.798,
                  1.29);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity", 0.8236,
                  0.886);
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("t", Rankine::get_instance(), "Temperature",
                  Quantity<Fahrenheit>(190), Quantity<Fahrenheit>(275));
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 51 bottomhole samples from UAE reservoirs.");
    add_ref("dokla:1992");
    add_par_synonym("yo", "api", "Api");
  }

public:
  inline Quantity<SCF_STB> impl(const Quantity<Sgg>& yg,
                                const Quantity<psia>& p,
                                const Quantity<Sg_do>& yo,
                                const Quantity<Rankine>& t) const;

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Sg_do>& yo,
                               const Quantity<Rankine>& t) const
  {
    return impl(yg, p, yo, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& yo = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(yg, p, yo, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(yg, p, yo, t);
  }
};

extern Quantity<SCF_STB> RsDoklaOsman__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Sg_do>& yo,
  const Quantity<Rankine>& t);
extern double RsDoklaOsman__correlation__valid__fct(double yg, double p,
                                                    double yo, double t);

/** RsGlaso

P must be strictly greater than zero for the calculation of the logarithm.


*/
class RsGlaso : public SolutionGasOilRelation
{
  Correlation_Singleton(RsGlaso);

  RsGlaso()
    : SolutionGasOilRelation("RsGlaso", SCF_STB::get_instance(), 90, 2637)
  {
    set_author("Glaso");
    set_title("GLASO CORRELATION, CALCULATION OF SOLUTION GOR (FOR PRESSURES "
              "BELOW THE BUBBLE POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.65,
                  1.276);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("api", Api::get_instance(), "API oil gravity", 22.3, 48.1);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 80, 280);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 26 samples from the North Sea (collected from wells in "
           "the region 56 to 62°N) and 19 samples from the Middle East, "
           "Algeria, and several areas in the U.S.");
    add_ref("glaso:1980");
  }

public:
  inline Quantity<SCF_STB> impl(const Quantity<Sgg>& yg,
                                const Quantity<psia>& p,
                                const Quantity<Api>& api,
                                const Quantity<Fahrenheit>& t) const;

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t) const
  {
    return impl(yg, p, api, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(yg, p, api, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(yg, p, api, t);
  }
};

extern Quantity<SCF_STB> RsGlaso__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
  const Quantity<Fahrenheit>& t);
extern double RsGlaso__correlation__valid__fct(double yg, double p, double api,
                                               double t);

/** RsHanafy


*/
class RsHanafy : public SolutionGasOilRelation
{
  Correlation_Singleton(RsHanafy);

  RsHanafy()
    : SolutionGasOilRelation("RsHanafy", SCF_STB::get_instance(), 7, 4272)
  {
    set_author("Hanafy et al.");
    set_title("HANAFY ET AL. CORRELATION, CALCULATION OF SOLUTION GOR (FOR "
              "PRESSURES BELOW THE BUBBLE POINT)");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_db("Based on experimental PVT data of 324 fluid samples taken from 176 "
           "wells located in 75 fields. This data represents 15 productive "
           "zones of 123 reservoirs distributed along three different regions "
           "of Egypt, including the Gulf of Suez, Western Desert, and Sinai.");
    add_ref("hanafy:1997");
  }

public:
  inline Quantity<SCF_STB> impl(const Quantity<psia>& p) const;

  Quantity<SCF_STB> operator()(const Quantity<psia>& p) const
  {
    return impl(p);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& p = it.get_curr();

      return impl(p);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& p = it.get_curr();

    return impl(p);
  }
};

extern Quantity<SCF_STB> RsHanafy__correlation__fct(const Quantity<psia>& p);
extern double RsHanafy__correlation__valid__fct(double p);

/** RsKartoatmodjoSchmidt


*/
class RsKartoatmodjoSchmidt : public SolutionGasOilRelation
{
  Correlation_Singleton(RsKartoatmodjoSchmidt);

  RsKartoatmodjoSchmidt()
    : SolutionGasOilRelation("RsKartoatmodjoSchmidt", SCF_STB::get_instance(),
                             0, 2890)
  {
    set_author("Kartoatmodjo & Schmidt");
    set_title("KARTOATMODJO & SCHMIDT CORRELATION, CALCULATION OF SOLUTION GOR "
              "(FOR PRESSURES BELOW THE BUBBLE POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.4824,
                  1.668);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("api", Api::get_instance(), "API oil gravity", 14.4, 59);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 75, 320);
    def_parameter_latex_symbol("t", "T");
    add_parameter("tsep", Fahrenheit::get_instance(), "Separator temperature",
                  38, 294);
    def_parameter_latex_symbol("tsep", "T_{sp}");
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
  }

public:
  inline Quantity<SCF_STB> impl(const Quantity<Sgg>& yg,
                                const Quantity<psia>& p,
                                const Quantity<Api>& api,
                                const Quantity<Fahrenheit>& t,
                                const Quantity<Fahrenheit>& tsep,
                                const Quantity<psia>& psep) const;

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<Fahrenheit>& tsep,
                               const Quantity<psia>& psep) const
  {
    return impl(yg, p, api, t, tsep, psep);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();
      it.next();
      const auto& tsep = it.get_curr();
      it.next();
      const auto& psep = it.get_curr();

      return impl(yg, p, api, t, tsep, psep);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& tsep = it.get_curr();
    it.next();
    const auto& psep = it.get_curr();

    return impl(yg, p, api, t, tsep, psep);
  }
};

extern Quantity<SCF_STB> RsKartoatmodjoSchmidt__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
  const Quantity<Fahrenheit>& t, const Quantity<Fahrenheit>& tsep,
  const Quantity<psia>& psep);
extern double RsKartoatmodjoSchmidt__correlation__valid__fct(
  double yg, double p, double api, double t, double tsep, double psep);

/** RsLasater


*/
class RsLasater : public SolutionGasOilRelation
{
  Correlation_Singleton(RsLasater);

  RsLasater()
    : SolutionGasOilRelation("RsLasater", SCF_STB::get_instance())
  {
    set_author("Lasater");
    set_title("LASATER CORRELATION, CALCULATION OF SOLUTION GOR (FOR PRESSURES "
              "BELOW THE BUBBLE POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.574,
                  1.223);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(51.1), Quantity<Api>(17.9));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("t", Rankine::get_instance(), "Temperature",
                  Quantity<Fahrenheit>(82), Quantity<Fahrenheit>(272));
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 158 experimentally measured bubble point pressures of 137 "
           "independent systems.");
    add_db("The data were obtained from Canada, Western and Mid-Continental "
           "United States, and South America.");
    add_ref("lasater:1958");
    add_ref("beggs:1987");
    add_par_synonym("yo", "api", "Api");
  }

public:
  inline Quantity<SCF_STB> impl(const Quantity<Sgg>& yg,
                                const Quantity<psia>& p,
                                const Quantity<Sg_do>& yo,
                                const Quantity<Rankine>& t) const;

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Sg_do>& yo,
                               const Quantity<Rankine>& t) const
  {
    return impl(yg, p, yo, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& yo = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(yg, p, yo, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(yg, p, yo, t);
  }
};

extern Quantity<SCF_STB> RsLasater__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Sg_do>& yo,
  const Quantity<Rankine>& t);
extern double RsLasater__correlation__valid__fct(double yg, double p, double yo,
                                                 double t);

/** RsPetroskyFarshad


*/
class RsPetroskyFarshad : public SolutionGasOilRelation
{
  Correlation_Singleton(RsPetroskyFarshad);

  RsPetroskyFarshad()
    : SolutionGasOilRelation("RsPetroskyFarshad", SCF_STB::get_instance(), 217,
                             1406)
  {
    set_author("Petrosky & Farshad");
    set_title("PETROSKY & FARSHAD CORRELATION, CALCULATION OF SOLUTION GOR "
              "(FOR PRESSURES BELOW THE BUBBLE POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.5781,
                  0.8519);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("api", Api::get_instance(), "API oil gravity", 16.3, 45);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 114, 288);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 81 laboratory PVT analyses.");
    add_db("Fluid samples were obtained from reservoirs located offshore Texas "
           "and Louisiana. Producing areas from Galveston Island, eastward, "
           "through Main Pass are represented.");
    add_ref("petrosky:1993");
  }

public:
  inline Quantity<SCF_STB> impl(const Quantity<Sgg>& yg,
                                const Quantity<psia>& p,
                                const Quantity<Api>& api,
                                const Quantity<Fahrenheit>& t) const;

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t) const
  {
    return impl(yg, p, api, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(yg, p, api, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(yg, p, api, t);
  }
};

extern Quantity<SCF_STB> RsPetroskyFarshad__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
  const Quantity<Fahrenheit>& t);
extern double RsPetroskyFarshad__correlation__valid__fct(double yg, double p,
                                                         double api, double t);

/** RsStanding


*/
class RsStanding : public SolutionGasOilRelation
{
  Correlation_Singleton(RsStanding);

  RsStanding()
    : SolutionGasOilRelation("RsStanding", SCF_STB::get_instance(), 20, 1425)
  {
    set_author("Standing");
    set_title("STANDING CORRELATION, CALCULATION OF SOLUTION GOR (FOR "
              "PRESSURES BELOW THE BUBBLE POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.59,
                  0.95);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("api", Api::get_instance(), "API oil gravity", 16.5, 63.8);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 100, 258);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 105 experimentally determined bubble point pressures from "
           "22 different Californian crude-oil-natural-gas mixtures.");
    add_ref("standing:1947");
  }

public:
  inline Quantity<SCF_STB> impl(const Quantity<Sgg>& yg,
                                const Quantity<psia>& p,
                                const Quantity<Api>& api,
                                const Quantity<Fahrenheit>& t) const;

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t) const
  {
    return impl(yg, p, api, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(yg, p, api, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(yg, p, api, t);
  }
};

extern Quantity<SCF_STB> RsStanding__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
  const Quantity<Fahrenheit>& t);
extern double RsStanding__correlation__valid__fct(double yg, double p,
                                                  double api, double t);

/** RsTotalCFP


*/
class RsTotalCFP : public SolutionGasOilRelation
{
  Correlation_Singleton(RsTotalCFP);

  RsTotalCFP()
    : SolutionGasOilRelation("RsTotalCFP", SCF_STB::get_instance())
  {
    set_author("Total CFP");
    set_title("TOTAL COMPAGNIE FRANÇAISE DES PÉTROLES (CFP) CORRELATION, "
              "CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE "
              "POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity");
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("api", Api::get_instance(), "API oil gravity", 0, 45);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature");
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 336 PVT analyses of crude oil from eastern Venezuela.");
    add_ref("totalCFP:1983");
    add_ref("banzer:1996");
  }

public:
  inline Quantity<SCF_STB> impl(const Quantity<Sgg>& yg,
                                const Quantity<psia>& p,
                                const Quantity<Api>& api,
                                const Quantity<Fahrenheit>& t) const;

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t) const
  {
    return impl(yg, p, api, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(yg, p, api, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(yg, p, api, t);
  }
};

extern Quantity<SCF_STB> RsTotalCFP__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
  const Quantity<Fahrenheit>& t);
extern double RsTotalCFP__correlation__valid__fct(double yg, double p,
                                                  double api, double t);

/** RsVasquezBeggs

The gas gravity is corrected if it resulted from a first-stage separation at a
pressure other than 100 psig (approximately 114.7 psia).


*/
class RsVasquezBeggs : public SolutionGasOilRelation
{
  Correlation_Singleton(RsVasquezBeggs);

  RsVasquezBeggs()
    : SolutionGasOilRelation("RsVasquezBeggs", SCF_STB::get_instance(), 0, 2199)
  {
    set_author("Vazquez & Beggs");
    set_title("VAZQUEZ & BEGGS CORRELATION, CALCULATION OF SOLUTION GOR (FOR "
              "PRESSURES BELOW THE BUBBLE POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.511,
                  1.35);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("api", Api::get_instance(), "API oil gravity", 15.3, 59.3);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 75, 294);
    def_parameter_latex_symbol("t", "T");
    add_parameter("tsep", Fahrenheit::get_instance(), "Separator temperature",
                  76, 150);
    def_parameter_latex_symbol("tsep", "T_{sp}");
    add_parameter("psep", psia::get_instance(), "Separator pressure", 60, 565);
    def_parameter_latex_symbol("psep", "P_{sp}");
    add_db("Based on more than 600 laboratory PVT analyses from fields all "
           "over the world.");
    add_db("The equation for solution GOR was obtained by regression analysis "
           "using 5008 measured data points.");
    add_ref("vazquez:1980");
    add_ref("alShammasi:2001");
  }

public:
  inline Quantity<SCF_STB> impl(const Quantity<Sgg>& yg,
                                const Quantity<psia>& p,
                                const Quantity<Api>& api,
                                const Quantity<Fahrenheit>& t,
                                const Quantity<Fahrenheit>& tsep,
                                const Quantity<psia>& psep) const;

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<Fahrenheit>& tsep,
                               const Quantity<psia>& psep) const
  {
    return impl(yg, p, api, t, tsep, psep);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();
      it.next();
      const auto& tsep = it.get_curr();
      it.next();
      const auto& psep = it.get_curr();

      return impl(yg, p, api, t, tsep, psep);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& tsep = it.get_curr();
    it.next();
    const auto& psep = it.get_curr();

    return impl(yg, p, api, t, tsep, psep);
  }
};

extern Quantity<SCF_STB> RsVasquezBeggs__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
  const Quantity<Fahrenheit>& t, const Quantity<Fahrenheit>& tsep,
  const Quantity<psia>& psep);
extern double RsVasquezBeggs__correlation__valid__fct(double yg, double p,
                                                      double api, double t,
                                                      double tsep, double psep);

/** RsVelarde


*/
class RsVelarde : public SolutionGasOilRelation
{
  Correlation_Singleton(RsVelarde);

  RsVelarde()
    : SolutionGasOilRelation("RsVelarde", SCF_STB::get_instance())
  {
    set_author("Velarde, Blasingame & McCain");
    set_title("VELARDE, BLASINGAME & McCAIN CORRELATION, CALCULATION OF "
              "SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE POINT)");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.561,
                  1.101);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("pb", psig::get_instance(), "Bubble point pressure",
                  Quantity<psia>(106), Quantity<psia>(5312));
    def_parameter_latex_symbol("pb", "P_b");
    add_parameter("p", psig::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("api", Api::get_instance(), "API oil gravity", 11.6, 53.4);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 70, 307);
    def_parameter_latex_symbol("t", "T");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 102,
                  1808);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_db("Based on 195 PVT reports, including 2097 data sets.");
    add_ref("velarde:1997");
  }

public:
  inline Quantity<SCF_STB> impl(const Quantity<Sgg>& yg,
                                const Quantity<psig>& pb,
                                const Quantity<psig>& p,
                                const Quantity<Api>& api,
                                const Quantity<Fahrenheit>& t,
                                const Quantity<SCF_STB>& rsb) const;

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg,
                               const Quantity<psig>& pb,
                               const Quantity<psig>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<SCF_STB>& rsb) const
  {
    return impl(yg, pb, p, api, t, rsb);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& pb = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();

      return impl(yg, pb, p, api, t, rsb);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();

    return impl(yg, pb, p, api, t, rsb);
  }
};

extern Quantity<SCF_STB> RsVelarde__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<psig>& pb, const Quantity<psig>& p,
  const Quantity<Api>& api, const Quantity<Fahrenheit>& t,
  const Quantity<SCF_STB>& rsb);
extern double RsVelarde__correlation__valid__fct(double yg, double pb, double p,
                                                 double api, double t,
                                                 double rsb);

/** RsCegarra

The correlation could not be verified because the original reference is not
available. Date: August 16 2016.


*/
class RsCegarra : public SolutionGasOilRelation
{
  Correlation_Singleton(RsCegarra);

  RsCegarra()
    : SolutionGasOilRelation("RsCegarra", SCF_STB::get_instance(), 135, 4290)
  {
    set_author("Cegarra");
    set_title("CEGARRA CORRELATION (MODIFIED STANDING CORRELATION), "
              "CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE "
              "POINT)");
    set_hidden();
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.654,
                  1.337);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("api", Api::get_instance(), "API oil gravity", 22, 41.9);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 117, 354);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 375 consistent PVT analyses. The data bank was compiled "
           "from 8394 fields of eastern Venezuela subdivided into 5192 fields "
           "of light oil and 3202 fields of medium oil.");
    add_db("The correlation takes the functional form of Standing.");
    add_ref("cegarra:2007");
    add_ref("standing:1947");
  }

public:
  inline Quantity<SCF_STB> impl(const Quantity<Sgg>& yg,
                                const Quantity<psia>& p,
                                const Quantity<Api>& api,
                                const Quantity<Fahrenheit>& t) const;

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t) const
  {
    return impl(yg, p, api, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(yg, p, api, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(yg, p, api, t);
  }
};

extern Quantity<SCF_STB> RsCegarra__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
  const Quantity<Fahrenheit>& t);
extern double RsCegarra__correlation__valid__fct(double yg, double p,
                                                 double api, double t);

/** RsPerezML

The correlation could not be verified because the original reference is not
available. Date: August 16 2016.


*/
class RsPerezML : public SolutionGasOilRelation
{
  Correlation_Singleton(RsPerezML);

  RsPerezML()
    : SolutionGasOilRelation("RsPerezML", SCF_STB::get_instance(), 38, 121)
  {
    set_author("Pérez, Heny & Lago");
    set_title("PÉREZ, HENY & LAGO CORRELATION (MODIFIED MILLÁN-ARCIA "
              "CORRELATION), CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW "
              "THE BUBBLE POINT)");
    set_hidden();
    add_parameter("pb", psia::get_instance(), "Bubble point pressure", 405,
                  1335);
    def_parameter_latex_symbol("pb", "P_b");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb");
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_ref("perez:2001");
  }

public:
  inline Quantity<SCF_STB> impl(const Quantity<psia>& pb,
                                const Quantity<psia>& p,
                                const Quantity<SCF_STB>& rsb) const;

  Quantity<SCF_STB> operator()(const Quantity<psia>& pb,
                               const Quantity<psia>& p,
                               const Quantity<SCF_STB>& rsb) const
  {
    return impl(pb, p, rsb);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& pb = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();

      return impl(pb, p, rsb);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& pb = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();

    return impl(pb, p, rsb);
  }
};

extern Quantity<SCF_STB> RsPerezML__correlation__fct(
  const Quantity<psia>& pb, const Quantity<psia>& p,
  const Quantity<SCF_STB>& rsb);
extern double RsPerezML__correlation__valid__fct(double pb, double p,
                                                 double rsb);

/** RsMillanArcia

The correlation could not be verified because the original reference is not
available. Date: August 16 2016.


*/
class RsMillanArcia : public SolutionGasOilRelation
{
  Correlation_Singleton(RsMillanArcia);

  RsMillanArcia()
    : SolutionGasOilRelation("RsMillanArcia", SCF_STB::get_instance(), 53, 459)
  {
    set_author("Millán-Arcia");
    set_title("MILLÁN-ARCIA CORRELATION, CALCULATION OF SOLUTION GOR (FOR "
              "PRESSURES BELOW THE BUBBLE POINT)");
    set_hidden();
    add_parameter("pb", psia::get_instance(), "Bubble point pressure", 222,
                  3432.7);
    def_parameter_latex_symbol("pb", "P_b");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb");
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_db("Venezuelan heavy crudes were correlated.");
    add_ref("millan:1984");
    add_ref("perez:2001");
  }

public:
  inline Quantity<SCF_STB> impl(const Quantity<psia>& pb,
                                const Quantity<psia>& p,
                                const Quantity<SCF_STB>& rsb) const;

  Quantity<SCF_STB> operator()(const Quantity<psia>& pb,
                               const Quantity<psia>& p,
                               const Quantity<SCF_STB>& rsb) const
  {
    return impl(pb, p, rsb);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& pb = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();

      return impl(pb, p, rsb);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& pb = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();

    return impl(pb, p, rsb);
  }
};

extern Quantity<SCF_STB> RsMillanArcia__correlation__fct(
  const Quantity<psia>& pb, const Quantity<psia>& p,
  const Quantity<SCF_STB>& rsb);
extern double RsMillanArcia__correlation__valid__fct(double pb, double p,
                                                     double rsb);

/** RsManucciRosales

The correlation could not be verified because the original reference is not
available. Date: August 16 2016.


*/
class RsManucciRosales : public SolutionGasOilRelation
{
  Correlation_Singleton(RsManucciRosales);

  RsManucciRosales()
    : SolutionGasOilRelation("RsManucciRosales", SCF_STB::get_instance(), 180,
                             2500)
  {
    set_author("Manucci & Rosales");
    set_title("MANUCCI & ROSALES CORRELATION, CALCULATION OF SOLUTION GOR (FOR "
              "PRESSURES BELOW THE BUBBLE POINT)");
    set_hidden();
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity");
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("api", Api::get_instance(), "API oil gravity", 14, 40);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 165, 304);
    def_parameter_latex_symbol("t", "T");
    add_db("Crude oils of eastern Venezuela were correlated, by using "
           "differential separation data from laboratory PVT analysis.");
    add_ref("manucci:1968");
    add_ref("perez:2001");
  }

public:
  inline Quantity<SCF_STB> impl(const Quantity<Sgg>& yg,
                                const Quantity<psia>& p,
                                const Quantity<Api>& api,
                                const Quantity<Fahrenheit>& t) const;

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg, const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t) const
  {
    return impl(yg, p, api, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(yg, p, api, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(yg, p, api, t);
  }
};

extern Quantity<SCF_STB> RsManucciRosales__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
  const Quantity<Fahrenheit>& t);
extern double RsManucciRosales__correlation__valid__fct(double yg, double p,
                                                        double api, double t);

/** RsAbovePb


*/
class RsAbovePb : public SolutionGasOilRelation
{
  Correlation_Singleton(RsAbovePb);

  RsAbovePb()
    : SolutionGasOilRelation("RsAbovePb", SCF_STB::get_instance())
  {
    set_author("Not defined");
    set_title("Faked constant for internal computations above bubble point");
    set_hidden();
    add_parameter("pb", psia::get_instance(), "Bubble point pressure");
    def_parameter_latex_symbol("pb", "P_b");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb");
    def_parameter_latex_symbol("rsb", "R_{sbp}");
  }

public:
  inline Quantity<SCF_STB> impl(const Quantity<psia>& pb,
                                const Quantity<psia>& p,
                                const Quantity<SCF_STB>& rsb) const;

  Quantity<SCF_STB> operator()(const Quantity<psia>& pb,
                               const Quantity<psia>& p,
                               const Quantity<SCF_STB>& rsb) const
  {
    return impl(pb, p, rsb);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& pb = it.get_curr();
      it.next();
      const auto& p = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();

      return impl(pb, p, rsb);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& pb = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();

    return impl(pb, p, rsb);
  }
};

extern Quantity<SCF_STB> RsAbovePb__correlation__fct(
  const Quantity<psia>& pb, const Quantity<psia>& p,
  const Quantity<SCF_STB>& rsb);
extern double RsAbovePb__correlation__valid__fct(double pb, double p,
                                                 double rsb);

#endif // SOLUTION_GAS_OIL_RELATION_H
