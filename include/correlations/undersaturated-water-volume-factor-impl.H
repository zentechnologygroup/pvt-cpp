# ifndef UNDERSATURATED_WATER_VOLUME_FACTOR_IMPL_H
# define UNDERSATURATED_WATER_VOLUME_FACTOR_IMPL_H

# include <correlations/undersaturated-water-volume-factor.H>
# include <correlations/spivey-dry.H>


void BwaSpiveyMN::precondition(const Quantity<Celsius> & t,
			       const Quantity<mPascal> & p) const
{
  const double pv = PvSpiveyMN::get_instance().impl(Quantity<Kelvin>(t).raw());
  if (p < pv)
    {
      ostringstream s;
      s << "BwaSpiveyMN::impl: received pressure " << p
	<< " is less than computed vapor pressure " << pv;
      ALEPHTHROW(WrongValueVaporPressure, s.str());
    }
}

inline double
BwaSpiveyMN::impl(const double & t, // comes in Celsius
		  const double & p,
		  const double & nacl) noexcept
{
  const double tk = unit_convert<Celsius, Kelvin>(t);
  
  // Vapor pressure of pure water, calculated from the IAWPS-95 formulation
  const double pv = PvSpiveyMN::get_instance().impl(tk);

  return BwSpiveyMNDry::get_instance().impl(t, p, pv, nacl);
}

void BwaMcCain::precondition(const Quantity<psia> & p,
                	     const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "BwaMcCain::impl: pressure " << p << " is not greater than pb " << pb;
      ALEPHTHROW(WrongValueUndersaturatedCondition, s.str());
    }
}

inline double
BwaMcCain::impl(const double & p,
                const double & pb,
		const double & bwbp,
		const double & cwa) noexcept
{
  return bwbp * exp(cwa * (pb - p));
}






# endif // UNDERSATURATED_WATER_VOLUME_FACTOR_IMPL_H
