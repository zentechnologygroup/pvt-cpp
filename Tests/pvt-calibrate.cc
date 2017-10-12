
# include <gmock/gmock.h>

# include <gsl/gsl_rng.h>

# include <ah-zip.H>

# include <metadata/pvt-calibrate.H>

using namespace std;
using namespace testing;
using namespace Aleph;

TEST(VectorDesc, simple_operations)
{
  {
    VectorDesc d;
    ASSERT_FALSE(d.is_valid());
    ASSERT_ANY_THROW(d.validate());
  }

  {
    ASSERT_THROW(VectorDesc(0, "foo"), InvalidRange);

    // invalid temp
    ASSERT_THROW(VectorDesc(0, 820, 1.003, 1e5, 1e1, { 10, 20, 30 },
			    &psia::get_instance(), "rs", &RB_STB::get_instance(),
			    { 1e-6, 2e-6, 3e-6 }), InvalidRange);

    // invalid pb
    ASSERT_THROW(VectorDesc(125, 0, 1.003, 1e5, 1e1, { 10, 20, 30 },
			    &psia::get_instance(), "rs", &RB_STB::get_instance(),
			    { 1e-6, 2e-6, 3e-6 }), InvalidRange);

    // dimension of p array is greater than zero
    ASSERT_THROW(VectorDesc(125, 820, 1.003, 1e5, 1e1, { 10 },
			    &psia::get_instance(), "rs", &RB_STB::get_instance(),
			    { 1e-6, 2e-6, 3e-6 }), LengthMismatch);

    // dimension of p and y must be equal; p is incomplete
    ASSERT_THROW(VectorDesc(125, 820, 1.003, 1e5, 1e1, { 10, 20 },
			    &psia::get_instance(), "rs", &RB_STB::get_instance(),
			    { 1e-6, 2e-6, 3e-6 }), LengthMismatch);

    // dimension of p and y must be equal; y is incomplete
    ASSERT_THROW(VectorDesc(125, 820, 1.003, 1e5, 1e1, { 10, 20, 30 },
			    &psia::get_instance(), "rs", &RB_STB::get_instance(),
			    { 1e-6, 2e-6 }), LengthMismatch);

    // p must be sorted
    ASSERT_THROW(VectorDesc(125, 820, 1.003, 1e5, 1e1, { 10, 30, 20 },
			    &psia::get_instance(), "rs", &RB_STB::get_instance(),
			    { 1e-6, 2e-6, 3e-6 }), SamplesUnsorted);

    ASSERT_NO_THROW(VectorDesc(125, 820, 1.003, 1e5, 1e1, { 10, 20, 30 },
			       &psia::get_instance(), "rs",
			       &RB_STB::get_instance(), { 1e-6, 2e-6, 3e-6 }));

    ASSERT_NO_THROW(VectorDesc(125, -1, 1.003, 1e5, 1e1, { 10, 20, 30 },
			       &psia::get_instance(), "uo",
			       &RB_STB::get_instance(), { 1e-6, 2e-6, 3e-6 }));

    ASSERT_THROW(VectorDesc(125, -1, 1.003, 1e5, 1e1, { 10, 20, 30 },
			    &psia::get_instance(), "rs",
			    &RB_STB::get_instance(), { 1e-6, 2e-6, 3e-6 }),
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
		"rs", &RB_STB::get_instance(), { 1e-6, 2e-6, 3e-6 });
  VectorDesc d2(125, 820, 1.003, 1e5, 1e1, { 10, 20, 30 }, &psia::get_instance(),
		"rs", &RB_STB::get_instance(), { 1e-6, 2e-6, 3e-6 });
  VectorDesc d3(125, 820, 1.003, 1e5, 1e1, { 10, 20, 30, 40 }, &psia::get_instance(),
		"rs", &RB_STB::get_instance(), { 1e-6, 2e-6, 3e-6, 4e-6 });
  VectorDesc d4(125, 820, 1.003, 1e5, 1e1, { 10, 21, 30, 40 }, &psia::get_instance(),
		"rs", &RB_STB::get_instance(), { 1e-6, 2e-6, 3e-6, 4e-6 });

  ASSERT_TRUE(d1.is_parallel(d2));
  ASSERT_FALSE(d1.is_parallel(d3));
  ASSERT_FALSE(d3.is_parallel(d4));
}

struct SimpleVector : public Test
{
  VectorDesc v;
  SimpleVector() : v(125, 820, 1.003, 1e5, 1e1, { 10, 20, 30, 40, 50 },
		     &psia::get_instance(), "rs",
		     &RB_STB::get_instance(), { 1e-6, 2e-6, 3e-6, 4e-6, 5e-6 }) {}
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
      &psia::get_instance(), "uo", &CP::get_instance(),
      {404.6, 345.3, 278, 256, 260, 270, 279.8, 302.4, 316.3, 335.0, 349.7, 370});

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

using ArrayP = pair<Array<double>, Array<double>>;

struct UoBoundTest : public TestWithParam<ArrayP>
{
  const pair<Array<double>, Array<double>> & p = GetParam();
  const Array<double> & pressure = p.first;
  const Array<double> & uo = p.second;
  VectorDesc v;
  UoBoundTest() : v(200, 3000, -1, -1, -1, pressure, &psia::get_instance(), "uo",
		    &CP::get_instance(), uo) {}
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

static unique_ptr<gsl_rng, void(*)(gsl_rng*)> r(gsl_rng_alloc(gsl_rng_mt19937),
						gsl_rng_free);

struct UoTestDesc
{
  Array<double> p, uo;
  UoTestDesc(const DynList<double> & __p, const DynList<double> & __uo)
    : p(__p), uo(__uo) {}
};

constexpr unsigned long seed = 0;
constexpr unsigned long Max_Num_Points = 10;
constexpr double Max_Pressure = 10000;
constexpr double Max_uob = 30000;
constexpr double Max_uoa = 30000;

ArrayP generate_random_uo()
{
  gsl_rng_set(r.get(), seed);

  // select random nuob and nuoa. Both must be greater than or equal to 2
  size_t nuob = 2 + gsl_rng_uniform_int(r.get(), Max_Num_Points);
  size_t nuoa = 2 + gsl_rng_uniform_int(r.get(), Max_Num_Points);

  // Generate nuob + nuoa pressure values
  Array<double> p;
  for (size_t i = 0; i < nuob + nuoa; ++i)
    p.append(gsl_rng_uniform_pos(r.get())*Max_Pressure);
  in_place_sort(p); // pressure must be sorted

  // select a random uobp between uo_percentile and 2*uo_percentile
  const double uo_percentile = 0.1*Max_Pressure;
  const double uo_pivot =
    uo_percentile + 2*uo_percentile*gsl_rng_uniform_pos(r.get());
  assert(uo_pivot > uo_percentile and uo_pivot < 2*uo_percentile);

  // generate nuob random values inversely sorted
  DynList<double> uob;
  for (size_t i = 0; i < nuob; ++i)
    uob.append(uo_pivot + (Max_uob - uo_pivot)*gsl_rng_uniform_pos(r.get()));
  uob = sort(uob).rev();

  // generate nuoa random values sorted
  DynList<double> uoa;
  for (size_t i = 0; i < nuoa; ++i)
    uoa.append(uo_pivot + (Max_uoa - uo_pivot)*gsl_rng_uniform_pos(r.get()));
  in_place_sort(uoa);

  // cout << "uob ="; uob.for_each([] (auto v) { cout << " " << v; }); cout << endl;
  // cout << "uoa ="; uoa.for_each([] (auto v) { cout << " " << v; }); cout << endl;

  uob.append(uoa);
  Array<double> uo = uob;

  assert(p.size() == uo.size());

  // cout << "p ="; p.for_each([] (auto v) { cout << " " << v; }); cout << endl;
  // cout << "uo ="; uo.for_each([] (auto v) { cout << " " << v; }); cout << endl;

  return ArrayP(p, Array<double>(uo));
}

vector<ArrayP> uo_list =
  to_vector(range(33).maps<ArrayP>([] (auto) { return generate_random_uo(); }));

//INSTANTIATE_TEST_CASE_P(random_uo, UoBoundTest, Values(uo_list));

INSTANTIATE_TEST_CASE_P(random_uo, UoBoundTest, ValuesIn(uo_list));


// int main(int argc, char** argv)
// {
//   // Disables elapsed time by default.
//   //::testing::GTEST_FLAG(print_time) = false;

//   // This allows the user to override the flag on the command line.
//   ::testing::InitGoogleTest(&argc, argv);

//   return RUN_ALL_TESTS();
// }
