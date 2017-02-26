
# ifndef GAS_DENSITY_IMPL_H
# define GAS_DENSITY_IMPL_H

# include <correlations/gas-density.H>

inline double
Pg::impl(const double & yg,
	 const double & t,
	 const double & p,
	 const double & z) noexcept
{
  return (2.70*p*yg) / (z*t);
}


# endif // GAS_DENSITY_IMPL_H
