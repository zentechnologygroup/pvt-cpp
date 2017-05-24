# ifndef GAS_PSEUDOCRITICAL_PRESSURE_IMPL_H
# define GAS_PSEUDOCRITICAL_PRESSURE_IMPL_H

# include <correlations/gas-pseudocritical-pressure.H>

inline double
PpchcStanding::impl(const double & yghc) noexcept
{
  const double ppchc = 677 + 15*yghc - 37.5*yghc*yghc;
  return ppchc;
}

inline double
PpchcSutton::impl(const double & yghc) noexcept
{
  const double ppchc = 756.8 - 131.0 * yghc - 3.6 * yghc*yghc;
  return ppchc;
}

inline double
PpchcStandingHeavierFractions::impl(const double & yghc) noexcept
{ 
  const double ppchc = 706 - 51.7*yghc - 11.1*yghc*yghc;
  return ppchc;
}                                                                                   

inline double
PpchcElsharkawyEtAl::impl(const double & yghc) noexcept
{
  const double ppchc = 787.06 - 147.34*yghc - 7.916*yghc*yghc;
  return ppchc;
}


# endif // GAS_PSEUDOCRITICAL_PRESSURE_IMPL_H
