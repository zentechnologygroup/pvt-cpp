
# include <gmock/gmock.h>

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


// TODO: generar los tres casos de prueba para split_uo

// TODO: generar pruebas aleatorias (al menos 40) para split_uo

