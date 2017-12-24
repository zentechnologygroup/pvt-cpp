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
    AdjustBob_PbInBob, AdjustBob_PbInBoa, AdjustBoa_PbInBob, AdjustBoa_PbInBoa,
    OK  
  };

  struct Violation
  {
    ViolationType type;
    double t, pb, y_in_pb;
    DynList<tuple<double, double>> below;
    DynList<tuple<double, double>> above;
    size_t nb = 0, na = 0;
    Violation(const ViolationType & __type,
	      double __t, double __pb, double __y_in_pb,
	      DynList<tuple<double, double>> & __below, size_t __nb,
	      DynList<tuple<double, double>> & __above, size_t __na)
      : type(__type), t(__t), pb(__pb), y_in_pb(__y_in_pb),
	below(move(__below)), above(move(__above)), nb(__nb), na(__na) {}
    friend ostream & operator << (ostream & o, const Violation & v)
    {
      return o << "t            = " << v.t << endl
	       << "pb           = " << v.pb << endl
	       << "y_in_pb      = " << v.y_in_pb << endl
	       << "last_p_in_y  = " << get<0>(v.below.get_last()) << endl
	       << "last_y       = " << get<1>(v.below.get_last()) << endl
	       << "first_p_in_y = " << get<0>(v.above.get_first()) << endl
	       << "first_y      = " << get<1>(v.above.get_first()) << endl
	       << "p_below = "
	       << join(v.below.maps<double>([] (auto t) { return get<0>(t); }))
	       << endl
	       << "y_below = "
	       << join(v.below.maps<double>([] (auto t) { return get<1>(t); }))
	       << endl
	       << "p_above = "
	       << join(v.above.maps<double>([] (auto t) { return get<0>(t); }))
	       << endl
	       << "y_above = "
	       << join(v.above.maps<double>([] (auto t) { return get<1>(t); }));
    }
  };

  Violation uo_review(const PvtGrid::Property & prop) const
  {
    const double & pb = prop.pb;
    const double & uobp = prop.y_in_pb;

    auto uo_parts =
      zip_partition([&pb] (auto t) { return get<0>(t) <= pb; }, prop.p, prop.y);
    DynList<tuple<double, double>> & uob_zone = get<0>(uo_parts);
    DynList<tuple<double, double>> & uoa_zone = get<2>(uo_parts);
    const size_t nb = get<1>(uo_parts);
    const size_t na = get<3>(uo_parts);

    const double last_p_in_uob = get<0>(uob_zone.get_last());
    const double first_p_in_uoa = get<0>(uoa_zone.get_first());
    assert(last_p_in_uob < first_p_in_uoa);
    const double last_uob = get<1>(uob_zone.get_last());
    const double first_uoa = get<1>(uoa_zone.get_last());

    ViolationType violation_type = ViolationType::OK;
    const bool uob_nearest_from_pb =
      fabs(pb - last_p_in_uob) < fabs(pb - first_p_in_uoa);
    if (uob_nearest_from_pb)
      violation_type = last_uob > first_uoa ? ViolationType::AdjustUob_PbInUob :
	ViolationType::AdjustUoa_PbInUob;
    else
      violation_type = last_uob > first_uoa ? ViolationType::AdjustUob_PbInUoa :
	ViolationType::AdjustUoa_PbInUoa;

    Violation ret(violation_type, prop.t, pb, uobp, uob_zone, nb, uoa_zone, na);
    cout << ret << endl;
    return ret;
  }

  
};



# endif
