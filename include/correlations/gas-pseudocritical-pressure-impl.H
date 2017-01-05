# ifndef GAS_PSEUDOCRITICAL_PRESSURE_IMPL_H
# define GAS_PSEUDOCRITICAL_PRESSURE_IMPL_H

# include <correlations/gas-pseudocritical-pressure.H>

inline double
PpchcStanding::impl(const double & yghc) const
{
  const double ppchc = 677 + 15*yghc - 37.5*yghc*yghc;
  return ppchc;
}

inline double
PpchcSutton::impl(const double & yghc) const
{
  const double ppchc = 756.8 - 131.0 * yghc - 3.6 * yghc*yghc;
  return ppchc;
}

inline double
PpchcGuoGhalambor::impl(const double & yghc,
			const double &,
			const double &) const
{
  const double ppchc = 709.604 - 58.718*yghc;

  return ppchc;
}

void PpcAhmed::precondition(const Quantity<MoleFraction> & n2_concentration,
			    const Quantity<MoleFraction> & co2_concentration,
			    const Quantity<MoleFraction> & h2s_concentration) const
{
  if (n2_concentration.raw() + co2_concentration.raw() +
      h2s_concentration.raw() >= 1)
    throw domain_error("In PpcAhmed::impl: invalid values for "
		       "nonhydrocarbon concentrations");
}

inline double
PpcAhmed::impl(const double & yghc,
	       const double & n2_concentration,
	       const double & co2_concentration,
	       const double & h2s_concentration) const
{
  const double ppc = 678 - 50*(yghc - 0.5) - 206.7*n2_concentration +
    440*co2_concentration + 606.7*h2s_concentration;

  return ppc;
}

inline double
PpchcStandingHeavierFractions::impl(const double & yghc) const
{ 
  const double ppchc = 706 - 51.7*yghc - 11.1*yghc*yghc;
  return ppchc;
}                                                                                   

inline double
PpchcElsharkawyEtAl::impl(const double & yghc) const
{
  const double ppchc = 787.06 - 147.34*yghc - 7.916*yghc*yghc;
  return ppchc;
}


# endif // GAS_PSEUDOCRITICAL_PRESSURE_IMPL_H
