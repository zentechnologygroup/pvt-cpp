# ifndef GAS_COMPRESSIBILITY_IMPL_H
# define GAS_COMPRESSIBILITY_IMPL_H

# include <gas-compressibility.H>

static void check_range(const string & caller, const string & op_msg,
			double val, double min, double max)
{
  if (not (val < min or val >max))
    return;

  ostringstream s;
  s << "In " << caller << ": ratio " << op_msg << " = " << val
    << " is not inside the interval [" << min << ", " << max << "]";
  throw domain_error(s.str());
}

inline Quantity<ZFactor>
ZFactorSarem::impl(const Quantity<Rankine> & t,
		   const Quantity<psia> & p,
		   const Quantity<Rankine> & tsc,
		   const Quantity<psia> & psc) const
{
  const double tsr = t.raw()/tsc.raw();
  check_range("ZFactorSarem::impl", "t/tsc", tsr, 1.05, 2.95);

  const double psr = p.raw()/psc.raw();
  check_range("ZFactorSarem::impl", "p/psc", psr, 0.1, 14.9);

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
  check_range("ZFactorHallYarborough", "t/tsc", tsr, 1.2, 3);

  const double psr = p.raw()/psc.raw();
  check_range("ZFactorHallYarborough", "p/psc", psr, 0.1, 24);

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
	pow(1 - pr, 4) - 2*b*pr + c*d*pow(pr, d-1);

      prf = pr - f/dfdpr;

      prprev = prf;
    }       

  pr = prf;
            
  const double z = (0.06125 * psr * tsr_1 * exp(-1.2 * tsr_1_1*tsr_1_1)) / pr;

  return Quantity<ZFactor>(z);  
}

inline Quantity<ZFactor>
ZFactorDranchukPR::impl(const Quantity<Rankine> & t,
			const Quantity<psia> & p,
			const Quantity<Rankine> & tsc,
			const Quantity<psia> & psc) const
{
  const double tsr = t.raw()/tsc.raw();
  check_range("ZFactorDranchukPR", "t/tsc", tsr, 1.05, 3);

  const double psr = p.raw()/psc.raw();
  check_range("ZFactorDranchukPR", "p/psc", psr, 0.2, 30);

  constexpr double a1 = 0.31506237;
  constexpr double a2 = -1.0467099;
  constexpr double a3 = -0.57832729;
  constexpr double a4 = 0.53530771;
  constexpr double a5 = -0.61232032;
  constexpr double a6 = -0.10488813;
  constexpr double a7 = 0.68157001;
  constexpr double a8 = 0.68446549;

  const double tsr2 = tsr*tsr;
  const double tsr3 = tsr2*tsr;

      // pr: Densidad reducida definida por el autor del metodo
  constexpr double epsilon = 1.0e-8;
  double z = 0.5;
  double zprev = 0.6;
  double zf = 0;
            
      // iteracion por metodo Newton-Raphson
  while (fabs(zprev - z) > epsilon)
    {
      z = zprev;
      const double pr = 0.27*psr/(z * tsr);

      const double pr2 = pr*pr;
      const double pr3 = pr2*pr;
      const double pr4 = pr3*pr;
      const double pr5 = pr4*pr;
            
      const double f = z - (1 + (a1 + a2/tsr + a3/tsr3)*pr + (a4 + a5/tsr)*pr2 +
			    ((a5*a6*pr5)/tsr) +
			    a7*(1 + a8*pr2) * (pr2/tsr3) * exp(-a8*pr2));

      const double a8_x_pr2 = a8 * pr2;
      const double a8_x_pr2_2 = a8_x_pr2*a8_x_pr2;
      const double dfdz = 1 + (a1 + a2/tsr + a3/tsr3)*(pr/z) +
	2*(a4 + a5/tsr)*pr2/z + (5*a5*a6*pr5)/(z * tsr) +
	(2*a7*pr2)/(z * tsr3)*(1 + a8_x_pr2 - a8_x_pr2_2)*exp(-a8_x_pr2);

      zf = z - f/dfdz;
      zprev = zf;
    }

  return Quantity<ZFactor>(zf);
}

inline Quantity<ZFactor>
ZFactorDranchukAK::impl(const Quantity<Rankine> & t,
			const Quantity<psia> & p,
			const Quantity<Rankine> & tsc,
			const Quantity<psia> & psc) const
{
  const double tsr = t.raw()/tsc.raw();
  check_range("ZFactorDranchukAK", "t/tsc", tsr, 1, 3);

  const double psr = p.raw()/psc.raw();
  check_range("ZFactorDranchukAK", "p/psc", psr, 0.2, 30);
        
  constexpr double a1 = 0.3265;
  constexpr double a2 = -1.07;
  constexpr double a3 = -0.5339;
  constexpr double a4 = 0.01569;
  constexpr double a5 = -0.05165;
  constexpr double a6 = 0.5475;
  constexpr double a7 = -0.7361;
  constexpr double a8 = 0.1844;
  constexpr double a9 = 0.1056;
  constexpr double a10 = 0.6134;
  constexpr double a11 = 0.721;
            
  // pr: Densidad reducida definida por el autor del metodo
  constexpr double epsilon = 1.0e-8;
  double z = 0.5;
  double zprev = 0.6;
  double zf;

  const double tsr2 = tsr*tsr;
  const double tsr3 = tsr2*tsr;
  const double tsr4 = tsr3*tsr;
  const double tsr5 = tsr4*tsr;

  while (fabs(zprev - z) > epsilon)
    {
      z = zprev;
      const double pr = 0.27*psr/(z * tsr);
      const double pr2 = pr*pr;
      const double pr3 = pr2*pr;
      const double pr4 = pr3*pr;
      const double pr5 = pr4*pr;

      const double f = z - (1 + (a1 + a2/tsr + a3/tsr3 + a4/tsr4 + a5/tsr5)*pr +
			    (a6 + a7/tsr + a8/tsr2)*pr2 -
			    a9*(a7/tsr + a8/tsr2)*pr5 +
			    a10*(1 + a11*pr2)*(pr2/tsr3)*exp(-a11*pr2));

      const double a11_x_pr2 = a11*pr2;
      const double a11_x_pr2_2 = a11_x_pr2*a11_x_pr2;

      const double dfdz = 1 +
	((a1 + a2/tsr + a3/tsr3 + a4/tsr4 + a5/tsr5)*(pr/z)) +
	(2*(a6 + a7/tsr + a8/tsr2)*(pr2/z)) - (5*a9*(a7/tsr + a8/tsr2)*pr5/z) +
	(((2*a10*pr2)/(z* tsr3))*(1 + a11_x_pr2 - a11_x_pr2_2)*exp(-a11_x_pr2));

      zf = z - f/dfdz;
      zprev = zf;
    }     

  return Quantity<ZFactor>(zf);
}

inline Quantity<ZFactor>
ZFactorGopal::impl(const Quantity<Rankine> & t,
		   const Quantity<psia> & p,
		   const Quantity<Rankine> & tsc,
		   const Quantity<psia> & psc) const
{
  const double tsr = t.raw()/tsc.raw();
  check_range("ZFactorGopal", "t/tsc", tsr, 1.05, 3);

  const double psr = p.raw()/psc.raw();
  check_range("ZFactorGopal", "p/psc", psr, 0.2, 15);

  double z = 0;
  if (psr > 0.2 and psr <= 1.2)
    {
      if (tsr > 1.05 and tsr <= 1.2)
	z = psr*(1.6643*tsr - 2.2114) - 0.3647*tsr + 1.4385;
      else if (tsr > 1.2 and tsr <= 1.4)
	z = psr*(0.0522*tsr - 0.8511) - 0.0364*tsr + 1.0490;
      else if (tsr > 1.4 and tsr <= 2.0)
	z = psr*(0.1391*tsr - 0.2988) + 0.0007*tsr + 0.9969;
      else // 2.0 < Tsr <= 3.0
	z = psr*(0.0295*tsr - 0.0825) + 0.0009*tsr + 0.9967;
    }
  else if (psr > 1.2 and psr <= 2.8)
    {
      if (tsr > 1.05 and tsr <= 1.2)
	z = psr*(-1.3570*tsr + 1.4942) + 4.6315*tsr - 4.7009;
      else if (tsr > 1.2 and tsr <= 1.4)
	z = psr*(0.1717*tsr - 0.3232) + 0.5869*tsr + 0.1229;
      else if (tsr > 1.4 and tsr <= 2.0)
	z = psr*(0.0984*tsr - 0.2053) + 0.0621*tsr + 0.8580;
      else // 2.0 < Tsr <= 3.0
	z = psr*(0.0211*tsr - 0.0527) + 0.0127*tsr + 0.9549;
    }
  else if (psr > 2.8 and psr <= 5.4)
    {
      if (tsr > 1.05 and tsr <= 1.2)
	z = psr*(-0.3278*tsr + 0.4752) + 1.8223*tsr - 1.9036;
      else if (tsr > 1.2 and tsr <= 1.4)
	z = psr*(-0.2521*tsr + 0.3871) + 1.6087*tsr - 1.6635;
      else if (tsr > 1.4 and tsr <= 2.0)
	z = psr*(-0.0284*tsr + 0.0625) + 1.4714*tsr - 0.0011;
      else // 2.0 < Tsr <= 3.0:
	z = psr*(0.0041*tsr + 0.0039) + 0.0607*tsr + 0.7927;
    }
  else // 5.4 < psr <= 15
    z = psr*pow(0.711 + 3.66*tsr + 0.0039, -1.4667) -
      1.637/(0.319*tsr + 0.522) + 2.071;
                            
  return Quantity<ZFactor>(z);
}


inline Quantity<ZFactor>
ZFactorBrillBeggs::impl(const Quantity<Rankine> & t,
			const Quantity<psia> & p,
			const Quantity<Rankine> & tsc,
			const Quantity<psia> & psc) const
{
  const double tsr = t.raw()/tsc.raw();
  check_range("ZFactorBrillBeggs", "t/tsc", tsr, 1.2, 2.4);

  const double psr = p.raw()/psc.raw();
  check_range("ZFactorBrillBeggs", "p/psc", psr, 0, 13);

  const double a = 1.39*sqrt(tsr - 0.92) - 0.36*tsr - 0.10;

  const double b = (0.62 - 0.23*tsr)*psr + ((0.066/(tsr - 0.86)) - 0.037) * (Psr ** 2) + (0.32/(10 ** (9 * (Tsr - 1)))) * (Psr ** 6)
            C = 0.132 - 0.32 * log10(Tsr)
            D = 10 ** (0.3106 - 0.49 * Tsr + 0.1824 * (Tsr ** 2))
            
            mathDomain = B  #Se define el dominio matematico de la funcion de Z, sensible al argumento B de la funcion exponencial         
                     
            if mathDomain > 700:
                Z = A + ((0)) + C * (Psr ** D) #el cero se corresponde con numeros muy altos en el denominador i.e >1e+300
    
            else:
                Z = A + ((1 - A)/exp(B)) + C * (Psr ** D)
            
}

# endif // GAS_COMPRESSIBILITY_IMPL_H
