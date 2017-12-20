# ifndef SMOOTH_GRID_H
# define SMOOTH_GRID_H

# include <pvt-grid-compute.H>

struct SmoothGrid
{
  PvtGrid grid;

  SmoothGrid(PvtGrid && __grid) : grid(move(__grid))
  {
    grid.make_pb_table();
  }

  enum class ViolationType
  {
    AdjustUob_PbInUob, AdjustUob_PbInUoa, AdjustUoa_PbInUob, AdjustUoa_PbInUoa,
    AdjustBob_PbInUob, AdjustBob_PbInUoa, AdjustBoa_PbInUob, AdjustBoa_PbInUoa,
    OK  
  };

  struct Violation
  {
    ViolationType type;
    pair<double, double> below;
    pair<double, double> above;
  };

  ViolationType uo_review(const PvtGrid::Property & prop) const
  {
    const double & pb = prop.pb;
    const double & yval = prop.yval;
    auto uo_parts =
      zip_partition([&pb] (auto t) { return get<0>(t) < pb; }, prop.p, prop.y);
    DynList<tuple<double, double>> & uob_zone = uo_parts.first;
    DynList<tuple<double, double>> & uoa_zone = uo_parts.second;
    const double last_p_in_uob = get<0>(uob_zone.get_last());
    const double last_uob = get<1>(uob_zone.get_last());
    const double first_p_in_uoa = get<0>(uoa_zone.get_first());
    const double first_uoa = get<1>(uoa_zone.get_last());

    cout << "t              = " << prop.t << endl
	 << "last_p_in_uob  = " << last_p_in_uob << endl
	 << "last_uob       = " << last_uob << endl
	 << "pb             = " << pb << endl
	 << "yval           = " << yval << endl
	 << "first_p_in_uoa = " << first_p_in_uoa << endl
	 << "first_uoa      = " << last_uob << endl;
  }

  
};



# endif
