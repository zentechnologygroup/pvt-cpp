# ifndef GAS_PSEUDOCRITICAL_PRESSURE_IMPL_H
# define GAS_PSEUDOCRITICAL_PRESSURE_IMPL_H

# include <correlations/gas-pseudocritical-pressure.H>

inline Quantity<psia>
PpchcStanding::impl(const Quantity<Sgg> & yghc) const
{
  const double raw_yghc = yghc.raw();
  const double ppchc = 677 + 15*raw_yghc - 37.5*raw_yghc*raw_yghc;
  return Quantity<psia>(ppchc);
}

inline Quantity<psia>
PpchcSutton::impl(const Quantity<Sgg> & yghc) const
{
  const double ppchc = 756.8 - 131.0 * yghc.raw() - 3.6 * pow(yghc, 2);
  return Quantity<psia>(ppchc);
}

inline Quantity<psia>
PpchcGuoGhalambor::impl(const Quantity<Sgg> & yghc,
			const Quantity<MoleFraction> &,
			const Quantity<MoleFraction> &) const
{
  const double ppchc = 709.604 - 58.718*yghc.raw();

  return Quantity<psia>(ppchc);
}

void PpcAhmed::precondition(const Quantity<MoleFraction> & n2_concentration,
			    const Quantity<MoleFraction> & co2_concentration,
			    const Quantity<MoleFraction> & h2s_concentration) const
{
  if (n2_concentration.raw() + co2_concentration.raw() + h2s_concentration.raw() >= 1)
    {
      ostringstream s;
      s << "In PpcAhmed::impl: invalid values for nonhydrocarbon concentrations.";
      throw domain_error(s.str());
    }
}

inline Quantity<psia>
PpcAhmed::impl(const Quantity<Sgg> & yghc,
	       const Quantity<MoleFraction> & n2_concentration,
	       const Quantity<MoleFraction> & co2_concentration,
	       const Quantity<MoleFraction> & h2s_concentration) const
{
  const double ppc = 678 - 50*(yghc.raw() - 0.5) - 206.7*n2_concentration.raw() +
    440*co2_concentration.raw() + 606.7*h2s_concentration.raw();

  return Quantity<psia>(ppc);
}

inline Quantity<psia>
PpchcStandingHeavierFractions::impl(const Quantity<Sgg> & yghc) const
{ 
  const double ppchc = 706 - 51.7*yghc.raw() - 11.1*yghc.raw()*yghc.raw();
  return Quantity<psia>(ppchc);
}                                                                                   

inline Quantity<psia>
PpchcElsharkawyEtAl::impl(const Quantity<Sgg> & yghc) const
{
  const double ppchc = 787.06 - 147.34*yghc.raw() - 7.916*yghc.raw()*yghc.raw();
  return Quantity<psia>(ppchc);
}


# endif // GAS_PSEUDOCRITICAL_PRESSURE_IMPL_H
