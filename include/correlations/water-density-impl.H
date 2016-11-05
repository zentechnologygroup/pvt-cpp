# ifndef WATER_DENSITY_IMPL_H
# define WATER_DENSITY_IMPL_H

# include <correlations/water-density.H>


inline Quantity<Gr_cm3>
PwSpiveyMN::impl(const Quantity<Celsius> & t,
		 const Quantity<mPascal> & p,
		 const Quantity<Molality_NaCl> & nacl_concentration) const
{
  // Transformation from °F to °K
  const double tk = Quantity<Kelvin>(t).raw();
        
  // Vapor pressure of pure water, calculated from the IAWPS-95 formulation
  constexpr double tc = 647.096; // °K
  constexpr double pc = 22.064; // MPa

  const double v = 1 - (tk/tc);
  const double v2 = v*v;
  const double v3 = v2*v;
  const double v4 = v3*v;
        
  const double lnpv = ((tc/tk) * (-7.85951783*v + 1.84408259*pow(v, 1.5) +
				  -11.7866497*v3 + 22.6807411*pow(v, 3.5) +
				  -15.9618719*v4 + 1.80122502 * pow(v, 7.5))) +
    log(pc);

  const double pv = exp(lnpv);
  if (p < pv)
    {
      ostringstream s;
      s << "PwSpiveyMN::impl: received pressure " << p
	<< " is less than computed vapour pressure " << pv << " "
	<< p.unit.name;
      throw domain_error(s.str());
    }

  const double t100 = t.raw()/100;
  const double t100_2 = t100*t100;
  const double log_p_pv = log(p.raw() - pv);
  const double p2 = pow2(p);
  
  // Solubility of methane in pure water
  const double a = -0.007751*t100_2 + 0.013624*t100 + -0.0781;
  const double b = 0.01193*t100_2 + 0.0851*t100 + 1.02766;
  const double c = (1.8316*t100_2 + -7.8119*t100 + -3.6231) /
    (-0.10733*t100_2 + 1.09192*t100 + 1);
  const double m = nacl_concentration.raw();
  const double m2 = m*m;

  const double mch4pw = exp(a*log_p_pv*log_p_pv + b*log_p_pv + c);
            
  // Solubility of methane in brine
  const double c1 = 7.015e-2 + 1.074e-4*tk + 2.260e-1*p.raw()/tk + -1.227e-3*p2/tk;
  constexpr double c2 = -6.28e-3;

  // mCH4w: SOLUBILITY OF METHANE IN BRINE [gmol NaCl/kgH2O] AT THE
  // TEMPERATURE AND PRESSURE OF EVALUATION 
  const double mch4w = mch4pw * exp(-2*c1*m - c2*m2);
            
  // VMCH4w: PARTIAL MOLAR VOLUME OF METHANE IN BRINE AT THE
  // TEMPERATURE AND PRESSURE OF EVALUATION
  
  // Derivatives with respect to P
  const double c3 = -8.5658e-2 + 1.31961e-2*log(tk) + 7.338/tk +
    9.05e-2/(680 - tk);
  
  const double c4 = 2.260e-1/tk + 2 * -1.227e-3 * p.raw()/tk;

  constexpr double R = 8.314467; // Universal gas constant [MPa
				 // cm³/gmol K] (McCain et al., 2011) 
            
  // Partial molar volume of methane in brine
  const double vmch4w = R*tk*(c3 + 2*m*c4);
            
  // pw: DENSITY OF BRINE WITH DISSOLVED METHANE
  // Density of methane-free brine at the temperature and pressure of
  // evaluation 
  const double pgfw =
    PwSpiveyMNGasFree::get_instance().impl(t, p, nacl_concentration).raw(); 

  // Specific volume of the gas-free brine [cm³/g]
  const double vgfw = 1.0/pgfw;
            
  // Molecular weights MNaCl: 58.4428 g/gmol; MCH4: 16.043 g/gmol 
  // Density of brine with dissolved methane [g/cm³] (pw = mass/volume)
  const double mass = 1000 + m*58.4428 + mch4w*16.043;
  const double volume = (1000 + m*58.4428) * vgfw + mch4w * vmch4w;
  const double pw = mass/volume;
            
  return Quantity<Gr_cm3>(pw);
}

inline Quantity<Gr_cm3>
PwSpiveyMNGasFree::impl(const Quantity<Celsius> & t,
			const Quantity<mPascal> & p,
			const Quantity<Molality_NaCl> & nacl_concentration) const
{
  const double t_100 = t.raw()/100;
  const double t_100_2 = t_100*t_100;
  const double m = nacl_concentration.raw();

  // Densidad del agua [g/cm³] pura a la presion de referencia 70Mpa y
  // T de evaluacion 
  const double ppwr = (-0.127213*t_100_2 + 0.645486*t_100 + 1.03265) /
    (-0.070291*t_100_2 + 0.639589*t_100 + 1);
        
  // Coeficientes de compresibilidad del agua pura 
  const double epw = (4.221*t_100_2 + -3.478*t_100 + 6.221) / 
    (0.5182*t_100_2 + -0.4405*t_100 + 1);

  const double fpw = (-11.403*t_100_2 + 29.932*t_100 + 27.952) /
    (0.20684*t_100_2 + 0.3768*t_100 + 1);
        
  // Coeficientes para la densidad de la salmuera libre de gas a la
  // presion de referencia 70Mpa 
  const double d1 = (-1.1149e-4*t_100_2 + 1.7105e-4*t_100 + -4.3766e-4); 
  const double d2 = (-8.878e-4*t_100_2 + -1.388e-4*t_100 + -2.96318e-3) /
    (0.51103*t_100 + 1);
  const double d3 = (2.1466e-3*t_100_2 + 1.2427e-2*t_100 + 4.2648e-2) /
    (-8.1009e-2*t_100_2 + 0.525417*t_100 + 1);
  const double d4 = (2.356e-4*t_100_2 + -3.636e-4*t_100 + -2.278e-4);

  // Densidad de la salmuera libre de gas
  const double pgfwr = ppwr + d1*m*m + d2*pow(m, 3.0/2.0) + d3*m + d4 * sqrt(m);
        
  // Coeficientes de compresibilidad de la salmuera libre de gas
  constexpr double e =  0.1249;

  const double f1 = (-0.617*t_100_2 + -0.747*t_100 + -0.4339)/(10.26*t_100 + 1);
  const double f2 = (9.917*t_100 + 5.1128) / (3.892*t_100 + 1);
  const double f3 = 0.0365*t_100_2 + -0.0369*t_100;
        
  const double ew = epw + e*m;
  const double fw = fpw + f1*pow(m, 1.5) + f2*m + f3*sqrt(m);
        
  const double iwr = (1/ew)*log(fabs(ew + fw));
  const double iw = (1/ew)*log(fabs((ew*(p.raw()/70)) + fw));
        
  // Densidad de la salmuera libre de gas a las condiciones T y P de
  // evaluacion [g/cm³] 
  const double pgfw = pgfwr*exp(iw - iwr);
                        
  return Quantity<Gr_cm3>(pgfw);
}


inline Quantity<Lb_ft3>
PwMcCain::impl(const Quantity<Dissolved_Salt_Percent> & s,
	       const Quantity<RB_STB> & bw) const
{
  const double s2 = s.raw()*s.raw();
  const double pwl = 62.368 + 0.438603*s.raw() + 1.60074e-3*s2;
        
  const double pw = pwl/bw.raw();

  return Quantity<Lb_ft3>(pw);
}

inline Quantity<Gr_cm3>
PpwSpiveyMN::impl(const Quantity<Celsius> & t,
		  const Quantity<mPascal> & p) const
{
  const double t100 = t.raw()/100;
  const double t100_2 = t100*t100;

  // Densidad del agua pura [g/cm³] a la presion de referencia 70Mpa
  const double ppwr = (-0.127213*t100_2 + 0.645486*t100 + 1.03265) /
    (-0.070291*t100_2 + 0.639589*t100 + 1);
        
  // Coeficientes de densidad del agua pura [g/cm³] segun Spivey
  const double epw = (4.221*t100_2 + -3.478*t100 + 6.221) /
    (0.5182*t100_2 + -0.4405*t100 + 1);
     
  const double fpw = (-11.403*t100_2 + 29.932*t100 + 27.952) /
    (0.20684*t100_2 + 0.3768*t100 + 1);
        
  const double ipwr = (1/epw)*log(fabs(epw + fpw));
  const double ipw = (1/epw)*log(fabs((epw * (p.raw()/70))+ fpw));
        
  // Densidad del agua pura [g/cm³]
  const double ppw = ppwr * exp(ipw - ipwr);
        
  return Quantity<Gr_cm3>(ppw);
}

# endif // WATER_DENSITY_IMPL_H
