# ifndef PVT_CALIBRATE_H
# define PVT_CALIBRATE_H

# include <correlations/correlation-stats.H>
# include "empirical-data.H"

class PvtAnalyzer
{
  EmpiricalData data;

public:

  EmpiricalData & get_data() { return data; }

  void check_data() const
  {
    data.check_constant("t", Temperature::get_instance());
    data.check_constant("pb", Pressure::get_instance());
    data.check_constant("api", OilGravity::get_instance());
    data.check_constant("rsb", GORGLRvolumeRatio::get_instance());
    data.check_constant("yg", gasSpecificGravity::get_instance());

    data.check_variable("Below Pb", "p", Pressure::get_instance());
    data.check_variable("Below Pb", "rs", GORGLRvolumeRatio::get_instance());
    data.check_variable("Below Pb", "bo", FVFvolumeRatio::get_instance());
    data.check_variable("Below Pb", "uo", DinamicViscosity::get_instance());

    data.check_variable("Above Pb", "p", Pressure::get_instance());
    data.check_variable("Above Pb", "rs", GORGLRvolumeRatio::get_instance());
    data.check_variable("Above Pb", "bo", FVFvolumeRatio::get_instance());
    data.check_variable("Above Pb", "uo", DinamicViscosity::get_instance());
  }

  double get_pb() const { return get<1>(data.find_const("pb")); } 

  DynList<const Correlation *> pb_correlations() const
  {
    return data.matching_const_correlations("pb");
  }

  DynList<const Correlation *> pb_valid_correlations() const
  {
    return data.matching_const_valid_correlations("pb");
  }

  DynList<const Correlation*> rs_correlations() const
  {
    return data.matching_correlations("Below Pb").
      filter([] (auto ptr) { return ptr->target_name() == "rs"; });
  }

  DynList<const Correlation*> rs_valid_correlations() const
  {
    return data.valid_correlations("Below Pb").
      filter([] (auto ptr) { return ptr->target_name() == "rs"; });
  }

  //                      ok    result     correlation       message if not ok
  using ConstCorrStatus = tuple<bool, double, const Correlation*, string>;

  /// Takes a list of correlations for constant values of data set an
  /// compute them according to the present data
  DynList<ConstCorrStatus>
  compute_constant_correlations(const DynList<const Correlation*> & corr_list,
			       bool check = true) const
  {
    return corr_list.maps<ConstCorrStatus>([&] (auto p)
      {
	try
	  {
	    return make_tuple(true, this->data.compute(p, check), p, "");
	  }
	catch (exception & e)
	  {
	    return make_tuple(false, numeric_limits<double>::max(),
			      p, e.what()); 
	  }
      });
  }

  DynList<ConstCorrStatus> best_pb_correlations(bool check = true) const
  {
    const double pb = get_pb();
    return sort(compute_constant_correlations(pb_valid_correlations(), check),
		[pb] (const auto & t1, const auto & t2)
		{
		  return abs(get<1>(t1) - pb) < abs(pb - get<1>(t2));
		});
  }

  //                          ok     results        correlation, message if not ok
  using VarCorrYStatus = tuple<bool, DynList<double>, const Correlation*, string>;

  DynList<VarCorrYStatus> best_rs_correlations(bool check = true) const
  {
    auto rs_correlations = rs_valid_correlations();
    DynList<VarCorrYStatus> ret;
    for (auto it = rs_correlations.get_it(); it.has_curr(); it.next())
      {
	auto corr_ptr = it.get_curr();
	try
	  {
	    auto result = data.compute(0, corr_ptr, check);
	    ret.append(make_tuple(true, move(result), corr_ptr, ""));
	  }
	catch (exception & e) { /* ignore it! */ }
      }

    return ret;
  }

  using VarCorrStatus =
    //   ok      y                r2     correlation,  message if not ok
    tuple<bool, DynList<double>, double, const Correlation*, string>;

  DynList<VarCorrStatus> best_mat_rs_correlations(bool check = true) const
  {
    // como asegurar que el orden de las y sea el mismo tanto para en
    // data set como las correlaciones? R: ordenar por 
    CorrStat stat(data.values("rs"));

    auto rs_correlations = rs_valid_correlations();
    DynList<VarCorrStatus> ret;
    for (auto it = rs_correlations.get_it(); it.has_curr(); it.next())
      {
	auto corr_ptr = it.get_curr();
	try
	  {
	    auto result = data.compute(0, corr_ptr, check);
	    auto r2 = stat.r2(result);
	    ret.append(make_tuple(true, move(result), r2, corr_ptr, ""));
	  }
	catch (exception & e) { /* ignore it! */ }
      }

    return sort(ret, [] (auto t1, auto t2) { return get<2>(t1) < get<2>(t2); });
  }

  PvtAnalyzer() {}

  PvtAnalyzer(istream & input)
  {
    stringstream s;
    s << input.rdbuf();
    data.set_from_json(s.str());
  }
};

# endif //PVT_CALIBRATE_H
