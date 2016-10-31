# ifndef UNDERSATURATED_COMPRESSIBILITY_IMPL_H
# define UNDERSATURATED_COMPRESSIBILITY_IMPL_H

# include <correlations/undersaturated-compressibility.H>

inline Quantity<psia_1>
CoaDeGhetto::impl(const Quantity<Sgg> & yg,
		  const Quantity<Api> & api,
		  const Quantity<SCF_STB> & rsb,
		  const Quantity<Fahrenheit> & t,
		  const Quantity<Fahrenheit> & tsep,
		  const Quantity<psia> & p,
		  const Quantity<psia> & psep) const
{
  const double raw_api = api.get_value();

  const double c = 1 + 0.5912*raw_api*tsep.get_value()*log10(psep/114.7)*1e-4;
        
  double co = 0;

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
CoaHanafy::impl(const Quantity<SCF_STB> & rsb) const
{
  const double bob = 0.0006*rsb.raw() + 1.079; // Bubble point oil volume factor
  const double pob = 1 / (2.366 - 1.358/bob); // Bubble point oil density
  const double co = 1e-6 * exp(2.582/pob - 0.990);

  return Quantity<psia_1>(co);
}

inline Quantity<psia_1>
CoaKartoatmodjoSchmidt::impl(const Quantity<Sgg> & yg,
			    const Quantity<Api> & api, 
			    const Quantity<SCF_STB> & rsb,
			    const Quantity<Fahrenheit> & t,
			    const Quantity<Fahrenheit> & tsep,
			    const Quantity<psia> & p,
			    const Quantity<psia> & psep) const
{
  const double c =
    1 + 0.1595 * pow(api, 0.4078) * pow(tsep, -0.2466) * log10(psep/114.7);
        
  const double ygcorr = c*yg.raw(); // Gas specific gravity correction
				    // (considering the standardized
				    // separator pressure: Psep=100 psig) 

  const double co = 6.8257e-6 * pow(rsb, 0.5002) * p.raw() *
    pow(t, 0.76606) * pow(ygcorr, -0.35505);

  return Quantity<psia_1>(co);
}


inline Quantity<psia_1>
CoaPetroskyFarshad::impl(const Quantity<Sgg> & yg,
			const Quantity<Api> & api,
			const Quantity<SCF_STB> & rsb,
			const Quantity<Fahrenheit> & t,
			const Quantity<psia> & p) const
{
            
  const double co = 1.705e-7 * pow(rsb, 0.69357) * pow(yg, 0.1885) *
    pow(api, 0.3272) * pow(t, 0.6729) * pow(p, -0.5906);

  return Quantity<psia_1>(co);
}

inline Quantity<psia_1>
CoaVasquezBeggs::impl(const Quantity<Sgg> & yg,
		     const Quantity<Api> & api,
		     const Quantity<SCF_STB> & rsb,
		     const Quantity<Fahrenheit> & t,
		     const Quantity<Fahrenheit> & tsep,
		     const Quantity<psia> & p,
		     const Quantity<psia> & psep) const
{

  // Gas gravity that would result from separator conditions of 100
  // psig (approximately 114.7 psia) 
  const double ygs =
    yg.raw() * (1. + 5.912e-5 * api.raw() * tsep.raw() * log10(psep/114.7));
            
  const double co = (-1433 + 5*rsb.raw() + 17.2*t.raw() - 1180*ygs +
		     12.61*api.raw()) / (1e5*p.raw());
    
  return Quantity<psia_1>(co);
}



inline Quantity<psia_1>
CoaPerezML::impl(const Quantity<Sgg> & yg,
		const Quantity<Api> & api,
		const Quantity<SCF_STB> & rsb,
		const Quantity<Fahrenheit> & t,
		const Quantity<psia> & p,
		const Quantity<psia> & pb) const
{
            
  const double cob = 2.856e-7 * pow(rsb, 0.69357) * pow(yg, 0.1885) *
    pow(api, 0.3272) * pow(t, 0.6729) * pow(pb, -0.5906);
  
  const double co = cob * sqrt(p/pb);

  return Quantity<psia_1>(co);
}

inline Quantity<psia_1>
CoaMillanArcia::impl(const Quantity<Api> & api,
		    const Quantity<SCF_STB> & rsb,
		    const Quantity<Fahrenheit> & t,
		    const Quantity<psia> & p,
		    const Quantity<psia> & pb) const
{

  const double cob = 2.075883e-6 * pow(api, 0.5307) * pow(t, -0.1143) *
    exp(2.0523e-4*pb.raw() + 4.0568e-4*rsb.raw());

  const double co = 0.8557 * cob * exp(-0.00143 * p/pb);

  return Quantity<psia_1>(co);
}

# endif // UNDERSATURATED_COMPRESSIBILITY_IMPL_H
