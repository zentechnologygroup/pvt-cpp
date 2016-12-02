# ifndef SATURATED_OIL_FORMATION_VOLUME_FACTOR_IMPL_H
# define SATURATED_OIL_FORMATION_VOLUME_FACTOR_IMPL_H

# include <correlations/saturated-oil-formation-volume-factor.H>


inline Quantity<RB_STB>
BobAlmarhoun::impl(const Quantity<Sgg> & yg,
		   const Quantity<Sg_do> & yo,
		   const Quantity<SCF_STB> & rs,
		   const Quantity<Rankine> & t) const
{
  const double f = pow(rs, 0.74239) * pow(yg, 0.323294) * pow(yo, -1.20204);
  const double bo =
    0.497069 + 0.000862963*t.raw() + 0.00182594*f + 0.00000318099*f*f; 

  return Quantity<RB_STB>(bo);
}

inline Quantity<RB_STB>
BobAlShammasi::impl(const Quantity<Sgg> & yg,
		    const Quantity<Sg_do> & yo,
		    const Quantity<SCF_STB> & rs,
		    const Quantity<Fahrenheit> & t) const
{
  const double raw_t = t.get_value();
  const double raw_yg = yg.get_value();
  const double raw_yo = yo.get_value();

  const double raw_rs = rs.get_value();
  const double bo = 1 + 0.000000553*(raw_rs*(raw_t - 60)) +
    0.000181*raw_rs/raw_yo + 0.000449*((raw_t - 60)/raw_yo) +
    0.000206*(raw_rs*raw_yg/raw_yo);

  return Quantity<RB_STB>(bo);
}

inline Quantity<RB_STB>
BobDeGhetto::impl(const Quantity<Sgg> & yg,
		 const Quantity<Api> & api,
		 const Quantity<SCF_STB> & rs,
		 const Quantity<Fahrenheit> & t,
		 const Quantity<Fahrenheit> & tsep,
		 const Quantity<psia> & psep) const

{
  constexpr double c1 = 0.0004677;
  constexpr double c2 = 0.00001751;
  constexpr double c3 = -0.00000001811;

  const double raw_api = api.get_value();
  const double raw_t = t.get_value();
        
  const double ygs = yg.get_value() *
    (1. + 0.00005912*raw_api*tsep.get_value()*log10(psep.get_value()/114.7));

  const double raw_rs = rs.get_value();
  const double bo = 1 + c1*raw_rs + c2*(raw_t - 60) * (raw_api/ygs) +
    c3*raw_rs*(raw_t - 60)*(raw_api/ygs);

  return Quantity<RB_STB>(bo);
}


inline Quantity<RB_STB>
BobDindorukChristman::impl(const Quantity<Sgg> & yg,
			  const Quantity<Api> & api,
			  const Quantity<SCF_STB> & rs,
			  const Quantity<Fahrenheit> & t,
			  const Quantity<Fahrenheit> & tsep) const
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
        
  const double yo = Quantity<Sg_do>(api).get_value();

  const double raw_t = t.get_value();
  const double raw_yg = yg.get_value();
  const double t_60 = raw_t - 60;

  const double raw_rs = rs.get_value();
  const double d = a8 + (2 *pow(raw_rs, a9)/pow(raw_yg, a10))*t_60;
  const double d2 = d*d;
      
  const double a = pow(pow(raw_rs, a1)*pow(raw_yg, a2)/pow(yo, a3) +
		       a4*pow(t_60, a5) + a6*raw_rs, a7) /  d2;
      
  double bo = a11 + a12*a + a13*a*a + a14*t_60*api.get_value()/raw_yg;
  if (bo > 2)
    {
      const double t_tsep = raw_t - tsep.get_value();
      const double bodl = bo;
      const double n = pow(t_tsep, b1)*pow(log10(bodl)*tanh(bodl), b2) +
	b3*pow(bodl - 1, b4);
      const double d = pow(1 + pow(bodl, b5)*pow(t_tsep, b6)*
			   pow(log10(bodl), b7), b8);

      bo = 1 + (n / d);
    }

  return Quantity<RB_STB>(bo);
}


inline Quantity<RB_STB>
BobDoklaOsman::impl(const Quantity<Sgg> & yg,
		   const Quantity<Sg_do> & yo,
		   const Quantity<SCF_STB> & rs,
		   const Quantity<Rankine> & t) const
{
  const double raw_t = t.get_value();


  const double m = pow(rs, 0.773572) * pow(yg, 0.40402) * pow(yo, -0.882605);
  const double bo =
    0.431935e-1 + 0.156667e-2*raw_t + 0.139775e-2*m + 0.380525e-5*m*m;

  return Quantity<RB_STB>(bo);
}

inline Quantity<RB_STB>
BobGlaso::impl(const Quantity<Sgg> & yg,
	       const Quantity<Sg_do> & yo,
	       const Quantity<SCF_STB> & rs,
	       const Quantity<Fahrenheit> & t) const
{
  const double raw_t = t.get_value();
  const double yg_yo = yg.get_value()/yo.get_value();

  const double raw_rs = rs.get_value();
  const double f = raw_rs*pow(yg_yo, 0.526) + 0.968*raw_t;
  const double log_f = log10(f);
  const double log_f_2 = log_f*log_f;
      
  const double bo = 1 + pow(10, -6.58511 + 2.91329*log_f - 0.27683*log_f_2);

  return Quantity<RB_STB>(bo);  
}


inline Quantity<RB_STB>
BobHanafy::impl(const Quantity<SCF_STB> & rs) const
{
  const double bo = 0.0006*rs.get_value() + 1.079;

  return Quantity<RB_STB>(bo);  
}

inline Quantity<RB_STB>
BobKartoatmodjo::impl(const Quantity<Sgg> & yg,
		     const Quantity<Sg_do> & yo,
		     const Quantity<SCF_STB> & rs,
		     const Quantity<Fahrenheit> & t,
		     const Quantity<Fahrenheit> & tsep,
		     const Quantity<psia> & psep) const
{
  const Quantity<Api> api(yo);

  const double c =
    1 + 0.1595*pow(api, .4078)*pow(tsep, -0.2466)*log10(psep/114.7);
       
  const double ygcorr = c*yg.get_value();

  const double f =
    pow(rs, 0.755)*pow(ygcorr, 0.25)*pow(yo, -1.5) + 0.45*t.get_value();
  const double bo = 0.98496 + 0.0001 * pow(f, 1.5);

  return Quantity<RB_STB>(bo);
}

inline Quantity<RB_STB>
BobStanding::impl(const Quantity<Sgg> & yg,
		  const Quantity<Sg_do> & yo,
		  const Quantity<SCF_STB> & rs,
		  const Quantity<Fahrenheit> & t) const
{
  const double raw_yg = yg.get_value();
  const double raw_yo = yo.get_value();
  const double raw_t = t.get_value();

  const double raw_rs = rs.get_value();
  const double bo =
    0.972 + 0.000147*pow(raw_rs*sqrt(raw_yg/raw_yo) + 1.25*raw_t, 1.175);

  return Quantity<RB_STB>(bo);
}


inline Quantity<RB_STB>
BobPetroskyFarshad::impl(const Quantity<Sgg> & yg,
			const Quantity<Sg_do> & yo,
			const Quantity<SCF_STB> & rs,
			const Quantity<Fahrenheit> & t) const
{
  const double f =
    pow(rs, 0.3738)*pow(yg, 0.2914)/pow(yo, 0.6265) + 0.24626*pow(t, 0.5371);
  const double bo = 1.0113 + 7.2046e-5*pow(f, 3.0936);
        
  return Quantity<RB_STB>(bo);
}


inline Quantity<RB_STB>
BobTotalCFP::impl(const Quantity<Sgg> & yg,
		 const Quantity<Api> & api,
		 const Quantity<SCF_STB> & rs,
		 const Quantity<Fahrenheit> & t) const
{
  const double t_60 = t.get_value() - 60;
  const double api_yg = api.get_value()/yg.get_value();

  const double raw_rs = rs.get_value();
  const double bo = 1.022 + 4.857e-4*raw_rs - 2.009e-6*t_60*api_yg +
    17.569e-9*raw_rs*t_60*api_yg;

  return Quantity<RB_STB>(bo);
}


inline Quantity<RB_STB>
BobVasquezBeggs::impl(const Quantity<Sgg> & yg,
		     const Quantity<Api> & api,
		     const Quantity<SCF_STB> & rs,
		     const Quantity<Fahrenheit> & t,
		     const Quantity<Fahrenheit> & tsep,
		     const Quantity<psia> & psep) const
{
  const double raw_api = api.get_value();
  double c1 = 0, c2 = 0, c3 = 0;
  if (raw_api <= 30)
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

  const double raw_yg = yg.get_value();
  const double ygs = raw_yg*
    (1. + 5.912e-5 * raw_api * tsep.get_value() * log10(psep/114.7));

  const double t_60 = t.get_value() - 60;
  const double api_ygs = raw_api/ygs;

  const double raw_rs = rs.get_value();
  const double bo =
    1 + c1*raw_rs + c2*t_60 * api_ygs + c3 * raw_rs * t_60 * api_ygs;

  return Quantity<RB_STB>(bo);
}


inline Quantity<RB_STB>
BobPerezML::impl(const Quantity<Sgg> & yg,
		const Quantity<Sg_do> & yo,
		const Quantity<SCF_STB> & rsb,
		const Quantity<Fahrenheit> & t,
		const Quantity<psia> & p,
		const Quantity<psia> & pb) const
{

  if (p > pb)
    {
      ostringstream s;
      s << "BobPerezML::impl: pressure " << p << " is greater than pb " << pb;
      throw domain_error(s.str());
    }

  const double raw_rsb = rsb.get_value();
  const double raw_t = t.get_value();
  const double yg_yo = yg.get_value()/yo.get_value();

  const double c = 1e-4 * (raw_rsb*sqrt(yg_yo) + 1.25*raw_t);
  const double bob = 0.974322 + 4.35048*c - 2.85869*c*c;        

  const double raw_p = p.get_value();
  const double raw_pb = pb.get_value();

  const double bo = bob * (1 - (8.801 + 0.1547*raw_t) * 1e-3 * (1 - raw_p/raw_pb));

  return Quantity<RB_STB>(bo);
}


inline Quantity<RB_STB>
BobVelardeMcCain::impl(const Quantity<Sgg> & yg,
		      const Quantity<Sg_do> & yo,
		      const Quantity<SCF_STB> & rs,
		      const Quantity<SCF_STB> & rsb,
		      const Quantity<Fahrenheit> & t,
		      const Quantity<psia> & p) const
{
  constexpr double pwater = 62.4; // water specific weight in  lb/ft³ or lb/scf
  const double psto = pwater*yo.get_value();

  const double raw_yg = yg.get_value();
  const double raw_yo = yo.get_value();
  const double raw_rs = rs.get_value();
        
  double a = 0;
  double b = 52.8 - 0.01*rsb.get_value(); // Initial value pseudoliquid density
  double ppo = 0, ppof = 0;
  while (fabs(a - b) > 0.00001)
    {
      a = b;
      ppo = a;
      const double pa = -49.893 + 85.0149*raw_yg - 3.70373*raw_yg*ppo +
	0.047981*raw_yg*ppo*ppo + 2.98914*ppo- 0.035688*ppo*ppo;
      ppof = (raw_rs*raw_yg + 4600*raw_yo) / (73.71 + raw_rs*raw_yg/pa);
      b = ppof;
    }

  ppo = ppof;

  const double raw_p = p.get_value();
  const double t_60 = t.get_value() - 60;

  const double p_1000 = raw_p/1000;
  const double p_1000_2 = p_1000*p_1000;
      
  const double pbs = ppo + (0.167 + 16.181*pow(10, -0.0425*ppo)) *
    p_1000 - 0.01*(0.299 + 263*pow(10, -0.0603*ppo))*p_1000_2;
      
  const double poR = pbs - (0.00302 + 1.505 * pow(pbs, -0.951)) *
    pow(t_60, 0.938) + 0.0233*pow(10, -0.0161*pbs)*pow(t_60, 0.475);
            
  const double bo = (psto + 0.01357*raw_rs*raw_yg)/poR;

  return Quantity<RB_STB>(bo);
}


inline Quantity<RB_STB>
BobMillanArcia::impl(const Quantity<Api> & api,
		    const Quantity<SCF_STB> & rsb,
		    const Quantity<psia> & p,
		    const Quantity<psia> & pb) const
{

  if (p > pb)
    {
      ostringstream s;
      s << "BobMillanArcia::impl: pressure " << p << " is greater than pb " << pb;
      throw domain_error(s.str());
    }

  const double raw_p = p.get_value();
  const double raw_pb = pb.get_value();
  const double raw_api = api.get_value();

  const double bob = 1.3537 * pow(rsb, 0.1505) * pow(raw_pb, -0.1239) *
    exp(-0.00405*raw_api);

  double bo = 0;  
  if (raw_p < raw_pb) // Saturated oil
    bo = bob * (0.9419 + 0.0608 * raw_p/raw_pb);

  return Quantity<RB_STB>(bo);
}


inline Quantity<RB_STB>
BobManucciRosales::impl(const Quantity<Api> & api,
			const Quantity<SCF_STB> & rs,
			const Quantity<Fahrenheit> & t) const
{
  const double raw_api = api.get_value();
  const double raw_t = t.get_value();

  const double x = 3.27e-4*raw_t + 0.00321*raw_api;

  const double bo = 0.2378 * pow(rs, 0.221) * pow(10, x);

  return Quantity<RB_STB>(bo);
}


# endif // SATURATED_OIL_FORMATION_VOLUME_FACTOR_IMPL_H
