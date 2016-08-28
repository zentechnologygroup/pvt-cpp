
# ifndef BUBBLE_POINT_PRESSURE_H
# define BUBBLE_POINT_PRESSURE_H

# include <correlation.H>


/** AL-MARHOUN CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE

    Based on 69 bottomhole fluid samples from 69 Middle East oil reservoirs.
        
    @see Al-Marhoun, M.A.: "PVT Correlation for Middle East Crude
    Oils". Journal Petroleum Technology, May 1988, pp. 650-666.  

    VALIDADA CON PYTHON!
 */
class PbAlMarhoun : public BubblePointPressure
{
  Correlation_Singleton(PbAlMarhoun);

  PbAlMarhoun()
    : BubblePointPressure("PbAlMarhoun", psia::get_instance(), 130, 3573)
  {
    add_parameter("yg", Sgg::get_instance(), 0.752, 1.367);
    add_parameter("yo", Sg_do::get_instance(), 0.803521, 0.937707);
    add_parameter("rsb", SCF_STB::get_instance(), 26, 1602);
    add_parameter("t", Fahrenheit::get_instance(), 74, 240);
  }

  Quantity<psia> impl(const Quantity<Sgg> & yg,     // gas specific gravity 
		      const Quantity<Sg_do> & yo,    // oil specific gravity 
		      const Quantity<SCF_STB> & rsb, // Solution GOR at Pb
		      const Quantity<Rankine> & t) const 
  {
    double r = 0.00538088 * pow(rsb, 0.715082) * pow(yg, -1.87784) *
      pow(yo, 3.1437) * pow(t, 1.32657);

    return Quantity<psia>(r);
  }

public:

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);
    
    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & yo = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); it.next();
    const auto & t = it.get_curr();

    return impl(yg, yo, rsb, t);
  }

  Quantity<psia> operator () 
  (const Quantity<Sgg>     & yg,           // gas specific gravity 
   const Quantity<Sg_do>   & yo,           // oil specific gravity 
   const Quantity<SCF_STB> & rsb,          // Solution GOR at Pb
   const Quantity<Fahrenheit> & t) const
  {
    return compute({yg, yo, rsb, t});
  }
};

/*****************************************************************/

/** AL-SHAMMASI CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE

    Based on a total of 1709 data sets (1661 data sets from 13
    different published literature papers and 48 data sets originated
    from Kuwait reservoirs from unpublished sources).
        
    @see Al-Shammasi A.A.: "Bubble Point Pressure and Oil Formation Volumen
    Factor Correlations". SPE paper 53185 Presented at the Middle East
    Oil Show and Conference, 20-23 february 1999. 

    VALIDADA CON PYTHON!
 */
class PbAlShammasi : public BubblePointPressure
{
   Correlation_Singleton(PbAlShammasi);

  PbAlShammasi()
    : BubblePointPressure("PbAlShammasi", psia::get_instance(), 31.7, 7127)
  {
    add_parameter("yg", Sgg::get_instance(), 0.51, 3.44);
    add_parameter("yo", Sg_do::get_instance(), 0.724898, 1.02909);
    add_parameter("rsb", SCF_STB::get_instance(), 6, 3298.6);
    add_parameter("t", Fahrenheit::get_instance(), 74, 341.6);
  }

  Quantity<psia> impl(const Quantity<Sgg> & yg,     // gas specific gravity 
		      const Quantity<Sg_do> & yo,    // oil specific gravity 
		      const Quantity<SCF_STB> & rsb, // Solution GOR at Pb
		      const Quantity<Fahrenheit> & t) const 
  {
    constexpr double c1 = 5.527215;
    constexpr double c2 = -1.841408;
    constexpr double c3 = 0.783716;

    double r = pow(yo, c1) * exp(c2 * floor(yo.get_value() * yg.get_value())) *
      pow(rsb.get_value() * (t.get_value() + 460) * yg.get_value(), c3);

    return Quantity<psia>(r);
  }

public:

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);
    
    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & yo = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); it.next();
    const auto & t = it.get_curr();

    return impl(yg, yo, rsb, t);
  }

  Quantity<psia> operator () 
  (const Quantity<Sgg> & yg,               // gas specific gravity 
   const Quantity<Sg_do> & yo,             // oil specific gravity 
   const Quantity<SCF_STB> & rsb,          // Solution GOR at Pb
   const Quantity<Fahrenheit> & t) const
  {
    return compute({yg, yo, rsb, t});
  }
};

/*****************************************************************/

/** DE GHETTO CORRELATION FOR EXTRA-HEAVY AND HEAVY OILS (MODIFIED
    STANDING CORRELATION), CALCULATION OF BUBBLE POINT PRESSURE    
        
    Based on 1200 measured data points of 63 heavy and extra-heavy oil
    samples obtained from the Mediterranean Basin, Africa and the
    Persian Gulf.  

    Oil samples have been divided in two different API gravity
    classes: extra-heavy oils for °API<=10, heavy oils for
    10<°API<=22.3. 
        
    @see: Giambattista De Ghetto, Francesco Paone and Marco
    Villa. "Pressure-Volume-Temperature. Correlations for Heavy and
    Extra Heavy Oils," SPE 30316, 1995. 

    @see: M.B. Standing. "A Pressure-Volume-Temperature Correlation
    for Mixtures of California Oil and Gases", Drill. & Prod. Prac.,
    Beal C. The Viscosity-Temperature Chart 0361-39 Nat. Bur. Stds
    Circular C461-1947". 

    VALIDADA CON PYTHON!
 */
class PbDeGhetto : public BubblePointPressure
{
   Correlation_Singleton(PbDeGhetto);

  PbDeGhetto()
    : BubblePointPressure("PbDeGhetto", psia::get_instance(), 208.86, 4021.96)
  {
    add_parameter("yg", Sgg::get_instance(), 0.623, 1.517);
    add_parameter("rsb", SCF_STB::get_instance(), 17.21, 640.25);
    add_parameter("api", Api::get_instance(), 6, 22.3);
    add_parameter("t", Fahrenheit::get_instance(), 131.4, 250.7);
  }

  Quantity<psia> impl(const Quantity<Sgg> & yg,     // gas specific gravity 
		      const Quantity<SCF_STB> & rsb, // Solution GOR at Pb
		      const Quantity<Api> & api,    // oil specific gravity 
		      const Quantity<Fahrenheit> & t) const 
  {
    const double raw_api = api.get_value();
    const double raw_rsb = rsb.get_value();
    const double raw_yg = yg.get_value();
    const double raw_t = t.get_value();
    double pb = 0;
    
    if (raw_api <= 10) // Extra-heavy oil
      pb = (pow(raw_rsb/raw_yg, 1/1.1128)*10.7025 /
	    pow(10, 0.0169*raw_api - 0.00156*raw_t));
    else
      pb = 15.7286 * pow(raw_rsb/raw_yg, 0.7885) *
	(pow(10, 0.0020*raw_t) / pow(10, 0.0142*raw_api));

    return Quantity<psia>(pb);
  }

public:

  Quantity<psia> operator () (const Quantity<Sgg> & yg, 
			      const Quantity<SCF_STB> & rsb,
			      const Quantity<Api> & api,    
			      const Quantity<Fahrenheit> & t) const 
  {
    return compute({yg, rsb, api, t});
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);
    
    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); it.next();
    const auto & api = it.get_curr(); it.next();
    const auto & t = it.get_curr();

    return impl(yg, rsb, api, t);
  }
};

/*****************************************************************/

/**  DINDORUK & CHRISTMAN CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE   

     Based on more than 100 PVT reports from the Gulf of Mexico.
        
     @see: Dindoruk B., Christman G.: "PVT Properties and Viscosity
     Correlation for Gulf of Mexico Oils". SPE Journal Reservoir
     Evaluation & Engineering Volumen 7, Number 6, December 2004. 

     VALIDADA CON PYTHON! pero se sale de rango
 */
class PbDindorukChristman : public BubblePointPressure
{
   Correlation_Singleton(PbDindorukChristman);

  PbDindorukChristman()
    : BubblePointPressure("PbDindorukChristman", psia::get_instance(),
			  926, 12230)
  {
    add_parameter("yg", Sgg::get_instance(), 0.623, 1.517);
    add_parameter("rsb", SCF_STB::get_instance(), 133, 3050);
    add_parameter("api", Api::get_instance(), 14.7, 40);
    add_parameter("t", Fahrenheit::get_instance(), 117, 276);
  }

  Quantity<psia> impl(const Quantity<Sgg> & yg,     // gas specific gravity 
		      const Quantity<SCF_STB> & rsb, // Solution GOR at Pb
		      const Quantity<Api> & api,    // oil specific gravity 
		      const Quantity<Fahrenheit> & t) const 
  {
    constexpr double a1  = 0.000000000142828; // 1.42828 * 10 ** -10;
    constexpr double a2  = 2.844591797;
    constexpr double a3  = -0.000674896; // -6.74896 * 10 ** -4;
    constexpr double a4  = 1.225226436;
    constexpr double a5  = 0.033383304;
    constexpr double a6  = -0.272945957;
    constexpr double a7  = -0.084226069;
    constexpr double a8  = 1.869979257;
    constexpr double a9  = 1.221486524;
    constexpr double a10 = 1.370508349;
    constexpr double a11 = 0.011688308;

    const double n = a1*pow(t, a2) + a3*pow(api, a4);
    
    const double d = a5 + 2*pow(rsb, a6) / pow(yg, a7);
    const double d2 = d*d;

    const double a = n/d2;

    double pb = a8 * (pow(rsb, a9) * pow(10, a) / pow(yg, a10) + a11);

    return Quantity<psia>(pb);
  }

public:

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);
    
    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); it.next();
    const auto & api = it.get_curr(); it.next();
    const auto & t = it.get_curr();

    return impl(yg, rsb, api, t);
  }

  Quantity<psia> operator () 
  (const Quantity<Sgg> & yg,               // gas specific gravity 
   const Quantity<SCF_STB> & rsb,          // Solution GOR at Pb
   const Quantity<Api> & api,              // oil specific gravity 
   const Quantity<Fahrenheit> & t) const
  {
    return compute({yg, rsb, api, t});
  }
};

/*****************************************************************/

/** DOKLA & OSMAN CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE 
        
    Based on 51 bottomhole samples from UAE reservoirs.
        
    @see: Dokla, M.E., and Osman, M.E.: "Correlation of PVT Properties
    for UAE Crudes". Trans AIME, 1992, 293, 41-46. 

    VALIDADA CON PYTHON! ligera diferencia quizá explicada por la conversión
 */
class PbDoklaOsman : public BubblePointPressure
{
   Correlation_Singleton(PbDoklaOsman);

  PbDoklaOsman()
    : BubblePointPressure("PbDoklaOsman", psia::get_instance(), 590, 4640)
  {
    add_parameter("yg", Sgg::get_instance(), 0.798, 1.290);
    add_parameter("rsb", SCF_STB::get_instance(), 181, 2266);
    add_parameter("yo", Sg_do::get_instance(), 0.8236, 0.886);
    add_parameter("t", Fahrenheit::get_instance(), 190, 275);
  }

  Quantity<psia> impl(const Quantity<Sgg> & yg,     // gas specific gravity 
		      const Quantity<SCF_STB> & rsb, // Solution GOR at Pb
		      const Quantity<Sg_do> & yo,    // oil specific gravity 
		      const Quantity<Rankine> & t) const 
  {
    const double pb = 8363.86 * pow(yg, -1.01049) * pow(yo, 0.107991) *
      pow(t, -0.952584) * pow(rsb, 0.724047);

    return Quantity<psia>(pb);
  }

public:

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);
    
    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); it.next();
    const auto & yo = it.get_curr(); it.next();
    const auto & temperature = it.get_curr();

    return impl(yg, rsb, yo, temperature);
  }

  Quantity<psia> operator () (const Quantity<Sgg> & yg, 
			      const Quantity<SCF_STB> & rsb, 
			      const Quantity<Sg_do> & yo,
			      const Quantity<Rankine> & t) const
  {
    return compute({yg, rsb, yo, t});
  }
};

/*****************************************************************/

/**  HANAFY CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE   
        
     Based on experimental PVT data of 324 fluid samples taken from
     176 wells located in 75 fields. This data represents 15
     productive zones of 123 reservoirs distributed along three
     different regions of Egypt, including the Gulf of Suez, Western
     Desert, and Sinai. 
     
     @see: H.H. Hanafy, S.M. Macary, Y.M. ElNady, A.A. Bayomi and
     M.H. El Batanony. "A New Approach for Predicting the Crude Oil
     Properties", SPE 37439, 1997. 

     VALIDADA CON PYTHON!
 */
class PbHanafy : public BubblePointPressure
{
 Correlation_Singleton(PbHanafy);

  PbHanafy()
    : BubblePointPressure("PbHanafy", psia::get_instance(), 590, 4640)
  {
    add_parameter("rsb", SCF_STB::get_instance(), 36, 5003);
  }

  Quantity<psia> impl(const Quantity<SCF_STB> & rsb) const
  {
        // Total flash gas-oil ratio
    const double rsft = 69 + 1.071 * rsb.get_value();
    
        // Initial differential gas-oil ratio
    const double rsi =  23.94 + 1.101 * rsft;
    
    const double pb = (3.205 * rsi) + 157.27;

    return Quantity<psia>(pb);
  }

public:

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);

    const VtlQuantity & rsb = pars.get_first();

    return impl(rsb);
  }

  Quantity<psia> operator () (const Quantity<SCF_STB> & rsb) const
  {
    return compute({rsb});
  }
};

/*****************************************************************/

/** KARTOATMODJO & SCHMIDT CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE   

    Based on a set of 5392 data points, which represent 740 different
    crude oil samples.   

    The data bank was collected from PVT reports and literature. The
    first major source was from South East Asia, mainly Indonesia. The
    second source was North America, including the offshore area. The
    rest came from the Middle East and Latin America. 

    @see: R.S. Kartoatmodjo and Z. Schmidt. "New Correlations For
    Crude Oil Physical Properties", SPE 23556, 1991. 

    VALIDADA CON PYTHON!
 */
class PbKartoatmodjoSchmidt : public BubblePointPressure
{
 Correlation_Singleton(PbKartoatmodjoSchmidt);

  PbKartoatmodjoSchmidt()
    : BubblePointPressure("PbKartoatmodjoSchmidt", psia::get_instance(),
			  14.7, 6054.7)
  {
    // TODO: the line below contains the original limits which don't
    //    pass the min val for Sgg unit
    //    add_parameter("yg", Sgg::get_instance(), 0.4824, 1.668);
    add_parameter("yg", Sgg::get_instance(), 0.5, 1.668); // TODO
    add_parameter("rsb", SCF_STB::get_instance(), 0, 2890);
    add_parameter("api", Api::get_instance(), 14.4, 59);
    add_parameter("t", Fahrenheit::get_instance(), 75, 320);
    add_parameter("tsep", Fahrenheit::get_instance(), 38, 294);
    add_parameter("psep", psia::get_instance(), 14.7, 1414.7);
  }

  Quantity<psia> impl(const Quantity<Sgg> & yg,
		      const Quantity<SCF_STB> & rsb,
		      const Quantity<Api> & api,
		      const Quantity<Fahrenheit> & t,
		      const Quantity<Fahrenheit> & tsep,
		      const Quantity<psia> & psep) const
  {
    const double raw_api = api.get_value();
    const double c = 1 + 0.1595 * pow(raw_api, 0.4078) * pow(tsep, -0.2466) *
      log10(psep.get_value() / 114.7);
        
    const double ygcorr = c * yg.get_value();

    double c1, c2, c3, c4;
    if (raw_api > 30)
      {
	c1 = 0.0315;
	c2 = 0.7587;
	c3 = 11.289;
	c4 = 0.9143;
      }
    else
      {
	c1 = 0.05958;
	c2 = 0.7972;
	c3 = 13.1405;
	c4 = 0.9986;
      }

    const double p = rsb.get_value() /
      (c1 * pow(ygcorr, c2) * pow(10, c3 * raw_api / (t.get_value() + 460)));

    return Quantity<psia>(pow(p, c4));
  }

public:

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);

    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); it.next();
    const auto & api = it.get_curr(); it.next();
    const auto & t = it.get_curr(); it.next();
    const auto & tsep = it.get_curr(); it.next();
    const auto & psep = it.get_curr(); 

    return impl(yg, rsb, api, t, tsep, psep);
  }

  Quantity<psia> operator () (const Quantity<Sgg> & yg,
			      const Quantity<SCF_STB> & rsb,
			      const Quantity<Api> & api,
			      const Quantity<Fahrenheit> & t,
			      const Quantity<Fahrenheit> & tsep,
			      const Quantity<psia> & psep) const
  {
    return compute({yg, rsb, api, t, tsep, psep});
  }
};

/*****************************************************************/

/** LASATER CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE  
        
    Based on 158 experimentally measured bubble point pressures of 137
    independent systems. 

    The data were obtained from Canada, Western and Mid-Continental
    United States, and South America. 
          
    @see: Lasater, J.A., "Bubble Point Pressure Correlation" Trans.,
    AIME (1958) 213,379-381. 

    @see: Beggs, H.D. "Oil System Correlations", Petroleum Engineering
    Handbook, H.C. SPE, Richardson, TX (1987). Cap 22. Pag 22-6. 

    VALIDADA CON PYTHON!
 */
class PbLasater : public BubblePointPressure
{
  Correlation_Singleton(PbLasater);

  PbLasater()
    : BubblePointPressure("PbLasater", psia::get_instance(), 48, 5780)
  {
    add_parameter("yg", Sgg::get_instance(), 0.574, 1.223);
    add_parameter("rsb", SCF_STB::get_instance(), 3, 2905);
    add_parameter("api", Api::get_instance(), 17.9, 51.1);
    add_parameter("t", Fahrenheit::get_instance(), 82, 272);
  }

  Quantity<psia> impl(const Quantity<Sgg> & yg,
		      const Quantity<SCF_STB> & rsb,
		      const Quantity<Api> & api,
		      const Quantity<Fahrenheit> & t) const
  {
        // effective molecular weight
    const double mo =
      api <= 40 ? 630 - (10*api.get_value()) : 73110 * pow(api, -1.562);

    const Quantity<Sg_do> yo = api;

    // mol fraction of gas
    double pbfactor = 0;
    double ygfactor = (rsb.get_value()/379.3) /
      ((rsb.get_value()/379.3) + (350 * yo.get_value()/mo));
    if (ygfactor <= 0.6)
      pbfactor = (0.679 * exp(2.786*ygfactor)) - 0.323;
    else
      pbfactor = (8.26 * pow(ygfactor, 3.56)) + 1.95;

    const double pb = (pbfactor * (t.get_value() + 459.6)) / yg.get_value();

    return Quantity<psia>(pb);
  }

public:

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);

    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); it.next();
    const auto & api = it.get_curr(); it.next();
    const auto & t = it.get_curr(); 

    return impl(yg, rsb, api, t);
  }

  Quantity<psia> operator () (const Quantity<Sgg> & yg,
			      const Quantity<SCF_STB> & rsb,
			      const Quantity<Api> & api,
			      const Quantity<Fahrenheit> & t) const
  {
    return compute({yg, rsb, api, t});
  }
};

/*****************************************************************/

/** PETROSKY & FARSHAD CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE   
        
    Based on 81 laboratory PVT analyses.
    
    Fluid samples were obtained from reservoirs located offshore Texas
    and Louisiana. Producing areas from Galveston Island, eastward,
    through Main Pass are represented. 
        
    @see:G.E. Petrosky Jr. and
    F.F. Farshad. "Pressure-Volume-Temperature Correlations for Gulf
    of Mexico Crude Oils," SPE 26644, 1993.

    VALIDADA CON PYTHON!
 */
class PbPetroskyFarshad : public BubblePointPressure
{
  Correlation_Singleton(PbPetroskyFarshad);

  PbPetroskyFarshad()
    : BubblePointPressure("PbPetroskyFarshad", psia::get_instance(), 1574, 6523)
  {
    add_parameter("yg", Sgg::get_instance(), 0.5781, 0.8519);
    add_parameter("rsb", SCF_STB::get_instance(), 217, 1406);
    add_parameter("api", Api::get_instance(), 16.3, 45);
    add_parameter("t", Fahrenheit::get_instance(), 114, 288);
  }

  Quantity<psia> impl(const Quantity<Sgg> & yg,
		      const Quantity<SCF_STB> & rsb,
		      const Quantity<Api> & api,
		      const Quantity<Fahrenheit> & t) const
  {
        // effective molecular weight
    const double x = 0.00004561*pow(t, 1.3911) - 0.0007916*pow(api, 1.541);

    const double pb = 112.727 *
      ((pow(rsb, 0.5774)/pow(yg, 0.8439)*pow(10, x)) - 12.34);

    return Quantity<psia>(pb);
  }

public:

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);

    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); it.next();
    const auto & api = it.get_curr(); it.next();
    const auto & t = it.get_curr(); 

    return impl(yg, rsb, api, t);
  }

  Quantity<psia> operator () (const Quantity<Sgg> & yg,
			      const Quantity<SCF_STB> & rsb,
			      const Quantity<Api> & api,
			      const Quantity<Fahrenheit> & t) const
  {
    return compute({yg, rsb, api, t});
  }
};

/*****************************************************************/

/** STANDING CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE 
    
    Based on 105 experimentally determined bubble point pressures from
    22 different Californian crude-oil-natural-gas mixtures. 
        
    @see: M.B. Standing. "A Pressure-Volume-Temperature Correlation
    for Mixtures of California Oil and Gases", Drill. & Prod. Prac.,
    Beal C. The Viscosity-Temperature Chart 0361-39 Nat. Bur. Stds
    Circular C461-1947". 

    @see: A.A. Al-Shammasi. "A Review of Bubblepoint Pressure and Oil
    Formation Volume Factor Correlations", SPE 71302, 2001. 

    VALIDADA CON PYTHON!
 */
class PbStanding : public BubblePointPressure
{
  Correlation_Singleton(PbStanding);

  PbStanding()
    : BubblePointPressure("PbStanding", psia::get_instance(), 130, 7000)
  {
    add_parameter("yg", Sgg::get_instance(), 0.59, 0.95);
    add_parameter("rsb", SCF_STB::get_instance(), 20, 1425);
    add_parameter("api", Api::get_instance(), 16.5, 63.8);
    add_parameter("t", Fahrenheit::get_instance(), 100, 258);
  }

  Quantity<psia> impl(const Quantity<Sgg> & yg,
		      const Quantity<SCF_STB> & rsb,
		      const Quantity<Api> & api,
		      const Quantity<Fahrenheit> & t) const
  {
    const double pb =
      pow(rsb.get_value()/yg.get_value(), 0.83) *
      (pow(10, 0.00091*t.get_value() -0.0125*api.get_value())) -
      1.4;

    return Quantity<psia>(18.2*pb);
  }

public:

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);

    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); it.next();
    const auto & api = it.get_curr(); it.next();
    const auto & t = it.get_curr(); 

    return impl(yg, rsb, api, t);
  }

  Quantity<psia> operator () (const Quantity<Sgg> & yg,
			      const Quantity<SCF_STB> & rsb,
			      const Quantity<Api> & api,
			      const Quantity<Fahrenheit> & t) const
  {
    return compute({yg, rsb, api, t});
  }
};

/*****************************************************************/

/** TOTAL CFP CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE  
        
    @see: TOTAL Compagnie Francaise Des Petroles CFP: " Proyectos de
    Inyección de Fluidos Correlaciones PVT para Crudos del Oriente de
    Venezuela" S.A. MENEVEN Sep. 1983. 

    @see: C. Bánzer. "Correlaciones Numéricas P.V.T.", Universidad del
    Zulia, 1996. 

    VALIDADA CON PYTHON!
 */
class PbTotalCFP : public BubblePointPressure
{
  Correlation_Singleton(PbTotalCFP);

  PbTotalCFP()
    : BubblePointPressure("PbTotalCFP", psia::get_instance())
  {
    add_parameter("yg", Sgg::get_instance());
    add_parameter("rsb", SCF_STB::get_instance());
    add_parameter("api", Api::get_instance(), Api::get_instance().min_val, 45);
    add_parameter("t", Fahrenheit::get_instance());
  }

  Quantity<psia> impl(const Quantity<Sgg> & yg,
		      const Quantity<SCF_STB> & rsb,
		      const Quantity<Api> & api,
		      const Quantity<Fahrenheit> & t) const
  {
    double c1 = 0, c2 = 0, c3 = 0, c4 = 0;

    auto raw_api = api.get_value();
    if (raw_api <= 10)
      {
	c1 = 12.847;
	c2 = 0.9636;
	c3 = 0.000993;
	c4 = 0.03417;
      }
    else if (raw_api <= 35)
      {
	c1 = 25.2755;
	c2 = 0.7617;
	c3 = 0.000835;
	c4 = 0.011292;
      }
    else
      {
	c1 = 216.4711;
	c2 = 0.6922;
	c3 = -0.000427;
	c4 = 0.02314;
      }

    const double pb = c1 * pow(rsb.get_value()/yg.get_value(), c2) *
      pow(10, (c3*t.get_value()) - (c4*raw_api));

    return Quantity<psia>(pb);
  }

public:

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);

    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); it.next();
    const auto & api = it.get_curr(); it.next();
    const auto & t = it.get_curr(); 

    return impl(yg, rsb, api, t);
  }

  Quantity<psia> operator () (const Quantity<Sgg> & yg,
			      const Quantity<SCF_STB> & rsb,
			      const Quantity<Api> & api,
			      const Quantity<Fahrenheit> & t) const
  {
    return compute({yg, rsb, api, t});
  }
};

/*****************************************************************/

/** VASQUEZ & BEGGS CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE   

    Based on more than 600 laboratory PVT analyses from fields all over the world.
    
    @see: M.E. Vasquez and H.D. Beggs. "Correlations for Fluid
    Physical Property Prediction," JPT 968-70, June 1980.  

    @see: H.B. Bradley. "Petroleum Engineering Handbook", Society of
    Petroleum Engineers, Richardson TX, U.S.A., 1987. 

    VALIDADA CON PYTHON!
*/
class PbVasquezBeggs : public BubblePointPressure
{
  Correlation_Singleton(PbVasquezBeggs);

  PbVasquezBeggs()
    : BubblePointPressure("PbVasquezBeggs", psia::get_instance(), 50, 5250)
  {
    add_parameter("yg", Sgg::get_instance(), 0.56, 1.18);
    add_parameter("rsb", SCF_STB::get_instance(), 20, 2070);
    add_parameter("api", Api::get_instance(), 16, 58);
    add_parameter("t", Fahrenheit::get_instance(), 70, 295);
    add_parameter("tsep", Fahrenheit::get_instance());
    add_parameter("pset", psia::get_instance());
  }

  Quantity<psia> impl(const Quantity<Sgg> & yg,
		      const Quantity<SCF_STB> & rsb,
		      const Quantity<Api> & api,
		      const Quantity<Fahrenheit> & t,
		      const Quantity<Fahrenheit> & tsep,
		      const Quantity<psia> & psep) const
  {
    double c1 = 0, c2 = 0, c3 = 0;

    auto raw_api = api.get_value();
    if (raw_api <= 30)
      {
	c1 = 0.0362;
	c2 = 1.0937;
	c3 = 25.724;
      }
    else 
      {
	c1 = 0.0178;
	c2 = 1.187;
	c3 = 23.931;
      }

    const double ygs = yg.get_value() * (1 + 5.912e-05 * raw_api *
					 tsep.get_value() *
					 log10(psep.get_value()/114.7));

    const double pb_aux = rsb.get_value() /
      (c1*ygs*exp((c3*raw_api) / (t.get_value() + 460)));

    const double pb = pow(pb_aux, 1/c2);

    return Quantity<psia>(pb);
  }

public:

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);

    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); it.next();
    const auto & api = it.get_curr(); it.next();
    const auto & t = it.get_curr(); it.next();
    const auto & tsep = it.get_curr(); it.next();
    const auto & psep = it.get_curr();

    return impl(yg, rsb, api, t, tsep, psep);
  }

  Quantity<psia> operator () (const Quantity<Sgg> & yg,
			      const Quantity<SCF_STB> & rsb,
			      const Quantity<Api> & api,
			      const Quantity<Fahrenheit> & t,
			      const Quantity<Fahrenheit> & tsep,
			      const Quantity<psia> & psep) const
  {
    return compute({yg, rsb, api, t, tsep, psep});
  }
};

/*****************************************************************/

/** VELARDE CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE  

    Based on 728 laboratory measured bubble point pressures.
        
    @see:J. Velarde, T.A. Blasingame and W.D. McCain, Jr. "Correlation
    of Black Oil Properties at Pressures Below Bubble Point Pressure -
    A New Approach," The Petroleum Society 97-93, 1997.  

    VALIDADA CON PYTHON!
 */
class PbVelarde : public BubblePointPressure
{
  Correlation_Singleton(PbVelarde);

  PbVelarde()
    : BubblePointPressure("PbVelarde", psia::get_instance(), 70, 6700)
  {
    add_parameter("yg", Sgg::get_instance(), 0.556, 1.367);
    add_parameter("rsb", SCF_STB::get_instance(), 10, 1870);
    add_parameter("api", Api::get_instance(), 12, 55);
    add_parameter("t", Fahrenheit::get_instance(), 74, 327);
  }

  Quantity<psia> impl(const Quantity<Sgg> & yg,
		      const Quantity<SCF_STB> & rsb,
		      const Quantity<Api> & api,
		      const Quantity<Fahrenheit> & t) const
  {
    const double x = 0.013098 * pow(t, 0.282372) - 8.2e-06 * pow(api, 2.176124);

    const double pb = 1091.47 *
      pow((pow(rsb, 0.081465) * pow(yg, -0.161488) * pow(10, x) - 0.740152),
	  5.354891);

    return Quantity<psia>(pb);
  }

public:

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);

    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); it.next();
    const auto & api = it.get_curr(); it.next();
    const auto & t = it.get_curr();

    return impl(yg, rsb, api, t);
  }

  Quantity<psia> operator () (const Quantity<Sgg> & yg,
			      const Quantity<SCF_STB> & rsb,
			      const Quantity<Api> & api,
			      const Quantity<Fahrenheit> & t) const
  {
    return compute({yg, rsb, api, t});
  }
};

/*****************************************************************/

/** STANDING CORRELATION MODIFIED BY CEGARRA, CALCULATION OF BUBBLE
    POINT PRESSURE

    Based on 375 consistent PVT analyses. The data bank was compiled
    from 8394 fields of eastern Venezuela subdivided into 5192 fields
    of light oil and 3202 fields of medium oil. The correlation takes
    the functional form of Standing. 
        
    @see: Cegarra, L., 2007: "Correlación para el cálculo de la
    presión de burbuja de los yacimientos de crudo liviano y mediano
    del Oriente de Venezuela". Trabajo especial de grado. Universidad
    del Zulia  

    @see: M.B. Standing. "A Pressure-Volume-Temperature Correlation
    for Mixtures of California Oil and Gases , " Drill. &
    Prod. Prac.,Beal C. The Viscosity-Temperature Chart 0361-39
    Nat. Bur . Stds Circular C461-1947". 

    VALIDADA CON PYTHON!
 */
class PbCegarra : public BubblePointPressure
{
  Correlation_Singleton(PbCegarra);

  PbCegarra()
    : BubblePointPressure("PbCegarra", psia::get_instance(), 735, 7303)
  {
    add_parameter("yg", Sgg::get_instance(), 0.654, 1.337);
    add_parameter("rsb", SCF_STB::get_instance(), 135, 4290);
    add_parameter("api", Api::get_instance(), 22, 41.9);
    add_parameter("t", Fahrenheit::get_instance(), 117, 354);
  }

  Quantity<psia> impl(const Quantity<Sgg> & yg,
		      const Quantity<SCF_STB> & rsb,
		      const Quantity<Api> & api,
		      const Quantity<Fahrenheit> & t) const
  {
    const double raw_api = api.get_value();

    double l1 = 0, l2 = 0, l3 = 0, l4 = 0, l5 = 0;
    if (raw_api < 29.9)
      {
	l1 = 154.158;
	l2 = 0.4577;
	l3 = 0.0006680;
	l4 = 0.000514;
	l5 = 4.70257;
      }
    else if (raw_api >= 29.9)
      {
	l1 = 809.238;
	l2 = 0.32;
	l3 = 0.00061;
	l4 = 0.011;
	l5 = 1.1142;
      }

    const double pb = l1 * (pow((rsb.get_value()/yg.get_value()), l2) *
			    pow(10, l3*t.get_value() - l4*raw_api) - l5);

    return Quantity<psia>(pb);
  }

public:

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);

    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); it.next();
    const auto & api = it.get_curr(); it.next();
    const auto & t = it.get_curr();

    return impl(yg, rsb, api, t);
  }

  Quantity<psia> operator () (const Quantity<Sgg> & yg,
			      const Quantity<SCF_STB> & rsb,
			      const Quantity<Api> & api,
			      const Quantity<Fahrenheit> & t) const
  {
    return compute({yg, rsb, api, t});
  }
};

/*****************************************************************/

/** MILLÁN-ARCIA CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE  

    Venezuelan heavy crudes were correlated.
        
    @see: E.A. Millán-Arcia, 1984: Correlaciones para estimar el
    comportamiento PVT de crudos pesados venezolanos. I Jornadas
    Técnicas Corpoven 

    @see: C.A. Heny, V.L. Pérez, M.E. Lago, 2001: Evaluación y
    generación de correlaciones PVT para crudos extrapesados
    venezolanos. Informe Técnico INT-8713,2001. Proyecto: Estudio y
    Modelaje de Procesos de Recuperación de Crudos Pesados. Los
    Teques, Octubre de 2001 

    VALIDADA CON PYTHON!
 */ 
class PbPerezML : public BubblePointPressure
{
  Correlation_Singleton(PbPerezML);

  PbPerezML()
    : BubblePointPressure("PbPerezML", psia::get_instance(), 405, 1335)
  {
    add_parameter("yg", Sgg::get_instance(), 0.571, 0.981);
    add_parameter("rsb", SCF_STB::get_instance(), 38, 121);
    add_parameter("api", Api::get_instance(), 6.4, 12.9);
    add_parameter("t", Fahrenheit::get_instance(), 112, 300);
  }

  Quantity<psia> impl(const Quantity<Sgg> & yg,
		      const Quantity<SCF_STB> & rsb,
		      const Quantity<Api> & api,
		      const Quantity<Fahrenheit> & t) const
  {
    const double x = 0.0002573*t.get_value() - 0.0253643*api.get_value();
        
    const double pb = 10.667657 *
      pow(rsb.get_value()/yg.get_value(), 1.00139101) * pow(10, x);

    return Quantity<psia>(pb);
  }

public:

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);

    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); it.next();
    const auto & api = it.get_curr(); it.next();
    const auto & t = it.get_curr();

    return impl(yg, rsb, api, t);
  }

  Quantity<psia> operator () (const Quantity<Sgg> & yg,
			      const Quantity<SCF_STB> & rsb,
			      const Quantity<Api> & api,
			      const Quantity<Fahrenheit> & t) const
  {
    return compute({yg, rsb, api, t});
  }
};


/*****************************************************************/

/** MILLÁN-ARCIA CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE  

    Venezuelan heavy crudes were correlated.
        
    @see: E.A. Millán-Arcia, 1984: Correlaciones para estimar el
    comportamiento PVT de crudos pesados venezolanos. I Jornadas
    Técnicas Corpoven

    @see: C.A. Heny, V.L. Pérez, M.E. Lago, 2001: Evaluación y
    generación de correlaciones PVT para crudos extrapesados
    venezolanos. Informe Técnico INT-8713,2001. Proyecto: Estudio y
    Modelaje de Procesos de Recuperación de Crudos Pesados. Los
    Teques, Octubre de 2001 

    VALIDADA CON PYTHON!
 */
class PbMillanArcia : public BubblePointPressure
{
  Correlation_Singleton(PbMillanArcia);

  PbMillanArcia()
    : BubblePointPressure("PbMillanArcia", psia::get_instance(), 222, 3432.7)
  {
    add_parameter("yg", Sgg::get_instance());
    add_parameter("rsb", SCF_STB::get_instance(), 53, 459);
    add_parameter("api", Api::get_instance(), 9, 20.2);
    add_parameter("t", Fahrenheit::get_instance(), 87, 195);
  }

  Quantity<psia> impl(const Quantity<Sgg> & yg,
		      const Quantity<SCF_STB> & rsb,
		      const Quantity<Api> & api,
		      const Quantity<Fahrenheit> & t) const
  {
    const double x = 0.00091*t.get_value() - 0.0125*api.get_value();
        
    const double pb = 25.3302 * pow(pow(rsb.get_value()/yg.get_value(), 0.8303) *
				    pow(10, x), 0.9433);

    return Quantity<psia>(pb);
  }

public:

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);

    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); it.next();
    const auto & api = it.get_curr(); it.next();
    const auto & t = it.get_curr();

    return impl(yg, rsb, api, t);
  }

  Quantity<psia> operator () (const Quantity<Sgg> & yg,
			      const Quantity<SCF_STB> & rsb,
			      const Quantity<Api> & api,
			      const Quantity<Fahrenheit> & t) const
  {
    return compute({yg, rsb, api, t});
  }
};

/*****************************************************************/

/** MANUCCI-ROSALES CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE 

    Crude oils of eastern Venezuela were correlated, by using
    differential separation data from laboratory PVT analysis.  
        
    @see: J.E. Manucci, E.E. Rosales, 1968: "Correlaciones de presión
    de burbujeo y factor volumétrico del petróleo para crudos el
    oriente de Venezuela". Corporación Venezolana del Petróleo, II
    Jornadas técnicas de petróleo, marzo 1968 

    @see: C.A. Heny, V.L. Pérez, M.E. Lago, 2001: Evaluación y
    generación de correlaciones PVT para crudos extrapesados
    venezolanos. Informe Técnico INT-8713,2001. Proyecto: Estudio y
    Modelaje de Procesos de Recuperación de Crudos Pesados. Los
    Teques, Octubre de 2001 

    VALIDADA CON PYTHON!
 */
class PbManucciRosales : public BubblePointPressure
{
  Correlation_Singleton(PbManucciRosales);

  PbManucciRosales()
    : BubblePointPressure("PbManucciRosales", psia::get_instance(), 1230, 5035)
  {
    add_parameter("yg", Sgg::get_instance());
    add_parameter("rsb", SCF_STB::get_instance(), 180, 2500);
    add_parameter("api", Api::get_instance(), 14, 40);
    add_parameter("t", Fahrenheit::get_instance(), 165, 304);
  }

  Quantity<psia> impl(const Quantity<Sgg> & yg,
		      const Quantity<SCF_STB> & rsb,
		      const Quantity<Api> & api,
		      const Quantity<Fahrenheit> & t) const
  {
    const double x = 0.000922*t.get_value() - 0.0072*api.get_value();
        
    const double pb = 84.88 * pow(rsb.get_value()/yg.get_value(), 0.53) *
      pow(10, x);

    return Quantity<psia>(pb);
  }

public:

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);

    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); it.next();
    const auto & api = it.get_curr(); it.next();
    const auto & t = it.get_curr();

    return impl(yg, rsb, api, t);
  }

  Quantity<psia> operator () (const Quantity<Sgg> & yg,
			      const Quantity<SCF_STB> & rsb,
			      const Quantity<Api> & api,
			      const Quantity<Fahrenheit> & t) const
  {
    return compute({yg, rsb, api, t});
  }
};


# endif // BUBBLE_POINT_PRESSURE_H
