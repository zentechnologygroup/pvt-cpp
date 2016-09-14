# ifndef GAS_PSEUDOCRITICAL_PRESSURE_IMPL_H
# define GAS_PSEUDOCRITICAL_PRESSURE_IMPL_H

# include <gas-pseudocritical-pressure.H>

inline Quantity<Sgg>
YgHCWichertAziz::impl(const Quantity<Sgg> & yg,
		      const Quantity<MolePercent> & n2_concentration,
		      const Quantity<MolePercent> & co2_concentration,
		      const Quantity<MolePercent> & h2s_concentration) const
{
  const double n2 = n2_concentration.raw();
  const double co2 = co2_concentration.raw();
  const double h2s = h2s_concentration.raw();
  const double yghc = (yg.raw() - 0.967*n2 - 1.52*co2 - 1.18*h2s) /
    (1 - n2 - co2 - h2s);

  return Quantity<Sgg>(yghc);
}



inline Quantity<psia>
PscMKay::impl(const Quantity<psia> & pschc,
	      const Quantity<MolePercent> & n2_concentration,
	      const Quantity<MolePercent> & co2_concentration,
	      const Quantity<MolePercent> & h2s_concentration) const
{
  const double n2 = n2_concentration.raw();
  const double co2 = co2_concentration.raw();
  const double h2s = h2s_concentration.raw();
  const double pscm = (1 - n2 - co2 - h2s)*pschc.raw() + 493*n2 + 1071*co2 +
    1306 * h2s;

  return Quantity<psia>(pscm);
}



inline Quantity<psia>
AdjustedPscWichertAziz::impl(const Quantity<psia> & pscm,
			     const Quantity<Fahrenheit> & tscm,
			     const Quantity<MolePercent> & co2_concentration,
			     const Quantity<MolePercent> & h2s_concentration)
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
PscHCStanding::impl(const Quantity<Sgg> & yghc,
		    const Quantity<MolePercent> &,
		    const Quantity<MolePercent> &,
		    const Quantity<MolePercent> &) const
{
  const double raw_yghc = yghc.raw();
  const double pschc = 677 + 15*raw_yghc - 37.5*raw_yghc*raw_yghc;
  return Quantity<psia>(pschc);
}

inline Quantity<psia>
PscHcSutton::impl(const Quantity<Sgg> & yghc,
		  const Quantity<MolePercent> &,
		  const Quantity<MolePercent> &) const
{
  const double pschc = 756.8 - 131.0 * yghc.raw() - 3.6 * pow(yghc, 2);
  return Quantity<psia>(pschc);
}

inline Quantity<psia>
PscHCGuoGhalambor::impl(const Quantity<Sgg> & yghc,
			const Quantity<MolePercent> & n2_concentration,
			const Quantity<MolePercent> & co2_concentration,
			const Quantity<MolePercent> & h2s_concentration) const
{
  if (n2_concentration.raw() + co2_concentration.raw() +
      h2s_concentration.raw() > 7)
    {
      ostringstream s;
      s << "Precondition Total content of inorganic compounds "
	<< "(n2_concentration + co2_concentration + h2s_concentration <= 7";
      throw domain_error(s.str());
    }

  const double pschc = 709.604 - 58.718*yghc.raw();

  return Quantity<psia>(pschc);
}

inline Quantity<psia>
PscAhmed::impl(const Quantity<Sgg> & yghc,
	       const Quantity<MolePercent> & n2_concentration,
	       const Quantity<MolePercent> & co2_concentration,
	       const Quantity<MolePercent> & h2s_concentration) const
{
  const double psc = 678 - 50*(yghc.raw() - 0.5) - 206.7*n2_concentration.raw() +
    440*co2_concentration.raw() + 606.7*h2s_concentration.raw();

  return Quantity<psia>(psc);
}


# endif // GAS_PSEUDOCRITICAL_PRESSURE_IMPL_H
