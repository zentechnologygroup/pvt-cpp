# ifndef UNDERSATURATED_WATER_ISOTHERMAL_COMPRESSIBILITY_IMPL_H
# define UNDERSATURATED_WATER_ISOTHERMAL_COMPRESSIBILITY_IMPL_H

# include <correlations/undersaturated-water-isothermal-compressibility.H>
# include <correlations/spivey-dry.H>
# include "water-density.H"


void CwaSpiveyMN::precondition(const Quantity<Celsius> & t,
			       const Quantity<mPascal> & p) const
{
  const double pv = PvSpiveyMN::get_instance().impl(Quantity<Kelvin>(t).raw());
  if (p < pv)
    {
      ostringstream s;
      s << "CwaSpiveyMN::impl: received pressure " << p
	<< " is less than computed vapor pressure " << pv;
      ALEPHTHROW(WrongValueVaporPressure, s.str());
    }
}


inline double
CwaSpiveyMN::impl(const double & t, // comes in Celsius
		 const double & p,
		 const double & nacl) noexcept
{
  const double tk = unit_convert<Celsius, Kelvin>(t);

  // Vapor pressure of pure water, calculated from the IAWPS-95 formulation
  const double pv = PvSpiveyMN::get_instance().impl(tk);

  // vgfw: SPECIFIC VOLUME OF METHANE-FREE BRINE
  // Density of methane-free brine at the temperature and pressure of
  // evaluation [g/cm³]
  const double pgfw =
    PwSpiveyMNGasFree::get_instance().impl(t, p, nacl);
  
  // Specific volume of methane-free brine [cm³/g]
  const double vgfw = 1.0/pgfw;

  // mCH4w: SOLUBILITY OF METHANE IN BRINE [gmol NaCl/kgH2O] 
  // AT THE TEMPERATURE AND PRESSURE OF EVALUATION
  const double mch4w =
    MwSpiveyMN::get_instance().impl(t, p, pv, nacl);

  // VMCH4w: PARTIAL MOLAR VOLUME OF METHANE IN BRINE AT THE
  // TEMPERATURE AND PRESSURE OF EVALUATION  
  const double m = nacl;

  // Derivatives with respect to P
  const double c3 = -8.5658e-2 + 1.31961e-2*log(tk) + 7.338/tk +
    9.05e-2/(680 - tk);
  
  const double c4 = 2.260e-1/tk + 2 * -1.227e-3 * p/tk;

  constexpr double R = 8.314467; // Universal gas constant [MPa
				 // cm³/gmol K] (McCain et al., 2011) 

  // Partial molar volume of methane in brine
  const double vmch4w = R*tk*(c3 + 2*m*c4);

  // dvgfwdP: DERIVATIVE OF SPECIFIC VOLUME WITH RESPECT TO PRESSURE 

  const double cgfw =
    CgfwSpiveyMN::get_instance().impl(t, p, nacl);
  
  const double dvgfwdp = -vgfw * cgfw;  

  // dVMCH4wdP: DERIVATIVE OF MOLAR VOLUME OF METHANE DISSOLVED 
  // IN BRINE WITH RESPECT TO PRESSURE
  const double dc4dp = 2 * -1.227e-3/tk;
  const double dvmch4wdp = R * tk * 2 * m * dc4dp;

  const double volume = (1000 + m * 58.4428) * vgfw + mch4w * vmch4w;

  // Isothermal compressibility for brine undersaturated with respect to methane
  const double cw = -(1/volume) * ((1000 + m*58.4428)*dvgfwdp + mch4w*dvmch4wdp);
  
  return cw;
}

inline double
CwaDodsonStanding::impl(const double & t,
		       const double & p,
		       const double & rsw,
		       const double & nacl) noexcept
{
  const double t2 = t*t;
  const double t3 = t2*t;
  const double s07 = pow(nacl, 0.7);

  // Compressibility of gas-free water
  const double a = 3.8546 - 1.34e-4*p;
  const double b = -0.01052 + 4.77e-7*p;
  const double c = 3.9267e-5 - 8.8e-10*p;
  const double cwp = (a + b*t + c*t2)/1e6;

  // Compressibility of gas-saturated water
  const double cws = cwp * (1 + 8.9e-3*rsw);

  // Correction of the water compressibility, due to dissolved solids
  const double cw = cws * (1 + (s07 * (-5.2e-2 + 2.7e-4*t -
			               1.14e-6*t2 + 1.121e-9*t3)));
  return cw;
}


inline double
CwaOsif::impl(const double & t, const double & p, const double & nacl) noexcept
{
  const double cw = 1/(7.033*p + 541.5*nacl -
		       537*t + 403.3e3);
  return cw;
}


# endif // UNDERSATURATED_WATER_ISOTHERMAL_COMPRESSIBILITY_IMPL_H
