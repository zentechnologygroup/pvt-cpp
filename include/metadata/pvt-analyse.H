# ifndef PVT_CALIBRATE_H
# define PVT_CALIBRATE_H

# include <correlations/correlation-stats.H>
# include "empirical-data.H"

class PvtAnalyzer
{
  EmpiricalData data;

public:

  struct SelectedCorrelation
  {
    const Correlation * correlation = nullptr;
    const double c = 0;
    const double m = 1;
    const bool is_calibrated = false;

    SelectedCorrelation(const Correlation * ptr, double c = 0, double m = 1)
      : correlation(ptr), c(c), m(m), is_calibrated(c != 0 or m != 1) {}
  };

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
    data.check_variable("Below Pb", "bob", FVFvolumeRatio::get_instance());
    data.check_variable("Below Pb", "uo", DinamicViscosity::get_instance());

    data.check_variable("Above Pb", "p", Pressure::get_instance());
    data.check_variable("Above Pb", "rs", GORGLRvolumeRatio::get_instance());
    data.check_variable("Above Pb", "boa", FVFvolumeRatio::get_instance());
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

  DynList<const Correlation*> cob_correlations() const
  {
    return Correlation::array().
      filter([] (auto p) { return p->target_name() == "co"; }).
      filter([this] (auto p) { return data.can_be_applied(0, p); });
  }

  DynList<const Correlation*> bob_correlations() const
  {
    return data.matching_correlations("Below Pb").
      filter([] (auto ptr) { cout << ptr->name << endl;
	  return ptr->target_name() == "bob"; });
  }

  DynList<const Correlation*> bob_valid_correlations() const
  {
    return data.valid_correlations(0).
      filter([] (auto ptr) { return ptr->target_name() == "bob"; });
  }

  DynList<const Correlation*> boa_correlations() const
  {
    return data.matching_correlations("Above Pb").
      filter([] (auto ptr) { return ptr->target_name() == "bo"; });
  }

  DynList<const Correlation*> boa_valid_correlations() const
  {
    return data.valid_correlations(1).
      filter([] (auto ptr) { return ptr->target_name() == "bo"; });
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
    //   ok      y                                    r2,     mse,   sigma-dist 
    tuple<bool, DynList<double>, const Correlation*, double, double, double,
	  string>; // message if not ok

  /// Returns the rs correlations sorted by the statistical `name`
  DynList<VarCorrStatus>
  best_correlations(const DynList<const Correlation*> & l,
		    bool check = true) const
  {
    const string corr_name  = l.get_first()->name;
    CorrStat stat(data.values(0, corr_name));

    DynList<VarCorrStatus> ret;
    for (auto it = l.get_it(); it.has_curr(); it.next())
      {
	auto corr_ptr = it.get_curr();
	if (corr_ptr->target_name() != corr_name)
	  {
	    ostringstream s;
	    s << "PvtAnalyzer::best_correlations(): Correlation "
	      << corr_ptr->name << " target name " << corr_ptr->target_name()
	      << " contained in list is not the name " << corr_name;
	    throw domain_error(s.str()); 
	  }
	    
	try
	  {
	    auto result = data.compute(0, corr_ptr, check);
	    auto r2 = stat.r2(result);
	    auto mse = stat.mse(result);
	    auto sigma_distance = stat.sigma_distance(result);
	    ret.append(make_tuple(true, move(result), corr_ptr, r2, mse,
				  sigma_distance, ""));
	  }
	catch (exception & e) { /* ignore it! */ }
      }

    return sort(ret, [] (auto t1, auto t2) { return get<3>(t1) < get<3>(t2); });
  }

  DynList<VarCorrStatus> best_rs_correlations(bool check = true) const
  {
    return best_correlations(rs_valid_correlations(), check);
  }

  DynList<VarCorrStatus> best_bob_correlations(bool check = true) const
  {
    return best_correlations(bob_valid_correlations(), check);
  }

  using VarCorrNlFit =
    //   ok      y              correlation,  message if not ok, Fit values
    tuple<bool, DynList<double>, const Correlation*, string, CorrStat::NlFit>;

  // TODO: parámetros de bounds que serían según la tendencia de la pendiente
  DynList<VarCorrNlFit>
  correlations_nlfits(const DynList<const Correlation*> & corr_list,
		    size_t seti, bool check = true) const
  {
    const string & var_name = corr_list.get_first()->target_name();
    CorrStat stat(data.values(seti, var_name));

    DynList<VarCorrNlFit> ret = corr_list.maps<VarCorrNlFit>([&] (auto ptr)
      {
	try
	  {
	    auto result = data.compute(0, ptr, check);
	    auto fit = stat.nlfit(result);
	    return make_tuple(true, move(result), ptr, "", fit);
	  }
	catch (exception & e)
	  {
	    return make_tuple(false, DynList<double>(), ptr, e.what(),
			      CorrStat::NlFit());
	  }
      });

    return sort(ret, [] (auto t1, auto t2)
		{ return get<4>(t1).sum_line < get<4>(t2).sum_line; });
  }

  using VarCorrLFit =
    //   ok      y              correlation,  message if not ok, Fit values
    tuple<bool, DynList<double>, const Correlation*, string, CorrStat::LFit>;

  string to_string(const VarCorrLFit & t) const
  {
    auto corr = get<2>(t);
    const auto & name = corr->name;

    ostringstream s;
    
    s << name << ": ";
    if (not get<0>(t))
      {
	s << ": " << get<3>(t);
	return s.str();
      }

    const auto & desc = get<4>(t);
    s << "ltuned." << name << " = " << desc.c << " + " << desc.m << " " 
      << corr->call_string() << " error = " << desc.sumsq;

    return s.str();
  }

  string to_R(const VarCorrLFit & t) const
  {
    auto corr = get<2>(t);
    const auto & name = corr->name;

    ostringstream s;

    s << Rvector("ltuned." +  name, get<1>(t));

    return s.str();
  }

  DynList<VarCorrLFit>
  correlations_lfits(const DynList<const Correlation*> & corr_list,
		     size_t seti, bool check = true) const
  {
    const string & var_name = corr_list.get_first()->target_name();
    CorrStat stat(data.values(seti, var_name));

    DynList<VarCorrLFit> ret = corr_list.maps<VarCorrLFit>([&] (auto ptr)
      {
	try
	  {
	    auto result = data.compute(seti, ptr, check);
	    auto fit = stat.linear_fit(result);
	    return make_tuple(true, move(result), ptr, "", fit);
	  }
	catch (exception & e)
	  {
	    return make_tuple(false, DynList<double>(), ptr, e.what(),
			      CorrStat::LFit());
	  }
      });

    return sort(ret, [] (auto t1, auto t2)
		{ return get<4>(t1).sumsq < get<4>(t2).sumsq; });
  }

  DynList<VarCorrLFit> rs_correlations_lfits(bool check = true) const
  {
    return correlations_lfits(rs_valid_correlations(), 0, check);
  }

  DynList<VarCorrNlFit> rs_correlations_nlfits(bool check = true) const
  {
    return correlations_nlfits(rs_valid_correlations(), 0, check);
  }

  DynList<VarCorrLFit> bob_correlations_lfits(bool check = true) const
  {
    return correlations_lfits(bob_valid_correlations(), 0, check);
  }

  string to_R(const Array<double> & lab_data,
	      const Array<double> & xdata,
	      const string & xlabel,
	      const string & ylabel,
	      const DynList<VarCorrStatus> & l) const
  {
    using P = pair<string, Array<double>>;
    ostringstream s;

    const string & name = get<2>(l.get_first())->target_name();

    s << Rvector(name, lab_data) << endl;
    s << Rvector("xdata", xdata) << endl;

    DynList<P> corrs =
      l.maps<P>([] (auto t) { return make_pair(get<2>(t)->name, get<1>(t)); });

    auto colnames = corrs.maps<string>([] (auto p)
				       { return "\"" + p.first + "\""; });
    colnames.insert("\"" + name + "\"");

    s << Rvector("cnames", colnames) << endl;

    s << "plot(xdata, " << name << ", type=\"l\", xlab=\"" << xlabel
      << "\", ylab=\"" << ylabel << "\")" << endl;

    size_t col = 2;
    corrs.for_each([&s, &col] (auto p)
      {
	s << "lines(xdata, " << p.first << ",lwd=2,lty=" << col << ", col="
	  << col << ")" << endl;
	++col;
      });

    s << Rvector("nums", range<int>(1, corrs.size() + 1)) << endl;

    s << "legend(\"topleft\", legend=cnames, lty=nums, col=nums)" << endl;

    return s.str();
  }

  string to_R(const string & prefix,  DynList<pair<string, DynList<double>>> & l)
  {
    ostringstream s;
    s << Rvector(prefix + ".cnames", l.maps<string>([] (auto p)
					  { return "\"" + p.first + + "\""; }));
    s << endl;
    l.for_each([&s, &prefix] (auto p)
	       { s << Rvector(prefix + p.first, p.second) << endl; });	

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
