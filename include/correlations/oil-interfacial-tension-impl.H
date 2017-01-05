
# ifndef OIL_INTERFACIAL_TENSION_IMPL_H
# define OIL_INTERFACIAL_TENSION_IMPL_H

# include <correlations/oil-interfacial-tension.H>

inline double
SgoBakerSwerdloff::impl(const double & t,
			const double & api,
			const double & p) const
{
  // Dead oil interfacial tension measured at temperatures of 68 and 100 °F
  const double sgo68 = 39 - 0.2571*api;
  const double sgo100 = 37.5 - 0.2571*api;  
  double sgoT = 0;
  
  if (t <= 68)
    sgoT = sgo68;
  else if (t >= 100)
    sgoT = sgo100;
  else
    // Linear interpolation between the values obtained at 68 and 100 °F
    sgoT = sgo68 - (t - 68)*(sgo68 - sgo100)/32.0;
            
  // Effect of dissolved gas on the dead oil interfacial tension
  
  const double c = exp(-8.6306e-4 * p);
      
  const double sgop = c*sgoT; // Interfacial tension at any pressure
        
  return sgop;
}



# endif // OIL_INTERFACIAL_TENSION_IMPL_H
