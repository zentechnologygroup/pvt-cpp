# ifndef ADDITIONAL_GAS_PRODUCED_IMPL_H
# define ADDITIONAL_GAS_PRODUCED_IMPL_H

# include <correlations/additional-gas-produced.H>

inline double
GpaspMcCain::impl(const double & tsep,
		  const double & psep,
		  const double & yg,
		  const double & api) noexcept
{
  constexpr double a1 = 1.45993;
  constexpr double a2 = 1.3394;
  constexpr double a3 = 7.09434;
  constexpr double a4 = 1.14356;
  constexpr double a5 = -0.93446;

  return a1*pow(psep - 14.65, a2)*pow(yg, a3)*pow(api, a4)*pow(tsep, a5);
}


inline double
Gpasp2McCain::impl(const double & tsep,
	     const double & tsep2,
	     const double & psep,
             const double & yg,
	     const double & api) noexcept
{
  constexpr double a1 = 2.99222;
  constexpr double a2 = 0.970497;
  constexpr double a3 = 6.80491;
  constexpr double a4 = 1.07916;
  constexpr double a5 = -1.19605;
  constexpr double a6 = 0.553669;

  return a1*pow(psep - 14.65, a2)*pow(yg, a3)*
    pow(api, a4)*pow(tsep, a5)*pow(tsep2, a6);
}

# endif // ADDITIONAL_GAS_PRODUCED_IMPL_H
