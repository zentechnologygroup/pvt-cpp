# ifndef GAS_PSEUDOCRITICAL_PRESSURE_IMPL_H
# define GAS_PSEUDOCRITICAL_PRESSURE_IMPL_H

# include <correlations/gas-pseudocritical-pressure.H>

inline Quantity<psia>
PscHCStanding::impl(const Quantity<Sgg> & yghc) const
{
  const double raw_yghc = yghc.raw();
  const double pschc = 677 + 15*raw_yghc - 37.5*raw_yghc*raw_yghc;
  return Quantity<psia>(pschc);
}

inline Quantity<psia>
PscHcSutton::impl(const Quantity<Sgg> & yghc) const
{
  const double pschc = 756.8 - 131.0 * yghc.raw() - 3.6 * pow(yghc, 2);
  return Quantity<psia>(pschc);
}

inline Quantity<psia>
PscHCGuoGhalambor::impl(const Quantity<Sgg> & yghc,
			const Quantity<MoleFraction> &,
			const Quantity<MoleFraction> &) const
{
  // if (n2_concentration.raw() + h2s_concentration.raw() > 7)
  //   {
  //     ostringstream s;
  //     s << "Precondition Total content of inorganic compounds "
  // 	<< "(n2_concentration + co2_concentration + h2s_concentration <= 7";
  //     throw domain_error(s.str());
  //   }

  const double pschc = 709.604 - 58.718*yghc.raw();

  return Quantity<psia>(pschc);
}

inline Quantity<psia>
PscAhmed::impl(const Quantity<Sgg> & yghc,
	       const Quantity<MoleFraction> & n2_concentration,
	       const Quantity<MoleFraction> & co2_concentration,
	       const Quantity<MoleFraction> & h2s_concentration) const
{
  const double psc = 678 - 50*(yghc.raw() - 0.5) - 206.7*n2_concentration.raw() +
    440*co2_concentration.raw() + 606.7*h2s_concentration.raw();

  return Quantity<psia>(psc);
}

inline Quantity<psia>
PscHCStandingHeavierFractions::impl (const Quantity<Sgg> & yghc) const
{ 
  const double pschc = 706 - 51.7*yghc.raw() - 11.1*yghc.raw()*yghc.raw();
  return Quantity<psia>(pschc);
}                                                                                   

inline Quantity<psia>
PscHCElsharkawyEtAl::impl(const Quantity<Sgg> & yghc) const
{
  const double pschc = 787.06 - 147.34*yghc.raw() - 7.916*yghc.raw()*yghc.raw();
  return Quantity<psia>(pschc);
}


# endif // GAS_PSEUDOCRITICAL_PRESSURE_IMPL_H
