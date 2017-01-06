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
      throw domain_error(s.str());
    }
}

inline Quantity<RB_STB>
BoaMcCain::impl(const Quantity<RB_STB> & bobp,
		const Quantity<psia> & p,
		const Quantity<psia> & pb,
		const Quantity<psia_1> & co) const
{
  const double bwa = bobp.raw() * exp(co.raw() * (pb.raw() - p.raw()));
  return Quantity<RB_STB>(bwa);
}

void BoaDeGhetto::precondition(const Quantity<psia> & p,
			       const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "BoaDeGhetto::impl: pressure " << p << " is not greater than pb " << pb;
      throw domain_error(s.str());
    }   
}

inline Quantity<RB_STB>
BoaDeGhetto::impl(const Quantity<RB_STB> & bobp,
		  const Quantity<Sgg> & yg,
		  const Quantity<Api> & api,
		  const Quantity<SCF_STB> & rsb,
		  const Quantity<Fahrenheit> & t,
		  const Quantity<Fahrenheit> & tsep,
		  const Quantity<psia> & p,
		  const Quantity<psia> & pb,
		  const Quantity<psia> & psep) const
{
  auto co = CoaDeGhetto::get_instance().impl(yg, api, rsb, t, tsep, p, psep);
  return BoaMcCain::get_instance().impl(bobp, p, pb, co);
}

void BoaHanafy::precondition(const Quantity<psia> & p,
			     const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      cout << fabs(p.raw() - pb.raw()) << " " << " " << (p < pb) << endl;
      ostringstream s;
      s << "BoaHanafy::impl: pressure " << p << " is not greater than pb " << pb;
      throw domain_error(s.str());
    }  
}

inline Quantity<RB_STB> 
BoaHanafy::impl(const Quantity<RB_STB> & bobp,
		const Quantity<psia> & p,
		const Quantity<psia> & pb) const
{
  const double pob = 1 / (2.366 - 1.358/bobp.raw()); // Bubble point oil density
  const double co = 1e-6 * exp(2.582/pob - 0.990);
  return BoaMcCain::get_instance().impl(bobp, p, pb, co);
}

void BoaKartoatmodjo::precondition(const Quantity<psia> & p,
				   const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "BoaKartoatmodjo::impl: pressure " << p
	<< " is not greater than pb " << pb;
      throw domain_error(s.str());
    }  
}

inline Quantity<RB_STB>
BoaKartoatmodjo::impl(const Quantity<RB_STB> & bobp,
		      const Quantity<Sgg> & yg,
		      const Quantity<Sg_do> & yo,
		      const Quantity<SCF_STB> & rsb,
		      const Quantity<Fahrenheit> & t,
		      const Quantity<Fahrenheit> & tsep,
		      const Quantity<psia> & p,
		      const Quantity<psia> & pb,
		      const Quantity<psia> & psep) const
{
  auto co = CoaKartoatmodjoSchmidt::get_instance().impl(yg, yo, rsb, t, tsep,
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
      throw domain_error(s.str());
    }
}


inline Quantity<RB_STB>
BoaPetroskyFarshad::impl(const Quantity<RB_STB> & bobp,
			 const Quantity<Sgg> & yg,
			 const Quantity<Sg_do> & yo,
			 const Quantity<SCF_STB> & rsb, 
			 const Quantity<Fahrenheit> & t,
			 const Quantity<psia> & p,
			 const Quantity<psia> & pb) const
{
  auto co = CoaPetroskyFarshad::get_instance().impl(yg, yo, rsb, t, p);
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
      throw domain_error(s.str());
    }
}

inline Quantity<RB_STB>
BoaVasquezBeggs::impl(const Quantity<RB_STB> & bobp, 
		      const Quantity<Sgg> & yg,
		      const Quantity<Api> & api,
		      const Quantity<SCF_STB> & rsb,
		      const Quantity<Fahrenheit> & t,
		      const Quantity<Fahrenheit> & tsep,
		      const Quantity<psia> & p,
		      const Quantity<psia> & pb,
		      const Quantity<psia> & psep) const
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
      throw domain_error(s.str());
    }
}

inline Quantity<RB_STB>
BoaPerezML::impl(const Quantity<RB_STB> & bobp,
		 const Quantity<Sgg> & yg,
		 const Quantity<Sg_do> & yo,
		 const Quantity<SCF_STB> & rsb,
		 const Quantity<Fahrenheit> & t,
		 const Quantity<psia> & p,
		 const Quantity<psia> & pb) const
{
  auto co = CoaPerezML::get_instance().impl(yg, yo, rsb, t, p, pb);
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
      throw domain_error(s.str());
    }
}

inline Quantity<RB_STB>
BoaMillanArcia::impl(const Quantity<RB_STB> & bobp,
		     const Quantity<Api> & api,
		     const Quantity<SCF_STB> & rsb,
		     const Quantity<Fahrenheit> & t,
		     const Quantity<psia> & p,
		     const Quantity<psia> & pb) const 
{
  auto co = CoaMillanArcia::get_instance().impl(api, rsb, t, p, pb);
  return BoaMcCain::get_instance().impl(bobp, p, pb, co);
}



 # endif // UNDERSATURATED_OIL_FORMATION_VOLUME_FACTOR_IMPL_H
