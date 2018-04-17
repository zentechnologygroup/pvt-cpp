# ifndef GAS_VISCOSITY_IMPL_H
# define GAS_VISCOSITY_IMPL_H

# include <correlations/gas-viscosity.H>

void UgCarrKB::precondition(const Quantity<MoleFraction> & n2,
			    const Quantity<MoleFraction> & co2,
			    const Quantity<MoleFraction> & h2s) const
{
  if (n2.raw() + co2.raw() +
      h2s.raw() >= 1)
    {
    ostringstream s;
    s << "In UgCarrKB::impl: invalid values for nonhydrocarbon concentrations";
    ALEPHTHROW(WrongNonhydrocarbonsConcentrations, s.str());
    }
}

inline double
UgCarrKB::impl(const double & t,
	       const double & tpr,
	       const double & ppr,
	       const double & yg,
	       const double & n2,
	       const double & co2,
	       const double & h2s) noexcept
{
  const double & tsr = tpr;
  const double & psr = ppr;

  const double log10yg = log10(yg);

  const double ugs = (1.709e-5 - 2.062e-6*yg) * t + 8.188e-3 - 6.15e-3*log10yg;
        
  const double cco2 = 1e-3*co2 * (9.08 * log10yg + 6.24);
  const double cn2 = 1e-3*n2 * (8.48 * log10yg + 9.59);
  const double ch2s = 1e-3*h2s * (8.49 * log10yg + 3.73);
        
  const double ugsc = ugs + cco2 + ch2s + cn2;
        
  constexpr double a0 = -2.46211820e0;
  constexpr double a1 = 2.97054714e0;
  constexpr double a2 = -2.86264054e-1;
  constexpr double a3 = 8.05420522e-3;
  constexpr double a4 = 2.80860949;
  constexpr double a5 = -3.49803305;
  constexpr double a6 = 3.60373020e-1;
  constexpr double a7 = -1.04432413e-2;
  constexpr double a8 = -7.93385684e-1;
  constexpr double a9 = 1.39643306;
  constexpr double a10 = -1.49144925e-1;
  constexpr double a11 = 4.41015512e-3;
  constexpr double a12 = 8.39387178e-2;
  constexpr double a13 = -1.86408848e-1;
  constexpr double a14 = 2.03367881e-2;
  constexpr double a15 = -6.09579263e-4;

  const double psr2 = psr*psr;
  const double psr3 = psr2*psr;
  const double tsr2 = tsr*tsr;
  const double tsr3 = tsr2*tsr;
        
  const double x = a0 + a1*psr + a2*psr*psr + a3*psr3 +
    tsr*(a4 + a5*psr + a6*psr2 + a7 * psr3) +
    tsr2*(a8 + a9*psr + a10*psr2 + a11*psr3 ) + 
    tsr3*(a12 + a13*psr + a14*psr2 + a15*psr3);
        
  const double ugugs = exp(x)/tsr;
        
  const double ug = ugsc * ugugs;

  return ug;
}

inline double
UgLeeGE::impl(const double & t,
	      const double & p,
	      const double & yg,
	      const double & z) noexcept
{
  const double mg = 28.96*yg; // gas molecular weight [lb/lbmol]
            
  const double k = ((9.4 + 0.02*mg) * pow(t, 1.5) )/(209 + 19*mg + t);
  const double x = 3.5 + 986/t + 0.01*mg;
  const double y = 2.4 - 0.2*x;
            
  const double pg = 1.4935e-3*p*mg/(z*t); // gas density [g/cm3]
            
  const double ug = k*exp(x*pow(pg, y));

  return ug;
}

inline double
UgDeanStiel::impl(const double & t,
		  const double & p,
		  const double & tpc,
		  const double & ppc,
		  const double & yg,
		  const double & z) noexcept
{
  const double tsr = t/tpc;
  const double psr = p/ppc;
            
  const double mg = 28.96*yg; // gas molecular weight [lb/lbmol]

      // viscosity parameter
  const double em = 5.4402*pow(tpc, 1.0/6.0) / (pow(mg, 0.5)*pow(ppc, 2.0/3.0));
            
  const double pgr = 0.27*psr/(z*tsr); // pgr: relative gas density

  double ugs = 0;
  if (tsr <= 1.5)
    ugs = 34e-5*pow(tsr, 8.0/9.0) / em; 
  else 
    ugs = 166.8e-5 * pow(0.1338*tsr - 0.0932, 5.0/9.0) / em;

  const double ug = ugs + (10.8e-5*(exp(1.439*pgr) -
				    exp(-1.111*pow(pgr, 1.888))))/em;

  return ug;
}




# endif // GAS_VISCOSITY_IMPL_H
