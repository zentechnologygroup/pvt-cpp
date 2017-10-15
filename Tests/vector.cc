
# include <gmock/gmock.h>

# include <ah-zip.H>

# include "calibrate.H"

using namespace std;
using namespace testing;
using namespace Aleph;

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
			    &psia::get_instance(), "rs", { 1e-6, 2e-6, 3e-6 },
			    &SCF_STB::get_instance()), InvalidRange);

    // invalid pb
    ASSERT_THROW(VectorDesc(125, 0, 1.003, 1e5, 1e1, { 10, 20, 30 },
			    &psia::get_instance(), "rs", { 1e-6, 2e-6, 3e-6 },
			    &SCF_STB::get_instance()), InvalidRange);

    // dimension of p array is greater than zero
    ASSERT_THROW(VectorDesc(125, 820, 1.003, 1e5, 1e1, { 10 },
			    &psia::get_instance(), "rs", { 1e-6, 2e-6, 3e-6 },
			    &SCF_STB::get_instance()), LengthMismatch);

    // dimension of p and y must be equal; p is incomplete
    ASSERT_THROW(VectorDesc(125, 820, 1.003, 1e5, 1e1, { 10, 20 },
			    &psia::get_instance(), "rs", { 1e-6, 2e-6, 3e-6 },
			    &SCF_STB::get_instance()), LengthMismatch);

    // dimension of p and y must be equal; y is incomplete
    ASSERT_THROW(VectorDesc(125, 820, 1.003, 1e5, 1e1, { 10, 20, 30 },
			    &psia::get_instance(), "rs", { 1e-6, 2e-6 },
			     &SCF_STB::get_instance()), LengthMismatch);

    // p must be sorted
    ASSERT_THROW(VectorDesc(125, 820, 1.003, 1e5, 1e1, { 10, 30, 20 },
			    &psia::get_instance(), "rs", { 1e-6, 2e-6, 3e-6 },
			    &SCF_STB::get_instance()), SamplesUnsorted);

    ASSERT_NO_THROW(VectorDesc(125, 820, 1.003, 1e5, 1e1, { 10, 20, 30, 820 },
			       &psia::get_instance(), "rs",
			       { 10-6, 20, 30, 40 }, &SCF_STB::get_instance()));

    ASSERT_NO_THROW(VectorDesc(125, -1, 1.003, 1e5, 1e1, { 10, 20, 30 },
			       &psia::get_instance(), "uo",
			       { 1e-6, 2e-6, 3e-6 }, &SCF_STB::get_instance()));

    ASSERT_THROW(VectorDesc(125, -1, 1.003, 1e5, 1e1, { 10, 20, 30 },
			    &psia::get_instance(), "rs",
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

  VectorDesc d1(125, 820, 1.003, 1e5, 1e1, { 10, 20, 30 }, &psia::get_instance(),
		"rs", { 1e-6, 2e-6, 3e-6 }, &RB_STB::get_instance());
  VectorDesc d2(125, 820, 1.003, 1e5, 1e1, { 10, 20, 30 }, &psia::get_instance(),
		"rs", { 1e-6, 2e-6, 3e-6 }, &RB_STB::get_instance());
  VectorDesc d3(125, 820, 1.003, 1e5, 1e1, { 10, 20, 30, 40 },
		&psia::get_instance(), "rs", { 1e-6, 2e-6, 3e-6, 4e-6 },
		&RB_STB::get_instance());
  VectorDesc d4(125, 820, 1.003, 1e5, 1e1, { 10, 21, 30, 40 },
		&psia::get_instance(), "rs",
		{ 1e-6, 2e-6, 3e-6, 4e-6 }, &RB_STB::get_instance());

  ASSERT_TRUE(d1.is_parallel(d2));
  ASSERT_FALSE(d1.is_parallel(d3));
  ASSERT_FALSE(d3.is_parallel(d4));
}

struct SimpleVector : public Test
{
  VectorDesc v;
  SimpleVector() : v(125, 820, 1.003, 1e5, 1e1, { 10, 20, 30, 40, 50 },
		     &psia::get_instance(), "rs",
		     { 1e-6, 2e-6, 3e-6, 4e-6, 5e-6 }, &RB_STB::get_instance()) {}
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
  ASSERT_EQ(v.gety(50), 5e-6);

  ASSERT_NEAR(v.gety(15), 1.5e-6, 1e-15); // interpolation test for gety

  ASSERT_NEAR(v.gety(5), 0.5e-6, 1e-15); // extrapolation test by the left

  ASSERT_NEAR(v.gety(55), 5.5e-6, 1e-15); // extrapolation test by the right

  // test gety on a container
  const DynList<double> y = v.gety(build_dynlist<double>(5, 10, 15, 55));
  const DynList<double> expected = { 0.5e-6, 1e-6, 1.5e-6, 5.5e-6 };
  for (auto it = zip_it(y, expected); it.has_curr(); it.next())
    {
      auto t = it.get_curr();
      ASSERT_NEAR(get<0>(t), get<1>(t), 1e-15);
    }
}

TEST(VectorDesc, uo_split)
{
  VectorDesc
    v(200, 3000, -1, -1, -1,
      {200, 500, 800, 1000, 1100, 1300, 1500, 1800, 2100, 2400, 2700, 3000},
      &psia::get_instance(), "uo",
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
    : v(200, 3000, -1, -1, -1, pressure, &psia::get_instance(), "uo",
	uo, &CP::get_instance()) {}
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
  ASSERT_NO_THROW(VectorDesc(125, 820, -1, -1, -1, {820, 600, 450, 300, 15},
			     &psia::get_instance(), "rs",
			     {80, 62, 47, 35, 0}, &SCF_STB::get_instance()));

  ASSERT_NO_THROW(VectorDesc(125, 820, -1, -1, -1, {15, 300, 450, 600, 820},
			     &psia::get_instance(), "rs",
			     {0, 35, 47, 62, 80}, &SCF_STB::get_instance()));

  // Invalid configurations

  // rs values are not sorted
  ASSERT_THROW(VectorDesc(125, 820, -1, -1, -1, {15, 300, 450, 600, 820},
			  &psia::get_instance(), "rs",
			  {0, 47, 35, 62, 80}, &SCF_STB::get_instance()),
	       SamplesUnsorted);

  // pressure values are not sorted
  ASSERT_THROW(VectorDesc(125, 820, -1, -1, -1, {15, 300, 600, 450, 820},
			  &psia::get_instance(), "rs",
			  {0, 35, 47, 62, 80}, &SCF_STB::get_instance()),
	       SamplesUnsorted);

  // last pressure value i p array is not the pb
  ASSERT_THROW(VectorDesc(125, 820, -1, -1, -1, {15, 300, 450, 600, 820.1},
			  &psia::get_instance(), "rs",
			  {0, 35, 47, 62, 80}, &SCF_STB::get_instance()),
	       PressureMismatch);

  // a rs value is out of unit range
  ASSERT_THROW(VectorDesc(125, 820, -1, -1, -1, {15, 300, 450, 600, 820},
			  &psia::get_instance(), "rs",
			  {-5.1e7, 35, 47, 62, 80}, &SCF_STB::get_instance()),
	       OutOfUnitRange);

  // a p value is out of unit range
  ASSERT_THROW(VectorDesc(125, 35000, -1, -1, -1, {15, 300, 450, 600, 35000},
			  &psia::get_instance(), "rs",
			  {-5.1e7, 35, 47, 62, 80}, &SCF_STB::get_instance()),
	       OutOfUnitRange);

  // invalid unit for pressure
  ASSERT_THROW(VectorDesc(125, 820, -1, -1, -1, {820, 600, 450, 300, 15},
			  &psia_1::get_instance(), "rs",
			  {80, 62, 47, 35, 0}, &SCF_STB::get_instance()),
	       InvalidUnit);
  // invalid unit for rs
  ASSERT_THROW(VectorDesc(125, 820, -1, -1, -1, {15, 300, 450, 600, 820},
			  &psia::get_instance(), "rs",
			  {-5.1e7, 35, 47, 62, 80}, &STB_MMscf::get_instance()),
	       InvalidUnit);
}

TEST(VectorCtor, bob)
{
  // valid configurations
  ASSERT_NO_THROW(VectorDesc(125, 820, 1.0919, -1, -1, {820, 600, 450, 300, 15},
			     &psia::get_instance(), "bob",
			     {1.0919, 1.0865, 1.0790, 1.0713, 1.0228},
			     &RB_STB::get_instance()));

  ASSERT_NO_THROW(VectorDesc(125, 820, 1.0919, -1, -1, {15, 300, 450, 600, 820},
  			     &psia::get_instance(), "bob",
  			     {1.0228, 1.0713, 1.0790, 1.0865, 1.0919},
			     &RB_STB::get_instance()));

  // Invalid configurations

  // bob values are not sorted
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, -1, -1, {15, 300, 450, 600, 820},
  			  &psia::get_instance(), "bob",
  			  {1.0228, 1.0713, 1.0865, 1.0790, 1.0919},
			  &RB_STB::get_instance()), SamplesUnsorted);

  // pressure values are not sorted
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, -1, -1, {15, 300, 600, 450, 820},
  			  &psia::get_instance(), "bob",
  			  {1.0228, 1.0713, 1.0790, 1.0865, 1.0919},
			  &RB_STB::get_instance()), SamplesUnsorted);

  // last pressure value i p array is not the pb
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, -1, -1, {15, 300, 450, 600, 820.1},
  			  &psia::get_instance(), "bob",
  			  {1.0228, 1.0713, 1.0790, 1.0865, 1.0919},
			  &RB_STB::get_instance()), PressureMismatch);

  // a bob value is out of unit range
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, -1, -1, {15, 300, 450, 600, 820},
  			  &psia::get_instance(), "bob",
  			  {1.0228, 1.0713, 1.0790, 1.0865, 11.0919}, // <
			  &RB_STB::get_instance()), OutOfUnitRange);

  // a p value is out of unit range
  ASSERT_THROW(VectorDesc(125, 35000, 1.0919, -1, -1, {15, 300, 450, 600, 35000},
  			  &psia::get_instance(), "bob",
  			  {-5.1e7, 35, 47, 62, 80}, &RB_STB::get_instance()),
  	       OutOfUnitRange);

  // invalid unit for pressure
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, -1, -1, {820, 600, 450, 300, 15},
  			  &psia_1::get_instance(), "bob",
  			  {80, 62, 47, 35, 0}, &RB_STB::get_instance()),
  	       InvalidUnit);
  // invalid unit for bob
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, -1, -1, {15, 300, 450, 600, 820},
  			  &psia::get_instance(), "bob",
  			  {-5.1e7, 35, 47, 62, 80}, &STB_MMscf::get_instance()),
  	       InvalidUnit);
}

TEST(VectorCtor, coa)
{
  // valid configurations

  try
    {
      // TODO: valores de presión son dobles porque se calcula el promedio. 
      VectorDesc v(125, 820, -1, -1, -1,
			     {3000, 2800, 2600, 2400, 2200, 2000, 1800,
			      1600, 1400, 1200, 1000, 2800, 2600, 2400, 2200,
			      2000, 1800, 1600, 1400, 1200, 1000, 820},
			     &psia::get_instance(), "coa",
			     {3.76E-06, 4.07E-06, 4.28E-06, 4.45E-06, 4.62E-06,
			      4.76E-06, 4.86E-06, 5.24E-06, 5.51E-06, 5.61E-06,
				 6.00E-06}, &RB_STB::get_instance());
    }
  catch (exception & e) { cout << e.what() << endl; }

  ASSERT_NO_THROW(VectorDesc(125, 820, -1, -1, -1,
			     {3000, 2800, 2600, 2400, 2200, 2000, 1800,
			      1600, 1400, 1200, 1000, 2800, 2600, 2400, 2200,
			      2000, 1800, 1600, 1400, 1200, 1000, 820},
			     &psia::get_instance(), "coa",
			     {3.76E-06, 4.07E-06, 4.28E-06, 4.45E-06, 4.62E-06,
			      4.76E-06, 4.86E-06, 5.24E-06, 5.51E-06, 5.61E-06,
			      6.00E-06}, &RB_STB::get_instance()));

  ASSERT_NO_THROW(VectorDesc(125, 820, 1.0919, -1, -1, {15, 300, 450, 600, 820},
  			     &psia::get_instance(), "coa",
  			     {1.0228, 1.0713, 1.0790, 1.0865, 1.0919},
			     &RB_STB::get_instance()));

  // Invalid configurations

  // coa values are not sorted
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, -1, -1, {15, 300, 450, 600, 820},
  			  &psia::get_instance(), "coa",
  			  {1.0228, 1.0713, 1.0865, 1.0790, 1.0919},
			  &RB_STB::get_instance()), SamplesUnsorted);

  // pressure values are not sorted
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, -1, -1, {15, 300, 600, 450, 820},
  			  &psia::get_instance(), "coa",
  			  {1.0228, 1.0713, 1.0790, 1.0865, 1.0919},
			  &RB_STB::get_instance()), SamplesUnsorted);

  // last pressure value i p array is not the pb
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, -1, -1, {15, 300, 450, 600, 820.1},
  			  &psia::get_instance(), "coa",
  			  {1.0228, 1.0713, 1.0790, 1.0865, 1.0919},
			  &RB_STB::get_instance()), PressureMismatch);

  // a coa value is out of unit range
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, -1, -1, {15, 300, 450, 600, 820},
  			  &psia::get_instance(), "coa",
  			  {1.0228, 1.0713, 1.0790, 1.0865, 11.0919}, // <
			  &RB_STB::get_instance()), OutOfUnitRange);

  // a p value is out of unit range
  ASSERT_THROW(VectorDesc(125, 35000, 1.0919, -1, -1, {15, 300, 450, 600, 35000},
  			  &psia::get_instance(), "coa",
  			  {-5.1e7, 35, 47, 62, 80}, &RB_STB::get_instance()),
  	       OutOfUnitRange);

  // invalid unit for pressure
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, -1, -1, {820, 600, 450, 300, 15},
  			  &psia_1::get_instance(), "coa",
  			  {80, 62, 47, 35, 0}, &RB_STB::get_instance()),
  	       InvalidUnit);
  // invalid unit for coa
  ASSERT_THROW(VectorDesc(125, 820, 1.0919, -1, -1, {15, 300, 450, 600, 820},
  			  &psia::get_instance(), "coa",
  			  {-5.1e7, 35, 47, 62, 80}, &STB_MMscf::get_instance()),
  	       InvalidUnit);
}

TEST(VectorCtor, boa)
{

}

TEST(VectorCtor, uob)
{

}

TEST(VectorCtor, uoa)
{

}
