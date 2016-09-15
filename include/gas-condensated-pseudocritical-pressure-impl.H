# ifndef GAS_CONDENSATED_PSEUDOCRITICAL_PRESSURE__IMPL_H
# define GAS_CONDENSATED_PSEUDOCRITICAL_PRESSURE__IMPL_H

# include <gas-condensated-pseudocritical-pressure.H>

inline Quantity<psia>
CondensatePscHCStanding::impl(const Quantity<Sgg> & yghc,
			      const Quantity<MolePercent> &,
			      const Quantity<MolePercent> &,
			      const Quantity<MolePercent> &) const
{
  const double pschc = 706 - 51.7*yghc.raw() - 11.1*yghc.raw()*yghc.raw();
  return Quantity<psia>(pschc);
}


inline Quantity<psia>
CondensatePscHCElsharkawyEtAl::impl(const Quantity<Sgg> & yghc) const
{
  const double pschc = 787.06 - 147.34*yghc.raw() - 7.916*yghc.raw()*yghc.raw();
  return Quantity<psia>(pschc);
}



# endif // GAS_CONDENSATED_PSEUDOCRITICAL_PRESSURE_IMPL_H
