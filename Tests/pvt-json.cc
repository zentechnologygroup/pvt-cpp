
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

struct FluidTest : public Test
{
  PvtData data;
  FluidTest() // fluid5 
  {
    ifstream in("test.json");
    data = PvtData(in);
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

TEST_F(FluidTest, ctor)
{
  ASSERT_TRUE(data.search_vectors("rs").is_unitarian());
}
