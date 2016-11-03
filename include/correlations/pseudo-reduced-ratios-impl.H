# ifndef PSEUDO_REDUCED_RATIOS_IMPL_H
# define PSEUDO_REDUCED_RATIOS_IMPL_H

# include <correlations/pseudo-reduced-ratios.H>


inline Quantity<PseudoReducedTemperature>
Tpr::impl(const Quantity<Rankine> & t,
	  const Quantity<Rankine> & tpc) const
{
  return Quantity<PseudoReducedTemperature>(t.raw()/tpc.raw());
}



inline Quantity<PseudoReducedPressure>
Ppr::impl(const Quantity<psia> & p,
	  const Quantity<psia> & ppc) const
{
  return Quantity<PseudoReducedPressure>(p.raw()/ppc.raw());
}




 # endif // PSEUDO_REDUCED_RATIOS_IMPL_H
