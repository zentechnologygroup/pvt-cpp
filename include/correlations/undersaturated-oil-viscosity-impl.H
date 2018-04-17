
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
      ALEPHTHROW(WrongValueUndersaturatedCondition, s.str());
    }
}


inline double
UoaKartoatmodjoSchmidt::impl(const double & uobp,
			     const double & pb,
			     const double & p) noexcept
{
  const double p_pb = p - pb;
  double uoa = 1.00081*uobp +
    0.001127*p_pb*(-0.006517*pow(uobp, 1.8148) + 0.038*pow(uobp,  1.59));
  
  uoa = max(0.0, uoa);
  return uoa;
}

void UoaDeGhettoEtAl::precondition(const Quantity<psia> & p,
				   const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "UoaDeGhettoEtAl::impl: pressure " << p
	<< " is not greater than pb " << pb;
      ALEPHTHROW(WrongValueUndersaturatedCondition, s.str());
    }
}

inline double
UoaDeGhettoEtAl::impl(const double & uobp,
		      const double & p,
		      const double & pb,
		      const double & uod,
		      const double & api) noexcept
{
  double uoa = 0;
  if (api <= 10) // Extra-heavy oil
    {                  
      constexpr double k = 0.006456542290346556; // (10 ** -2.19)
      const double n = k * pow(uod, 1.055) * pow(pb, 0.3132);
      const double d = pow(10, 0.0099*api);
      uoa = uobp - (1 - p/pb) * (n/d);
    }
  else
    uoa = 0.9886*uobp + 0.002763*(p - pb) *
      (-0.01153 * pow(uobp, 1.7933) + 0.0316 * pow(uobp, 1.5939));

  uoa = max(0.0, uoa);
  return uoa;
}

void UoaBeal::precondition(const Quantity<psia> & p,
			   const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "UoaBeal::impl: pressure " << p << " is not greater than pb " << pb;
      ALEPHTHROW(WrongValueUndersaturatedCondition, s.str());
    }
}

inline double
UoaBeal::impl(const double & uobp,
	      const double & p,
	      const double & pb) noexcept
{
   double uoa = uobp + 0.001*(p - pb) *
    (0.024*pow(uobp, 1.6) + 0.038 * pow(uobp, 0.56));

  uoa = max(0.0, uoa);
  return uoa;
}

void UoaVasquezBeggs::precondition(const Quantity<psia> & p,
				   const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "UoaVasquezBeggs::impl: pressure " << p
	<< " is not greater than pb " << pb;
      ALEPHTHROW(WrongValueUndersaturatedCondition, s.str());
    }
}

inline double
UoaVasquezBeggs::impl(const double & uobp,
		      const double & p,
		      const double & pb) noexcept
{
  constexpr double c1 = 2.6;
  constexpr double c2 = 1.187;
  constexpr double c3 = -11.513;
  constexpr double c4 = -8.98e-5;

  const double m = c1 * pow(p, c2) * exp(c3 + c4*p);

  double uoa = uobp * pow(p/pb, m);

  uoa = max(0.0, uoa);
  return uoa;
}

void UoaKhan::precondition(const Quantity<psia> & p,
			   const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "UoaKhan::impl: pressure " << p << " is not greater than pb " << pb;
      ALEPHTHROW(WrongValueUndersaturatedCondition, s.str());
    }
}

inline double
UoaKhan::impl(const double & uobp,
	      const double & p,
	      const double & pb) noexcept
{
  double uoa = uobp * exp(9.6e-5*(p - pb));
  uoa = max(0.0, uoa);
  return uoa;
}

void UoaPetroskyFarshad::precondition(const Quantity<psia> & p,
				      const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "UoaPetroskyFarshad::impl: pressure " << p
	<< " is not greater than pb " << pb;
      ALEPHTHROW(WrongValueUndersaturatedCondition, s.str());
    }
}

inline double
UoaPetroskyFarshad::impl(const double & uobp,
			 const double & p,
			 const double & pb) noexcept
{
  const double log_uobp = log10(uobp);
  const double log_uobp_2 = log_uobp*log_uobp;
  const double a = -1.0146 + 1.3322*log_uobp - 0.4876*log_uobp_2 -
    1.15036*log_uobp_2*log_uobp;
  
  double uoa = uobp + 1.3449e-3*(p - pb)*pow(10, a);

  uoa = max(0.0, uoa);
  return uoa;
}

void UoaAbedini::precondition(const Quantity<psia> & p,
			      const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "UoaAbedini::impl: pressure " << p << " is not greater than pb " << pb;
      ALEPHTHROW(WrongValueUndersaturatedCondition, s.str());
    }
}

inline double
UoaAbedini::impl(const double & uobp,
		 const double & p,
		 const double & pb) noexcept
{
  double uoa = uobp +
    0.001*(p - pb)*((0.05601*pow(uobp, 1.45198)) +
		    (0.47557 * pow(uobp, 0.35997)) +
		    -0.2257*pow(uobp, 0.86389) +
		    -0.29598*pow(pb, -0.41866) +
		    -0.07734*pow(pb, -0.29981) +
		    -0.42436*pow(pb, -0.1946) +
		    -1.64149*pow(pb, -0.31339));
  uoa = max(0.0, uoa);
  return uoa;
}

void UoaPerezML::precondition(const Quantity<psia> & p,
			      const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "UoaPerezML::impl: pressure " << p << " is not greater than pb " << pb;
      ALEPHTHROW(WrongValueUndersaturatedCondition, s.str());
    }
}

inline double
UoaPerezML::impl(const double & uobp,
		 const double & p,
		 const double & pb) noexcept
{
  double uoa = uobp * (1 + 3.181e-4 * (p - pb));
  uoa = max(0.0, uoa);
  return uoa;
}

void UoaDindorukChristman::precondition(const Quantity<psia> & p,
					const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "UoaDindorukChristman::impl: pressure " << p
	<< " is not greater than pb " << pb;
      ALEPHTHROW(WrongValueUndersaturatedCondition, s.str());
    }

   if (not (202 <= (p - pb) and (p - pb) <= 10140))
     {
      ostringstream s;
      s << "UoaDindorukChristman::impl: P - Pb value " << p - pb
	<< " is out of the development range";
      ALEPHTHROW(WrongCombinationInputValues, s.str());
     }
}


inline double
UoaDindorukChristman::impl(const double & uobp,
			   const double & p,
			   const double & pb,
			   const double & rsb) noexcept
{
  constexpr double a1 = 0.776644115;
  constexpr double a2 = 0.987658646;
  constexpr double a3 = -0.190564677;
  constexpr double a4 = 0.009147711;
  constexpr double a5 = -0.000019111;
  constexpr double a6 = 0.000063340;

  const double p_pb = p - pb;
  
  const double a = a1 + a2*log10(uobp) + a3*log10(rsb) + a4*uobp*log10(rsb) +
    a5*p_pb;
        
  double uoa = uobp + a6*p_pb*pow(10, a);
        
  uoa = max(0.0, uoa);
  return uoa;
}



# endif // UNDERSATURATED_OIL_VISCOSITY_IMPL_H
