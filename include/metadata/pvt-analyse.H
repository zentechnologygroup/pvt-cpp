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

  using VarCorrStatus =
    //   ok      y              statistical, correlation,  message if not ok
    tuple<bool, DynList<double>, double, const Correlation*, string>;

  DynList<VarCorrStatus> best_rs_correlations(const string & name,
					      bool check = true) const
  {
    // como asegurar que el orden de las y sea el mismo tanto para en
    // data set como las correlaciones? R: ordenar por 
    CorrStat stat(data.values(0, "rs"));
    auto statistical = stat.statistical(name);

    auto rs_correlations = rs_valid_correlations();
    DynList<VarCorrStatus> ret;
    for (auto it = rs_correlations.get_it(); it.has_curr(); it.next())
      {
	auto corr_ptr = it.get_curr();
	try
	  {
	    auto result = data.compute(0, corr_ptr, check);
	    auto s = (stat.*statistical)(result);
	    ret.append(make_tuple(true, move(result), s, corr_ptr, ""));
	  }
	catch (exception & e) { /* ignore it! */ }
      }

    return sort(ret, [] (auto t1, auto t2) { return get<3>(t1) < get<3>(t2); });
  }

  string to_R(const Array<double> & lab_data,
	      const DynList<VarCorrStatus> & l) const
  {
    using P = pair<string, Array<double>>;
    ostringstream s;

    const string & name = get<3>(l.get_first())->target_name();

    s << Rvector(name, lab_data) << endl;

    DynList<P> corrs =
      l.maps<P>([] (auto t) { return make_pair(get<3>(t)->name, get<1>(t)); });

    corrs.for_each([&s] (auto p) { cout << Rvector(p.first, p.second) << endl; });
    
    s << "plot(" << name << ", type=\"l\")" << endl;

    auto colnames = corrs.maps<string>([] (auto p)
				       { return "\"" + p.first + "\""; });
    colnames.insert("\"" + name + "\"");

    s << Rvector("cnames", colnames) << endl;

    return s.str();
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
