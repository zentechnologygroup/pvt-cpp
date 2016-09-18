# ifndef GAS_COMPRESSIBILITY_IMPL_H
# define GAS_COMPRESSIBILITY_IMPL_H

# include <gas-compressibility.H>


inline Quantity<ZFactor>
ZFactorSarem::impl(const Quantity<Rankine> & t,
		   const Quantity<psia> & p,
		   const Quantity<Rankine> & tsc,
		   const Quantity<psia> & psc) const
{
  const double tsr = t.raw()/tsc.raw();
  if (tsr < 1.05 or tsr > 2.95)
    {
      ostringstream s;
      s << "Ratio t/tsc = " << t << " / " << tsc << " = " << tsr
	<< " is not inside the range [1.05, 2.95]";
      throw domain_error(s.str());
    }

  const double psr = p.raw()/psc.raw();
  if (psr < 0.1 or psr > 14.9)
    {
      ostringstream s;
      s << "Ratio p/psc = " << p << " / " << psc << " = " << psr
	<< " is not inside the range [0.1, 14.9]";
      throw domain_error(s.str());
    }

  const double x = (2*psr - 15)/14.8;
  const double y = (2*tsr - 4)/1.9;

  const double x2 = x*x;
  const double x3 = x2*x;
  const double x4 = x3*x;
  const double x5 = x4*x;

  const double y2 = y*y;
  const double y3 = y2*y;
  const double y4 = y3*y;
  const double y5 = y4*y;
            
  // Especificacion de los polinomios de Legendre en funcion de Psr y
  // Tsr implicitos en x y y  

  constexpr double p0x = 0.7071068;
  const double p1x = 1.224745*x;
  const double p2x = 0.7905695 * (3*x2 - 1);
  const double p3x = 0.9354145 * (5*x3 - 3*x);
  const double p4x = 0.265165 * (35*x4 - 30*x2 + 3);
  const double p5x = 0.293151 * (63*x5 - 70*x3 + 15*x);
            
  constexpr double p0y = 0.7071068;
  const double p1y = 1.224745*y;
  const double p2y = 0.7905695 * (3*y2 - 1);
  const double p3y = 0.9354145 * (5*y3 - 3*y);
  const double p4y = 0.265165 * (35*y4 - 30*y2 + 3);
  const double p5y = 0.293151 * (63*y5 - 70*y3 + 15*y);
            
  const double z = 2.1433504*p0x*p0y + 0.0831762*p0x*p1y + -0.0214670*p0x*p2y +
    -0.0008714*p0x*p3y + 0.0042846*p0x*p4y + -0.0016595*p0x*p5y +
    0.3312352*p1x*p0y + -0.1340361*p1x*p1y + 0.0668810*p1x*p2y +
    -0.0271743*p1x*p3y + 0.0088512*p1x*p4y + -0.002152*p1x*p5y +
    0.1057287*p2x*p0y + -0.0503937*p2x*p1y + 0.0050925*p2x*p2y +
    0.0105513*p2x*p3y + -0.0073182*p2x*p4y + 0.0026960*p2x*p5y +
    0.0521840*p3x*p0y + 0.0443121*p3x*p1y + -0.0193294*p3x*p2y +
    0.0058973*p3x*p3y + 0.0015367*p3x*p4y + -0.0028327*p3x*p5y +
    0.0197040*p4x*p0y + -0.0263834*p4x*p1y + 0.019262*p4x*p2y +
    -0.0115354*p4x*p3y + 0.0042910*p4x*p4y + -0.0081303*p4x*p5y +
    0.0053096*p5x*p0y + 0.0089178*p5x*p1y + -0.0108948*p5x*p2y +
    0.0095594*p5x*p3y + -0.0060114*p5x*p4y + 0.0031175*p5x*p5y;

  return Quantity<ZFactor>(z);
}


inline Quantity<ZFactor>
ZFactorHallYarborough::impl(const Quantity<Rankine> & t,
			    const Quantity<psia> & p,
			    const Quantity<Rankine> & tsc,
			    const Quantity<psia> & psc) const
{
  const double tsr = t.raw()/tsc.raw();
  if (tsr < 1.2 or tsr > 3)
    {
      ostringstream s;
      s << "Ratio t/tsc = " << t << " / " << tsc << " = " << tsr
	<< " is not inside the range [1.2, 3]";
      throw domain_error(s.str());
    }

  const double psr = p.raw()/psc.raw();
  if (psr < 0.1 or psr > 24)
    {
      ostringstream s;
      s << "Ratio p/psc = " << p << " / " << psc << " = " << psr
	<< " is not inside the range [0.1, 24]";
      throw domain_error(s.str());
    }

  const double tsr_1 = 1/tsr;
  const double tsr_1_2 = tsr_1*tsr_1;
  const double tsr_1_3 = tsr_1_2*tsr_1;
  const double tsr_1_1 = 1 - tsr_1;
  const double a = 0.06125*tsr_1*exp(-1.2*tsr_1_1*tsr_1_1);
  const double b = 14.76*tsr_1 - 9.76*tsr_1_2 + 4.58*tsr_1_3;
  const double c = 90.7*tsr_1 - 242.2*tsr_1_2 + 42.4*tsr_1_3;
  const double d = 2.18 + 2.82*tsr_1;

  // pr: Densidad reducida definida por el autor del metodo
  constexpr double epsilon = 1.0e-10;
  double pr = 0, prf = 0;
  double prprev = 0.00001;
            
  // iteracion por metodo Newton-Raphson
  while (fabs(prprev - pr) > epsilon)
    {
      pr = prprev;
      const double pr2 = pr*pr;
      const double pr3 = pr2*pr;
      const double pr4 = pr3*pr;
      
      const double f = -a*psr + (pr + pr2 + pr3 - pr4)/pow(1 - pr, 3) - b*pr2 +
	c*pow(pr, d);

      const double pr_x_4 = 4*pr;
      const double pr_x_4_2 = pr_x_4*pr_x_4;
      const double pr_x_4_3 = pr_x_4_2*pr_x_4;
      const double pr_x_4_4 = pr_x_4_3*pr_x_4;

      const double dfdpr = (1 + pr_x_4 + pr_x_4_2 - pr_x_4_3 + pr_x_4_4) /
	pow(1 - pr, 4) - 2 * b*pr + c*d* pow(pr, d-1);

      prf = pr - f/dfdpr;

      prprev = prf;
    }       

  pr = prf;
            
  const double z = (0.06125*psr * tsr_1_1 * exp(-1.2 * tsr_1_1*tsr_1_1)) / pr;

  return Quantity<ZFactor>(z);  
}


# endif // GAS_COMPRESSIBILITY_IMPL_H
