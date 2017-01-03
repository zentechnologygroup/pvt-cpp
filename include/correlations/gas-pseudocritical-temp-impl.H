# ifndef GAS_PSEUDOCRITICAL_TEMP_IMPL_H
# define GAS_PSEUDOCRITICAL_TEMP_IMPL_H

# include <correlations/gas-pseudocritical-temp.H>

inline Quantity<Rankine>
TpchcStanding::impl(const Quantity<Sgg> & yghc) const
{
  const double yghc2 = yghc.raw()*yghc.raw();
  const double tpchc = 168 + 325*yghc.raw() - 12.5*yghc2;

  return Quantity<Rankine>(tpchc);
}


inline Quantity<Rankine>
TpchcStandingHeavierFractions::impl(const Quantity<Sgg> & yghc) const
{
  const double yghc2 = yghc.raw()*yghc.raw();
  const double tpchc = 187 + 330*yghc.raw() - 71.5*yghc2;

  return Quantity<Rankine>(tpchc);
}


inline Quantity<Rankine>
TpchcSutton::impl(const Quantity<Sgg> & yghc) const
{
  const double yghc2 = yghc.raw()*yghc.raw();
  const double tpchc = 169.2 + 349.5*yghc.raw() - 74.0*yghc2;

  return Quantity<Rankine>(tpchc);
}

inline Quantity<Rankine>
TpchcElsharkawyEtAl::impl(const Quantity<Sgg> & yghc) const
{
  const double yghc2 = yghc.raw()*yghc.raw();
  const double tpchc = 149.18 + 358.14*yghc.raw() - 66.976*yghc2;

  return Quantity<Rankine>(tpchc);
}



# endif // GAS_PSEUDOCRITICAL_TEMP_IMPL_H
