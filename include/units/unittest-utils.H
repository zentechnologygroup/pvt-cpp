# ifndef UNITTEST_UTILS_H
# define UNITTEST_UTILS_H

# include <units/units.H>

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

private:

  double error = 0;

public:

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
    error = fabs(expected - result)/expected;
    return error <= tolerance;
  }

  string to_string() const
  {
    ostringstream s;

    s.precision(numeric_limits<double>::max_digits10);
    s << "line        = " << csv_line << endl
      << "source unit = " << src_unit_name << endl
      << "target unit = " << tgt_unit_name << endl
      << "value       = " << value << endl
      << "expected    = " << expected << endl
      << "result      = " << result << endl
      << "error       = " << error << endl
      << "tolerance   = " << tolerance;

    return s.str();
  }
};

class UnitTester
{
  DynList<UnitTest> test_list;
  DynList<string> failed_list;
  DynList<string> passed_list;

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
	if (test.pass())
	  passed_list.append(test.to_string());
	else
	  failed_list.append(test.to_string());
      }
  }

  static void report(const DynList<string> & l)
  {
     for (auto it = l.get_it(); it.has_curr(); it.next())
      cout << it.get_curr() << endl
	   << endl;
  }
  
  void report_failed() const
  {
    report(failed_list);
  }

  void report_passed() const
  {
    report(passed_list);
  }

  bool all_passed() const { return failed_list.is_empty(); }
};

# endif
