# ifndef GAS_ISOTHERMAL_COMPRESSIBILITY_IMPL_H
# define GAS_ISOTHERMAL_COMPRESSIBILITY_IMPL_H

# include <correlations/correlation-utils.H>
# include "gas-isothermal-compressibility.H"

inline Quantity<psia_1>
CgSarem::impl(const Quantity<PseudoReducedTemperature> & tpr,
	      const Quantity<PseudoReducedPressure> & ppr,
	      const Quantity<psia> & ppc,
	      const Quantity<ZFactor> & z) const
{
  const double tsr = tpr.raw();
  const double psr = ppr.raw();

  const double x = (2*psr - 15)/14.8;
  const double y = (2*tsr - 4)/1.9;

  const double x2 = x*x;
  const double x3 = x2*x;
  const double x4 = x3*x;

  const double y2 = y*y;
  const double y3 = y2*y;
  const double y4 = y3*y;
  const double y5 = y4*y;
                
  constexpr double p1x = 0.16551;
  const double p2x = 0.641002 * x;
  const double p3x = 0.379221 * (5*x2 - 1);
  const double p4x = 0.716652 * (7*x3 - 3*x);
  const double p5x = 0.594225 * (21*x4 - 14*x2 + 1);
                
  constexpr double p0y = 0.7071068;
  const double p1y = 1.224745 * y;
  const double p2y = 0.7905695 * (3*y2 - 1);
  const double p3y = 0.9354145 * (5*y3 - 3*y);
  const double p4y = 0.265165 * (35*y4 - 30*y2 + 3);
  const double p5y = 0.293151 * (63*y5 - 70*y3 + 15*y);
                
  const double dzdppr = 0.3312352*p1x*p0y +
    -0.1340361*p1x*p1y + 0.0668810*p1x*p2y + -0.0271743*p1x*p3y +
    0.0088512*p1x*p4y + -0.002152*p1x*p5y + 0.1057287*p2x*p0y +
    -0.0503937*p2x*p1y + 0.0050925*p2x*p2y + 0.0105513*p2x*p3y +
    -0.0073182*p2x*p4y + 0.0026960*p2x*p5y + 0.0521840*p3x*p0y +
    0.0443121*p3x*p1y + -0.0193294*p3x*p2y + 0.0058973*p3x*p3y +
    0.0015367*p3x*p4y + -0.0028327*p3x*p5y + 0.0197040*p4x*p0y +
    -0.0263834*p4x* p1y + 0.019262*p4x*p2y + -0.0115354*p4x*p3y +
    0.0042910*p4x*p4y + -0.0081303*p4x*p5y + 0.0053096*p5x*p0y +
    0.0089178*p5x*p1y + -0.0108948*p5x*p2y + 0.0095594*p5x*p3y +
    -0.0060114*p5x*p4y + 0.0031175*p5x*p5y;
                
  const double cgr = (1/psr) - (1/z.raw()) * dzdppr;
                
  const double cg = cgr/ppc.raw();

  return Quantity<psia_1>(cg);
}

inline Quantity<psia_1>
CgHallYarborough::impl(const Quantity<PseudoReducedTemperature> & __tpr,
		       const Quantity<PseudoReducedPressure> & __ppr,
		       const Quantity<psia> & ppc,
		       const Quantity<ZFactor> & z) const
{
  const double tpr = __tpr.raw();
  const double ppr = __ppr.raw();

  const double tpr_1 = 1/tpr;
  const double tpr_1_2 = tpr_1*tpr_1;
  const double tpr_1_3 = tpr_1_2*tpr_1;

  const double a = 0.06125*tpr_1 * exp(-1.2*pow(1 - tpr_1, 2));

  const double b = 14.76*tpr_1 - 9.76*tpr_1_2 + 4.58*tpr_1_3;
  const  double c = 90.7*tpr_1 - 242.2*tpr_1_2 + 42.4*tpr_1_3;
  const double d = 2.18 + 2.82*tpr_1;
                
  constexpr double epsilon = 1.0e-8;
  double pr = 0, prf = 0;
  double prprev = 0.01;
                
  while (fabs(prprev - pr) > epsilon)
    {
      pr = prprev;

      const double pr2 = pr*pr;
      const double pr3 = pr*pr;
      const double pr4 = pr3*pr;

      const double f = -a*ppr + (pr + pr2 + pr3 - pr4)/pow(1 - pr, 3)
	- b*pr2 + c*pow(pr, d);

      const double dfdpr= (1 + 4*pr + 4*pr2 - 4*pr3 + pr4) /
	pow(1 - pr, 4) - 2*b*pr + c*d*pow(pr, d-1);

      prf = pr - f/dfdpr;
      prprev = prf;
  }
                
  pr = prf;

  const double pr2 = pr*pr;
  const double pr3 = pr*pr;
  const double pr4 = pr3*pr;

  const double pr_x_4 = 4*pr;

  const double one_pr_4 = pow(1 - pr, 4);

  const double dprdppr = a*one_pr_4 / (1 + pr_x_4 + 4*pr2 - 4*pr3 + pr4 -
				       one_pr_4*(2*b*pr - (c*d*pow(pr, d - 1))));
                
  const double dzdppr = a/pr - (a*ppr/pr2)*dprdppr;
                
  const double cgr = 1/ppr - (1/z.raw())*dzdppr;
            
  const double cg = cgr/ppc.raw();

  return Quantity<psia_1>(cg);
}

inline Quantity<psia_1>
CgMattarBA::impl(const Quantity<PseudoReducedTemperature> & __tpr,
		 const Quantity<PseudoReducedPressure> & __ppr,
		 const Quantity<psia> & ppc,
		 const Quantity<ZFactor> & z) const
{
  const double tpr = __tpr.raw();
  const double ppr = __ppr.raw();

  constexpr double a1 = 0.31506237;
  constexpr double a2 = -1.0467099;
  constexpr double a3 = -0.57832729;
  constexpr double a4 = 0.53530771;
  constexpr double a5 = -0.61232032;
  constexpr double a6 = -0.10488813;
  constexpr double a7 = 0.68157001;
  constexpr double a8 = 0.68446549;
                
  const double pr = 0.27*ppr/(z.raw() * tpr);
  const double pr2 = pr*pr;
  const double pr3 = pr2*pr;
  const double pr4 = pr3*pr;

  const double tpr2 = tpr*tpr;
  const double tpr3 = tpr2*tpr;

  const double a8_x_pr2 = a8*pr2;
  const double a8_x_pr2_2 = a8_x_pr2*a8_x_pr2;


  const double dzdpr = a1 + a2/tpr + a3/tpr3 + 2*(a4 + a5/tpr)*pr
    + 5*a5*a6*pr4/tpr +
    (2*a7*pr/tpr3)*(1 + a8_x_pr2 - a8_x_pr2_2)*exp(-a8_x_pr2);

  const double z2 = z.raw()*z.raw();
  const double cgr = 1/ppr - (0.27/(z2*tpr))*(dzdpr/
					      (1 + ((pr/z.raw()) * dzdpr)));
                
  const double cg =  cgr/ppc.raw();

  return Quantity<psia_1>(cg);
}


inline Quantity<psia_1>
CgGopal::impl(const Quantity<PseudoReducedTemperature> & __tpr,
	      const Quantity<PseudoReducedPressure> & __ppr,
	      const Quantity<psia> & ppc,
	      const Quantity<ZFactor> & z) const
{
  const double tpr = __tpr.raw();
  const double ppr = __ppr.raw();

  double dzdppr = 0;
  if (0.2 <= ppr and ppr <= 1.2)
    {
      if (1.05 <= tpr and tpr <= 1.2)
	dzdppr = 1.6643 * tpr - 2.2114;
      else if (1.2 < tpr and tpr <= 1.4)
	dzdppr = 0.0522 * tpr - 0.8511;
      else if (1.4 < tpr and tpr <= 2.0)
	dzdppr = 0.1391 * tpr - 0.2988;
      else // 2.0 < Tpr <= 3.0
	dzdppr = 0.0295 * tpr - 0.0825;
    }                   
  else if (1.2 < ppr and ppr <= 2.8)
    {
      if (1.05 <= tpr and tpr <= 1.2)
	dzdppr = -1.3570 * tpr + 1.4942;
      else if (1.2 < tpr and tpr <= 1.4)
	dzdppr = 0.1717 * tpr - 0.3232;
      else if (1.4 < tpr and tpr <= 2.0)
	dzdppr = 0.0984 * tpr - 0.2053;            
      else // 2.0 < Tpr <= 3.0
	dzdppr = 0.0211 * tpr - 0.0527;
    }
  else if (2.8 < ppr and ppr <= 5.4)
    {
      if (1.05 <= tpr and tpr <= 1.2)
	dzdppr = -0.3278 * tpr + 0.4752;
      else if (1.2 < tpr and tpr <= 1.4)
	dzdppr = -0.2521 * tpr + 0.3871;
      else if (1.4 < tpr and tpr <= 2.0)
	dzdppr = -0.0284 * tpr + 0.0625;
      else // 2.0 < Tpr <= 3.0
	dzdppr = 0.0041 * tpr + 0.0039;
    }
  else // 5.4 < Ppr <= 15
    dzdppr = pow(0.711 + 3.66 * tpr, -1.4667);
                
  const double cgr = (1/ppr) - (1/z.raw()) * dzdppr;
                
  const double cg = cgr/ppc.raw();

  return Quantity<psia_1>(cg);
}


inline Quantity<psia_1>
CgBrillBeggs::impl(const Quantity<PseudoReducedTemperature> & __tpr,
		   const Quantity<PseudoReducedPressure> & __ppr,
		   const Quantity<psia> & ppc,
		   const Quantity<ZFactor> & z) const
{
  const double tpr = __tpr.raw();
  const double ppr = __ppr.raw();

  const double a = 1.39 * sqrt(tpr - 0.92) - 0.36*tpr - 0.1;
  const double b = (0.62 - 0.23*tpr)*ppr + 
    (0.066/(tpr - 0.86) - 0.037)*ppr*ppr +
    (0.32/pow(10, 9*(tpr - 1)))*pow(ppr, 6);

  const double c = 0.132 - 0.32*log10(tpr);
  const double d = pow(10, 0.3106 - 0.49*tpr + 0.1824*tpr*tpr);

  double dzdppr = c*d*pow(ppr, d - 1);

  if (b <= 690)
    // The fraction at the left side of the partial derivative of the
    // Z-factor equation with respect to Ppr becomes zero with very
    // high numbers in the denominator (i.e. exp(B) > 4e+299) 
    dzdppr += (1 - a)/(((0.62 - 0.23*tpr) +
			(((0.132/(tpr - 0.86)) - 0.074) * ppr) + 
			((1.92/   pow(10, 9*(tpr - 1))) * pow(ppr, 5)))*exp(b));

  const double cgr = (1/ppr) - ((1/z.raw()) * dzdppr);
                
  const double cg = cgr/ppc.raw();

  return Quantity<psia_1>(cg);
}


inline Quantity<psia_1>
CgPapay::impl(const Quantity<PseudoReducedTemperature> & __tpr,
	      const Quantity<PseudoReducedPressure> & __ppr,
	      const Quantity<psia> & ppc,
	      const Quantity<ZFactor> & z) const
{
  const double tpr = __tpr.raw();
  const double ppr = __ppr.raw();

  const double dzdppr = -3.52/pow(10, 0.9813*tpr) +
    0.548*ppr/pow(10, 0.8157*tpr);

  const double cgr = 1/ppr - 1/z.raw()*dzdppr;
                
  const double cg = cgr/ppc.raw();

  return Quantity<psia_1>(cg);
}



# endif
