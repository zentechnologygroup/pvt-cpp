
# ifndef DEAD_OIL_VISCOSITY_IMPL_H
# define DEAD_OIL_VISCOSITY_IMPL_H

# include <correlations/dead-oil-viscosity.H>

inline double
UodBeal::impl(const double & api,
	      const double & t) const
{
  const double a = pow(10, 0.43 + (8.33 / api));
  const double b = 0.32 + (1.8e7 / pow(api, 4.53));
  const double c = 360.0 / (t + 200.0);

  return b * pow(c, a);
}

inline double
UodBeggsRobinson::impl(const double & api,
		       const double & t) const
{
  const double z = 3.0324 - 0.02023*api;
  const double y = pow(10, z);
  const double x = y * pow(t, -1.163);
        
  return pow(10, x) - 1;
}

inline double
UodEgbogahNg::impl(const double & api,
		   const double & t) const
{
  const double a = 1.8653 - 0.025086*api - 0.5644 * log10(t);
  const double b = pow(10, a);
  const double c = pow(10, b);

  return c - 1;
}

inline double
UodGlaso::impl(const double & api,
	       const double & t) const
{
  const double c = 3.141e10 * pow(t, -3.444);
  const double d = 10.313*log10(t) - 36.447;
  const double uod = c * pow(log10(api), d);

  return uod; 
}

inline double
UodKartoatmodjoSchmidt::impl(const double & api,
			     const double & t) const
{
  const double a = 5.7526*log10(t) - 26.9718;
  const double uod = 16e8 * pow(t, -2.8177) * pow(log10(api), a);

  return uod; 
}

inline double
UodSattarinEtAl::impl(const double & api,
		      const double & t) const
{
  double a = 0, b = 0, uod = 0;
  if (api < 28)
    {
      //a = -5.8936e7*pow(t, 2) + 3.511e10*t.raw() - 5.2145e12;
      a = -5.893604166667e7*pow(t, 2) + 3.51100078125001e10*t -
	5.21451314506928e12;
      b = 0.00418025*pow(t, 2) - 2.504062075*t + 368.787057006;
      uod = a*pow(api, b);
    }
  else
    {
      a = 0.00735*pow(t, 2) - 4.3175*t + 641.3572;
      b = -1.51*t + 568.84;
      uod = a * exp(b/api)/api;
    }

  return uod;
}

inline double
UodNaseri::impl(const double & api,
		const double & t) const
{
  const double a = log10(api);
  const double b = log10(t);
  const double c = 11.2699 - 4.2699*a - 2.052*b;
  const double uod = pow(10, c);

  return uod; 
}

inline double
UodPetroskyFarshad::impl(const double & api,
			 const double & t) const
{
  const double x = 4.59388*log10(t) - 22.82792;
  const double uod = 2.3511e7 * pow(t, -2.10255) * pow(log10(api), x);

  return uod;
}

inline double
UodDeGhettoEtAl::impl(const double & api,
		      const double & t) const
{
  double a = 0;
  if (api <=10) // Extra-heavy oil
    a = pow(10, 1.90296 - 0.012619*api - 0.61748*log10(t));
  else
    a = pow(10, 2.06492 - 0.0179*api - 0.70226*log10(t));
        
  const double uod = pow(10, a) - 1;

  return uod;
}

inline double
UodPerezML::impl(const double & api,
		 const double & t) const
{
  const double z = 1.6288 - 0.01537*api;
  const double x = pow(10, z) * pow(t, -0.4479);
		       
  const double uod = pow(10, x) - 1;

  return uod;
}

inline double
UodDindorukChristman::impl(const double & api,
			   const double & t,
			   const double & pb,
			   const double & rsb) const
{
  constexpr double a1 = 14.505357625;
  constexpr double a2 = -44.868655416;
  constexpr double a3 = 9.36579e9;
  constexpr double a4 = -4.194017808;
  constexpr double a5 = -3.1461171e-9;
  constexpr double a6 = 1.517652716;
  constexpr double a7 = 0.010433654;
  constexpr double a8 = -0.000776880;
        
  const double a = a1 * log10(t) + a2;
        
  const double uod = (a3*pow(t, a4)*pow(log10(api), a)) /
    (a5*pow(pb, a6) + a7*pow(rsb, a8));

  return uod;
}


# endif // DEAD_OIL_VISCOSITY_IMPL_H
