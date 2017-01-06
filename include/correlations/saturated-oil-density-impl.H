# ifndef SATURATED_OIL_DENSITY_IMPL_H
# define SATURATED_OIL_DENSITY_IMPL_H

# include <correlations/saturated-oil-density.H>


Quantity<Lb_ft3>
PobBradley::impl(const Quantity<Sgg> & yg,
		 const Quantity<SCF_STB> & rs,
		 const Quantity<RB_STB> & bob,
		 const Quantity<Sg_do> & yo) const
{
  const double po = (350*yo.raw() + 0.0764*yg.raw()*rs.raw())/(5.615*bob.raw());

  return Quantity<Lb_ft3>(po);
}




 # endif // SATURATED_OIL_DENSITY_IMPL_H
