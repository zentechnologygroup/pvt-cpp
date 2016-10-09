# ifndef WATER_VOLUME_FACTOR_IMPL_H
# define WATER_VOLUME_FACTOR_IMPL_H

# include "water-volume-factor.H"


inline Quantity<RB_STB>
BwSpiveyMN::impl(const Quantity<Celsius> & t,
		 const Quantity<mPascal> & p,
		 const Quantity<Disolved_Salt_Percent> & s) const
{
  // TERMINO1 vgfw: VOLUMEN ESPECIFICO DE LA SALMUERA LIBRE DE GAS A
  // CONDICIONES T Y P DE EVALUACION 
  // Densidad del agua [g/cm³] pura a la presion de referencia 70Mpa y
  // T de evaluacion

  const double t100 = t.raw()/100;
  const double t100_2 = t100*t100;
  const double s100 = s.raw()/100;

  const double ppwr = (-0.127213*t100_2 + 0.645486*t100 + 1.03265) /
    (-0.070291*t100_2 + 0.639589*t100 + 1);
        
  // Coeficientes de compresibilidad del agua pura 
  const double epw = (4.221*t100_2 + -3.478*t100 + 6.221) /
    (0.5182*t100_2 + -0.4405*t100 + 1);

  const double fpw = (-11.403*t100_2 + 29.932*t100 + 27.952) /
    (0.20684*t100_2 + 0.3768*t100 + 1);
        
  // Concentracion molal de NaCl [gmol/kgH2O] a partir de S
  const double m = (1000*s100)/(58.4428 * (1 - s100));
  const double m2 = m*m;
  const double sqrtm = sqrt(m);
  const double m_3_2 = pow(m, 1.5);

  // Coeficientes para la densidad de la salmuera libre de gas a la
  // presion de referencia 70Mpa 
  const double d1 = -1.1149e-4*t100_2 + 1.7105e-4*t100 + -4.3766e-4;
  const double d2 = (-8.878e-4*t100_2 + -1.388e-4*t100 + -2.96318e-3) /
    (0.51103*t100 + 1);
  const double d3 = (2.1466e-3*t100_2 + 1.2427e-2*t100 + 4.2648e-2) /
    (-8.1009e-2*t100_2 + 0.525417*t100 + 1);
  const double d4 = 2.356e-4*t100_2 + -3.636e-4*t100 + -2.278e-4;
        
  const double pgfwr = ppwr + d1*m2 + d2*m_3_2 + d3*m + d4*sqrtm;
        
  // Coeficientes de compresibilidad de la salmuera libre de gas
  constexpr double e = 0.1249;

  const double f1 = (-0.617*t100_2 + -0.747*t100 + -0.4339) / (10.26*t100 + 1);
  const double f2 = (9.917*t100 + 5.1128) / (3.892*t100 + 1);
  const double f3 = 0.0365*t100_2 + -0.0369*t100;
        
  const double ew = epw + e*m;
  const double fw = fpw + f1*m_3_2 + f2*m + f3*sqrtm;
        
  const double iwr = (1/ew) * log(fabs(ew + fw));
  const double iw = (1/ew) * log(fabs((ew * (p.raw()/70.))+ fw));
        
  // densidad de la salmuera libre de gas a las condiciones T y P de
  // evaluacion 
  const double pgfw = pgfwr * exp(iw - iwr);
        
  // Volumen especifico de la salmuera libre de gas
  const double vgfw = 1/pgfw;
        
  // TERMINO2 vgfwstd: VOLUMEN ESPECIFICO DE LA SALMUERA LIBRE DE GAS
  // A CONDICIONES ESTANDAR 
        
  constexpr double tstd100 = 15.555556/100; // ºC
  constexpr double tstd100_2 = tstd100*tstd100;
  constexpr double pstd = 0.101325; // MPa
        
  // Densidad del agua [g/cm³] pura a la presion de referencia 70Mpa y
  // T estandar 
  const double ppwrstd = (-0.127213*tstd100_2 + 0.645486*tstd100 + 1.03265) /
    (-0.070291*tstd100_2 + 0.639589*tstd100 + 1);
        
  // Coeficientes de compresibilidad del agua pura 
  const double epwstd = (4.221*tstd100_2 + -3.478*tstd100 + 6.221) /
    (0.5182*tstd100_2 + -0.4405*tstd100 + 1);
  const double fpwstd = (-11.403*tstd100_2 + 29.932*tstd100 + 27.952) /
    (0.20684*tstd100_2 + 0.3768*tstd100 + 1);
        
  // Concentracion molal de NaCl [gmol/kgH2O] a partir de S

  // Coeficientes para la densidad de la salmuera libre de gas a la
  // presion de referencia 70Mpa 
  const double d1std = -1.1149e-4*tstd100_2 + 1.7105e-4*tstd100 + -4.3766e-4;
  const double d2std = (-8.878e-4*tstd100_2 + -1.388e-4*tstd100 + -2.96318e-3) /
    (0.51103*tstd100 + 1);
  const double d3std = (2.1466e-3*tstd100_2 + 1.2427e-2*tstd100 + 4.2648e-2) /
    (-8.1009e-2*tstd100_2 + 0.525417*tstd100 + 1);
  const double d4std = (2.356e-4*tstd100_2 + -3.636e-4*tstd100 + -2.278e-4);
        
  const double pgfwrstd =
    ppwrstd + d1std*m2 + d2std*m_3_2 + d3std*m + d4std*sqrtm;
        
    // Coeficientes de compresibilidad de la salmuera libre de gas
  constexpr double estd = e;

  const double f1std = (-0.617*tstd100_2 + -0.747*tstd100 + -0.4339) /
    (10.26*tstd100 + 1);
  const double f2std = (9.917*tstd100 + 5.1128) / (3.892*tstd100 + 1);
  const double f3std = (0.0365*tstd100_2 + -0.0369*tstd100);

  const double ewstd = epwstd + estd*m;
  const double fwstd = fpwstd + f1std*m_3_2 + f2std*m + f3std * sqrtm;
        
  const double iwrstd = (1/ewstd) * log(fabs(ewstd + fwstd));
  const double iwstd = (1/ewstd) * log(fabs((ewstd * (pstd/70.))+ fwstd));
        
  // densidad de la salmuera libre de gas a las condiciones T y P estandar
  const double pgfwstd = pgfwrstd * exp(iwstd - iwrstd);
        
  // Volumen especifico de la salmuera libre de gas
  const double vgfwstd = 1/pgfwstd;
        
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
        
  // TERMINO4 mCH4w: SOLUBILIDAD DEL METANO EN LA SALMUERA
  // [gmol/KgH2O] A CONDICIONES P Y T DE EVALUACION 

  // Concentracion molal de NaCl [gmol/kgH2O] a partir de S

  // Presion de vapor del agua pura a partir de la formulacion IAWPS-95
  constexpr double tc = 647.096; // K
  constexpr double pc = 22.064; // MPa
  const double v = 1 - t273/tc;
  const double v2 = v*v;
  const double v3 = v2*v;
  const double v4 = v3*v;
        
  const double lnpv = (tc/t273)*(-7.85951783*v + 1.84408259 * pow(v, 1.5) +
				 -11.7866497*v3 + 22.6807411*pow(v, 3.5) +
				 -15.9618719*v4 + 1.80122502*pow(v, 7.5)) +
    log(pc);

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
      s << "BwSpiveyMN::impl: resulting calculations outside of mathematical"
	" domain";
      throw domain_error(s.str());
    }

  const double log_p_pv = log(p_pv);
  const double log_p_pv_2 = log_p_pv*log_p_pv;
        
  const double mch4pw = exp(a*log_p_pv_2 + b*log_p_pv + c);
            
  // Solubilidad del metano en salmuera
  const double c1 = -0.80898 + 1.0827e-3*t273 + 183.85/t273 + 3.924e-4*p.raw() +
    -1.97e-6 * p.raw()*p.raw();
  constexpr double c2 = -3.89e-3;
            
  const double mch4w = mch4pw * exp(-2*c1* m - c2*m2);
            
  // calculo DEL FACTOR VOLUMETRICO DE AGUA EN [cm³/std cm³]
  const double bw = (((1000 + m*58.4428)*vgfw) + mch4w*vmch4w) /
    ((1000 + m*58.4428) * vgfwstd);
            
  return Quantity<RB_STB>(bw);
}

inline Quantity<RB_STB>
BwMcCain::impl(const Quantity<Fahrenheit> & t,
	       const Quantity<psia> & p) const
{
  const double t2 = t.raw()*t.raw();
  const double p2 = p.raw()*p.raw();

  // Cambio de volumen por efecto de la temperatura
  const double dvwt = -1.0001e-2 + 1.33391e-4*t.raw() + 5.50654e-7*t2;

  // Cambio de volumen por efecto de la presión
  const double dvwp = -1.95301e-9*p.raw()*t.raw() - 1.72834e-13*p2*t.raw() -
    3.58922e-7*p.raw() - 2.25341e-10*p2;
        
  const double bw = (1 + dvwp) * (1 + dvwt);

  return Quantity<RB_STB>(bw);
}


inline Quantity<RB_STB>
BwMcCoy::impl(const Quantity<Fahrenheit> & t,
	      const Quantity<psia> & p,
	      const Quantity<psia> & pb,
	      const Quantity<Disolved_Salt_Percent> & s,
	      const Quantity<psia_1> & cw) const
{
  const double t2 = t.raw()*t.raw();
  const double p2 = p.raw()*p.raw();
  
  const double a = 0.9911 + 6.35e-5*t.raw() + 8.5e-7*t2;
  const double b = -1.093e-6 - 3.497e-9*t.raw() + 4.57e-12*t2;
  const double c = -5e-11 + 6.429e-13*t.raw() - 1.43e-15*t2;

  const double t60 = t.raw() - 60;
  const double t60_2 = t60*t60;
  double bw = 0;
  if (p < pb) // agua saturada con gas
    {
      const double bpw = a + b*p.raw() + c*p2; // solubilidad del gas
					       // en agua pura 

      // correccion por salinidad
      bw = bpw * (1 + (s.raw() * (5.1e-8*p.raw() +
				  (5.47e-6 - 1.95e-10*p.raw())*t60 -
				  (3.23e-8 - 8.5e-13*p.raw()*t60))));
    }
  else // agua subsaturada
    {
      const double pb2 = pb.raw()*pb.raw();
      const double bpwb = a + b*pb.raw() + c*pb2; // solubilidad del
						  // gas en agua pura
						  // en el punto de
						  // burbuja Pb 
      const double bwb = bpwb *
	(1 + (s.raw()*(5.1e-8*pb.raw() + (5.47e-6 - 1.95e-10*pb.raw())*t60) -
	      (3.23e-8 - 8.5e-13*pb.raw()*t60_2)));


      bw = bwb * exp(cw * (pb.raw() - p.raw()));
    }

  return Quantity<RB_STB>(bw);
}






# endif // WATER_VOLUME_FACTOR_IMPL_H
