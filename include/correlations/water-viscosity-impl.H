# ifndef WATER_VISCOSITY_IMPL_H
# define WATER_VISCOSITY_IMPL_H

# include <correlations/water-viscosity.H>
# include <correlations/water-density.H>

inline Quantity<Paxs>
UwMaoDuan::impl(const Quantity<Kelvin> & t,
		const Quantity<Gr_cm3> & ppw,
		const Quantity<Disolved_Salt_PPM> & s) const
{
  const double t2 = pow2(t);
  
  // Viscosidad del agua pura [Pa.s]
  const double upw = exp((0.28853170e7/t2 + -0.11072577e5/t.raw() +
			  -0.90834095e1 + 0.30925651e-1*t.raw() +
			  -0.27407100e-4*t2) + 
			 ppw * (-0.19283851e7/t2 + 0.56216046e4/t.raw() + 
				0.13827250e2 + -0.47609523e-1*t.raw() + 
				0.35545041e-4*t2));
        
  // calculo de los coeficientes de salinidad 
  const double a = -0.21319213 + 0.13651589e-2*t.raw() + -0.12191756e-5*t2;
  const double b = 0.69161945e-1 + -0.27292263e-3*t.raw() + 0.20852448e-6*t2;
  const double c = -0.25988855e-2 + 0.77989227e-5*t.raw();

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
