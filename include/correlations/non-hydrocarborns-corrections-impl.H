# ifndef NON_HYDROCARBORNS_CORRECTIONS_IMPL_H
# define NON_HYDROCARBORNS_CORRECTIONS_IMPL_H

# include <correlations/non-hydrocarborns-corrections.H>

void
YghcWichertAziz::precondition(const Quantity<MoleFraction> & n2_concentration,
			      const Quantity<MoleFraction> & co2_concentration,
			      const Quantity<MoleFraction> & h2s_concentration)
  const
{
  if (n2_concentration.raw() + co2_concentration.raw() +
      h2s_concentration.raw() >= 1)
    throw domain_error("In yghcWichertAziz::impl: invalid values for nonhydrocarbon concentrations");
}

inline Quantity<Sgg>
YghcWichertAziz::impl(const Quantity<Sgg> & yg,
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

void
PpcmKayMixingRule::precondition(const Quantity<MoleFraction> & n2_concentration,
				const Quantity<MoleFraction> & co2_concentration,
				const Quantity<MoleFraction> & h2s_concentration)
  const
{
  if (n2_concentration.raw() + co2_concentration.raw() +
      h2s_concentration.raw() >= 1)
    throw domain_error("In PpcMKayMixingRule::impl: invalid values for nonhydrocarbon concentrations");
}

inline Quantity<psia>
PpcmKayMixingRule::impl(const Quantity<psia> & ppchc,
			const Quantity<MoleFraction> & n2_concentration,
			const Quantity<MoleFraction> & co2_concentration,
			const Quantity<MoleFraction> & h2s_concentration) const
{
  const double n2 = n2_concentration.raw();
  const double co2 = co2_concentration.raw();
  const double h2s = h2s_concentration.raw();
  const double ppcm = (1 - n2 - co2 - h2s)*ppchc.raw() + 493*n2 + 1071*co2 +
    1306 * h2s;

  return Quantity<psia>(ppcm);
}

void AdjustedppcmWichertAziz::precondition
(const Quantity<MoleFraction> & co2_concentration,
 const Quantity<MoleFraction> & h2s_concentration) const
{
  if (co2_concentration.raw() + h2s_concentration.raw() >= 1)
    throw domain_error("In AdjustedppcMWichertAziz::impl: invalid values for nonhydrocarbon concentrations");
}

inline Quantity<psia>
AdjustedppcmWichertAziz::impl(const Quantity<psia> & ppcm,
			      const Quantity<Fahrenheit> & tpcm,
			      const Quantity<MoleFraction> & co2_concentration,
			      const Quantity<MoleFraction> & h2s_concentration)
  const
{
  const double a = co2_concentration.raw() + h2s_concentration.raw();
  const double b = h2s_concentration.raw();
        
  const double e = 120*(pow(a, 0.9) - pow(a, 1.6)) + 15*(sqrt(b) - pow(b, 4));

  const double n = ppcm.raw() * (tpcm.raw() - e);
  const double d = tpcm.raw() + b * (1 - b) * e;
        
  const double adjusted_ppc = n/d;

  return Quantity<psia>(adjusted_ppc);
}

void TpcmKayMixingRule::precondition
(const Quantity<MoleFraction> & n2_concentration,
 const Quantity<MoleFraction> & co2_concentration,
 const Quantity<MoleFraction> & h2s_concentration) const
{
  if (n2_concentration.raw() + co2_concentration.raw() +
      h2s_concentration.raw() >= 1)
    throw domain_error("In TpcmKayMixingRule::impl: invalid values for nonhydrocarbon concentrations");
}

inline Quantity<Rankine>
TpcmKayMixingRule::impl(const Quantity<Rankine> & tpchc,
			const Quantity<MoleFraction> & n2_concentration,
			const Quantity<MoleFraction> & co2_concentration,
			const Quantity<MoleFraction> & h2s_concentration) const
{
  const double n2 = n2_concentration.raw();
  const double co2 = co2_concentration.raw();
  const double h2s = h2s_concentration.raw();
  const double tpcm = (1 - n2 - co2 - h2s)*tpchc.raw() + 227*n2 + 548*co2 +
    672*h2s;

  return Quantity<Rankine>(tpcm);
}

void AdjustedtpcmWichertAziz::precondition
(const Quantity<MoleFraction> & co2_concentration,
 const Quantity<MoleFraction> & h2s_concentration) const
{
  if (co2_concentration.raw() + h2s_concentration.raw() >= 1)
    throw domain_error("In AdjustedtpcmWichertAziz::impl: invalid values for nonhydrocarbon concentrations");
}

inline Quantity<Rankine>
AdjustedtpcmWichertAziz::impl(const Quantity<Rankine> & tpcm,
			      const Quantity<MoleFraction> & co2_concentration,
			      const Quantity<MoleFraction> & h2s_concentration)
  const
{
  const double a = co2_concentration.raw() + h2s_concentration.raw();
  const double b = h2s_concentration.raw();
  const double b4 = b*b*b*b;

  const double e = 120*(pow(a,  0.9) - pow(a, 1.6)) + 15*(sqrt(b) - b4);

  const double ret = tpcm.raw() - e;

  return Quantity<Rankine>(ret);
}


# endif // NON_HYDROCARBORNS_CORRECTIONS_IMPL_H
