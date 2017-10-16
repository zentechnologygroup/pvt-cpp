
# include <gmock/gmock.h>

# include <tclap/CmdLine.h>

# include "calibrate.H"

using namespace testing;
using namespace Aleph;
using namespace TCLAP;
using namespace std;

using ArrayP = pair<Array<double>, Array<double>>;

struct UoBoundTest : public TestWithParam<ArrayP>
{
  const pair<Array<double>, Array<double>> & p = GetParam();
  const Array<double> & pressure = p.first;
  const Array<double> & uo = p.second;
  VectorDesc v;
  UoBoundTest() : v(200, 3000, -1, -1, -1, pressure, &psia::get_instance(), "uo",
		    uo, &CP::get_instance()) { cout << "UoBoundTest" << endl; }
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

vector<ArrayP> uo_list;//  =
 // to_vector(range(33).maps<ArrayP>([] (auto) { return generate_random_uo(); }));

INSTANTIATE_TEST_CASE_P(random_uo, UoBoundTest, ValuesIn(uo_list));

CmdLine cmd = { "split-uo", ' ', "0" };

ValueArg<unsigned long> seed_arg = { "s", "seed", "seed", false, 0, "seed", cmd };
ValueArg<size_t> num_tests =
  { "n", "num-tests", "number of tests", false, 1000, "num", cmd };

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
