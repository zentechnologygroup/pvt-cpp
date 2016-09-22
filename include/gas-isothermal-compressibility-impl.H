# ifndef GAS_ISOTHERMAL_COMPRESSIBILITY_IMPL_H
# define GAS_ISOTHERMAL_COMPRESSIBILITY_IMPL_H

# include <gas-isothermal-compressibility.H>
# include <correlation-utils.H>

inline Quantity<psia_1>
CgSarem::impl(const Quantity<Rankine> & t,
	      const Quantity<psia> & p,
	      const Quantity<Rankine> & tpc,
	      const Quantity<psia> & ppc,
	      const Quantity<ZFactor> & z) const
{
  const double tsr = t.raw()/tpc.raw();
  check_range("CgSarem", "t/tsc", tsr, 1.05, 2.95);

  const double psr = p.raw()/ppc.raw();
  check_range("CgSarem", "p/psc", psr, 0.1, 14.9);

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
CgHallYarborough::impl(const Quantity<Rankine> & t,
		       const Quantity<psia> & p,
		       const Quantity<Rankine> & tpc,
		       const Quantity<psia> & ppc,
		       const Quantity<ZFactor> & z) const
{
  const double tpr = t.raw()/tpc.raw();
  check_range("CgSarem", "t/tsc", tpr, 1.2, 3);

  const double ppr = p.raw()/ppc.raw();
  check_range("CgSarem", "p/psc", ppr, 0.1, 24);

  const double tpr_1 = 1/tpr;
  const double tpr_1_2 = tpr_1*tpr_1;
  const double tpr_1_3 = tpr_1_2*tpr_1;

  const double a = 0.06125*tpr_1 * exp(-1.2*tpr_1_2*tpr_1_2);

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

      const double f = -a*ppr + (pr + pr2 + pr3 - pr4)/pow(1 - pr, 3) - b*pr2 +
	c*pow(pr, d);

      const double pr_x_4 = 4*pr;
      const double pr_x_4_2 = pr_x_4*pr_x_4;
      const double pr_x_4_3 = pr_x_4_2*pr_x_4;
      const double pr_x_4_4 = pr_x_4_3*pr_x_4;
      
      const double dfdpr = (1 + pr_x_4 + pr_x_4_2 - pr_x_4_3 + pr_x_4_4) /
	pow(1 - pr, 4) - 2*b*pr + c*d* pow(pr, d-1);
      const double prf = pr - f/dfdpr;
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


# endif
