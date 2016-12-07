# ifndef UNITTEST_UTILS_H
# define UNITTEST_UTILS_H

# include <cmath>
# include <iostream>
# include <iomanip>
# include <string>
# include <term-colors.H>
# include <correlations/correlation.H>

using namespace std;

inline void __put_in_list(DynList<VtlQuantity> &) { /* empty */ }

template <class ... Args> 
void __put_in_list(DynList<VtlQuantity> & l, const VtlQuantity & q,
		   const Args & ... args)
{
  l.append(q);
  __put_in_list(l, args ...);
}

# define CORRELATION_BACKEND_NAME "./test-corr -P"

struct CorrelationTester
{
  struct Test
  {
    size_t line;
    const Correlation * corr_ptr = nullptr;
    bool passed = false;
    DynList<VtlQuantity> parameters;
    pair<double, const Unit*> expected;

    Test(size_t line, const Correlation & correlation, const VtlQuantity & expected)
      : line(line), corr_ptr(&correlation),
	expected(make_pair(expected.raw(), &expected.unit)) {}
    
    Test() {}

    string call_string() const
    {
      ostringstream s;
      s << corr_ptr->name << "(";
      for (auto it = parameters.get_it(); it.has_curr(); it.next())
	{
	  const auto & par = it.get_curr();
	  s << par;
	  if (&par != &parameters.get_last())
	    s << ", ";
	}
      s << ")";
      return s.str();
    }

    string python_call() const
    {
      ostringstream s;
      s << corr_ptr->name << "Correlation(";
      for (auto it = parameters.get_it(); it.has_curr(); it.next())
	{
	  const auto & par = it.get_curr();
	  s << par.raw();
	  if (&par != &parameters.get_last())
	    s << ", ";
	}
      s << ")";
      return s.str();
    }

    string backend_call() const
    {
      DynList<pair<size_t, const Unit *>> unit_changes;
      ostringstream s;
      s << CORRELATION_BACKEND_NAME << corr_ptr->name;
      for (auto it = get_enum_zip_it(corr_ptr->get_preconditions(), parameters);
	   it.has_curr(); it.next())
	{
	  auto t = it.get_curr();
	  const auto & par = get<0>(t);
	  const VtlQuantity & arg = get<1>(t);
	  s << " " << arg.raw();
	  if (&par.unit != &arg.unit)
	    unit_changes.append(make_pair(get<2>(t) + 1, &arg.unit));
	}
      for (auto it = unit_changes.get_it(); it.has_curr(); it.next())
	{
	  auto p = it.get_curr();
	  s << " -u \"" << p.first << " " << p.second->symbol << "\"";
	}
      return s.str();
    }
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

    Test test(csv_line, correlation, expected);
    auto result = correlation.impl(args...);

    // In order to correctly compare, we must have the values in the
    // same unit. So, we convert expected to the same unit of
    // correlation return unit
    auto expected_result = VtlQuantity(correlation.unit, expected);

    test.passed = fabs(result.raw() - expected_result.raw()) <= epsilon;
    auto & test_ref = test_list.append(test);
    if (test.passed)
      {
	cout << COLOR_FGREEN("  passed") << endl
	     << endl;
	return;
      }

    auto error =
      100*fabs(expected_result.raw() - result.raw())/expected_result.raw();

    DynList<VtlQuantity> & l = test_ref.parameters;
    __put_in_list(l, args...);

    ostringstream s;
    s << COLOR_FRED("  failed") << endl
      << COLOR_YEL << "    " << test_ref.call_string() << " = "
      << result << endl << COLOR_RESET
      << "    Expected = " << expected_result;
    if (&expected.unit != &result.unit)
      s << " (" << expected << ")";

    s << endl
      << "    epsilon  = " << epsilon << endl
      << fixed << setprecision(2) << "    error    = " << error << " %";

    cout << COLOR_RED << s.str() << COLOR_RESET << endl
	 << endl;
  }

  void summary() const
  {
    size_t success_counter = 0;
    size_t failed_counter = 0;
    cout << "Failed tests summary:"  << endl;
    for (auto it = test_list.get_it(); it.has_curr(); it.next())
      {
	const auto & test = it.get_curr();
	if (test.passed)
	  {
	    ++success_counter;
	    continue;
	  }
	//cout << "In " << test
      }
  }
};

# endif

			     
