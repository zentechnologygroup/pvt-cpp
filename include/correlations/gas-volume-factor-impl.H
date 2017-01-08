# ifndef GAS_VOLUME_FACTOR_IMPL_H
# define GAS_VOLUME_FACTOR_IMPL_H

# include <correlations/gas-volume-factor.H>

inline double
Bg::impl(const double & t,
	 const double & p,
	 const double & z) noexcept
{
  return 0.0282793*z*t/p;
}



# endif // GAS_VOLUME_FACTOR_IMPL_H
