# ifndef EQUIVALENT_VOLUME_IMPL_H
# define EQUIVALENT_VOLUME_IMPL_H

# include <correlations/equivalent-volume.H>

inline double
VeqspMcCain::impl(const double & tsep,
		  const double & psep,
		  const double & yg,
		  const double & api) noexcept
{
  constexpr double a0 = 635.53;
  constexpr double a1 = 0.361821;
  constexpr double a2 = 1.05435;
  constexpr double a3 = 5.08305;
  constexpr double a4 = 1.58124;
  constexpr double a5 = -0.791301;

  return a0 + a1*pow(psep, a2)*pow(yg, a3)*pow(api, a4)*pow(tsep, a5);
}


inline double
Veqsp2McCain::impl(const double & tsep,
		   const double & tsep2,
		   const double & psep,
		   const double & yg,
		   const double & api) noexcept
{
  constexpr double a0 = 535.916;
  constexpr double a1 = 2.6231;
  constexpr double a2 = 0.793183;
  constexpr double a3 = 4.6612;
  constexpr double a4 = 1.2094;
  constexpr double a5 = -0.849115;
  constexpr double a6 = 0.269869;

  return a0 +
    a1*pow(psep, a2)*pow(yg, a3)*pow(api, a4)*pow(tsep, a5)*pow(tsep2, a6);
}

# endif // EQUIVALENT_VOLUME_IMPL_H
