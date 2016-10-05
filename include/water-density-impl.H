# ifndef WATER_DENSITY_IMPL_H
# define WATER_DENSITY_IMPL_H

# include <water-density.H>


inline Quantity<Gr_cm3>
PwSpiveyMN::impl(const Quantity<Celsius> & t,
		 const Quantity<Pascal> & pr,
		 const Quantity<Disolved_Salt_PPM> & s) const
{
  const double p = pr.raw()/1000000; // pascal to MPA

  const double t_100 = t.raw()/100;
  const double t_100_2 = t_100*t_100;
  const double s_100 = s.raw()/100;

  // Densidad del agua [g/cm³] pura a la presion de referencia 70Mpa y
  // T de evaluacion 
  const double ppwr = (-0.127213*t_100_2 + 0.645486*t_100 + 1.03265) /
    (-0.070291*t_100_2 + 0.639589*t_100 + 1);
    
  // Coeficientes de compresibilidad del agua pura 
  const double epw = (4.221*t_100_2 + -3.478*t_100 + 6.221) /
    (0.5182*t_100_2 + -0.4405*t_100 + 1);

  const double fpw = (-11.403*t_100_2 + 29.932*t_100 + 27.952) /
    (0.20684*t_100_2 + 0.3768*t_100 + 1);
        
  // Concentracion molal de NaCl [gmol/kgH2O] a partir de S
  double m = (1000 * s_100)/(58.4428 * (1 - s_100));
        
  // Coeficientes para la densidad de la salmuera libre de gas a la
  // presion de referencia 70Mpa 
  const double d1 = -1.1149e-4*t_100_2 + 1.7105e-4*t_100 + -4.3766e-4;

  const double d2 = (-8.878e-4*t_100_2 + -1.388e-4*t_100 + -2.96318e-3) /
    (0.51103*t_100 + 1);

  const double d3 = (2.1466e-3*t_100_2 + 1.2427e-2*t_100 + 4.2648e-2) /
    (-8.1009e-2*t_100_2 + 0.525417*t_100 + 1);
  const double d4 = (2.356e-4*t_100_2 + -3.636e-4*t_100 + -2.278e-4);
        
  /// Densidad de la salmuera libre de gas
  const double pgfwr = ppwr + d1*m*m + d2*pow(m, 1.5) + d3*m + d4*sqrt(m);
        
  // Coeficientes de compresibilidad de la salmuera libre de gas
  constexpr double e = 0.1249;

  const double f1 = (-0.617*t_100_2 + -0.747*t_100 + -0.4339) /
    (10.26*t_100 + 1);
  
  const double f2 = (9.917*t_100 + 5.1128) / (3.892*t_100 + 1);

  const double f3 = 0.0365*t_100_2 + -0.0369*t_100;
        
  const double ew = epw + e*m;

  const double fw = fpw + f1*pow(m, 3.0/2.0) + f2*m + f3*sqrt(m);
        
  const double iwr = (1/ew) * log(fabs(ew + fw));
  const double iw = (1/ew) * log(fabs((ew * (p/70.))+ fw));
        
  // Densidad de la salmuera libre de gas a las condiciones T y P de
  // evaluacion [g/cm³] 
  const double pgfw = pgfwr * exp(iw - iwr);
        
  // Volumen especifico de la salmuera libre de gas [cm³/g]
  const double vgfw = 1/pgfw;
        
  // mCH4w: SOLUBILIDAD DEL METANO EN LA SALMUERA [gmol/KgH2O] A
  // CONDICIONES P Y T DE EVALUACION 

  // Presion de vapor del agua pura a partir de la formulacion IAWPS-95
  constexpr double tc = 647.096; // K
  constexpr double pc = 22.064; // MPa
  const double v = 1 - ((t.raw() + 273.15)/tc);

  const double v3 = v*v*v;
  const double v4 = v3*v;

  const double lnpv = ((tc/(t.raw() + 273.15)) *
		       (-7.85951783*v + 1.84408259*pow(v, 1.5) +
			-11.7866497*v3 + 22.6807411*pow(v, 3.5) +
			-15.9618719*v4  + 1.80122502*pow(v, 7.5))) + log(pc);

  const double pv = exp(lnpv);
        
  // Solubilidad del metano en agua pura
  const double a = ((-0.004462*t_100) + -0.06763);
  const double b = (-0.03602*t_100_2 + 0.18917*t_100 + 0.97242);
  const double c = (0.6855*t_100_2 + -3.1992*t_100 + -3.7968) /
    (0.07711*t_100_2 + 0.2229*t_100 + 1);
         
       
  const double domain = p - pv;
  if (domain <= 0)
    throw domain_error("PwSpiveyMN::impl: correlation outside of domain");

  const double log_p_pv = log(p - pv);
  const double mch4pw = exp(a*log_p_pv*log_p_pv + b*log_p_pv + c);
            
  // solubilidad del metano en salmuera
  const double c1 = -0.80898 + 1.0827e-3*(t.raw() + 273.15) +
    183.85/(t.raw() + 273.15) + 3.924e-4*p + -1.97e-6 * p*p;
  constexpr double c2 = -3.89e-3;
            
  const double mch4w = mch4pw*exp(-2*c1*m - c2*m*m);
            
  // VMCH4w: VOLUMEN PARCIAL MOLAR DE METANO EN LA SALMUERA A
  // CONDICIONES P Y T DE EVALUACION
  
  // Derivadas con respecto a P de los siguientes terminos
  const double c3 = 7.698589e-2 + -5.0253331e-5*(t.raw() + 273.15) +
    -30.092013/(t.raw() + 273.15) + 4.8468502e+3/pow(t.raw() + 273.15, 2);

  const double c4 = 3.924e-4 + 2*-1.97e-6*p;

  // Volumen molar parcial de metano en la salmuera
  const double vmch4w = 8.314467*(t.raw() + 273.15)*(c3 + 2*m*c4);
            
  // pesos moleculares MNaCl: 58.4428 g/gmol; MCH4: 16.043 g/gmol 
  // Densidad de la salmuera con gas disuelto [g/cm³]
  const double pw = (1000 + m*58.4428 + mch4w*16.043) /
    ((1000 + m*58.4428)*vgfw + 16.043*vmch4w);
            
  return Quantity<Gr_cm3>(pw);
}

inline Quantity<Gr_cm3>
PwSpiveyMNGasFree::impl(const Quantity<Celsius> & t,
			const Quantity<Pascal> & pr,
			const Quantity<Disolved_Salt_PPM> & s) const
{
  const double p = pr.raw()/1000000;

  const double t_100 = t.raw()/100;
  const double t_100_2 = t_100*t_100;
  const double s_100 = s.raw()/100;

  // Densidad del agua [g/cm³] pura a la presion de referencia 70Mpa y
  // T de evaluacion 
  const double ppwr = (-0.127213*t_100_2 + 0.645486*t_100 + 1.03265) /
    (-0.070291*t_100_2 + 0.639589*t_100 + 1);
        
  // Coeficientes de compresibilidad del agua pura 
  const double epw = (4.221*t_100_2 + -3.478*t_100 + 6.221) / 
    (0.5182*t_100_2 + -0.4405*t_100 + 1);

  const double fpw = (-11.403*t_100_2 + 29.932*t_100 + 27.952) /
    (0.20684*t_100_2 + 0.3768*t_100 + 1);
        
  // Concentracion molal de NaCl [gmol/kgH2O] a partir de S
  const double m = (1000*s_100)/(58.4428*(1 - s_100));
        
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
  const double iw = (1/ew)*log(fabs((ew*(p/70)) + fw));
        
  // Densidad de la salmuera libre de gas a las condiciones T y P de
  // evaluacion [g/cm³] 
  const double pgfw = pgfwr*exp(iw - iwr);
                        
  return Quantity<Gr_cm3>(pgfw);
}


inline Quantity<Lb_ft3>
PwMcCain::impl(const Quantity<Disolved_Salt_PPM> & s,
	       const Quantity<RB_STB> & bw) const
{
  const double s2 = s.raw()*s.raw();
  const double pwl = 62.368 + 0.438603*s.raw() + 1.60074e-3*s2;
        
  const double pw = pwl/bw.raw();

  return Quantity<Lb_ft3>(pw);
}


# endif // WATER_DENSITY_IMPL_H
