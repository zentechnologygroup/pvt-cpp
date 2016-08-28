# ifndef SOLUTION_GAS_OIL_RELATION_H
# define SOLUTION_GAS_OIL_RELATION_H

# include <correlation.H>

Declare_Correlation_Subtype(SolutionGasOilRelation, OilCorrelation);

/**  AL-SHAMMASI CORRELATION, CALCULATION OF SOLUTION GOR

     Based on a total of 1709 data sets (1661 data sets from 13
     different published literature papers and 48 data sets originated
     from Kuwait reservoirs from unpublished sources). 

     @see: Al-Shammasi A.A.: "Bubble Point pressure and Oil Formation
     Volumen Factor Correlations" SPE paper 53185 Presented at the
     Middle East Oil Show and Conference, 20-23 february 1999

     Validada con python!
 */
class RsAlShammasi : public SolutionGasOilRelation
{
  Correlation_Singleton(RsAlShammasi);

  RsAlShammasi()
    : SolutionGasOilRelation("RsAlShammasi", SCF_STB::get_instance())
  {
    add_parameter("yg", Sgg::get_instance(), 0.51, 3.44);
    add_parameter("pb", psia::get_instance(), 31.7, 7127);
    add_parameter("p", psia::get_instance(), Quantity<Atmosphere>(1));

    // min and max of api are put inverted because Sg_do unit is inverse
    add_parameter("yo", Sg_do::get_instance(),
		  Quantity<Api>(63.7), Quantity<Api>(6));
    
    add_parameter("t", Fahrenheit::get_instance(), 74, 341.6);
    add_parameter("rsb", SCF_STB::get_instance(), 6, 3298.6);
  }

  Quantity<SCF_STB> impl(const Quantity<Sgg> & yg,
			 const Quantity<psia> & pb,
			 const Quantity<psia> & p,
			 const Quantity<Sg_do> & yo,
			 const Quantity<Fahrenheit> & t,
			 const Quantity<SCF_STB> & rsb) const
  {
    if (p >= pb)
      return rsb;
    
    constexpr double c1 = 5.527215;
    constexpr double c2 = -1.841408;
    constexpr double c3_inv = 1/0.783716;

    const double raw_yo = yo.get_value();
    const double raw_yg = yg.get_value();
    const double raw_p  = p.get_value();

    const double rs =
      pow(raw_p/(pow(raw_yo, c1)*exp(c2*floor(raw_yo*raw_yg))), c3_inv) /
      ((t.get_value() + 460) * raw_yg); 

    return Quantity<SCF_STB>(rs);
  }

public:

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);
    
    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & pb = it.get_curr(); it.next();
    const auto & p = it.get_curr(); it.next();
    const auto & yo = it.get_curr(); it.next();
    const auto & t = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); 

    return impl(yg, pb, p, yo, t, rsb);
  }

  Quantity<SCF_STB> operator () (const Quantity<Sgg> & yg,
				 const Quantity<psia> & pb,
				 const Quantity<psia> & p,
				 const Quantity<Sg_do> & yo,
				 const Quantity<Fahrenheit> & t,
				 const Quantity<SCF_STB> & rsb) const
  {
    return compute({yg, pb, p, yo, t, rsb});
  }
};

/*****************************************************************/

/** AL-MARHOUN CORRELATION, CALCULATION OF SOLUTION GOR
          
    Based on 69 bottomhole fluid samples from 69 Middle East oil reservoirs.
  
    @see: Al-Marhoun, M.A.: "PVT Correlation for Middle East Crude
    Oils" Journal Petroleum Technology, May 1988, pp. 650-666. 

    Validada con python!
 */
class RsAlMarhoun : public SolutionGasOilRelation
{
  Correlation_Singleton(RsAlMarhoun);

  RsAlMarhoun()
    : SolutionGasOilRelation("RsAlMarhoun", SCF_STB::get_instance())
  {
    add_parameter("yg", Sgg::get_instance(), 0.752, 1.367);
    add_parameter("pb", psia::get_instance(), 130, 3573);
    add_parameter("p", psia::get_instance(), 20, 3573);

    // min and max of api are put inverted because Sg_do unit is inverse
    add_parameter("yo", Sg_do::get_instance(),
		  Quantity<Api>(44.6), Quantity<Api>(19.4));
    
    add_parameter("t", Rankine::get_instance(),
		  Quantity<Fahrenheit>(74), Quantity<Fahrenheit>(240));
    add_parameter("rsb", SCF_STB::get_instance(), 26, 1602);
  }

  Quantity<SCF_STB> impl(const Quantity<Sgg> & yg,
			 const Quantity<psia> & pb,
			 const Quantity<psia> & p,
			 const Quantity<Sg_do> & yo,
			 const Quantity<Fahrenheit> & t,
			 const Quantity<SCF_STB> & rsb) const
  {
    if (p >= pb)
      return rsb;

    constexpr double c1 = 0.00538088;
    constexpr double c2 = -1.87784;
    constexpr double c3 = 3.1437;
    constexpr double c4 = 1.32657;
    constexpr double c5 = 1 / 0.715082;
        
    const double rs =
      pow(p.get_value() / c1*pow(yg, c2)*pow(yo, c3)*pow(t, c4), c5);

    return Quantity<SCF_STB>(rs);
  }

public:

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);
    
    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & pb = it.get_curr(); it.next();
    const auto & p = it.get_curr(); it.next();
    const auto & yo = it.get_curr(); it.next();
    const auto & t = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); 

    return impl(yg, pb, p, yo, t, rsb);
  }

  Quantity<SCF_STB> operator () (const Quantity<Sgg> & yg,
				 const Quantity<psia> & pb,
				 const Quantity<psia> & p,
				 const Quantity<Sg_do> & yo,
				 const Quantity<Fahrenheit> & t,
				 const Quantity<SCF_STB> & rsb) const
  {
    return compute({yg, pb, p, yo, t, rsb});
  }
};

/*****************************************************************/

/** DE GHETTO CORRELATION FOR EXTRA-HEAVY (MODIFIED STANDING
    CORRELATION) AND HEAVY (MODIFIED VASQUEZ-BEGGS CORRELATION) OILS,
    CALCULATION OF SOLUTION GOR
          
    Based on 1200 measured data points of 63 heavy and extra-heavy oil
    samples obtained from the Mediterranean Basin, Africa and the
    Persian Gulf.  

    Oil samples have been divided in two different API gravity
    classes: extra-heavy oils for °API<=10, heavy oils for
    10<°API<=22.3. 
        
    @see: Giambattista De Ghetto, Francesco Paone and Marco
    Villa. "Pressure-Volume-Temperature Correlations for Heavy and
    Extra Heavy Oils," SPE 30316, 1995. 

    @see: M.B. Standing. "A Pressure-Volume-Temperature Correlation
    for Mixtures of California Oil and Gases", Drill. & Prod. Prac.,
    Beal C. The Viscosity-Temperature Chart 0361-39 Nat. Bur. Stds
    Circular C461-1947". 
        
    @note: De Ghetto uses Yg as the average specific gravity of total
    surface gases, while the original correlation (Standing) uses
    it as the gravity of dissolved gas.  
    Since Standing doesn't specify if Yg is the gravity of total
    surface gases or of the separator, it is assumed that the data is
    from a one stage separation with average gas gravity. 

    Validada con python!
 */
class RsDeGhetto : public SolutionGasOilRelation
{
  Correlation_Singleton(RsDeGhetto);

  RsDeGhetto()
    : SolutionGasOilRelation("RsDeGhetto", SCF_STB::get_instance())
  {
    add_parameter("yg", Sgg::get_instance(), 0.623, 1.517);
    add_parameter("pb", psia::get_instance(), 208.6, 4021.96);
    add_parameter("p", psia::get_instance(), 1038.49, 7411.54);
    add_parameter("api", Api::get_instance(), 6, 22.3);    
    add_parameter("t", Fahrenheit::get_instance(), 131.4, 250.7);
    add_parameter("tsep", Fahrenheit::get_instance(), 59, 177.8);
    add_parameter("psep", psia::get_instance(), 14.5, 752.2);
    add_parameter("rsb", SCF_STB::get_instance(), 26, 1602);
  }

  Quantity<SCF_STB> impl(const Quantity<Sgg> & yg,
			 const Quantity<psia> & pb,
			 const Quantity<psia> & p,
			 const Quantity<Api> & api,
			 const Quantity<Fahrenheit> & t,
			 const Quantity<Fahrenheit> & tsep,
			 const Quantity<psia> & psep,
			 const Quantity<SCF_STB> & rsb) const
  {
    if (p >= pb)
      return rsb;

    const double raw_api = api.get_value();
    const double raw_yg = yg.get_value();
    const double raw_t = t.get_value();
    const double raw_p = p.get_value();

    double rs = 0;
    if (raw_api <= 10)
      rs = raw_yg * pow(raw_p/10.7025*pow(10, 0.0169*raw_api-0.00156*raw_t),
			1.1128);
    else
      {
	const double raw_tsep = tsep.get_value();
	const double raw_psep = psep.get_value();
	const double ygcorr = raw_yg *
	  (1 + 0.5912 * raw_api * raw_tsep * log10(raw_psep/114.7) * 0.0001);
	rs = ((ygcorr * pow(raw_p, 1.2057))/56.434) *
	  pow(10, 10.9267*raw_api/(raw_t + 460));
      }

    return Quantity<SCF_STB>(rs);
  }

public:

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);
    
    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & pb = it.get_curr(); it.next();
    const auto & p = it.get_curr(); it.next();
    const auto & yo = it.get_curr(); it.next();
    const auto & t = it.get_curr(); it.next();
    const auto & tsep = it.get_curr(); it.next();
    const auto & psep = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); 

    return impl(yg, pb, p, yo, t, tsep, psep, rsb);
  }

  Quantity<SCF_STB> operator () (const Quantity<Sgg> & yg,
				 const Quantity<psia> & pb,
				 const Quantity<psia> & p,
				 const Quantity<Api> & api,
				 const Quantity<Fahrenheit> & t,
				 const Quantity<Fahrenheit> & tsep,
				 const Quantity<psia> & psep,
				 const Quantity<SCF_STB> & rsb) const
  {
    return compute({yg, pb, p, api, t, tsep, psep, rsb});
  }
};

/*****************************************************************/

/** DINDORUK & CHRISTMAN CORRELATION, CALCULATION OF SOLUTION GOR

    Based on more than 100 PVT reports from the Gulf of Mexico.
        
    @see: Dindoruk B., Christman G.: "PVT Properties and Viscosity
    Correlation for Gulf of Mexico Oils". SPE Journal Reservoir
    Evaluation & Engineering Volumen 7, Number 6, December 2004 

    Validada con python!
 */
class RsDindorukChristman : public SolutionGasOilRelation
{
  Correlation_Singleton(RsDindorukChristman);

  RsDindorukChristman()
    : SolutionGasOilRelation("RsDindorukChristman", SCF_STB::get_instance())
  {
    add_parameter("yg", Sgg::get_instance(), 0.6017, 1.027);
    add_parameter("pb", psia::get_instance(), 926, 12230);
    add_parameter("p", psia::get_instance());
    add_parameter("api", Api::get_instance(), 14.7, 40);    
    add_parameter("t", Fahrenheit::get_instance(), 117, 276);
    add_parameter("rsb", SCF_STB::get_instance(), 133, 3050);
  }

  Quantity<SCF_STB> impl(const Quantity<Sgg> & yg,
			 const Quantity<psia> & pb,
			 const Quantity<psia> & p,
			 const Quantity<Api> & api,
			 const Quantity<Fahrenheit> & t,
			 const Quantity<SCF_STB> & rsb) const
  {
    if (p >= pb)
      return rsb;

    constexpr double a1  = 4.86996e-06;
    constexpr double a2  = 5.730982539;
    constexpr double a3  = 0.0099251;
    constexpr double a4  = 1.776179364;
    constexpr double a5  = 44.2500268;
    constexpr double a6  = 2.702889206;
    constexpr double a7  = 0.744335673;
    constexpr double a8  = 3.35975497;
    constexpr double a9  = 28.10133245;
    constexpr double a10 = 1.57905016;
    constexpr double a11 = 0.928131344;

    const double raw_p = p.get_value();
    const double raw_api = api.get_value();
    const double d = a5 + 2*pow(raw_api, a6)/pow(pb, a7);
    const double d2 = d*d;

    const double a = (a1*pow(raw_api, a2) + a3*pow(t, a4)) / d2;

    const double rs = pow((raw_p/a8 + a9) * pow(yg, a10) * pow(10, a), a11);

    return Quantity<SCF_STB>(rs);
  }

public:

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);
    
    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & pb = it.get_curr(); it.next();
    const auto & p = it.get_curr(); it.next();
    const auto & api = it.get_curr(); it.next();
    const auto & t = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); 

    return impl(yg, pb, p, api, t, rsb);
  }

  Quantity<SCF_STB> operator () (const Quantity<Sgg> & yg,
				 const Quantity<psia> & pb,
				 const Quantity<psia> & p,
				 const Quantity<Api> & api,
				 const Quantity<Fahrenheit> & t,
				 const Quantity<SCF_STB> & rsb) const
  {
    return compute({yg, pb, p, api, t, rsb});
  }
};

/*****************************************************************/

/** DOKLA & OSMAN CORRELATION, CALCULATION OF SOLUTION GOR
     
    Based on 51 bottomhole samples from UAE reservoirs.
        
    @see: Dokla, M.E., and Osman, M.E.: "Correlation of PVT Properties
    for UAE Crudes" Trans AIME, 1992, 293, 41-46. 

    Validada con python!
 */
class RsDoklaOsman : public SolutionGasOilRelation
{
  Correlation_Singleton(RsDoklaOsman);

 RsDoklaOsman()
    : SolutionGasOilRelation("RsDoklaOsman", SCF_STB::get_instance())
  {
    add_parameter("yg", Sgg::get_instance(), 0.798, 1.29);
    add_parameter("pb", psia::get_instance(), 590, 4640);
    add_parameter("p", psia::get_instance());
    add_parameter("yo", Sg_do::get_instance(), 0.8236, 0.886);    
    add_parameter("t", Rankine::get_instance(),
		  Quantity<Fahrenheit>(190), Quantity<Fahrenheit>(275));
    add_parameter("rsb", SCF_STB::get_instance(), 181, 2266);
  }

  Quantity<SCF_STB> impl(const Quantity<Sgg> & yg,
			 const Quantity<psia> & pb,
			 const Quantity<psia> & p,
			 const Quantity<Sg_do> & yo,
			 const Quantity<Rankine> & t,
			 const Quantity<SCF_STB> & rsb) const
  {
    if (p >= pb)
      return rsb;

    constexpr double k = 11956.20204068456430404144; // (1/0.836386 * 10 **4)
    constexpr double d = 1/0.724047;

    const double raw_p = p.get_value();

    const double rs = pow(k * raw_p * pow(yg, 1.01049) * pow(yo, -0.107991) *
			  pow(t, 0.952584), d);

    return Quantity<SCF_STB>(rs);
  }

public:

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);
    
    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & pb = it.get_curr(); it.next();
    const auto & p = it.get_curr(); it.next();
    const auto & yo = it.get_curr(); it.next();
    const auto & t = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); 

    return impl(yg, pb, p, yo, t, rsb);
  }

  Quantity<SCF_STB> operator () (const Quantity<Sgg> & yg,
				 const Quantity<psia> & pb,
				 const Quantity<psia> & p,
				 const Quantity<Sg_do> & yo,
				 const Quantity<Rankine> & t,
				 const Quantity<SCF_STB> & rsb) const
  {
    return compute({yg, pb, p, yo, t, rsb});
  }
};


/*****************************************************************/

/** GLASO CORRELATION, CALCULATION OF SOLUTION GOR
    
    Based on 26 samples from the North Sea (collected from wells in
    the region 56 to 62°N) and 19 samples from the Middle East,
    Algeria, and several areas in the U.S. cg  
    
    @see: Oistein Glaso. "Generalized Pressure-Volume-Temperature
    Correlations," Journal of Petroleum Technology, 1980. 

    @see: C. Bánzer. "Correlaciones Numéricas P.V.T.", Universidad del
    Zulia, 1996. 

    Validada con python!
 */
class RsGlaso : public SolutionGasOilRelation
{
  Correlation_Singleton(RsGlaso);

  RsGlaso()
    : SolutionGasOilRelation("RsGlaso", SCF_STB::get_instance())
  {
    add_parameter("yg", Sgg::get_instance(), 0.65, 1.276);
    add_parameter("pb", psia::get_instance(), 150, 7127);
    add_parameter("p", psia::get_instance());
    add_parameter("api", Api::get_instance(), 22.3, 48.1);    
    add_parameter("t", Fahrenheit::get_instance(), 80, 280);
    add_parameter("rsb", SCF_STB::get_instance(), 90, 2637);
  }

  Quantity<SCF_STB> impl(const Quantity<Sgg> & yg,
			 const Quantity<psia> & pb,
			 const Quantity<psia> & p,
			 const Quantity<Api> & api,
			 const Quantity<Fahrenheit> & t,
			 const Quantity<SCF_STB> & rsb) const
  {
    if (p >= pb)
      return rsb;

    constexpr double a = -0.30218;
    constexpr double b = 1.7447;
    constexpr double b2 = b*b; // b**2
    const double c = 1.7669 - log10(p);

    // TODO: considerar resolver la eq 2do grado y decidir por el
    // valor de p. La solución es log10(p)=(17669*a-2500*b^2)/(10000*a)
    double r = 0;
    if (b2 - 4*a*c < 0)
      r =  -b/(2*a);
    else
      r = (-b + sqrt(b2 - 4*a*c)) / (2*a);

    const double f = pow(10, r);
    const double rs = yg.get_value() *
      pow((f * pow(api, 0.989) ) / ( pow(t, 0.172) ), 1/0.816);

    return Quantity<SCF_STB>(rs);
  }

public:

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);
    
    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & pb = it.get_curr(); it.next();
    const auto & p = it.get_curr(); it.next();
    const auto & api = it.get_curr(); it.next();
    const auto & t = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); 

    return impl(yg, pb, p, api, t, rsb);
  }

  Quantity<SCF_STB> operator () (const Quantity<Sgg> & yg,
				 const Quantity<psia> & pb,
				 const Quantity<psia> & p,
				 const Quantity<Api> & api,
				 const Quantity<Rankine> & t,
				 const Quantity<SCF_STB> & rsb) const
  {
    return compute({yg, pb, p, api, t, rsb});
  }
};

/*****************************************************************/

/** HANAFY CORRELATION, CALCULATION OF SOLUTION GOR

    Based on experimental PVT data of 324 fluid samples taken from 176
    wells located in 75 fields. This data represents 15 productive
    zones of 123 reservoirs distributed along three different regions
    of Egypt, including the Gulf of Suez, Western Desert, and Sinai. 
                    
    @see: H.H. Hanafy, S.M. Macary, Y.M. ElNady, A.A. Bayomi and
    M.H. El Batanony. "A New Approach for Predicting the Crude Oil
    Properties," SPE 37439, 1997. 
*/
class RsHanafy : public SolutionGasOilRelation
{
  Correlation_Singleton(RsHanafy);

  RsHanafy()
    : SolutionGasOilRelation("RsHanafy", SCF_STB::get_instance())
  {
    add_parameter("pb", psia::get_instance(), 36, 5003);
    add_parameter("p", psia::get_instance());
    add_parameter("rsb", SCF_STB::get_instance(), 97, 4272);
  }

  Quantity<SCF_STB> impl(const Quantity<psia> & pb,
			 const Quantity<psia> & p,
			 const Quantity<SCF_STB> & rsb) const
  {
    if (p >= pb)
      return rsb;

    const double rs = -49.069 + 0.312*p.get_value();

    return Quantity<SCF_STB>(rs);
  }

public:

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);
    
    auto it = pars.get_it();
    const auto & pb = it.get_curr(); it.next();
    const auto & p = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); 

    return impl(pb, p, rsb);
  }

  Quantity<SCF_STB> operator () (const Quantity<psia> & pb,
				 const Quantity<psia> & p,
				 const Quantity<SCF_STB> & rsb) const
  {
    return compute({pb, p, rsb});
  }
};


/*****************************************************************/

/** KARTOATMODJO & SCHMIDT CORRELATION, CALCULATION OF SOLUTION GOR

    Based on a set of 5392 data points, which represent 740 different
    crude oil samples. 

    The data bank was collected from PVT reports and literature. The
    first major source was from South East Asia, mainly Indonesia. The
    second source was North America, including the offshore area. The
    rest came from the Middle East and Latin America. 

    @see: R.S. Kartoatmodjo and Z. Schmidt. "New Correlations For
    Crude Oil Physical Properties", SPE 23556, 1991. 

    Validada con python!
 */
class RsKartoatmodjoSchmidt : public SolutionGasOilRelation
{
  Correlation_Singleton(RsKartoatmodjoSchmidt);

  RsKartoatmodjoSchmidt()
    : SolutionGasOilRelation("RsKartoatmodjoSchmidt", SCF_STB::get_instance())
  {
    add_parameter("yg", Sgg::get_instance(), 0.28, 1.668);
    add_parameter("pb", psia::get_instance(), 14.7, 6054.7);
    add_parameter("p", psia::get_instance());
    add_parameter("api", Api::get_instance(), 14.4, 59);    
    add_parameter("t", Fahrenheit::get_instance(), 75, 320);
    add_parameter("tsep", Fahrenheit::get_instance(), 38, 294);
    add_parameter("psep", psia::get_instance(), 14.7, 1414.7);
    add_parameter("rsb", SCF_STB::get_instance(), 0, 2890);
  }

  Quantity<SCF_STB> impl(const Quantity<Sgg> & yg,
			 const Quantity<psia> & pb,
			 const Quantity<psia> & p,
			 const Quantity<Api> & api,
			 const Quantity<Fahrenheit> & t,
			 const Quantity<Fahrenheit> & tsep,
			 const Quantity<psia> & psep,
			 const Quantity<SCF_STB> & rsb) const
  {
    if (p >= pb)
      return rsb;

    const double raw_t = t.get_value();

    const double c =
      1 + 0.1595 * pow(api, 0.4078) * pow(tsep, -0.2466) * log10(psep/114.7);
    const double ygcorr = c*yg.get_value();

    double rs = 0;
    const double raw_api = api.get_value();
    if (raw_api > 30)
      {
	constexpr double c1 = 0.0315;
	constexpr double c2 = 0.7587;
	constexpr double c3 = 1.0937;
	constexpr double c4 = 11.289;
	rs = c1 * pow(ygcorr, c2) * pow(p, c3) *
	  pow(10, c4*raw_api/(raw_t + 460));
      }
    else
      {
	constexpr double c1 = 0.05958;
	constexpr double c2 = 0.7972;
	constexpr double c3 = 1.0014;
	constexpr double c4 = 13.1405;
	rs = c1 * pow(ygcorr, c2) * pow(p, c3) *
	  pow(10, c4*raw_api/(raw_t + 460));
      }

    return Quantity<SCF_STB>(rs);
  }

public:

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);
    
    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & pb = it.get_curr(); it.next();
    const auto & p = it.get_curr(); it.next();
    const auto & api = it.get_curr(); it.next();
    const auto & t = it.get_curr(); it.next();
    const auto & tsep = it.get_curr(); it.next();
    const auto & psep = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); 

    return impl(yg, pb, p, api, t, tsep, psep, rsb);
  }

  Quantity<SCF_STB> operator () (const Quantity<Sgg> & yg,
				 const Quantity<psia> & pb,
				 const Quantity<psia> & p,
				 const Quantity<Api> & api,
				 const Quantity<Fahrenheit> & t,
				 const Quantity<Fahrenheit> & tsep,
				 const Quantity<psia> & psep,
				 const Quantity<SCF_STB> & rsb) const
  {
    return compute({yg, pb, p, api, t, tsep, psep, rsb});
  }
};


/*****************************************************************/

/** LASATER CORRELATION, CALCULATION OF SOLUTION GOR
         
    Based on 158 experimentally measured bubble point pressures of 137
    independent systems.

    The data were obtained from Canada, Western and Mid-Continental
    United States, and South America. 
          
    @see: Lasater, J.A., "Bubble Point Pressure Correlation" Trans.,
        AIME (1958) 213,379-381. 
    @:see: Beggs, H.D. "Oil System Correlations", Petroleum
        Engineering Handbook, H.C. SPE, Richardson, TX (1987). Cap
        22. Pag 22-6.  

     Validada con python!
 */
class RsLasater : public SolutionGasOilRelation
{
  Correlation_Singleton(RsLasater);

  RsLasater()
    : SolutionGasOilRelation("RsLasater", SCF_STB::get_instance())
  {
    add_parameter("yg", Sgg::get_instance(), 0.574, 1.223);
    add_parameter("pb", psia::get_instance(), 48, 5780);
    add_parameter("p", psia::get_instance());
    add_parameter("yo", Sg_do::get_instance(),
		  Quantity<Api>(51.1), Quantity<Api>(17.9));
    add_parameter("t", Rankine::get_instance(),
		  Quantity<Fahrenheit>(82), Quantity<Fahrenheit>(272));
    add_parameter("rsb", SCF_STB::get_instance(), 3, 2905);
  }

  Quantity<SCF_STB> impl(const Quantity<Sgg> & yg,
			 const Quantity<psia> & pb,
			 const Quantity<psia> & p,
			 const Quantity<Sg_do> & yo,
			 const Quantity<Rankine> & t,
			 const Quantity<SCF_STB> & rsb) const
  {
    if (p >= pb)
      return rsb;

    const double raw_api = Quantity<Api>(yo).get_value();
    const double mo =
      raw_api <= 40 ? 630 - 10*raw_api : pow(73110*raw_api, -1.562);

    const double raw_p = p.get_value();
    const double raw_yg = yg.get_value();
    const double raw_t = t.get_value();

        // Calculation of Xpb = 
    double sy = 0;
    const double xpb = raw_p*raw_yg/raw_t; // Bubble point pressure factor
    if (xpb < 3.29)
      {
	constexpr double c1 = 0.359;
	const double     c2 = (1.473*raw_p*raw_yg) / raw_t;
	constexpr double c3 = 0.476;
	sy = c1 * log(c2 + c3);
      }
    else
      {        
	const double     c1 = (0.121*raw_p*raw_yg) / raw_t;
	constexpr double c2 = -0.236;
	constexpr double c3 = 0.281;
	sy = pow(c1 + c2, c3);
      }        

    if (sy >= 1)
      sy = 0.99999999;
        
    const double rs = (132755*yo.get_value()*sy) / (mo*(1 - sy));
    
    return Quantity<SCF_STB>(rs >= 0 ? rs : 0);
  }

public:

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);
    
    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & pb = it.get_curr(); it.next();
    const auto & p = it.get_curr(); it.next();
    const auto & yo = it.get_curr(); it.next();
    const auto & t = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); 

    return impl(yg, pb, p, yo, t, rsb);
  }

  Quantity<SCF_STB> operator () (const Quantity<Sgg> & yg,
				 const Quantity<psia> & pb,
				 const Quantity<psia> & p,
				 const Quantity<Sg_do> & yo,
				 const Quantity<Rankine> & t,
				 const Quantity<SCF_STB> & rsb) const
  {
    return compute({yg, pb, p, yo, t, rsb});
  }
};


/*****************************************************************/

/**  PETROSKY & FARSHAD CORRELATION, CALCULATION OF SOLUTION GOR

     Based on 81 laboratory PVT analyses.
  
   Fluid samples were obtained from reservoirs located offshore Texas
   and Louisiana. Producing areas from Galveston Island, eastward,
   through Main Pass are represented.
        
   @see:G.E. Petrosky Jr. and
   F.F. Farshad. "Pressure-Volume-Temperature Correlations for Gulf of
   Mexico Crude Oils," SPE 26644, 1993. 
          
   Validada con python!
 */
class RsPetroskyFarshad : public SolutionGasOilRelation
{
  Correlation_Singleton(RsPetroskyFarshad);

  RsPetroskyFarshad()
    : SolutionGasOilRelation("RsPetroskyFarshad", SCF_STB::get_instance())
  {
    add_parameter("yg", Sgg::get_instance(), 0.5781, 0.8519);
    add_parameter("pb", psia::get_instance(), 1574, 6523);
    add_parameter("p", psia::get_instance());
    add_parameter("api", Api::get_instance(), 16.3, 45);
    add_parameter("t", Fahrenheit::get_instance(), 114, 288);
    add_parameter("rsb", SCF_STB::get_instance(), 217, 1406);
  }

  Quantity<SCF_STB> impl(const Quantity<Sgg> & yg,
			 const Quantity<psia> & pb,
			 const Quantity<psia> & p,
			 const Quantity<Api> & api,
			 const Quantity<Fahrenheit> & t,
			 const Quantity<SCF_STB> & rsb) const
  {
    if (p >= pb)
      return rsb;

    const double raw_p = p.get_value();
    const double x = 0.0007916 * pow(api, 1.541) - 0.00004561 * pow(t, 1.3911);
        
    const double rs = pow((raw_p/112.727 + 12.34) * pow(yg, 0.8439) * pow(10, x),
			  1.73184);

    return Quantity<SCF_STB>(rs);
  }

public:

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);
    
    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & pb = it.get_curr(); it.next();
    const auto & p = it.get_curr(); it.next();
    const auto & api = it.get_curr(); it.next();
    const auto & t = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); 

    return impl(yg, pb, p, api, t, rsb);
  }

  Quantity<SCF_STB> operator () (const Quantity<Sgg> & yg,
				 const Quantity<psia> & pb,
				 const Quantity<psia> & p,
				 const Quantity<Api> & api,
				 const Quantity<Fahrenheit> & t,
				 const Quantity<SCF_STB> & rsb) const
  {
    return compute({yg, pb, p, api, t, rsb});
  }
};

/*****************************************************************/

// Begin_Correlation(RsTest, SolutionGasOilRelation, Fahrenheit);
// Declare_Correlation_Parameter(p1, Fahrenheit, 10, 300);
// Declare_Correlation_Parameter(p2, psia, 100, 3000);
// End_Correlation_Parameters();
// End_Correlation();

class RsStanding : public SolutionGasOilRelation
{
  Correlation_Singleton(RsStanding);

  RsStanding()
    : SolutionGasOilRelation("RsStanding", SCF_STB::get_instance(), 0, 10)
  {
    add_parameter("yg", Sgg::get_instance());
    add_parameter("pb", psia::get_instance());
    add_parameter("p", psia::get_instance());
    add_parameter("api", Api::get_instance());
    add_parameter("t", Fahrenheit::get_instance());
    add_parameter("rsb", SCF_STB::get_instance());
  }

public:
  inline Quantity<SCF_STB> impl(const Quantity<Sgg> & yg,
				const Quantity<psia> & pb,
				const Quantity<psia> & p,
				const Quantity<Api> & api,
				const Quantity<Fahrenheit> & t,
				const Quantity<SCF_STB> & rsb) const
    ;
  Quantity<SCF_STB> operator () (const Quantity<Sgg> & yg,
				 const Quantity<psia> & pb,
				 const Quantity<psia> & p,
				 const Quantity<Api> & api,
				 const Quantity<Fahrenheit> & t,
				 const Quantity<SCF_STB> & rsb) const
  {
    return compute({yg,pb,p,api,t,rsb});
  }
  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);

    auto it = pars.get_it();
    const auto & yg = it.get_curr();it.next();
    const auto & pb = it.get_curr();it.next();
    const auto & p = it.get_curr();it.next();
    const auto & api = it.get_curr();it.next();
    const auto & t = it.get_curr();it.next();
    const auto & rsb = it.get_curr();

    return impl(yg,pb,p,api,t,rsb);
  }
};

inline Quantity<SCF_STB> RsStanding::impl(const Quantity<Sgg> & yg,
					  const Quantity<psia> & pb,
					  const Quantity<psia> & p,
					  const Quantity<Api> & api,
					  const Quantity<Fahrenheit> & t,
					  const Quantity<SCF_STB> & rsb) const
{
  // put here the implementation
}


# endif // SOLUTION_GAS_OIL_REKATION_H
