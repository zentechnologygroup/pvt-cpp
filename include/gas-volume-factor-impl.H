# ifndef GAS_VOLUME_FACTOR_IMPL_H
# define GAS_VOLUME_FACTOR_IMPL_H

# include <gas-volume-factor.H>


inline Quantity<RCF_SCF>
Bg::impl(const Quantity<Rankine> & t,
	 const Quantity<psia> & p,
	 const Quantity<ZFactor> & z) const
{
  return Quantity<RCF_SCF>(0.02827*z.raw()*t.raw()/p.raw());
}



# endif // GAS_VOLUME_FACTOR_IMPL_H
