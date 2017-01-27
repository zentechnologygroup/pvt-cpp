# ifndef UNDERSATURATED_OIL_DENSITY_IMPL_H
# define UNDERSATURATED_OIL_DENSITY_IMPL_H

# include <correlations/undersaturated-oil-density.H>

void PoaBradley::precondition(const Quantity<psia> & p,
			      const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "PoaBradley::impl: pressure " << p << " is not greater than pb " << pb;
      ZENTHROW(WrongValueUndersaturatedCondition, s.str());
    }
}

inline double
PoaBradley::impl(const double & pobp,
		 const double & p,
		 const double & pb,
		 const double & co) noexcept
{
  const double po = pobp * exp(co * (p - pb));

  return po;
}




# endif // UNDERSATURATED_OIL_DENSITY_IMPL_H
