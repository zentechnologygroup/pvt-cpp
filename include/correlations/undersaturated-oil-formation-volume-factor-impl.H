# ifndef UNDERSATURATED_OIL_FORMATION_VOLUME_FACTOR_IMPL_H
# define UNDERSATURATED_OIL_FORMATION_VOLUME_FACTOR_IMPL_H

# include <correlations/undersaturated-oil-formation-volume-factor.H>
# include "correlations/undersaturated-oil-compressibility.H"

void BoaMcCain::precondition(const Quantity<psia> & p,
			       const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "BoaMcCain::impl: pressure " << p << " is not greater than pb " << pb;
      ALEPHTHROW(WrongValueUndersaturatedCondition, s.str());
    }
}

inline double
BoaMcCain::impl(const double & bobp,
		const double & p,
		const double & pb,
		const double & coa) noexcept
{
  return bobp * exp(coa * (pb - p));
}

void BoaDeGhetto::precondition(const Quantity<psia> & p,
			       const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "BoaDeGhetto::impl: pressure " << p << " is not greater than pb " << pb;
      ALEPHTHROW(WrongValueUndersaturatedCondition, s.str());
    }   
}

inline double
BoaDeGhetto::impl(const double & bobp,
		  const double & yg,
		  const double & api,
		  const double & rsb,
		  const double & t,
		  const double & tsep,
		  const double & p,
		  const double & pb,
		  const double & psep) noexcept
{
  auto co = CoaDeGhetto::get_instance().impl(yg, api, rsb, t, tsep, p, psep);
  return BoaMcCain::get_instance().impl(bobp, p, pb, co);
}

void BoaHanafy::precondition(const Quantity<psia> & p,
			     const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "BoaHanafy::impl: pressure " << p << " is not greater than pb " << pb;
      ALEPHTHROW(WrongValueUndersaturatedCondition, s.str());
    }  
}

inline double 
BoaHanafy::impl(const double & bobp,
		const double & p,
		const double & pb) noexcept
{
  const double pob = 1 / (2.366 - 1.358/bobp); // Bubble point oil density
  const double co = 1e-6 * exp(2.582/pob - 0.990);
  return BoaMcCain::get_instance().impl(bobp, p, pb, co);
}

void BoaKartoatmodjoSchmidt::precondition(const Quantity<psia> & p,
				   const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "BoaKartoatmodjoSchmidt::impl: pressure " << p
	<< " is not greater than pb " << pb;
      ALEPHTHROW(WrongValueUndersaturatedCondition, s.str());
    }  
}

inline double
BoaKartoatmodjoSchmidt::impl(const double & bobp,
		      const double & yg,
		      const double & yo,
		      const double & rsb,
		      const double & t,
		      const double & tsep,
		      const double & p,
		      const double & pb,
		      const double & psep) noexcept
{
  const double api = unit_convert<Sg_do, Api>(yo);
  auto co = CoaKartoatmodjoSchmidt::get_instance().impl(yg, api, rsb, t, tsep,
							p, psep);
  return BoaMcCain::get_instance().impl(bobp, p, pb, co);
}

void BoaPetroskyFarshad::precondition(const Quantity<psia> & p,
				      const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "BoaPetroskyFarshad::impl: pressure " << p
	<< " is not greater than pb " << pb;
      ALEPHTHROW(WrongValueUndersaturatedCondition, s.str());
    }
}

inline double
BoaPetroskyFarshad::impl(const double & bobp,
			 const double & yg,
			 const double & yo,
			 const double & rsb, 
			 const double & t,
			 const double & p,
			 const double & pb) noexcept
{
  const double api = unit_convert<Sg_do, Api>(yo);
  auto co = CoaPetroskyFarshad::get_instance().impl(yg, api, rsb, t, p);
  return BoaMcCain::get_instance().impl(bobp, p, pb, co);
}

void BoaVasquezBeggs::precondition(const Quantity<psia> & p,
				   const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "BoaVasquezBeggs::impl: pressure " << p
	<< " is not greater than pb " << pb;
      ALEPHTHROW(WrongValueUndersaturatedCondition, s.str());
    }
}

inline double
BoaVasquezBeggs::impl(const double & bobp, 
		      const double & yg,
		      const double & api,
		      const double & rsb,
		      const double & t,
		      const double & tsep,
		      const double & p,
		      const double & pb,
		      const double & psep) noexcept
{
  auto co = CoaVasquezBeggs::get_instance().impl(yg, api, rsb, t, tsep, p, psep);
  return BoaMcCain::get_instance().impl(bobp, p, pb, co);
}

void BoaPerezML::precondition(const Quantity<psia> & p,
			      const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "BoaPerezML::impl: pressure " << p << " is not greater than pb " << pb;
      ALEPHTHROW(WrongValueUndersaturatedCondition, s.str());
    }
}

inline double
BoaPerezML::impl(const double & bobp,
		 const double & yg,
		 const double & yo,
		 const double & rsb,
		 const double & t,
		 const double & p,
		 const double & pb) noexcept
{
  const double api = unit_convert<Sg_do, Api>(yo);
  auto co = CoaPerezML::get_instance().impl(yg, api, rsb, t, p, pb);
  return BoaMcCain::get_instance().impl(bobp, p, pb, co);
}

void BoaMillanArcia::precondition(const Quantity<psia> & p,
				  const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "BoaMillanArcia::impl: pressure " << p
	<< " is not greater than pb " << pb;
      ALEPHTHROW(WrongValueUndersaturatedCondition, s.str());
    }
}

inline double
BoaMillanArcia::impl(const double & bobp,
		     const double & api,
		     const double & rsb,
		     const double & t,
		     const double & p,
		     const double & pb) noexcept
{
  auto co = CoaMillanArcia::get_instance().impl(api, rsb, t, p, pb);
  return BoaMcCain::get_instance().impl(bobp, p, pb, co);
}



 # endif // UNDERSATURATED_OIL_FORMATION_VOLUME_FACTOR_IMPL_H
