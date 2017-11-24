
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

  ASSERT_FALSE(rs_stats.is_empty());

  auto rs_stat = rs_stats.get_first();

  auto rs_in = data.inputing(data.search_vectors("uob"), rs_stat.corr_ptr,
			     CorrStat::c(rs_stat.desc),
			     CorrStat::m(rs_stat.desc));

  cout << "new rs inputs fron" << rs_stat.corr_ptr->name << endl;
  rs_in.for_each([] (auto & v) { cout << v << endl; }); cout << endl;

  rs_in.for_each([this] (auto & v) { data.add_vector(v); });

  rs_in = data.inputing(data.search_vectors("uob"), rs_stat.corr_ptr,
			CorrStat::c(rs_stat.desc), CorrStat::m(rs_stat.desc));

  ASSERT_TRUE(rs_in.is_empty());
}

TEST_F(FluidTest, inputing_t_rs)
{
  auto rs_corr_list = data.can_be_applied("rs");
  auto rs_stats = sort(rs_corr_list.maps<PvtData::StatsDesc>([this] (auto ptr)
    {
      return data.apply(ptr);
    }), cmp_r2);
  ASSERT_FALSE(rs_stats.is_empty());

  auto stat = rs_stats.get_first();
  data.set_correlation(stat);

  auto rs_vectors = data.search_vectors("rs");
  ASSERT_TRUE(rs_vectors.is_unitarian());

  auto rs = rs_vectors.get_first();

  auto rs_200 = data.auto_inputing(200, rs->get_pressures(), "rs");
  cout << "rs 200" << endl
       << rs_200 << endl;
}
