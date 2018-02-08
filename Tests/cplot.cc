

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

TEST_F(SimplePlot, Basic)
{
  ASSERT_THROW(cplot.init_blackoil(), CorrelationNotFound);
  cplot.set_pb(&PbAlMarhoun::get_instance());
  ASSERT_NO_THROW(cplot.verify_correlation(&PbAlMarhoun::get_instance(), "pb"));
  
  cplot.set_rs(&RsAlMarhoun::get_instance());
  cplot.set_bob(&BobAlmarhoun::get_instance());
  
}
