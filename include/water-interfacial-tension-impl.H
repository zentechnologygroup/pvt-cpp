# ifndef WATER_INTERFACIAL_TENSION_IMPL_H
# define WATER_INTERFACIAL_TENSION_IMPL_H

# include <water-interfacial-tension.H>


inline Quantity<dynes_cm>
SgwJenningsNewman::impl(const Quantity<Fahrenheit> & t,
			const Quantity<psia> & p) const
{
  const double a = 79.1618 - 0.118978*t.raw();
  const double b = -5.28473e-3 + 9.87913e-6*t.raw();
  const double c = (2.33814 - 4.57194e-4*t.raw() - 7.52678e-6*pow2(t)) * 1e-7;
        
  const double sgw = a + b*p.raw() + c*pow2(p);

  return Quantity<dynes_cm>(sgw);
}




 # endif // WATER_INTERFACIAL_TENSION_IMPL_H
