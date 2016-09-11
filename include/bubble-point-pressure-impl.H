# ifndef BUBBLE_POINT_PRESSURE_IMPL_H
# define BUBBLE_POINT_PRESSURE_IMPL_H

# include <bubble-point-pressure.H>

inline Quantity<psia>
PbAlMarhoun::impl(const Quantity<Sgg> & yg,
		  const Quantity<Sg_do> & yo,
		  const Quantity<SCF_STB> & rsb,
		  const Quantity<Rankine> & t) const
{
  const double r = 0.00538088 * pow(rsb, 0.715082) * pow(yg, -1.87784) *
    pow(yo, 3.1437) * pow(t, 1.32657);

  return Quantity<psia>(r);
}

inline Quantity<psia>
PbAlShammasi::impl(const Quantity<Sgg> & yg,
		   const Quantity<Sg_do> & yo,
		   const Quantity<SCF_STB> & rsb,
		   const Quantity<Fahrenheit> & t) const
{
  constexpr double c1 = 5.527215;
  constexpr double c2 = -1.841408;
  constexpr double c3 = 0.783716;

  const double r = pow(yo, c1) *
    exp(c2 * floor(yo.get_value() * yg.get_value())) *
    pow(rsb.get_value() * (t.get_value() + 460) * yg.get_value(), c3);

  return Quantity<psia>(r);
}

inline Quantity<psia>
PbDeGhetto::impl(const Quantity<Sgg> & yg,
		 const Quantity<SCF_STB> & rsb,
		 const Quantity<Api> & api,
		 const Quantity<Fahrenheit> & t) const
{
  const double raw_api = api.get_value();
  const double raw_rsb = rsb.get_value();
  const double raw_yg = yg.get_value();
  const double raw_t = t.get_value();
  double pb = 0;
    
  if (raw_api <= 10) // Extra-heavy oil
    pb = (pow(raw_rsb/raw_yg, 1/1.1128)*10.7025 /
	  pow(10, 0.0169*raw_api - 0.00156*raw_t));
  else
    pb = 15.7286 * pow(raw_rsb/raw_yg, 0.7885) *
      (pow(10, 0.0020*raw_t) / pow(10, 0.0142*raw_api));

  return Quantity<psia>(pb);
}

inline Quantity<psia>
PbDindorukChristman::impl(const Quantity<Sgg> & yg,
			  const Quantity<SCF_STB> & rsb,
			  const Quantity<Api> & api,
			  const Quantity<Fahrenheit> & t) const
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

  double pb = a8 * (pow(rsb, a9) * pow(10, a) / pow(yg, a10) + a11);

  return Quantity<psia>(pb);
}

inline Quantity<psia>
PbDoklaOsman::impl(const Quantity<Sgg> & yg,
		   const Quantity<SCF_STB> & rsb,
		   const Quantity<Sg_do> & yo,
		   const Quantity<Fahrenheit> & t) const
{
  const double pb = 8363.86 * pow(yg, -1.01049) * pow(yo, 0.107991) *
      pow(t, -0.952584) * pow(rsb, 0.724047);

  return Quantity<psia>(pb);
}

inline Quantity<psia>
PbHanafy::impl(const Quantity<SCF_STB> & rsb) const
{
  const double rsft = 69 + 1.071 * rsb.get_value();
    
  // Initial differential gas-oil ratio
  const double rsi =  23.94 + 1.101 * rsft;
    
  const double pb = (3.205 * rsi) + 157.27;

  return Quantity<psia>(pb);
}

inline Quantity<psia>
PbKartoatmodjoSchmidt::impl(const Quantity<Sgg> & yg,
			    const Quantity<SCF_STB> & rsb,
			    const Quantity<Api> & api,
			    const Quantity<Fahrenheit> & t,
			    const Quantity<Fahrenheit> & tsep,
			    const Quantity<psia> & psep) const
{
  const double raw_api = api.get_value();
  const double c = 1 + 0.1595 * pow(raw_api, 0.4078) * pow(tsep, -0.2466) *
    log10(psep.get_value() / 114.7);
        
  const double ygcorr = c * yg.get_value();

  double c1, c2, c3, c4;
  if (raw_api > 30)
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

  const double p = rsb.get_value() /
    (c1 * pow(ygcorr, c2) * pow(10, c3 * raw_api / (t.get_value() + 460)));

  return Quantity<psia>(pow(p, c4));
}

inline Quantity<psia>
PbLasater::impl(const Quantity<Sgg> & yg,
		const Quantity<SCF_STB> & rsb,
		const Quantity<Api> & api,
		const Quantity<Fahrenheit> & t) const
{
      // effective molecular weight
  const double mo =
    api <= 40 ? 630 - (10*api.get_value()) : 73110 * pow(api, -1.562);

  const Quantity<Sg_do> yo = api;

      // mol fraction of gas
  double pbfactor = 0;
  double ygfactor = (rsb.get_value()/379.3) /
    ((rsb.get_value()/379.3) + (350 * yo.get_value()/mo));
  if (ygfactor <= 0.6)
    pbfactor = (0.679 * exp(2.786*ygfactor)) - 0.323;
  else
    pbfactor = (8.26 * pow(ygfactor, 3.56)) + 1.95;

  const double pb = (pbfactor * (t.get_value() + 459.6)) / yg.get_value();

  return Quantity<psia>(pb);
}

inline Quantity<psia>
PbPetroskyFarshad::impl(const Quantity<Sgg> & yg,
			const Quantity<SCF_STB> & rsb,
			const Quantity<Api> & api,
			const Quantity<Fahrenheit> & t) const
{
      // effective molecular weight
  const double x = 0.00004561*pow(t, 1.3911) - 0.0007916*pow(api, 1.541);

  const double pb = 112.727 *
    ((pow(rsb, 0.5774)/pow(yg, 0.8439)*pow(10, x)) - 12.34);

  return Quantity<psia>(pb);
}

inline Quantity<psia>
PbStanding::impl(const Quantity<Sgg> & yg,
		 const Quantity<SCF_STB> & rsb,
		 const Quantity<Api> & api,
		 const Quantity<Fahrenheit> & t) const
{
  const double pb =
    pow(rsb.get_value()/yg.get_value(), 0.83) *
    (pow(10, 0.00091*t.get_value() -0.0125*api.get_value())) -
    1.4;

  return Quantity<psia>(18.2*pb);
}

inline Quantity<psia>
PbTotalCFP::impl(const Quantity<Sgg> & yg,
		 const Quantity<SCF_STB> & rsb,
		 const Quantity<Api> & api,
		 const Quantity<Fahrenheit> & t) const
{
  double c1 = 0, c2 = 0, c3 = 0, c4 = 0;

  auto raw_api = api.get_value();
  if (raw_api <= 10)
    {
      c1 = 12.847;
      c2 = 0.9636;
      c3 = 0.000993;
      c4 = 0.03417;
    }
  else if (raw_api <= 35)
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

  const double pb = c1 * pow(rsb.get_value()/yg.get_value(), c2) *
    pow(10, (c3*t.get_value()) - (c4*raw_api));

  return Quantity<psia>(pb);
}


inline Quantity<psia>
PbVasquezBeggs::impl(const Quantity<Sgg> & yg,
		     const Quantity<SCF_STB> & rsb,
		     const Quantity<Api> & api,
		     const Quantity<Fahrenheit> & t,
		     const Quantity<Fahrenheit> & tsep,
		     const Quantity<psia> & psep) const
{
  double c1 = 0, c2 = 0, c3 = 0;

  auto raw_api = api.get_value();
  if (raw_api <= 30)
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

  const double ygs = yg.get_value() * (1 + 5.912e-05 * raw_api *
				       tsep.get_value() *
				       log10(psep.get_value()/114.7));

  const double pb_aux = rsb.get_value() /
    (c1*ygs*exp((c3*raw_api) / (t.get_value() + 460)));

  const double pb = pow(pb_aux, 1/c2);

  return Quantity<psia>(pb);
}


inline Quantity<psia>
PbVelarde::impl(const Quantity<Sgg> & yg,
		const Quantity<SCF_STB> & rsb,
		const Quantity<Api> & api,
		const Quantity<Fahrenheit> & t) const
{
  const double x = 0.013098 * pow(t, 0.282372) - 8.2e-06 * pow(api, 2.176124);

  const double pb = 1091.47 *
    pow((pow(rsb, 0.081465) * pow(yg, -0.161488) * pow(10, x) - 0.740152),
	5.354891);

  return Quantity<psia>(pb);
}

inline Quantity<psia>
PbCegarra::impl(const Quantity<Sgg> & yg,
		const Quantity<SCF_STB> & rsb,
		const Quantity<Api> & api,
		const Quantity<Fahrenheit> & t) const
{
  const double raw_api = api.get_value();

  double l1 = 0, l2 = 0, l3 = 0, l4 = 0, l5 = 0;
  if (raw_api < 29.9)
    {
      l1 = 154.158;
      l2 = 0.4577;
      l3 = 0.0006680;
      l4 = 0.000514;
      l5 = 4.70257;
    }
  else if (raw_api >= 29.9)
    {
      l1 = 809.238;
      l2 = 0.32;
      l3 = 0.00061;
      l4 = 0.011;
      l5 = 1.1142;
    }

  const double pb = l1 * (pow((rsb.get_value()/yg.get_value()), l2) *
			  pow(10, l3*t.get_value() - l4*raw_api) - l5);

  return Quantity<psia>(pb);
}


inline Quantity<psia>
PbPerezML::impl(const Quantity<Sgg> & yg,
		const Quantity<SCF_STB> & rsb,
		const Quantity<Api> & api,
		const Quantity<Fahrenheit> & t) const
{
  const double x = 0.0002573*t.get_value() - 0.0253643*api.get_value();
        
  const double pb = 10.667657 *
    pow(rsb.get_value()/yg.get_value(), 1.00139101) * pow(10, x);

  return Quantity<psia>(pb);
}


inline Quantity<psia>
PbMillanArcia::impl(const Quantity<Sgg> & yg,
		    const Quantity<SCF_STB> & rsb,
		    const Quantity<Api> & api,
		    const Quantity<Fahrenheit> & t) const
{
  const double x = 0.00091*t.get_value() - 0.0125*api.get_value();
        
  const double pb = 25.3302 * pow(pow(rsb.get_value()/yg.get_value(), 0.8303) *
				  pow(10, x), 0.9433);

  return Quantity<psia>(pb);
}



inline Quantity<psia>
PbManucciRosales::impl(const Quantity<Sgg> & yg,
		       const Quantity<SCF_STB> & rsb,
		       const Quantity<Api> & api,
		       const Quantity<Fahrenheit> & t) const
{
  const double x = 0.000922*t.get_value() - 0.0072*api.get_value();
        
  const double pb = 84.88 * pow(rsb.get_value()/yg.get_value(), 0.53) *
    pow(10, x);

  return Quantity<psia>(pb);
}


# endif // BUBBLE_POINT_PRESSURE_IMPL_H
