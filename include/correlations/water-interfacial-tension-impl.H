# ifndef WATER_INTERFACIAL_TENSION_IMPL_H
# define WATER_INTERFACIAL_TENSION_IMPL_H

# include <correlations/water-interfacial-tension.H>


inline double
SgwJenningsNewman::impl(const double & t,
			const double & p) const
{
  const double a = 79.1618 - 0.118978*t;
  const double b = -5.28473e-3 + 9.87913e-6*t;
  const double c = (2.33814 - 4.57194e-4*t - 7.52678e-6*t*t) * 1e-7;
        
  const double sgw = a + b*p + c*p*p;

  return sgw;
}




 # endif // WATER_INTERFACIAL_TENSION_IMPL_H
