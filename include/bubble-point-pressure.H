
# ifndef BUBBLE_POINT_PRESSURE_H
# define BUBBLE_POINT_PRESSURE_H

# include <correlation.H>

Declare_Correlation_Subtype(BubblePointPressure, OilCorrelation);

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
    add_parameter("t", Rankine::get_instance(), 533.67, 699.67);
  }

  Quantity<psia> compute(const Quantity<Sgg> & yg,     // gas specific gravity 
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

    return compute(yg, yo, rsb, t);
  }

  Quantity<psia> operator () 
  (const Quantity<Sgg>     & yg,           // gas specific gravity 
   const Quantity<Sg_do>   & yo,           // oil specific gravity 
   const Quantity<SCF_STB> & rsb,          // Solution GOR at Pb
   const Quantity<Rankine> & t) const
  {
    return compute({yg, yo, rsb, t});
  }
};


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

  Quantity<psia> compute(const Quantity<Sgg> & yg,     // gas specific gravity 
			 const Quantity<Sg_do> & yo,    // oil specific gravity 
			 const Quantity<SCF_STB> & rsb, // Solution GOR at Pb
			 const Quantity<Fahrenheit> & t) const 
  {
    constexpr double c1 = 5.527215;
    constexpr double c2 = -1.841408;
    constexpr double c3 = 0.783716;

    // NOTE python version uses floor(yo*yp). However, original doesn't use it
    double r = pow(yo, c1) * exp(c2 * floor(yo.get_value() * yg.get_value())) *
      pow(rsb.get_value() * (t.get_value() + 460) * yg.get_value(), c3);

    return r >= 0 ? Quantity<psia>(r) : 0;
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

    return compute(yg, yo, rsb, t);
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


/**  DE GHETTO CORRELATION FOR EXTRA HEAVY OILS, CALCULATION OF BUBBLE
     POINT PRESSURE
        
     Based on 1200 measured data points from 65 heavy and extra-heavy
     oil samples. Oil samples have been divided in two different API
     gravity classes: extra-heavy oils for °API<=10, heavy oils for
     10<°API<=22.3. 
        
     Giambattista De Ghetto, Francesco Paone and Marco
     Villa. "Pressure-Volume-Temperature. Correlations for Heavy
     and Extra Heavy Oils," SPE 30316, 1995. 

     @see M.B. Standing. "A Pressure-Volume-Temperature Correlation
     for Mixtures of California Oil and Gases", Drill. & Prod. Prac.,
     Beal C. The Viscosity-Temperature Chart 0361-39 Nat. Bur. Stds
     Circular C461-1947". 

     VALIDADA CON PYTHON!
 */
class PbDeGhettoEHOil : public BubblePointPressure
{
   Correlation_Singleton(PbDeGhettoEHOil);

  PbDeGhettoEHOil()
    : BubblePointPressure("PbDeGhettoEHOil", psia::get_instance(),
			  208.86, 4021.96)
  {
    add_parameter("yg", Sgg::get_instance(), 0.623, 1.517);
    add_parameter("rsb", SCF_STB::get_instance(), 17.21, 640.25);
    add_parameter("api", Api::get_instance(), 6, 22.3);
    add_parameter("t", Fahrenheit::get_instance(), 131.4, 250.7);
  }

  Quantity<psia> compute(const Quantity<Sgg> & yg,     // gas specific gravity 
			 const Quantity<SCF_STB> & rsb, // Solution GOR at Pb
			 const Quantity<Api> & api,    // oil specific gravity 
			 const Quantity<Fahrenheit> & t) const 
  {
    const double n = 10.7025*pow(rsb.get_value()/yg.get_value(), 1.0/1.1128);
    const double d = pow(10, 0.0169*api.get_value() - 0.00156*t.get_value()); 
    const double ret  = n/d;

    return Quantity<psia>(ret);
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

    return compute(yg, rsb, api, t);
  }
};


/** DE GHETTO CORRELATION FOR HEAVY OILS (MODIFIED STANDING
    CORRELATION), CALCULATION OF BUBBLE POINT PRESSURE 

    Based on 1200 measured data points from 65 heavy and extra-heavy
    oil samples. Oil samples have been divided in two different API
    gravity classes: extra-heavy oils for °API<=10, heavy oils for
    10<°API<=22.3. 
        
    @see: Giambattista De Ghetto, Francesco Paone and Marco
    Villa. "Pressure-Volume-Temperature. Correlations for Heavy and
    Extra Heavy Oils," SPE 3 0316, 1995.  

    @see: M.B. Standing. "A Pressure-Volume-Temperature Correlation
    for Mixtures of California Oil and Gases", Drill. & Prod. Prac.,
    Beal C. The Viscosity-Temperature Chart 0361-39 Nat. Bur. Stds
    Circular C461-1947". 

    VALIDADA CON PYTHON!
 */
class PbDeGhettoHOil : public BubblePointPressure
{
   Correlation_Singleton(PbDeGhettoHOil);

  PbDeGhettoHOil()
    : BubblePointPressure("PbDeGhettoHOil", psia::get_instance(),
			  208.86, 4021.96)
  {
    add_parameter("yg", Sgg::get_instance(), 0.623, 1.517);
    add_parameter("rsb", SCF_STB::get_instance(), 17.21, 640.25);
    add_parameter("api", Api::get_instance(), 6, 22.3);
    add_parameter("t", Fahrenheit::get_instance(), 131.4, 250.7);
  }

  Quantity<psia> compute(const Quantity<Sgg> & yg,     // gas specific gravity 
			 const Quantity<SCF_STB> & rsb, // Solution GOR at Pb
			 const Quantity<Api> & api,    // oil specific gravity 
			 const Quantity<Fahrenheit> & t) const 
  {
    double factor = pow(rsb.get_value()/yg.get_value(), 0.7885) *
      (pow(10, 0.0020*t.get_value()) / pow(10, 0.0142*api.get_value()));

    return Quantity<psia>(15.7286*factor);
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

    return compute(yg, rsb, api, t);
  }
};

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

  Quantity<psia> compute(const Quantity<Sgg> & yg,     // gas specific gravity 
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

    return compute(yg, rsb, api, t);
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
    add_parameter("t", Rankine::get_instance(), 649.67, 734.67);
  }

  Quantity<psia> compute(const Quantity<Sgg> & yg,     // gas specific gravity 
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

    return compute(yg, rsb, yo, temperature);
  }

  Quantity<psia> operator () (const Quantity<Sgg> & yg, // gas specific gravity 
			      const Quantity<SCF_STB> & rsb, // Solution GOR at Pb
			      const Quantity<Sg_do> & yo, // oil specific gravity 
			      const Quantity<Rankine> & t) const
  {
    return compute({yg, rsb, yo, t});
  }
};

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

  Quantity<psia> compute(const Quantity<SCF_STB> & rsb) const
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

    return compute(rsb);
  }

  Quantity<psia> operator () (const Quantity<SCF_STB> & rsb) const
  {
    return compute({rsb});
  }
};

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

  Quantity<psia> compute(const Quantity<Sgg> & yg,
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

    return compute(yg, rsb, api, t, tsep, psep);
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

  Quantity<psia> compute(const Quantity<Sgg> & yg,
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

    return compute(yg, rsb, api, t);
  }

  Quantity<psia> operator () (const Quantity<Sgg> & yg,
			      const Quantity<SCF_STB> & rsb,
			      const Quantity<Api> & api,
			      const Quantity<Fahrenheit> & t) const
  {
    return compute({yg, rsb, api, t});
  }
};

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

  Quantity<psia> compute(const Quantity<Sgg> & yg,
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

    return compute(yg, rsb, api, t);
  }

  Quantity<psia> operator () (const Quantity<Sgg> & yg,
			      const Quantity<SCF_STB> & rsb,
			      const Quantity<Api> & api,
			      const Quantity<Fahrenheit> & t) const
  {
    return compute({yg, rsb, api, t});
  }
};

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

  Quantity<psia> compute(const Quantity<Sgg> & yg,
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

    return compute(yg, rsb, api, t);
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
