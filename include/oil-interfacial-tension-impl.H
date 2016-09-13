
# ifndef OIL_INTERFACIAL_TENSION_IMPL_H
# define OIL_INTERFACIAL_TENSION_IMPL_H

# include <oil-interfacial-tension.H>

inline Quantity<dynes_cm>
SgoBakerSwerdloff::impl(const Quantity<Fahrenheit> & t,
			const Quantity<Api> & api,
			const Quantity<psia> & p) const
{

  // Dead oil interfacial tension measured at temperatures of 68 and 100 °F
  const double sgo68 = 39 - 0.2571*api.raw();
  const double sgo100 = 37.5 - 0.2571*api.raw();  
  double sgoT = 0;
  
  if (t <= 68)
    sgoT = sgo68;
  else if (t >= 100)
    sgoT = sgo100;
  else
    // Linear interpolation between the values obtained at 68 and 100 °F
    sgoT = sgo68 - (t.raw() - 68)*(sgo68 - sgo100)/32;
            
  // Effect of dissolved gas on the dead oil interfacial tension
  const double c = 1 - 0.024*pow(p, 0.45);
        
  const double sgop = c*sgoT; // Interfacial tension at any pressure
        
  return Quantity<dynes_cm>(sgop);    
}



# endif // OIL_INTERFACIAL_TENSION_IMPL_H
