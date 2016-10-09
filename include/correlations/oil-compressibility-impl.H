# ifndef OIL_COMPRESSIBILITY_IMPL_H
# define OIL_COMPRESSIBILITY_IMPL_H

# include "oil-compressibility.H"

inline Quantity<psia_1>
CoMcCainEtAl::impl(const Quantity<Api> & api,
		   const Quantity<SCF_STB> & rsb,
		   const Quantity<Rankine> & t,
		   const Quantity<psia> & p,
		   const Quantity<psia> & pb) const
{
  const double co = exp(-7.573 - 1.450*log(p) - 0.383*log(pb) + 1.402*log(t) +
			0.256*log(api) + 0.449*log(rsb));
  return Quantity<psia_1>(co);
}


inline Quantity<psia_1>
CoDeGhetto::impl(const Quantity<Sgg> & yg,
		 const Quantity<Api> & api,
		 const Quantity<SCF_STB> & rsb,
		 const Quantity<Fahrenheit> & t,
		 const Quantity<Fahrenheit> & tsep,
		 const Quantity<psia> & p,
		 const Quantity<psia> & pb,
		 const Quantity<psia> & psep) const
{
  const double raw_api = api.get_value();

  const double c = 1 + 0.5912*raw_api*tsep.get_value()*log10(psep/114.7)*1e-4;
        
  double co = 0;
  if (p <= pb) // Saturated oil ==> use McCain et al. correlation
    return CoMcCainEtAl::get_instance () (api, rsb, t, p, pb);
  
  // Undersaturated oil 

  const double ygcorr = c*yg.raw(); // Gas specific gravity correction
				    // considering a separator
				    // pressure of 114.7 psia 

  if (api <= 10) // Extra-heavy oil  
    co = (-889.6 + 3.1374*rsb.raw() + 20*t.raw() -
	  627.3*ygcorr - 81.4476*api.raw()) / (1e5*p.raw());
  else
    co = (-2841.8 + 2.9646*rsb.raw() + 25.5439*t.raw() -
	  1230.5*ygcorr + 41.91*api.raw()) / (1e5*p.raw());

  return Quantity<psia_1>(co);
}


inline Quantity<psia_1>
CoHanafy::impl(const Quantity<Api> & api,
	       const Quantity<SCF_STB> & rsb,
	       const Quantity<Fahrenheit> & t,
	       const Quantity<psia> & p,
	       const Quantity<psia> & pb) const
{
  if (p <= pb) // Saturated oil - McCain et al. correlation
    return CoMcCainEtAl::get_instance()(api, rsb, t, p, pb);
            
      // Undersaturated oil    
        
  const double bob = 0.0006*rsb.raw() + 1.079; // Bubble point oil volume factor
  const double pob = 1 / (2.366 - 1.358/bob); // Bubble point oil density
  const double co = 1e-6 * exp(2.582/pob - 0.990);

  return Quantity<psia_1>(co);
}


inline Quantity<psia_1>
CoKartoatmodjoSchmidt::impl(const Quantity<Sgg> & yg,
			    const Quantity<Api> & api,
			    const Quantity<SCF_STB> & rsb,
			    const Quantity<Fahrenheit> & t,
			    const Quantity<Fahrenheit> & tsep,
			    const Quantity<psia> & p,
			    const Quantity<psia> & pb,
			    const Quantity<psia> & psep) const
{

  if (p <= pb) // Saturated oil - McCain et al. correlation
    return CoMcCainEtAl::get_instance()(api, rsb, t, p, pb);

  const double c =
    1 + 0.1595 * pow(api, 0.4078) * pow(tsep, -0.2466) * log10(psep/114.7);
        
  const double ygcorr = c*yg.raw(); // Gas specific gravity correction
				    // (considering the standardized
				    // separator pressure: Psep=100 psig) 

  const double co = 6.8257e-6 * pow(rsb, 0.5002) * p.raw() *
    pow(t, 0.76606) * pow(ygcorr, 0.35505);

  return Quantity<psia_1>(co);
}


inline Quantity<psia_1>
CoPetroskyFarshad::impl(const Quantity<Sgg> & yg,
			const Quantity<Api> & api,
			const Quantity<SCF_STB> & rsb,
			const Quantity<Fahrenheit> & t,
			const Quantity<psia> & p,
			const Quantity<psia> & pb) const
{
  if (p <= pb) // Saturated oil - McCain et al. correlation
    return CoMcCainEtAl::get_instance()(api, rsb, t, p, pb);
            
  const double co = 1.705e-7 * pow(rsb, 0.69357) * pow(yg, 0.1885) *
    pow(api, 0.3272) * pow(t, 0.6729) * pow(p, -0.5906);

  return Quantity<psia_1>(co);
}

inline Quantity<psia_1>
CoVasquezBeggs::impl(const Quantity<Sgg> & yg,
		     const Quantity<Api> & api,
		     const Quantity<SCF_STB> & rsb,
		     const Quantity<Fahrenheit> & t,
		     const Quantity<Fahrenheit> & tsep,
		     const Quantity<psia> & p,
		     const Quantity<psia> & pb,
		     const Quantity<psia> & psep) const
{
  if (p <= pb) // Saturated oil - McCain et al. correlation
    return CoMcCainEtAl::get_instance()(api, rsb, t, p, pb); 

  // Gas gravity that would result from separator conditions of 100
  // psig (approximately 114.7 psia) 
  const double ygs =
    yg.raw() * (1. + 5.912e-5 * api.raw() * tsep.raw() * log10(psep/114.7));
            
  const double co = (-1433 + 5*rsb.raw() + 17.2*t.raw() - 1180*ygs +
		     12.61*api.raw()) / (1e5*p.raw());
    
  return Quantity<psia_1>(co);
}



inline Quantity<psia_1>
CoPerezML::impl(const Quantity<Sgg> & yg,
		const Quantity<Api> & api,
		const Quantity<SCF_STB> & rsb,
		const Quantity<Fahrenheit> & t,
		const Quantity<psia> & p,
		const Quantity<psia> & pb) const
{
  if (p <= pb) // Saturated oil - McCain et al. correlation
    return CoMcCainEtAl::get_instance()(api, rsb, t, p, pb);
            
  const double cob = 2.856e-7 * pow(rsb, 0.69357) * pow(yg, 0.1885) *
    pow(api, 0.3272) * pow(t, 0.6729) * pow(pb, -0.5906);
  
  const double co = cob * sqrt(p/pb);

  return Quantity<psia_1>(co);
}

inline Quantity<psia_1>
CoMillanArcia::impl(const Quantity<Api> & api,
		    const Quantity<SCF_STB> & rsb,
		    const Quantity<Fahrenheit> & t,
		    const Quantity<psia> & p,
		    const Quantity<psia> & pb) const
{
  if (p <= pb) // Saturated oil - McCain et al. correlation
    return CoMcCainEtAl::get_instance()(api, rsb, t, p, pb);
            
  const double cob = 2.075883e-6 * pow(api, 0.5307) * pow(t, -0.1143) *
    exp(2.0523e-4*pb.raw() + 4.0568e-4*rsb.raw());

  const double co = 0.8557 * cob * exp(-0.00143 * p/pb);

  return Quantity<psia_1>(co);
}



# endif // OIL_COMPRESSIBILITY_IMPL_H



