# ifndef SATURATED_OIL_FORMATION_VOLUME_FACTOR_IMPL_H
# define SATURATED_OIL_FORMATION_VOLUME_FACTOR_IMPL_H

# include <correlations/saturated-oil-formation-volume-factor.H>


inline double
BobAlmarhoun::impl(const double & yg,
		   const double & yo,
		   const double & rs,
		   const double & t) noexcept
{
  const double f = pow(rs, 0.74239) * pow(yg, 0.323294) * pow(yo, -1.20204);
  return 0.497069 + 0.000862963*t + 0.00182594*f + 0.00000318099*f*f; 
}

inline double
BobAlShammasi::impl(const double & yg,
		    const double & yo,
		    const double & rs,
		    const double & t) noexcept
{
  const double bo = 1 + 0.000000553*(rs*(t - 60)) +
    0.000181*rs/yo + 0.000449*((t - 60)/yo) + 0.000206*(rs*yg/yo);

  return bo;
}

inline double
BobCegarra::impl(const double & yg,
		         const double & yo,
		         const double & rs,
		         const double & t) noexcept
{
  const double bob = BobAlShammasi::get_instance().impl(yg, yo, rs, t);
  return bob;
}

inline double
BobDeGhetto::impl(const double & yg,
		 const double & api,
		 const double & rs,
		 const double & t,
		 const double & tsep,
		 const double & psep) noexcept

{
  constexpr double c1 = 0.0004677;
  constexpr double c2 = 0.00001751;
  constexpr double c3 = -0.00000001811;
        
  const double ygs = yg * (1. + 0.00005912*api*tsep*log10(psep/114.7));

  return 1 + c1*rs + c2*(t - 60) * (api/ygs) + c3*rs*(t - 60)*(api/ygs);
}

inline double
BobDindorukChristman::impl(const double & yg,
			  const double & api,
			  const double & rs,
			  const double & t,
			  const double & tsep) noexcept
{
  constexpr double a1 = 2.510755;
  constexpr double a2 = -4.852538;
  constexpr double a3 = 1.1835e1;
  constexpr double a4 = 1.365428e5;
  constexpr double a5 = 2.25288;
  constexpr double a6 = 1.00719e1;
  constexpr double a7 = 4.450849e-1;
  constexpr double a8 = 5.352624;
  constexpr double a9 = -6.309052e-1;
  constexpr double a10 = 9.000749e-1;
  constexpr double a11 = 9.871766e-1;
  constexpr double a12 = 7.865146e-4;
  constexpr double a13 = 2.689173e-6;
  constexpr double a14 = 1.100001e-5;
        
  constexpr double b1 = 4.236114474;
  constexpr double b2 = 24.316998249;
  constexpr double b3 = 0.958319868;
  constexpr double b4 = 0.924700438;
  constexpr double b5 = 1.287177430;
  constexpr double b6 = 1.353868836;
  constexpr double b7 = 12.581487761;
  constexpr double b8 = 9.828286832;
        
  const double yo = unit_convert<Api, Sg_do>(api);

  const double t_60 = t - 60;

  const double d = a8 + (2 *pow(rs, a9)/pow(yg, a10))*t_60;
  const double d2 = d*d;
      
  const double a = pow(pow(rs, a1)*pow(yg, a2)/pow(yo, a3) +
		       a4*pow(t_60, a5) + a6*rs, a7) /  d2;
      
  double bo = a11 + a12*a + a13*a*a + a14*t_60*api/yg;
  if (bo > 2)
    {
      const double t_tsep = t - tsep;
      const double bodl = bo;
      const double n = pow(t_tsep, b1)*pow(log10(bodl)*tanh(bodl), b2) +
	b3*pow(bodl - 1, b4);
      const double d = pow(1 + pow(bodl, b5)*pow(t_tsep, b6)*
			   pow(log10(bodl), b7), b8);

      bo = 1 + (n / d);
    }

  return bo;
}

inline double
BobDoklaOsman::impl(const double & yg,
		   const double & yo,
		   const double & rs,
		   const double & t) noexcept
{
  const double m = pow(rs, 0.773572) * pow(yg, 0.40402) * pow(yo, -0.882605);

  return 0.431935e-1 + 0.156667e-2*t + 0.139775e-2*m + 0.380525e-5*m*m;
}

inline double
BobGlaso::impl(const double & yg,
	       const double & yo,
	       const double & rs,
	       const double & t) noexcept
{
  const double yg_yo = yg/yo;

  const double f = rs*pow(yg_yo, 0.526) + 0.968*t;
  const double log_f = log10(f);
  const double log_f_2 = log_f*log_f;
      
  return 1 + pow(10, -6.58511 + 2.91329*log_f - 0.27683*log_f_2);
}

inline double
BobHanafy::impl(const double & rs) noexcept
{
  return 0.0006*rs + 1.079;
}

inline double
BobKartoatmodjo::impl(const double & yg,
		     const double & yo,
		     const double & rs,
		     const double & t,
		     const double & tsep,
		     const double & psep) noexcept
{
  const double api = unit_convert<Sg_do, Api>(yo);

  const double c =
    1 + 0.1595*pow(api, .4078)*pow(tsep, -0.2466)*log10(psep/114.7);
       
  const double ygcorr = c*yg;

  const double f = pow(rs, 0.755)*pow(ygcorr, 0.25)*pow(yo, -1.5) + 0.45*t;

  return 0.98496 + 0.0001 * pow(f, 1.5);
}

inline double
BobStanding::impl(const double & yg,
		  const double & yo,
		  const double & rs,
		  const double & t) noexcept
{
  return 0.972 + 0.000147*pow(rs*sqrt(yg/yo) + 1.25*t, 1.175);
}

inline double
BobLasater::impl(const double & yg,
		 const double & yo,
		 const double & rs,
		 const double & t) noexcept
{
  const double bob = BobStanding::get_instance().impl(yg, yo, rs, t);
  return bob;
}

inline double
BobPetroskyFarshad::impl(const double & yg,
			const double & yo,
			const double & rs,
			const double & t) noexcept
{
  const double f =
    pow(rs, 0.3738)*pow(yg, 0.2914)/pow(yo, 0.6265) + 0.24626*pow(t, 0.5371);

  return 1.0113 + 7.2046e-5*pow(f, 3.0936);
}

inline double
BobTotalCFP::impl(const double & yg,
		 const double & api,
		 const double & rs,
		 const double & t) noexcept
{
  const double t_60 = t - 60;
  const double api_yg = api/yg;

  return 1.022 + 4.857e-4*rs - 2.009e-6*t_60*api_yg + 17.569e-9*rs*t_60*api_yg;
}

inline double
BobVasquezBeggs::impl(const double & yg,
		     const double & api,
		     const double & rs,
		     const double & t,
		     const double & tsep,
		     const double & psep) noexcept
{
  double c1 = 0, c2 = 0, c3 = 0;
  if (api <= 30)
    {
      c1 = 4.677e-4;
      c2 = 1.751e-5;
      c3 = -1.811e-8;
    }
  else
    {      
      c1 = 4.67e-4;
      c2 = 1.1e-5;
      c3 = 1.337e-9;
    }

  const double ygs = yg* (1. + 5.912e-5 * api * tsep * log10(psep/114.7));

  const double t_60 = t - 60;
  const double api_ygs = api/ygs;

  return 1 + c1*rs + c2*t_60 * api_ygs + c3 * rs * t_60 * api_ygs;
}

void BobPerezML::precondition(const Quantity<psia> & p,
			      const Quantity<psia> & pb) const
{
  if (p > pb)
    {
      ostringstream s;
      s << "BobPerezML::impl: pressure " << p << " is greater than pb " << pb;
      ALEPHTHROW(WrongValueSaturatedCondition, s.str());
    }
}

inline double
BobPerezML::impl(const double & yg,
		const double & yo,
		const double & rsb,
		const double & t,
		const double & p,
		const double & pb) noexcept
{
  const double yg_yo = yg/yo;

  const double c = 1e-4 * (rsb*sqrt(yg_yo) + 1.25*t);
  const double bob = 0.974322 + 4.35048*c - 2.85869*c*c;        

  return bob * (1 - (8.801 + 0.1547*t) * 1e-3 * (1 - p/pb));
}

inline double
BobVelardeMcCain::impl(const double & yg,
		      const double & yo,
		      const double & rs,
		      const double & rsb,
		      const double & t,
		      const double & p) noexcept
{
  constexpr double pwater = 62.4; // water specific weight in  lb/ft³ or lb/scf
  const double psto = pwater*yo;

  double a = 0;
  double b = 52.8 - 0.01*rsb; // Initial value pseudoliquid density
  double ppo = 0, ppof = 0;
  while (fabs(a - b) > 0.00001)
    {
      a = b;
      ppo = a;
      const double pa = -49.893 + 85.0149*yg - 3.70373*yg*ppo +
	0.047981*yg*ppo*ppo + 2.98914*ppo- 0.035688*ppo*ppo;
      ppof = (rs*yg + 4600*yo) / (73.71 + rs*yg/pa);
      b = ppof;
    }

  ppo = ppof;

  const double t_60 = t - 60;
  const double p_1000 = p/1000;
  const double p_1000_2 = p_1000*p_1000;
      
  const double pbs = ppo + (0.167 + 16.181*pow(10, -0.0425*ppo)) *
    p_1000 - 0.01*(0.299 + 263*pow(10, -0.0603*ppo))*p_1000_2;
      
  const double poR = pbs - (0.00302 + 1.505 * pow(pbs, -0.951)) *
    pow(t_60, 0.938) + 0.0233*pow(10, -0.0161*pbs)*pow(t_60, 0.475);
            
  const double bo = (psto + 0.01357*rs*yg)/poR;

  return bo;
}

void BobMillanArcia::precondition(const Quantity<psia> & p,
				  const Quantity<psia> & pb) const
{
  if (p > pb)
    {
      ostringstream s;
      s << "BobMillanArcia::impl: pressure " << p << " is greater than pb " << pb;
      ALEPHTHROW(WrongValueSaturatedCondition, s.str());
    }
}

inline double
BobMillanArcia::impl(const double & api,
		    const double & rsb,
		    const double & p,
		    const double & pb) noexcept
{
  const double bob = 1.3537 * pow(rsb, 0.1505) * pow(pb, -0.1239) *
    exp(-0.00405*api);

  return bob * (0.9419 + 0.0608 * p/pb);
}


inline double
BobManucciRosales::impl(const double & api,
			const double & rs,
			const double & t) noexcept
{
  const double x = 3.27e-4*t + 0.00321*api;

  return 0.2378 * pow(rs, 0.221) * pow(10, x);
}

inline double
BobLopezCR::impl(const double & yg,
		 const double & api,
		 const double & rs,
		 const double & t) noexcept
{
  return 0.93176 + 0.000422061 * t + 0.000372363 * rs + 0.0545485 * yg + 0.00159098 * api;
}

inline double
BobSalazar::impl(const double & yg,
		 const double & api,
		 const double & p) noexcept
{
  return 0.99619 + 0.00003 * p + 0.00245 * api + 0.01433 * yg;
}

# endif // SATURATED_OIL_FORMATION_VOLUME_FACTOR_IMPL_H
