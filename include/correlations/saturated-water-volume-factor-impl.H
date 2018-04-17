# ifndef SATURATED_WATER_VOLUME_FACTOR_IMPL_H
# define SATURATED_WATER_VOLUME_FACTOR_IMPL_H

# include <correlations/saturated-water-volume-factor.H>
# include <correlations/spivey-dry.H>

void BwbSpiveyMN::precondition(const Quantity<Celsius> & t,
			       const Quantity<mPascal> & p) const
{
  const double pv = PvSpiveyMN::get_instance().impl(Quantity<Kelvin>(t).raw());
  if (p < pv)
    {
      ostringstream s;
      s << "BwbSpiveyMN::impl: received pressure " << p
	<< " is less than computed vapor pressure " << pv;
      ALEPHTHROW(WrongValueVaporPressure, s.str());
    }
}

inline double
BwbSpiveyMN::impl(const double & t, // comes in Celsius
		  const double & p,
		  const double & nacl) noexcept
{
  // Vapor pressure of pure water, calculated from the IAWPS-95 formulation
  const double tk = unit_convert<Celsius, Kelvin>(t);
  const double pv = PvSpiveyMN::get_instance().impl(tk);
  return BwSpiveyMNDry::get_instance().impl(t, p, pv, nacl);
}

inline double
BwbMcCain::impl(const double & t,
	       const double & p) noexcept
{
  const double t2 = t*t;
  const double p2 = p*p;

  // Volume change as an effect of temperature
  const double dvwt = -1.0001e-2 + 1.33391e-4*t + 5.50654e-7*t2;

  // Volume change as an effect of pressure
  const double dvwp = -1.95301e-9*p*t - 1.72834e-13*p2*t -
    3.58922e-7*p - 2.25341e-10*p2;
        
  return (1 + dvwp) * (1 + dvwt);
}

inline double
BwbMcCoy::impl(const double & t,
	      const double & p,
	      const double & nacl) noexcept
{
  const double & s = nacl;
  const double t2 = t*t;
  const double p2 = p*p;
  
  // Coefficients for the calculation of Bw for pure water saturated with gas
  const double a = 0.9911 + 6.35e-5*t + 8.5e-7*t2;
  const double b = -1.093e-6 - 3.497e-9*t + 4.57e-12*t2;
  const double c = -5e-11 + 6.429e-13*t - 1.43e-15*t2;

  const double t60 = t - 60;
  const double t60_2 = t60*t60;
 
  const double bpw = a + b*p + c*p2; // Solubility of gas in pure water

  // Correction due to salinity
  const double bw = bpw * (1 + (s * (5.1e-8*p +
				     (5.47e-6 - 1.95e-10*p)*t60 -
				     (3.23e-8 - 8.5e-13*p*t60_2))));
   
  return bw;
}


# endif // SATURATED_WATER_VOLUME_FACTOR_IMPL_H
