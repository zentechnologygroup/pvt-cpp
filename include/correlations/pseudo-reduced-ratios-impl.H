# ifndef PSEUDO_REDUCED_RATIOS_IMPL_H
# define PSEUDO_REDUCED_RATIOS_IMPL_H

# include <correlations/pseudo-reduced-ratios.H>


inline Quantity<PseudoReducedTemperature>
Tpr::impl(const Quantity<Rankine> & t,
	  const Quantity<Rankine> & tpcm) const
{
  return Quantity<PseudoReducedTemperature>(t.raw()/tpcm.raw());
}



inline Quantity<PseudoReducedPressure>
Ppr::impl(const Quantity<psia> & p,
	  const Quantity<psia> & ppcm) const
{
  return Quantity<PseudoReducedPressure>(p.raw()/ppcm.raw());
}




 # endif // PSEUDO_REDUCED_RATIOS_IMPL_H
