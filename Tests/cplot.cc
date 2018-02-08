

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
  cplot.set_pb(&RsAlMarhoun::get_instance());
  return;
  ASSERT_THROW(cplot.set_pb(&RsAlMarhoun::get_instance()), CorrelationNotFound);
  cplot.set_pb(&PbAlMarhoun::get_instance());
  
  ASSERT_THROW(cplot.init_blackoil(), CorrelationNotFound);
  cplot.set_rs(&RsAlMarhoun::get_instance());
  ASSERT_THROW(cplot.init_blackoil(), CorrelationNotFound);

  cplot.set_bob(&BobAlmarhoun::get_instance());
  ASSERT_THROW(cplot.init_blackoil(), CorrelationNotFound);
  cplot.set_coa(&CoaDeGhetto::get_instance());

  ASSERT_THROW(cplot.init_blackoil(), CorrelationNotFound);
  cplot.set_bob(&BoaMcCain::get_instance());
  ASSERT_THROW(cplot.init_blackoil(), CorrelationNotFound);

  cplot.set_bob(&BoaMcCain::get_instance());
  ASSERT_THROW(cplot.init_blackoil(), CorrelationNotFound);
}
