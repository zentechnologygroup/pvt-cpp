

# include <gmock/gmock.h>

# include <iostream>

# include <correlations/cplot.H>

using namespace std;
using namespace Aleph;
using namespace testing;

struct SimplePlot : public Test
{
  Cplot cplot;
};

TEST_F(SimplePlot, Set_Correlations)
{
  // This would be the general structure of this test
  ASSERT_THROW(cplot.set_pb(RsAlMarhoun::get_instance()),
	       InvalidTargetType);
  ASSERT_NO_THROW(cplot.set_pb(PbAlMarhoun::get_instance()));

  ASSERT_THROW(cplot.set_pb(&RsAlMarhoun::get_instance()),
	       InvalidTargetType);
  ASSERT_NO_THROW(cplot.set_pb(&PbAlShammasi::get_instance()));

  ASSERT_NO_THROW(cplot.set_pb(PbAlMarhoun::get_instance(), 2, 2));
  ASSERT_EQ(cplot.c_pb, 2);
  ASSERT_EQ(cplot.m_pb, 2);

  // The remaining tests would be similar but with others correlations
}

TEST_F(SimplePlot, Verify_static_method)
{
  ASSERT_THROW(cplot.verify_correlation(cplot.pb_corr, "rs"), InvalidTargetType);
}
