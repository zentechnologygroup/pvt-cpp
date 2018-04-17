# ifndef SATURATED_OIL_COMPRESSIBILITY_IMPL_H
# define SATURATED_OIL_COMPRESSIBILITY_IMPL_H

# include <correlations/saturated-oil-compressibility.H>

void CobMcCainEtAl::precondition(const Quantity<psia> & p,
				 const Quantity<psia> & pb) const
{
  if (p > pb)
    {
      ostringstream s;
      s << "CobMcCainEtAl::impl: pressure " << p << " is greater than pb " << pb;
      ALEPHTHROW(WrongValueSaturatedCondition, s.str());
    }
}

inline double
CobMcCainEtAl::impl(const double & api,
		    const double & rsb,
		    const double & t,
		    const double & p,
		    const double & pb) noexcept
{
  return exp(-7.573 - 1.450*log(p) - 0.383*log(pb) + 1.402*log(t) +
	     0.256*log(api) + 0.449*log(rsb));
}


# endif // SATURATED_OIL_COMPRESSIBILITY_IMPL_H
