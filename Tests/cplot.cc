

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

TEST_F(SimplePlot, Set_Optional_Correlations)
{
  // This would be the general structure of this test
  ASSERT_THROW(cplot.set_cob(RsAbovePb::get_instance()), InvalidTargetType);
  ASSERT_NO_THROW(cplot.set_cob(CobMcCainEtAl::get_instance()));
  // The remaining tests would be similar but with others correlations
}

TEST_F(SimplePlot, Set_Mandatory_Correlations)
{
  // This would be the general structure of this test
  ASSERT_THROW(cplot.set_pb(RsAlMarhoun::get_instance()),
	       InvalidTargetType);
  ASSERT_NO_THROW(cplot.set_pb(PbAlMarhoun::get_instance()));

  ASSERT_THROW(cplot.set_pb_corr(&RsAlMarhoun::get_instance()),
	       InvalidTargetType);
  ASSERT_NO_THROW(cplot.set_pb_corr(&PbAlShammasi::get_instance()));

  ASSERT_NO_THROW(cplot.set_pb(PbAlMarhoun::get_instance(), 2, 2));
  ASSERT_EQ(cplot.c_pb, 2);
  ASSERT_EQ(cplot.m_pb, 2);

  // The remaining tests would be similar but with others correlations
}

TEST_F(SimplePlot, Verify_static_method)
{
  ASSERT_NO_THROW(cplot.verify_correlation(&RsAlMarhoun::get_instance(), "rs"));
  ASSERT_THROW(cplot.verify_correlation(&RsAlMarhoun::get_instance(), "pb"),
	       InvalidTargetType);
  ASSERT_NO_THROW(cplot.set_pb(PbAlMarhoun::get_instance()));
  ASSERT_THROW(cplot.verify_correlation(cplot.pb_corr, "rs"), InvalidTargetType);
}

TEST_F(SimplePlot, blackoil_correlations_are_set)
{
  ASSERT_THROW(cplot.blackoil_correlations_are_set(), CorrelationNotFound);
  cplot.set_pb(PbAlMarhoun::get_instance());
  ASSERT_THROW(cplot.blackoil_correlations_are_set(), CorrelationNotFound);
  cplot.set_rs(RsAlMarhoun::get_instance());
  ASSERT_THROW(cplot.blackoil_correlations_are_set(), CorrelationNotFound);
  cplot.set_bob(BobAlShammasi::get_instance());
  ASSERT_THROW(cplot.blackoil_correlations_are_set(), CorrelationNotFound);
  cplot.set_coa(CoaDeGhetto::get_instance());
  ASSERT_THROW(cplot.blackoil_correlations_are_set(), CorrelationNotFound);
  cplot.set_boa(BoaMcCain::get_instance());
  ASSERT_THROW(cplot.blackoil_correlations_are_set(), CorrelationNotFound);
  cplot.set_uod(UodBeal::get_instance());
  ASSERT_THROW(cplot.blackoil_correlations_are_set(), CorrelationNotFound);
  cplot.set_uob(UobBeggsRobinson::get_instance());
  ASSERT_THROW(cplot.blackoil_correlations_are_set(), CorrelationNotFound);
  cplot.set_uoa(UoaAbedini::get_instance());
  ASSERT_THROW(cplot.blackoil_correlations_are_set(), CorrelationNotFound);
  cplot.set_zfactor(ZfactorBrillBeggs::get_instance());
  ASSERT_NO_THROW(cplot.blackoil_correlations_are_set());
}

TEST_F(SimplePlot, Set_parameters)
{
  ASSERT_THROW(cplot.test_api(), ValueNotFound);
  ASSERT_THROW(cplot.test_rsb(), ValueNotFound);
  ASSERT_THROW(cplot.test_tsep(), ValueNotFound);
  ASSERT_THROW(cplot.test_tsep2(), ValueNotFound);
  ASSERT_THROW(cplot.test_psep(), ValueNotFound);
  ASSERT_THROW(cplot.test_ogr(), ValueNotFound);
  ASSERT_THROW(cplot.test_n2(), ValueNotFound);
  ASSERT_THROW(cplot.test_co2(), ValueNotFound);
  ASSERT_THROW(cplot.test_h2s(), ValueNotFound);
  ASSERT_THROW(cplot.test_nacl(), ValueNotFound);

  ASSERT_THROW(cplot.set_api(12, Fahrenheit::get_instance()), InvalidUnit);
  ASSERT_THROW(cplot.set_rsb(12, Fahrenheit::get_instance()), InvalidUnit);
  ASSERT_THROW(cplot.set_tsep(12, Api::get_instance()), InvalidUnit);
  ASSERT_THROW(cplot.set_tsep2(12, Api::get_instance()), InvalidUnit);
  ASSERT_THROW(cplot.set_psep(12, Fahrenheit::get_instance()), InvalidUnit);
  ASSERT_THROW(cplot.set_ogr(12, Fahrenheit::get_instance()), InvalidUnit);
  ASSERT_THROW(cplot.set_n2(12, Fahrenheit::get_instance()), InvalidUnit);
  ASSERT_THROW(cplot.set_co2(12, Fahrenheit::get_instance()), InvalidUnit);
  ASSERT_THROW(cplot.set_h2s(12, Fahrenheit::get_instance()), InvalidUnit);
  ASSERT_THROW(cplot.set_nacl(12, Fahrenheit::get_instance()), InvalidUnit);

  ASSERT_NO_THROW(cplot.set_api(12, Api::get_instance()));
  ASSERT_NO_THROW(cplot.set_rsb(800, SCF_STB::get_instance()));
  ASSERT_NO_THROW(cplot.set_tsep(120, Fahrenheit::get_instance()));
  ASSERT_NO_THROW(cplot.set_tsep2(120, Fahrenheit::get_instance()));
  ASSERT_NO_THROW(cplot.set_psep(200, psia::get_instance()));
  ASSERT_NO_THROW(cplot.set_ogr(12, STB_MMscf::get_instance()));
  ASSERT_THROW(cplot.set_n2(111, MolePercent::get_instance()), OutOfUnitRange);
  ASSERT_NO_THROW(cplot.set_co2(10, MolePercent::get_instance()));
  ASSERT_NO_THROW(cplot.set_h2s(10, MolePercent::get_instance()));
  ASSERT_THROW(cplot.set_nacl(10, MolePercent::get_instance()), InvalidUnit);
  ASSERT_NO_THROW(cplot.set_nacl(6, Molality_NaCl::get_instance()));  
}

TEST_F(SimplePlot, blackoil_ready)
{
  ASSERT_THROW(cplot.blackoil_ready(), CorrelationNotFound);
}
