# ifndef GAS_VISCOSITY_IMPL_H
# define GAS_VISCOSITY_IMPL_H

# include <gas-viscosity.H>

inline Quantity<CP>
UgCarrKB::impl(const Quantity<Rankine> & t,
	       const Quantity<psia> & p,
	       const Quantity<Rankine> & tsc,
	       const Quantity<psia> & psc,
	       const Quantity<Sgg> & yg,
	       const Quantity<MolePercent> & n2_concentration,
	       const Quantity<MolePercent> & co2_concentration,
	       const Quantity<MolePercent> & h2s_concentration) const
{
  const double tsr = t.raw()/tsc.raw();
  const double psr = p.raw()/psc.raw();

  const double log10yg = log10(yg);

  const double ugs = (1.709e-5 - 2.062e-6*yg.raw()) * (t.raw() - 460) +
    8.188e-3 - 6.15e-3*log10yg;
        
  const double cco2 = 1e-3*co2_concentration.raw() * (9.08 * log10yg + 6.24);
  const double cn2 = 1e-3*n2_concentration.raw() * (8.48 * log10yg + 9.59);
  const double ch2s = 1e-3*h2s_concentration.raw() * (8.49 * log10yg + 3.73);
        
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
    tsr3*(a12 + a13*psr + a14*psr2 + a15*psr3) - log(tsr);
        
  const double ugugsc = exp(x);
        
  const double ug = ugsc * ugugsc;

  return Quantity<CP>(ug);
}




# endif // GAS_VISCOSITY_IMPL_H
