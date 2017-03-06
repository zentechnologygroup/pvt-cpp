# ifndef EQUIVALENT_VOLUME_IMPL_H
# define EQUIVALENT_VOLUME_IMPL_H

# include <correlations/equivalent-volume.H>

inline double
Veqsp1McCain::impl(const double & tsp1,
	     const double & psp1,
             const double & ygsp1,
	     const double & api) noexcept
{
  constexpr double a0 = 635.53;
  constexpr double a1 = 0.361821;
  constexpr double a2 = 1.05435;
  constexpr double a3 = 5.08305;
  constexpr double a4 = 1.58124;
  constexpr double a5 = -0.791301;

  return a0 + a1 * pow(psp1, a2) * pow(ygsp1, a3) * pow(api, a4) * pow(tsp1, a5);
}


inline double
Veqsp2McCain::impl(const double & tsp1,
	     const double & tsp2,
	     const double & psp1,
             const double & ygsp1,
	     const double & api) noexcept
{
  constexpr double a0 = 535.916;
  constexpr double a1 = 2.6231;
  constexpr double a2 = 0.793183;
  constexpr double a3 = 4.6612;
  constexpr double a4 = 1.2094;
  constexpr double a5 = -0.849115;
  constexpr double a6 = 0.269869;

  return a0 + a1 * pow(psp1, a2) * pow(ygsp1, a3) * pow(api, a4) * pow(tsp1, a5) * pow(tsp2, a6);
}

# endif // EQUIVALENT_VOLUME_IMPL_H
