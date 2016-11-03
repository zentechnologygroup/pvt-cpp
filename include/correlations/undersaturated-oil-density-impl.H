# ifndef UNDERSATURATED_OIL_DENSITY_IMPL_H
# define UNDERSATURATED_OIL_DENSITY_IMPL_H

# include <correlations/undersaturated-oil-density.H>


inline Quantity<Lb_ft3>
PoaBradley::impl(const Quantity<Sgg> & yg,
		 const Quantity<SCF_STB> & rsb,
		 const Quantity<RB_STB> & bobp,
		 const Quantity<Sgg> & yo,
		 const Quantity<psia> & p,
		 const Quantity<psia> & pb,
		 const Quantity<psia_1> & co) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "PoaBradley::impl: pressure " << p << " is less than pb" << pb;
      throw domain_error(s.str());
    }
  
  const double pob =
    (350*yo.raw() + 0.0764*yg.raw()*rsb.raw())/(5.615*bobp.raw());

  const double po = pob * exp(co.raw() * (p.raw() - pb.raw()));

  return Quantity<Lb_ft3>(po);
}




# endif // UNDERSATURATED_OIL_DENSITY_IMPL_H
