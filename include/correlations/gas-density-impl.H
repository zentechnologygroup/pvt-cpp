
# ifndef GAS_DENSITY_IMPL_H
# define GAS_DENSITY_IMPL_H

# include <correlations/gas-density.H>

inline Quantity<Lb_ft3>
Pg::impl(const Quantity<Sgg> & yg,
	 const Quantity<psia> & pb,
	 const Quantity<Rankine> & t,
	 const Quantity<psia> & p,
	 const Quantity<Zfactor> & z) const
{
  if (p > pb)
    {
      ostringstream s;
      s << "In Pg::impl: parameter p = " << p
	<< " is greater than parameter pb = " << pb;
      throw domain_error(s.str());
    }

  const double pg = (2.70*p.raw()*yg.raw()) / (z.raw()*t.raw());

  return Quantity<Lb_ft3>(pg);
}


# endif // GAS_DENSITY_IMPL_H
