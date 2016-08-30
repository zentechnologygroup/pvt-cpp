
# ifndef OIL_FORMATION_VOLUME_FACTOR_IMPL_H
# define OIL_FORMATION_VOLUME_FACTOR_IMPL_H

# include <oil-formation-volume-factor.H>

inline Quantity<RB_STB>
BoAlmarhoun::impl(const Quantity<Sgg> & yg,
		  const Quantity<Sg_do> & yo,
		  const Quantity<SCF_STB> & rs,
		  const Quantity<SCF_STB> & rsb,
		  const Quantity<Rankine> & t,
		  const Quantity<psia> & p,
		  const Quantity<psia> & pb,
		  const Quantity<psia_1> & co) const
{
  const double raw_p = p.get_value();
  const double raw_pb = pb.get_value();
  const double raw_t = t.get_value();

  double bo = 0;
  if (raw_p < raw_pb) // Saturated oil
    {
      const double f = pow(rs, 0.74239) * pow(yg, 0.323294) * pow(yo, -1.20204);
      bo = 0.497069 + 0.000862963*raw_t + 0.00182594*f + 0.00000318099*f*f; 
    }
  else // Undersaturated oil
    {
      const double fb = pow(rsb, 0.74239)*pow(yg, 0.323294)*pow(yo, -1.20204);
      const double bob = 0.497069 + 0.000862963*raw_t + 0.00182594*fb +
	0.00000318099*fb*fb;
      bo = bob * exp(co.get_value()*(raw_pb - raw_p));      
    }

  return Quantity<RB_STB>(bo);
}

inline Quantity<RB_STB>
BoAlShammasi::impl(const Quantity<Sgg> & yg,
		   const Quantity<Sg_do> & yo,
		   const Quantity<SCF_STB> & rs,
		   const Quantity<SCF_STB> & rsb,
		   const Quantity<Fahrenheit> & t,
		   const Quantity<psia> & p,
		   const Quantity<psia> & pb,
		   const Quantity<psia_1> & co) const
{
  const double raw_p = p.get_value();
  const double raw_pb = pb.get_value();
  const double raw_t = t.get_value();
  const double raw_yg = yg.get_value();
  const double raw_yo = yo.get_value();

  double bo = 0;
  if (raw_p < raw_pb) // Saturated oil
    {
      const double raw_rs = rs.get_value();
      bo = 1 + 0.000000553*(raw_rs*(raw_t - 60)) + 0.000181*raw_rs/raw_yo +
	0.000449*((raw_t - 60)/raw_yo) + 0.000206*(raw_rs*raw_yg/raw_yo);
    }        
  else
    {
       const double raw_rsb = rsb.get_value();
       const double bob = 1 +  0.000000553*(raw_rsb*(raw_t - 60)) +
	 0.000181*(raw_rsb/raw_yo) + 0.000449*((raw_t - 60)/raw_yo) +
	 0.000206*(raw_rsb*raw_yg/raw_yo);
            
       bo = bob * exp(co.get_value() * (raw_pb - raw_p));
    }

  return Quantity<RB_STB>(bo);
}

inline Quantity<RB_STB>
BoDeGhetto::impl(const Quantity<Sgg> & yg,
		 const Quantity<Api> & api,
		 const Quantity<SCF_STB> & rs,
		 const Quantity<SCF_STB> & rsb,
		 const Quantity<Fahrenheit> & t,
		 const Quantity<Fahrenheit> & tsep,
		 const Quantity<psia> & p,
		 const Quantity<psia> & pb,
		 const Quantity<psia> & psep,
		 const Quantity<psia_1> & co) const

{
  constexpr double c1 = 0.0004677;
  constexpr double c2 = 0.00001751;
  constexpr double c3 = -0.00000001811;

  const double raw_p = p.get_value();
  const double raw_pb = pb.get_value();
  const double raw_api = api.get_value();
  const double raw_t = t.get_value();
        
  const double ygs = yg.get_value() *
    (1. + 0.00005912*raw_api*tsep.get_value()*log10(psep.get_value()/114.7));

  double bo = 0;
  if (raw_p < raw_pb) // Saturated oil
    {
      const double raw_rs = rs.get_value();
      bo = 1 + c1*raw_rs + c2*(raw_t - 60) * (raw_api/ygs) +
	c3*raw_rs*(raw_t - 60)*(raw_api/ygs);
    }
  else // Undersaturated oil
    {
      const double raw_rsb = rsb.get_value();
      const double bob = 1 + c1*raw_rsb + c2*(raw_t - 60)*(raw_api/ygs) +
	c3*raw_rsb * (raw_t - 60) * (raw_api/ygs);

      bo = bob * exp(co.get_value() * (raw_pb - raw_p));
    }

  return Quantity<RB_STB>(bo);
}


inline Quantity<RB_STB>
BoDindorukChristman::impl(const Quantity<Sgg> & yg,
			  const Quantity<Api> & api,
			  const Quantity<SCF_STB> & rs,
			  const Quantity<SCF_STB> & rsb,
			  const Quantity<Fahrenheit> & t,
			  const Quantity<Fahrenheit> & tsep,
			  const Quantity<psia> & p,
			  const Quantity<psia> & pb,
			  const Quantity<psia_1> & co) const
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

  double bo = 0;
  const double raw_p = p.get_value();
  const double raw_pb = pb.get_value();
  if (raw_p < raw_pb) // Saturated oil
    {
      const double raw_rs = rs.get_value();
      const double d = a8 + (2 *pow(raw_rs, a9)/pow(raw_yg, a10))*t_60;
      const double d2 = d*d;
      
      const double a = pow(pow(raw_rs, a1)*pow(raw_yg, a2)/pow(yo, a3) +
			   a4*pow(t_60, a5) + a6*raw_rs, a7) /  d2;
      
      bo = a11 + a12*a + a13*a*a + a14*t_60*api.get_value()/raw_yg;
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
    }
  else // Undersaturated oil
    {
      const double raw_rsb = rsb.get_value();
      const double d = a8 + (2*pow(raw_rsb, a9)/pow(yg, a10))*t_60;
      const double d2 = d*d;

      const double ab = pow(pow(raw_rsb, a1)*pow(yg, a2)/pow(yo, a3)  + a4 *
			    pow(t_60, a5) + a6 * raw_rsb, a7) / d2;

      double bob = a11 + a12*ab + a13*ab*ab + a14*t_60*api.get_value() /
	raw_yg;
            
      if (bob > 2)
	{
	  const double t_tsep = raw_t - tsep.get_value();
	  double bodl = bo;
	  const double n = pow(t_tsep, b1) * pow(log10(bodl)*tanh(bodl), b2) +
	    pow(b3*(bodl - 1), b4);
	  const double d = pow(1 + pow(bodl, b5)*pow(t_tsep, b6)*
			       pow(log10(bodl), b7), b8);

	  bob = 1 + (n / d);
	}
      
      bo = bob * exp(co.get_value()*(raw_pb - raw_p));
    }

  return Quantity<RB_STB>(bo);
}


inline Quantity<RB_STB>
BoDoklaOsman::impl(const Quantity<Sgg> & yg,
		   const Quantity<Sg_do> & yo,
		   const Quantity<SCF_STB> & rs,
		   const Quantity<SCF_STB> & rsb,
		   const Quantity<Rankine> & t,
		   const Quantity<psia> & p,
		   const Quantity<psia> & pb,
		   const Quantity<psia_1> & co) const
{
  const double raw_p = p.get_value();
  const double raw_pb = pb.get_value();
  const double raw_t = t.get_value();

  double bo = 0;
  if (raw_p < raw_pb) // Saturated oil
    {
      const double m = pow(rs, 0.773572) * pow(yg, 0.40402) * pow(yo, -0.882605);
      bo = 0.431935e-1 + 0.156667e-2*raw_t + 0.139775e-2*m + 0.380525e-5*m*m;
    }
  else // Undersaturated oil
    {
      const double mb =
	pow(rsb, 0.773572) * pow(yg, 0.40402) * pow(yo, -0.882605);
      const double bob = 0.431935e-1 + 0.156667e-2*raw_t + 0.139775e-2*mb
	+ 0.380525e-5*mb*mb;
      bo = bob * exp(co.get_value()*(raw_pb - raw_p));
    }

  return Quantity<RB_STB>(bo);
}

inline Quantity<RB_STB>
BoGlaso::impl(const Quantity<Sgg> & yg,
	      const Quantity<Sg_do> & yo,
	      const Quantity<SCF_STB> & rs,
	      const Quantity<SCF_STB> & rsb,
	      const Quantity<Fahrenheit> & t,
	      const Quantity<psia> & p,
	      const Quantity<psia> & pb,
	      const Quantity<psia_1> & co) const
{
  const double raw_p = p.get_value();
  const double raw_pb = pb.get_value();
  const double raw_t = t.get_value();
  const double yg_yo = yg.get_value()/yo.get_value();

  double bo = 0;
  if (raw_p < raw_pb) // Saturated oil
    {
      const double raw_rs = rs.get_value();
      const double f = raw_rs*pow(yg_yo, 0.526) + 0.968*raw_t;
      const double log_f = log10(f);
      const double log_f_2 = log_f*log_f;
      
      bo = 1 + pow(10, -6.58511 + 2.91329*log_f - 0.27683*log_f_2);
    }
  else // Undersaturated oil
    {
      const double raw_rsb = rsb.get_value();
      const double fb = raw_rsb*pow(yg_yo, 0.526) + 0.968*raw_t;
      const double log_fb = log10(fb);
      const double log_fb_2 = log_fb*log_fb;

      const double bob =
	1 + pow(10, -6.58511 + 2.91329*log_fb - 0.27683*log_fb_2);

      bo = bob * exp(co.get_value()*(raw_pb - raw_p));        
    }

  return Quantity<RB_STB>(bo);  
}





# endif // OIL_FORMATION_VOLUME_FACTOR_IMPL_H
