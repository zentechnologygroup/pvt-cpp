# ifndef WATER_VISCOSITY_IMPL_H
# define WATER_VISCOSITY_IMPL_H

# include "water-viscosity.H"


inline Quantity<Paxs>
UwMaoDuan::impl(const Quantity<Fahrenheit> & t,
		const Quantity<mPascal> & p,
		const Quantity<Disolved_Salt_PPM> & s) const
{
  const double t100 = t.raw()/100;
  const double t100_2 = t100*t100;
  const double t273 = t.raw() + 273;
  const double t273_2 = t273*t273;

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
        
  // Viscosidad del agua pura [Pa.s]
  const double upw = exp((0.28853170e7/t273_2 + -0.11072577e5/t273 +
			  -0.90834095e1 + 0.30925651e-1*t273 +
			  -0.27407100e-4*t273_2) + 
			 ppw * (-0.19283851e7/t273_2 + 0.56216046e4/t273 + 
				0.13827250e2 + -0.47609523e-1*t273 + 
				0.35545041e-4*t273_2));
        
  // calculo de los coeficientes de salinidad 
  const double a = -0.21319213 + 0.13651589e-2*t273 + -0.12191756e-5*t273_2;
  const double b = 0.69161945e-1 + -0.27292263e-3*t273 + 0.20852448e-6*t273_2;
  const double c = -0.25988855e-2 + 0.77989227e-5*t273;

  const double s100 = s.raw()/100;

  // Concentracion molal de NaCl [gmol/kgH2O] a partir de S
  const double m = (1000*s100) /(58.4428 * (1 - s100));
  const double m2 = m*m;
  const double m3 = m2*m;
        
  // Viscosidad relativa de la salmuera
  const double uwr = exp(a*m + b*m2 + c*m3);
        
  // viscosidad de la salmuera [Pa.s]
  const double uw = uwr * upw; 
        
  return Quantity<Paxs>(uw);
}


inline Quantity<CP>
UwVanWingen::impl(const Quantity<Fahrenheit> & t) const
{
  const double uw = exp(1.003 - 1.479e-2*t.raw() + 1.982e-5*t.raw()*t.raw());

  return Quantity<CP>(uw);
}


inline Quantity<CP>
UwMatthewsRussel::impl(const Quantity<Fahrenheit> & t,
		       const Quantity<psia> & p,
		       const Quantity<Disolved_Salt_Percent> & s) const
{
  const double s2 = s.raw()*s.raw();
  const double a = -0.04518 + 0.009313*s.raw() - 0.000393*s2;
  const double b = 70.634 + 0.09576*s2;
  const double uwat = a + b/t.raw();
        
  // calculo de la viscosidad del agua corregida por presion
  const double uw = uwat*(1 + 3.5e-12*p.raw()*p.raw()*(t.raw() - 40));

  return Quantity<CP>(uw);
}


inline Quantity<CP>
UwMcCain::impl(const Quantity<Fahrenheit> & t,
	       const Quantity<psia> & p,
	       const Quantity<Disolved_Salt_Percent> & s) const
{
  const double s2 = s.raw()*s.raw();
  const double s3 = s2*s.raw();
  const double s4 = s3*s.raw();

  const double a = 109.574 - 8.40564*s.raw() + 0.313314*s2 + 8.72213e-3*s3;
  const double b = -1.12166 + 2.63951e-2*s.raw() - 6.79461e-4*s2 -
    5.47119e-5*s3 + 1.55586e-6*s4;
  
  const double uwat = a * pow(t, b);
        
  const double uw =
    uwat * (0.9994 + 4.0295e-5*p.raw() + 3.1062e-9*p.raw()*p.raw());

  return Quantity<CP>(uw);
}


inline Quantity<CP>
UwMcCoy::impl(const Quantity<Fahrenheit> & t,
	      const Quantity<Disolved_Salt_Percent> & s) const
{
  const double uwp =
    0.02414 * pow(10, (247.8/(((0.555555556 * t.raw()) + 255.37) - 140)));

  const double uw = uwp * (1 - 1.87e-3*sqrt(s) + 2.18e-4*pow(s, 2.5) +
			   ((sqrt(t) - 1.35e-2*t.raw()) *
			    (2.76e-3*s.raw() - 3.44e-4*pow(s, 1.5))));

  return Quantity<CP>(uw);
}


# endif // WATER_VISCOSITY_IMPL_H
