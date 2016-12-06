# ifndef TEST_UTILS_H
# define TEST_UTILS_H

# include <cmath>
# include <correlations/correlation.H>

template <class Unit1, class Unit2>
inline bool are_equal(const Quantity<Unit1> & q1, const Quantity<Unit1> & q2,
		       double epsilon = 1e-3)
{
  return fabs(q1 - q2) <= epsilon;
}

# endif

			     
