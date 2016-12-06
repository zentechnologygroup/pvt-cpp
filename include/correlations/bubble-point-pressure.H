#ifndef BUBBLE_POINT_PRESSURE_H
#define BUBBLE_POINT_PRESSURE_H

#include <correlations/correlation.H>

/** PbAlMarhoun


*/
class PbAlMarhoun : public BubblePointPressure
{
  Correlation_Singleton(PbAlMarhoun);

  PbAlMarhoun()
    : BubblePointPressure("PbAlMarhoun", psia::get_instance(), 130, 3573)
  {
    set_author("Al-Marhoun");
    set_title("AL-MARHOUN CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.752,
                  1.367);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(44.6), Quantity<Api>(19.4));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 26,
                  1602);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("t", Rankine::get_instance(), "Temperature",
                  Quantity<Fahrenheit>(74), Quantity<Fahrenheit>(240));
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 69 bottomhole fluid samples from 69 Middle East oil "
           "reservoirs.");
    add_ref("alMarhoun:1988");
    add_par_synonym("yo", "api", "api");
  }

public:
  inline Quantity<psia> impl(const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
                             const Quantity<SCF_STB>& rsb,
                             const Quantity<Rankine>& t) const;

  Quantity<psia> operator()(const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Rankine>& t) const
  {
    return impl(yg, yo, rsb, t);
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

      return impl(yg, yo, rsb, t);
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

    return impl(yg, yo, rsb, t);
  }
};

extern Quantity<psia> PbAlMarhoun__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
  const Quantity<SCF_STB>& rsb, const Quantity<Rankine>& t);
extern double PbAlMarhoun__correlation__valid__fct(double yg, double yo,
                                                   double rsb, double t);

/** PbAlShammasi


*/
class PbAlShammasi : public BubblePointPressure
{
  Correlation_Singleton(PbAlShammasi);

  PbAlShammasi()
    : BubblePointPressure("PbAlShammasi", psia::get_instance(), 31.7, 7127)
  {
    set_author("Al-Shammasi");
    set_title("AL-SHAMMASI CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.51,
                  3.44);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(63.7), Quantity<Api>(6));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 6,
                  3298.6);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 74, 341.6);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on a total of 1709 data sets (1661 data sets from 13 "
           "different published literature papers and 48 data sets originated "
           "from Kuwait reservoirs from unpublished sources).");
    add_ref("alShammasi:1999");
    add_par_synonym("yo", "api", "api");
  }

public:
  inline Quantity<psia> impl(const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
                             const Quantity<SCF_STB>& rsb,
                             const Quantity<Fahrenheit>& t) const;

  Quantity<psia> operator()(const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Fahrenheit>& t) const
  {
    return impl(yg, yo, rsb, t);
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

      return impl(yg, yo, rsb, t);
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

    return impl(yg, yo, rsb, t);
  }
};

extern Quantity<psia> PbAlShammasi__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
  const Quantity<SCF_STB>& rsb, const Quantity<Fahrenheit>& t);
extern double PbAlShammasi__correlation__valid__fct(double yg, double yo,
                                                    double rsb, double t);

/** PbDeGhetto

De Ghetto, Paone & Villa use Yg as the average specific gravity of total surface
gases, while the original correlation (Standing) uses it as the gravity of
dissolved gas. Since Standing doesn't specify if Yg is the gravity of total
surface gases or of the separator, it is assumed that the data is from a one
stage separation with average gas gravity.


*/
class PbDeGhetto : public BubblePointPressure
{
  Correlation_Singleton(PbDeGhetto);

  PbDeGhetto()
    : BubblePointPressure("PbDeGhetto", psia::get_instance(), 208.86, 4021.96)
  {
    set_author("De Ghetto, Paone & Villa");
    set_title("DE GHETTO, PAONE & VILLA CORRELATION FOR EXTRA-HEAVY AND HEAVY "
              "OILS (MODIFIED STANDING CORRELATION), CALCULATION OF BUBBLE "
              "POINT PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.623,
                  1.517);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 17.21,
                  640.25);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
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
    add_ref("standing:1947");
  }

public:
  inline Quantity<psia> impl(const Quantity<Sgg>& yg,
                             const Quantity<SCF_STB>& rsb,
                             const Quantity<Api>& api,
                             const Quantity<Fahrenheit>& t) const;

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t) const
  {
    return impl(yg, rsb, api, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();
      it.next();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(yg, rsb, api, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(yg, rsb, api, t);
  }
};

extern Quantity<psia> PbDeGhetto__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
  const Quantity<Api>& api, const Quantity<Fahrenheit>& t);
extern double PbDeGhetto__correlation__valid__fct(double yg, double rsb,
                                                  double api, double t);

/** PbDindorukChristman


*/
class PbDindorukChristman : public BubblePointPressure
{
  Correlation_Singleton(PbDindorukChristman);

  PbDindorukChristman()
    : BubblePointPressure("PbDindorukChristman", psia::get_instance(), 926,
                          12230)
  {
    set_author("Dindoruk & Christman");
    set_title(
      "DINDORUK & CHRISTMAN CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.623,
                  1.517);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 133,
                  3050);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("api", Api::get_instance(), "API oil gravity", 14.7, 40);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 117, 276);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on more than 100 PVT reports from the Gulf of Mexico.");
    add_ref("dindoruk:2004");
  }

public:
  inline Quantity<psia> impl(const Quantity<Sgg>& yg,
                             const Quantity<SCF_STB>& rsb,
                             const Quantity<Api>& api,
                             const Quantity<Fahrenheit>& t) const;

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t) const
  {
    return impl(yg, rsb, api, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();
      it.next();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(yg, rsb, api, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(yg, rsb, api, t);
  }
};

extern Quantity<psia> PbDindorukChristman__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
  const Quantity<Api>& api, const Quantity<Fahrenheit>& t);
extern double PbDindorukChristman__correlation__valid__fct(double yg,
                                                           double rsb,
                                                           double api,
                                                           double t);

/** PbDoklaOsman


*/
class PbDoklaOsman : public BubblePointPressure
{
  Correlation_Singleton(PbDoklaOsman);

  PbDoklaOsman()
    : BubblePointPressure("PbDoklaOsman", psia::get_instance(), 590, 4640)
  {
    set_author("Dokla & Osman");
    set_title(
      "DOKLA & OSMAN CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.798,
                  1.29);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 181,
                  2266);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity", 0.8236,
                  0.886);
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("t", Rankine::get_instance(), "Temperature",
                  Quantity<Fahrenheit>(190), Quantity<Fahrenheit>(275));
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 51 bottomhole samples from UAE reservoirs.");
    add_ref("dokla:1992");
    add_par_synonym("yo", "api", "api");
  }

public:
  inline Quantity<psia> impl(const Quantity<Sgg>& yg,
                             const Quantity<SCF_STB>& rsb,
                             const Quantity<Sg_do>& yo,
                             const Quantity<Rankine>& t) const;

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Sg_do>& yo,
                            const Quantity<Rankine>& t) const
  {
    return impl(yg, rsb, yo, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();
      it.next();
      const auto& yo = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(yg, rsb, yo, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(yg, rsb, yo, t);
  }
};

extern Quantity<psia> PbDoklaOsman__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
  const Quantity<Sg_do>& yo, const Quantity<Rankine>& t);
extern double PbDoklaOsman__correlation__valid__fct(double yg, double rsb,
                                                    double yo, double t);

/** PbHanafy


*/
class PbHanafy : public BubblePointPressure
{
  Correlation_Singleton(PbHanafy);

  PbHanafy()
    : BubblePointPressure("PbHanafy", psia::get_instance(), 36, 5003)
  {
    set_author("Hanafy et al.");
    set_title(
      "HANAFY ET AL. CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE");
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
  inline Quantity<psia> impl(const Quantity<SCF_STB>& rsb) const;

  Quantity<psia> operator()(const Quantity<SCF_STB>& rsb) const
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

extern Quantity<psia> PbHanafy__correlation__fct(const Quantity<SCF_STB>& rsb);
extern double PbHanafy__correlation__valid__fct(double rsb);

/** PbKartoatmodjoSchmidt


*/
class PbKartoatmodjoSchmidt : public BubblePointPressure
{
  Correlation_Singleton(PbKartoatmodjoSchmidt);

  PbKartoatmodjoSchmidt()
    : BubblePointPressure("PbKartoatmodjoSchmidt", psia::get_instance(), 14.7,
                          6054.7)
  {
    set_author("Kartoatmodjo & Schmidt");
    set_title("KARTOATMODJO & SCHMIDT CORRELATION, CALCULATION OF BUBBLE POINT "
              "PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.4824,
                  1.668);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 0,
                  2890);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
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
  inline Quantity<psia> impl(const Quantity<Sgg>& yg,
                             const Quantity<SCF_STB>& rsb,
                             const Quantity<Api>& api,
                             const Quantity<Fahrenheit>& t,
                             const Quantity<Fahrenheit>& tsep,
                             const Quantity<psia>& psep) const;

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t,
                            const Quantity<Fahrenheit>& tsep,
                            const Quantity<psia>& psep) const
  {
    return impl(yg, rsb, api, t, tsep, psep);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();
      it.next();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();
      it.next();
      const auto& tsep = it.get_curr();
      it.next();
      const auto& psep = it.get_curr();

      return impl(yg, rsb, api, t, tsep, psep);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& tsep = it.get_curr();
    it.next();
    const auto& psep = it.get_curr();

    return impl(yg, rsb, api, t, tsep, psep);
  }
};

extern Quantity<psia> PbKartoatmodjoSchmidt__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
  const Quantity<Api>& api, const Quantity<Fahrenheit>& t,
  const Quantity<Fahrenheit>& tsep, const Quantity<psia>& psep);
extern double PbKartoatmodjoSchmidt__correlation__valid__fct(
  double yg, double rsb, double api, double t, double tsep, double psep);

/** PbLasater


*/
class PbLasater : public BubblePointPressure
{
  Correlation_Singleton(PbLasater);

  PbLasater()
    : BubblePointPressure("PbLasater", psia::get_instance(), 48, 5780)
  {
    set_author("Lasater");
    set_title("LASATER CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.574,
                  1.223);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 3,
                  2905);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("yo", Sg_do::get_instance(), "Oil specific gravity",
                  Quantity<Api>(51.1), Quantity<Api>(17.9));
    def_parameter_latex_symbol("yo", "\\gamma_o");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 82, 272);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 158 experimentally measured bubble point pressures of 137 "
           "independent systems.");
    add_db("The data were obtained from Canada, Western and Mid-Continental "
           "United States, and South America.");
    add_ref("lasater:1958");
    add_ref("beggs:1987");
  }

public:
  inline Quantity<psia> impl(const Quantity<Sgg>& yg,
                             const Quantity<SCF_STB>& rsb,
                             const Quantity<Sg_do>& yo,
                             const Quantity<Fahrenheit>& t) const;

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Sg_do>& yo,
                            const Quantity<Fahrenheit>& t) const
  {
    return impl(yg, rsb, yo, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();
      it.next();
      const auto& yo = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(yg, rsb, yo, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(yg, rsb, yo, t);
  }
};

extern Quantity<psia> PbLasater__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
  const Quantity<Sg_do>& yo, const Quantity<Fahrenheit>& t);
extern double PbLasater__correlation__valid__fct(double yg, double rsb,
                                                 double yo, double t);

/** PbPetroskyFarshad


*/
class PbPetroskyFarshad : public BubblePointPressure
{
  Correlation_Singleton(PbPetroskyFarshad);

  PbPetroskyFarshad()
    : BubblePointPressure("PbPetroskyFarshad", psia::get_instance(), 1574, 6523)
  {
    set_author("Petrosky & Farshad");
    set_title(
      "PETROSKY & FARSHAD CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.5781,
                  0.8519);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 217,
                  1406);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
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
  inline Quantity<psia> impl(const Quantity<Sgg>& yg,
                             const Quantity<SCF_STB>& rsb,
                             const Quantity<Api>& api,
                             const Quantity<Fahrenheit>& t) const;

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t) const
  {
    return impl(yg, rsb, api, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();
      it.next();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(yg, rsb, api, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(yg, rsb, api, t);
  }
};

extern Quantity<psia> PbPetroskyFarshad__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
  const Quantity<Api>& api, const Quantity<Fahrenheit>& t);
extern double PbPetroskyFarshad__correlation__valid__fct(double yg, double rsb,
                                                         double api, double t);

/** PbStanding


*/
class PbStanding : public BubblePointPressure
{
  Correlation_Singleton(PbStanding);

  PbStanding()
    : BubblePointPressure("PbStanding", psia::get_instance(), 130, 7000)
  {
    set_author("Standing");
    set_title("STANDING CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.59,
                  0.95);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 20,
                  1425);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("api", Api::get_instance(), "API oil gravity", 16.5, 63.8);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 100, 258);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 105 experimentally determined bubble point pressures from "
           "22 different Californian crude-oil-natural-gas mixtures.");
    add_ref("standing:1947");
    add_ref("alShammasi:2001");
  }

public:
  inline Quantity<psia> impl(const Quantity<Sgg>& yg,
                             const Quantity<SCF_STB>& rsb,
                             const Quantity<Api>& api,
                             const Quantity<Fahrenheit>& t) const;

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t) const
  {
    return impl(yg, rsb, api, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();
      it.next();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(yg, rsb, api, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(yg, rsb, api, t);
  }
};

extern Quantity<psia> PbStanding__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
  const Quantity<Api>& api, const Quantity<Fahrenheit>& t);
extern double PbStanding__correlation__valid__fct(double yg, double rsb,
                                                  double api, double t);

/** PbTotalCFP


*/
class PbTotalCFP : public BubblePointPressure
{
  Correlation_Singleton(PbTotalCFP);

  PbTotalCFP()
    : BubblePointPressure("PbTotalCFP", psia::get_instance())
  {
    set_author("Total CFP");
    set_title("TOTAL COMPAGNIE FRANÇAISE DES PÉTROLES (CFP) CORRELATION, "
              "CALCULATION OF BUBBLE POINT PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity");
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb");
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("api", Api::get_instance(), "API oil gravity",
                  Api::get_instance().min_val, 45);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature");
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 336 PVT analyses of crude oil from eastern Venezuela.");
    add_ref("totalCFP:1983");
    add_ref("banzer:1996");
  }

public:
  inline Quantity<psia> impl(const Quantity<Sgg>& yg,
                             const Quantity<SCF_STB>& rsb,
                             const Quantity<Api>& api,
                             const Quantity<Fahrenheit>& t) const;

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t) const
  {
    return impl(yg, rsb, api, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();
      it.next();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(yg, rsb, api, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(yg, rsb, api, t);
  }
};

extern Quantity<psia> PbTotalCFP__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
  const Quantity<Api>& api, const Quantity<Fahrenheit>& t);
extern double PbTotalCFP__correlation__valid__fct(double yg, double rsb,
                                                  double api, double t);

/** PbVasquezBeggs

The gas gravity is corrected if it resulted from a first-stage separation at a
pressure other than 100 psig (approximately 114.7 psia).


*/
class PbVasquezBeggs : public BubblePointPressure
{
  Correlation_Singleton(PbVasquezBeggs);

  PbVasquezBeggs()
    : BubblePointPressure("PbVasquezBeggs", psia::get_instance(), 15, 6055)
  {
    set_author("Vazquez & Beggs");
    set_title(
      "VAZQUEZ & BEGGS CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.511,
                  1.35);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 0,
                  2199);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
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
    add_ref("vazquez:1980");
    add_ref("beggs:1987");
    add_ref("alShammasi:2001");
  }

public:
  inline Quantity<psia> impl(const Quantity<Sgg>& yg,
                             const Quantity<SCF_STB>& rsb,
                             const Quantity<Api>& api,
                             const Quantity<Fahrenheit>& t,
                             const Quantity<Fahrenheit>& tsep,
                             const Quantity<psia>& psep) const;

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t,
                            const Quantity<Fahrenheit>& tsep,
                            const Quantity<psia>& psep) const
  {
    return impl(yg, rsb, api, t, tsep, psep);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();
      it.next();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();
      it.next();
      const auto& tsep = it.get_curr();
      it.next();
      const auto& psep = it.get_curr();

      return impl(yg, rsb, api, t, tsep, psep);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& tsep = it.get_curr();
    it.next();
    const auto& psep = it.get_curr();

    return impl(yg, rsb, api, t, tsep, psep);
  }
};

extern Quantity<psia> PbVasquezBeggs__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
  const Quantity<Api>& api, const Quantity<Fahrenheit>& t,
  const Quantity<Fahrenheit>& tsep, const Quantity<psia>& psep);
extern double PbVasquezBeggs__correlation__valid__fct(double yg, double rsb,
                                                      double api, double t,
                                                      double tsep, double psep);

/** PbVelarde


*/
class PbVelarde : public BubblePointPressure
{
  Correlation_Singleton(PbVelarde);

  PbVelarde()
    : BubblePointPressure("PbVelarde", psia::get_instance(), 70, 6700)
  {
    set_author("Velarde, Blasingame & McCain");
    set_title("VELARDE, BLASINGAME & McCAIN CORRELATION, CALCULATION OF BUBBLE "
              "POINT PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.556,
                  1.367);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 10,
                  1870);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("api", Api::get_instance(), "API oil gravity", 12, 55);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 74, 327);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 728 laboratory measured bubble point pressures.");
    add_ref("velarde:1997");
  }

public:
  inline Quantity<psia> impl(const Quantity<Sgg>& yg,
                             const Quantity<SCF_STB>& rsb,
                             const Quantity<Api>& api,
                             const Quantity<Fahrenheit>& t) const;

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t) const
  {
    return impl(yg, rsb, api, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();
      it.next();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(yg, rsb, api, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(yg, rsb, api, t);
  }
};

extern Quantity<psia> PbVelarde__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
  const Quantity<Api>& api, const Quantity<Fahrenheit>& t);
extern double PbVelarde__correlation__valid__fct(double yg, double rsb,
                                                 double api, double t);

/** PbCegarra

The correlation could not be verified because the original reference is not
available. Date: August 10 2016.


*/
class PbCegarra : public BubblePointPressure
{
  Correlation_Singleton(PbCegarra);

  PbCegarra()
    : BubblePointPressure("PbCegarra", psia::get_instance(), 735, 7303)
  {
    set_author("Cegarra");
    set_title("CEGARRA CORRELATION (MODIFIED STANDING CORRELATION), "
              "CALCULATION OF BUBBLE POINT PRESSURE");
    set_hidden();
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.654,
                  1.337);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 135,
                  4290);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("api", Api::get_instance(), "API oil gravity", 22, 41.9);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 117, 354);
    def_parameter_latex_symbol("t", "T");
    add_db("Based on 375 consistent PVT analyses. The data bank was compiled "
           "from 8394 fields of eastern Venezuela subdivided into 5192 fields "
           "of light oil and 3202 fields of medium oil. The correlation takes "
           "the functional form of Standing.");
    add_ref("cegarra:2007");
    add_ref("standing:1947");
  }

public:
  inline Quantity<psia> impl(const Quantity<Sgg>& yg,
                             const Quantity<SCF_STB>& rsb,
                             const Quantity<Api>& api,
                             const Quantity<Fahrenheit>& t) const;

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t) const
  {
    return impl(yg, rsb, api, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();
      it.next();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(yg, rsb, api, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(yg, rsb, api, t);
  }
};

extern Quantity<psia> PbCegarra__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
  const Quantity<Api>& api, const Quantity<Fahrenheit>& t);
extern double PbCegarra__correlation__valid__fct(double yg, double rsb,
                                                 double api, double t);

/** PbPerezML

The correlation could not be verified because the original reference is not
available. Date: August 10 2016.


*/
class PbPerezML : public BubblePointPressure
{
  Correlation_Singleton(PbPerezML);

  PbPerezML()
    : BubblePointPressure("PbPerezML", psia::get_instance(), 405, 1335)
  {
    set_author("Pérez, Heny & Lago");
    set_title("PÉREZ, HENY & LAGO CORRELATION (MODIFIED TOTAL CFP "
              "CORRELATION), CALCULATION OF BUBBLE POINT PRESSURE");
    set_hidden();
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.571,
                  0.981);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 38,
                  121);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("api", Api::get_instance(), "API oil gravity", 6.4, 12.9);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 112, 300);
    def_parameter_latex_symbol("t", "T");
    add_ref("perez:2001");
  }

public:
  inline Quantity<psia> impl(const Quantity<Sgg>& yg,
                             const Quantity<SCF_STB>& rsb,
                             const Quantity<Api>& api,
                             const Quantity<Fahrenheit>& t) const;

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t) const
  {
    return impl(yg, rsb, api, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();
      it.next();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(yg, rsb, api, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(yg, rsb, api, t);
  }
};

extern Quantity<psia> PbPerezML__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
  const Quantity<Api>& api, const Quantity<Fahrenheit>& t);
extern double PbPerezML__correlation__valid__fct(double yg, double rsb,
                                                 double api, double t);

/** PbMillanArcia

The correlation could not be verified because the original reference is not
available. Date: August 10 2016.


*/
class PbMillanArcia : public BubblePointPressure
{
  Correlation_Singleton(PbMillanArcia);

  PbMillanArcia()
    : BubblePointPressure("PbMillanArcia", psia::get_instance(), 222, 3432.7)
  {
    set_author("Millán-Arcia");
    set_title("MILLÁN-ARCIA CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE");
    set_hidden();
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity");
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 53,
                  459);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("api", Api::get_instance(), "API oil gravity", 9, 20.2);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 87, 195);
    def_parameter_latex_symbol("t", "T");
    add_db("Venezuelan heavy crudes were correlated.");
    add_ref("millan:1984");
    add_ref("perez:2001");
  }

public:
  inline Quantity<psia> impl(const Quantity<Sgg>& yg,
                             const Quantity<SCF_STB>& rsb,
                             const Quantity<Api>& api,
                             const Quantity<Fahrenheit>& t) const;

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t) const
  {
    return impl(yg, rsb, api, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();
      it.next();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(yg, rsb, api, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(yg, rsb, api, t);
  }
};

extern Quantity<psia> PbMillanArcia__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
  const Quantity<Api>& api, const Quantity<Fahrenheit>& t);
extern double PbMillanArcia__correlation__valid__fct(double yg, double rsb,
                                                     double api, double t);

/** PbManucciRosales

The correlation could not be verified because the original reference is not
available. Date: August 10 2016.


*/
class PbManucciRosales : public BubblePointPressure
{
  Correlation_Singleton(PbManucciRosales);

  PbManucciRosales()
    : BubblePointPressure("PbManucciRosales", psia::get_instance(), 1230, 5035)
  {
    set_author("Manucci & Rosales");
    set_title(
      "MANUCCI & ROSALES CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE");
    set_hidden();
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity");
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 180,
                  2500);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
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
  inline Quantity<psia> impl(const Quantity<Sgg>& yg,
                             const Quantity<SCF_STB>& rsb,
                             const Quantity<Api>& api,
                             const Quantity<Fahrenheit>& t) const;

  Quantity<psia> operator()(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t) const
  {
    return impl(yg, rsb, api, t);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();
      it.next();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();

      return impl(yg, rsb, api, t);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();

    return impl(yg, rsb, api, t);
  }
};

extern Quantity<psia> PbManucciRosales__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
  const Quantity<Api>& api, const Quantity<Fahrenheit>& t);
extern double PbManucciRosales__correlation__valid__fct(double yg, double rsb,
                                                        double api, double t);

/** PbGlaso


*/
class PbGlaso : public BubblePointPressure
{
  Correlation_Singleton(PbGlaso);

  PbGlaso()
    : BubblePointPressure("PbGlaso", psia::get_instance(), 135.304, 7112.3)
  {
    set_author("Glaso");
    set_title("GLASO CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE");
    add_parameter("yg", Sgg::get_instance(), "Gas specific gravity", 0.65,
                  1.276);
    def_parameter_latex_symbol("yg", "\\gamma_g");
    add_parameter("rsb", SCF_STB::get_instance(), "Solution GOR at Pb", 90,
                  2637);
    def_parameter_latex_symbol("rsb", "R_{sbp}");
    add_parameter("api", Api::get_instance(), "API oil gravity", 22.3, 48.1);
    def_parameter_latex_symbol("api", "\\gamma_{API}");
    add_parameter("t", Fahrenheit::get_instance(), "Temperature", 80, 280);
    def_parameter_latex_symbol("t", "T");
    add_parameter("n2_concentration", MoleFraction::get_instance(),
                  "N2 concentration", Quantity<MolePercent>(0),
                  Quantity<MolePercent>(26));
    def_parameter_latex_symbol("n2_concentration", "C_{N_2}");
    add_parameter("co2_concentration", MoleFraction::get_instance(),
                  "CO2 concentration", Quantity<MolePercent>(0),
                  Quantity<MolePercent>(26));
    def_parameter_latex_symbol("co2_concentration", "C_{CO_2}");
    add_parameter("h2s_concentration", MoleFraction::get_instance(),
                  "H2S concentration", Quantity<MolePercent>(0),
                  Quantity<MolePercent>(50));
    def_parameter_latex_symbol("h2s_concentration", "C_{H_2S}");
    add_db("Based on 26 samples from the North Sea (collected from wells in "
           "the region 56 to 62°N) and 19 samples from the Middle East, "
           "Algeria, and several areas in the U.S.");
    add_db("The equation was derived from laboratory data, exclusively "
           "sampling North Sea oils. However, the author claims that the "
           "correlation should be valid for all types of gas/oil mixtures "
           "after correcting for non-hydrocarbons in the surface gases and "
           "paraffinicity of the oil.");
    add_ref("glaso:1980");
  }

public:
  inline Quantity<psia> impl(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t,
    const Quantity<MoleFraction>& n2_concentration,
    const Quantity<MoleFraction>& co2_concentration,
    const Quantity<MoleFraction>& h2s_concentration) const;

  Quantity<psia> operator()(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t,
    const Quantity<MoleFraction>& n2_concentration,
    const Quantity<MoleFraction>& co2_concentration,
    const Quantity<MoleFraction>& h2s_concentration) const
  {
    return impl(yg, rsb, api, t, n2_concentration, co2_concentration,
                h2s_concentration);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yg = it.get_curr();
      it.next();
      const auto& rsb = it.get_curr();
      it.next();
      const auto& api = it.get_curr();
      it.next();
      const auto& t = it.get_curr();
      it.next();
      const auto& n2_concentration = it.get_curr();
      it.next();
      const auto& co2_concentration = it.get_curr();
      it.next();
      const auto& h2s_concentration = it.get_curr();

      return impl(yg, rsb, api, t, n2_concentration, co2_concentration,
                  h2s_concentration);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& n2_concentration = it.get_curr();
    it.next();
    const auto& co2_concentration = it.get_curr();
    it.next();
    const auto& h2s_concentration = it.get_curr();

    return impl(yg, rsb, api, t, n2_concentration, co2_concentration,
                h2s_concentration);
  }
};

extern Quantity<psia> PbGlaso__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
  const Quantity<Api>& api, const Quantity<Fahrenheit>& t,
  const Quantity<MoleFraction>& n2_concentration,
  const Quantity<MoleFraction>& co2_concentration,
  const Quantity<MoleFraction>& h2s_concentration);
extern double PbGlaso__correlation__valid__fct(double yg, double rsb,
                                               double api, double t,
                                               double n2_concentration,
                                               double co2_concentration,
                                               double h2s_concentration);

#endif // BUBBLE_POINT_PRESSURE_H
