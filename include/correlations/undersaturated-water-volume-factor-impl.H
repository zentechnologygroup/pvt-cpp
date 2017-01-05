# ifndef UNDERSATURATED_WATER_VOLUME_FACTOR_IMPL_H
# define UNDERSATURATED_WATER_VOLUME_FACTOR_IMPL_H

# include <correlations/undersaturated-water-volume-factor.H>
# include <correlations/spivey-dry.H>

inline double
BwaSpiveyMN::impl(const double & t, // comes in Celsius
		  const double & p,
		  const double & nacl_concentration) const
{
  // Transformation from °C to °K
  const Quantity<Celsius> tc = t;
  
  // Vapor pressure of pure water, calculated from the IAWPS-95 formulation
  const double pv = PvSpiveyMN::get_instance().call(tc).raw();
  if (p < pv)
    {
      ostringstream s;
      s << "BwaSpiveyMN::impl: received pressure " << p
	<< " is less than computed vapor pressure " << pv;
      throw domain_error(s.str());
    }

  return BwSpiveyMNDry::get_instance().call(tc, p, pv, nacl_concentration).raw();
}

inline double
BwaMcCain::impl(const double & p,
                const double & pb,
		const double & bwbp,
		const double & cw) const
{
  return bwbp * exp(cw * (pb - p));
}






# endif // UNDERSATURATED_WATER_VOLUME_FACTOR_IMPL_H
