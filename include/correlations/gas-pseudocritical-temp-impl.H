# ifndef GAS_PSEUDOCRITICAL_TEMP_IMPL_H
# define GAS_PSEUDOCRITICAL_TEMP_IMPL_H

# include <correlations/gas-pseudocritical-temp.H>

inline double
TpchcStanding::impl(const double & yghc) const
{
  const double yghc2 = yghc*yghc;
  const double tpchc = 168 + 325*yghc - 12.5*yghc2;

  return tpchc;
}


inline double
TpchcStandingHeavierFractions::impl(const double & yghc) const
{
  const double yghc2 = yghc*yghc;
  const double tpchc = 187 + 330*yghc - 71.5*yghc2;

  return tpchc;
}


inline double
TpchcSutton::impl(const double & yghc) const
{
  const double yghc2 = yghc*yghc;
  const double tpchc = 169.2 + 349.5*yghc - 74.0*yghc2;

  return tpchc;
}

inline double
TpchcElsharkawyEtAl::impl(const double & yghc) const
{
  const double yghc2 = yghc*yghc;
  const double tpchc = 149.18 + 358.14*yghc - 66.976*yghc2;

  return tpchc;
}



# endif // GAS_PSEUDOCRITICAL_TEMP_IMPL_H
