# ifndef BUBBLE_POINT_PRESSURE_IMPL_H
# define BUBBLE_POINT_PRESSURE_IMPL_H

# include <correlations/bubble-point-pressure.H>

inline double
PbAlMarhoun::impl(const double & yg,
		  const double & yo,
		  const double & rsb,
		  const double & t) noexcept
{
  return 0.00538088 * pow(rsb, 0.715082) * pow(yg, -1.87784) *
    pow(yo, 3.1437) * pow(t, 1.32657);
}

inline double
PbAlShammasi::impl(const double & yg,
		   const double & yo,
		   const double & rsb,
		   const double & t) noexcept 
{
  constexpr double c1 = 5.527215;
  constexpr double c2 = -1.841408;
  constexpr double c3 = 0.783716;

  const double pb = pow(yo, c1) * exp(c2*yo*yg) * pow(rsb*(t + 460) * yg, c3);

  return pb;
}

inline double
PbDeGhettoDetachedFromRs::impl(const double& yg,
		 const double & rsb,
		 const double & api,
		 const double & t) noexcept
{
  double pb = 0;
    
  if (api <= 10) // Extra-heavy oil
    pb = (pow(rsb/yg, 1/1.1128)*10.7025 / pow(10, 0.0169*api - 0.00156*t));
  else
    pb = 15.7286*pow(rsb/yg, 0.7885) * (pow(10, 0.0020*t) / pow(10, 0.0142*api));

  return pb;
}

inline double
PbDeGhettoEtAl::impl(const double & yg,
		 const double & rsb,
		 const double & api,
		 const double & t,
		 const double & tsep,
		 const double & psep) noexcept
{
  double pb = 0;
    
  if (api <= 10) // Extra-heavy oil
    pb = (pow(rsb/yg, 1/1.1128)*10.7025 / pow(10, 0.0169*api - 0.00156*t)); 
  else {
    const double ygcorr = yg*(1 + 0.5912*api*tsep*log10(psep/114.7) * 0.0001);
    const double pb_aux = (56.434 * rsb)/(ygcorr * pow(10, 10.9267 * api / (t + 460)));
    pb = pow(pb_aux, 1/1.2057);
  }

  return pb;
}

inline double
PbDindorukChristmanDetachedFromRs::impl(const double & yg,
			  const double & rsb,
			  const double & api,
			  const double & t) noexcept
{
  constexpr double a1  = 0.000000000142828; // 1.42828 * 10 ** -10;
  constexpr double a2  = 2.844591797;
  constexpr double a3  = -0.000674896; // -6.74896 * 10 ** -4;
  constexpr double a4  = 1.225226436;
  constexpr double a5  = 0.033383304;
  constexpr double a6  = -0.272945957;
  constexpr double a7  = -0.084226069;
  constexpr double a8  = 1.869979257;
  constexpr double a9  = 1.221486524;
  constexpr double a10 = 1.370508349;
  constexpr double a11 = 0.011688308;

  const double n = a1*pow(t, a2) + a3*pow(api, a4);
    
  const double d = a5 + 2*pow(rsb, a6) / pow(yg, a7);
  const double d2 = d*d;

  const double a = n/d2;

  return  a8 * (pow(rsb, a9) * pow(10, a) / pow(yg, a10) + a11);
}

inline double
PbDindorukChristman::impl(const double & yg,
			  const double & rsb,
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

  constexpr double epsilon = 1.0e-8;
  double p = 0, pf = 0;
  double pprev = 0.01;
                
  while (fabs(pprev - p) > epsilon){
      	p = pprev;

	const double d = a5 + 2*pow(api, a6)/pow(p, a7);
  	const double d2 = d*d;
  	const double a = (a1*pow(api, a2) + a3*pow(t, a4)) / d2;
      	const double f = pow((p/a8 + a9) * pow(yg, a10) * pow(10, a), a11) - rsb;

      	const double dfdp = pow(10, -a)*a11*pow(yg,-a10)*pow((p/a8 + a9) * pow(yg, a10) * pow(10, a), a11)
*(4*pow(10,a)*a7*pow(api,a6)*pow(p,-a7)*pow(yg,a10)*(a9 + p/a8)*
(a1*pow(api,a2) + a3*pow(t,a4))*log(10)/(p*pow(d,3)) + pow(10,a) *pow(yg,a10)/a8)
/(a9 + p/a8);

      	pf = p - f/dfdp;
      	pprev = pf;
  }
                
  p = pf;
  return p;

}

inline double
PbDoklaOsman::impl(const double & yg,
		   const double & rsb,
		   const double & yo,
		   const double & t) noexcept
{
  return 8363.86 * pow(yg, -1.01049) * pow(yo, 0.107991) *
    pow(t, -0.952584) * pow(rsb, 0.724047);
}

inline double
PbHanafyDetachedFromRs::impl(const double & rsb) noexcept
{
  const double rsft = 69 + 1.071 * rsb;
    
  // Initial differential gas-oil ratio
  const double rsi =  23.94 + 1.101 * rsft;
    
  return 3.205*rsi + 157.27;
}

inline double
PbHanafy::impl(const double & rsb) noexcept
{

  return 3.205*rsb + 157.27;
}

inline double
PbKartoatmodjoSchmidt::impl(const double & yg,
			    const double & rsb,
			    const double & api,
			    const double & t,
			    const double & tsep,
			    const double & psep) noexcept
{
  const double c = 1 + 0.1595 * pow(api, 0.4078) * pow(tsep, -0.2466) *
    log10(psep/114.7);
        
  const double ygcorr = c * yg;

  double c1, c2, c3, c4;
  if (api > 30)
    {
      c1 = 0.0315;
      c2 = 0.7587;
      c3 = 11.289;
      c4 = 0.9143;
    }
  else
    {
      c1 = 0.05958;
      c2 = 0.7972;
      c3 = 13.1405;
      c4 = 0.9986;
    }

  const double p = rsb /
    (c1 * pow(ygcorr, c2) * pow(10, c3 * api / (t + 460)));

  return pow(p, c4);
}

inline double
PbLasater::impl(const double & yg,
		const double & rsb,
		const double & yo,
		const double & t) noexcept
{
  const Quantity<Api> api = unit_convert<Sg_do, Api>(yo);

      // effective molecular weight
  const double mo = api <= 40 ? 630 - (10*api.raw()) : 73110 * pow(api, -1.562);

      // mol fraction of gas
  double pbfactor = 0;
  double ygfactor = (rsb/379.3) / ((rsb/379.3) + (350 * yo/mo));
  if (ygfactor <= 0.6)
    pbfactor = (0.679 * exp(2.786*ygfactor)) - 0.323;
  else
    pbfactor = (8.26 * pow(ygfactor, 3.56)) + 1.95;

  const double pb = (pbfactor * (t + 459.6)) / yg;

  return pb;
}

inline double
PbPetroskyFarshad::impl(const double & yg,
			const double & rsb,
			const double & api,
			const double & t) noexcept
{
      // effective molecular weight
  const double x = 0.00004561*pow(t, 1.3911) - 0.0007916*pow(api, 1.541);

  return 112.727 * ((pow(rsb, 0.5774)/pow(yg, 0.8439)*pow(10, x)) - 12.34);
}

inline double
PbStanding::impl(const double & yg,
		 const double & rsb,
		 const double & api,
		 const double & t) noexcept
{
  const double pb = pow(rsb/yg, 0.83) * (pow(10, 0.00091*t -0.0125*api)) - 1.4;
  return 18.2*pb;
}

inline double
PbTotalCFP::impl(const double & yg,
		 const double & rsb,
		 const double & api,
		 const double & t) noexcept
{
  double c1 = 0, c2 = 0, c3 = 0, c4 = 0;

  if (api <= 10)
    {
      c1 = 12.847;
      c2 = 0.9636;
      c3 = 0.000993;
      c4 = 0.03417;
    }
  else if (api <= 35)
    {
      c1 = 25.2755;
      c2 = 0.7617;
      c3 = 0.000835;
      c4 = 0.011292;
    }
  else
    {
      c1 = 216.4711;
      c2 = 0.6922;
      c3 = -0.000427;
      c4 = 0.02314;
    }

  return c1 * pow(rsb/yg, c2) * pow(10, c3*t - c4*api);
}


inline double
PbVasquezBeggs::impl(const double & yg,
		     const double & rsb,
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

  const double ygs = yg * (1 + 5.912e-05 * api * tsep * log10(psep/114.7));

  const double pb_aux = rsb / (c1*ygs*exp((c3*api) / (t + 460)));

  const double pb = pow(pb_aux, 1/c2);

  return pb;
}


inline double
PbVelarde::impl(const double & yg,
		const double & rsb,
		const double & api,
		const double & t) noexcept
{
  const double x = 0.013098 * pow(t, 0.282372) - 8.2e-06 * pow(api, 2.176124);

  const double pb = 1091.47 *
    pow((pow(rsb, 0.081465) * pow(yg, -0.161488) * pow(10, x) - 0.740152),
	5.354891);

  return pb;
}

inline double
PbCegarra::impl(const double & yg,
		const double & rsb,
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
  else if (api >= 29.9)
    {
      l1 = 809.238;
      l2 = 0.32;
      l3 = 0.00061;
      l4 = 0.011;
      l5 = 1.1142;
    }

  const double pb = l1 * (pow((rsb/yg), l2) * pow(10, l3*t - l4*api) - l5);

  return pb;
}

inline double
PbPerezML::impl(const double & yg,
		const double & rsb,
		const double & api,
		const double & t) noexcept
{
  const double x = 0.0002573*t - 0.0253643*api;
        
  return 10.667657 * pow(rsb/yg, 1.00139101) * pow(10, x);
}

inline double
PbMillanArcia::impl(const double & yg,
		    const double & rsb,
		    const double & api,
		    const double & t) noexcept
{
  const double x = 0.00091*t - 0.0125*api;
        
  return 25.3302 * pow(pow(rsb/yg, 0.8303) * pow(10, x), 0.9433);

}

inline double
PbManucciRosales::impl(const double & yg,
		       const double & rsb,
		       const double & api,
		       const double & t) noexcept
{
  const double x = 0.000922*t - 0.0072*api;
        
  return  84.88 * pow(rsb/yg, 0.53) * pow(10, x);
}

void PbGlaso::precondition(const Quantity<MoleFraction> & n2,
			   const Quantity<MoleFraction> & co2,
			   const Quantity<MoleFraction> & h2s) const
{
  if (n2.raw() + co2.raw() +
      h2s.raw() >= 1)
    {
      ostringstream s;
      s << "In PbGlaso::impl: invalid values for nonhydrocarbon concentrations.";
      ALEPHTHROW(WrongNonhydrocarbonsConcentrations, s.str());
    }
}

inline double
PbGlaso::impl(const double & yg,
	      const double & rsb,
	      const double & api,
	      const double & t,
	      const double & n2,
	      const double & co2,
	      const double & h2s) noexcept
{
  const double x = pow(rsb/yg, 0.816)*pow(t, 0.172)/pow(api, 0.989);

  const double log10x = log10(x);
  const double pbhc = pow(10, 1.7669 + 1.7447*log10x - 0.30218*log10x*log10x);

      // Effects of nonhydrocarbons on bubble point pressure
  const double n2effect = 1 + ((-2.65e-4*api + 5.5e-3)*t +
			       (0.0931*api - 0.8295)) *
    n2 + (1.954e-11*pow(api, 4.699)*t + 0.027*api - 2.366) *
    n2*n2;

  const double co2effect = 1 - 693.8*co2 * pow(t, -1.553);
  
  const double h2seffect = 1 - (0.9035 + 0.0015*api) * h2s +
    0.019*(45 - api)*h2s*h2s;
  
  const double pb = pbhc * n2effect * co2effect * h2seffect;

  return pb;
}

inline double
PbLopezCR::impl(const double & yg,
		const double & rsb,
		const double & api,
		const double & t) noexcept
{
  const double pb = (rsb + 35.6135 + 0.052482 * t - 51.479 * yg - 1.98597 * api) / 0.0830159;

  return pb;
}

inline double
PbLopezCRDetachedFromRs::impl(const double & yg,
			      const double & rsb,
			      const double & api,
			      const double & t) noexcept
{
  const double pb = 92.8034 + 6.77638 * (rsb/yg) + 0.688117 * t - 24.0569 * api;

  return pb;
}

inline double
PbSalazar::impl(const double & yg,
		const double & rsb,
		const double & api,
		const double & t) noexcept
{
  const double pb = 402.144 + 2.150 * t + 12.282 * rsb - 542.629 * yg - 46.415 * api;

  return pb;
}

# endif // BUBBLE_POINT_PRESSURE_IMPL_H
