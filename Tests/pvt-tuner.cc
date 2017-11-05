
# include <gmock/gmock.h>

# include <metadata/pvt-tuner.H>

using namespace std;
using namespace testing;
using namespace std;

const UnitsInstancer & __units_sys = UnitsInstancer::init();

constexpr double epsilon = 1e-9;
static auto near_cmp = [] (double v1, double v2)
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

  data.add_vector(100, 820, -1, -1, -1, parray100, psig::get_instance(),
		  "rs", rs100, SCF_STB::get_instance());

  ASSERT_THROW(data.add_vector(100, 820, -1, -1, -1, parray100,
			       psig::get_instance(),
			       "rs", rs100, SCF_STB::get_instance()),
	       DuplicatedVarName);
  auto vlist = data.search_vectors("rs");
  ASSERT_FALSE(vlist.is_empty());
  auto vptr = vlist.get_first();
  ASSERT_EQ(vptr->yname, "rs");
  ASSERT_EQ(vptr->t, 100);
  ASSERT_EQ(vptr->pb, 820);
  ASSERT_EQ(vptr->punit, &psig::get_instance());
  ASSERT_EQ(vptr->yunit, &SCF_STB::get_instance());
  ASSERT_TRUE(zip_all([] (auto t) { return get<0>(t) == get<1>(t); },
		      parray100, vptr->p));
  ASSERT_TRUE(zip_all([] (auto t) { return get<0>(t) == get<1>(t); },
		      rs100, vptr->y));

  data.add_vector(200, 1120, -1, -1, -1, parray200, psig::get_instance(),
		  "rs", rs200, SCF_STB::get_instance());
  ASSERT_THROW(data.add_vector(200, 1120, -1, -1, -1, parray200,
			       psig::get_instance(),
			       "rs", rs200, SCF_STB::get_instance()),
	       DuplicatedVarName);
  vlist = data.search_vectors("rs");
  ASSERT_FALSE(vlist.is_empty());
  vptr = vlist.get_first();

  ASSERT_EQ(vptr->yname, "rs");
  ASSERT_EQ(vptr->t, 100);
  ASSERT_EQ(vptr->pb, 820);
  ASSERT_EQ(vptr->punit, &psig::get_instance());
  ASSERT_EQ(vptr->yunit, &SCF_STB::get_instance());
  ASSERT_TRUE(zip_all([] (auto t) { return get<0>(t) == get<1>(t); },
  		      parray100, vptr->p));
  ASSERT_TRUE(zip_all([] (auto t) { return get<0>(t) == get<1>(t); },
  		      rs100, vptr->y));

  vptr = vlist.get_last();
  
  ASSERT_EQ(vptr->yname, "rs");
  ASSERT_EQ(vptr->t, 200);
  ASSERT_EQ(vptr->pb, 1120);
  ASSERT_EQ(vptr->punit, &psig::get_instance());
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
  data.add_const("psep", 100, psig::get_instance());
  data.add_const("rsb", 79.5, SCF_STB::get_instance());
  data.add_const("yg", 0.608, Sgg::get_instance());
  data.add_const("tsep", 100, Fahrenheit::get_instance());

  data.add_vector(125, 820, 1.0919, PVT_INVALID_VALUE, 7500, 
		  {820, 650, 550, 450, 0}, psig::get_instance(), "uob",
		  {7500, 11350, 14000, 18120, 30000}, CP::get_instance());
  data.add_vector(125, 820, 1.0919, 30000, 7500, 
		  {3000, 2700, 2400, 2100, 1800, 1500, 1200, 1000},
		  psig::get_instance(), "uoa",
		  {12891, 11384, 10377, 9530, 8762, 8240, 7869, 7638},
		  CP::get_instance());
  data.add_vector(300, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		  PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		  {3000, 2700, 2400, 2100, 1800, 1500, 1200, 900, 600, 400, 200},
		  psig::get_instance(), "uo",
		  {38.14, 36.77, 35.66, 34.42, 33.44, 32.19, 30.29,
		      32.5, 37.1, 40.8, 44.5}, CP::get_instance());
  data.add_vector(200, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		  PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		  {3000, 2700, 2400, 2100, 1800, 1500, 1300,
		      1100, 1000, 800, 500, 200}, psig::get_instance(), "uo",
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
      const VectorDesc * uob = get<0>(t);
      ASSERT_EQ(uob->uod, uob->y(0));
      ASSERT_EQ(uob->uobp, uob->y.get_last());
      ASSERT_EQ(uob->t, get<1>(t));
    }
  ASSERT_GT(k, 0);

  uoa_list = data.search_vectors("uoa");
  k = 0;
  for (auto it = zip_it(uoa_list, t); it.has_curr(); it.next(), ++k)
    {
      auto t = it.get_curr();
      const VectorDesc * uoa = get<0>(t);
      ASSERT_NE(uoa->uod, PVT_INVALID_VALUE);
      ASSERT_EQ(uoa->t, get<1>(t));
    }
  ASSERT_GT(k, 0);

  // now we test a bad split
  data.rm_vector(200, "uob");
  data.rm_vector(200, "uoa");
  data.rm_vector(300, "uob");
  data.rm_vector(300, "uoa");
  data.rm_vector(200, "uo");
  data.rm_vector(300, "uo");

  // these uo vectors are the same than above, excepts that the
  // temperature is swapped. So, the split would cause a pb for 300
  // degF less that the pb for 200 degF
  data.add_vector(200, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		  PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		  {3000, 2700, 2400, 2100, 1800, 1500, 1200, 900, 600, 400, 200},
		  psig::get_instance(), "uo",
		  {38.14, 36.77, 35.66, 34.42, 33.44, 32.19, 30.29,
		      32.5, 37.1, 40.8, 44.5}, CP::get_instance());
  data.add_vector(300, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		  PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		  {3000, 2700, 2400, 2100, 1800, 1500, 1300,
		      1100, 1000, 800, 500, 200}, psig::get_instance(), "uo",
		  {370, 349.7, 335, 316.3, 302.4, 279.8, 270, 260,
		      256, 278, 345.3, 404.6}, CP::get_instance());
  ASSERT_THROW(data.split_uo(), PressureMismatch);
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
    data.add_const("psep", 100, psig::get_instance());
    data.add_const("rsb", 79.5, SCF_STB::get_instance());
    data.add_const("yg", 0.608, Sgg::get_instance());
    data.add_const("tsep", 100, Fahrenheit::get_instance());

    data.add_vector(125, 820, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		    PVT_INVALID_VALUE, {820, 600, 450, 300, 15},
		    psig::get_instance(), "rs",
		    {79.5, 62, 47, 35, 0}, SCF_STB::get_instance());
    data.add_vector(125, 820, 1.0919, PVT_INVALID_VALUE,
		    PVT_INVALID_VALUE, {600, 450, 300, 15},
		    psig::get_instance(), "bob",
		    {1.0865, 1.0790, 1.0713, 1.0228},
		    RB_STB::get_instance());
    data.add_vector(125, 820, 1.0919, PVT_INVALID_VALUE,
		    PVT_INVALID_VALUE,
		    {3000, 2800, 2600, 2400, 2200, 2000, 1800,
			1600, 1400, 1200, 1000, 2800, 2600, 2400, 2200,
			2000, 1800, 1600, 1400, 1200, 1000, 820},
		    psig::get_instance(), "coa",
		    {3.76E-06, 4.07E-06, 4.28E-06, 4.45E-06, 4.62E-06,
			4.76E-06, 4.86E-06, 5.24E-06, 5.51E-06, 5.61E-06,
			6.00E-06}, psia_1::get_instance());
    data.add_vector(125, 820, 1.0919, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		    {3000, 2800, 2600, 2400, 2200, 2000, 1800, 1600, 1400,
			1200, 1000},
		    psig::get_instance(), "boa",
		    {1.0804, 1.0813, 1.0822, 1.0830, 1.0840, 1.0850,
			1.0861, 1.0872, 1.0883, 1.0895, 1.0907},
		    RB_STB::get_instance());
    data.add_vector(125, 820, 1.0919, PVT_INVALID_VALUE, 7500, 
		    {820, 650, 550, 450, 0}, psig::get_instance(), "uob",
		    {7500, 11350, 14000, 18120, 30000}, CP::get_instance());
    data.add_vector(125, 820, 1.0919, 30000, 7500, 
		    {3000, 2700, 2400, 2100, 1800, 1500, 1200, 1000},
		    psig::get_instance(), "uoa",
		    {12891, 11384, 10377, 9530, 8762, 8240, 7869, 7638},
		    CP::get_instance());
    data.add_vector(200, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		    PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		    {3000, 2700, 2400, 2100, 1800, 1500, 1300,
			1100, 1000, 800, 500, 200}, psig::get_instance(), "uo",
		    {370, 349.7, 335, 316.3, 302.4, 279.8, 270, 260,
			256, 278, 345.3, 404.6}, CP::get_instance());
    data.add_vector(300, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		    PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		    {3000, 2700, 2400, 2100, 1800, 1500, 1200, 900, 600, 400, 200},
		    psig::get_instance(), "uo",
		    {38.14, 36.77, 35.66, 34.42, 33.44, 32.19, 30.29,
			32.5, 37.1, 40.8, 44.5}, CP::get_instance());
    data.split_uo();

    {
      auto uob_list = data.search_vectors("uob");
      assert(not uob_list.is_empty());
      assert(uob_list.all([] (auto & v) { return v->uod != PVT_INVALID_VALUE and
	      v->uobp != PVT_INVALID_VALUE; }));
    }
    {
      auto uoa_list = data.search_vectors("uoa");
      assert(not uoa_list.is_empty());
      assert(uoa_list.all([] (auto & v) { return v->uod != PVT_INVALID_VALUE and
	      v->uobp != PVT_INVALID_VALUE; }));
    }
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
	  ASSERT_TRUE(eq(v1->p, v2->p, near_cmp));
	  ASSERT_TRUE(eq(v1->y, v2->y, near_cmp));
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


TEST_F(FluidTest, tp_sets)
{
  auto tp = data.tp_sets();
  ASSERT_FALSE(tp.is_empty());
}

TEST_F(FluidTest, compute_pb)
{
  const Correlation * pb_corr = &PbAlMarhoun::get_instance();
  const DynList<PvtData::TpSet> tp = data.tp_sets();
  const DynList<double> expected = { 1522.54 - 14, 1786.91 - 14, 2154.86 - 14 };
  for (auto it = zip_it(tp, expected); it.has_curr(); it.next())
    {
      auto t = it.get_curr();
      ASSERT_NEAR(data.compute_pb(get<0>(t).t, pb_corr).raw(), get<1>(t), 2);
    }
  
  constexpr double c = 10, m = 0.98;
  for (auto it = zip_it(tp, expected); it.has_curr(); it.next())
    {
      auto t = it.get_curr();
      ASSERT_NEAR(data.compute_pb(get<0>(t).t, pb_corr, c, m).raw(),
		  c + m*get<1>(t), 2);
    }
}

TEST_F(FluidTest, pb_stats)
{
  const DynList<const Correlation*> pb_corrs = data.can_be_applied("pb");
  ASSERT_FALSE(pb_corrs.is_empty());

  auto stats = pb_corrs.maps<PvtData::ConstStats>
    ([this] (auto ptr) { return data.pb_stats(ptr); });

  auto str_list =
    stats.maps<DynList<string>>([] (auto & c) { return c.to_dynlist(); });

  cout << to_string(format_string(str_list)) << endl;

  for (auto it = stats.get_it(); it.has_curr(); it.next())
    {
      auto & s = it.get_curr();
      const double & c = CorrStat::c(s.desc);
      const double & m = CorrStat::m(s.desc);
      cout << s.corr_ptr->name << " c = " << c << " m = " << m << endl;
      for (auto it = zip_it(s.t, s.ylab, s.ycorr, s.ytuned); it.has_curr(); it.next())
	{
	  auto t = it.get_curr();
	  cout << "  t = " << get<0>(t) << " pb = " << get<1>(t) << " pbcorr = "
	       << get<2>(t) << " tuned = " << get<3>(t) << endl;
	}
    }
}

TEST_F(FluidTest, compute_uod)
{
  const Correlation * uod_corr = &UodBeal::get_instance();
  const DynList<PvtData::TpSet> tp = data.tp_sets();
  const DynList<double> expected = { 19838.5, 70.8092, 0.16593 };
  for (auto it = zip_it(tp, expected); it.has_curr(); it.next())
    {
      auto t = it.get_curr();
      ASSERT_NEAR(data.compute_uod(get<0>(t).t, get<0>(t).pb, uod_corr).raw(),
		  get<1>(t), 1e-1);
    }

  constexpr double c = 10, m = 0.98;
  for (auto it = zip_it(tp, expected); it.has_curr(); it.next())
    {
      auto t = it.get_curr();
      ASSERT_NEAR(data.compute_uod(get<0>(t).t, get<0>(t).pb, uod_corr, c, m).raw(),
		  c + m*get<1>(t), 1e-1);
    }

  data.set_pb(&PbAlMarhoun::get_instance(), 11, 0.97);
  const DynList<double> pb = tp.maps<double>([this] (auto t)
    {
      return data.get_pb(t.t, t.pb).second.first;
    });
  for (auto it = zip_it(tp, pb); it.has_curr(); it.next())
    {
      auto t = it.get_curr();
      ASSERT_NEAR(data.compute_uod(get<0>(t).t, get<0>(t).pb, uod_corr).raw(),
		  data.compute_uod(get<0>(t).t, get<1>(t), uod_corr).raw(), 1e-6);
    }
}

TEST_F(FluidTest, compute_rs)
{
  const Correlation * rs_corr = &RsSalazar::get_instance();
  const DynList<const VectorDesc*> rs_lab = data.search_vectors("rs");

  {
    const DynList<VectorDesc> rs_list = rs_lab.maps<VectorDesc>([&] (auto rs_ptr)
      {
	return data.compute_rs(*rs_ptr, rs_corr); 
      });

    ASSERT_TRUE(rs_list.is_unitarian());

    auto rsv_lab = rs_lab.get_first();
    auto rsv_corr = rs_list.get_first();

    cout << *rsv_lab << endl
	 << rsv_corr << endl;
    const DynList<double> rs =
      { 6.0222179429, 29.2269077329, 41.4399023591, 53.6528969854, 71.5652891040 };

    for (auto it = zip_it(rsv_lab->p, rsv_corr.p, rsv_corr.y, rs);
	 it.has_curr(); it.next())
      {
	auto t = it.get_curr();
	ASSERT_EQ(get<0>(t), get<1>(t));
	ASSERT_NEAR(get<2>(t), get<3>(t), 1e-6);
      }
  }

  data.set_pb(&PbAlShammasi::get_instance(), -544.171681, 1.299236);
  constexpr double c = 0.652887, m = 1.134960;

  {
    const DynList<VectorDesc> rs_list = rs_lab.maps<VectorDesc>([&] (auto rs_ptr)
      {
	return data.compute_rs(*rs_ptr, rs_corr, c, m); 
      });

    auto rsv_lab = rs_lab.get_first();
    auto rsv_corr = rs_list.get_first();
    ASSERT_NEAR(rsv_corr.pb, data.get_pb(*rsv_lab).second.first, 2);
    const DynList<double> rs =
      { 6.129834, 32.466229, 46.327489, 60.188750, 79.500000 };
    for (auto it = zip_it(rsv_corr.y, rs); it.has_curr(); it.next())
      {
	auto t = it.get_curr();
	ASSERT_NEAR(get<0>(t), get<1>(t), 10);
      }
  }
}

TEST_F(FluidTest, rs_stats)
{
  const DynList<const Correlation*> rs_list = data.can_be_applied("rs");

  const DynList<PvtData::VectorStats> rs_stats = rs_list.
    maps<PvtData::VectorStats>([this] (auto ptr) { return data.rs_stats(ptr); });

  data.set_pb(&PbAlShammasi::get_instance(), -833.658951, 1.566572);

  auto str_list = rs_stats.maps<DynList<string>>([] (auto & s)
						 { return s.to_dynlist(); });
  cout << to_string(format_string(str_list)) << endl;

  for (auto it = rs_stats.get_it(); it.has_curr(); it.next())
    {
      auto & s = it.get_curr();
      cout << s.corr_ptr->name << endl;
      for (auto it = s.temps.get_it(); it.has_curr(); it.next())
	{
	  auto p = it.get_curr();
	  auto & t = p.second;
	  cout << "  t = " << p.first << endl;
	  auto l = zip_maps<DynList<string>>([] (auto t)
            {
	      return build_dynlist<string>(to_string(get<0>(t)),
					   to_string(get<1>(t)),
					   to_string(get<2>(t)),
					   to_string(get<3>(t)));
	    }, t.p, t.ylab, t.ycorr, t.ytuned); 
	  l.insert(DynList<string>({"p", "ylab", "ycorr", "tuned"}));
	  cout << shift_lines_to_left(to_string(format_string(l)), 4) << endl
	       << endl;
	}
    }
}

TEST_F(FluidTest, compute_bob)
{
  auto bob_list = data.search_vectors("bob");
  ASSERT_TRUE(bob_list.is_unitarian());

  auto bob = data.compute_bob(*bob_list.get_first(), &BobTotalCFP::get_instance());

  cout << bob << endl;

  data.set_pb(&PbAlShammasi::get_instance(), -544.171681, 1.299236);
  data.set_rs(&RsPerezML::get_instance(), -1.094461, 1.034336);

  bob = data.compute_bob(*bob_list.get_first(), &BobSalazar::get_instance());

  cout << bob << endl;
}

TEST_F(FluidTest, bob_stats)
{
  auto bob_corrs = data.can_be_applied("bob");
  auto stats = bob_corrs.maps<PvtData::VectorStats>
    ([this] (auto ptr) { return data.bob_stats(ptr); });
  auto str = stats.maps<DynList<string>>([] (auto & s) { return s.to_dynlist(); });
  cout << to_string(format_string(str)) << endl
       << endl;

  data.set_pb(&PbAlShammasi::get_instance(), -544.171681, 1.299236);
  data.set_rs(&RsPerezML::get_instance(), -0.965045, 1.029166);

  stats = bob_corrs.maps<PvtData::VectorStats>
    ([this] (auto ptr) { return data.bob_stats(ptr); });
  str = stats.maps<DynList<string>>([] (auto & s) { return s.to_dynlist(); });
  cout << to_string(format_string(str)) << endl
       << endl;

  for (auto it = stats.get_it(); it.has_curr(); it.next())
    {
      auto & s = it.get_curr();
      cout << s.corr_ptr->name << endl;
      for (auto it = s.temps.get_it(); it.has_curr(); it.next())
	{
	  auto p = it.get_curr();
	  auto & t = p.second;
	  cout << "  t = " << p.first << endl;
	  auto l = zip_maps<DynList<string>>([] (auto t)
            {
	      return build_dynlist<string>(to_string(get<0>(t)),
					   to_string(get<1>(t)),
					   to_string(get<2>(t)),
					   to_string(get<3>(t)));
	    }, t.p, t.ylab, t.ycorr, t.ytuned); 
	  l.insert(DynList<string>({"p", "ylab", "ycorr", "tuned"}));
	  cout << shift_lines_to_left(to_string(format_string(l)), 4) << endl
	       << endl;
	}
    }
}

TEST_F(FluidTest, compute_coa)
{
  auto coa_list = data.search_vectors("coa");
  ASSERT_TRUE(coa_list.is_unitarian());

  auto coa_v = *coa_list.get_first();

  auto coa = data.compute_coa(coa_v, &CoaPerezML::get_instance());

  cout << coa << endl
       << endl;

  data.set_pb(&PbAlShammasi::get_instance(), -544.171681, 1.299236);

  coa = data.compute_coa(coa_v, &CoaPerezML::get_instance());

  cout << coa << endl
       << endl;
}

TEST_F(FluidTest, coa_stats)
{
  auto coa_corrs = data.can_be_applied("coa");
  auto stats = coa_corrs.maps<PvtData::VectorStats>
    ([this] (auto ptr) { return data.coa_stats(ptr); });
  auto str = stats.maps<DynList<string>>([] (auto & s) { return s.to_dynlist(); });
  cout << to_string(format_string(str)) << endl
       << endl;

  data.set_pb(&PbAlShammasi::get_instance(), -544.171681, 1.299236);
  data.set_rs(&RsPerezML::get_instance(), -0.965045, 1.029166);
  data.set_bob(&BobTotalCFP::get_instance(), -1.220134, 2.201747);

  stats = coa_corrs.maps<PvtData::VectorStats>
    ([this] (auto ptr) { return data.coa_stats(ptr); });
  str = stats.maps<DynList<string>>([] (auto & s) { return s.to_dynlist(); });
  cout << to_string(format_string(str)) << endl
       << endl;

  for (auto it = stats.get_it(); it.has_curr(); it.next())
    {
      auto & s = it.get_curr();
      cout << s.corr_ptr->name << endl;
      for (auto it = s.temps.get_it(); it.has_curr(); it.next())
	{
	  auto p = it.get_curr();
	  auto & t = p.second;
	  cout << "  t = " << p.first << endl;
	  auto l = zip_maps<DynList<string>>([] (auto t)
            {
	      return build_dynlist<string>(to_string(get<0>(t), 10),
					   to_string(get<1>(t), 10),
					   to_string(get<2>(t), 10),
					   to_string(get<3>(t), 10));
	    }, t.p, t.ylab, t.ycorr, t.ytuned); 
	  l.insert(DynList<string>({"p", "ylab", "ycorr", "tuned"}));
	  cout << shift_lines_to_left(to_string(format_string(l)), 4) << endl
	       << endl;
	}
    }
}

TEST_F(FluidTest, compute_boa)
{
  
  data.set_pb(&PbSalazar::get_instance(), -107.106998, 1.004085);
  data.set_rs(&RsSalazar::get_instance(), -2.572312, 1.206391);
  data.set_bob(&BobTotalCFP::get_instance(), -0.768736, 1.764331);
  data.set_coa(&CoaPerezML::get_instance(), 0.000001, 0.953269);

  auto boa_list = data.search_vectors("boa");
  ASSERT_TRUE(boa_list.is_unitarian());

  auto boav = *boa_list.get_first();

  auto boa = data.compute_boa(boav, &BoaMcCain::get_instance());
  cout << boa << endl;

  auto coa = data.rm_vector(125, "coa");

  cout << data.compute_boa(boav, &BoaMcCain::get_instance()) << endl;

  data.rm_coa_correlation();

  ASSERT_THROW(data.compute_boa(boav, &BoaMcCain::get_instance()),
	       VarNameNotFound);

  data.add_vector(coa);

  cout << data.compute_boa(boav, &BoaMcCain::get_instance()) << endl;
}


TEST_F(FluidTest, boa_stats)
{
  auto boa_corrs = data.can_be_applied("boa");
  auto stats = boa_corrs.maps<PvtData::VectorStats>
    ([this] (auto ptr) { return data.boa_stats(ptr); });
  auto str = stats.maps<DynList<string>>([] (auto & s) { return s.to_dynlist(); });
  cout << to_string(format_string(str)) << endl
       << endl;
  
  data.set_pb(&PbAlShammasi::get_instance(), -544.171681, 1.299236);
  data.set_rs(&RsPerezML::get_instance(), -0.965045, 1.029166);
  data.set_bob(&BobTotalCFP::get_instance(), -1.220134, 2.201747);
  data.set_coa(&CoaPerezML::get_instance(), 0.000001, 0.953269);

  stats = boa_corrs.maps<PvtData::VectorStats>
    ([this] (auto ptr) { return data.boa_stats(ptr); });
  str = stats.maps<DynList<string>>([] (auto & s) { return s.to_dynlist(); });
  cout << to_string(format_string(str)) << endl
       << endl;

  for (auto it = stats.get_it(); it.has_curr(); it.next())
    {
      auto & s = it.get_curr();
      cout << s.corr_ptr->name << endl;
      for (auto it = s.temps.get_it(); it.has_curr(); it.next())
	{
	  auto p = it.get_curr();
	  auto & t = p.second;
	  cout << "  t = " << p.first << endl;
	  auto l = zip_maps<DynList<string>>([] (auto t)
            {
	      return build_dynlist<string>(to_string(get<0>(t), 10),
					   to_string(get<1>(t), 10),
					   to_string(get<2>(t), 10),
					   to_string(get<3>(t), 10));
	    }, t.p, t.ylab, t.ycorr, t.ytuned); 
	  l.insert(DynList<string>({"p", "ylab", "ycorr", "tuned"}));
	  cout << shift_lines_to_left(to_string(format_string(l)), 4) << endl
	       << endl;
	}
    }
}

TEST_F(FluidTest, uod_stats)
{  
  auto uod_corrs = data.can_be_applied("uod");
  auto s = uod_corrs.maps<PvtData::ConstStats>([this] (auto ptr)
					       { return data.uod_stats(ptr); });
  auto str = s.maps<DynList<string>>([] (auto & s) { return s.to_dynlist(); });

  cout << to_string(format_string(str)) << endl
       << endl;

  for (auto it = s.get_it(); it.has_curr(); it.next())
    {
      auto & s = it.get_curr();
      const double & c = CorrStat::c(s.desc);
      const double & m = CorrStat::m(s.desc);
      cout << s.corr_ptr->name << " c = " << c << " m = " << m << endl;
      for (auto it = zip_it(s.t, s.ylab, s.ycorr, s.ytuned); it.has_curr(); it.next())
	{
	  auto t = it.get_curr();
	  cout << "  t = " << get<0>(t) << " uod = " << get<1>(t) << " uodcorr = "
	       << get<2>(t) << " tuned = " << get<3>(t) << endl;
	}
    }

  data.set_pb(&PbSalazar::get_instance(), -107.106998, 1.004085);
  s = uod_corrs.maps<PvtData::ConstStats>([this] (auto ptr)
					       { return data.uod_stats(ptr); });
  str = s.maps<DynList<string>>([] (auto & s) { return s.to_dynlist(); });

  cout << to_string(format_string(str)) << endl
       << endl;

  for (auto it = s.get_it(); it.has_curr(); it.next())
    {
      auto & s = it.get_curr();
      const double & c = CorrStat::c(s.desc);
      const double & m = CorrStat::m(s.desc);
      cout << s.corr_ptr->name << " c = " << c << " m = " << m << endl;
      for (auto it = zip_it(s.t, s.ylab, s.ycorr, s.ytuned); it.has_curr(); it.next())
	{
	  auto t = it.get_curr();
	  cout << "  t = " << get<0>(t) << " uod = " << get<1>(t) << " uodcorr = "
	       << get<2>(t) << " tuned = " << get<3>(t) << endl;
	}
    }
}

TEST_F(FluidTest, compute_uob)
{
  data.set_pb(&PbSalazar::get_instance(), -107.106998, 1.004085);
  data.set_rs(&RsSalazar::get_instance(), -2.572312, 1.206391);
  data.set_bob(&BobTotalCFP::get_instance(), -0.768736, 1.764331);
  data.set_coa(&CoaPerezML::get_instance(), 0.000001, 0.953269);
  data.set_boa(&BoaMcCain::get_instance(), -0.316909, 1.118688);

  auto uob_list = data.search_vectors("uob");
  ASSERT_EQ(uob_list.size(), 3);

  auto uobv = *uob_list.get_first();

  auto uob = data.compute_uob(uobv, &UobPerezML::get_instance());
  cout << "uob completo" << endl
       << uob << endl
       << endl
       << "uob calibrado completo" << endl
       << data.compute_uob(uobv, &UobKartoatmodjoSchmidt::get_instance(),
			   -11744.928482, 59.551349) << endl;

  auto rs = data.rm_vector(125, "rs");

  cout << "uob sin rs pero usando correlación" << endl
       << data.compute_uob(uobv, &UobPerezML::get_instance()) << endl
       << endl;

  data.rm_rs_correlation();

  ASSERT_THROW(data.compute_uob(uobv, &UobPerezML::get_instance()),
	       VarNameNotFound);

  data.set_rs(&RsSalazar::get_instance(), -2.572312, 1.206391);

  auto bob = data.rm_vector(125, "bob");

  cout << "uob sin rs y sin bob pero usando correlaciones" << endl
       << data.compute_uob(uobv, &UobPerezML::get_instance()) << endl
       << endl;

  data.rm_bob_correlation();

  ASSERT_THROW(data.compute_uob(uobv, &UobPerezML::get_instance()),
	       VarNameNotFound);
}

TEST_F(FluidTest, uob_stats)
{  
  data.set_pb(&PbAlShammasi::get_instance(), -544.171681, 1.299236);
  data.set_rs(&RsPerezML::get_instance(), -0.965045, 1.029166);
  data.set_bob(&BobTotalCFP::get_instance(), -1.220134, 2.201747);
  data.set_coa(&CoaPerezML::get_instance(), 0.000001, 0.953269);
  data.set_boa(&BoaMcCain::get_instance(), -0.416240, 1.151065);
  data.set_uod(&UodPerezML::get_instance(), 1155.463174, -0.078120);

  auto uob_corrs = data.can_be_applied("uob");

  auto s = uob_corrs.maps<PvtData::VectorStats>([this] (auto ptr)
						{ return data.uob_stats(ptr); });

  auto str = s.maps<DynList<string>>([] (auto & s) { return s.to_dynlist(); });

  cout << to_string(format_string(str)) << endl;

  for (auto it = s.get_it(); it.has_curr(); it.next())
    {
      auto & s = it.get_curr();
      cout << s.corr_ptr->name << endl;
      for (auto it = s.temps.get_it(); it.has_curr(); it.next())
	{
	  auto p = it.get_curr();
	  auto & t = p.second;
	  cout << "  t = " << p.first << endl;
	  auto l = zip_maps<DynList<string>>([] (auto t)
            {
	      return build_dynlist<string>(to_string(get<0>(t), 10),
					   to_string(get<1>(t), 10),
					   to_string(get<2>(t), 10),
					   to_string(get<3>(t), 10));
	    }, t.p, t.ylab, t.ycorr, t.ytuned); 
	  l.insert(DynList<string>({"p", "ylab", "ycorr", "tuned"}));
	  cout << shift_lines_to_left(to_string(format_string(l)), 4) << endl
	       << endl;
	}
    }
}

TEST_F(FluidTest, compute_uoa)
{
  auto uoa_list = data.search_vectors("uoa");
  ASSERT_EQ(uoa_list.size(), 3);

  for (auto it = uoa_list.get_it(); it.has_curr(); it.next())
    {
      auto uoa = it.get_curr();
      cout << *uoa << endl
	   << endl;
      ASSERT_GT(uoa->y.get_first(), uoa->uobp);
      auto uoa_corr = data.compute_uoa(*uoa, &UoaDeGhettoEtAl::get_instance(),
				       32.567066, 0.890956);

      cout << "By correlation" << endl
	   << uoa_corr << endl;
    }
}

TEST_F(FluidTest, uoa_stats)
{
  
  auto uoa_corrs = data.can_be_applied("uoa");

  auto s = uoa_corrs.maps<PvtData::VectorStats>([this] (auto ptr)
						{ return data.uoa_stats(ptr); });

  auto str = s.maps<DynList<string>>([] (auto & s) { return s.to_dynlist(); });

  cout << to_string(format_string(str)) << endl;

  data.set_pb(&PbSalazar::get_instance(), -107.106998, 1.004085);
  data.set_rs(&RsSalazar::get_instance(), -2.572312, 1.206391);
  data.set_bob(&BobTotalCFP::get_instance(), -0.768736, 1.764331);
  data.set_coa(&CoaPerezML::get_instance(), 0.000001, 0.953269);
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

// TODO probar construcción de CorrInput
// TODO: probar en merge_with que valores de bobp uod y uobp den correctos
// TODO: probar to_parpair en  vector
// Todo: parallel_vectors

// TODO 1ro debo programar inputing

// Luego debo verificar parallel vectors

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
  {
    auto l = data.can_be_applied("pb");
  }
  return;
  {
    auto l = data.can_be_applied("rs");
    l.for_each([] (auto ptr) { cout << ptr->name << endl; });

    cout << "****************************************************************" << endl;

    Correlation::array().filter([] (auto ptr) { return ptr->target_name() == "rs"; }).
      for_each([] (auto ptr) { cout << ptr->name << endl; });
  }
}


TEST_F(FluidTest, list_restrictions)
{

}

TEST_F(FluidTest, get_vectors)
{

}


TEST_F(FluidTest, build_samples) // Revisar si es necesaria
{

}

TEST_F(FluidTest, iapply)
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


