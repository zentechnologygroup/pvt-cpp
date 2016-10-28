# ifndef UNDERSATURATED_OIL_FORMATION_VOLUME_FACTOR_IMPL_H
# define UNDERSATURATED_OIL_FORMATION_VOLUME_FACTOR_IMPL_H

# include <correlations/undersaturated-oil-formation-volume-factor.H>


inline Quantity<RB_STB>
BoaMcCain::impl(const Quantity<psia> & p,
		const Quantity<psia> & pb,
		const Quantity<RB_STB> & bob,
		const Quantity<psia_1> & co) const

{
  const double bwa = bob.raw() * exp(co.raw() * (pb.raw() - p.raw()));

  return Quantity<RB_STB>(bwa);
}




 # endif // UNDERSATURATED_OIL_FORMATION_VOLUME_FACTOR_IMPL_H
