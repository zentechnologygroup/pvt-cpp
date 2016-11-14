# ifndef UNDERSATURATED_WATER_ISOTHERMAL_COMPRESSIBILITY_IMPL_H
# define UNDERSATURATED_WATER_ISOTHERMAL_COMPRESSIBILITY_IMPL_H

# include <correlations/undersaturated-water-isothermal-compressibility.H>
# include <correlations/spivey-dry.H>


inline Quantity<mPa_1>
CwaSpiveyMN::impl(const Quantity<Celsius> & t,
		 const Quantity<mPascal> & p,
		 const Quantity<Molality_NaCl> & nacl_concentration) const
{

  // Transformation from °C to °K
  const double tk = Quantity<Kelvin>(t).raw();

  // Vapor pressure of pure water, calculated from the IAWPS-95 formulation
  const double pv =
    PvSpiveyMN::get_instance().impl(tk).raw();

  if (p < pv)
    {
      ostringstream s;
      s << "CwaSpiveyMN::impl: received pressure " << p
	<< " is less than computed vapour pressure " << pv << " "
	<< p.unit.name;
      throw domain_error(s.str());
    }

  // vgfw: SPECIFIC VOLUME OF METHANE-FREE BRINE
  // Density of methane-free brine at the temperature and pressure of
  // evaluation [g/cm³]
  const double pgfw =
    PwSpiveyMNGasFree::get_instance().impl(t, p, nacl_concentration).raw();
  
  // Specific volume of methane-free brine [cm³/g]
  const double vgfw = 1.0/pgfw;

  // mCH4w: SOLUBILITY OF METHANE IN BRINE [gmol NaCl/kgH2O] 
  // AT THE TEMPERATURE AND PRESSURE OF EVALUATION
  const double mch4w =
    MwSpiveyMN::get_instance().impl(t, p, pv, nacl_concentration).raw();


  // VMCH4w: PARTIAL MOLAR VOLUME OF METHANE IN BRINE AT THE TEMPERATURE AND PRESSURE OF EVALUATION 
  const double m = nacl_concentration.raw();
  // Derivatives with respect to P
  const double c3 = -8.5658e-2 + 1.31961e-2*log(tk) + 7.338/tk + 9.05e-2/(680 - tk);
  
  const double c4 = 2.260e-1/tk + 2 * -1.227e-3 * p.raw()/tk;

  constexpr double R = 8.314467; // Universal gas constant [MPa
				 // cm³/gmol K] (McCain et al., 2011) 

  // Partial molar volume of methane in brine
  const double vmch4w = R*tk*(c3 + 2*m*c4);

  // dvgfwdP: DERIVATIVE OF SPECIFIC VOLUME WITH RESPECT TO PRESSURE 

  const double cgfw =
    CgfwSpiveyMN::get_instance().impl(t, p, nacl_concentration).raw();
  
  const double dvgfwdp = -vgfw * cgfw;  

  // dVMCH4wdP: DERIVATIVE OF MOLAR VOLUME OF METHANE DISSOLVED 
  // IN BRINE WITH RESPECT TO PRESSURE
  const double dc4dp = 2 * -1.227e-3/tk;
  const double dvmch4wdp = R * tk * 2 * m * dc4dp;

  const double volume = (1000 + m * 58.4428) * vgfw + mch4w * vmch4w;

  // Isothermal compressibility for brine undersaturated with respect to methane
  const double cw = -(1/volume) * ((1000 + m*58.4428)*dvgfwdp + mch4w*dvmch4wdp);
  
  return Quantity<mPa_1>(cw);                   
}


inline Quantity<psia_1>
CwaDodsonStanding::impl(const Quantity<Fahrenheit> & t,
		       const Quantity<psia> & p,
		       const Quantity<SCF_STB> & rsw,
		       const Quantity<Dissolved_Salt_Percent> & nacl_concentration) const
{
  const double t2 = pow2(t);
  const double t3 = t2*t.raw();
  const double s07 = pow(nacl_concentration, 0.7);

  // Compressibility of gas-free water
  const double a = 3.8546 - 1.34e-4*p.raw();
  const double b = -0.01052 + 4.77e-7*p.raw();
  const double c = 3.9267e-5 - 8.8e-10*p.raw();
  const double cwp = (a + b*t.raw() + c*t2)/1e6;

  // Compressibility of gas-saturated water
  const double cws = cwp * (1 + 8.9e-3*rsw.raw());

  // Correction of the water compressibility, due to dissolved solids
  const double cw = cws * (1 + (s07 * (-5.2e-2 + 2.7e-4*t.raw() -
			               1.14e-6*t2 + 1.121e-9*t3)));

  return Quantity<psia_1>(cw);
}


inline Quantity<psia_1>
CwaOsif::impl(const Quantity<Fahrenheit> & t,
	     const Quantity<psia> & p,
	     const Quantity<CgL> & nacl_concentration) const
{
  const double cw = 1/(7.033*p.raw() + 541.5*nacl_concentration.raw() -
			   537*t.raw() + 403.3e3);

  return Quantity<psia_1>(cw);
}




# endif // UNDERSATURATED_WATER_ISOTHERMAL_COMPRESSIBILITY_IMPL_H