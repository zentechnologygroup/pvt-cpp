# ifndef PVT_ANALYSE_H
# define PVT_ANALYSE_H

# include <ahSort.H>
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

  EmpiricalData & get_data() noexcept { return data; }

  const EmpiricalData & get_data() const noexcept { return data; }

  void check_sorted(const string & set_name, const string & col_name,
		    bool ascending = true) const
  {
    if (ascending)
      {
	if (not is_sorted(data.values(set_name, col_name).first))
	  {
	    ostringstream s;
	    s << "Column " << col_name << " of variable data set " << set_name
	      << " is not sorted";
	    ALEPHTHROW(SamplesUnsorted, s.str());
	  }
	return;
      }

    if (not is_inversely_sorted(data.values(set_name, col_name).first))
      {
	ostringstream s;
	s << "Column " << col_name << " of variable data set " << set_name
	  << " is not inversely sorted";
	ALEPHTHROW(SamplesUnsorted, s.str());
      }
  }

  static double get_epsilon(double min_val, double max_val, double error)
  {
    assert(min_val < max_val);
    assert(error > 0 and error < 1);
    return (max_val - min_val)*error;
  }

  static bool check_values(double v1, double v2, double epsilon)
  {
    return fabs(v1 - v2) < epsilon;
  }

  void check_data() 
  {
    data.check_constant("t", Temperature::get_instance());
    data.check_constant("pb", Pressure::get_instance());
    data.check_constant("api", OilGravity::get_instance());
    data.check_constant("rsb", GORGLRvolumeRatio::get_instance());
    data.check_constant("yg", GasSpecificGravity::get_instance());

    data.check_variable("Below Pb", "p", Pressure::get_instance());
    data.check_variable("Below Pb", "rs", GORGLRvolumeRatio::get_instance());
    data.check_variable("Below Pb", "bob", FVFvolumeRatio::get_instance());
    data.check_variable("Below Pb", "uob", DynamicViscosity::get_instance());

    data.check_variable("Above Pb", "p", Pressure::get_instance());
    data.check_variable("Above Pb", "rs", GORGLRvolumeRatio::get_instance());
    data.check_variable("Above Pb", "boa", FVFvolumeRatio::get_instance());
    data.check_variable("Above Pb", "uoa", DynamicViscosity::get_instance());

    // insert a new row in Above data set that is a copy oof the pb
    // row lightly increased
    const auto & bp_row = data.var_sets(0).samples.get_last();
    {
      Array<double> next_row =
	{ nextafter(bp_row(0), numeric_limits<double>::max()),
	  bp_row(1),
	  nextafter(bp_row(2), numeric_limits<double>::max()),
	  nextafter(bp_row(3), numeric_limits<double>::min())
	};
      assert(next_row(0) > bp_row(0));
      assert(next_row(1) == bp_row(1));
      assert(next_row(2) > bp_row(2));
      assert(next_row(3) < bp_row(3));
      //      data.var_sets(1).samples.insert(bp_row);
      data.var_sets(1).samples.insert(next_row);
    }
    
    check_sorted("Below Pb", "p");
    check_sorted("Above Pb", "p");
    check_sorted("Below Pb", "rs");
    check_sorted("Above Pb", "rs");
    check_sorted("Below Pb", "bob");
    check_sorted("Above Pb", "boa", false);
    check_sorted("Below Pb", "uob", false);
    check_sorted("Above Pb", "uoa");

    {
      const double & pb = bp_row(0);
      const double defined_pb = get<1>(data.find_const("pb"));
      if (defined_pb != pb)
	{
	  ostringstream s;
	  s << "Defined pb value " << defined_pb
	    << " is not equal to the stored value in the matrix " << pb;
	  ALEPHTHROW(SampleInvalid, s.str());
	}
    }

    const double & rsb = bp_row(1);
    {
      const double & rs = bp_row(1);
      const double defined_rsb = get<1>(data.find_const("rsb"));
      if (defined_rsb != rs)
	{
	  ostringstream s;
	  s << "Defined rsb value " << defined_rsb
	    << " is not equal to the stored value in the matrix " << rsb;
	  ALEPHTHROW(SampleInvalid, s.str());
	}
    }

    const auto & above_set = data.var_sets(1);
    const auto & last_row = above_set.samples.get_last();
    const auto & highest_rs = last_row(1);

    data.set_const("rsb", highest_rs);

    {
      const double & bobp = bp_row(2);
      data.def_const("bobp", bobp, data.var_sets(0).var_units(2));
    }

    {
      const double & uobp = bp_row(3);
      data.def_const("uobp", uobp, data.var_sets(0).var_units(3));
    }
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

  //                                // value error
  DynList<tuple<const Correlation*, double, double>>
    pb_best_correlations() const
  {
    using T = tuple<const Correlation*, double, double>;

    double pb = get_pb();

    auto ret = pb_valid_correlations().maps<T>([pb, this] (auto p)
      {
	auto val = data.compute(p, false);
	return make_tuple(p, val, fabs(val - pb));
      });

    return sort(ret, [] (auto t1, auto t2) { return get<2>(t1) < get<2>(t2); });
  }

  DynList<const Correlation*>
  valid_correlations(const string & target_name,
		     const string & set_name) const
  {
    return data.matching_correlations(set_name).
      filter([&target_name] (auto ptr)
	     { return ptr->target_name() == target_name; });
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
    
  using Desc =                // values          statistics     error msg
  tuple<const Correlation*, DynList<double>, CorrStat::Desc, string>;

  static const Correlation * correlation(const Desc & d) { return get<0>(d); }
  static DynList<double> values(const Desc & d) { return get<1>(d); }
  static const CorrStat::Desc & stats(const Desc & d) { return get<2>(d); }
  static double r2(const Desc & d) { return CorrStat::r2(stats(d)); }
  static double mse(const Desc & d) { return CorrStat::mse(stats(d)); }
  static double sigma(const Desc & d) { return CorrStat::sigma(stats(d)); }
  static double sumsq(const Desc & d) { return CorrStat::sumsq(stats(d)); }
  static double c(const Desc & d) { return CorrStat::c(stats(d)); }
  static double m(const Desc & d) { return CorrStat::m(stats(d)); }

  /// Compute statistics for each correlation contained in `l`. The
  /// unit of results is the unit of stored data
  DynList<Desc>
  correlations_stats(const DynList<const Correlation*> & l, size_t seti,
		     bool check = true) const
  {
    DynList<Desc> ret;
    if (l.is_empty())
      return ret;
    
    const string corr_name  = l.get_first()->target_name();
    auto p = data.values(seti, corr_name);
    const auto & vals = p.first;
    const Unit * lab_unit = p.second;
    CorrStat stat(vals);

    for (auto it = l.get_it(); it.has_curr(); it.next())
      {
	auto corr_ptr = it.get_curr();
	if (corr_ptr->target_name() != corr_name)
	  {
	    ostringstream s;
	    s << "PvtAnalyzer::best_correlations(): Correlation "
	      << corr_ptr->name << " target name " << corr_ptr->target_name()
	      << " contained in list is not the name " << corr_name;
	    ALEPHTHROW(SampleContainsTargetName, s.str()); 
	  }

	try
	  {
	    auto result = data.compute(seti, corr_ptr, check);
	    if (&corr_ptr->unit != lab_unit)
	      result.mutable_for_each([&] (auto & v)
                { v = unit_convert(corr_ptr->unit, v, *lab_unit); }); 
	    auto stats = stat.stats(result);
	    ret.append(make_tuple(corr_ptr, move(result), stats, ""));
	  }
	catch (exception & e)
	  {
	    ostringstream s;
	    s << "PvtAnalyzer::correlations_stats: evaluating correlation "
	      << corr_ptr->name << ": " << e.what();
	    ALEPHTHROW(MetadataException, s.str());
	    /* ignore it! */
	  }
      }

    return sort(ret, [] (auto t1, auto t2)
		{ return get<0>(t1)->name < get<0>(t2)->name; });
  }

  DynList<Desc> rs_best_correlations() const
  {
    return correlations_stats(rs_valid_correlations(), 0);
  }

  DynList<Desc> bob_best_correlations() const
  {
    return correlations_stats(bob_valid_correlations(), 0);
  }

  DynList<Desc> boa_best_correlations() const
  {
    return correlations_stats(boa_valid_correlations(), 1);
  }

  DynList<Desc> uob_best_correlations() const
  {
    return correlations_stats(uob_valid_correlations(), 0);
  }

  DynList<Desc> uoa_best_correlations() const
  {
    return correlations_stats(uoa_valid_correlations(), 1);
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
      filter([] (auto ptr) { return ptr->target_name() == "bob"; });
  }

  DynList<const Correlation*> bob_valid_correlations() const
  {
    return data.valid_correlations(0).
      filter([] (auto ptr) { return ptr->target_name() == "bob"; });
  }

  DynList<const Correlation*> boa_correlations() const
  {
    return data.matching_correlations("Above Pb").
      filter([] (auto ptr) { return ptr->target_name() == "boa"; });
  }

  DynList<const Correlation*> boa_valid_correlations() const
  {
    return data.valid_correlations("Above Pb").
      filter([] (auto ptr) { return ptr->target_name() == "boa"; });
  }

  DynList<const Correlation*> uod_correlations() const
  {
    return data.matching_correlations("Below Pb").
      filter([] (auto ptr) { return ptr->target_name() == "uod"; });
  }

  DynList<const Correlation*> uod_valid_correlations() const
  {
    return data.valid_correlations(0).
      filter([] (auto ptr) { return ptr->target_name() == "uod"; });
  }

  DynList<const Correlation*> uob_correlations() const
  {
    return data.matching_correlations("Below Pb").
      filter([] (auto ptr) { return ptr->target_name() == "uob"; });
  }

  using IncompletePar = pair<string, DynList<const Correlation*>>;
  
  /// Retorna una lista de pares correlacion,lista-nombres de
  /// correlaciones uob que no pueden ser calculaladas. Por cada
  /// correlación, una lista de parámetros faltantes es dada
  DynList<pair<const Correlation*, DynList<IncompletePar>>>
    uob_incomplete_correlations() const
  {
    auto l = Correlation::array().filter([this] (auto p)
     {
       return p->target_name() == "uob" and not data.can_be_applied(0, p);
     });

    return l.maps<pair<const Correlation*, DynList<IncompletePar>>>
      ([this] (auto corr)
       {
	 auto missing_pars = data.missing_parameters(0, corr);
	 return make_pair(corr, missing_pars.template maps<IncompletePar>
			  ([&] (const auto & par_name)
           {
	     auto corrs = data.valid_correlations(0).filter([&] (auto corr)
               {
		 return corr->target_name() == par_name;
	       });
	     return make_pair(par_name, move(corrs));
	   }));
       });
  }

  DynList<string> uob_required_values() const
  {
    DynSetTree<string> missing_pars;
    for (auto it = Correlation::array().get_it(); it.has_curr(); it.next())
      {
	auto corr = it.get_curr();
	if (not (corr->target_name() == "uob" and
		 not data.can_be_applied(0, corr)))
	  continue;
	data.missing_parameters(0, corr).for_each([&] (const auto & name)
          {
	    missing_pars.insert(name);
	  });
      }
    return missing_pars.keys();
  }

  /// Return the uod values according to the set of available and
  /// computable correlations
  DynList<pair<const Correlation*, double>>
    compute_required_uod(bool check = true) const
  {
    DynList<pair<const Correlation*, double>> ret;
    Correlation::array().filter
      ([] (auto p) { return p->target_name() == "uod"; }).
      for_each([&ret, this, check] (auto p)
	       {
		 if (not data.can_be_applied(p))
		   return;
		 try
		   {
		     ret.append(make_pair(p, data.compute(p, check)));
		   }
		 catch (...) { /* ignore it */ }
	       });

    return ret;
  }

  DynList<const Correlation*> uob_valid_correlations() const
  {
    return data.valid_correlations(0).
      filter([] (auto ptr) { return ptr->target_name() == "uob"; });
  }

  DynList<const Correlation*> uoa_correlations() const
  {
    return data.matching_correlations("Above Pb").
      filter([] (auto ptr) { return ptr->target_name() == "uoa"; });
  }

  DynList<const Correlation*> uoa_valid_correlations() const
  {
    return data.valid_correlations(1).
      filter([] (auto ptr) { return ptr->target_name() == "uoa"; });
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

  string to_string(const DynList<VarCorrStatus> & l) const
  {
    DynList<DynList<string>> lines;
    for (auto it = l.get_it(); it.has_curr(); it.next())
      {
	const auto & s = it.get_curr();
	if (not get<0>(s))
	  continue;
	lines.append({get<2>(s)->name, ::to_string(get<3>(s)),
	      ::to_string(get<4>(s)), ::to_string(get<4>(s))});
      }
    lines.insert({"Correlation", "r2", "mse", "sigma-distance"});

    return Aleph::to_string(format_string(lines));
  }

  /// Returns the rs correlations sorted by the correlation name
  /// TODO: borrar luego. Esta version es por backward 
  DynList<VarCorrStatus>
  best_correlations(const DynList<const Correlation*> & l, size_t seti,
		    bool check = true) const
  {
    const string corr_name  = l.get_first()->target_name();
    auto p = data.values(seti, corr_name);
    const Unit * lab_unit = p.second;
    CorrStat stat(p.first);

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
	    ALEPHTHROW(SampleContainsTargetName, s.str()); 
	  }

	try
	  {
	    auto result = data.compute(seti, corr_ptr, check);
	    if (&corr_ptr->unit != lab_unit)
	      result.mutable_for_each([&] (auto & v)
                { unit_convert(corr_ptr->unit, v, *lab_unit); });
	    auto r2 = stat.r2(result);
	    auto mse = stat.mse(result);
	    auto sigma_distance = stat.sigma_distance(result);
	    ret.append(make_tuple(true, move(result), corr_ptr, r2, mse,
				  sigma_distance, ""));
	  }
	catch (exception & e) { /* ignore it! */ }
      }

    return sort(ret, [] (auto t1, auto t2)
		{ return get<2>(t1)->name < get<2>(t2)->name; });
  }

  //                  used uod correlation (nullptr ==> there is no uod_corr)
  DynList<pair<Desc, const Correlation*>>
  uob_correlations_lfits(bool check = true)
  {
    using P = pair<Desc, const Correlation*>;
    DynList<P> ret;

    DynList<const Correlation*> complete_uob = uob_valid_correlations();
    if (not complete_uob.is_empty())
      ret.append(correlations_stats(complete_uob, 0, check).maps<P>
		 ([] (const Desc & desc) { return make_pair(desc, nullptr); }));

    if (get<0>(data.search_const("uod")))
      return ret; // if uod defined ==> there is no incomplete uob correlations

    // Todas estas correlaciones de uod deberían estár en el rango de aplicación
    DynList<pair<const Correlation*, double>> uods;
    uod_valid_correlations().for_each([&] (auto p)
      {
	try
	  {
	    uods.append(make_pair(p, data.compute(p, check)));
	  }
	catch (...) { /* ignore it; simply do not put it into the list */ }
      });

    for (auto it = uods.get_it(); it.has_curr(); it.next())
      {
	auto uod_pair = it.get_curr();
	const Correlation * uod_corr = uod_pair.first;
	
	data.def_const("uod", uod_pair.second, &uod_corr->unit);

	// uob que contengan al parámetro uod
	auto corr_list = uob_valid_correlations().filter([&] (auto p)
          {
	    return p->names().exists([] (const auto & s) { return s == "uod"; });
	  });

	ret.append(correlations_stats(corr_list, 0, check).maps<P>
		   ([uod_corr] (const Desc & desc)
		    { return make_pair(desc, uod_corr); }));

	data.remove_last_const("uod");
      }

    return ret;
  }

  DynList<Desc> uoa_correlations_lfits(bool check = true) const
  {
    return correlations_stats(uoa_valid_correlations(), 1, check);    
  }

  DynList<VarCorrStatus> best_rs_correlations(bool check = true) const
  {
    return best_correlations(rs_valid_correlations(), 0, check);
  }

  DynList<VarCorrStatus> best_bob_correlations(bool check = true) const
  {
    return best_correlations(bob_valid_correlations(), 0, check);
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
  
  Array<double>
  load_values(const Correlation * corr_ptr, const string & corr_base_name,
	      size_t seti) const
  {
    if (corr_ptr->target_name() != corr_base_name)
      {
	ostringstream s;
	s << "PvtAnalyzer::load_value: correlation " << corr_ptr->name
	  << " has not " << corr_base_name << " as target name";
	ALEPHTHROW(SampleContainsTargetName, s.str());
      }

    auto p = data.values(seti, corr_base_name);
    auto & vals = p.first;
    if (p.second != &corr_ptr->unit)
      vals.mutable_for_each([&] (auto & v)
        { v = unit_convert(*p.second, v, corr_ptr->unit); });
    
    return move(vals);
  }

  Array<double>
  load_values(const Correlation * correlation_ptr, size_t seti) const
  {
    return load_values(correlation_ptr, correlation_ptr->target_name(), seti);
  }

  /// Performs a linear regression fit for the `correlation_ptr`
  /// taking as data the onst names and the varables stored in the var
  /// set `seti`. The used samples for doing the fit are taken from
  /// the var set `seti`
  CorrStat::LFit lfit(const Correlation * correlation_ptr, size_t seti,
		      bool check = true) const
  {
    CorrStat stat(load_values(correlation_ptr, seti));
    auto result = data.compute(seti, correlation_ptr, check);
    return stat.linear_fit(result);
  }

  CorrStat::LFit
  rs_lfit(const Correlation * corr_ptr, bool check = true) const
  {
    CorrStat stat(load_values(corr_ptr, "rs", 0));
    return stat.linear_fit(data.compute(0, corr_ptr, check));
  }

  CorrStat::LFit
  bob_lfit(const Correlation * corr_ptr, bool check = true) const
  {
    CorrStat stat(load_values(corr_ptr, "bob", 0));
    return stat.linear_fit(data.compute(0, corr_ptr, check));
  }

  CorrStat::LFit
  boa_lfit(const Correlation * corr_ptr, bool check = true) const
  {
    CorrStat stat(load_values(corr_ptr, "boa", 1));
    return stat.linear_fit(data.compute(1, corr_ptr, check));
  }

  DynList<VarCorrLFit>
  correlations_lfits(const DynList<const Correlation*> & corr_list,
		     size_t seti, bool check = true) const
  {
    const string & var_name = corr_list.get_first()->target_name();
    auto p = data.values(seti, var_name);
    auto & vals = p.first;
    const Unit * lab_unit = p.second;
    CorrStat stat(vals);

    DynList<VarCorrLFit> ret = corr_list.maps<VarCorrLFit>([&] (auto ptr)
      {
	try
	  {
	    auto result = data.compute(seti, ptr, check);
	    if (&ptr->unit != lab_unit)
	      result.mutable_for_each([&] (auto & v)
                { v = unit_convert(ptr->unit, v, *lab_unit); } );
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
		{ return get<2>(t1)->name < get<2>(t2)->name; });
  }

  DynList<VarCorrLFit> rs_correlations_lfits(bool check = true) const
  {
    return correlations_lfits(rs_valid_correlations(), 0, check);
  }

  DynList<pair<string, DynList<double>>>
   lfits_list(const DynList<VarCorrLFit> & l, size_t seti) const
  {
    return l.maps<pair<string, DynList<double>>>([this, seti] (auto t)
    {
      auto r = get<4>(t);
      auto ptr = get<2>(t);
      auto values = data.tuned_compute(seti, ptr, r.c, r.m);
      return make_pair(ptr->name, move(values));
    });
  }

  DynList<pair<string, DynList<double>>>
  lfits_list(const DynList<VarCorrLFit> & l, const string & set_name) const
  {
    return lfits_list(l, data.set_index(set_name));
  }

  DynList<pair<string, DynList<double>>>
	 rs_lfits_list(const DynList<VarCorrLFit> & l) const
  {
    return lfits_list(l, 0);
  }

  DynList<VarCorrLFit> bob_correlations_lfits(bool check = true) const
  {
    return correlations_lfits(bob_valid_correlations(), 0, check);
  }

  DynList<VarCorrLFit> boa_correlations_lfits(bool check = true) const
  {
    return correlations_lfits(boa_valid_correlations(), 1, check);
  }

  DynList<pair<string, DynList<double>>>
  bob_lfits_list(const DynList<VarCorrLFit> & l) const
  {
    return lfits_list(l, 0);
  }  

  DynList<pair<string, DynList<double>>>
  boa_lfits_list(const DynList<VarCorrLFit> & l) const
  {
    return lfits_list(l, 1);
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

    s << "plot(xdata, " << name << ", xlab=\"" << xlabel
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

  string to_R(const string & prefix,
	      const DynList<pair<string, DynList<double>>> & l)
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
    check_data();
  }
};

# endif


