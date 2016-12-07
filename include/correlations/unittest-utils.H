# ifndef UNITTEST_UTILS_H
# define UNITTEST_UTILS_H

# include <cmath>
# include <iostream>
# include <string>
# include <correlations/correlation.H>

using namespace std;

template <class CorrClass, typename ... Args>
VtlQuantity call_compute(Args... args)
{
  return CorrClass(args...).to_VtlQuantity();
}

// TODO: recibe correlación como template e imprime parámetros
// TODO: tal vez desde ruby se genera la impresión de la llamada
template <class Unit1, class Unit2>
inline void correlation_test(const string & name,
			     size_t csv_line,
			     const Quantity<Unit1> & result,
			     const Quantity<Unit2> & expected,
			     double epsilon = 1e-3)
{
  if (fabs(result.raw() - expected.raw()) <= epsilon)
    return;

  cout << "Test for correlation " << name << " csv line " << csv_line
       << " failed" << endl
       << "    Result   = " << result << endl
       << "    Expected = " << expected << endl
       << "    epsilon  = " << epsilon << endl
       << endl;
}


inline void __zen__test(bool pred, const char * exp_text,
			  const char * filename, const size_t linenum)
{
  if (pred)
    return;

  cout << "assert(" << exp_text << ") " << filename << " : " << linenum
       << " violated";
  abort();
}

# ifdef NDEBUG
# define zen_assert(cond) ((void) 0)
# else
# define zen_assert(cond)						\
  (void)((cond) || (__zen__assert(cond, #cond, __FILE__, __LINE__),0))
# endif


# endif

			     
