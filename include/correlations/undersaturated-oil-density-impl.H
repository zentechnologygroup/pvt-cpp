# ifndef UNDERSATURATED_OIL_DENSITY_IMPL_H
# define UNDERSATURATED_OIL_DENSITY_IMPL_H

# include <correlations/undersaturated-oil-density.H>


inline Quantity<Lb_ft3>
PoaBradley::impl(const Quantity<Lb_ft3> & pobp,
		 const Quantity<psia> & p,
		 const Quantity<psia> & pb,
		 const Quantity<psia_1> & co) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "PoaBradley::impl: pressure " << p << " is not greater than pb" << pb;
      throw domain_error(s.str());
    }

  const double po = pobp.raw() * exp(co.raw() * (p.raw() - pb.raw()));

  return Quantity<Lb_ft3>(po);
}




# endif // UNDERSATURATED_OIL_DENSITY_IMPL_H
