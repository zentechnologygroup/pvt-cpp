
# include <gmock/gmock.h>

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
  VectorDesc d1(125, 820, 1.003, 1e5, 1e1, { 10, 20, 30 }, &psia::get_instance(),
		"rs", &RB_STB::get_instance(), { 1e-6, 2e-6, 3e-6 });
  VectorDesc d2(125, 820, 1.003, 1e5, 1e1, { 10, 20, 30 }, &psia::get_instance(),
		"rs", &RB_STB::get_instance(), { 1e-6, 2e-6, 3e-6 });
  VectorDesc d3(125, 820, 1.003, 1e5, 1e1, { 10, 20, 30, 40 }, &psia::get_instance(),
		"rs", &RB_STB::get_instance(), { 1e-6, 2e-6, 3e-6, 4e-6 });

  ASSERT_TRUE(d1.is_parallel(d2));
  ASSERT_FALSE(d1.is_parallel(d3));
}

//struct SimpleVector : public
