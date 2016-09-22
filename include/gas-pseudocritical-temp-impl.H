# ifndef GAS_PSEUDOCRITICAL_TEMP_IMPL_H
# define GAS_PSEUDOCRITICAL_TEMP_IMPL_H

# include <gas-pseudocritical-temp-impl.H>

inline Quantity<Rankine>
TscHCStanding::impl(const Quantity<Sgg> & yghc,
		    const Quantity<MoleFraction> &,
		    const Quantity<MoleFraction> &,
		    const Quantity<MoleFraction> &) const
{
  const double yghc2 = yghc.raw()*yghc.raw();
  const double tschc = 168 + 325*yghc.raw() - 12.5*yghc2;

  return Quantity<Rankine>(tschc);
}


inline Quantity<Rankine>
TscHCStandingHeavierFractions::impl(const Quantity<Sgg> & yghc,
				    const Quantity<MoleFraction> &,
				    const Quantity<MoleFraction> &,
				    const Quantity<MoleFraction> &) const
{
  const double yghc2 = yghc.raw()*yghc.raw();
  const double tschc = 187 + 330*yghc.raw() - 71.5*yghc2;

  return Quantity<Rankine>(tschc);
}


inline Quantity<Rankine>
TscHCSutton::impl(const Quantity<Sgg> & yghc,
		  const Quantity<MoleFraction> & ,
		  const Quantity<MoleFraction> & ,
		  const Quantity<MoleFraction> & ) const
{
  const double yghc2 = yghc.raw()*yghc.raw();
  const double tschc = 169.2 + 349.5*yghc.raw() - 74.0*yghc2;

  return Quantity<Rankine>(tschc);
}

inline Quantity<Rankine>
TpcHCElsharkawyEtAl::impl(const Quantity<Sgg> & yghc,
			  const Quantity<MoleFraction> & ,
			  const Quantity<MoleFraction> & ,
			  const Quantity<MoleFraction> & ) const
{
  const double yghc2 = yghc.raw()*yghc.raw();
  const double tpchc = 149.18 + 358.14*yghc.raw() - 66.976*yghc2;

  return Quantity<Rankine>(tpchc);
}



# endif // GAS_PSEUDOCRITICAL_TEMP_IMPL_H
