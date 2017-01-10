# ifndef WATER_VISCOSITY_IMPL_H
# define WATER_VISCOSITY_IMPL_H

# include <correlations/water-viscosity.H>
# include <correlations/water-density.H>

inline double
UwMaoDuan::impl(const double & t,
		const double & nacl_concentration,
		const double & ppw) noexcept
{
  const double t2 = t*t;
  
  // Viscosity of pure water [Pa.s]
  const double upw = exp((0.28853170e7/t2 + -0.11072577e5/t +
			  -0.90834095e1 + 0.30925651e-1*t +
			  -0.27407100e-4*t2) + 
			 ppw * (-0.19283851e7/t2 + 0.56216046e4/t + 
				0.13827250e2 + -0.47609523e-1*t + 
				0.35545041e-4*t2));
        
  // Calculation of the coefficients (salinity of sodium chloride) 
  const double a = -0.21319213 + 0.13651589e-2*t + -0.12191756e-5*t2;
  const double b = 0.69161945e-1 + -0.27292263e-3*t + 0.20852448e-6*t2;
  const double c = -0.25988855e-2 + 0.77989227e-5*t;

  const double m = nacl_concentration;
  const double m2 = m*m;
  const double m3 = m2*m;
        
  // Relative viscosity
  const double uwr = exp(a*m + b*m2 + c*m3);
        
  // Viscosity of the solution [Pa.s]
  const double uw = uwr * upw; 
        
  return uw;
}

inline double
UwVanWingen::impl(const double & t) noexcept
{
  const double uw = exp(1.003 - 1.479e-2*t + 1.982e-5*t*t);

  return uw;
}

inline double
UwMatthewsRussel::impl(const double & t,
		       const double & p,
		       const double & nacl_concentration) noexcept
{
  const double s = nacl_concentration;
  const double s2 = s*s;

  // Calculation of water viscosity at atmospheric pressure
  const double a = -0.04518 + 0.009313*s - 0.000393*s2;
  const double b = 70.634 + 0.09576*s2;
  const double uwat = a + b/t;
        
  // Calculation of water viscosity corrected for pressure
  const double uw = uwat*(1 + 3.5e-12*p*p*(t - 40));

  return uw;
}

inline double
UwMcCain::impl(const double & t,
	       const double & p,
	       const double & nacl_concentration) noexcept
{
  const double s = nacl_concentration;
  const double s2 = s*s;
  const double s3 = s2*s;
  const double s4 = s3*s;

  const double a = 109.574 - 8.40564*s + 0.313314*s2 + 8.72213e-3*s3;
  const double b = -1.12166 + 2.63951e-2*s - 6.79461e-4*s2 -
    5.47119e-5*s3 + 1.55586e-6*s4;
  
  const double uwat = a * pow(t, b);
        
  const double uw = uwat * (0.9994 + 4.0295e-5*p + 3.1062e-9*p*p);

  return uw;
}


inline double
UwMcCoy::impl(const double & t,
	      const double & nacl_concentration) noexcept
{
  const double s = nacl_concentration;
  const double uwp = 0.02414 * pow(10, 247.8/(t - 140));

  const double uw = uwp * (1 - 1.87e-3*sqrt(s) + 2.18e-4*pow(s, 2.5) +
			   ((sqrt(t) - 1.35e-2*t) *
			    (2.76e-3*s - 3.44e-4*pow(s, 1.5))));

  return uw;
}


# endif // WATER_VISCOSITY_IMPL_H
