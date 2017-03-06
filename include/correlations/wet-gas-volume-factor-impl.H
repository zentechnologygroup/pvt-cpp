# ifndef WET_GAS_VOLUME_FACTOR_IMPL_H
# define WET_GAS_VOLUME_FACTOR_IMPL_H

# include <correlations/wet-gas-volume-factor.H>

inline double
Bwg::impl(const double & t,
	  const double & p,
          const double & z,
	  const double & rsp1,
	  const double & veq) noexcept
{
  return 0.00502*z*(rsp1 + veq)*t/p;
}



# endif // WET_GAS_VOLUME_FACTOR_IMPL_H
