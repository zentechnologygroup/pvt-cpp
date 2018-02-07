
# include <gmock/gmock.h>

# include <iostream>

# include <correlations/correlation-invoker.H>
# include <correlations/pvt-correlations.H>

using namespace std;
using namespace Aleph;
using namespace testing;

struct Invoker : public Test
{
  CorrelationInvoker invoker = { Fahrenheit::get_instance(),
				 psia::get_instance() };
};

TEST_F(Invoker, SimpleNoExceptions)
{
  auto corr_ptr = &PbAlMarhoun::get_instance();
  auto ret = invoker.compute(corr_ptr, true,
			     npar("yg", .8, Sgg::get_instance()),
			     npar("yo", .9, Sg_do::get_instance()),
			     npar("rsb", 100, SCF_STB::get_instance()),
			     npar("t", 130, Fahrenheit::get_instance()));
  ASSERT_NEAR(ret.raw(), 749.131, 1e-3);
  ASSERT_TRUE(invoker.exception_list.is_empty());
}

TEST_F(Invoker, SimpleWithExceptions)
{
  auto corr_ptr = &PbAlMarhoun::get_instance();
  auto ret = invoker.compute(corr_ptr, true,
			     npar("yg", .8, Sgg::get_instance()),
			     npar("yo", .9, Sg_do::get_instance()),
			     npar("rsb", 100, SCF_STB::get_instance()));

  ASSERT_TRUE(invoker.exception_list.is_empty());

  invoker.report_exceptions = true;

  ret = invoker.compute(corr_ptr, true, npar("yg", .8, Sgg::get_instance()),
			     npar("yo", .9, Sg_do::get_instance()),
			     npar("rsb", 100, SCF_STB::get_instance()));

  cout << invoker.exception_list.get_first() << endl;
}

