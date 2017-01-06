
# ifndef GAS_DENSITY_IMPL_H
# define GAS_DENSITY_IMPL_H

# include <correlations/gas-density.H>

void Pg::precondition(const Quantity<psia> & p,
		      const Quantity<psia> & pb) const
{
  if (p > pb)
    {
      ostringstream s;
      s << "In Pg::impl: parameter p = " << p
	<< " is greater than parameter pb = " << pb;
      throw domain_error(s.str());
    }
}

inline double
Pg::impl(const double & yg,
	 const double &,
	 const double & t,
	 const double & p,
	 const double & z) const
{
  return (2.70*p*yg) / (z*t);
}


# endif // GAS_DENSITY_IMPL_H
