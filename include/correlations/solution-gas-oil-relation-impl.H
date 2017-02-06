
# ifndef SOLUTION_GAS_OIL_RELATION_IMPL_H
# define SOLUTION_GAS_OIL_RELATION_IMPL_H

# include <correlations/solution-gas-oil-relation.H>

inline double
RsAlMarhoun::impl(const double & yg,
		   const double & p,
		   const double & yo,
		   const double & t) noexcept
{
  constexpr double c1 = 0.00538088;
  constexpr double c2 = -1.87784;
  constexpr double c3 = 3.1437;
  constexpr double c4 = 1.32657;
  constexpr double c5 = 1.0 / 0.715082;

  return pow(p / (c1 * pow(yg, c2) * pow(yo, c3) * pow(t, c4)), c5);
}


inline double
RsAlShammasi::impl(const double & yg,
		  const double & p,
		  const double & yo,
		  const double & t) noexcept
{
  constexpr double c1 = 5.527215;
  constexpr double c2 = -1.841408;
  constexpr double c3_inv = 1/0.783716;

  return pow(p/(pow(yo, c1)*exp(c2*yo*yg)), c3_inv) / ((t + 460) * yg); 
}

inline double
RsDeGhetto::impl(const double & yg,
		 const double & p,
		 const double & api,
		 const double & t,
		 const double & tsep,
		 const double & psep) noexcept
{
  double rs = 0;
  if (api <= 10)
    rs = yg * pow(p/10.7025*pow(10, 0.0169*api-0.00156*t), 1.1128);
  else
    {
      const double ygcorr = yg*(1 + 0.5912*api*tsep*log10(psep/114.7) * 0.0001);
      rs = ((ygcorr * pow(p, 1.2057))/56.434) * pow(10, 10.9267*api/(t + 460));
    }

  return rs;
}

inline double
RsDindorukChristman::impl(const double & yg,
			  const double & p,
			  const double & api,
			  const double & t) noexcept
{
  constexpr double a1  = 4.86996e-06;
  constexpr double a2  = 5.730982539;
  constexpr double a3  = 0.0099251;
  constexpr double a4  = 1.776179364;
  constexpr double a5  = 44.2500268;
  constexpr double a6  = 2.702889206;
  constexpr double a7  = 0.744335673;
  constexpr double a8  = 3.35975497;
  constexpr double a9  = 28.10133245;
  constexpr double a10 = 1.57905016;
  constexpr double a11 = 0.928131344;

  const double d = a5 + 2*pow(api, a6)/pow(p, a7);
  const double d2 = d*d;

  const double a = (a1*pow(api, a2) + a3*pow(t, a4)) / d2;

  const double rs = pow((p/a8 + a9) * pow(yg, a10) * pow(10, a), a11);

  return double(rs);
}

inline double
RsDoklaOsman::impl(const double & yg,
		   const double & p,
		   const double & yo,
		   const double & t) noexcept
{
  constexpr double k = 0.11956202e-3; // 1/(0.836386 * 10 **4)
  constexpr double d = 1/0.724047;

  return pow(k * p * pow(yg, 1.01049) * pow(yo, -0.107991) * pow(t, 0.952584), d);
}

inline double
RsGlaso::impl(const double & yg,
	      const double & p,
	      const double & api,
	      const double & t) noexcept
{
  constexpr double a = -0.30218;
  constexpr double b = 1.7447;
  constexpr double b2 = b*b; // b**2
  const double c = 1.7669 - log10(p);

  // TODO: considerar resolver la eq 2do grado y decidir por el
  // valor de p. La solución es log10(p)=(17669*a-2500*b^2)/(10000*a)
  double r = 0;
  if (b2 - 4*a*c < 0)
    r =  -b/(2*a);
  else
    r = (-b + sqrt(b2 - 4*a*c)) / (2*a);

  const double f = pow(10, r);
  return yg * pow((f * pow(api, 0.989) ) / ( pow(t, 0.172) ), 1/0.816);
}

inline double
RsHanafy::impl(const double & p) noexcept
{
  return -49.069 + 0.312*p;
}

inline double
RsKartoatmodjoSchmidt::impl(const double & yg,
			    const double & p,
			    const double & api,
			    const double & t,
			    const double & tsep,
			    const double & psep) noexcept
{
  const double c =
    1 + 0.1595 * pow(api, 0.4078) * pow(tsep, -0.2466) * log10(psep/114.7);
  const double ygcorr = c*yg;

  double rs = 0;
  if (api > 30)
    {
      constexpr double c1 = 0.0315;
      constexpr double c2 = 0.7587;
      constexpr double c3 = 1.0937;
      constexpr double c4 = 11.289;
      rs = c1 * pow(ygcorr, c2) * pow(p, c3) * pow(10, c4*api/(t + 460));
    }
  else
    {
      constexpr double c1 = 0.05958;
      constexpr double c2 = 0.7972;
      constexpr double c3 = 1.0014;
      constexpr double c4 = 13.1405;
      rs = c1 * pow(ygcorr, c2) * pow(p, c3) * pow(10, c4*api/(t + 460));
    }

  return rs;
}

inline double
RsLasater::impl(const double & yg,
		const double & p,
		const double & yo,
		const double & t) noexcept
{
  const double api = unit_convert<Sg_do, Api>(yo);

  const double mo =
    api <= 40 ? 630 - 10*api : 73110 * pow(api, -1.562);

  // Calculation of Xpb = 
  double sy = 0;
  const double xpb = p*yg/t; // Bubble point pressure factor
  if (xpb < 3.29)
    {
      constexpr double c1 = 0.359;
      const double     c2 = 1.473 * xpb;
      constexpr double c3 = 0.476;
      sy = c1 * log(c2 + c3);
    }
  else
    {        
      const double     c1 = 0.121 * xpb;
      constexpr double c2 = -0.236;
      constexpr double c3 = 0.281;
      sy = pow(c1 + c2, c3);
    }        

  if (sy >= 1)
    sy = 0.99999999;
        
  const double rs = (132755*yo*sy) / (mo*(1 - sy));
    
  return rs >= 0 ? rs : 0;
}

inline double
RsPetroskyFarshad::impl(const double & yg,
			const double & p,
			const double & api,
			const double & t) noexcept
{
  const double x = 0.0007916 * pow(api, 1.541) - 0.00004561 * pow(t, 1.3911);
        
  return pow((p/112.727 + 12.34) * pow(yg, 0.8439) * pow(10, x), 1.73184);
}

inline double RsStanding::impl(const double & yg,
			       const double & p,
			       const double & api,
			       const double & t) noexcept
{
  return yg*pow((p/18.2 + 1.4)*pow(10, 0.0125*api - 0.00091*t), 1.2048);
}

inline double RsTotalCFP::impl(const double & yg,
			       const double & p,
			       const double & api,
			       const double & t) noexcept
{
  double c1 = 0, c2 = 0, c3 = 0, c4 = 0;
  if (api <= 10)
    {
      c1 = 12.2651;
      c2 = 0.030405;
      c4 = 0.9669;
    }
  else if (api <= 35)
    {
      c1 = 15.0057;
      c2 = 0.0152;
      c3 = 0.0004484;
      c4 = 1.095;
    }
  else // API <= 45:
    {
      c1 = 112.925;
      c2 = 0.0248;
      c3 = -0.001469;
      c4 = 1.129;
    }
        
  return yg * pow((p/c1) * pow(10, c2*api - c3*t), c4);
}

inline double
RsVasquezBeggs::impl(const double & yg,
		     const double & p,
		     const double & api,
		     const double & t,
		     const double & tsep,
		     const double & psep) noexcept
{
  double c1 = 0, c2 = 0, c3 = 0;
  if (api <= 30)
    {
      c1 = 0.0362;
      c2 = 1.0937;
      c3 = 25.724;
    }
  else
    {
      c1 = 0.0178;
      c2 = 1.187;
      c3 = 23.931;
    }

  const double ygs = yg * (1 + 0.00005912*api*tsep*log10(psep/114.7));

  const double rs = c1 * ygs * pow(p, c2) * exp((c3*api) / (t + 460));

  return rs > 0 ? rs : 0;
}

inline double RsVelarde::impl(const double & yg,
					 const double & pb,
					 const double & p,
					 const double & api,
					 const double & t,
					 const double & rsb) noexcept
{
  constexpr double a0 = 9.73e-07;
  constexpr double a1 = 1.672608;
  constexpr double a2 = 0.92987;
  constexpr double a3 = 0.247235;
  constexpr double a4 = 1.056052;
        
  constexpr double b0 = 0.022339;
  constexpr double b1 = -1.00475;
  constexpr double b2 = 0.337711;
  constexpr double b3 = 0.132795;
  constexpr double b4 = 0.302065;
        
  constexpr double c0 = 0.725167;
  constexpr double c1 = -1.48548;
  constexpr double c2 = -0.164741;
  constexpr double c3 = -0.09133;
  constexpr double c4 = 0.047094;

  const double s1 = a0 * pow(yg, a1) * pow(api, a2) * pow(t, a3) * pow(pb, a4);

  const double s2 = b0 * pow(yg, b1) * pow(api, b2) * pow(t, b3) * pow(pb, b4);

  const double s3 = c0 * pow(yg, c1) * pow(api, c2) * pow(t, c3) * pow(pb, c4);
        
  const double pr = p / pb;
        
  const double rsr = s1*pow(pr, s2) + (1 - s1)*pow(pr, s3);
        
  return rsr*rsb;
}

inline double RsCegarra::impl(const double & yg,
					 const double & p,
					 const double & api,
					 const double & t) noexcept
{
  double l1 = 0, l2 = 0, l3 = 0, l4 = 0, l5 = 0;
  if (api < 29.9)
    {
      l1 = 154.158;
      l2 = 0.4577;
      l3 = 0.0006680;
      l4 = 0.000514;
      l5 = 4.70257;
    }
  else 
    {
      l1 = 809.238;
      l2 = 0.32;
      l3 = 0.00061;
      l4 = 0.011;
      l5 = 1.1142;
    }

  return yg * pow((p/l1 + l5) * pow(10, l4*api< - l3*t), 1/l2);
}

inline double RsPerezML::impl(const double & pb,
			      const double & p,
			      const double & rsb) noexcept
{
  return rsb * pow((p/pb), 0.881);
}

inline double
RsMillanArcia::impl(const double & pb,
		    const double & p,
		    const double & rsb) noexcept
{
  return (rsb/1.031) * pow(p/pb, 0.83);
}

inline double
RsManucciRosales::impl(const double & yg,
		       const double & p,
		       const double & api,
		       const double & t) noexcept
{
  const double x = 0.000922*t - 0.0072*api;
            
  return  yg * pow(p/(84.88 * pow(10, x) ), 1/0.53);
}

void RsAbovePb::precondition(const Quantity<psia> & p,
			     const Quantity<psia> & pb) const
{
  if (p <= pb)
    {
      ostringstream s;
      s << "Rsa::impl: pressure" << p << " is less than bubble point pressure "
	<< pb;
      ZENTHROW(WrongValueUndersaturatedCondition, s.str());
    }
}

double
RsAbovePb::impl(const double &,
		const double &,
		const double & rsb) noexcept
{
  return rsb;
}


# endif // SOLUTION_GAS_OIL_RELATION_IMPL_H
