# ifndef WATER_DENSITY_IMPL_H
# define WATER_DENSITY_IMPL_H

# include <correlations/water-density.H>
# include <correlations/spivey-dry.H>

void PwSpiveyMN::precondition(const Quantity<Celsius> & t,
			      const Quantity<mPascal> & p) const
{
  const double pv = PvSpiveyMN::get_instance().impl(Quantity<Kelvin>(t).raw());
  if (p < pv)
    {
      ostringstream s;
      s << "PwSpiveyMN::impl: received pressure " << p
	<< " is less than computed vapor pressure " << pv;
      ALEPHTHROW(WrongValueVaporPressure, s.str());
    }
}

inline double
PwSpiveyMN::impl(const double & t, // comes in Celsius
		 const double & p,
		 const double & nacl) noexcept
{
  const double m = nacl;

  const double tk = unit_convert<Celsius, Kelvin>(t);

  // Vapor pressure of pure water, calculated from the IAWPS-95 formulation
  const double pv = PvSpiveyMN::get_instance().impl(tk);

  // mCH4w: SOLUBILITY OF METHANE IN BRINE [gmol NaCl/kgH2O] 
  // AT THE TEMPERATURE AND PRESSURE OF EVALUATION
  const double mch4w = MwSpiveyMN::get_instance().impl(t, p, pv, m);

  // VMCH4w: PARTIAL MOLAR VOLUME OF METHANE IN BRINE AT THE
  // TEMPERATURE AND PRESSURE OF EVALUATION
  
  // Derivatives with respect to P
  const double c3 = -8.5658e-2 + 1.31961e-2*log(tk) + 7.338/tk +
    9.05e-2/(680 - tk);
  
  const double c4 = 2.260e-1/tk + 2 * -1.227e-3 * p/tk;

  constexpr double R = 8.314467; // Universal gas constant [MPa
				 // cm³/gmol K] (McCain et al., 2011) 
            
  // Partial molar volume of methane in brine
  const double vmch4w = R*tk*(c3 + 2*m*c4);
            
  // pw: DENSITY OF BRINE WITH DISSOLVED METHANE
  // Density of methane-free brine at the temperature and pressure of
  // evaluation 
  const double pgfw =
    PwSpiveyMNGasFree::get_instance().impl(t, p, nacl);

  // Specific volume of the gas-free brine [cm³/g]
  const double vgfw = 1.0/pgfw;
            
  // Molecular weights MNaCl: 58.4428 g/gmol; MCH4: 16.043 g/gmol 
  // Density of brine with dissolved methane [g/cm³] (pw = mass/volume)
  const double mass = 1000 + m*58.4428 + mch4w*16.043;
  const double volume = (1000 + m*58.4428) * vgfw + mch4w * vmch4w;
  const double pw = mass/volume;
            
  return pw;
}

inline double
PwSpiveyMNGasFree::impl(const double & t, // comes in Celsius
			const double & p,
			const double & nacl) noexcept
{
  const double t_100 = t/100;
  const double t_100_2 = t_100*t_100;
  const double m = nacl;

  // Pure water density [g/cm³] at the reference pressure (70 Mpa) and
  // evaluation temperature 
  const double ppwr = (-0.127213*t_100_2 + 0.645486*t_100 + 1.03265) /
    (-0.070291*t_100_2 + 0.639589*t_100 + 1);
        
  // Coefficients of compressibility of pure water  
  const double epw = (4.221*t_100_2 + -3.478*t_100 + 6.221) / 
    (0.5182*t_100_2 + -0.4405*t_100 + 1);

  const double fpw = (-11.403*t_100_2 + 29.932*t_100 + 27.952) /
    (0.20684*t_100_2 + 0.3768*t_100 + 1);
        
  // Coefficients of gas-free brine density at the temperature of evaluation 
  // and the reference pressure of 70 MPa
  const double d1 = (-7.925e-5*t_100_2 + -1.93e-6*t_100 + -3.4254e-4); 
  const double d2 = (1.0998e-3*t_100_2 + -2.8755e-3*t_100 + -3.5819e-3) /
    (-0.72877*t_100_2 + 1.92016*t_100 + 1);
  const double d3 = (-7.6402e-3*t_100_2 + 3.6963e-2*t_100 + 4.36083e-2) /
    (-0.333661*t_100_2 + 1.185685*t_100 + 1);
  const double d4 = (3.746e-4*t_100_2 + -3.328e-4*t_100 + -3.346e-4);

  // Density of gas-free brine
  const double pgfwr = ppwr + d1*m*m + d2*pow(m, 3.0/2.0) + d3*m + d4 * sqrt(m);
        
  // Coefficients of gas-free brine compressibility
  constexpr double e =  0.1353;

  const double f1 = (-1.409*t_100_2 + -0.361*t_100 + -0.2532)/(9.216*t_100 + 1);
  const double f2 = (5.614*t_100 + 4.6782) / (-0.307*t_100_2 + 2.6069*t_100 + 1);
  const double f3 = -0.1127*t_100_2 + 0.2047*t_100 + -0.0452;
        
  const double ew = epw + e*m;
  const double fw = fpw + f1*pow(m, 1.5) + f2*m + f3*sqrt(m);
        
  const double iwr = (1/ew)*log(fabs(ew + fw));
  const double iw = (1/ew)*log(fabs((ew*(p/70)) + fw));
        
  // Density of gas-free brine [g/cm³] at the temperature and pressure
  // of evaluation
  const double pgfw = pgfwr*exp(iw - iwr);
                        
  return pgfw;
}

inline double
PwMcCain::impl(const double & nacl,
	       const double & bw) noexcept
{ 
  const double s = nacl;
  const double s2 = s*s;
  // Water density at standard conditions
  const double pwl = 62.368 + 0.438603*s + 1.60074e-3*s2;
  // Water density at P and T
  const double pw = pwl/bw;

  return pw;
}

inline double
PpwSpiveyMN::impl(const double & t,
		  const double & p) noexcept
{
  const double t100 = t/100;
  const double t100_2 = t100*t100;

  // Pure water density at the reference pressure (70 Mpa) [g/cm³]
  const double ppwr = (-0.127213*t100_2 + 0.645486*t100 + 1.03265) /
    (-0.070291*t100_2 + 0.639589*t100 + 1);
        
  // The coefficients of pure water density, expressed as functions of temperature
  const double epw = (4.221*t100_2 + -3.478*t100 + 6.221) /
    (0.5182*t100_2 + -0.4405*t100 + 1);
     
  const double fpw = (-11.403*t100_2 + 29.932*t100 + 27.952) /
    (0.20684*t100_2 + 0.3768*t100 + 1);
        
  const double ipwr = (1/epw)*log(fabs(epw + fpw));
  const double ipw = (1/epw)*log(fabs((epw * (p/70))+ fpw));
        
  // Pure water density [g/cm³]
  const double ppw = ppwr * exp(ipw - ipwr);
        
  return ppw;
}

# endif // WATER_DENSITY_IMPL_H
