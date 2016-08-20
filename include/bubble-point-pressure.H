
# ifndef BUBBLE_POINT_PRESSURE_H
# define BUBBLE_POINT_PRESSURE_H

# include <correlation.H>

Declare_Correlation_Subtype(BubblePointPressure, OilCorrelation);

/** AL-MARHOUN CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE

    Based on 69 bottomhole fluid samples from 69 Middle East oil reservoirs.
        
    @see Al-Marhoun, M.A.: "PVT Correlation for Middle East Crude
    Oils". Journal Petroleum Technology, May 1988, pp. 650-666.  

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
    add_parameter("temperature", Rankine::get_instance(), 533.67, 699.67);
  }

  Quantity<psia> compute(const Quantity<Sgg> & yg,     // gas specific gravity 
			 const Quantity<Sg_do> & yo,    // oil specific gravity 
			 const Quantity<SCF_STB> & rsb, // Solution GOR at Pb
			 const Quantity<Rankine> & temperature) const 
  {
    double r = 0.00538088 * pow(rsb, 0.715082) * pow(yg, -1.87784) *
      pow(yo, 3.1437) * pow(temperature, 1.32657);

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
    const auto & temperature = it.get_curr();

    return compute(yg, yo, rsb, temperature);
  }

  Quantity<psia> operator () 
  (const Quantity<Sgg>     & yg,           // gas specific gravity 
   const Quantity<Sg_do>   & yo,           // oil specific gravity 
   const Quantity<SCF_STB> & rsb,          // Solution GOR at Pb
   const Quantity<Rankine> & temperature) const
  {
    return compute({yg, yo, rsb, temperature});
  }
};


/** AL-SHAMMASI CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE

    Based on a total of 1709 data sets (1661 data sets from 13
    different published literature papers and 48 data sets originated
    from Kuwait reservoirs from unpublished sources).
        
    @see Al-Shammasi A.A.: "Bubble Point Pressure and Oil Formation Volumen
    Factor Correlations". SPE paper 53185 Presented at the Middle East
    Oil Show and Conference, 20-23 february 1999. 
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
    add_parameter("temperature", Fahrenheit::get_instance(), 74, 341.6);
  }

  Quantity<psia> compute(const Quantity<Sgg> & yg,     // gas specific gravity 
			 const Quantity<Sg_do> & yo,    // oil specific gravity 
			 const Quantity<SCF_STB> & rsb, // Solution GOR at Pb
			 const Quantity<Rankine> & temperature) const 
  {
    constexpr double c1 = 5.527215;
    constexpr double c2 = -1.841408;
    constexpr double c3 = 0.783716;

    double r = pow(yo, c1) * exp(c2 * floor(yo.get_value()*yg.get_value())) *
      pow((rsb.get_value()*(temperature.get_value() + 460)* yg.get_value()), c3);

    return r >= 0 ? Quantity<psia>(r) : 0;
  }

public:

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);
    
    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & yp = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); it.next();
    const auto & temperature = it.get_curr();

    return compute(yg, yp, rsb, temperature);
  }

  Quantity<psia> operator () 
  (const Quantity<Sgg> & yg,               // gas specific gravity 
   const Quantity<Sg_do> & yo,             // oil specific gravity 
   const Quantity<SCF_STB> & rsb,          // Solution GOR at Pb
   const Quantity<Fahrenheit> & temperature) const
  {
    return compute({yg, yo, rsb, temperature});
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
    add_parameter("temperature", Fahrenheit::get_instance(), 131.4, 250.7);
  }

  Quantity<psia> compute(const Quantity<Sgg> & yg,     // gas specific gravity 
			 const Quantity<SCF_STB> & rsb, // Solution GOR at Pb
			 const Quantity<Api> & api,    // oil specific gravity 
			 const Quantity<Fahrenheit> & temperature) const 
  {
    const double n = 10.7025*pow(rsb.get_value()/yg.get_value(), 1.0/1.1128);
    const double d =
      pow(10, 0.0169*api.get_value() - 0.00156*temperature.get_value()); 
    const double ret  = n/d;

    return Quantity<psia>(ret);
  }

public:

  Quantity<psia> operator () (const Quantity<Sgg> & yg, 
			      const Quantity<SCF_STB> & rsb,
			      const Quantity<Api> & api,    
			      const Quantity<Fahrenheit> & temperature) const 
  {
    return compute({yg, rsb, api, temperature});
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);
    
    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); it.next();
    const auto & api = it.get_curr(); it.next();
    const auto & temperature = it.get_curr();

    return compute(yg, rsb, api, temperature);
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
    add_parameter("temperature", Fahrenheit::get_instance(), 131.4, 250.7);
  }

  Quantity<psia> compute(const Quantity<Sgg> & yg,     // gas specific gravity 
			 const Quantity<SCF_STB> & rsb, // Solution GOR at Pb
			 const Quantity<Api> & api,    // oil specific gravity 
			 const Quantity<Fahrenheit> & temperature) const 
  {
    double factor = pow(rsb.get_value()/yg.get_value(), 0.7885) *
      (pow(10, 0.0020*temperature.get_value()) / pow(10, 0.0142*api.get_value()));

    return Quantity<psia>(15.7286*factor);
  }

public:

  Quantity<psia> operator () (const Quantity<Sgg> & yg, 
			      const Quantity<SCF_STB> & rsb,
			      const Quantity<Api> & api,    
			      const Quantity<Fahrenheit> & temperature) const 
  {
    return compute({yg, rsb, api, temperature});
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);
    
    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); it.next();
    const auto & api = it.get_curr(); it.next();
    const auto & temperature = it.get_curr();

    return compute(yg, rsb, api, temperature);
  }
};

/**  DINDORUK & CHRISTMAN CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE   

     Based on more than 100 PVT reports from the Gulf of Mexico.
        
     @see: Dindoruk B., Christman G.: "PVT Properties and Viscosity
     Correlation for Gulf of Mexico Oils". SPE Journal Reservoir
     Evaluation & Engineering Volumen 7, Number 6, December 2004. 
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
    add_parameter("temperature", Fahrenheit::get_instance(), 117, 276);
  }

  Quantity<psia> compute(const Quantity<Sgg> & yg,     // gas specific gravity 
			 const Quantity<SCF_STB> & rsb, // Solution GOR at Pb
			 const Quantity<Api> & api,    // oil specific gravity 
			 const Quantity<Fahrenheit> & temperature) const 
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

    double div = a5 + pow(2*rsb, a6) / pow(yg, a7);
    double a = (pow(a1*temperature, a2) + pow(a3*api, a4)) / div*div;

    double pb = a8 + (pow(rsb, a9) * pow(10, a) / pow(yg, a10)) + a11;

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
    const auto & temperature = it.get_curr();

    return compute(yg, rsb, api, temperature);
  }

  Quantity<psia> operator () 
  (const Quantity<Sgg> & yg,               // gas specific gravity 
   const Quantity<SCF_STB> & rsb,          // Solution GOR at Pb
   const Quantity<Api> & api,             // oil specific gravity 
   const Quantity<Fahrenheit> & temperature) const
  {
    return compute({yg, rsb, api, temperature});
  }
};


/** DOKLA & OSMAN CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE 
        
    Based on 51 bottomhole samples from UAE reservoirs.
        
    @see: Dokla, M.E., and Osman, M.E.: "Correlation of PVT Properties
    for UAE Crudes". Trans AIME, 1992, 293, 41-46. 
 */
class PbDoklaOsman : public BubblePointPressure
{
   Correlation_Singleton(PbDoklaOsman);

  PbDoklaOsman()
    : BubblePointPressure("PbDoklaOsman", psia::get_instance(), 590, // 4640
			  6000
			  )
  {
    add_parameter("yg", Sgg::get_instance(), 0.798, 1.290);
    add_parameter("rsb", SCF_STB::get_instance(), 181, 2266);
    add_parameter("yo", Sg_do::get_instance(), 0.8236, 0.886);
    add_parameter("temperature", Rankine::get_instance(), 649.67, 734.67);
  }

  Quantity<psia> compute(const Quantity<Sgg> & yg,     // gas specific gravity 
			 const Quantity<SCF_STB> & rsb, // Solution GOR at Pb
			 const Quantity<Sg_do> & yo,    // oil specific gravity 
			 const Quantity<Rankine> & temperature) const 
  {
    const double pb = 8363.86 * pow(yg, -1.01049) * pow(yo, 0.107991) *
      pow(temperature, -0.952584) * pow(rsb, 0.724047);

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

  Quantity<psia> operator () 
  (const Quantity<Sgg> & yg,               // gas specific gravity 
   const Quantity<SCF_STB> & rsb,          // Solution GOR at Pb
   const Quantity<Sg_do> & yo,             // oil specific gravity 
   const Quantity<Rankine> & temperature) const
  {
    return compute({yg, rsb, yo, temperature});
  }
};

// TODO: encontrar la manera de que el singleton tenga una
// referencia. Tal vez con punero 


# endif // BUBBLE_POINT_PRESSURE_H
