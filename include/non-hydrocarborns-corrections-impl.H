# ifndef NON_HYDROCARBORNS_CORRECTIONS_IMPL_H
# define NON_HYDROCARBORNS_CORRECTIONS_IMPL_H

# include <non-hydrocarborns-corrections.H>

inline Quantity<Sgg>
YgHCWichertAziz::impl(const Quantity<Sgg> & yg,
		      const Quantity<MoleFraction> & n2_concentration,
		      const Quantity<MoleFraction> & co2_concentration,
		      const Quantity<MoleFraction> & h2s_concentration) const
{
  const double n2 = n2_concentration.raw();
  const double co2 = co2_concentration.raw();
  const double h2s = h2s_concentration.raw();
  const double yghc = (yg.raw() - 0.967*n2 - 1.52*co2 - 1.18*h2s) /
    (1 - n2 - co2 - h2s);

  return Quantity<Sgg>(yghc);
}

inline Quantity<psia>
PscMKayMixingRule::impl(const Quantity<psia> & pschc,
			const Quantity<MoleFraction> & n2_concentration,
			const Quantity<MoleFraction> & co2_concentration,
			const Quantity<MoleFraction> & h2s_concentration) const
{
  const double n2 = n2_concentration.raw();
  const double co2 = co2_concentration.raw();
  const double h2s = h2s_concentration.raw();
  const double pscm = (1 - n2 - co2 - h2s)*pschc.raw() + 493*n2 + 1071*co2 +
    1306 * h2s;

  return Quantity<psia>(pscm);
}



inline Quantity<psia>
AdjustedPscMWichertAziz::impl(const Quantity<psia> & pscm,
			      const Quantity<Fahrenheit> & tscm,
			      const Quantity<MoleFraction> & co2_concentration,
			      const Quantity<MoleFraction> & h2s_concentration)
  const
{
  const double a = co2_concentration.raw() + h2s_concentration.raw();
  const double b = h2s_concentration.raw();
        
  const double e = 120*(pow(a, 0.9) - pow(a, 1.6)) + 15*(sqrt(b) - pow(b, 4));

  const double n = pscm.raw() * (tscm.raw() - e);
  const double d = tscm.raw() + b * (1 - b) * e;
        
  const double adjusted_psc = n/d;

  return Quantity<psia>(adjusted_psc);
}


inline Quantity<psia>
TscMKayMixingRule::impl(const Quantity<psia> & tschc,
			const Quantity<MoleFraction> & n2_concentration,
			const Quantity<MoleFraction> & co2_concentration,
			const Quantity<MoleFraction> & h2s_concentration) const
{
  const double n2 = n2_concentration.raw();
  const double co2 = co2_concentration.raw();
  const double h2s = h2s_concentration.raw();
  const double tscm = (1 - n2 - co2 - h2s)*tschc.raw() + 227*n2 + 548*co2 +
    672*h2s;

  return Quantity<psia>(tscm);
}



# endif // NON_HYDROCARBORNS_CORRECTIONS_IMPL_H
