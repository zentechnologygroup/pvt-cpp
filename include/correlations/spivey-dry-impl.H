#ifndef SPIVEY_DRY_IMPL_H
#define SPIVEY_DRY_IMPL_H

#include <correlations/correlation.H>
# include "spivey-dry.H"
# include "water-density.H"

inline double
PvSpiveyMN::impl(const double & t) noexcept // t comes in Kelvin
{
  const double & tk = t;

  constexpr double tc = 647.096; // °K
  constexpr double pc = 22.064; // MPa

  const double v = 1 - (tk/tc);
  const double v2 = v*v;
  const double v3 = v2*v;
  const double v4 = v3*v;
        
  const double lnpv = ((tc/tk) *
		       (-7.85951783*v + 1.84408259*pow(v, 1.5) +
			-11.7866497*v3 + 22.6807411*pow(v, 3.5) +
			-15.9618719*v4 + 1.80122502 * pow(v, 7.5))) + log(pc);

  const double pv = exp(lnpv);

  return pv;
}

inline double
MwSpiveyMN::impl(const double & t, // comes in Celsius
		  const double & p,
		  const double & pv,
		  const double & nacl) noexcept
{
  const Quantity<Celsius> tc = t; 

  const double t_100 = t/100;
  const double t_100_2 = t_100*t_100;

  const double a = -0.007751*t_100_2 + 0.013624*t_100 + -0.0781;
  const double b = 0.01193*t_100_2 + 0.0851*t_100 + 1.02766;
  const double c = (1.8316*t_100_2 + -7.8119*t_100 + -3.6231) /
    (-0.10733*t_100_2 + 1.09192*t_100 + 1);

  const double log_p_pv = log(p - pv);
  const double mch4pw = exp(a*log_p_pv*log_p_pv + b*log_p_pv + c);

  const double tk = Quantity<Kelvin>(tc).raw();
  const double p2 = p*p;
  const double c1 = 7.015e-2 + 1.074e-4*tk + 2.260e-1*p/tk + -1.227e-3*p2/tk;
  constexpr double c2 = -6.28e-3;
  const double & m = nacl;
  const double m2 = m*m;

  const double mch4w = mch4pw * exp(-2*c1*m - c2*m2);

  return mch4w;
}

inline double
CgfwSpiveyMN::impl(const double & t, // comes in Celsius
		  const double & p,
		  const double & nacl) noexcept
{
  const double & m = nacl; 

  // Coefficients of compressibility of pure water
  const double t_100 = t/100;
  const double t_100_2 = t_100*t_100;

  const double epw = (4.221*t_100_2 + -3.478*t_100 + 6.221) / 
    (0.5182*t_100_2 + -0.4405*t_100 + 1);

  const double fpw = (-11.403*t_100_2 + 29.932*t_100 + 27.952) /
    (0.20684*t_100_2 + 0.3768*t_100 + 1);

  // Coefficients of gas-free brine compressibility
  constexpr double e = 0.1353;

  const double f1 = (-1.409*t_100_2 + -0.361*t_100 + -0.2532)/(9.216*t_100 + 1);
  const double f2 = (5.614*t_100 + 4.6782) / (-0.307*t_100_2 + 2.6069*t_100 + 1);
  const double f3 = -0.1127*t_100_2 + 0.2047*t_100 + -0.0452;
        
  const double ew = epw + e*m;
  const double fw = fpw + f1*pow(m, 1.5) + f2*m + f3*sqrt(m);

  // Compressibility of gas-free brine
  constexpr double inv_pref = 1.0/70.0; //Pressure of reference: 70 [MPa]
  const double p_70 = p/70;
  const double cgfw1 = 1.0/(ew*p_70 + fw);
  const double cgfw = inv_pref * cgfw1;

  return cgfw;
}

void BwSpiveyMNDry::precondition(const Quantity<mPascal> & p,
				 const Quantity<mPascal> & pv) const
{
  if (p < pv)
    {
      ostringstream s;
      s << "BwSpiveyMN::impl: received pressure " << p
	<< " is less than computed vapor pressure " << pv;
      ALEPHTHROW(WrongValueVaporPressure, s.str());
    }  
}


inline double
BwSpiveyMNDry::impl(const double & t, // comes in Celsius
		    const double & p,
		    const double & pv,
		    const double & nacl) noexcept
{
  const double & m = nacl;

  // Transformation from °C to °K
  const double tk = unit_convert<Celsius, Kelvin>(t);

  // pw: DENSITY OF BRINE WITH DISSOLVED METHANE
  // Density of methane-free brine at the temperature and pressure of
  // evaluation 
  const double pgfw =
    PwSpiveyMNGasFree::get_instance().impl(t, p, nacl);

  // Specific volume of the gas-free brine [cm³/g]
  const double vgfw = 1.0/pgfw;

  // mCH4w: SOLUBILITY OF METHANE IN BRINE [gmol NaCl/kgH2O] 
  // AT THE TEMPERATURE AND PRESSURE OF EVALUATION
  const double mch4w =
    MwSpiveyMN::get_instance().impl(t, p, pv, nacl);

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

  const Quantity<Celsius> Tstd = 15.555556; // °C (60 °F)  
  constexpr double Pstd = 0.101352; // MPa (14.7 psia)

  // Density of methane-free brine [g/cm³] at standard temperature and pressure
  const double pgfwstd =
    PwSpiveyMNGasFree::get_instance().impl(Tstd.raw(), Pstd, nacl);

  // Specific volume of methane-free brine [cm³/g]
  const double vgfwstd = 1.0/pgfwstd;

  // Bw: FORMATION VOLUME FACTOR [res cm³/ST cm³]       
  // Molecular weight MNaCl: 58.4428 g/gmol
  const double n = (1000 + m * 58.4428) * vgfw + mch4w * vmch4w; // Volume
								 // at
								 // reservoir
								 // conditions 
  const double d = (1000 + m * 58.4428) * vgfwstd; // Volume at stock
						   // tank conditions
  const double bw = n/d;

  return double(bw);
}


#endif // SPIVEY_DRY_IMPL_H
