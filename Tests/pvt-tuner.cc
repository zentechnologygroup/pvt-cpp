
# include <gmock/gmock.h>

# include <metadata/pvt-tuner.H>

using namespace std;
using namespace testing;
using namespace std;

constexpr double epsilon = 1e-9;
static auto cmp_double = [] (double v1, double v2)
{
  return fabs(v1 - v2) <= epsilon;
};

TEST(EmptyPvtData, simple_ops)
{
  PvtData d;

  ASSERT_TRUE(d.const_values.is_empty());
  ASSERT_TRUE(d.vectors.is_empty());
}

TEST(PvtData, add_const)
{
  PvtData data;
  data.add_const("api", 8, Api::get_instance());

  ASSERT_THROW(data.add_const("api", 9, Api::get_instance()),
	       DuplicatedConstName);

  auto cptr = data.search_const("api");
  ASSERT_NE(cptr, nullptr);
  ASSERT_EQ(cptr->name, "api");
  ASSERT_EQ(cptr->value, 8);
  ASSERT_EQ(cptr->unit_ptr, &Api::get_instance());

  Array<double> parray100 = { 100, 200, 300, 820 };
  Array<double> parray200 = { 100, 300, 800, 1120 };
  Array<double> rs100 = {16, 70, 120, 150};
  Array<double> rs200 = {20, 90, 140, 200};

  data.add_vector(100, 820, -1, -1, -1, parray100, psia::get_instance(),
		  "rs", rs100, SCF_STB::get_instance());

  ASSERT_THROW(data.add_vector(100, 820, -1, -1, -1, parray100,
			       psia::get_instance(),
			       "rs", rs100, SCF_STB::get_instance()),
	       DuplicatedVarName);
  auto vlist = data.search_vectors("rs");
  ASSERT_FALSE(vlist.is_empty());
  auto vptr = vlist.get_first();
  ASSERT_EQ(vptr->yname, "rs");
  ASSERT_EQ(vptr->t, 100);
  ASSERT_EQ(vptr->pb, 820);
  ASSERT_EQ(vptr->punit, &psia::get_instance());
  ASSERT_EQ(vptr->yunit, &SCF_STB::get_instance());
  ASSERT_TRUE(zip_all([] (auto t) { return get<0>(t) == get<1>(t); },
		      parray100, vptr->p));
  ASSERT_TRUE(zip_all([] (auto t) { return get<0>(t) == get<1>(t); },
		      rs100, vptr->y));

  data.add_vector(200, 1120, -1, -1, -1, parray200, psia::get_instance(),
		  "rs", rs200, SCF_STB::get_instance());
  ASSERT_THROW(data.add_vector(200, 1120, -1, -1, -1, parray200,
			       psia::get_instance(),
			       "rs", rs200, SCF_STB::get_instance()),
	       DuplicatedVarName);
  vlist = data.search_vectors("rs");
  ASSERT_FALSE(vlist.is_empty());
  vptr = vlist.get_first();

  ASSERT_EQ(vptr->yname, "rs");
  ASSERT_EQ(vptr->t, 100);
  ASSERT_EQ(vptr->pb, 820);
  ASSERT_EQ(vptr->punit, &psia::get_instance());
  ASSERT_EQ(vptr->yunit, &SCF_STB::get_instance());
  ASSERT_TRUE(zip_all([] (auto t) { return get<0>(t) == get<1>(t); },
  		      parray100, vptr->p));
  ASSERT_TRUE(zip_all([] (auto t) { return get<0>(t) == get<1>(t); },
  		      rs100, vptr->y));

  vptr = vlist.get_last();
  
  ASSERT_EQ(vptr->yname, "rs");
  ASSERT_EQ(vptr->t, 200);
  ASSERT_EQ(vptr->pb, 1120);
  ASSERT_EQ(vptr->punit, &psia::get_instance());
  ASSERT_EQ(vptr->yunit, &SCF_STB::get_instance());
  ASSERT_TRUE(zip_all([] (auto t) { return get<0>(t) == get<1>(t); },
  		      parray200, vptr->p));
  ASSERT_TRUE(zip_all([] (auto t) { return get<0>(t) == get<1>(t); },
  		      rs200, vptr->y));
}

TEST(PvtData, split_uo)
{
  PvtData data;
  data.add_const("api", 8.3, Api::get_instance());
  data.add_const("co2", 0.86, MolePercent::get_instance());
  data.add_const("n2", 0.19, MolePercent::get_instance());
  data.add_const("psep", 100, psia::get_instance());
  data.add_const("rsb", 79.5, SCF_STB::get_instance());
  data.add_const("yg", 0.608, Sgg::get_instance());
  data.add_const("tsep", 100, Fahrenheit::get_instance());

  data.add_vector(125, 820, 1.0919, PVT_INVALID_VALUE, 7500, 
		  {820, 650, 550, 450, 0}, psia::get_instance(), "uob",
		  {7500, 11350, 14000, 18120, 30000}, CP::get_instance());
  data.add_vector(125, 820, 1.0919, 30000, 7500, 
		  {3000, 2700, 2400, 2100, 1800, 1500, 1200, 1000},
		  psia::get_instance(), "uoa",
		  {12891, 11384, 10377, 9530, 8762, 8240, 7869, 7638},
		  CP::get_instance());
  data.add_vector(200, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		  PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		  {3000, 2700, 2400, 2100, 1800, 1500, 1200, 900, 600, 400, 200},
		  psia::get_instance(), "uo",
		  {38.14, 36.77, 35.66, 34.42, 33.44, 32.19, 30.29,
		      32.5, 37.1, 40.8, 44.5}, CP::get_instance());
  data.add_vector(300, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		  PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		  {3000, 2700, 2400, 2100, 1800, 1500, 1300,
		      1100, 1000, 800, 500, 200}, psia::get_instance(), "uo",
		  {370, 349.7, 335, 316.3, 302.4, 279.8, 270, 260,
		      256, 278, 345.3, 404.6}, CP::get_instance());

  auto uo_list = data.search_vectors("uo");
  ASSERT_EQ(uo_list.size(), 2);

  auto uob_list = data.search_vectors("uob");
  ASSERT_TRUE(uob_list.is_unitarian());

  auto uoa_list = data.search_vectors("uoa");
  ASSERT_TRUE(uoa_list.is_unitarian());

  const DynList<double> t = {125, 200, 300};

  data.split_uo();
  uob_list = data.search_vectors("uob");
  size_t k = 0;
  for (auto it = zip_it(uob_list, t); it.has_curr(); it.next(), ++k)
    {
      auto t = it.get_curr();
      ASSERT_EQ(get<0>(t)->t, get<1>(t));
    }
  ASSERT_GT(k, 0);

  uoa_list = data.search_vectors("uoa");
  k = 0;
  for (auto it = zip_it(uoa_list, t); it.has_curr(); it.next(), ++k)
    {
      auto t = it.get_curr();
      ASSERT_EQ(get<0>(t)->t, get<1>(t));
    }
  ASSERT_GT(k, 0);
}

struct FluidTest : public Test
{
  const DynList<double> temps = { 125, 200, 300};
  PvtData data;
  FluidTest() // fluid5 
  {
    data.add_const("api", 8.3, Api::get_instance());
    data.add_const("co2", 0.86, MolePercent::get_instance());
    data.add_const("n2", 0.19, MolePercent::get_instance());
    data.add_const("psep", 100, psia::get_instance());
    data.add_const("rsb", 79.5, SCF_STB::get_instance());
    data.add_const("yg", 0.608, Sgg::get_instance());
    data.add_const("tsep", 100, Fahrenheit::get_instance());

    data.add_vector(125, 820, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		    PVT_INVALID_VALUE, {820, 600, 450, 300, 15},
		    psia::get_instance(), "rs",
		    {80, 62, 47, 35, 0}, SCF_STB::get_instance());
    data.add_vector(125, 820, 1.0919, PVT_INVALID_VALUE,
		    PVT_INVALID_VALUE, {820, 600, 450, 300, 15},
		    psia::get_instance(), "bob",
		    {1.0919, 1.0865, 1.0790, 1.0713, 1.0228},
		    RB_STB::get_instance());
    data.add_vector(125, 820, 1.0919, PVT_INVALID_VALUE,
		    PVT_INVALID_VALUE,
		    {3000, 2800, 2600, 2400, 2200, 2000, 1800,
			1600, 1400, 1200, 1000, 2800, 2600, 2400, 2200,
			2000, 1800, 1600, 1400, 1200, 1000, 820},
		    psia::get_instance(), "coa",
		    {3.76E-06, 4.07E-06, 4.28E-06, 4.45E-06, 4.62E-06,
			4.76E-06, 4.86E-06, 5.24E-06, 5.51E-06, 5.61E-06,
			6.00E-06}, psia_1::get_instance());
    data.add_vector(125, 820, 1.0919, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		    {3000, 2800, 2600, 2400, 2200, 2000, 1800, 1600, 1400,
			1200, 1000, 820},
		    psia::get_instance(), "boa",
		    {1.0804, 1.0813, 1.0822, 1.0830, 1.0840, 1.0850,
			1.0861, 1.0872, 1.0883, 1.0895, 1.0907, 1.0919},
		    RB_STB::get_instance());
    data.add_vector(125, 820, 1.0919, PVT_INVALID_VALUE, 7500, 
		    {820, 650, 550, 450, 0}, psia::get_instance(), "uob",
		    {7500, 11350, 14000, 18120, 30000}, CP::get_instance());
    data.add_vector(125, 820, 1.0919, 30000, 7500, 
		    {3000, 2700, 2400, 2100, 1800, 1500, 1200, 1000},
		    psia::get_instance(), "uoa",
		    {12891, 11384, 10377, 9530, 8762, 8240, 7869, 7638},
		    CP::get_instance());
    data.add_vector(200, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		    PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		    {3000, 2700, 2400, 2100, 1800, 1500, 1200, 900, 600, 400, 200},
		    psia::get_instance(), "uo",
		    {38.14, 36.77, 35.66, 34.42, 33.44, 32.19, 30.29,
			32.5, 37.1, 40.8, 44.5}, CP::get_instance());
    data.add_vector(300, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		    PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		    {3000, 2700, 2400, 2100, 1800, 1500, 1300,
			1100, 1000, 800, 500, 200}, psia::get_instance(), "uo",
		    {370, 349.7, 335, 316.3, 302.4, 279.8, 270, 260,
			256, 278, 345.3, 404.6}, CP::get_instance());
    data.split_uo();
  }
};

TEST_F(FluidTest, json)
{
  string json = data.to_json().dump();
  istringstream s(json);
  PvtData test(s);

  ASSERT_EQ(data.pb_corr, test.pb_corr);
  ASSERT_EQ(data.rs_corr, test.rs_corr);
  ASSERT_EQ(data.bob_corr, test.bob_corr);
  ASSERT_EQ(data.coa_corr, test.coa_corr);
  ASSERT_EQ(data.boa_corr, test.boa_corr);
  ASSERT_EQ(data.uod_corr, test.uod_corr);
  ASSERT_EQ(data.uob_corr, test.uob_corr);
  ASSERT_EQ(data.uoa_corr, test.uoa_corr);

  const DynList<string> const_names =
    { "api", "co2", "n2", "psep", "rsb", "yg", "tsep" };

  for (auto it = const_names.get_it(); it.has_curr(); it.next())
    {
      const string & name = it.get_curr();
      auto ptr1 = data.search_const(name);
      auto ptr2 = test.search_const(name);
      ASSERT_EQ(ptr1->name, ptr2->name);
      ASSERT_EQ(ptr1->value, ptr2->value);
      ASSERT_EQ(ptr1->unit_ptr, ptr2->unit_ptr);
    }

  const DynList<string> property_names = { "rs", "bob", "coa", "boa", "uob", "uoa" };
  for (auto it = property_names.get_it(); it.has_curr(); it.next())
    {
      const string & name = it.get_curr();
      auto vlist1 = data.search_vectors(name);
      auto vlist2 = test.search_vectors(name);
      for (auto it = zip_it(vlist1, vlist2); it.has_curr(); it.next())
	{
	  auto t = it.get_curr();
	  const VectorDesc * v1 = get<0>(t);
	  const VectorDesc * v2 = get<1>(t);
	  ASSERT_NEAR(v1->t, v2->t, epsilon);
	  ASSERT_NEAR(v1->pb, v2->pb, epsilon);
	  ASSERT_NEAR(v1->bobp, v2->bobp, epsilon);
	  ASSERT_NEAR(v1->uod, v2->uod, epsilon);
	  ASSERT_NEAR(v1->uobp, v2->uobp, epsilon);
	  ASSERT_EQ(v1->punit, v2->punit);
	  ASSERT_EQ(v1->yunit, v2->yunit);
	  ASSERT_TRUE(eq(v1->p, v2->p, cmp_double));
	  ASSERT_TRUE(eq(v1->y, v2->y, cmp_double));
	}
    }
}

TEST_F(FluidTest, rm_const)
{
  auto api_ptr = data.search_const("api");
  ASSERT_NE(api_ptr, nullptr);
  ASSERT_EQ(api_ptr->name, "api");
  ASSERT_NO_THROW(data.rm_const("api"));
  ASSERT_EQ(data.search_const("api"), nullptr);
}

TEST_F(FluidTest, rm_vector)
{
  DynList<const VectorDesc*> uo_list = data.search_vectors("uo");
  ASSERT_FALSE(uo_list.is_empty());

  auto fst_uo = uo_list.get_first();
  ASSERT_EQ(fst_uo->yname, "uo");
  ASSERT_NO_THROW(data.rm_vector(200, "uo"));

  uo_list = data.search_vectors("uo");
  ASSERT_FALSE(uo_list.exists([] (auto vptr) { return vptr->t == 200; }));

  ASSERT_NO_THROW(data.rm_vector(300, "uo"));

  ASSERT_TRUE(data.search_vectors("uo").is_empty());
  ASSERT_FALSE(data.names.has("uo"));
}

TEST_F(FluidTest, set_correlations)
{
  ASSERT_THROW(data.set_pb(&RsAlMarhoun::get_instance()), InvalidTargetName);
  ASSERT_NO_THROW(data.set_pb(&PbAlMarhoun::get_instance()));
  ASSERT_EQ(data.pb_corr, &PbAlMarhoun::get_instance());
  ASSERT_EQ(data.c_pb, 0);
  ASSERT_EQ(data.m_pb, 1);

  ASSERT_NO_THROW(data.set_pb(&PbAlShammasi::get_instance(), 0.1, 0.99));
  ASSERT_EQ(data.pb_corr, &PbAlShammasi::get_instance());
  ASSERT_EQ(data.c_pb, 0.1);
  ASSERT_EQ(data.m_pb, 0.99);

  ASSERT_THROW(data.set_pb(&UodBeal::get_instance()), InvalidTargetName);
  ASSERT_NO_THROW(data.set_rs(&RsAlMarhoun::get_instance()));
  ASSERT_EQ(data.rs_corr, &RsAlMarhoun::get_instance());
  ASSERT_EQ(data.c_rs, 0);
  ASSERT_EQ(data.m_rs, 1);

  ASSERT_NO_THROW(data.set_rs(&RsPerezML::get_instance(), 0.1, 0.99));
  ASSERT_EQ(data.rs_corr, &RsPerezML::get_instance());
  ASSERT_EQ(data.c_rs, 0.1);
  ASSERT_EQ(data.m_rs, 0.99);

  ASSERT_THROW(data.set_bob(&UodBeal::get_instance()), InvalidTargetName);
  ASSERT_NO_THROW(data.set_bob(&BobAlShammasi::get_instance()));
  ASSERT_EQ(data.bob_corr, &BobAlShammasi::get_instance());
  ASSERT_EQ(data.c_bob, 0);
  ASSERT_EQ(data.m_bob, 1);

  ASSERT_NO_THROW(data.set_bob(&BobPerezML::get_instance(), 0.1, 0.99));
  ASSERT_EQ(data.bob_corr, &BobPerezML::get_instance());
  ASSERT_EQ(data.c_bob, 0.1);
  ASSERT_EQ(data.m_bob, 0.99);

  ASSERT_THROW(data.set_coa(&UodBeal::get_instance()), InvalidTargetName);
  ASSERT_NO_THROW(data.set_coa(&CoaDeGhetto::get_instance()));
  ASSERT_EQ(data.coa_corr, &CoaDeGhetto::get_instance());
  ASSERT_EQ(data.c_coa, 0);
  ASSERT_EQ(data.m_coa, 1);

  ASSERT_NO_THROW(data.set_coa(&CoaPerezML::get_instance(), 0.1, 0.99));
  ASSERT_EQ(data.coa_corr, &CoaPerezML::get_instance());
  ASSERT_EQ(data.c_coa, 0.1);
  ASSERT_EQ(data.m_coa, 0.99);

  ASSERT_THROW(data.set_boa(&UodBeal::get_instance()), InvalidTargetName);
  ASSERT_NO_THROW(data.set_boa(&BoaDeGhetto::get_instance()));
  ASSERT_EQ(data.boa_corr, &BoaDeGhetto::get_instance());
  ASSERT_EQ(data.c_boa, 0);
  ASSERT_EQ(data.m_boa, 1);

  ASSERT_NO_THROW(data.set_boa(&BoaPerezML::get_instance(), 0.1, 0.99));
  ASSERT_EQ(data.boa_corr, &BoaPerezML::get_instance());
  ASSERT_EQ(data.c_boa, 0.1);
  ASSERT_EQ(data.m_boa, 0.99);

  ASSERT_THROW(data.set_uod(&BobAlShammasi::get_instance()), InvalidTargetName);
  ASSERT_NO_THROW(data.set_uod(&UodBeal::get_instance()));
  ASSERT_EQ(data.uod_corr, &UodBeal::get_instance());
  ASSERT_EQ(data.c_uod, 0);
  ASSERT_EQ(data.m_uod, 1);

  ASSERT_NO_THROW(data.set_uod(&UodPerezML::get_instance(), 0.1, 0.99));
  ASSERT_EQ(data.uod_corr, &UodPerezML::get_instance());
  ASSERT_EQ(data.c_uod, 0.1);
  ASSERT_EQ(data.m_uod, 0.99);

  ASSERT_THROW(data.set_uob(&BobAlShammasi::get_instance()), InvalidTargetName);
  ASSERT_NO_THROW(data.set_uob(&UobBeggsRobinson::get_instance()));
  ASSERT_EQ(data.uob_corr, &UobBeggsRobinson::get_instance());
  ASSERT_EQ(data.c_uob, 0);
  ASSERT_EQ(data.m_uob, 1);

  ASSERT_NO_THROW(data.set_uob(&UobPerezML::get_instance(), 0.1, 0.99));
  ASSERT_EQ(data.uob_corr, &UobPerezML::get_instance());
  ASSERT_EQ(data.c_uob, 0.1);
  ASSERT_EQ(data.m_uob, 0.99);

  ASSERT_THROW(data.set_uoa(&BobAlShammasi::get_instance()), InvalidTargetName);
  ASSERT_NO_THROW(data.set_uoa(&UoaBeal::get_instance()));
  ASSERT_EQ(data.uoa_corr, &UoaBeal::get_instance());
  ASSERT_EQ(data.c_uoa, 0);
  ASSERT_EQ(data.m_uoa, 1);

  ASSERT_NO_THROW(data.set_uoa(&UoaPerezML::get_instance(), 0.1, 0.99));
  ASSERT_EQ(data.uoa_corr, &UoaPerezML::get_instance());
  ASSERT_EQ(data.c_uoa, 0.1);
  ASSERT_EQ(data.m_uoa, 0.99);
}

TEST_F(FluidTest, build_entries_for_correlation)
{
  /// TODO falta definir

  /// verificar que las presiones en todos los arreglso sean las mismas
  {
    auto l = data.build_input_for_correlation(&BobLopezCR::get_instance());
    for (auto it = l.get_it(); it.has_curr(); it.next())
      cout << it.get_curr() << endl;
  }

  {
    auto l = data.build_input_for_correlation(&UobBeggsRobinson::get_instance());
    for (auto it = l.get_it(); it.has_curr(); it.next())
      cout << it.get_curr() << endl;
  }

}

TEST_F(FluidTest, compute_values_without_inputing)
{
  data.compute_values(&BobLopezCR::get_instance());

  data.compute_values(&UobBeggsRobinson::get_instance());
}

// TODO: probar en merge_with que valores de bobp uod y uobp den correctos
// TODO: probar to_parpair en  vector
// Todo: parallel_vectors

// TODO 1ro debo programar inputing

// Luego debo verificar parallel vectors

TEST_F(FluidTest, get_pars)
{
  {
    const DynList<const VectorDesc*> uob_vectors = data.search_vectors("uob");
    const DynList<PvtData::Sample> l = data.get_pars(uob_vectors, "rs");
    for (auto it = zip_it(uob_vectors, l); it.has_curr(); it.next())
      {
	auto t = it.get_curr();
	const VectorDesc * vptr = get<0>(t);
	const PvtData::Sample & sample = get<1>(t);
	ASSERT_TRUE(eq(vptr->p, sample.pvals, cmp_double));
      }
  }

  {
    const DynList<const VectorDesc*> uo_vectors = data.search_vectors("uo");
    const DynList<PvtData::Sample> l = data.get_pars(uo_vectors, "rs");
    for (auto it = zip_it(uo_vectors, l); it.has_curr(); it.next())
      {
	auto t = it.get_curr();
	const VectorDesc * vptr = get<0>(t);
	const PvtData::Sample & sample = get<1>(t);
	ASSERT_TRUE(eq(vptr->p, sample.pvals, cmp_double));
	vptr->p.for_each([] (auto p) { cout << p << " "; }); cout << endl;
	sample.pvals.for_each([] (auto p) { cout << p << " "; }); cout << endl;
      }
  }

  {
    const DynList<const VectorDesc*> uo_vectors = data.search_vectors("uoa");
    const DynList<PvtData::Sample> l = data.get_pars(uo_vectors, "boa");
    for (auto it = zip_it(uo_vectors, l); it.has_curr(); it.next())
      {
	auto t = it.get_curr();
	const VectorDesc * vptr = get<0>(t);
	const PvtData::Sample & sample = get<1>(t);
	ASSERT_TRUE(eq(vptr->p, sample.pvals, cmp_double));
      }
    } 
}

TEST_F(FluidTest, matches_with_pars)
{
  auto lbefore = data.matches_with_pars("rs");

  auto api_ptr = data.search_const("api");
  const auto api = *api_ptr;
  data.rm_const("api");

  ASSERT_EQ(data.search_const("api"), nullptr);

  auto lafter = data.matches_with_pars("rs");

  ASSERT_FALSE(eq(lbefore, lafter));

  //data.add_const(const ConstDesc &c)
}


TEST_F(FluidTest, can_be_applied)
{
  auto l = data.can_be_applied("rs");
  l.for_each([] (auto ptr) { cout << ptr->name << endl; });

  cout << "****************************************************************" << endl;

  Correlation::array().filter([] (auto ptr) { return ptr->target_name() == "rs"; }).
    for_each([] (auto ptr) { cout << ptr->name << endl; });
}


TEST_F(FluidTest, list_restrictions)
{

}


TEST_F(FluidTest, get_vectors)
{

}

TEST_F(FluidTest, tp_sets)
{

}

TEST_F(FluidTest, pbapply)
{

}

TEST_F(FluidTest, rsapply)
{

}


TEST_F(FluidTest, bobapply)
{

}

TEST_F(FluidTest, coaapply)
{

}


TEST_F(FluidTest, boaapply)
{

}


TEST_F(FluidTest, uodapply)
{

}


TEST_F(FluidTest, uobapply)
{

}


TEST_F(FluidTest, uoaapply)
{

}

TEST_F(FluidTest, build_samples) // Revisar si es necesaria
{

}

TEST_F(FluidTest, iapply)
{

}

TEST_F(FluidTest, pbstats)
{

}

TEST_F(FluidTest, rsstats)
{

}

TEST_F(FluidTest, bobstats)
{

}

TEST_F(FluidTest, coastats)
{

}

TEST_F(FluidTest, boastats)
{

}

TEST_F(FluidTest, uodstats)
{

}

TEST_F(FluidTest, uobstats)
{

}

TEST_F(FluidTest, uoastats)
{

}

TEST_F(FluidTest, istats)
{

}

TEST_F(FluidTest, auto_apply)
{

}



TEST(PvtData, are_all_correlations_defined_and_missing)
{

}


