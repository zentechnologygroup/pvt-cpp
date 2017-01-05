# ifndef GAS_COMPRESSIBILITY_IMPL_H
# define GAS_COMPRESSIBILITY_IMPL_H

# include <correlations/correlation-utils.H>

# include <correlations/gas-compressibility.H>

inline double
ZfactorSarem::impl(const double & tpr,
		   const double & ppr) const
{
  const double x = (2*ppr - 15)/14.8;
  const double y = (2*tpr - 4)/1.9;

  const double x2 = x*x;
  const double x3 = x2*x;
  const double x4 = x3*x;
  const double x5 = x4*x;

  const double y2 = y*y;
  const double y3 = y2*y;
  const double y4 = y3*y;
  const double y5 = y4*y;
            
  // Especificacion de los polinomios de Legendre en funcion de Ppr y
  // Tpr implicitos en x y y  

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

  return z;
}


inline double
ZfactorHallYarborough::impl(const double & tpr,
			    const double & ppr) const
{
  const double tpr_1 = 1/tpr;
  const double tpr_1_2 = tpr_1*tpr_1;
  const double tpr_1_3 = tpr_1_2*tpr_1;
  const double tpr_1_1 = 1 - tpr_1;
  
  const double a = 0.06125*tpr_1*exp(-1.2*tpr_1_1*tpr_1_1);
  const double b = 14.76*tpr_1 - 9.76*tpr_1_2 + 4.58*tpr_1_3;
  const double c = 90.7*tpr_1 - 242.2*tpr_1_2 + 42.4*tpr_1_3;
  const double d = 2.18 + 2.82*tpr_1;

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
      
      const double f = - a*ppr +
	(pr + pr2 + pr3 - pr4) / pow(1 - pr, 3) - b*pr2 + c*pow(pr, d);
      const double dfdpr= (1 + 4*pr + 4*pr2 - 4*pr3 + pr4) / pow(1 - pr, 4) -
	2*b*pr + c*d*pow(pr, d - 1);
      prf = pr - f/dfdpr;
      prprev = prf;
    }       

  pr = prf;
            
  const double z = (0.06125*ppr*tpr_1*exp(-1.2 * tpr_1_1*tpr_1_1)) / pr;

  return z;
}

inline double
ZfactorDranchukPR::impl(const double & tpr,
			const double & ppr) const
{
  constexpr double a1 = 0.31506237;
  constexpr double a2 = -1.0467099;
  constexpr double a3 = -0.57832729;
  constexpr double a4 = 0.53530771;
  constexpr double a5 = -0.61232032;
  constexpr double a6 = -0.10488813;
  constexpr double a7 = 0.68157001;
  constexpr double a8 = 0.68446549;

  const double tpr2 = tpr*tpr;
  const double tpr3 = tpr2*tpr;

      // pr: Densidad reducida definida por el autor del metodo
  constexpr double epsilon = 1.0e-8;
  double z = 0.5;
  double zprev = 0.6;
  double zf = 0;
            
      // iteracion por metodo Newton-Raphson
  while (fabs(zprev - z) > epsilon)
    {
      z = zprev;
      const double pr = 0.27*ppr/(z * tpr);

      const double pr2 = pr*pr;
      const double pr3 = pr2*pr;
      const double pr4 = pr3*pr;
      const double pr5 = pr4*pr;
            
      const double f = z - (1 + (a1 + a2/tpr + a3/tpr3)*pr +
			    (a4 + a5/tpr)*pr2 + ((a5*a6*pr5)/tpr) +
			    a7*(1 + a8*pr2) * (pr2/tpr3) * exp(-a8*pr2));

      const double a8_x_pr2 = a8 * pr2;
      const double a8_x_pr2_2 = a8_x_pr2*a8_x_pr2;
      const double dfdz = 1 + (a1 + a2/tpr + a3/tpr3)*(pr/z) +
	2*(a4 + a5/tpr)*pr2/z + (5*a5*a6*pr5)/(z * tpr) +
	(2*a7*pr2)/(z * tpr3)*(1 + a8_x_pr2 - a8_x_pr2_2)*exp(-a8_x_pr2);

      zf = z - f/dfdz;
      zprev = zf;
    }

  return zf;
}

inline double
ZfactorDranchukAK::impl(const double & tpr,
			const double & ppr) const
{
  //todo son dos conjuntos d erangos
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

  const double tpr2 = tpr*tpr;
  const double tpr3 = tpr2*tpr;
  const double tpr4 = tpr3*tpr;
  const double tpr5 = tpr4*tpr;

  while (fabs(zprev - z) > epsilon)
    {
      z = zprev;
      const double pr = 0.27*ppr/(z * tpr);
      const double pr2 = pr*pr;
      const double pr3 = pr2*pr;
      const double pr4 = pr3*pr;
      const double pr5 = pr4*pr;

      const double f = z - (1 + (a1 + a2/tpr + a3/tpr3 + a4/tpr4 + a5/tpr5)*pr +
			    (a6 + a7/tpr + a8/tpr2)*pr2 -
			    a9*(a7/tpr + a8/tpr2)*pr5 +
			    a10*(1 + a11*pr2)*(pr2/tpr3)*exp(-a11*pr2));

      const double a11_x_pr2 = a11*pr2;
      const double a11_x_pr2_2 = a11_x_pr2*a11_x_pr2;

      const double dfdz = 1 +
	((a1 + a2/tpr + a3/tpr3 + a4/tpr4 + a5/tpr5)*(pr/z)) +
	(2*(a6 + a7/tpr + a8/tpr2)*(pr2/z)) -
	(5*a9*(a7/tpr + a8/tpr2)*pr5/z) +
	(((2*a10*pr2)/(z* tpr3))*(1 + a11_x_pr2 - a11_x_pr2_2)*exp(-a11_x_pr2));

      zf = z - f/dfdz;
      zprev = zf;
    }     

  return zf;
}

inline double
ZfactorGopal::impl(const double & tpr,
		   const double & ppr) const
{
  double z = 0;
  if (0.2 <= ppr and ppr <= 1.2)
    {
      if (1.05 <= tpr and tpr <= 1.2)
	z = ppr*(1.6643*tpr - 2.2114) - 0.3647*tpr + 1.4385;
      else if (tpr > 1.2 and tpr <= 1.4)
	z = ppr*(0.0522*tpr - 0.8511) - 0.0364*tpr + 1.0490;
      else if (tpr > 1.4 and tpr <= 2.0)
	z = ppr*(0.1391*tpr - 0.2988) + 0.0007*tpr + 0.9969;
      else // 2.0 < Tpr <= 3.0
	z = ppr*(0.0295*tpr - 0.0825) + 0.0009*tpr + 0.9967;
    }
  else if (1.2 < ppr and ppr <= 2.8)
    {
      if (1.05 <= tpr and tpr <= 1.2)
	z = ppr*(-1.3570*tpr + 1.4942) + 4.6315*tpr - 4.7009;
      else if (tpr > 1.2 and tpr <= 1.4)
	z = ppr*(0.1717*tpr - 0.3232) + 0.5869*tpr + 0.1229;
      else if (tpr > 1.4 and tpr <= 2.0)
	z = ppr*(0.0984*tpr - 0.2053) + 0.0621*tpr + 0.8580;
      else // 2.0 < Tpr <= 3.0
	z = ppr*(0.0211*tpr - 0.0527) + 0.0127*tpr + 0.9549;
    }
  else if (ppr > 2.8 and ppr <= 5.4)
    {
      if (1.05 <= tpr and tpr <= 1.2)
	z = ppr*(-0.3278*tpr + 0.4752) + 1.8223*tpr - 1.9036;
      else if (tpr > 1.2 and tpr <= 1.4)
	z = ppr*(-0.2521*tpr + 0.3871) + 1.6087*tpr - 1.6635;
      else if (tpr > 1.4 and tpr <= 2.0)
	z = ppr*(-0.0284*tpr + 0.0625) + 0.4714*tpr - 0.0011;
      else // 2.0 < Tpr <= 3.0:
	z = ppr*(0.0041*tpr + 0.0039) + 0.0607*tpr + 0.7927;
    }
  else // 5.4 < ppr <= 15
    z = ppr*pow(0.711 + 3.66*tpr + 0.0039, -1.4667) -
      1.637/(0.319*tpr + 0.522) + 2.071;
                            
  return z;
}


inline double
ZfactorBrillBeggs::impl(const double & tpr,
			const double & ppr) const
{
  const double a = 1.39*sqrt(tpr - 0.92) - 0.36*tpr - 0.10;

  const double ppr2 = ppr*ppr;
  const double b = (0.62 - 0.23*tpr)*ppr +
    (0.066/(tpr - 0.86) - 0.037)*ppr2 +
    (0.32/pow(10, 9*(tpr - 1)))*pow(ppr, 6);

  const double c = 0.132 - 0.32*log10(tpr);

  const double tpr2 = tpr*tpr;
  const double d = pow(10, 0.3106 - 0.49 * tpr + 0.1824 * tpr2);
            
  double z = 0;
  if (b > 700)
    z = a + c*pow(ppr, d); //The fraction (1 - A)/exp(B) becomes
				 //zero with very high numbers in the
				 //denominator (i.e. exp(B) > 1e+304) 
  else
    z = a + ((1 - a)/exp(b)) + c*pow(ppr, d);

  return z;
}

inline double
ZfactorPapay::impl(const double & tpr,
		   const double & ppr) const
{
  const double ppr2 = ppr*ppr;
  const double z = 1 - 3.52*ppr/pow(10, 0.9813*tpr) +
    0.274*ppr2/pow(10, 0.8157*tpr);

  return z;
}




# endif // GAS_COMPRESSIBILITY_IMPL_H
