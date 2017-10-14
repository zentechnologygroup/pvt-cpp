
# include <gmock/gmock.h>

# include <metadata/pvt-calibrate.H>

using namespace std;
using namespace testing;
using namespace std;

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

  Array<double> parray = { 100, 200, 300 };
  Array<double> rs100 = {16, 70, 120};
  Array<double> rs200 = {20, 90, 140};

  data.add_vector(100, 820, -1, -1, -1, parray, psia::get_instance(),
		  "rs", rs100, SCF_STB::get_instance());

  ASSERT_THROW(data.add_vector(100, 820, -1, -1, -1, parray,
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
		      parray, vptr->p));
  ASSERT_TRUE(zip_all([] (auto t) { return get<0>(t) == get<1>(t); },
		      rs100, vptr->y));

  data.add_vector(200, 1120, -1, -1, -1, parray, psia::get_instance(),
		  "rs", rs200, SCF_STB::get_instance());
  ASSERT_THROW(data.add_vector(200, 1120, -1, -1, -1, parray,
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
  		      parray, vptr->p));
  ASSERT_TRUE(zip_all([] (auto t) { return get<0>(t) == get<1>(t); },
  		      rs100, vptr->y));

  vptr = vlist.get_last();
  
  ASSERT_EQ(vptr->yname, "rs");
  ASSERT_EQ(vptr->t, 200);
  ASSERT_EQ(vptr->pb, 1120);
  ASSERT_EQ(vptr->punit, &psia::get_instance());
  ASSERT_EQ(vptr->yunit, &SCF_STB::get_instance());
  ASSERT_TRUE(zip_all([] (auto t) { return get<0>(t) == get<1>(t); },
  		      parray, vptr->p));
  ASSERT_TRUE(zip_all([] (auto t) { return get<0>(t) == get<1>(t); },
  		      rs200, vptr->y));
}

struct Fluid5 : public Test
{
  PvtData data;
  Fluid5()
  {
    data.add_const("api", 8.3, Api::get_instance());
    data.add_const("co2", 0.86, MolePercent::get_instance());
    data.add_const("n2", 0.19, MolePercent::get_instance());
    data.add_const("psep", 100, psia::get_instance());
    data.add_const("rsb", 79.5, SCF_STB::get_instance());
    data.add_const("yg", 0.608, Sgg::get_instance());
    data.add_const("tsep", 100, Fahrenheit::get_instance());

    data.add_vector(125, 820, -1, -1, -1, {820, 600, 450, 300, 15},
		    psia::get_instance(), "rs", {80, 62, 47, 35, 0},
		    SCF_STB::get_instance());
  }
};

TEST(PvtData, are_all_correlations_defined_and_missing)
{

}

TEST_F(Fluid5, simple_test)
{

}
