# ifndef SATURATED_WATER_VOLUME_FACTOR_IMPL_H
# define SATURATED_WATER_VOLUME_FACTOR_IMPL_H

# include <correlations/saturated-water-volume-factor.H>
# include <correlations/spivey-dry.H>

inline Quantity<Rm3_Sm3>
BwbSpiveyMN::impl(const Quantity<Celsius> & t,
		  const Quantity<mPascal> & p,
		  const Quantity<Molality_NaCl> & nacl_concentration) const
{
  // Transformation from °C to °K
  const double tk = Quantity<Kelvin>(t).raw();

  // Vapor pressure of pure water, calculated from the IAWPS-95 formulation
  const double pv = PvSpiveyMN::get_instance().impl(tk).raw();

  if (p < pv)
    {
      ostringstream s;
      s << "BwbSpiveyMN::impl: received pressure " << p
	<< " is less than computed vapor pressure " << pv << " "
	<< p.unit.symbol;
      throw domain_error(s.str());
    }

  return BwSpiveyMNDry::get_instance().impl(t, p, pv, nacl_concentration);
}


inline Quantity<RB_STB>
BwbMcCain::impl(const Quantity<Fahrenheit> & t,
	       const Quantity<psia> & p) const
{
  const double t2 = t.raw()*t.raw();
  const double p2 = p.raw()*p.raw();

  // Volume change as an effect of temperature
  const double dvwt = -1.0001e-2 + 1.33391e-4*t.raw() + 5.50654e-7*t2;

  // Volume change as an effect of pressure
  const double dvwp = -1.95301e-9*p.raw()*t.raw() - 1.72834e-13*p2*t.raw() -
    3.58922e-7*p.raw() - 2.25341e-10*p2;
        
  const double bw = (1 + dvwp) * (1 + dvwt);

  return Quantity<RB_STB>(bw);
}


inline Quantity<RB_STB>
BwbMcCoy::impl(const Quantity<Fahrenheit> & t,
	      const Quantity<psia> & p,
	      const Quantity<Dissolved_Salt_Percent> & nacl_concentration) const
{
  const double s = nacl_concentration.raw();
  const double t2 = t.raw()*t.raw();
  const double p2 = p.raw()*p.raw();
  
  // Coefficients for the calculation of Bw for pure water saturated with gas
  const double a = 0.9911 + 6.35e-5*t.raw() + 8.5e-7*t2;
  const double b = -1.093e-6 - 3.497e-9*t.raw() + 4.57e-12*t2;
  const double c = -5e-11 + 6.429e-13*t.raw() - 1.43e-15*t2;

  const double t60 = t.raw() - 60;
  const double t60_2 = t60*t60;
 
  const double bpw = a + b*p.raw() + c*p2; // Solubility of gas in pure water

  // Correction due to salinity
  const double bw = bpw * (1 + (s * (5.1e-8*p.raw() +
				  (5.47e-6 - 1.95e-10*p.raw())*t60 -
				  (3.23e-8 - 8.5e-13*p.raw()*t60_2))));
   

  return Quantity<RB_STB>(bw);
}






# endif // SATURATED_WATER_VOLUME_FACTOR_IMPL_H
