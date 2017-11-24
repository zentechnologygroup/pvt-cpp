
# include <gmock/gmock.h>

# include <tclap/CmdLine.h>
# include <ah-zip.H>

# include "calibrate.H"

using namespace std;
using namespace testing;
using namespace Aleph;
using namespace TCLAP;

TEST(VectorDesc, simple_operations)
{
  {
    VectorDesc d;
    ASSERT_FALSE(d.is_valid());
    ASSERT_ANY_THROW(d.pre_validate());
  }

  {
    ASSERT_THROW(VectorDesc(0, "foo"), InvalidRange);

    // invalid temp
    ASSERT_THROW(VectorDesc(0, 820, 1.003, 1e5, 1e1, { 10, 20, 30 },
			    &psig::get_instance(), "rs", { 1e-6, 2e-6, 3e-6 },
			    &SCF_STB::get_instance()), InvalidRange);

    // invalid pb
    ASSERT_THROW(VectorDesc(125, 0, 1.003, 1e5, 1e1, { 10, 20, 30 },
			    &psig::get_instance(), "rs", { 1e-6, 2e-6, 3e-6 },
			    &SCF_STB::get_instance()), InvalidRange);

    // dimension of p array is greater than zero
    ASSERT_THROW(VectorDesc(125, 820, 1.003, 1e5, 1e1, { 10 },
			    &psig::get_instance(), "rs", { 1e-6, 2e-6, 3e-6 },
			    &SCF_STB::get_instance()), LengthMismatch);

    // dimension of p and y must be equal; p is incomplete
    ASSERT_THROW(VectorDesc(125, 820, 1.003, 1e5, 1e1, { 10, 820 },
			    &psig::get_instance(), "rs", { 1e-6, 2e-6, 3e-6 },
			    &SCF_STB::get_instance()), LengthMismatch);

    // dimension of p and y must be equal; y is incomplete
    ASSERT_THROW(VectorDesc(125, 820, 1.003, 1e5, 1e1, { 10, 20, 820 },
			    &psig::get_instance(), "rs", { 1e-6, 2e-6 },
			     &SCF_STB::get_instance()), LengthMismatch);

    // p must be sorted
    ASSERT_THROW(VectorDesc(125, 820, 1.003, 1e5, 1e1, { 30, 10, 820 },
			    &psig::get_instance(), "rs", { 1e-6, 2e-6, 3e-6 },
			    &SCF_STB::get_instance()), SamplesUnsorted);

    ASSERT_NO_THROW(VectorDesc(125, 820, 1.003, 1e5, 1e1, { 10, 20, 30, 820 },
			       &psig::get_instance(), "rs",
			       { 10-6, 20, 30, 40 }, &SCF_STB::get_instance()));

    ASSERT_THROW(VectorDesc(125, PVT_INVALID_VALUE, 1.003, 1e5, 1e1,
			    { 10, 20, 30 }, &psig::get_instance(), "uo",
			    { 1e-6, 2e-6, 3e-6 }, &SCF_STB::get_instance()),
		 InvalidUnit);

    ASSERT_THROW(VectorDesc(125, PVT_INVALID_VALUE, 1.003, 1e5, 1e1, { 10, 20, 30 },
			    &psig::get_instance(), "rs",
			    { 1e-6, 2e-6, 3e-6 }, &SCF_STB::get_instance()),
		 InvalidRange);

    ASSERT_FALSE(VectorDesc(100, "rs").is_valid());
  }
}

TEST(VectorDesc, operator_less_than)
{
  VectorDesc d1 = { 100, "rs" };
  VectorDesc d2 = { 100.001, "rs" };
  VectorDesc d3 = { 100, "uob" };
  VectorDesc d4 = { 100.001, "uob" };

  ASSERT_TRUE(d1 < d2);
  ASSERT_TRUE(d1 < d3);
  ASSERT_TRUE(d3 < d4);
  ASSERT_TRUE(d2 < d3);
  ASSERT_TRUE(d1 < d4);
}

TEST(VectorDesc, parallel)
{
  ASSERT_FALSE(VectorDesc(100, "rs").is_parallel(VectorDesc(200, "bob")));
  ASSERT_TRUE(VectorDesc(100, "rs").is_parallel(VectorDesc(100, "bob")));

  VectorDesc d1(125, 820, 1.003, 1e5, 1e1, { 10, 20, 30, 820 },
		&psig::get_instance(), "rs", { 1e-6, 2e-6, 3e-6, 4e-6 },
		&SCF_STB::get_instance());
  VectorDesc d2(125, 820, 1.003, 1e5, 1e1, { 10, 20, 30, 820  },
		&psig::get_instance(), "rs", { 1e-6, 2e-6, 3e-6, 4e-6 },
		&SCF_STB::get_instance());
  VectorDesc d3(125, 820, 1.003, 1e5, 1e1, { 10, 20, 31, 820 },
		&psig::get_instance(), "rs", { 1e-6, 2e-6, 3e-6, 4e-6 },
		&SCF_STB::get_instance());
  VectorDesc d4(125, 820, 1.003, 1e5, 1e1, { 10, 21, 30, 820 },
		&psig::get_instance(), "rs",
		{ 1e-6, 2e-6, 3e-6, 4e-6 }, &SCF_STB::get_instance());

  ASSERT_TRUE(d1.is_parallel(d2));
  ASSERT_FALSE(d1.is_parallel(d3));
  ASSERT_FALSE(d3.is_parallel(d4));
}

struct SimpleVector : public Test
{
  VectorDesc v;
  SimpleVector() : v(125, 820, 1.003, 1e5, 1e1, { 10, 20, 30, 40, 820 },
		     &psig::get_instance(), "rs",
		     { 1e-6, 2e-6, 3e-6, 4e-6, 5e-6 }, &SCF_STB::get_instance()) {}
};

TEST_F(SimpleVector, y_operations)
{
  ASSERT_THROW(v.get_yindex(0), ValueNotFound);
  ASSERT_THROW(v.get_yindex(1), ValueNotFound);
  ASSERT_EQ(v.get_yindex(1e-6), 0);
  ASSERT_EQ(v.get_yindex(2e-6), 1);
  ASSERT_EQ(v.get_yindex(3e-6), 2);
  ASSERT_EQ(v.get_yindex(4e-6), 3);
  ASSERT_EQ(v.get_yindex(5e-6), 4);

  ASSERT_EQ(v.gety(10), 1e-6);
  ASSERT_EQ(v.gety(20), 2e-6);
  ASSERT_EQ(v.gety(30), 3e-6);
  ASSERT_EQ(v.gety(40), 4e-6);
  ASSERT_EQ(v.gety(820), 5e-6);

  ASSERT_NEAR(v.gety(15), 1.5e-6, 1e-15); // interpolation test for gety

  ASSERT_NEAR(v.gety(5), 0.5e-6, 1e-15); // extrapolation test by the left

  ASSERT_NEAR(v.gety(35), 3.5e-6, 1e-15); // extrapolation test by the right

  // test gety on a container
  const DynList<double> y = v.gety(build_dynlist<double>(5, 10, 15, 35));
  const DynList<double> expected = { 0.5e-6, 1e-6, 1.5e-6, 3.5e-6 };
  for (auto it = zip_it(y, expected); it.has_curr(); it.next())
    {
      auto t = it.get_curr();
      ASSERT_NEAR(get<0>(t), get<1>(t), 1e-15);
    }
}

TEST(VectorDesc, gety_limits)
{
  VectorDesc v(125, 29000, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
	       PVT_INVALID_VALUE, { 0, 29000 }, &psig::get_instance(), "rs",
	       { 0, 1e9 }, &SCF_STB::get_instance());
  const double low_p = nextafter(v.punit->min_val, numeric_limits<double>::min());
  const double high_p = nextafter(v.punit->max_val, numeric_limits<double>::max());

  ASSERT_NEAR(v.gety(low_p), v.yunit->min_val, 1e-20);
  ASSERT_NEAR(v.gety(high_p), v.yunit->max_val, 1e-20);
}

TEST(VectorDesc, merge_with)
{
  VectorDesc v1(125, 29000, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		PVT_INVALID_VALUE, { 0, 100, 1000, 10000, 18000, 29000 },
		&psig::get_instance(), "rs",
		{ 0, 10, 1000, 10000, 18000, 34000}, &SCF_STB::get_instance());
  VectorDesc v2(125, 29000, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		PVT_INVALID_VALUE, { 0, 140, 1040, 10800, 19000, 29000,
		    100, 300, 2040, 15800, 24000, 29900},
		&psig::get_instance(), "coa",
		{ 1e-2, 5e-3, 4e-4, 3e-5, 2e-6, 1e-8 }, &psia_1::get_instance());

  cout << v1.merge_with(v2) << endl;
}

TEST(VectorDesc, uo_split)
{
  VectorDesc
    v(200, 3000, PVT_INVALID_VALUE, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
      {200, 500, 800, 1000, 1100, 1300, 1500, 1800, 2100, 2400, 2700, 3000},
      &psig::get_instance(), "uo",
      {404.6, 345.3, 278, 256, 260, 270, 279.8, 302.4, 316.3, 335.0, 349.7, 370},
      &CP::get_instance());

  auto p = v.split_uo();
  auto & p_below = p.first.p;
  auto & uob = p.first.y;
  auto & p_above = p.second.p;
  auto & uoa = p.second.y;

  ASSERT_EQ(p.first.pb, p.second.pb);
  ASSERT_EQ(p.first.p.get_last(), p.first.pb);
  ASSERT_EQ(p.first.uod, p.second.uod);
  ASSERT_EQ(p.first.uobp, p.second.uobp);

  auto & pb = p.first.pb;
  auto & uod = p.first.uod;
  auto & uobp = p.first.uobp;  

  ASSERT_TRUE(is_sorted(p_below));
  ASSERT_TRUE(is_sorted(p_above));
  ASSERT_TRUE(is_inversely_sorted(uob));
  ASSERT_TRUE(is_sorted(uoa));

  const double max_val = numeric_limits<double>::max();
  ASSERT_EQ(p_below.get_last(), pb);
  ASSERT_EQ(p_above.get_first(), nextafter(pb, max_val));
  ASSERT_EQ(uob.get_first(), uod);
  ASSERT_EQ(uob.get_last(), uobp);
  ASSERT_EQ(uoa.get_first(), nextafter(uobp, max_val));
}

struct UoBoundTest : public TestWithParam<ArrayP>
{
  const pair<Array<double>, Array<double>> & p = GetParam();
  const Array<double> & pressure = p.first;
  const Array<double> & uo = p.second;
  VectorDesc v;
  UoBoundTest()
    : v(200, 3000, PVT_INVALID_VALUE, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
	pressure, &psig::get_instance(), "uo", uo, &CP::get_instance()) {}
};

TEST_P(UoBoundTest, uo_split)
{
  auto p = v.split_uo();
  auto & p_below = p.first.p;
  auto & uob = p.first.y;
  auto & p_above = p.second.p;
  auto & uoa = p.second.y;

  EXPECT_EQ(p.first.pb, p.second.pb);
  EXPECT_EQ(p.first.p.get_last(), p.first.pb);
  EXPECT_EQ(p.first.uod, p.second.uod);
  EXPECT_EQ(p.first.uobp, p.second.uobp);

  auto & pb = p.first.pb;
  auto & uod = p.first.uod;
  auto & uobp = p.first.uobp;  

  EXPECT_TRUE(is_sorted(p_below));
  EXPECT_TRUE(is_sorted(p_above));
  EXPECT_TRUE(is_inversely_sorted(uob));
  EXPECT_TRUE(is_sorted(uoa));

  const double max_val = numeric_limits<double>::max();
  EXPECT_EQ(p_below.get_last(), pb);
  EXPECT_EQ(p_above.get_first(), nextafter(pb, max_val));
  EXPECT_EQ(uob.get_first(), uod);
  EXPECT_EQ(uob.get_last(), uobp);
  EXPECT_EQ(uoa.get_first(), nextafter(uobp, max_val));
}

INSTANTIATE_TEST_CASE_P
(uo_bound_conditions, UoBoundTest,
 ::testing::Values
 (ArrayP(build_array<double>(200, 500, 600, 900),
	 build_array<double>(404.6, 345.3, 350, 390)),
  ArrayP(build_array<double>(50, 100, 200, 300, 400),
	 build_array<double>(196.7, 173.33, 153.58, 160.72, 200))));

struct UoTestDesc
{
  Array<double> p, uo;
  UoTestDesc(const DynList<double> & __p, const DynList<double> & __uo)
    : p(__p), uo(__uo) {}
};

vector<ArrayP> uo_list =
  to_vector(range(33).maps<ArrayP>([] (auto) { return generate_random_uo(); }));

INSTANTIATE_TEST_CASE_P(random_uo, UoBoundTest, ValuesIn(uo_list));

TEST(VectorCtor, rs)
{
  // valid configurations
  ASSERT_NO_THROW(VectorDesc(125, 820, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
			     PVT_INVALID_VALUE, {820, 600, 450, 300, 15},
			     &psig::get_instance(), "rs",
			     {80, 62, 47, 35, 0}, &SCF_STB::get_instance()));

  ASSERT_NO_THROW(VectorDesc(125, 820, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
			     PVT_INVALID_VALUE, {15, 300, 450, 600, 820},
			     &psig::get_instance(), "rs",
			     {0, 35, 47, 62, 80}, &SCF_STB::get_instance()));

  // Invalid configurations

  // rs values are not sorted
  ASSERT_THROW(VectorDesc(125, 820, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
			  PVT_INVALID_VALUE, {15, 300, 450, 600, 820},
			  &psig::get_instance(), "rs",
			  {0, 47, 35, 62, 80}, &SCF_STB::get_instance()),
	       SamplesUnsorted);

  // pressure values are not sorted
  ASSERT_THROW(VectorDesc(125, 820, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
			  PVT_INVALID_VALUE, {15, 300, 600, 450, 820},
			  &psig::get_instance(), "rs",
			  {0, 35, 47, 62, 80}, &SCF_STB::get_instance()),
	       SamplesUnsorted);

  // last pressure value i p array is not the pb
  ASSERT_THROW(VectorDesc(125, 820, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
			  PVT_INVALID_VALUE, {15, 300, 450, 600, 820.1},
			  &psig::get_instance(), "rs",
			  {0, 35, 47, 62, 80}, &SCF_STB::get_instance()),
	       PressureMismatch);

  // a rs value is out of unit range
  ASSERT_THROW(VectorDesc(125, 820, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
			  PVT_INVALID_VALUE, {15, 300, 450, 600, 820},
			  &psig::get_instance(), "rs",
			  {-5.1e7, 35, 47, 62, 80}, &SCF_STB::get_instance()),
	       OutOfUnitRange);

  // a p value is out of unit range
  ASSERT_THROW(VectorDesc(125, 35000, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
			  PVT_INVALID_VALUE, {15, 300, 450, 600, 35000},
			  &psig::get_instance(), "rs",
			  {-5.1e7, 35, 47, 62, 80}, &SCF_STB::get_instance()),
	       OutOfUnitRange);

  // invalid unit for pressure
  ASSERT_THROW(VectorDesc(125, 820, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
			  PVT_INVALID_VALUE, {820, 600, 450, 300, 15},
			  &psia_1::get_instance(), "rs",
			  {80, 62, 47, 35, 0}, &SCF_STB::get_instance()),
	       InvalidUnit);
  // invalid unit for rs
  ASSERT_THROW(VectorDesc(125, 820, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
			  PVT_INVALID_VALUE, {15, 300, 450, 600, 820},
			  &psig::get_instance(), "rs",
			  {-5.1e7, 35, 47, 62, 80}, &STB_MMscf::get_instance()),
	       InvalidUnit);
}

TEST(VectorCtor, bob)
{
  // valid configurations
  ASSERT_NO_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE,
			     PVT_INVALID_VALUE, {600, 450, 300, 15},
			     &psig::get_instance(), "bob",
			     {1.0865, 1.0790, 1.0713, 1.0228},
			     &RB_STB::get_instance()));

  ASSERT_NO_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE,
			     PVT_INVALID_VALUE, {15, 300, 450, 600},
  			     &psig::get_instance(), "bob",
  			     {1.0228, 1.0713, 1.0790, 1.0865},
			     &RB_STB::get_instance()));

   ASSERT_NO_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE,
			      PVT_INVALID_VALUE, {820, 600, 450, 300, 15},
			     &psig::get_instance(), "bob",
			      {1.0919, 1.0865, 1.0790, 1.0713, 1.0228},
			     &RB_STB::get_instance()));

   ASSERT_NO_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE,
			      PVT_INVALID_VALUE, {15, 300, 450, 600, 820},
  			     &psig::get_instance(), "bob",
			      {1.0228, 1.0713, 1.0790, 1.0865, 1.0919},
			     &RB_STB::get_instance()));
   
  // Invalid configurations

   // samples has pb but not bobp
   ASSERT_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE,
			   PVT_INVALID_VALUE, {15, 300, 450, 600, 820},
			   &psig::get_instance(), "bob",
			   {1.0228, 1.0713, 1.0790, 1.0865, 1.091},
			   &RB_STB::get_instance()), SampleInvalid);

   // samples has bobp but not pb
   ASSERT_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE,
			   PVT_INVALID_VALUE, {15, 300, 450, 600, 820.1},
			   &psig::get_instance(), "bob",
			   {1.0228, 1.0713, 1.0790, 1.0865, 1.0919},
			   &RB_STB::get_instance()), SampleInvalid);

   // samples has bob greater than bobp 
   ASSERT_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE,
			   PVT_INVALID_VALUE, {15, 300, 450, 600, 819, 820},
			   &psig::get_instance(), "bob",
			   {1.0228, 1.0713, 1.0790, 1.0865, 1.0919, 1.09191},
			   &RB_STB::get_instance()), SampleInvalid);

   // samples has pb greater than pb
   ASSERT_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE,
			   PVT_INVALID_VALUE, {15, 300, 450, 600, 820, 830},
			   &psig::get_instance(), "bob",
			   {1.0228, 1.0713, 1.0790, 1.0865, 1.091, 1.0919},
			   &RB_STB::get_instance()), PressureMismatch);

  // bob values are not sorted
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE,
			  PVT_INVALID_VALUE, {15, 300, 450, 600},
  			  &psig::get_instance(), "bob",
  			  {1.0228, 1.0713, 1.0865, 1.0790},
			  &RB_STB::get_instance()), SamplesUnsorted);

  // pressure values are not sorted
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE,
			  PVT_INVALID_VALUE, {15, 300, 600, 450},
  			  &psig::get_instance(), "bob",
  			  {1.0228, 1.0713, 1.0790, 1.0865},
			  &RB_STB::get_instance()), SamplesUnsorted);

  // a bob value is out of unit range
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE,
			  PVT_INVALID_VALUE, {15, 300, 450, 600},
  			  &psig::get_instance(), "bob",
  			  {1.0228, 1.0713, 1.0790, 11.0919}, // <
			  &RB_STB::get_instance()), OutOfUnitRange);

  // a p value is out of unit range
  ASSERT_THROW(VectorDesc(125, 35000, 1.0919, PVT_INVALID_VALUE,
			  PVT_INVALID_VALUE, {15, 300, 450, 600, 35000},
  			  &psig::get_instance(), "bob",
  			  {-5.1e7, 35, 47, 62, 80}, &RB_STB::get_instance()),
  	       OutOfUnitRange);

  // invalid unit for pressure
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE,
			  PVT_INVALID_VALUE, {820, 600, 450, 300, 15},
  			  &psia_1::get_instance(), "bob",
  			  {80, 62, 47, 35, 0}, &RB_STB::get_instance()),
  	       InvalidUnit);
  // invalid unit for bob
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE,
			  PVT_INVALID_VALUE, {15, 300, 450, 600, 820},
  			  &psig::get_instance(), "bob",
  			  {-5.1e7, 35, 47, 62, 80}, &STB_MMscf::get_instance()),
  	       InvalidUnit);
}

TEST(VectorCtor, coa)
{
  // valid configurations
  ASSERT_NO_THROW(VectorDesc(125, 820, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
			     PVT_INVALID_VALUE,
			     {3000, 2800, 2600, 2400, 2200, 2000, 1800,
			      1600, 1400, 1200, 1000, 2800, 2600, 2400, 2200,
			      2000, 1800, 1600, 1400, 1200, 1000, 820},
			     &psig::get_instance(), "coa",
			     {3.76E-06, 4.07E-06, 4.28E-06, 4.45E-06, 4.62E-06,
			      4.76E-06, 4.86E-06, 5.24E-06, 5.51E-06, 5.61E-06,
			      6.00E-06}, &psia_1::get_instance()));

  ASSERT_NO_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE,
			     PVT_INVALID_VALUE,
			     DynList<double>({3000, 2800, 2600, 2400, 2200, 2000, 1800,
				   1600, 1400, 1200, 1000, 2800, 2600, 2400, 2200,
				   2000, 1800, 1600, 1400, 1200, 1000, 820}).rev(),
  			     &psig::get_instance(), "coa",
  			     DynList<double>({3.76E-06, 4.07E-06, 4.28E-06, 4.45E-06,
				   4.62E-06, 4.76E-06, 4.86E-06, 5.24E-06, 5.51E-06,
				   5.61E-06, 6.00E-06}).rev(),
			     &psia_1::get_instance()));

  // Invalid configurations

  // coa values are not sorted
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
			  {3000, 2800, 2600, 2400, 2200, 2000, 1800,
			      1600, 1400, 1200, 1000, 2800, 2600, 2400, 2200,
			      2000, 1800, 1600, 1400, 1200, 1000, 820},
  			  &psig::get_instance(), "coa",
  			  {3.76E-06, 4.07E-06, 4.28E-06,  4.62E-06, 4.45E-06,
			      4.76E-06, 4.86E-06, 5.24E-06, 5.61E-06, 5.51E-06,
			      6.00E-06},
			  &psia_1::get_instance()), SamplesUnsorted);

  // pressure values are not sorted
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
			  {3000, 2800, 2600, 2400, 2200, 2000, 1800,
			      1600, 1400, 1200, 1000, 2800, 2600, 2400, 2200,
			      1800, 1600, 1400, 1200, 2000, 1000, 820},
  			  &psig::get_instance(), "coa",
  			  {3.76E-06, 4.07E-06, 4.28E-06, 4.45E-06, 4.62E-06,
			      4.76E-06, 4.86E-06, 5.24E-06, 5.51E-06, 5.61E-06,
			      6.00E-06},
			  &psia_1::get_instance()), SamplesUnsorted);

  // pressure array size is unexpected
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
			  {3000, 2800, 2600, 2400, 2200, 2000, 1800,
			      1600, 1400, 1200, 1000, 2800, 2600, 2400, 2200,
			      2000, 1800, 1600, 1400, 1200, 1100, 1000, 820},
  			  &psig::get_instance(), "coa",
			  {3.76E-06, 4.07E-06, 4.28E-06, 4.45E-06, 4.62E-06,
			      4.76E-06, 4.86E-06, 5.24E-06, 5.51E-06, 5.61E-06,
			      6.00E-06},
			  &psia_1::get_instance()), LengthMismatch);

  // a coa value is out of unit range
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
			  {3000, 2800, 2600, 2400, 2200, 2000, 1800,
			      1600, 1400, 1200, 1000, 2800, 2600, 2400, 2200,
			      2000, 1800, 1600, 1400, 1200, 1000, 820},
  			  &psig::get_instance(), "coa",
			  {3.76E-06, 4.07E-06, 4.28E-06, 4.45E-06, 4.62E-06,
			      4.76E-06, 4.86E-06, 5.24E-06, 5.51E-06, 5.61E-06,
			      6.00E-01}, // <--
			  &psia_1::get_instance()), OutOfUnitRange);

  // a p value is out of unit range
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
			  {40000, 2800, 2600, 2400, 2200, 2000, 1800,
			      1600, 1400, 1200, 1000, 2800, 2600, 2400, 2200,
			      2000, 1800, 1600, 1400, 1200, 1000, 820},
  			  &psig::get_instance(), "coa",
  			  {3.76E-06, 4.07E-06, 4.28E-06, 4.45E-06, 4.62E-06,
			      4.76E-06, 4.86E-06, 5.24E-06, 5.51E-06, 5.61E-06,
			      6.00E-01}, 
			  &psia_1::get_instance()),
  	       OutOfUnitRange);

  // invalid unit for pressure
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
			  {3000, 2800, 2600, 2400, 2200, 2000, 1800,
			      1600, 1400, 1200, 1000, 2800, 2600, 2400, 2200,
			      2000, 1800, 1600, 1400, 1200, 1000, 820},
  			  &psia_1::get_instance(), "coa",
  			  {3.76E-06, 4.07E-06, 4.28E-06, 4.45E-06, 4.62E-06,
			      4.76E-06, 4.86E-06, 5.24E-06, 5.51E-06, 5.61E-06,
			      6.00E-06},
			  &psia_1::get_instance()),
  	       InvalidUnit);
  // invalid unit for coa
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
			  {3000, 2800, 2600, 2400, 2200, 2000, 1800,
			      1600, 1400, 1200, 1000, 2800, 2600, 2400, 2200,
			      2000, 1800, 1600, 1400, 1200, 1000, 820},
  			  &psig::get_instance(), "coa",
  			  {3.76E-06, 4.07E-06, 4.28E-06, 4.45E-06, 4.62E-06,
			      4.76E-06, 4.86E-06, 5.24E-06, 5.51E-06, 5.61E-06,
			      6.00E-06},
			  &STB_MMscf::get_instance()),
  	       InvalidUnit);

  DynList<double> pmax =
    reverse(build_dynlist<double>(3000, 2800, 2600, 2400, 2200, 2000, 1800,
				  1600, 1400, 1200, 1000));
  DynList<double> pmin =
    reverse(build_dynlist<double>(2800, 2600, 2400, 2200, 2000, 1800,
				  1600, 1400, 1200, 1000, 820));
  Array<double> pressures = pmax.append(pmin);
  Array<double> coa = build_array<double>(3.76E-06, 4.07E-06, 4.28E-06, 4.45E-06,
					  4.62E-06, 4.76E-06, 4.86E-06, 5.24E-06,
					  5.51E-06, 5.61E-06, 6.00E-06).rev();

  VectorDesc v = {125, 820, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		  PVT_INVALID_VALUE, pressures,
		  &psig::get_instance(), "coa", coa, &psia_1::get_instance() };

  for (auto it = zip_it(pmax, pmin, coa, v.p, v.y); it.has_curr(); it.next())
    {
      auto t = it.get_curr();
      const double & pmax = get<0>(t);
      const double & pmin = get<1>(t);
      const double & coa = get<2>(t);
      const double & pexp = get<3>(t);
      const double & coa_exp = get<4>(t);
      ASSERT_EQ(coa, coa_exp);
      ASSERT_EQ((pmax + pmin)/2, pexp);
    }
}

TEST(VectorCtor, boa)
{
  // valid configurations
  ASSERT_NO_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
			     {3000, 2800, 2600, 2400, 2200, 2000, 1800, 1600, 1400,
				 1200, 1000},
			     &psig::get_instance(), "boa",
			     {1.0804, 1.0813, 1.0822, 1.0830, 1.0840, 1.0850,
				 1.0861, 1.0872, 1.0883, 1.0895, 1.0907},
			     &RB_STB::get_instance()));

  ASSERT_NO_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
			     DynList<double>({3000, 2800, 2600, 2400, 2200, 2000,
				   1800, 1600, 1400, 1200, 1000}).rev(),
			     &psig::get_instance(), "boa",
		   DynList<double>({1.0804, 1.0813, 1.0822, 1.0830, 1.0840,
			 1.0850, 1.0861, 1.0872, 1.0883, 1.0895,
			 1.0907}).rev(), &RB_STB::get_instance()));

  // Invalid configurations

  // boa values are not sorted
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
			  {3000, 2800, 2600, 2400, 2200, 2000, 1800,
			      1600, 1400, 1200, 1000, 820},
  			  &psig::get_instance(), "boa",
  			  {1.0804, 1.0813, 1.0822, 1.0830, 1.0840,
			      1.0850, 1.0861, 1.0872, 1.0895, 1.0883, 
			      1.0907, 1.0919},
			  &RB_STB::get_instance()), SamplesUnsorted);

  // pressure values are not sorted
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
			  {3000, 2800, 2600, 2200, 2400, 2000, 1800,
			      1600, 1400, 1200, 1000, 820},
  			  &psig::get_instance(), "boa",
  			  {1.0804, 1.0813, 1.0822, 1.0830, 1.0840,
			      1.0850, 1.0861, 1.0872, 1.0883, 1.0895,
			      1.0907, 1.0919},
			  &RB_STB::get_instance()), SamplesUnsorted);

  // pressure array size is unexpected
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
			  {3000, 2800, 2600, 2400, 2200, 2000, 1800,
			      1600, 1400, 1000},
  			  &psig::get_instance(), "boa",
			  {1.0804, 1.0813, 1.0822, 1.0830, 1.0840,
			      1.0850, 1.0861, 1.0872, 1.0883, 1.0895,
			      1.0907},
			  &RB_STB::get_instance()), LengthMismatch);

  // a boa value is out of unit range
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
			  {3000, 2800, 2600, 2400, 2200, 2000, 1800,
			      1600, 1400, 1200, 1000, 820},
  			  &psig::get_instance(), "boa",
			  /* this --> */ {0.01, 1.0813, 1.0822, 1.0830, 1.0840,
			      1.0850, 1.0861, 1.0872, 1.0883, 1.0895,
			      1.0907, 1.0919}, 
			  &RB_STB::get_instance()), OutOfUnitRange);

  // a p value is out of unit range
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
			  {45000, 2800, 2600, 2400, 2200, 2000, 1800,
			 1600, 1400, 1200, 1000, 820},
  			  &psig::get_instance(), "boa",
  			  {1.0804, 1.0813, 1.0822, 1.0830, 1.0840,
			 1.0850, 1.0861, 1.0872, 1.0883, 1.0895,
			 1.0907, 1.0919}, 
			  &RB_STB::get_instance()),
  	       OutOfUnitRange);

  // invalid unit for pressure
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
			  {35000, 2800, 2600, 2400, 2200, 2000, 1800,
			 1600, 1400, 1200, 1000, 820},
  			  &RB_STB::get_instance(), "boa",
  			  {1.0804, 1.0813, 1.0822, 1.0830, 1.0840,
			 1.0850, 1.0861, 1.0872, 1.0883, 1.0895,
			 1.0907, 1.0919},
			  &RB_STB::get_instance()),
  	       InvalidUnit);
  // invalid unit for boa
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
			  {3000, 2800, 2600, 2400, 2200, 2000, 1800,
			      1600, 1400, 1200, 1000, 820},
  			  &psig::get_instance(), "boa",
  			  {1.0804, 1.0813, 1.0822, 1.0830, 1.0840,
			      1.0850, 1.0861, 1.0872, 1.0883, 1.0895,
			      1.0907, 1.0919},
			  &Fahrenheit::get_instance()),
  	       InvalidUnit);
}

TEST(VectorCtor, uob)
{
  {
    ASSERT_NO_THROW(VectorDesc(125, 820, 1.0919, 30000, 7500, 
			       {820, 650, 550, 450, 0},
			       &psig::get_instance(), "uob",
			       {7500, 11350, 14000, 18120, 30000},
			       &CP::get_instance()));
        
    VectorDesc v(125, 820, 1.0919, 30000, 7500,
		 {820, 650, 550, 450, 0}, &psig::get_instance(), "uob",
		 {7500, 11350, 14000, 18120, 30000}, &CP::get_instance());
    ASSERT_EQ(v.uod, 30000);
    ASSERT_EQ(v.uod, v.y.get_first());
    ASSERT_EQ(v.p.get_first(), 0);
    ASSERT_TRUE(eq(v.p, build_array<double>(820, 650, 550, 450, 0).rev()));
    ASSERT_TRUE(eq(v.y, build_array<double>(7500, 11350, 14000, 18120, 30000).rev()));
  }

  {
    ASSERT_NO_THROW(VectorDesc(125, 820, 1.0919, 30000, 7500,
			       DynList<double>({820, 650, 550, 450, 0}).rev(),
			       &psig::get_instance(), "uob",
			       DynList<double>({7500, 11350, 14000, 18120, 30000}).rev(),
			       &CP::get_instance()));

    VectorDesc v(125, 820, 1.0919, 30000, 7500,
		 DynList<double>({820, 650, 550, 450, 0}).rev(),
		 &psig::get_instance(), "uob",
		 DynList<double>({7500, 11350, 14000, 18120, 30000}).rev(),
		 &CP::get_instance());
    ASSERT_EQ(v.uod, 30000);
    ASSERT_EQ(v.uod, v.y.get_first());
    ASSERT_EQ(v.p.get_first(), 0);
    ASSERT_TRUE(eq(v.p, build_array<double>(820, 650, 550, 450, 0).rev()));
    ASSERT_TRUE(eq(v.y, build_array<double>(7500, 11350, 14000, 18120, 30000).rev()));
  }

  // // cases not specifying uod
  // {
  //   ASSERT_NO_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE, 7500, 
  // 			       {820, 650, 550, 450, 0},
  // 			       &psia::get_instance(), "uob",
  // 			       {7500, 11350, 14000, 18120, 30000},
  // 			       &CP::get_instance()));
        
  //   VectorDesc v(125, 820, 1.0919, PVT_INVALID_VALUE, 7500,
  // 		 {820, 650, 550, 450}, &psia::get_instance(), "uob",
  // 		 {7500, 11350, 14000, 18120}, &CP::get_instance());
  //   ASSERT_EQ(v.uod, v.y.get_first());
  //   ASSERT_EQ(v.p.get_first(), 0);
  //   ASSERT_TRUE(eq(v.p, build_array<double>(820, 650, 550, 450, 0).rev()));
  //   ASSERT_TRUE(eq(v.y, build_array<double>(7500, 11350, 14000, 18120, v.uod).rev()));
  // }

  // {
  //   ASSERT_NO_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE, 7500,
  // 			       DynList<double>({820, 650, 550, 450}).rev(),
  // 			       &psia::get_instance(), "uob",
  // 			       DynList<double>({7500, 11350, 14000, 18120}).rev(),
  // 			       &CP::get_instance()));

  //   VectorDesc v(125, 820, 1.0919, PVT_INVALID_VALUE, 7500,
  // 		 DynList<double>({820, 650, 550, 450}).rev(),
  // 		 &psia::get_instance(), "uob",
  // 		 DynList<double>({7500, 11350, 14000, 18120}).rev(),
  // 		 &CP::get_instance());
  //   ASSERT_EQ(v.uod, v.y.get_first());
  //   ASSERT_EQ(v.p.get_first(), 0);
  //   ASSERT_TRUE(eq(v.p, build_array<double>(820, 650, 550, 450, 0).rev()));
  //   ASSERT_TRUE(eq(v.y, build_array<double>(7500, 11350, 14000, 18120, v.uod).rev()));
  // }

  // Invalid uod
 ASSERT_THROW(VectorDesc(125, 820, 1.0919, 17000, 7500,
			 DynList<double>({820, 650, 550, 450}).rev(),
			 &psig::get_instance(), "uob",
			 DynList<double>({7500, 11350, 14000, 18120}).rev(),
			 &CP::get_instance()), SampleInvalid);

 // Invalid uobp
 ASSERT_THROW(VectorDesc(125, 820, 1.0919, 25000, 7500.1,
			 DynList<double>({820, 650, 550, 450, 0}).rev(),
			 &psig::get_instance(), "uob",
			 DynList<double>({7500, 11350, 14000, 18120, 25000}).rev(),
			 &CP::get_instance()), OutOfRange); 

  // uob values are not sorted
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, 25000, 7500,
			  {820, 650, 550, 450, 0},
  			  &psig::get_instance(), "uob",
  			  {7500, 14000, 11350, 18120, 25000},
			  &CP::get_instance()), SamplesUnsorted);

  // pressure values are not sorted
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, 25000, 7500,
			  {820, 550, 650, 450, 0},
  			  &psig::get_instance(), "uob",
  			  {7500, 11350, 14000, 18120, 25000},
			  &CP::get_instance()), SamplesUnsorted);

  // pressure array size is unexpected
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, 25000, 7500,
			  {820, 550, 450, 0},
  			  &psig::get_instance(), "uob",
			  {7500, 11350, 14000, 18120, 25000},
			  &CP::get_instance()), LengthMismatch);

  // a uob value is out of unit range
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, 25000, 7500,
			  {820, 650, 550, 450, 0},
  			  &psig::get_instance(), "uob",
			  {7500, 11350, 14000, 18120, 1e11}, // <-- this
			  &CP::get_instance()), OutOfUnitRange);

  // a p value is out of unit range
  ASSERT_THROW(VectorDesc(125, 35000, 1.0919, 25000, 7500,
			  {35000, 650, 550, 450, 0},
  			  &psig::get_instance(), "uob",
  			  {7500, 11350, 14000, 18120, 25000}, 
			  &CP::get_instance()),
  	       OutOfUnitRange);

  // invalid unit for pressure
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, 25000, 7500,
			  {820, 650, 550, 450, 0},
  			  &CP::get_instance(), "uob",
  			  {7500, 11350, 14000, 18120, 25000},
			  &CP::get_instance()),
  	       InvalidUnit);
  
  // invalid unit for uob
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, 25000, 7500,
			  {820, 650, 550, 450, 0},
  			  &psig::get_instance(), "uob",
  			  {7500, 11350, 14000, 18120, 25000},
			  &Fahrenheit::get_instance()),
  	       InvalidUnit);
}

TEST(VectorCtor, uoa)
{
  // --property "uoa CP Fahrenheit 125 psig 820 30045.5  3000 2700 2400 2100 1800 1500 1200 1000 820 12891 11384 10377 9530 8762 8240 7869 7638 7500"

  const Array<double> pressure = {3000, 2700, 2400, 2100, 1800, 1500, 1200, 1000};
  const Array<double> uoa = {12891, 11384, 10377, 9530, 8762, 8240, 7869, 7638};
  
  ASSERT_NO_THROW(VectorDesc(125, 820, 1.0919, 30000, 7500, 
			     pressure, &psig::get_instance(), "uoa",
			     uoa, &CP::get_instance()));

  ASSERT_NO_THROW(VectorDesc(125, 820, 1.0919, 30000, 7500,
			     pressure.rev(), &psig::get_instance(), "uoa",
			     uoa.rev(), &CP::get_instance()));

  // Invalid uod
 ASSERT_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE, 7638.1,
			 pressure.rev(), &psig::get_instance(), "uoa",
			 uoa.rev(), &CP::get_instance()), OutOfUnitRange);

 // Invalid uobp
 ASSERT_THROW(VectorDesc(125, 820, 1.0919, 30000, 7638.1,
			 pressure.rev(), &psig::get_instance(), "uoa",
			 uoa.rev(), &CP::get_instance()), SampleInvalid); 

  // uoa values are not sorted
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, 30000, 7500,
			  {3000, 2700, 2400, 2100, 1800, 1500, 1200, 1000},
  			  &psig::get_instance(), "uoa",
			  {12891, 11384, 10377, 9530, 8240, 8762, 7869, 7638},
			  &CP::get_instance()), SamplesUnsorted);

  // pressure values are not sorted
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, PVT_INVALID_VALUE, 7500,
			  {820, 550, 650, 450},
  			  &psig::get_instance(), "uoa",
  			  {7500, 11350, 14000, 18120},
			  &CP::get_instance()), SamplesUnsorted);

  // pressure array size is unexpected
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, 30000, 7500,
			  {3000, 2700, 2400, 2100, 1500, 1200, 1000},
  			  &psig::get_instance(), "uoa",
			  {12891, 11384, 10377, 9530, 8762, 8240, 7869, 7638},
			  &CP::get_instance()), LengthMismatch);

  // a uoa value is out of unit range
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, 30000, 7500,
			  {3000, 2700, 2400, 2100, 1800, 1500, 1200, 1000},
  			  &psig::get_instance(), "uoa",
			  {1e11, 11384, 10377, 9530, 8762, 8240, 7869, 7638},
			  &CP::get_instance()), OutOfUnitRange);

  // a p value is out of unit range
  ASSERT_THROW(VectorDesc(125, 35000, 1.0919, 30000, 7500,
			  {36000, 2700, 2400, 2100, 1800, 1500, 1200, 1000},
  			  &psig::get_instance(), "uoa",
			  {12891, 11384, 10377, 9530, 8762, 8240, 7869, 7638},
			  &CP::get_instance()),
  	       OutOfUnitRange);

  // invalid unit for pressure
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, 30000, 7500,
			  pressure, &CP::get_instance(), "uoa",
			  uoa, &CP::get_instance()),
  	       InvalidUnit);
  
  // invalid unit for uoa
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, 30000, 7500,
			  pressure, &psig::get_instance(), "uoa",
			  uoa, &psig::get_instance()),
  	       InvalidUnit);
}

TEST(VectorCtor, uo)
{
  // "uo CP Fahrenheit 300 psig  3000 2700 2400 2100 1800 1500 1200 900 600 400 200 38.14 36.77 35.66 34.42 33.44 32.19 30.29 32.5 37.1 40.8 44.5"

  ASSERT_NO_THROW(VectorDesc(125, 820, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
			     PVT_INVALID_VALUE,
			     {3000, 2700, 2400, 2100, 1800, 1500, 1200,
				 900, 600, 400, 200}, &psig::get_instance(), "uo",
			     {38.14, 36.77, 35.66, 34.42, 33.44, 32.19, 30.29,
				 32.5, 37.1, 40.8, 44.5}, &CP::get_instance()));

  // Pressures unsorted
  ASSERT_THROW(VectorDesc(125, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
			  PVT_INVALID_VALUE, PVT_INVALID_VALUE,
			  {3000, 2700, 2400, 2100, 1500, 1800, 1200,
			      900, 600, 400, 200}, &psig::get_instance(), "uo",
			  {38.14, 36.77, 35.66, 34.42, 33.44, 32.19, 30.29,
			      32.5, 37.1, 40.8, 44.5}, &CP::get_instance()),
	       SamplesUnsorted);
}

CmdLine cmd = { "vector", '=', "0" };

ValueArg<unsigned long> seed_arg = { "s", "seed", "seed", false, 0, "seed", cmd };
ValueArg<size_t> num_tests =
  { "n", "num-tests", "number of tests", false, 1000, "num", cmd };
ValueArg<string> filter = { "", "gtest_filter", "", false, "", "", cmd };

int main(int argc, char** argv)
{
  cmd.parse(argc, argv);

  gsl_rng_set(r.get(), seed_arg.getValue());

  uo_list = to_vector(range(num_tests.getValue()).maps<ArrayP>([] (auto)
    {
      return generate_random_uo();
    }));

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
