# ifndef SATURATED_OIL_DENSITY_IMPL_H
# define SATURATED_OIL_DENSITY_IMPL_H

# include <correlations/saturated-oil-density.H>


double
PobBradley::impl(const double & yg,
		 const double & rs,
		 const double & bob,
		 const double & yo) const
{
  return (350*yo + 0.0764*yg*rs)/(5.615*bob);
}


 # endif // SATURATED_OIL_DENSITY_IMPL_H
