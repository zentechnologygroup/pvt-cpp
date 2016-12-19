# ifndef SATURATED_WATER_ISOTHERMAL_COMPRESSIBILITY_IMPL_H
# define SATURATED_WATER_ISOTHERMAL_COMPRESSIBILITY_IMPL_H

# include <correlations/saturated-water-isothermal-compressibility.H>
# include <correlations/spivey-dry.H>

inline Quantity<mPa_1>
CwbSpiveyMN::impl(const Quantity<Celsius> & t,
		 const Quantity<mPascal> & p,
		 const Quantity<Molality_NaCl> & nacl_concentration,
		 const Quantity<Zfactor> & z) const
{

  // Transformation from °C to °K
  const double tk = Quantity<Kelvin>(t).raw();

  // Vapor pressure of pure water, calculated from the IAWPS-95 formulation
  const double pv =
    PvSpiveyMN::get_instance().impl(tk).raw();

  if (p < pv)
    {
      ostringstream s;
      s << "CwbSpiveyMN::impl: received pressure " << p
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

  // Isothermal compressibility for saturated brine
  const double t_100 = t.raw()/100;
  const double t_100_2 = t_100*t_100;
  const double a = -0.007751*t_100_2 + 0.013624*t_100 + -0.0781;
  const double b = 0.01193*t_100_2 + 0.0851*t_100 + 1.02766;
	
  const double log_p_pv = log(p.raw() - pv);
  const double p_pv = p.raw() - pv;

  const double dc1dp = 2.260e-1/tk + 2*-1.227e-3*p.raw()/tk;
  const double dmch4wdp = mch4w*((2*a*log_p_pv + b)/p_pv -
				     2*dc1dp*m);

  // VMCH4g: MOLAR VOLUME OF METHANE IN THE GAS PHASE
  const double vmch4g = z.raw()*R*tk/p.raw(); 
                               
  const double cw = -(1/volume) * ((1000 + m*58.4428)*dvgfwdp + mch4w*dvmch4wdp +
	      (dmch4wdp*(vmch4w - vmch4g)));

  return Quantity<mPa_1>(cw);                   
}


inline Quantity<psia_1>
CwbMcCain::impl(const Quantity<Fahrenheit> & t,
		const Quantity<psia> & p,
		const Quantity<Dissolved_Salt_Percent> & nacl_concentration,
		const Quantity<RB_SCF> & bg,
		const Quantity<RB_STB> & bw,
		const Quantity<psia_1> & cwa) const
{

  const double s = nacl_concentration.raw();
  const double t2 = pow2(t);
  const double t3 = t2*t.raw();
  const double t4 = t3*t.raw();

  // Estimation of the derivative of solution gas-water ratio with respect to pressure [scf/STB psia]
  const double b = 1.01021e-2 - 7.44241e-5 * t.raw() + 3.05553e-7 * t2 - 2.94883e-10 * t3;
  const double c = (-9.02505 + 0.130237 * t.raw() - 8.53425e-4 * t2 + 2.34122e-6 * t3 - 2.37049e-9 * t4) * 1e-7;

  const double drswdps = b + 2 * c * p.raw();

  // Correction of the derivative, due to dissolved solids
  const double drswdp = drswdps * pow(10, -0.0840655 * s * pow(t.raw(), -0.285854));

  // Isothermal compressibility
  const double cwb = cwa.raw() + bg.raw()/bw.raw() * drswdp;

  return Quantity<psia_1>(cwb);
}


# endif // SATURATED_WATER_ISOTHERMAL_COMPRESSIBILITY_IMPL_H
