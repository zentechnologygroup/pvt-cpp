# ifndef UNDERSATURATED_OIL_COMPRESSIBILITY_IMPL_H
# define UNDERSATURATED_OIL_COMPRESSIBILITY_IMPL_H

# include <correlations/undersaturated-oil-compressibility.H>

inline double
CoaDeGhetto::impl(const double & yg,
		  const double & api,
		  const double & rsb,
		  const double & t,
		  const double & tsep,
		  const double & p,
		  const double & psep) noexcept
{
  const double c = 1 + 0.5912*api*tsep*log10(psep/114.7)*1e-4;
        
  double co = 0;

  const double ygcorr = c*yg; // Gas specific gravity correction
			      // considering a separator pressure of
			      // 114.7 psia 

  if (api <= 10) // Extra-heavy oil  
    co = (-889.6 + 3.1374*rsb + 20*t - 627.3*ygcorr - 81.4476*api) / (1e5*p);
  else
    co = (-2841.8 + 2.9646*rsb + 25.5439*t - 1230.5*ygcorr + 41.91*api) /
      (1e5*p);

  return co;
}

inline double
CoaHanafy::impl(const double & rsb) noexcept
{
  const double bob = 0.0006*rsb + 1.079; // Bubble point oil volume factor
  const double pob = 1 / (2.366 - 1.358/bob); // Bubble point oil density
  const double co = 1e-6 * exp(2.582/pob - 0.990);

  return co;
}

inline double
CoaKartoatmodjoSchmidt::impl(const double & yg,
			    const double & api, 
			    const double & rsb,
			    const double & t,
			    const double & tsep,
			    const double & p,
			    const double & psep) noexcept
{
  const double c =
    1 + 0.1595 * pow(api, 0.4078) * pow(tsep, -0.2466) * log10(psep/114.7);
        
  const double ygcorr = c*yg; // Gas specific gravity correction
			      // (considering the standardized
			      // separator pressure: Psep=100 psig)

  const double a = 0.83415 + 0.5002 * log10(rsb) + 0.3613 * log10(api) +
    0.7606 * log10(t) - 0.35505 * log10(ygcorr);
  const double co = pow(10.0, a) / (p * 1e6);

  return co;
}

inline double
CoaPetroskyFarshad::impl(const double & yg,
			const double & api,
			const double & rsb,
			const double & t,
			const double & p) noexcept
{
  return 1.705e-7 * pow(rsb, 0.69357) * pow(yg, 0.1885) *
    pow(api, 0.3272) * pow(t, 0.6729) * pow(p, -0.5906);
}

inline double
CoaVasquezBeggs::impl(const double & yg,
		     const double & api,
		     const double & rsb,
		     const double & t,
		     const double & tsep,
		     const double & p,
		     const double & psep) noexcept
{
  // Gas gravity that would result from separator conditions of 100
  // psig (approximately 114.7 psia) 
  const double ygs = yg * (1. + 5.912e-5 * api * tsep * log10(psep/114.7));
            
  return (-1433 + 5*rsb + 17.2*t - 1180*ygs + 12.61*api) / (1e5*p);
}

void CoaPerezML::precondition(const Quantity<psia> & p,
			      const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "CoaPerezML::impl: pressure " << p << " is not greater than pb "
	<< pb;
      ALEPHTHROW(WrongValueUndersaturatedCondition, s.str());
    }
}

inline double
CoaPerezML::impl(const double & yg,
		const double & api,
		const double & rsb,
		const double & t,
		const double & p,
		const double & pb) noexcept
{
  const double cob = 2.856e-7 * pow(rsb, 0.69357) * pow(yg, 0.1885) *
    pow(api, 0.3272) * pow(t, 0.6729) * pow(pb, -0.5906);
  
  const double co = cob * sqrt(pb/p);

  return co;
}

void CoaMillanArcia::precondition(const Quantity<psia> & p,
				  const Quantity<psia> & pb) const
{
  if (not (p > pb))
    {
      ostringstream s;
      s << "CoaMillanArcia::impl: pressure " << p
	<< " is not greater than pb " << pb;
      ALEPHTHROW(WrongValueUndersaturatedCondition, s.str());
    }
}

inline double
CoaMillanArcia::impl(const double & api,
		    const double & rsb,
		    const double & t,
		    const double & p,
		    const double & pb) noexcept
{
  const double cob = 2.075883e-6 * pow(api, 0.5307) * pow(t, -0.1143) *
    exp(2.0523e-4*pb + 4.0568e-4*rsb);

  return 0.8557 * cob * exp(-0.00143 * p/pb);
}

# endif // UNDERSATURATED_OIL_COMPRESSIBILITY_IMPL_H
