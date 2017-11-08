
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

auto cmp_r2 = [] (const PvtData::StatsDesc & s1, const PvtData::StatsDesc & s2)
{
  return fabs(1 - CorrStat::r2(s1.desc)) < fabs(1 - CorrStat::r2(s2.desc));
};

TEST_F(FluidTest, inputing_rs)
{
  auto rs_corr_list = data.can_be_applied("rs");
  auto rs_stats = sort(rs_corr_list.maps<PvtData::StatsDesc>([this] (auto ptr)
    {
      return data.apply(ptr);
    }), cmp_r2);

  cout << to_string(format_string(rs_stats.maps<DynList<string>>
				  ([] (auto & s) { return s.to_dynlist(); })))
       << endl;

  auto rs_stat = rs_stats.get_first();

  auto rs_in = data.inputing(data.search_vectors("uob"), rs_stat.corr_ptr,
			     CorrStat::c(rs_stat.desc), CorrStat::m(rs_stat.desc));

  rs_in.for_each([] (auto & v) { cout << v << endl; });
}
