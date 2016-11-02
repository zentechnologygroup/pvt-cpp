
# ifndef OIL_DENSITY_IMPL_H
# define OIL_DENSITY_IMPL_H

# include <correlations/oil-density.H>

inline Quantity<Lb_ft3>
PoBradley::impl(const Quantity<Sgg> & yg,
		const Quantity<SCF_STB> & rs,
		const Quantity<RB_STB> & bo,
		const Quantity<Sg_do> & yo,
		const Quantity<psia> & p,
		const Quantity<psia> & pb,
		const Quantity<psia_1> & co) const
{
  double po = 0;
  if (p < pb)
    po = (350*yo.raw() + 0.0764*yg.raw()*rs.raw())/(5.615*bo.raw());
  else
    {
      const double raw_p = p.raw();
      const double raw_pb = pb.raw();
      const double raw_co = co.raw();
      const double bob = bo.raw() / exp(raw_co * (raw_pb - raw_p));
      const double pob = (350*yo.raw() + 0.0764*yg.raw()*rs.raw())/(5.615*bob);
      po = pob * exp(raw_co * (raw_p - raw_pb));
    }

  return Quantity<Lb_ft3>(po);
}


# endif // OIL_DENSITY_IMPL_H
