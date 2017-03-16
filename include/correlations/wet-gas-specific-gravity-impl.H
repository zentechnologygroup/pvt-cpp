# ifndef WET_GAS_SPECIFIC_GRAVITY_IMPL_H
# define WET_GAS_SPECIFIC_GRAVITY_IMPL_H

# include <correlations/wet-gas-specific-gravity.H>

inline double
YwgrMcCain::impl(const double & yg,
	     	 const double & yo,
             	 const double & rsp1,
	     	 const double & gpa,
	     	 const double & veq) noexcept
{
  return (rsp1 * yg + 4600.0 * yo + gpa)/(rsp1 + veq);
}

# endif // WET_GAS_SPECIFIC_GRAVITY_IMPL_H
