# ifndef WATER_ISOTHERMAL_COMPRESSIBILITY_IMPL_H
# define WATER_ISOTHERMAL_COMPRESSIBILITY_IMPL_H

# include <water-isothermal-compressibility.H>


inline Quantity<mPa_1>
CwSpiveyMN::impl(const Quantity<Celsius> & t,
		 const Quantity<mPascal> & p,
		 const Quantity<mPascal> & pb,
		 const Quantity<Disolved_Salt_Percent> & s,
		 const Quantity<ZFactor> & z) const
{
  const double t100 = t.raw()/100;
  const double t100_2 = t100*100;

  // Coeficientes de compresibilidad del agua pura 
  const double epw = (4.221*t100_2 + -3.478*t100 + 6.221) /
    (0.5182*t100_2 + -0.4405*t100 + 1);
  const double fpw = (-11.403*t100_2 + 29.932*t100 + 27.952) /
    (0.20684*t100_2 + 0.3768*t100 + 1);

  const double s100 = s.raw()/100;

  // concentracion molal de NaCl [gmol/kgH2O] a partir de S
  const double m = (1000 * s100) / (58.4428 * (1 - s100));
  const double m2 = m*m;
  const double sqrtm = sqrt(m);
        
  // Coeficientes de compresibilidad de la salmuera libre de gas
  const double e = 0.1249;
  const double f1 = (-0.617*t100_2 + -0.747*t100 + -0.4339) / (10.26*t100 + 1);
  const double f2 = (9.917*t100 + 5.1128) / (3.892*t100 + 1);
  const double f3 = 0.0365*t100_2 + -0.0369*t100;
        
  const double ew = epw + e*m;
  const double fw = fpw + f1*pow(m, 1.5) + f2*m + f3*sqrtm;

  const double p70 = p.raw()/70;
  
  // Compresibilidad de la salmuera libre de gas
  const double cgfw = 70/(ew*p70 + fw);
        
  //termino2 vgfw: VOLUMEN ESPECIFICO DE LA SALMUERA LIBRE DE GAS A
  //CONDICIONES T Y P DE EVALUACION 
  // Densidad del agua [g/cm³] pura a la presion de referencia 70Mpa y
  //T de evaluacion 
  const double ppwr = (-0.127213*t100_2 + 0.645486*t100 + 1.03265) /
    (-0.070291*t100_2 + 0.639589*t100 + 1);
        
    // Coeficientes para la densidad de la salmuera libre de gas a la
  // presion de referencia 70Mpa 
  const double d1 = -1.1149e-4*t100_2 + 1.7105e-4*t100 + -4.3766e-4;
  const double d2 = (-8.878e-4*t100_2 + -1.388e-4*t100 + -2.96318e-3) /
    (0.51103*t100 + 1);
  const double d3 = (2.1466e-3*t100_2 + 1.2427e-2*t100 + 4.2648e-2) /
    (-8.1009e-2*t100_2 + 0.525417*t100 + 1);
  const double d4 = 2.356e-4*t100_2 + -3.636e-4*t100 + -2.278e-4;
        
  const double pgfwr = ppwr + d1*m2 + d2*pow(m, 1.5) + d3*m + d4 * sqrtm;
        
  const double iwr = (1/ew) * log(fabs(ew + fw));
  const double iw = (1/ew) * log(fabs(ew*p70 + fw));
        
  // Densidad de la salmuera libre de gas a las condiciones T y P de evaluacion
  const double pgfw = pgfwr * exp(iw - iwr);
        
  // Volumen especifico de la salmuera libre de gas
  const double vgfw = 1/pgfw;
        
  // TERMINO3 VMCH4w: VOLUMEN PARCIAL MOLAR DE METANO EN LA SALMUERA A
  // CONDICIONES P Y T DE EVALUACION 

  const double t273 = t.raw() + 273.15;
  const double t273_2 = t273*t273;

  // Derivadas con respecto a P de los siguientes terminos
  const double c3 = 7.698589e-2 + -5.0253331e-5*t273 + -30.092013/t273 +
    4.8468502e3/t273_2;
  const double c4 = 3.924e-4 + 2*-1.97e-6*p.raw();
            
  // Volumen molar parcial de metano en la salmuera
  const double vmch4w = 8.314467*(t.raw() + 273) * (c3 + 2*m*c4);

  // TERMINO4 dvgfwdP: DERIVADA DEL VOLUMEN ESPECIFICO DE LA SALMUERA
  // LIBRE DE GAS RESPECTO A LA PRESION 
  const double dvgfwdp = -(vgfw) * cgfw; 
        
  // TERMINO5 dVMCH4wdP: DERIVADA DEL VOLUMEN MOLAR DE METANO DISUELTO
  // EN SALMUERA RESPECTO A LA PRESION 
  const double dvmch4wdp = 8.314467 * t273 * 2 * m * 2 * -1.97e-6;
        
  // TERMINO4 mCH4w: SOLUBILIDAD DEL METANO EN LA SALMUERA
  // [gmol/KgH2O] A CONDICIONES P Y T DE EVALUACION 
        
  // Presion de vapor del agua pura a partir de la formulacion IAWPS-95
  constexpr double tc = 647.096; // K
  constexpr double pc = 22.064; // MPa
  const double v = 1 - t273/tc;
  const double v2 = v*v;
  const double v3 = v2*v;
  const double v4 = v3*v;
  const double logpc = log(pc);
        
  const double lnpv = (tc/t273 * (-7.85951783*v + 1.84408259*pow(v, 1.5) +
				  -11.7866497*v3 + 22.6807411*pow(v, 3.5) +
				  -15.9618719*v4 + 1.80122502*pow(v, 7.5))) + logpc;

  const double pv = exp(lnpv);
        
  // Solubilidad del metano en agua pura
  const double a = -0.004462*t100 + -0.06763;
  const double b = -0.03602*t100_2 + 0.18917*t100 + 0.97242;
  const double c = (0.6855*t100_2 + -3.1992*t100 + -3.7968) /
    (0.07711*t100_2 + 0.2229*t100 + 1);
         
  const double p_pv = p.raw() - pv;
  if (p_pv <= 0)
    {
      ostringstream s;
      s << "CwSpiveyMN::impl: computed pv = " << pv << " is greater than p = " << p;
      throw domain_error(s.str());
    }

  const double p2 = pow2(p);
  const double log_p_pv = log(p.raw() - pv);
  const double log_p_pv_2 = log_p_pv*log_p_pv;

  double cw = 0;
  if (p > pb) // compresibilidad de salmuera subsaturada
    {
      const double mch4pw = exp(a*log_p_pv_2 + b*log_p_pv + c);
                
      // Solubilidad del metano en salmuera
      const  double c1 = -0.80898 + 1.0827e-3*t273 + 183.85/t273 +
	3.924e-4*p.raw() + -1.97e-6*p2;
      constexpr double c2 = -3.89e-3;
                
      const double mch4w = mch4pw * exp(-2*c1*m - c2*m2);
                
      // pesos moleculares MNaCl: 58.4428 g/gmol; MCH4: 16.043 g/gmol
      cw = ((1000 + m*58.4428)*dvgfwdp + mch4w*dvmch4wdp) /
	((1000 + m*58.4428)*vgfw + mch4w*vmch4w);
      cw = -cw;
    }
  else // compresibilidad de salmuera saturada 
    {
      const double mch4pw = exp(a*log_p_pv_2 + b*log_p_pv + c);
                
      // solubilidad del metano en salmuera
      const double c1 = -0.80898 + 1.0827e-3*t273 + 183.85/t273 + 3.924e-4*p.raw() +
	-1.97e-6* p2;
      constexpr double c2 = -3.89e-3;
                
      const double mch4w = mch4pw * exp(-2*c1*m - c2*m2);
                
      // TERMINO7 dmCH4wdP: DERIVADA DE LA SOLUBILIDAD DEL METANO CON
      // RESPECTO A LA PRESION 
      const double dmch4wdp = mch4w*((2*a*log_p_pv + b)/p_pv -
				     2*(3.924e-4 + 2*-1.97e-6*p.raw())*m);

      // TERMINO8 VMCH4g: VOLUMEN MOLAR [cm³/gmol] DE METANO EN LA
      // FASE GASEOSA A CONDICIONES T y P DE EVALUACION 
      const double vmch4g = 8.314467*z.raw()*t273/p.raw();
                    
      // pesos moleculares MNaCl: 58.4428 g/gmol; MCH4: 16.043 g/gmol 
                               
      cw = ((1000 + m*58.4428)*dvgfwdp + mch4w*dvmch4wdp +
	    (dmch4wdp*(vmch4w - vmch4g))) / ((1000 + m*58.4428)*vgfw + mch4w*vmch4w);
      cw = -cw;
    }

  return Quantity<mPa_1>(cw);                   
}




 # endif // WATER_ISOTHERMAL_COMPRESSIBILITY_IMPL_H
