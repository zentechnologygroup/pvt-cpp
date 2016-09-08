
# ifndef UNDERSATURATED_OIL_VISCOSITY_IMPL_H
# define UNDERSATURATED_OIL_VISCOSITY_IMPL_H

# include <undersaturated-oil-viscosity.H>

inline Quantity<CP>
UoaKartoatmodjoSchmidt::impl(const Quantity<CP> & uob,
			     const Quantity<psia> & pb,
			     const Quantity<psia> & p) const
{
  const double p_pb = p.raw() - pb.raw();
  const double uoa = 1.00081*uob.raw() +
    0.001127*p_pb*(-0.006517*pow(uob, 1.8148) + 0.038*pow(uob,  1.59));

  return Quantity<CP>(uoa);
}

inline Quantity<CP>
UoaDeGhettoEtAl::impl(const Quantity<CP> & uob,
		      const Quantity<psia> & p,
		      const Quantity<psia> & pb,
		      const Quantity<CP> & uod,
		      const Quantity<Api> & api) const
{
  double uoa = 0;
  if (api <= 10) // Extra-heavy oil
    {                  // 10^-2.19
      constexpr double k = 0.006456542290346556; // (10 ** -2.19)
      const double n = k * pow(uod, 1.055) * pow(pb, 0.3132);
      const double d = pow(10, 0.0099*api.raw());
      uoa = uob.raw() - (1 - (p.raw()/pb.ra())) * (n/d);
    }
  else
    {      
      uoa = 0.9886*uob + 0.002763*(p.raw() - pb.raw()) * (-0.01153 *

							  pow(uob, 1.7933) + 0.0316 * pow((uob, 1.5939) )
        
}



# endif // UNDERSATURATED_OIL_VISCOSITY_IMPL_H
