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


void put_in_list(DynList<VtlQuantity> & l)
{
  //l.append(q);
}

template <class ... Args>
void put_in_list(DynList<VtlQuantity> & l, const VtlQuantity & q,
		 const Args & ... args)
{
  l.append(q);
  put_in_list(l, args ...);
}

struct CorrelationTester
{
  struct Test
  {
    size_t line;
    const Correlation * corr_ptr = nullptr;
    bool passed = false;
    DynList<VtlQuantity> parameters;

    Test(size_t line, const Correlation & correlation)
      : line(line), corr_ptr(&correlation) {}
    Test() {}
  };

  DynList<Test> test_list;

  template <class RefUnit, class CorrClass, typename ... Args>
  inline void test(size_t csv_line,
		   const Quantity<RefUnit> & expected,
		   double epsilon,
		   const CorrClass & correlation,
		   Args ... args)
  {
    cout << "Testing " << correlation.name << " from csv line " << csv_line
	 << endl;
    Test test(csv_line, correlation);
    auto result = correlation.impl(args...);
    test.passed = fabs(result.raw() - expected.raw()) <= epsilon;
    test_list.append(test);
    if (test.passed)
      {
	cout << "  passed" << endl;
	return;
      }

    DynList<VtlQuantity> l;
    put_in_list(l, args...);

    cout << "  failed" << endl
	 << "    " << correlation.name << "(";
    for (auto it = l.get_it(); it.has_curr(); it.next())
      {
	const auto & par = it.get_curr();
	cout << par;
	if (&par != &test.parameters.get_last())
	  cout << ", ";
      }
    cout << ") = " << result << endl
	 << "    Expected = " << expected << endl
	 << "    epsilon  = " << epsilon << endl
	 << endl;
  }
};

template <class RefUnit, class CorrClass, typename ... Args>
inline void correlation_test(size_t csv_line,
			     const Quantity<RefUnit> & expected,
			     double epsilon,
			     const CorrClass & correlation,
			     Args ... args)
{
  auto result = correlation.impl(args...);
  if (fabs(result.raw() - expected.raw()) <= epsilon)
    return;

  cout << "Test for correlation " << correlation.name << " csv line " << csv_line
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

			     
