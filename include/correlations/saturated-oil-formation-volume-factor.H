#ifndef SATURATED_OIL_FORMATION_VOLUME_FACTOR_H
#define SATURATED_OIL_FORMATION_VOLUME_FACTOR_H

#include <correlations/correlation.H>

/** BobAlmarhoun


*/
class BobAlmarhoun : public SaturatedOilFormationVolumeFactor
{
  Correlation_Singleton(BobAlmarhoun);

  BobAlmarhoun()
    : SaturatedOilFormationVolumeFactor("BobAlmarhoun", RB_STB::get_instance(),
                                        1.032, 1.997)
  {
    set_author("Al-Marhoun");
    set_title("AL-MARHOUN CORRELATION, CALCULATION OF SATURATED OIL FORMATION "
              "VOLUME FACTOR");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.752,
                  1.367);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(44.6), Quantity<Api>(19.4));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 26, 1602);
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("t", Rankine::get_instance(), "Temperature",
                  Quantity<Fahrenheit>(74), Quantity<Fahrenheit>(240));
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 69 bottomhole fluid samples from 69 Middle East oil "
           "reservoirs.");
    add_ref("alMarhoun:1988");
    add_par_synonym("yo", "api", "Api");
  }

public:
  inline Quantity<RB_STB> impl(const Quantity<Sgg>& yg,
                               const Quantity<Sg_do>& yo,
                               const Quantity<SCF_STB>& rs,
                               const Quantity<Rankine>& t) const;

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<Rankine>& t) const
  {
    return impl(yg, yo, rs, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& yo = it.get_curr();
      it.next();
      const auto& rs = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(yg, yo, rs, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(yg, yo, rs, t);
  }
};

extern Quantity<RB_STB> BobAlmarhoun__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
  const Quantity<SCF_STB>& rs, const Quantity<Rankine>& t);
extern double BobAlmarhoun__correlation__valid__fct(double yg, double yo,
                                                    double rs, double t);

/** BobAlShammasi


*/
class BobAlShammasi : public SaturatedOilFormationVolumeFactor
{
  Correlation_Singleton(BobAlShammasi);

  BobAlShammasi()
    : SaturatedOilFormationVolumeFactor("BobAlShammasi", RB_STB::get_instance(),
                                        1.02, 2.916)
  {
    set_author("Al-Shammasi");
    set_title("Al-SHAMMASI CORRELATION, CALCULATION OF SATURATED OIL FORMATION "
              "VOLUME FACTOR");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.51,
                  3.44);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(63.7), Quantity<Api>(6));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 6, 3298.6);
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 74, 341.6);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on a total of 1709 data sets (1661 data sets from 13 "
           "different published literature papers and 48 data sets originated "
           "from Kuwait reservoirs from unpublished sources).");
    add_db("The data available after excluding the duplicates and the repeated "
           "data sets for formation volume factor are 1345 data sets.");
    add_ref("alShammasi:1999");
    add_par_synonym("yo", "api", "Api");
  }

public:
  inline Quantity<RB_STB> impl(const Quantity<Sgg>& yg,
                               const Quantity<Sg_do>& yo,
                               const Quantity<SCF_STB>& rs,
                               const Quantity<Fahrenheit>& t) const;

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<Fahrenheit>& t) const
  {
    return impl(yg, yo, rs, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& yo = it.get_curr();
      it.next();
      const auto& rs = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(yg, yo, rs, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(yg, yo, rs, t);
  }
};

extern Quantity<RB_STB> BobAlShammasi__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
  const Quantity<SCF_STB>& rs, const Quantity<Fahrenheit>& t);
extern double BobAlShammasi__correlation__valid__fct(double yg, double yo,
                                                     double rs, double t);

/** BobDeGhetto

De Ghetto et al. recommend the use of the Vazquez & Beggs correlation for the
estimation of Bo. The values for the coefficients c1, c2, and c3 are selected
only for API<=30.


*/
class BobDeGhetto : public SaturatedOilFormationVolumeFactor
{
  Correlation_Singleton(BobDeGhetto);

  BobDeGhetto()
    : SaturatedOilFormationVolumeFactor("BobDeGhetto", RB_STB::get_instance(),
                                        1.057, 1.362)
  {
    set_author("De Ghetto, Paone & Villa");
    set_title("DE GHETTO, PAONE & VILLA CORRELATION FOR EXTRA-HEAVY AND HEAVY "
              "OILS (VAZQUEZ & BEGGS CORRELATION), CALCULATION OF SATURATED "
              "OIL FORMATION VOLUME FACTOR");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.623,
                  1.517);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("api", Api::get_instance(), "API oil gravity", 6, 22.3);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 17.21, 640.25);
    def_parameter_latex_symbol("rs", "R_s");
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
           "classes: extra-heavy oils for °API<=10, heavy oils for "
           "10<°API<=22.3.");
    add_ref("deGhetto:1995");
  }

public:
  inline Quantity<RB_STB> impl(const Quantity<Sgg>& yg,
                               const Quantity<Api>& api,
                               const Quantity<SCF_STB>& rs,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<Fahrenheit>& tsep,
                               const Quantity<psia>& psep) const;

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<Fahrenheit>& tsep,
                              const Quantity<psia>& psep) const
  {
    return impl(yg, api, rs, t, tsep, psep);
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
      const auto& rs = it.get_curr();
      it.next();
      const auto& t = it.get_curr();
      it.next();
      const auto& tsep = it.get_curr();
      it.next();
      const auto& psep = it.get_curr();

      return impl(yg, api, rs, t, tsep, psep);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& tsep = it.get_curr();
    it.next();
    const auto& psep = it.get_curr();

    return impl(yg, api, rs, t, tsep, psep);
  }
};

extern Quantity<RB_STB> BobDeGhetto__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<Api>& api,
  const Quantity<SCF_STB>& rs, const Quantity<Fahrenheit>& t,
  const Quantity<Fahrenheit>& tsep, const Quantity<psia>& psep);
extern double BobDeGhetto__correlation__valid__fct(double yg, double api,
                                                   double rs, double t,
                                                   double tsep, double psep);

/** BobDindorukChristman

The range assigned for Rs was taken from the range expressed by the authors for
Rsb.


*/
class BobDindorukChristman : public SaturatedOilFormationVolumeFactor
{
  Correlation_Singleton(BobDindorukChristman);

  BobDindorukChristman()
    : SaturatedOilFormationVolumeFactor("BobDindorukChristman",
                                        RB_STB::get_instance(), 1.0845, 2.7381)
  {
    set_author("Dindoruk & Christman");
    set_title("DINDORUK & CHRISTMAN CORRELATION, CALCULATION OF SATURATED OIL "
              "FORMATION VOLUME FACTOR");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.6017,
                  1.027);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("api", Api::get_instance(), "API oil gravity", 14.7, 40);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 133, 3050);
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 117, 276);
    def_parameter_latex_symbol("t", "T");
    add_parameter("tsep", Fahrenheit::get_instance(), "Separator temperature");
    def_parameter_latex_symbol("tsep", "T_{sp}");
    add_db("Based on approximately 100 PVT reports from the Gulf of Mexico.");
    add_ref("dindoruk:2004");
  }

public:
  inline Quantity<RB_STB> impl(const Quantity<Sgg>& yg,
                               const Quantity<Api>& api,
                               const Quantity<SCF_STB>& rs,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<Fahrenheit>& tsep) const;

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<Fahrenheit>& tsep) const
  {
    return impl(yg, api, rs, t, tsep);
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
      const auto& rs = it.get_curr();
      it.next();
      const auto& t = it.get_curr();
      it.next();
      const auto& tsep = it.get_curr();

      return impl(yg, api, rs, t, tsep);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& tsep = it.get_curr();

    return impl(yg, api, rs, t, tsep);
  }
};

extern Quantity<RB_STB> BobDindorukChristman__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<Api>& api,
  const Quantity<SCF_STB>& rs, const Quantity<Fahrenheit>& t,
  const Quantity<Fahrenheit>& tsep);
extern double BobDindorukChristman__correlation__valid__fct(double yg,
                                                            double api,
                                                            double rs, double t,
                                                            double tsep);

/** BobDoklaOsman

The range assigned for Rs was taken from the range expressed by the authors for
Rsb.


*/
class BobDoklaOsman : public SaturatedOilFormationVolumeFactor
{
  Correlation_Singleton(BobDoklaOsman);

  BobDoklaOsman()
    : SaturatedOilFormationVolumeFactor("BobDoklaOsman", RB_STB::get_instance(),
                                        1.216, 2.493)
  {
    set_author("Dokla & Osman");
    set_title("DOKLA & OSMAN CORRELATION, CALCULATION OF SATURATED OIL "
              "FORMATION VOLUME FACTOR");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.789,
                  1.29);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity", 0.8236,
                  0.886);
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 181, 2266);
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("t", Rankine::get_instance(), "Temperature",
                  Quantity<Fahrenheit>(190), Quantity<Fahrenheit>(275));
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 51 bottomhole samples from UAE reservoirs.");
    add_ref("dokla:1992");
    add_par_synonym("yo", "api", "Api");
  }

public:
  inline Quantity<RB_STB> impl(const Quantity<Sgg>& yg,
                               const Quantity<Sg_do>& yo,
                               const Quantity<SCF_STB>& rs,
                               const Quantity<Rankine>& t) const;

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<Rankine>& t) const
  {
    return impl(yg, yo, rs, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& yo = it.get_curr();
      it.next();
      const auto& rs = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(yg, yo, rs, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(yg, yo, rs, t);
  }
};

extern Quantity<RB_STB> BobDoklaOsman__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
  const Quantity<SCF_STB>& rs, const Quantity<Rankine>& t);
extern double BobDoklaOsman__correlation__valid__fct(double yg, double yo,
                                                     double rs, double t);

/** BobGlaso


*/
class BobGlaso : public SaturatedOilFormationVolumeFactor
{
  Correlation_Singleton(BobGlaso);

  BobGlaso()
    : SaturatedOilFormationVolumeFactor("BobGlaso", RB_STB::get_instance(),
                                        1.087, 2.588)
  {
    set_author("Glaso");
    set_title("GLASO CORRELATION, CALCULATION OF SATURATED OIL FORMATION "
              "VOLUME FACTOR");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.65,
                  1.276);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(48.1), Quantity<Api>(22.3));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 90, 2637);
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 80, 280);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 26 samples from the North Sea (collected from wells in "
           "the region 56 to 62°N) and 19 samples from the Middle East, "
           "Algeria, and several areas in the U.S.");
    add_ref("glaso:1980");
    add_par_synonym("yo", "api", "Api");
  }

public:
  inline Quantity<RB_STB> impl(const Quantity<Sgg>& yg,
                               const Quantity<Sg_do>& yo,
                               const Quantity<SCF_STB>& rs,
                               const Quantity<Fahrenheit>& t) const;

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<Fahrenheit>& t) const
  {
    return impl(yg, yo, rs, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& yo = it.get_curr();
      it.next();
      const auto& rs = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(yg, yo, rs, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(yg, yo, rs, t);
  }
};

extern Quantity<RB_STB> BobGlaso__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
  const Quantity<SCF_STB>& rs, const Quantity<Fahrenheit>& t);
extern double BobGlaso__correlation__valid__fct(double yg, double yo, double rs,
                                                double t);

/** BobHanafy


*/
class BobHanafy : public SaturatedOilFormationVolumeFactor
{
  Correlation_Singleton(BobHanafy);

  BobHanafy()
    : SaturatedOilFormationVolumeFactor("BobHanafy", RB_STB::get_instance(),
                                        1.032, 4.35)
  {
    set_author("Hanafy et al.");
    set_title("HANAFY ET AL. CORRELATION, CALCULATION OF SATURATED OIL "
              "FORMATION VOLUME FACTOR");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 7, 4272);
    def_parameter_latex_symbol("rs", "R_s");
    add_db("Based on experimental PVT data of 324 fluid samples taken from 176 "
           "wells located in 75 fields. This data represents 15 productive "
           "zones of 123 reservoirs distributed along three different regions "
           "of Egypt, including the Gulf of Suez, Western Desert, and Sinai.");
    add_ref("hanafy:1997");
  }

public:
  inline Quantity<RB_STB> impl(const Quantity<SCF_STB>& rs) const;

  Quantity<RB_STB> operator()(const Quantity<SCF_STB>& rs) const
  {
    return impl(rs);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& rs = it.get_curr();

      return impl(rs);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& rs = it.get_curr();

    return impl(rs);
  }
};

extern Quantity<RB_STB> BobHanafy__correlation__fct(
  const Quantity<SCF_STB>& rs);
extern double BobHanafy__correlation__valid__fct(double rs);

/** BobKartoatmodjo


*/
class BobKartoatmodjo : public SaturatedOilFormationVolumeFactor
{
  Correlation_Singleton(BobKartoatmodjo);

  BobKartoatmodjo()
    : SaturatedOilFormationVolumeFactor("BobKartoatmodjo",
                                        RB_STB::get_instance(), 1.007, 2.144)
  {
    set_author("Kartoatmodjo & Schmidt");
    set_title("KARTOATMODJO & SCHMIDT CORRELATION, CALCULATION OF SATURATED "
              "OIL FORMATION VOLUME FACTOR");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.4824,
                  1.668);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(59), Quantity<Api>(14.4));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 0, 2890);
    def_parameter_latex_symbol("rs", "R_s");
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
    add_par_synonym("yo", "api", "Api");
  }

public:
  inline Quantity<RB_STB> impl(const Quantity<Sgg>& yg,
                               const Quantity<Sg_do>& yo,
                               const Quantity<SCF_STB>& rs,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<Fahrenheit>& tsep,
                               const Quantity<psia>& psep) const;

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<Fahrenheit>& tsep,
                              const Quantity<psia>& psep) const
  {
    return impl(yg, yo, rs, t, tsep, psep);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& yo = it.get_curr();
      it.next();
      const auto& rs = it.get_curr();
      it.next();
      const auto& t = it.get_curr();
      it.next();
      const auto& tsep = it.get_curr();
      it.next();
      const auto& psep = it.get_curr();

      return impl(yg, yo, rs, t, tsep, psep);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& tsep = it.get_curr();
    it.next();
    const auto& psep = it.get_curr();

    return impl(yg, yo, rs, t, tsep, psep);
  }
};

extern Quantity<RB_STB> BobKartoatmodjo__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
  const Quantity<SCF_STB>& rs, const Quantity<Fahrenheit>& t,
  const Quantity<Fahrenheit>& tsep, const Quantity<psia>& psep);
extern double BobKartoatmodjo__correlation__valid__fct(double yg, double yo,
                                                       double rs, double t,
                                                       double tsep,
                                                       double psep);

/** BobStanding

Lasater didn't present a correlation for Bo. Standing's correlation is used to
calculate Bob because of the similarity of his data bank and development ranges
with Lasater's.

The range assigned for Rs was taken from the range expressed by the author for
Rsb.


*/
class BobStanding : public SaturatedOilFormationVolumeFactor
{
  Correlation_Singleton(BobStanding);

  BobStanding()
    : SaturatedOilFormationVolumeFactor("BobStanding", RB_STB::get_instance())
  {
    set_author("Lasater");
    set_title("STANDING CORRELATION, CALCULATION OF SATURATED OIL FORMATION "
              "VOLUME FACTOR");
    set_hidden();
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.59,
                  1.337);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(63.8), Quantity<Api>(16.5));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 3, 4390);
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 82, 354);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 105 experimentally determined bubble point pressures from "
           "22 different Californian crude-oil-natural-gas mixtures.");
    add_ref("lasater:1958");
    add_ref("standing:1947");
    add_ref("beggs:1987");
    add_par_synonym("yo", "api", "Api");
  }

public:
  inline Quantity<RB_STB> impl(const Quantity<Sgg>& yg,
                               const Quantity<Sg_do>& yo,
                               const Quantity<SCF_STB>& rs,
                               const Quantity<Fahrenheit>& t) const;

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<Fahrenheit>& t) const
  {
    return impl(yg, yo, rs, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& yo = it.get_curr();
      it.next();
      const auto& rs = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(yg, yo, rs, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(yg, yo, rs, t);
  }
};

extern Quantity<RB_STB> BobStanding__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
  const Quantity<SCF_STB>& rs, const Quantity<Fahrenheit>& t);
extern double BobStanding__correlation__valid__fct(double yg, double yo,
                                                   double rs, double t);

/** BobPetroskyFarshad


*/
class BobPetroskyFarshad : public SaturatedOilFormationVolumeFactor
{
  Correlation_Singleton(BobPetroskyFarshad);

  BobPetroskyFarshad()
    : SaturatedOilFormationVolumeFactor("BobPetroskyFarshad",
                                        RB_STB::get_instance(), 1.1178, 1.6229)
  {
    set_author("Petrosky & Farshad");
    set_title("PETROSKY & FARSHAD CORRELATION, CALCULATION OF SATURATED OIL "
              "FORMATION VOLUME FACTOR");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.5781,
                  0.8519);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(45), Quantity<Api>(16.3));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 217, 1406);
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 114, 288);
    def_parameter_latex_symbol("t", "T");
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
  inline Quantity<RB_STB> impl(const Quantity<Sgg>& yg,
                               const Quantity<Sg_do>& yo,
                               const Quantity<SCF_STB>& rs,
                               const Quantity<Fahrenheit>& t) const;

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<Fahrenheit>& t) const
  {
    return impl(yg, yo, rs, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& yo = it.get_curr();
      it.next();
      const auto& rs = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(yg, yo, rs, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(yg, yo, rs, t);
  }
};

extern Quantity<RB_STB> BobPetroskyFarshad__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
  const Quantity<SCF_STB>& rs, const Quantity<Fahrenheit>& t);
extern double BobPetroskyFarshad__correlation__valid__fct(double yg, double yo,
                                                          double rs, double t);

/** BobTotalCFP


*/
class BobTotalCFP : public SaturatedOilFormationVolumeFactor
{
  Correlation_Singleton(BobTotalCFP);

  BobTotalCFP()
    : SaturatedOilFormationVolumeFactor("BobTotalCFP", RB_STB::get_instance())
  {
    set_author("Total CFP");
    set_title("TOTAL COMPAGNIE FRANÇAISE DES PÉTROLES (CFP) CORRELATION, "
              "CALCULATION OF SATURATED OIL FORMATION VOLUME FACTOR");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity");
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("api", Api::get_instance(), "API oil gravity");
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR");
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature");
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 336 PVT analyses of crude oil from eastern Venezuela.");
    add_ref("totalCFP:1983");
    add_ref("banzer:1996");
  }

public:
  inline Quantity<RB_STB> impl(const Quantity<Sgg>& yg,
                               const Quantity<Api>& api,
                               const Quantity<SCF_STB>& rs,
                               const Quantity<Fahrenheit>& t) const;

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<Fahrenheit>& t) const
  {
    return impl(yg, api, rs, t);
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
      const auto& rs = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(yg, api, rs, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(yg, api, rs, t);
  }
};

extern Quantity<RB_STB> BobTotalCFP__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<Api>& api,
  const Quantity<SCF_STB>& rs, const Quantity<Fahrenheit>& t);
extern double BobTotalCFP__correlation__valid__fct(double yg, double api,
                                                   double rs, double t);

/** BobVasquezBeggs


*/
class BobVasquezBeggs : public SaturatedOilFormationVolumeFactor
{
  Correlation_Singleton(BobVasquezBeggs);

  BobVasquezBeggs()
    : SaturatedOilFormationVolumeFactor("BobVasquezBeggs",
                                        RB_STB::get_instance(), 1.028, 2.226)
  {
    set_author("Vazquez & Beggs");
    set_title("VAZQUEZ & BEGGS CORRELATION, CALCULATION OF SATURATED OIL "
              "FORMATION VOLUME FACTOR");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.511,
                  1.35);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("api", Api::get_instance(), "API oil gravity", 15.3, 59.3);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 0, 2199);
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 75, 294);
    def_parameter_latex_symbol("t", "T");
    add_parameter("tsep", Fahrenheit::get_instance(), "Separator temperature",
                  76, 150);
    def_parameter_latex_symbol("tsep", "T_{sp}");
    add_parameter("psep", psia::get_instance(), "Separator pressure", 60, 565);
    def_parameter_latex_symbol("psep", "P_{sp}");
    add_db("Based on more than 600 laboratory PVT analyses from fields all "
           "over the world.");
    add_ref("vazquez:1980");
    add_ref("alShammasi:2001");
  }

public:
  inline Quantity<RB_STB> impl(const Quantity<Sgg>& yg,
                               const Quantity<Api>& api,
                               const Quantity<SCF_STB>& rs,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<Fahrenheit>& tsep,
                               const Quantity<psia>& psep) const;

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<Fahrenheit>& tsep,
                              const Quantity<psia>& psep) const
  {
    return impl(yg, api, rs, t, tsep, psep);
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
      const auto& rs = it.get_curr();
      it.next();
      const auto& t = it.get_curr();
      it.next();
      const auto& tsep = it.get_curr();
      it.next();
      const auto& psep = it.get_curr();

      return impl(yg, api, rs, t, tsep, psep);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& tsep = it.get_curr();
    it.next();
    const auto& psep = it.get_curr();

    return impl(yg, api, rs, t, tsep, psep);
  }
};

extern Quantity<RB_STB> BobVasquezBeggs__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<Api>& api,
  const Quantity<SCF_STB>& rs, const Quantity<Fahrenheit>& t,
  const Quantity<Fahrenheit>& tsep, const Quantity<psia>& psep);
extern double BobVasquezBeggs__correlation__valid__fct(double yg, double api,
                                                       double rs, double t,
                                                       double tsep,
                                                       double psep);

/** BobPerezML

The correlation could not be verified because the original reference is not
available. Date: August 22 2016.


*/
class BobPerezML : public SaturatedOilFormationVolumeFactor
{
  Correlation_Singleton(BobPerezML);

  BobPerezML()
    : SaturatedOilFormationVolumeFactor("BobPerezML", RB_STB::get_instance(),
                                        1.032, 1.183)
  {
    set_author("Pérez, Heny & Lago");
    set_title("PÉREZ, HENY & LAGO CORRELATION (EZEKWE-QUERIN-HUMPHREY "
              "CORRELATION USED BY PÉREZ, HENY & LAGO), CALCULATION OF "
              "SATURATED OIL FORMATION VOLUME FACTOR");
    set_hidden();
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
  inline Quantity<RB_STB> impl(const Quantity<Sgg>& yg,
                               const Quantity<Sg_do>& yo,
                               const Quantity<SCF_STB>& rsb,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<psia>& p,
                               const Quantity<psia>& pb) const;

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<psia>& p,
                              const Quantity<psia>& pb) const
  {
    return impl(yg, yo, rsb, t, p, pb);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
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

      return impl(yg, yo, rsb, t, p, pb);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
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

    return impl(yg, yo, rsb, t, p, pb);
  }
};

extern Quantity<RB_STB> BobPerezML__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
  const Quantity<SCF_STB>& rsb, const Quantity<Fahrenheit>& t,
  const Quantity<psia>& p, const Quantity<psia>& pb);
extern double BobPerezML__correlation__valid__fct(double yg, double yo,
                                                  double rsb, double t,
                                                  double p, double pb);

/** BobVelardeMcCain

The solution gas-oil ratios in the database are total gas-oil ratios (obtained
by adding the solution gas-oil ratios at the separator and the stock tank).


*/
class BobVelardeMcCain : public SaturatedOilFormationVolumeFactor
{
  Correlation_Singleton(BobVelardeMcCain);

  BobVelardeMcCain()
    : SaturatedOilFormationVolumeFactor("BobVelardeMcCain",
                                        RB_STB::get_instance(), 1.04, 2.082)
  {
    set_author("Velarde, Blasingame & McCain");
    set_title("VELARDE, BLASINGAME & McCAIN CORRELATION, CALCULATION OF "
              "SATURATED OIL FORMATION VOLUME FACTOR");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.561,
                  1.101);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(53.4), Quantity<Api>(11.6));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR");
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 102,
                  1808);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 70, 307);
    def_parameter_latex_symbol("t", "T");
    add_parameter("p", psia::get_instance(), "Pressure");
    def_parameter_latex_symbol("p", "P");
    add_db("Based on 195 PVT reports, including 2097 data sets.");
    add_ref("velarde:1997");
    add_par_synonym("yo", "api", "Api");
  }

public:
  inline Quantity<RB_STB> impl(const Quantity<Sgg>& yg,
                               const Quantity<Sg_do>& yo,
                               const Quantity<SCF_STB>& rs,
                               const Quantity<SCF_STB>& rsb,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<psia>& p) const;

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<psia>& p) const
  {
    return impl(yg, yo, rs, rsb, t, p);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& yo = it.get_curr();
      it.next();
      const auto& rs = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();
      it.next();
      const auto& t = it.get_curr();
      it.next();
      const auto& p = it.get_curr();

      return impl(yg, yo, rs, rsb, t, p);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();

    return impl(yg, yo, rs, rsb, t, p);
  }
};

extern Quantity<RB_STB> BobVelardeMcCain__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
  const Quantity<SCF_STB>& rs, const Quantity<SCF_STB>& rsb,
  const Quantity<Fahrenheit>& t, const Quantity<psia>& p);
extern double BobVelardeMcCain__correlation__valid__fct(double yg, double yo,
                                                        double rs, double rsb,
                                                        double t, double p);

/** BobMillanArcia

The correlation could not be verified because the original reference is not
available. Date: August 22 2016.


*/
class BobMillanArcia : public SaturatedOilFormationVolumeFactor
{
  Correlation_Singleton(BobMillanArcia);

  BobMillanArcia()
    : SaturatedOilFormationVolumeFactor("BobMillanArcia",
                                        RB_STB::get_instance(), 1.014, 1.248)
  {
    set_author("Millán-Arcia");
    set_title("MILLÁN-ARCIA CORRELATION, CALCULATION OF SATURATED OIL "
              "FORMATION VOLUME FACTOR");
    set_hidden();
    add_parameter("api", Api::get_instance(), "API oil gravity", 9, 20.2);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb");
    def_parameter_latex_symbol("rsb", "R_{sbp}");
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
  inline Quantity<RB_STB> impl(const Quantity<Api>& api,
                               const Quantity<SCF_STB>& rsb,
                               const Quantity<psia>& p,
                               const Quantity<psia>& pb) const;

  Quantity<RB_STB> operator()(const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<psia>& p,
                              const Quantity<psia>& pb) const
  {
    return impl(api, rsb, p, pb);
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
      const auto& p = it.get_curr();
      it.next();
      const auto& pb = it.get_curr();

      return impl(api, rsb, p, pb);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& api = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();

    return impl(api, rsb, p, pb);
  }
};

extern Quantity<RB_STB> BobMillanArcia__correlation__fct(
  const Quantity<Api>& api, const Quantity<SCF_STB>& rsb,
  const Quantity<psia>& p, const Quantity<psia>& pb);
extern double BobMillanArcia__correlation__valid__fct(double api, double rsb,
                                                      double p, double pb);

/** BobManucciRosales

The correlation could not be verified because the original reference is not
available. Date: August 22 2016.


*/
class BobManucciRosales : public SaturatedOilFormationVolumeFactor
{
  Correlation_Singleton(BobManucciRosales);

  BobManucciRosales()
    : SaturatedOilFormationVolumeFactor("BobManucciRosales",
                                        RB_STB::get_instance(), 1.05, 2.4)
  {
    set_author("Manucci & Rosales");
    set_title("MANUCCI & ROSALES CORRELATION, CALCULATION OF SATURATED OIL "
              "FORMATION VOLUME FACTOR");
    set_hidden();
    add_parameter("api", Api::get_instance(), "API oil gravity", 14, 40);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("rs", SCF_STB::get_instance(), "Solution GOR", 180, 2500);
    def_parameter_latex_symbol("rs", "R_s");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 165, 304);
    def_parameter_latex_symbol("t", "T");
    add_db("Crude oils of eastern Venezuela were correlated, by using "
           "differential separation data from laboratory PVT analysis.");
    add_ref("manucci:1968");
    add_ref("perez:2001");
  }

public:
  inline Quantity<RB_STB> impl(const Quantity<Api>& api,
                               const Quantity<SCF_STB>& rs,
                               const Quantity<Fahrenheit>& t) const;

  Quantity<RB_STB> operator()(const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<Fahrenheit>& t) const
  {
    return impl(api, rs, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& api = it.get_curr();
      it.next();
      const auto& rs = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(api, rs, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& api = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(api, rs, t);
  }
};

extern Quantity<RB_STB> BobManucciRosales__correlation__fct(
  const Quantity<Api>& api, const Quantity<SCF_STB>& rs,
  const Quantity<Fahrenheit>& t);
extern double BobManucciRosales__correlation__valid__fct(double api, double rs,
                                                         double t);

#endif // SATURATED_OIL_FORMATION_VOLUME_FACTOR_H
