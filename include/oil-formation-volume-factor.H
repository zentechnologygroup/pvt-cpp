#ifndef OIL_FORMATION_VOLUME_FACTOR_H
#define OIL_FORMATION_VOLUME_FACTOR_H

#include <correlation.H>

/** BoAlmarhoun

    AL-MARHOUN CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR

    Based on 69 bottomhole fluid samples from 69 Middle East oil reservoirs.

    @see: Al-Marhoun, M.A.: "PVT Correlation for Middle East Crude
    Oils", Journal Petroleum Technology, May 1988, pp. 650-666.

    @see: Bánzer, C.: "Correlaciones Numéricas P.V.T.", Universidad
    del Zulia, 1996.

    @see: McCain, W.D., Jr. "The Properties of Petroleum Fluids", 2nd
    ed. Tulsa, OK: PennWell Books, 1990.


*/
class BoAlmarhoun : public OilFormationVolumeFactor
{
  Correlation_Singleton(BoAlmarhoun);

  BoAlmarhoun()
    : OilFormationVolumeFactor("BoAlmarhoun", RB_STB::get_instance(), 1.032,
                               1.997)
  {
    add_parameter("yg", Sgg::get_instance(), 0.752, 1.367);
    add_parameter("yo", Sg_do::get_instance(), Quantity<Api>(44.6),
                  Quantity<Api>(19.4));
    add_parameter("rs", SCF_STB::get_instance(), 26, 1602);
    add_parameter("rsb", SCF_STB::get_instance());
    add_parameter("t", Rankine::get_instance(), Quantity<Fahrenheit>(100),
                  Quantity<Fahrenheit>(258));
    add_parameter("p", psia::get_instance());
    add_parameter("pb", psia::get_instance(), 130, 3573);
    add_parameter("co", psia_1::get_instance());
  }

public:
  inline Quantity<RB_STB> impl(
    const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
    const Quantity<SCF_STB>& rs, const Quantity<SCF_STB>& rsb,
    const Quantity<Rankine>& t, const Quantity<psia>& p,
    const Quantity<psia>& pb, const Quantity<psia_1>& co) const;

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Rankine>& t,
                              const Quantity<psia>& p, const Quantity<psia>& pb,
                              const Quantity<psia_1>& co) const
  {
    return compute({ yg, yo, rs, rsb, t, p, pb, co });
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars) const
  {
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
    it.next();
    const auto& pb = it.get_curr();
    it.next();
    const auto& co = it.get_curr();

    return impl(yg, yo, rs, rsb, t, p, pb, co);
  }
};

/** BoAlShammasi

   Al-SHAMMASI CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR

    @see: Al-Shammasi, A.A.: "Bubble Point Pressure and Oil Formation
    Volume Factor Correlations", SPE 53185, 1999

    @see: McCain, W.D., Jr. "The Properties of Petroleum Fluids", 2nd
    ed. Tulsa, OK: PennWell Books, 1990.

    Based on a total of 1709 data sets (1661 data sets from 13
    different published literature papers and 48 data sets originated
    from Kuwait reservoirs from unpublished sources).

    The data available after excluding the duplicates and the repeated
    data sets for formation volume factor are 1345 data sets.

*/
class BoAlShammasi : public OilFormationVolumeFactor
{
  Correlation_Singleton(BoAlShammasi);

  BoAlShammasi()
    : OilFormationVolumeFactor("BoAlShammasi", RB_STB::get_instance(), 1.02,
                               2.916)
  {
    add_parameter("yg", Sgg::get_instance(), 0.51, 3.44);
    add_parameter("yo", Sg_do::get_instance(), Quantity<Api>(63.7),
                  Quantity<Api>(6));
    add_parameter("rs", SCF_STB::get_instance(), 6, 3298.6);
    add_parameter("rsb", SCF_STB::get_instance());
    add_parameter("t", Fahrenheit::get_instance(), 74, 341.6);
    add_parameter("p", psia::get_instance());
    add_parameter("pb", psia::get_instance(), 31.7, 7127);
    add_parameter("co", psia_1::get_instance());
  }

public:
  inline Quantity<RB_STB> impl(
    const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
    const Quantity<SCF_STB>& rs, const Quantity<SCF_STB>& rsb,
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<psia>& pb, const Quantity<psia_1>& co) const;

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<psia>& p, const Quantity<psia>& pb,
                              const Quantity<psia_1>& co) const
  {
    return compute({ yg, yo, rs, rsb, t, p, pb, co });
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars) const
  {
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
    it.next();
    const auto& pb = it.get_curr();
    it.next();
    const auto& co = it.get_curr();

    return impl(yg, yo, rs, rsb, t, p, pb, co);
  }
};

/** BoDeGhetto

    DE GHETTO CORRELATION FOR EXTRA-HEAVY AND HEAVY OILS
    (VASQUEZ & BEGGS CORRELATION), CALCULATION OF OIL FORMATION VOLUME FACTOR

    @see: Giambattista De Ghetto, Francesco Paone and Marco
    Villa. "Pressure-Volume-Temperature Correlations for Heavy and
    Extra Heavy Oils," SPE 30316, 1995

    Based on 1200 measured data points of 63 heavy and extra-heavy oil
    samples obtained from the Mediterranean Basin, Africa and the
    Persian Gulf.

    Oil samples have been divided in two different API gravity
    classes: extra-heavy oils for °API<=10, heavy oils for
    10<°API<=22.3.

*/
class BoDeGhetto : public OilFormationVolumeFactor
{
  Correlation_Singleton(BoDeGhetto);

  BoDeGhetto()
    : OilFormationVolumeFactor("BoDeGhetto", RB_STB::get_instance(), 1.057,
                               1.362)
  {
    add_parameter("yg", Sgg::get_instance(), 0.623, 1.517);
    add_parameter("api", Api::get_instance(), 6, 22.3);
    add_parameter("rs", SCF_STB::get_instance(), 17.21, 640.25);
    add_parameter("rsb", SCF_STB::get_instance());
    add_parameter("t", Fahrenheit::get_instance(), 131.4, 250.7);
    add_parameter("tsep", Fahrenheit::get_instance(), 59, 177.8);
    add_parameter("p", psia::get_instance(), 1038.49, 7411.54);
    add_parameter("pb", psia::get_instance(), 208.86, 4021.96);
    add_parameter("psep", psia::get_instance(), 14.5, 752.2);
    add_parameter("co", psia_1::get_instance(), 1.057, 1.362);
  }

public:
  inline Quantity<RB_STB> impl(
    const Quantity<Sgg>& yg, const Quantity<Api>& api,
    const Quantity<SCF_STB>& rs, const Quantity<SCF_STB>& rsb,
    const Quantity<Fahrenheit>& t, const Quantity<Fahrenheit>& tsep,
    const Quantity<psia>& p, const Quantity<psia>& pb,
    const Quantity<psia>& psep, const Quantity<psia_1>& co) const;

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<Fahrenheit>& tsep,
                              const Quantity<psia>& p, const Quantity<psia>& pb,
                              const Quantity<psia>& psep,
                              const Quantity<psia_1>& co) const
  {
    return compute({ yg, api, rs, rsb, t, tsep, p, pb, psep, co });
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars) const
  {
    auto pars = verify_preconditions(__pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
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
    it.next();
    const auto& co = it.get_curr();

    return impl(yg, api, rs, rsb, t, tsep, p, pb, psep, co);
  }
};

/** BoDindorukChristman

    DINDORUK & CHRISTMAN CORRELATION, CALCULATION OF OIL
    FORMATION VOLUME FACTOR

    @see: Dindoruk, B. and Christman, P.G.: "Properties and Viscosity
    Correlations for Gulf of Mexico Oils", SPE 89030, 2004

    @see: McCain, W.D., Jr.: "The Properties of Petroleum Fluids", 2nd
    ed. Tulsa, OK: PennWell Books, 1990.

    Based on approximately 100 PVT reports from the Gulf of Mexico.

*/
class BoDindorukChristman : public OilFormationVolumeFactor
{
  Correlation_Singleton(BoDindorukChristman);

  BoDindorukChristman()
    : OilFormationVolumeFactor("BoDindorukChristman", RB_STB::get_instance(),
                               1.0845, 2.7381)
  {
    add_parameter("yg", Sgg::get_instance(), 0.6017, 1.027);
    add_parameter("api", Api::get_instance(), 14.7, 40);
    add_parameter("rs", SCF_STB::get_instance());
    add_parameter("rsb", SCF_STB::get_instance(), 133, 3050);
    add_parameter("t", Fahrenheit::get_instance(), 117, 276);
    add_parameter("tsep", Fahrenheit::get_instance());
    add_parameter("p", psia::get_instance());
    add_parameter("pb", psia::get_instance());
    add_parameter("co", psia_1::get_instance());
  }

public:
  inline Quantity<RB_STB> impl(
    const Quantity<Sgg>& yg, const Quantity<Api>& api,
    const Quantity<SCF_STB>& rs, const Quantity<SCF_STB>& rsb,
    const Quantity<Fahrenheit>& t, const Quantity<Fahrenheit>& tsep,
    const Quantity<psia>& p, const Quantity<psia>& pb,
    const Quantity<psia_1>& co) const;

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<Fahrenheit>& tsep,
                              const Quantity<psia>& p, const Quantity<psia>& pb,
                              const Quantity<psia_1>& co) const
  {
    return compute({ yg, api, rs, rsb, t, tsep, p, pb, co });
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars) const
  {
    auto pars = verify_preconditions(__pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
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
    const auto& co = it.get_curr();

    return impl(yg, api, rs, rsb, t, tsep, p, pb, co);
  }
};

/** BoDoklaOsman

    DOKLA & OSMAN CORRELATION, CALCULATION OF OIL FORMATION
    VOLUME FACTOR

    @see: Dokla, M.E. y Osman, M.E.: "Correlation of PVT Properties
    for UAE Crudes",Trans AIME (1992) 293, 41-46

    @see: Bánzer, C.: "Correlaciones Numéricas P.V.T.", Universidad del
    Zulia, 1996.

    @see: McCain, W.D., Jr.: "The Properties of Petroleum Fluids", 2nd
    ed. Tulsa, OK: PennWell Books, 1990.

        Based on 51 bottomhole samples from UAE reservoirs.


*/
class BoDoklaOsman : public OilFormationVolumeFactor
{
  Correlation_Singleton(BoDoklaOsman);

  BoDoklaOsman()
    : OilFormationVolumeFactor("BoDoklaOsman", RB_STB::get_instance(), 1.216,
                               2.493)
  {
    add_parameter("yg", Sgg::get_instance(), 0.789, 1.29);
    add_parameter("yo", Sg_do::get_instance(), 0.8236, 0.886);
    add_parameter("rs", SCF_STB::get_instance());
    add_parameter("rsb", SCF_STB::get_instance(), 181, 2266);
    add_parameter("t", Rankine::get_instance(), Quantity<Fahrenheit>(190),
                  Quantity<Fahrenheit>(275));
    add_parameter("p", psia::get_instance());
    add_parameter("pb", psia::get_instance(), 590, 4640);
    add_parameter("co", psia_1::get_instance());
  }

public:
  inline Quantity<RB_STB> impl(
    const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
    const Quantity<SCF_STB>& rs, const Quantity<SCF_STB>& rsb,
    const Quantity<Rankine>& t, const Quantity<psia>& p,
    const Quantity<psia>& pb, const Quantity<psia_1>& co) const;

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Rankine>& t,
                              const Quantity<psia>& p, const Quantity<psia>& pb,
                              const Quantity<psia_1>& co) const
  {
    return compute({ yg, yo, rs, rsb, t, p, pb, co });
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars) const
  {
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
    it.next();
    const auto& pb = it.get_curr();
    it.next();
    const auto& co = it.get_curr();

    return impl(yg, yo, rs, rsb, t, p, pb, co);
  }
};

/** BoGlaso

     GLASO CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR

    @see: Glaso, O.: "Generalized Pressure-Volume-Temperature
    Correlations," Journal of Petroleum Technology, 1980

    @see: Bánzer, C.: "Correlaciones Numéricas P.V.T.", Universidad
    del Zulia, 1996.

    @see: McCain, W.D., Jr.: "The Properties of Petroleum Fluids", 2nd
    ed. Tulsa, OK: PennWell Books, 1990.

    Based on 26 samples from the North Sea (collected from wells in
    the region 56 to 62°N) and 19 samples from the Middle East,
    Algeria, and several areas in the U.S.


*/
class BoGlaso : public OilFormationVolumeFactor
{
  Correlation_Singleton(BoGlaso);

  BoGlaso()
    : OilFormationVolumeFactor("BoGlaso", RB_STB::get_instance(), 1.087, 2.588)
  {
    add_parameter("yg", Sgg::get_instance(), 0.65, 1.276);
    add_parameter("yo", Sg_do::get_instance(), Quantity<Api>(48.1),
                  Quantity<Api>(22.3));
    add_parameter("rs", SCF_STB::get_instance(), 90, 2637);
    add_parameter("rsb", SCF_STB::get_instance());
    add_parameter("t", Fahrenheit::get_instance(), 80, 280);
    add_parameter("p", psia::get_instance());
    add_parameter("pb", psia::get_instance(), 150, 7127);
    add_parameter("co", psia_1::get_instance());
  }

public:
  inline Quantity<RB_STB> impl(
    const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
    const Quantity<SCF_STB>& rs, const Quantity<SCF_STB>& rsb,
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<psia>& pb, const Quantity<psia_1>& co) const;

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<psia>& p, const Quantity<psia>& pb,
                              const Quantity<psia_1>& co) const
  {
    return compute({ yg, yo, rs, rsb, t, p, pb, co });
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars) const
  {
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
    it.next();
    const auto& pb = it.get_curr();
    it.next();
    const auto& co = it.get_curr();

    return impl(yg, yo, rs, rsb, t, p, pb, co);
  }
};

/** BoHanafy

     HANAFY CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR

    @see: H.H. Hanafy, S.M. Macary, Y.M. ElNady, A.A. Bayomi and
    M.H. El Batanony. "A New Approach for Predicting the Crude Oil
    Properties," SPE 37439, 1997

    Based on experimental PVT data of 324 fluid samples taken from 176
    wells located in 75 fields. This data represents 15 productive
    zones of 123 reservoirs distributed along three different regions
    of Egypt, including the Gulf of Suez, Western Desert, and Sinai.


*/
class BoHanafy : public OilFormationVolumeFactor
{
  Correlation_Singleton(BoHanafy);

  BoHanafy()
    : OilFormationVolumeFactor("BoHanafy", RB_STB::get_instance(), 1.032, 4.35)
  {
    add_parameter("rs", SCF_STB::get_instance(), 7, 4272);
    add_parameter("rsb", SCF_STB::get_instance());
    add_parameter("p", psia::get_instance());
    add_parameter("pb", psia::get_instance(), 36, 5003);
    add_parameter("co", psia_1::get_instance(), 1.032e-06, 4.35e-06);
  }

public:
  inline Quantity<RB_STB> impl(const Quantity<SCF_STB>& rs,
                               const Quantity<SCF_STB>& rsb,
                               const Quantity<psia>& p,
                               const Quantity<psia>& pb,
                               const Quantity<psia_1>& co) const;

  Quantity<RB_STB> operator()(const Quantity<SCF_STB>& rs,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<psia>& p, const Quantity<psia>& pb,
                              const Quantity<psia_1>& co) const
  {
    return compute({ rs, rsb, p, pb, co });
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars) const
  {
    auto pars = verify_preconditions(__pars);

    auto it = pars.get_it();
    const auto& rs = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();
    it.next();
    const auto& co = it.get_curr();

    return impl(rs, rsb, p, pb, co);
  }
};

/** BoKartoatmodjo

     KARTOATMODJO & SCHMIDT CORRELATION, CALCULATION OF OIL
    FORMATION VOLUME FACTOR

    @see: R.S. Kartoatmodjo and Z. Schmidt. "New Correlations For
    Crude Oil Physical Properties", SPE 23556, 1991.

    Based on a set of 5392 data points, which represent 740 different
    crude oil samples.

    The data bank was collected from PVT reports and literature. The
    first major source was from South East Asia, mainly
    Indonesia. The second source was North America, including the
    offshore area. The rest came from the Middle East and Latin America.

*/
class BoKartoatmodjo : public OilFormationVolumeFactor
{
  Correlation_Singleton(BoKartoatmodjo);

  BoKartoatmodjo()
    : OilFormationVolumeFactor("BoKartoatmodjo", RB_STB::get_instance(), 1.007,
                               2.144)
  {
    add_parameter("yg", Sgg::get_instance(), 0.4824, 1.668);
    add_parameter("yo", Sg_do::get_instance(), Quantity<Api>(59),
                  Quantity<Api>(14.4));
    add_parameter("rs", SCF_STB::get_instance(), 0, 2890);
    add_parameter("rsb", SCF_STB::get_instance());
    add_parameter("t", Fahrenheit::get_instance(), 75, 320);
    add_parameter("tsep", Fahrenheit::get_instance(), 38, 294);
    add_parameter("p", psia::get_instance());
    add_parameter("pb", psia::get_instance(), 14.7, 6054.7);
    add_parameter("psep", psia::get_instance(), 14.7, 1414.7);
    add_parameter("co", psia_1::get_instance());
  }

public:
  inline Quantity<RB_STB> impl(
    const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
    const Quantity<SCF_STB>& rs, const Quantity<SCF_STB>& rsb,
    const Quantity<Fahrenheit>& t, const Quantity<Fahrenheit>& tsep,
    const Quantity<psia>& p, const Quantity<psia>& pb,
    const Quantity<psia>& psep, const Quantity<psia_1>& co) const;

  Quantity<RB_STB> operator()(
    const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
    const Quantity<SCF_STB>& rs, const Quantity<SCF_STB>& rsb,
    const Quantity<Fahrenheit>& t, const Quantity<Fahrenheit>& tsep,
    const Quantity<psia>& p, const Quantity<psia>& pb,
    const Quantity<psia>& psep, const Quantity<psia_1>& co) const
  {
    return compute({ yg, yo, rs, rsb, t, tsep, p, pb, psep, co });
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars) const
  {
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
    const auto& tsep = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();
    it.next();
    const auto& psep = it.get_curr();
    it.next();
    const auto& co = it.get_curr();

    return impl(yg, yo, rs, rsb, t, tsep, p, pb, psep, co);
  }
};

/** BoLasater

     LASATER CORRELATION (STANDING CORRELATION APPLIED FOR
    LASATER), CALCULATION OF OIL FORMATION VOLUME FACTOR

    @see: Lasater, J.A.: "Bubble Point Pressure Correlation", Trans
    AIME (1958) 213, 379-381

    @see: Standing, M.B.: "A Pressure-Volume-Temperature Correlation
    for Mixtures of California Oil and Gases", American Petroleum
    Institute, 47-275 API, 1947.

    @see: Beggs, H.D. "Oil System Correlations", Petroleum Engineering
    Handbook, H.C. SPE, Richardson, TX (1987). Cap 22. Pag 22-6.

    @see: McCain, W.D., Jr.: "The Properties of Petroleum Fluids", 2nd
    ed. Tulsa, OK: PennWell Books, 1990.

    Lasater correlation is based on data obtained from Canada, Western
    and Mid-Continental United States, and South America.

    Standing correlation for formation volumes of bubble point liquids
    is based on data collected from 22 different Californian
    crude-oil-natural-gas mixtures (Western United States).

*/
class BoLasater : public OilFormationVolumeFactor
{
  Correlation_Singleton(BoLasater);

  BoLasater()
    : OilFormationVolumeFactor("BoLasater", RB_STB::get_instance())
  {
    add_parameter("yg", Sgg::get_instance());
    add_parameter("yo", Sg_do::get_instance(), Quantity<Api>(51.1),
                  Quantity<Api>(17.9));
    add_parameter("rs", SCF_STB::get_instance());
    add_parameter("rsb", SCF_STB::get_instance(), 3, 2905);
    add_parameter("t", Fahrenheit::get_instance(), 82, 272);
    add_parameter("p", psia::get_instance());
    add_parameter("pb", psia::get_instance(), 48, 5780);
    add_parameter("co", psia_1::get_instance());
  }

public:
  inline Quantity<RB_STB> impl(
    const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
    const Quantity<SCF_STB>& rs, const Quantity<SCF_STB>& rsb,
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<psia>& pb, const Quantity<psia_1>& co) const;

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<psia>& p, const Quantity<psia>& pb,
                              const Quantity<psia_1>& co) const
  {
    return compute({ yg, yo, rs, rsb, t, p, pb, co });
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars) const
  {
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
    it.next();
    const auto& pb = it.get_curr();
    it.next();
    const auto& co = it.get_curr();

    return impl(yg, yo, rs, rsb, t, p, pb, co);
  }
};

/** BoPetroskyFarshad

     PETROSKY & FARSHAD CORRELATION, CALCULATION OF OIL
    FORMATION VOLUME FACTOR

    @see: G.E. Petrosky Jr. and
    F.F. Farshad. "Pressure-Volume-Temperature Correlations for
    Gulf of Mexico Crude Oils," SPE 26644, 1993

    Based on 81 laboratory PVT analyses. Fluid samples were obtained
    from reservoirs located offshore Texas and Louisiana.

    Producing areas from Galveston Island, eastward, through Main Pass
    are represented.

    Many Gulf of Mexico crudes can be characterized as naphthenic or
    biodegraded oils.

*/
class BoPetroskyFarshad : public OilFormationVolumeFactor
{
  Correlation_Singleton(BoPetroskyFarshad);

  BoPetroskyFarshad()
    : OilFormationVolumeFactor("BoPetroskyFarshad", RB_STB::get_instance(),
                               1.1178, 1.6229)
  {
    add_parameter("yg", Sgg::get_instance(), 0.5781, 0.8519);
    add_parameter("yo", Sg_do::get_instance(), Quantity<Api>(45),
                  Quantity<Api>(16.3));
    add_parameter("rs", SCF_STB::get_instance(), 217, 1406);
    add_parameter("rsb", SCF_STB::get_instance());
    add_parameter("t", Fahrenheit::get_instance(), 114, 288);
    add_parameter("p", psia::get_instance());
    add_parameter("pb", psia::get_instance(), 1574, 6523);
    add_parameter("co", psia_1::get_instance());
  }

public:
  inline Quantity<RB_STB> impl(
    const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
    const Quantity<SCF_STB>& rs, const Quantity<SCF_STB>& rsb,
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<psia>& pb, const Quantity<psia_1>& co) const;

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<psia>& p, const Quantity<psia>& pb,
                              const Quantity<psia_1>& co) const
  {
    return compute({ yg, yo, rs, rsb, t, p, pb, co });
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars) const
  {
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
    it.next();
    const auto& pb = it.get_curr();
    it.next();
    const auto& co = it.get_curr();

    return impl(yg, yo, rs, rsb, t, p, pb, co);
  }
};

/** BoStanding

     STANDING CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR

    @see: M.B. Standing. "A Pressure-Volume-Temperature Correlation for
    Mixtures of California Oil and Gases" Drill. & Prod. Prac.,Beal C.
    The Viscosity-Temperature Chart 0361-39 Nat. Bur . Stds Circular C461-1947

    @see: Al-Shammasi, A.A.: "A Review of Bubblepoint Pressure and Oil
    Formation Volume Factor Correlations", SPE 71302, 2001.

    @see: McCain, W.D., Jr.: "The Properties of Petroleum Fluids", 2nd
    ed. Tulsa, OK: PennWell Books, 1990.

    Based on 105 experimentally determined bubble point pressures from
    22 different Californian crude-oil-natural-gas mixtures.


*/
class BoStanding : public OilFormationVolumeFactor
{
  Correlation_Singleton(BoStanding);

  BoStanding()
    : OilFormationVolumeFactor("BoStanding", RB_STB::get_instance(), 1.024,
                               2.15)
  {
    add_parameter("yg", Sgg::get_instance(), 0.59, 0.95);
    add_parameter("yo", Sg_do::get_instance(), Quantity<Api>(63.8),
                  Quantity<Api>(16.5));
    add_parameter("rs", SCF_STB::get_instance(), 20, 1425);
    add_parameter("rsb", SCF_STB::get_instance());
    add_parameter("t", Fahrenheit::get_instance(), 100, 258);
    add_parameter("p", psia::get_instance());
    add_parameter("pb", psia::get_instance());
    add_parameter("co", psia_1::get_instance());
  }

public:
  inline Quantity<RB_STB> impl(
    const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
    const Quantity<SCF_STB>& rs, const Quantity<SCF_STB>& rsb,
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<psia>& pb, const Quantity<psia_1>& co) const;

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<psia>& p, const Quantity<psia>& pb,
                              const Quantity<psia_1>& co) const
  {
    return compute({ yg, yo, rs, rsb, t, p, pb, co });
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars) const
  {
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
    it.next();
    const auto& pb = it.get_curr();
    it.next();
    const auto& co = it.get_curr();

    return impl(yg, yo, rs, rsb, t, p, pb, co);
  }
};

/** BoTotalCFP

     TOTAL CFP CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR

    @see: TOTAL Compaigne Francaise des Petroles: "Proyectos de
    Inyección de Fluidos - Correlaciones PVT para Crudos del Oriente de
    Venezuela", S.A., MENEVEN. Septiembre de 1996

    @see: Bánzer, C.: "Correlaciones Numéricas P.V.T.", Universidad
    del Zulia, 1996.

*/
class BoTotalCFP : public OilFormationVolumeFactor
{
  Correlation_Singleton(BoTotalCFP);

  BoTotalCFP()
    : OilFormationVolumeFactor("BoTotalCFP", RB_STB::get_instance())
  {
    add_parameter("yg", Sgg::get_instance());
    add_parameter("api", Api::get_instance());
    add_parameter("rs", SCF_STB::get_instance());
    add_parameter("rsb", SCF_STB::get_instance());
    add_parameter("t", Fahrenheit::get_instance());
    add_parameter("p", psia::get_instance());
    add_parameter("pb", psia::get_instance());
    add_parameter("co", psia_1::get_instance());
  }

public:
  inline Quantity<RB_STB> impl(
    const Quantity<Sgg>& yg, const Quantity<Api>& api,
    const Quantity<SCF_STB>& rs, const Quantity<SCF_STB>& rsb,
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<psia>& pb, const Quantity<psia_1>& co) const;

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<psia>& p, const Quantity<psia>& pb,
                              const Quantity<psia_1>& co) const
  {
    return compute({ yg, api, rs, rsb, t, p, pb, co });
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars) const
  {
    auto pars = verify_preconditions(__pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();
    it.next();
    const auto& co = it.get_curr();

    return impl(yg, api, rs, rsb, t, p, pb, co);
  }
};

/** BoVasquezBeggs

     VASQUEZ & BEGGS CORRELATION, CALCULATION OF OIL
    FORMATION VOLUME FACTOR

    @see: M.E. Vasquez and H.D. Beggs. "Correlations for Fluid
    Physical Property Prediction," JPT 968-70, June 1980

    @see: Al-Shammasi, A.A.: "A Review of Bubblepoint Pressure and Oil
    Formation Volume Factor Correlations", SPE 71302, 2001.

    @see: C. Bánzer. "Correlaciones Numéricas P.V.T.", Universidad del
    Zulia, 1996.

*/
class BoVasquezBeggs : public OilFormationVolumeFactor
{
  Correlation_Singleton(BoVasquezBeggs);

  BoVasquezBeggs()
    : OilFormationVolumeFactor("BoVasquezBeggs", RB_STB::get_instance(), 1.028,
                               2.226)
  {
    add_parameter("yg", Sgg::get_instance(), 0.511, 1.35);
    add_parameter("api", Api::get_instance(), 15.3, 59.3);
    add_parameter("rs", SCF_STB::get_instance(), 0, 2199);
    add_parameter("rsb", SCF_STB::get_instance());
    add_parameter("t", Fahrenheit::get_instance(), 75, 294);
    add_parameter("tsep", Fahrenheit::get_instance(), 76, 150);
    add_parameter("p", psia::get_instance());
    add_parameter("pb", psia::get_instance());
    add_parameter("psep", psia::get_instance(), 60, 565);
    add_parameter("co", psia_1::get_instance());
  }

public:
  inline Quantity<RB_STB> impl(
    const Quantity<Sgg>& yg, const Quantity<Api>& api,
    const Quantity<SCF_STB>& rs, const Quantity<SCF_STB>& rsb,
    const Quantity<Fahrenheit>& t, const Quantity<Fahrenheit>& tsep,
    const Quantity<psia>& p, const Quantity<psia>& pb,
    const Quantity<psia>& psep, const Quantity<psia_1>& co) const;

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<Fahrenheit>& tsep,
                              const Quantity<psia>& p, const Quantity<psia>& pb,
                              const Quantity<psia>& psep,
                              const Quantity<psia_1>& co) const
  {
    return compute({ yg, api, rs, rsb, t, tsep, p, pb, psep, co });
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars) const
  {
    auto pars = verify_preconditions(__pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& api = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
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
    it.next();
    const auto& co = it.get_curr();

    return impl(yg, api, rs, rsb, t, tsep, p, pb, psep, co);
  }
};

#endif // OIL_FORMATION_VOLUME_FACTOR_H
