

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
  cplot.set_pb_correlation(&RsAlMarhoun::get_instance());
  return;
  ASSERT_THROW(cplot.set_pb_correlation(&RsAlMarhoun::get_instance()),
	       CorrelationNotFound);
  cplot.set_pb_correlation(&PbAlMarhoun::get_instance());
  
  ASSERT_THROW(cplot.init_blackoil(), CorrelationNotFound);
  cplot.set_rs_correlation(&RsAlMarhoun::get_instance());
  ASSERT_THROW(cplot.init_blackoil(), CorrelationNotFound);

  cplot.set_bob_correlation(&BobAlmarhoun::get_instance());
  ASSERT_THROW(cplot.init_blackoil(), CorrelationNotFound);
  cplot.set_coa_correlation(&CoaDeGhetto::get_instance());

  ASSERT_THROW(cplot.init_blackoil(), CorrelationNotFound);
  cplot.set_bob_correlation(&BoaMcCain::get_instance());
  ASSERT_THROW(cplot.init_blackoil(), CorrelationNotFound);

  cplot.set_bob_correlation(&BoaMcCain::get_instance());
  ASSERT_THROW(cplot.init_blackoil(), CorrelationNotFound);
}
