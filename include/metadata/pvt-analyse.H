# ifndef PVT_CALIBRATE_H
# define PVT_CALIBRATE_H

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

  //                          ok     results        correlation, message if not ok
  using VarCorrStatus = tuple<bool, DynList<double>, const Correlation*, string>;

  // DynList<DynList<VarCorrStatus>>
  // compute_var_correlations(const DynList<const Correlation*> & corr_list,
  // 			   const string & set_name,
  // 			   bool check = true) const
  // {
  //   size_t idx = data.set_index(set_name); 
  //   return corr_list.maps<DynList<VarCorrStatus>>([&] (auto ptr)
  //     {
  // 	try
  // 	  {
  // 	    auto result = data.compute(idx, ptr, check);
  // 	    return make_tuple(true, move(result), ptr, "");
  // 	  }
  // 	catch (exception & e)
  // 	  {
  // 	    DynList<double> l = { numeric_limits<double>::max() };
  // 	    return make_tuple(false, move(l), ptr, e.what());
  // 	  }
  //     });
  // }

  DynList<ConstCorrStatus> best_pb_correlations(bool check = true) const
  {
    const double pb = get_pb();
    return sort(compute_constant_correlations(pb_valid_correlations(), check),
		[pb] (const auto & t1, const auto & t2)
		{
		  return abs(get<1>(t1) - pb) < abs(pb - get<1>(t2));
		});
  }

  DynList<VarCorrStatus> best_rs_correlations(bool check = true) const
  {
    auto rs_correlations = rs_valid_correlations();
    DynList<VarCorrStatus> ret;
    for (auto it = rs_correlations.get_it(); it.has_curr(); it.next())
      {
	auto corr_ptr = it.get_curr();
	try
	  {
	    auto result = data.compute(0, corr_ptr, check);
	    cout << "size = " << result.size() << endl;
	    ret.append(make_tuple(true, move(result), corr_ptr, ""));
	  }
	catch (exception & e) { /* ignore it! */ }
      }

    return ret;
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
