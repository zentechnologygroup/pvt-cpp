
# ifndef DEAD_OIL_VISCOSITY_IMPL_H
# define DEAD_OIL_VISCOSITY_IMPL_H

# include <dead-oil-viscosity.H>

inline Quantity<CP>
UodBeal::impl(const Quantity<Api> & api,
	      const Quantity<Fahrenheit> & t) const
{
  const double a = pow(10, 0.43 + (8.33 / api.raw()));
  const double b = 0.32 + (1.8e7 / pow(api, 4.53));
  const double c = 360.0 / (t.raw() + 200.0);

  const double uod = b * pow(c, a);

  return Quantity<CP>(uod);
}

inline Quantity<CP>
UodBeggsRobinson::impl(const Quantity<Api> & api,
		       const Quantity<Fahrenheit> & t) const
{
  const double z = 3.0324 - 0.02023*api.raw();
  const double y = pow(10, z);
  const double x = y * pow(t, -1.163);
        
  const double uod = pow(10, x) - 1;

  return Quantity<CP>(uod); 
}

inline Quantity<CP>
UodEgbogahNg::impl(const Quantity<Api> & api,
		   const Quantity<Fahrenheit> & t) const
{
  const double a = 1.8653 - 0.025086*api.raw() - 0.5644 * log10(t);
  const double b = pow(10, a);
  const double c = pow(10, b);

  const double uod = c - 1;

  return Quantity<CP>(uod); 
}

inline Quantity<CP>
UodGlaso::impl(const Quantity<Api> & api,
	       const Quantity<Fahrenheit> & t) const
{
  const double c = 3.141e10 * pow(t, -3.444);
  const double d = 10.313*log10(t) - 36.447;
  const double uod = c * pow(log10(api), d);

  return Quantity<CP>(uod); 
}

inline Quantity<CP>
UodKartoatmodjoSchmidt::impl(const Quantity<Api> & api,
			     const Quantity<Fahrenheit> & t) const
{
  const double a = 5.7526*log10(t) - 26.9718;
  const double uod = 16e8 * pow(t, -2.8177) * pow(log10(api), a);

  return Quantity<CP>(uod); 
}

inline Quantity<CP>
UodSattarinEtAl::impl(const Quantity<Api> & api,
		      const Quantity<Kelvin> & t) const
{
  const Quantity<Rankine> taux = t;
  const double raw_api = api.raw();
  double a = 0, b = 0, uod = 0;
  if (raw_api < 28)
    {
      // a = -5.8936e7*pow(t, 2) + 3.511e10*t.raw() - 5.2145e12;
      // b = 0.00418*pow(t, 2) - 2.50406*t.raw() + 368.78706;
      a = -5.8936e7*pow(taux, 2) + 3.511e10*taux.raw() - 5.2145e12;
      b = 0.00418*pow(taux, 2) - 2.50406*taux.raw() + 368.78706;
      uod = a*pow(api, b);
    }
  else
    {
      a = 0.00735*pow(t, 2) - 4.3175*t.raw() + 641.3572;
      b = -1.51*t.raw() + 568.84;
      uod = a * exp(b/raw_api)/raw_api;
    }
        
  return Quantity<CP>(uod); 
}

inline Quantity<CP>
UodNaseri::impl(const Quantity<Api> & api,
		const Quantity<Fahrenheit> & t) const
{
  const double a = log10(api);
  const double b = log10(t);
  const double c = 11.2699 - 4.2699*a - 2.052*b;
  const double uod = pow(10, c);

  return Quantity<CP>(uod); 
}

inline Quantity<CP>
UodPetroskyFarshad::impl(const Quantity<Api> & api,
			 const Quantity<Fahrenheit> & t) const
{
  const double x = 4.59388*log10(t) - 22.82792;
  const double uod = 2.3511e7 * pow(t, -2.10255) * pow(log10(api), x);

  return Quantity<CP>(uod);
}

inline Quantity<CP>
UodDeGhettoEtAl::impl(const Quantity<Api> & api,
		      const Quantity<Fahrenheit> & t) const
{
  double a = 0;
  if (api <=10) // Extra-heavy oil
    a = pow(10, 1.90296 - 0.012619*api.raw() - 0.61748*log10(t));
  else
    a = pow(10, 2.06492 - 0.0179*api.raw() - 0.70226*log10(t));
        
  const double uod = pow(10, a) - 1;

  return Quantity<CP>(uod);
}

# endif // DEAD_OIL_VISCOSITY_IMPL_H
