
# ifndef OIL_FORMATION_VOLUME_FACTOR_IMPL_H
# define OIL_FORMATION_VOLUME_FACTOR_IMPL_H

# include <oil-formation-volume-factor-impl.H>

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
      const double fb = pow(rsb, 0.74239) * pow(yg, 0.323294) * pow(yo, -1.20204);
      const double bob = 0.497069 + 0.000862963*raw_t + 0.00182594*fb +
	0.00000318099*fb*fb;
      bo = bob * exp(co.get_value()*(raw_pb - raw_p));      
    }

  return Quantity<RB_STB>(max(min_val, bo));
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
  // put here the implementation
}

# endif // OIL_FORMATION_VOLUME_FACTOR_IMPL_H
