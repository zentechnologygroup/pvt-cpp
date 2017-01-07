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

inline double
YghcWichertAziz::impl(const double & yg,
		      const double & n2_concentration,
		      const double & co2_concentration,
		      const double & h2s_concentration)
{
  const double & n2 = n2_concentration;
  const double & co2 = co2_concentration;
  const double & h2s = h2s_concentration;
  const double yghc = (yg - 0.967*n2 - 1.52*co2 - 1.18*h2s) / (1 - n2 - co2 - h2s);

  return yghc;
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

inline double
PpcmKayMixingRule::impl(const double & ppchc,
			const double & n2_concentration,
			const double & co2_concentration,
			const double & h2s_concentration)
{
  const double & n2 = n2_concentration;
  const double & co2 = co2_concentration;
  const double & h2s = h2s_concentration;
  const double ppcm = (1 - n2 - co2 - h2s)*ppchc + 493*n2 + 1071*co2 +
    1306 * h2s;

  return ppcm;
}

void AdjustedppcmWichertAziz::precondition
(const Quantity<MoleFraction> & co2_concentration,
 const Quantity<MoleFraction> & h2s_concentration) const
{
  if (co2_concentration.raw() + h2s_concentration.raw() >= 1)
    throw domain_error("In AdjustedppcMWichertAziz::impl: invalid values for nonhydrocarbon concentrations");
}

inline double
AdjustedppcmWichertAziz::impl(const double & ppcm,
			      const double & tpcm,
			      const double & co2_concentration,
			      const double & h2s_concentration)
{
  const double a = co2_concentration + h2s_concentration;
  const double b = h2s_concentration;
        
  const double e = 120*(pow(a, 0.9) - pow(a, 1.6)) + 15*(sqrt(b) - pow(b, 4));

  const double n = ppcm * (tpcm - e);
  const double d = tpcm + b * (1 - b) * e;
        
  const double adjusted_ppc = n/d;

  return adjusted_ppc;
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

inline double
TpcmKayMixingRule::impl(const double & tpchc,
			const double & n2_concentration,
			const double & co2_concentration,
			const double & h2s_concentration)
{
  const double & n2 = n2_concentration;
  const double & co2 = co2_concentration;
  const double & h2s = h2s_concentration;
  const double tpcm = (1 - n2 - co2 - h2s)*tpchc + 227*n2 + 548*co2 + 672*h2s;

  return tpcm;
}

void AdjustedtpcmWichertAziz::precondition
(const Quantity<MoleFraction> & co2_concentration,
 const Quantity<MoleFraction> & h2s_concentration) const
{
  if (co2_concentration.raw() + h2s_concentration.raw() >= 1)
    throw domain_error("In AdjustedtpcmWichertAziz::impl: invalid values for nonhydrocarbon concentrations");
}

inline double
AdjustedtpcmWichertAziz::impl(const double & tpcm,
			      const double & co2_concentration,
			      const double & h2s_concentration)
{
  const double a = co2_concentration + h2s_concentration;
  const double b = h2s_concentration;
  const double b4 = b*b*b*b;

  const double e = 120*(pow(a,  0.9) - pow(a, 1.6)) + 15*(sqrt(b) - b4);

  const double ret = tpcm - e;

  return ret;
}


# endif // NON_HYDROCARBORNS_CORRECTIONS_IMPL_H
