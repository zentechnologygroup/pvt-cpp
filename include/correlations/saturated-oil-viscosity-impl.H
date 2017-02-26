
# ifndef SATURATED_OIL_VISCOSITY_IMPL_H
# define SATURATED_OIL_VISCOSITY_IMPL_H

# include <correlations/saturated-oil-viscosity.H>

inline double
UobBeggsRobinson::impl(const double & uod,
		       const double & rs) noexcept
{
  const double a = 10.715 * pow(rs + 100, -0.515);
  const double b = 5.44 * pow(rs + 150, -0.338);

  const double uob = a * pow(uod, b);

  return uob;
}

inline double
UobChewConnally::impl(const double & uod,
		      const double & rs) noexcept
{
  const double a = pow(10, rs * (2.2e-7*rs - 7.4e-4));
  const double b = 0.68 /pow(10, 8.62e-5*rs) + 0.25/pow(10, 1.1e-3*rs) +
    0.062/ pow(10, 3.74e-3*rs);
  const double uob = a * pow(uod, b);

  return uob;
}

void UobKhan::precondition(const Quantity<psia> & p,
			   const Quantity<psia> & pb) const
{
  if (p > pb)
    {
      ostringstream s;
      s << "UobKhan::impl: pressure " << p << " is greater than pb " << pb;
      ZENTHROW(WrongValueSaturatedCondition, s.str());
    }
}

inline double
UobKhan::impl(const double & rsb,
	      const double & yo,
	      const double & yg,
	      const double & t,
	      const double & p,
	      const double & pb) noexcept
{
  const double tr = (t + 459.67)/459.67; // Relative Temperature
        
  const double a = 0.09*sqrt(yg);
  const double b = cbrt(rsb);
  const double c = pow(tr, 4.5);
  const double d = pow(1 - yo, 3); 
        
  const double uo_bubble = a / (b*c*d); // Bubble Point Oil Viscosity [cp] 

  const double uob = uo_bubble * pow(p/pb,-0.14) * exp(-2.5e-4*(p - pb));

  return uob;
}

inline double
UobKartoatmodjoSchmidt::impl(const double & uod,
			     const double & rs) noexcept
{
  const double y = pow(10, -0.00081*rs);
  const double a = 0.2001 + 0.8428 * pow(10, -0.000845 * rs);
  const double b = pow(uod, 0.43 + 0.5165 * y);
        
  const double f = a * b;
        
  const double uob = -0.06821 + 0.9824*f + 0.0004034*f*f;

  return uob;
}

inline double
UobPetroskyFarshad::impl(const double & uod,
			 const double & rs) noexcept
{
  const double a = 0.1651 + 0.6165*pow(10, -6.0866e-4*rs);
  const double b = 0.5131 + 0.5109 * pow(10, -1.1831e-3*rs);
  const double uob = a * pow(uod, b);
  
  return uob;
}

inline double
UobPerezML::impl(const double & uod,
		 const double & rs) noexcept
{
  const double b = 0.5704 + 0.4296 * pow(10, -0.00180*rs);
        
  const double uob = pow(uod, b);

  return uob;
}

inline double
UobGilFonseca::impl(const double & uod,
		    const double & rs) noexcept
{
  const double a = 0.76922 + 0.2244*pow(10, -0.0139 * rs);
  const double b = 0.10563 + 0.89405*pow(10, -0.00057 * rs);
        
  const double uob = a*pow(uod, b);

  return uob;
}

inline double
UobDeGhettoEtAl::impl(const double & uod,
		      const double & rs,
		      const double & api) noexcept
{
  const double y = pow(10, -0.00081*rs);

  double uob = 0;
  if (api <= 10) // Extra-heavy oil
    {
      const double f =
	(-0.0335 + 1.0785*pow(10, -0.000845*rs)) * pow(uod, 0.5798 + 0.3432*y);
      uob = 2.3945 + 0.8927*f + 0.001567*f*f;
    }
  else
    {      
      const double f =
	(0.2478 + 0.6114*pow(10, -0.000845*rs)) * pow(uod, 0.4731 + 0.5158*y);
      uob = -0.6311 + 1.078*f + 0.003653*f*f;
    }

  return uob;
}

inline double
UobDindorukChristman::impl(const double & uod,
			   const double & rs) noexcept
{
  constexpr double a1 = 1.0;
  constexpr double a2 = 4.740729e-4;
  constexpr double a3 = -1.023451e-2;
  constexpr double a4 = 6.600358e-1;
  constexpr double a5 = 1.075080e-3;
  constexpr double a6 = 1.0;
  constexpr double a7 = -2.191172e-5;
  constexpr double a8 = -1.660981e-2;
  constexpr double a9 = 4.233179e-1;
  constexpr double a10 = -2.273945e-4;
        
  const double a = a1/exp(a2*rs) + a3*pow(rs, a4)/exp(a5*rs);
        
  const double b = a6/exp(a7*rs) + a8*pow(rs, a9)/exp(a10*rs);
        
  const double uob = a * pow(uod, b);

  return uob;
}


# endif // SATURATED_OIL_VISCOSITY_IMPL_H
