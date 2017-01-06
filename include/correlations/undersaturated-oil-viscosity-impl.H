
# ifndef UNDERSATURATED_OIL_VISCOSITY_IMPL_H
# define UNDERSATURATED_OIL_VISCOSITY_IMPL_H

# include <correlations/undersaturated-oil-viscosity.H>

void UoaKartoatmodjoSchmidt::precondition(const Quantity<psia> & p,
					  const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "UoaKartoatmodjoSchmidt::impl: pressure " <<
	p << " is not greater than pb " << pb;
      throw domain_error(s.str());
    }
}


inline Quantity<CP>
UoaKartoatmodjoSchmidt::impl(const Quantity<CP> & uobp,
			     const Quantity<psia> & pb,
			     const Quantity<psia> & p) const
{
  const double p_pb = p.raw() - pb.raw();
  const double uoa = 1.00081*uobp.raw() +
    0.001127*p_pb*(-0.006517*pow(uobp, 1.8148) + 0.038*pow(uobp,  1.59));
  return Quantity<CP>(uoa);
}

void UoaDeGhettoEtAl::precondition(const Quantity<psia> & p,
				   const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "UoaDeGhettoEtAl::impl: pressure " << p
	<< " is not greater than pb " << pb;
      throw domain_error(s.str());
    }
}

inline Quantity<CP>
UoaDeGhettoEtAl::impl(const Quantity<CP> & uobp,
		      const Quantity<psia> & p,
		      const Quantity<psia> & pb,
		      const Quantity<CP> & uod,
		      const Quantity<Api> & api) const
{
  double uoa = 0;
  if (api <= 10) // Extra-heavy oil
    {                  
      constexpr double k = 0.006456542290346556; // (10 ** -2.19)
      const double n = k * pow(uod, 1.055) * pow(pb, 0.3132);
      const double d = pow(10, 0.0099*api.raw());
      uoa = uobp.raw() - (1 - p.raw()/pb.raw()) * (n/d);
    }
  else
    uoa = 0.9886*uobp.raw() + 0.002763*(p.raw() - pb.raw()) *
      (-0.01153 * pow(uobp, 1.7933) + 0.0316 * pow(uobp, 1.5939));

  return Quantity<CP>(uoa);
}

void UoaBeal::precondition(const Quantity<psia> & p,
			   const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "UoaBeal::impl: pressure " << p << " is not greater than pb " << pb;
      throw domain_error(s.str());
    }
}

inline Quantity<CP>
UoaBeal::impl(const Quantity<CP> & uobp,
	      const Quantity<psia> & p,
	      const Quantity<psia> & pb) const
{
  const double uoa = uobp.raw() + 0.001*(p.raw() - pb.raw()) *
    (0.024*pow(uobp, 1.6) + 0.038 * pow(uobp, 0.56));
  return Quantity<CP>(uoa);
}

void UoaVasquezBeggs::precondition(const Quantity<psia> & p,
				   const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "UoaVasquezBeggs::impl: pressure " << p
	<< " is not greater than pb " << pb;
      throw domain_error(s.str());
    }
}

inline Quantity<CP>
UoaVasquezBeggs::impl(const Quantity<CP> & uobp,
		      const Quantity<psia> & p,
		      const Quantity<psia> & pb) const
{
  constexpr double c1 = 2.6;
  constexpr double c2 = 1.187;
  constexpr double c3 = -11.513;
  constexpr double c4 = -8.98e-5;

  const double raw_p = p.raw();
  const double raw_pb = pb.raw();
  const double m = c1 * pow(raw_p, c2) * exp(c3 + c4*raw_p);

  const double uoa = uobp.raw() * pow(raw_p/raw_pb, m);

  return Quantity<CP>(uoa);
}

void UoaKhan::precondition(const Quantity<psia> & p,
			   const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "UoaKhan::impl: pressure " << p << " is not greater than pb " << pb;
      throw domain_error(s.str());
    }
}

inline Quantity<CP>
UoaKhan::impl(const Quantity<CP> & uobp,
	      const Quantity<psia> & p,
	      const Quantity<psia> & pb) const
{
  const double uoa = uobp.raw() * exp(9.6e-5*(p.raw() - pb.raw()));
  return Quantity<CP>(uoa);
}

void UoaPetroskyFarshad::precondition(const Quantity<psia> & p,
				      const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "UoaPetroskyFarshad::impl: pressure " << p
	<< " is not greater than pb " << pb;
      throw domain_error(s.str());
    }
}

inline Quantity<CP>
UoaPetroskyFarshad::impl(const Quantity<CP> & uobp,
			 const Quantity<psia> & p,
			 const Quantity<psia> & pb) const
{
  const double log_uobp = log10(uobp);
  const double log_uobp_2 = log_uobp*log_uobp;
  const double a = -1.0146 + 1.3322*log_uobp - 0.4876*log_uobp_2 -
    1.15036*log_uobp_2*log_uobp;
  
  const double uoa = uobp.raw() + 1.3449e-3*(p.raw() - pb.raw())*pow(10, a);

  return Quantity<CP>(uoa);
}

void UoaAbedini::precondition(const Quantity<psia> & p,
			      const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "UoaAbedini::impl: pressure " << p << " is not greater than pb " << pb;
      throw domain_error(s.str());
    }
}

inline Quantity<CP>
UoaAbedini::impl(const Quantity<CP> & uobp,
		 const Quantity<psia> & p,
		 const Quantity<psia> & pb) const
{
  const double uoa = uobp.raw() +
    0.001*(p.raw() - pb.raw())*((0.05601*pow(uobp, 1.45198)) +
				(0.47557 * pow(uobp, 0.35997)) +
				-0.2257*pow(uobp, 0.86389) +
				-0.29598*pow(pb, -0.41866) +
				-0.07734*pow(pb, -0.29981) +
				-0.42436*pow(pb, -0.1946) +
				-1.64149*pow(pb, -0.31339));
  return Quantity<CP>(uoa);
}

void UoaPerezML::precondition(const Quantity<psia> & p,
			      const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "UoaPerezML::impl: pressure " << p << " is not greater than pb " << pb;
      throw domain_error(s.str());
    }
}

inline Quantity<CP>
UoaPerezML::impl(const Quantity<CP> & uobp,
		 const Quantity<psia> & p,
		 const Quantity<psia> & pb) const
{
  const double uoa = uobp.raw() * (1 + 3.181e-4 * (p.raw() - pb.raw()));
  return Quantity<CP>(uoa);
}

void UoaDindorukChristman::precondition(const Quantity<psia> & p,
					const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "UoaDindorukChristman::impl: pressure " << p
	<< " is not greater than pb " << pb;
      throw domain_error(s.str());
    }
}


inline Quantity<CP>
UoaDindorukChristman::impl(const Quantity<CP> & uobp,
			   const Quantity<psia> & p,
			   const Quantity<psia> & pb,
			   const Quantity<SCF_STB> & rsb) const
{
  constexpr double a1 = 0.776644115;
  constexpr double a2 = 0.987658646;
  constexpr double a3 = -0.190564677;
  constexpr double a4 = 0.009147711;
  constexpr double a5 = -0.000019111;
  constexpr double a6 = 0.000063340;

  const double p_pb = p.raw() - pb.raw();
  
  const double a = a1 + a2*log10(uobp) + a3*log10(rsb) + a4*uobp.raw()*log10(rsb) +
    a5*p_pb;
        
  const double uoa = uobp.raw() + a6*p_pb*pow(10, a);
        
  return Quantity<CP>(uoa);
}



# endif // UNDERSATURATED_OIL_VISCOSITY_IMPL_H
