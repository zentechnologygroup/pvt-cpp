#ifndef SOLUTION_GAS_OIL_RELATION_1_H
#define SOLUTION_GAS_OIL_RELATION_1_H

#include <correlation.H>

/** RsStanding

    STANDING CORRELATION, CALCULATION OF SOLUTION GOR

    Based on 105 experimentally determined bubble point pressures
    from 22 different Californian crude-oil-natural-gas mixtures.

    @see: M.B. Standing. "A Pressure-Volume-Temperature
    Correlation for Mixtures of California Oil and Gases , " Drill. &
    Prod. Prac.,Beal C. The Viscosity-Temperature Chart 0361-39
    Nat. Bur. Stds Circular C461-1947"

    @see: C. Bánzer. "Correlaciones Numéricas P.V.T.", Universidad
    del Zulia, 1996.
*/
class RsStanding : public SolutionGasOilRelation
{
  Correlation_Singleton(RsStanding);

  RsStanding()
    : SolutionGasOilRelation("RsStanding", SCF_STB::get_instance())
  {
    add_parameter("yg", Sgg::get_instance(), 0.59, 0.95);
    add_parameter("pb", psia::get_instance(), 130, 700);
    add_parameter("p", psia::get_instance());
    add_parameter("api", Api::get_instance(), 16.5, 63.8);
    add_parameter("t", Fahrenheit::get_instance(), 100, 258);
    add_parameter("rsb", SCF_STB::get_instance(), 20, 1425);
  }

public:
  inline Quantity<SCF_STB> impl(const Quantity<Sgg>& yg,
                                const Quantity<psia>& pb,
                                const Quantity<psia>& p,
                                const Quantity<Api>& api,
                                const Quantity<Fahrenheit>& t,
                                const Quantity<SCF_STB>& rsb) const;

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg,
                               const Quantity<psia>& pb,
                               const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<SCF_STB>& rsb) const
  {
    return compute({ yg, pb, p, api, t, rsb });
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars) const
  {
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
};

/** RsTotalCFP

    TOTAL CFP CORRELATION, CALCULATION OF SOLUTION GOR

    Based on 336 PVT analyses of crude oil from eastern Venezuela.

    @see: TOTAL Compagnie Francaise Des Petroles CFP: " Proyectos de
    Inyeccion de Fluidos Correlaciones PVT para Crudos del Oriente de
    Venezuela" S.A. MENEVEN Sep. 1983.

    @see: C. Bánzer. "Correlaciones Numéricas P.V.T.", Universidad del Zulia,
   1996.

*/
class RsTotalCFP : public SolutionGasOilRelation
{
  Correlation_Singleton(RsTotalCFP);

  RsTotalCFP()
    : SolutionGasOilRelation("RsTotalCFP", SCF_STB::get_instance())
  {
    add_parameter("yg", Sgg::get_instance());
    add_parameter("pb", psia::get_instance());
    add_parameter("p", psia::get_instance());
    add_parameter("api", Api::get_instance(), 0, 45);
    add_parameter("t", Fahrenheit::get_instance());
    add_parameter("rsb", SCF_STB::get_instance());
  }

public:
  inline Quantity<SCF_STB> impl(const Quantity<Sgg>& yg,
                                const Quantity<psia>& pb,
                                const Quantity<psia>& p,
                                const Quantity<Api>& api,
                                const Quantity<Fahrenheit>& t,
                                const Quantity<SCF_STB>& rsb) const;

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg,
                               const Quantity<psia>& pb,
                               const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<SCF_STB>& rsb) const
  {
    return compute({ yg, pb, p, api, t, rsb });
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars) const
  {
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
};

/** RsVasquezBeggs

    VASQUEZ & BEGGS CORRELATION, CALCULATION OF SOLUTION GOR

    Based on more than 600 laboratory PVT analyses from fields all
    over the world.

    The equation for solution GOR was obtained by regression analysis
    using 5008 measured data points.

    @see: M.E. Vasquez and H.D. Beggs. "Correlations for Fluid
    Physical Property Prediction," JPT 968-70, June 1980.

    @see: Al-Shammasi, A.A.: "A Review of Bubblepoint Pressure and Oil
    Formation Volume Factor Correlations", SPE 71302, 2001.

    @see: C. Bánzer. "Correlaciones Numéricas P.V.T.", Universidad del
    Zulia, 1996.

    @note: The gas gravity is corrected if it resulted from a
    first-stage separation at a pressure other than 100 psig
    (approximately 114.7 psia).

*/
class RsVasquezBeggs : public SolutionGasOilRelation
{
  Correlation_Singleton(RsVasquezBeggs);

  RsVasquezBeggs()
    : SolutionGasOilRelation("RsVasquezBeggs", SCF_STB::get_instance())
  {
    add_parameter("yg", Sgg::get_instance(), 0.511, 1.35);
    add_parameter("pb", psia::get_instance(), 15, 6055);
    add_parameter("p", psia::get_instance());
    add_parameter("api", Api::get_instance(), 15.3, 59.3);
    add_parameter("t", Fahrenheit::get_instance(), 75, 294);
    add_parameter("tsep", Fahrenheit::get_instance(), 76, 150);
    add_parameter("psep", psia::get_instance(), 60, 595);
    add_parameter("rsb", SCF_STB::get_instance(), 0, 2199);
  }

public:
  inline Quantity<SCF_STB> impl(
    const Quantity<Sgg>& yg, const Quantity<psia>& pb, const Quantity<psia>& p,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t,
    const Quantity<Fahrenheit>& tsep, const Quantity<psia>& psep,
    const Quantity<SCF_STB>& rsb) const;

  Quantity<SCF_STB> operator()(
    const Quantity<Sgg>& yg, const Quantity<psia>& pb, const Quantity<psia>& p,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t,
    const Quantity<Fahrenheit>& tsep, const Quantity<psia>& psep,
    const Quantity<SCF_STB>& rsb) const
  {
    return compute({ yg, pb, p, api, t, tsep, psep, rsb });
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars) const
  {
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
    const auto& tsep = it.get_curr();
    it.next();
    const auto& psep = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();

    return impl(yg, pb, p, api, t, tsep, psep, rsb);
  }
};

/** RsVelarde

    VELARDE CORRELATION, CALCULATION OF SOLUTION GOR

    Based on 195 PVT reports, including 2097 data sets.

     @see: J. Velarde, T.A. Blasingame and W.D. McCain, Jr.
    "Correlation of Black Oil Properties at Pressures below
     Bubble Point Pressure - A New Approach,"
    The Petroleum Society 97-93, 1997.

*/
class RsVelarde : public SolutionGasOilRelation
{
  Correlation_Singleton(RsVelarde);

  RsVelarde()
    : SolutionGasOilRelation("RsVelarde", SCF_STB::get_instance())
  {
    add_parameter("yg", Sgg::get_instance(), 0.561, 1.101);
    add_parameter("pb", psig::get_instance(), Quantity<psia>(106),
                  Quantity<psia>(5312));
    add_parameter("p", psig::get_instance());
    add_parameter("api", Api::get_instance(), 11.6, 53.4);
    add_parameter("t", Fahrenheit::get_instance(), 70, 307);
    add_parameter("rsb", SCF_STB::get_instance(), 102, 1808);
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
    return compute({ yg, pb, p, api, t, rsb });
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars) const
  {
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
};

/** RsCegarra

    CEGARRA CORRELATION (MODIFIED STANDING CORRELATION),
    CALCULATION OF SOLUTION GOR

    Based on 375 consistent PVT analyses. The data bank was compiled
    from 8394 fields of eastern Venezuela subdivided into 5192 fields
    of light oil and 3202 fields of medium oil. The correlation takes
    the functional form of Standing.

    @see: Cegarra, L., 2007: "Correlación para el cálculo de la
    presión de burbuja de los yacimientos de crudo liviano y mediano
    del Oriente de Venezuela". Trabajo especial de grado. Universidad
    del Zulia


@see: M.B. Standing. "A Pressure-Volume-Temperature Correlation for
    Mixtures of California Oil and Gases , " Drill. & Prod. Prac.,Beal
    C. The Viscosity-Temperature Chart 0361-39 Nat. Bur . Stds
    Circular C461-1947"

*/
class RsCegarra : public SolutionGasOilRelation
{
  Correlation_Singleton(RsCegarra);

  RsCegarra()
    : SolutionGasOilRelation("RsCegarra", SCF_STB::get_instance())
  {
    add_parameter("yg", Sgg::get_instance(), 0.654, 1.337);
    add_parameter("pb", psia::get_instance(), 735, 7303);
    add_parameter("p", psia::get_instance());
    add_parameter("api", Api::get_instance(), 22, 41.9);
    add_parameter("t", Fahrenheit::get_instance(), 117, 354);
    add_parameter("rsb", SCF_STB::get_instance(), 135, 4290);
  }

public:
  inline Quantity<SCF_STB> impl(const Quantity<Sgg>& yg,
                                const Quantity<psia>& pb,
                                const Quantity<psia>& p,
                                const Quantity<Api>& api,
                                const Quantity<Fahrenheit>& t,
                                const Quantity<SCF_STB>& rsb) const;

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg,
                               const Quantity<psia>& pb,
                               const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<SCF_STB>& rsb) const
  {
    return compute({ yg, pb, p, api, t, rsb });
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars) const
  {
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
};

/** RsPerezML

    PEREZ ET AL. CORRELATION (MODIFIED MILLÁN-ARCIA
    CORRELATION), CALCULATION OF SOLUTION GOR


    @see: C.A. Heny, V.L. Pérez, M.E. Lago, 2001: Evaluación y
    generación de correlaciones PVT para crudos extrapesados
    venezolanos. Informe Técnico INT-8713,2001. Proyecto: Estudio y
    Modelaje de Procesos de Recuperación de Crudos Pesados. Los Teques,
    Octubre de 2001

*/
class RsPerezML : public SolutionGasOilRelation
{
  Correlation_Singleton(RsPerezML);

  RsPerezML()
    : SolutionGasOilRelation("RsPerezML", SCF_STB::get_instance())
  {
    add_parameter("yg", Sgg::get_instance());
    add_parameter("pb", psia::get_instance(), 405, 1335);
    add_parameter("p", psia::get_instance());
    add_parameter("api", Api::get_instance(), 6.4, 12.9);
    add_parameter("t", Fahrenheit::get_instance(), 112, 300);
    add_parameter("rsb", SCF_STB::get_instance(), 38, 121);
  }

public:
  inline Quantity<SCF_STB> impl(const Quantity<Sgg>& yg,
                                const Quantity<psia>& pb,
                                const Quantity<psia>& p,
                                const Quantity<Api>& api,
                                const Quantity<Fahrenheit>& t,
                                const Quantity<SCF_STB>& rsb) const;

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg,
                               const Quantity<psia>& pb,
                               const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<SCF_STB>& rsb) const
  {
    return compute({ yg, pb, p, api, t, rsb });
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars) const
  {
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
};

/** RsMillanArcia

    MILLÁN-ARCIA CORRELATION, CALCULATION OF SOLUTION GOR

    Venezuelan heavy crudes,

    @see: E.A. Millán-Arcia, 1984: Correlaciones para estimar el
    comportamiento PVT de crudos pesados venezolanos. I Jornadas
    Técnicas Corpoven

    @see: C.A. Heny, V.L. Pérez, M.E. Lago, 2001: Evaluación y
    generación de correlaciones PVT para crudos extrapesados
    venezolanos. Informe Técnico INT-8713,2001. Proyecto: Estudio y
    Modelaje de Procesos de Recuperación de Crudos Pesados. Los Teques,
    Octubre de 2001

*/
class RsMillanArcia : public SolutionGasOilRelation
{
  Correlation_Singleton(RsMillanArcia);

  RsMillanArcia()
    : SolutionGasOilRelation("RsMillanArcia", SCF_STB::get_instance())
  {
    add_parameter("yg", Sgg::get_instance());
    add_parameter("pb", psia::get_instance(), 222, 3432.7);
    add_parameter("p", psia::get_instance());
    add_parameter("api", Api::get_instance(), 9, 20.2);
    add_parameter("t", Fahrenheit::get_instance(), 87, 195);
    add_parameter("rsb", SCF_STB::get_instance(), 53, 459);
  }

public:
  inline Quantity<SCF_STB> impl(const Quantity<Sgg>& yg,
                                const Quantity<psia>& pb,
                                const Quantity<psia>& p,
                                const Quantity<Api>& api,
                                const Quantity<Fahrenheit>& t,
                                const Quantity<SCF_STB>& rsb) const;

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg,
                               const Quantity<psia>& pb,
                               const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<SCF_STB>& rsb) const
  {
    return compute({ yg, pb, p, api, t, rsb });
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars) const
  {
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
};

/** RsManucciRosales

    MANUCCI-ROSALES CORRELATION, CALCULATION OF SOLUTION GOR

    Crude oils of eastern Venezuela were correlated, by using
    differential separation data from laboratory PVT analysis.

    @see: J.E. Manucci, E.E. Rosales, 1968: "Correlaciones de presión
    de burbujeo y factor volumétrico del petróleo para crudos el
    oriente de Venezuela". Corporación Venezolana del Petróleo, II
    Jornadas técnicas de petróleo, marzo 1968

    @see: C.A. Heny, V.L. Pérez, M.E. Lago, 2001: Evaluación y
    generación de correlaciones PVT para crudos extrapesados
    venezolanos. Informe Técnico INT-8713,2001. Proyecto: Estudio y
    Modelaje de Procesos de Recuperación de Crudos Pesados. Los Teques,
    Octubre de 2001

*/
class RsManucciRosales : public SolutionGasOilRelation
{
  Correlation_Singleton(RsManucciRosales);

  RsManucciRosales()
    : SolutionGasOilRelation("RsManucciRosales", SCF_STB::get_instance())
  {
    add_parameter("yg", Sgg::get_instance());
    add_parameter("pb", psia::get_instance(), 1230, 5035);
    add_parameter("p", psia::get_instance());
    add_parameter("api", Api::get_instance(), 14, 40);
    add_parameter("t", Fahrenheit::get_instance(), 165, 304);
    add_parameter("rsb", SCF_STB::get_instance(), 180, 2500);
  }

public:
  inline Quantity<SCF_STB> impl(const Quantity<Sgg>& yg,
                                const Quantity<psia>& pb,
                                const Quantity<psia>& p,
                                const Quantity<Api>& api,
                                const Quantity<Fahrenheit>& t,
                                const Quantity<SCF_STB>& rsb) const;

  Quantity<SCF_STB> operator()(const Quantity<Sgg>& yg,
                               const Quantity<psia>& pb,
                               const Quantity<psia>& p,
                               const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t,
                               const Quantity<SCF_STB>& rsb) const
  {
    return compute({ yg, pb, p, api, t, rsb });
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars) const
  {
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
};

#endif // SOLUTION_GAS_OIL_RELATION_1_H
