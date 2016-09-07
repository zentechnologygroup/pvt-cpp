
# ifndef SATURATED_OIL_VISCOSITY_IMPL_H
# define SATURATED_OIL_VISCOSITY_IMPL_H

# include <saturated-oil-viscosity.H>

inline Quantity<CP>
UobBeggsRobinson::impl(const Quantity<CP> & uod,
		       const Quantity<SCF_STB> & rs) const
{
  const double a = 10.715 * pow(rs + 100, -0.515);
  const double b = 5.44 * pow(rs + 150, -0.338);

  const double uob = a * pow(uod, b);

  return Quantity<CP>(uob);
}



# endif // DEAD_OIL_VISCOSITY_IMPL_H
