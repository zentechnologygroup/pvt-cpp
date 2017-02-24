# ifndef UNITTEST_UTILS_H
# define UNITTEST_UTILS_H

# include <string>

# include <units/units.H>

using namespace std;

struct UnitTest
{
  const string csv_name = "No-defined";
  const size_t csv_line = 0;
  const string src_unit_name = "No-defined";
  const string tgt_unit_name = "No-defined";
  const double value = 0;
  const double expected = 0;
  const double tolerance = 0;
  mutable double result = 0;

  UnitTest() {}

  UnitTest(const string & csv_name, size_t csv_line,
	   const string & src_unit_name, const string & tgt_unit_name,
	   double value, double expected, double tolerance)
    : csv_name(csv_name), csv_line(csv_line), src_unit_name(src_unit_name),
      tgt_unit_name(tgt_unit_name), value(value), expected(expected),
      tolerance(tolerance) {}

  double convert() const
  {
    return unit_convert_name_to_name(src_unit_name, value, tgt_unit_name);
  }
  
  bool pass()
  {
    result = convert();
    auto error = 100*fabs(expected - result)/expected;
    return error <= tolerance;
  }
};

class UnitTester
{
  DynList<UnitTest> test_list;
  DynList<string> failed_list;

public:

  void define_test(const string & csv_name, size_t csv_line,
		   const string & src_unit_name, const string & tgt_unit_name,
		   double value, double expected, double tolerance)
  {
    test_list.emplace(csv_name, csv_line, src_unit_name, tgt_unit_name,
		      value, expected, tolerance);
  }

  void perform()
  {
    for (auto it = test_list.get_it(); it.has_curr(); it.next())
      {
	auto & test = it.get_curr();
	if (test.
      }
  }
};

# endif
