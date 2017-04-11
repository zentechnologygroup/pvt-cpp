# ifndef PVT_CALIBRATE_H
# define PVT_CALIBRATE_H

# include <correlations/correlation-stats.H>
# include "metadata-exceptions.H"

struct VectorDesc
{
  const double t = 0; //  unit will always be in Fahrenheit
  const double uod = -1; // dead oil viscosity in cP
  const double pb = 0; // bubble point pressure in the same unit that p
  const Array<double> p;
  const Unit * punit = nullptr;

  const string yname = "no-name";
  const Unit * yunit = nullptr;
  const Array<double> y;

  void validate_p_sorted()
  {
    if (not is_sorted(p))
      ZENTHROW(SamplesUnsorted, "pressures are not sorted");
  }

  VectorDesc(double t, double pb, const Array<double> & p, const Unit * punit,
	     const string & yname, const Unit * yunit, const Array<double> & y)
    noexcept
    : t(t), pb(pb), p(p), punit(punit), yname(yname), yunit(yunit), y(y)
  {
    validate_p_sorted();
  }

  VectorDesc(double t, double uod, double pb, 
	     const Array<double> & p, const Unit * punit,
	     const string & yname, const Unit * yunit, const Array<double> & y)
    noexcept
    : t(t), uod(uod), pb(pb), p(p), punit(punit),
      yname(yname), yunit(yunit), y(y)
  {
    validate_p_sorted();
  }

  VectorDesc() {}

  VectorDesc(double t, const string & name) : t(t), yname(name) {}

  CorrStat::Desc stats(const Array<double> & yc, const Unit * unit_ptr) const
  {
    CorrStat s = y;
    return s.stats(yunit == unit_ptr ? yc : unit_convert(*unit_ptr, yc, *yunit));
  }

  bool operator < (const VectorDesc & rhs) const noexcept
  {
    if (yname == rhs.yname)
      return t < rhs.t;
    
    return yname < rhs.yname;
  }

  size_t get_yindex(double yval) const noexcept
  {
    return y.find_index([yval] (auto v) { return v == yval; });
  }

  double gety(double pval) const
  {
    const size_t & n = p.size();
    long i = binary_search(p, pval);
    if (i < 0 or i >= n)
      ZENTHROW(ValueNotFound, ::to_string(pval) + " not found in data set");

    const double pentry = p(i);
    if (pentry == pval)
      return y(i);

    if (pval < pentry)
      --i;

    assert(i >= 0);

    return interpolate(p(i), p(i+1), y(i), y(i+1), pval);
  }

  template <template <typename> class C>
  DynList<double> gety(const C<double> & pvals) const
  {
    return pvals.maps([this] (auto & v) { return this->gety(v); });
  }

  DynList<DynList<string>> to_dynlist() const
  {
    DynList<DynList<string>> ret = zip_maps<DynList<string>>([] (auto t)
      {
	return build_dynlist<string>(::to_string(get<0>(t)),
				     ::to_string(get<1>(t)));
			       
       }, p, y);
    ret.insert(build_dynlist<string>("p " + punit->name,
				     yname + " " + yunit->name));
    return ret;
  }

  string to_string() const
  {
    ostringstream s;
    s << "For t = " << t << " " << Fahrenheit::get_instance().name << " pb = "
      << pb << " " << punit->name;
    if (uod > 0)
      s << " uod = " << uod << " " << yunit->name;
    s << " :" << endl
      << shift_lines_to_left(Aleph::to_string(format_string(to_dynlist())), 2);
    return s.str();
  }

  friend ostream & operator << (ostream & out, const VectorDesc & d)
  {
    return out << d.to_string();
  }
};

struct ConstDesc
{
  const string name = "no-name";
  const double value = 0;
  const Unit * unit_ptr = nullptr;

  // only take in account the name in order to index and sort by name
  // in a binary search tree
  bool operator < (const ConstDesc & rhs) const noexcept
  {
    return name < rhs.name;
  }

  ConstDesc(const string name, double value, const Unit * unit_ptr) noexcept
    : name(name), value(value), unit_ptr(unit_ptr) {}

  ConstDesc(const string & name = "no-name") : name(name) {}

  string to_string() const
  {
    return name + " " + ::to_string(value) + " " + unit_ptr->name;
  }

  friend ostream & operator << (ostream & out, const ConstDesc & d)
  {
    return out << d.to_string();
  }

  DynList<string> to_dynlist() const
  {
    return build_dynlist<string>(name, ::to_string(value), unit_ptr->name);
  }
};

struct PvtData
{
  DynSetTree<ConstDesc> const_values; // indexed and sorted by name

  DynSetTree<double> temperatures; 

  DynSetTree<VectorDesc> vectors;

  DynSetTree<string> names;

  string to_string() const
  {
    DynList<DynList<string>> const_list =
      const_values.maps<DynList<string>>
      ([] (auto & c) { return c.to_dynlist(); });
    const_list.insert(build_dynlist<string>("name", "value", "unit"));

    ostringstream s;
    s << "Constants:" << endl
      << shift_lines_to_left(Aleph::to_string(format_string(const_list)), 2)
      << endl
      << endl
      << "Variables:" << endl;
    for (auto it = vectors.get_it(); it.has_curr(); it.next())
      {
	auto & v = it.get_curr();
	s << shift_lines_to_left(v.to_string(), 2);
	if (&v != &vectors.get_last())
	  s << endl << endl;
      }
    return s.str();
  }

  friend ostream & operator << (ostream & out, const PvtData & d)
  {
    return out << d.to_string();
  }

  void add_const(const string & name, double v, const Unit & unit)
  {
    const_values.emplace(name, v, &unit);
    names.append(name);
  }

  DynList<const VectorDesc*> search_vectors(const string & name) const noexcept
  {
    DynList<const VectorDesc*> ret;
    for (auto t : temperatures)
      {
	const VectorDesc * ptr = vectors.search(VectorDesc(t, name));
	if (ptr != nullptr)
	  ret.append(ptr);
      }

    return ret;
  }

  pair<const Unit*, const Unit*> search_units(const string & target_name) const
  {
    auto v = search_vectors(target_name);
    if (v.is_empty())
      ZENTHROW(VarNameNotFound, target_name + " not found");

    auto & desc_ptr = v.get_first();
    return make_pair(desc_ptr->punit, desc_ptr->yunit);
  }

  void
  add_vector(double t, double pb, const Array<double> & p, const Unit & punit,
	     const string yname, const Array<double> & y, const Unit & yunit)
  {
    if (&punit.physical_quantity != &Pressure::get_instance())
      ZENTHROW(InvalidPressureUnit, punit.name + " is not for pressure");

    if (not is_sorted(p))
      ZENTHROW(SamplesUnsorted, "pressure values are not sorted");

    auto sibling = search_vectors(yname);
    if (not sibling.is_empty() and not sibling.all([&yunit] (auto & p)
        { return p->yunit->is_sibling(yunit); }))
      ZENTHROW(InvalidTargetUnit, yunit.name + " is not sibling");

    temperatures.insert(t);
    vectors.emplace(t, pb, p, &punit, yname, &yunit, y);
    names.append("p");
    names.append("pb");
    names.append("t");
    names.append(yname);
  }

  const ConstDesc * search_const(const string & name) const noexcept
  {
    return const_values.search(ConstDesc(name));
  }

  using Sample = tuple<double, double, double, // t in degF, pb value, uod value
		       const Unit*, DynList<double>, // pressure unit &values
		       string, // property name
		       const Unit*, DynList<double>>; // target unit & values
  static const double & s_t(Sample & s) { return get<0>(s); }
  static const double & s_pb(Sample & s) { return get<1>(s); }
  static const double & s_uod(Sample & s) { return get<2>(s); }
  static const Unit* s_punit(Sample & s) { return get<3>(s); }
  static DynList<double> & s_pvals(Sample & s) { return get<4>(s); }
  static string & s_yname(Sample & s) { return get<5>(s); }
  static const Unit* s_yunit(Sample & s) { return get<6>(s); }
  static DynList<double> & s_yvals(Sample & s) { return get<7>(s); }

  DynList<Sample> get_pars(const DynList<const VectorDesc*> & ref_vectors,
			   const string & target_name) const
  {
    DynList<Sample> ret;
    auto samples = search_vectors(target_name);
    if (samples.is_empty())
      return ret;

    const VectorDesc * fst_vector = ref_vectors.get_first();

    auto ptr = samples.get_first();
    const Unit * punit = ptr->punit;
    const Unit * yunit = ptr->yunit;

    ret.append(Sample(ptr->t, ptr->pb, ptr->uod, punit,
		      to_dynlist(fst_vector->p),
		      target_name, yunit, ptr->gety(fst_vector->p)));

    for (auto it = zip_it_pos(1, samples, ref_vectors); it.has_curr(); it.next())
      {
	auto t = it.get_curr();
	auto sample_ptr = get<0>(t);
	auto vector_ptr = get<1>(t);
	DynList<double> p =
	  to_dynlist(unit_convert(*vector_ptr->punit, vector_ptr->p, *punit));
	DynList<double> y =
	  to_dynlist(unit_convert(*vector_ptr->yunit,
				  sample_ptr->gety(p), *yunit));
	ret.append(Sample(sample_ptr->t, ptr->pb, ptr->uod, punit, move(p),
			  target_name, yunit, move(y)));
      }

    return ret;
  }

  // Return true if corr_ptr parameters matches with the data
  // contained in this set
  bool matches_with_pars(const Correlation * corr_ptr) const
  {
    return corr_ptr->get_preconditions().all([this] (auto & par)
      {
	return par.names().exists([this] (auto & p)
          {
	    return names.has(p.first);
	  });
      });
  }

  DynList<const Correlation*>
  matches_with_pars(const string & target_name) const
  {
    return Correlation::array().filter([this, &target_name] (auto corr_ptr)
      {
	return corr_ptr->target_name() == target_name and
	  this->matches_with_pars(corr_ptr);
      });
  }

  enum class Type { NotFound, Constant, Vector };

  //                   name  , true if it is a constant
  using ParType = pair<string, Type>;

  // Return true if corr_ptr can be applied; that is, if all values
  // required by the correlation re available and these match with
  // correlation development ranges
  bool can_be_applied(const Correlation * corr_ptr) const
  {
    auto ref_vectors = search_vectors(corr_ptr->target_name());
    for (auto it = corr_ptr->get_preconditions().get_it(); it.has_curr();
	 it.next())
      {
	auto & par = it.get_curr();
	auto * ptr = par.names().find_ptr([this] (auto & p)
					  {
					    return names.has(p.first);
					  });
	if (ptr == nullptr)
	  return false; // this parameter is not in the data set

	const string & name_in_data = ptr->first;
	if (name_in_data == "t" or name_in_data == "p" or
	    name_in_data == "pb" or name_in_data == "uod")
	  continue;

	const ConstDesc * const_ptr = search_const(name_in_data);
	if (const_ptr) // is name_in_data as constant?
	  {
	    if ( (not par.check(VtlQuantity(*const_ptr->unit_ptr,
					    const_ptr->value))))
	      return false;
	    continue;
	  }

	auto pars_list = get_pars(ref_vectors, name_in_data);
	if (not pars_list.all([&par] (auto & s)
             {
	       const DynList<double> & yvals = s_yvals(s);
	       const Unit * yunit = s_yunit(s);
	       return yvals.all([&par, yunit] (auto v)
				{
				  return par.check(VtlQuantity(*yunit, v));
				});
	     }))
	  return false;
       }

    return true;
  }

  DynList<const Correlation*> can_be_applied(const string & target_name) const
  {
    return Correlation::array().filter([&target_name, this] (auto ptr)
      {
	return ptr->target_name() == target_name and this->can_be_applied(ptr);
      });
  }

  //                  t,      pb,     uod
  using TYPE = tuple<double, double, double,
		     //  p,            y,                 yc		     
		     DynList<double>, DynList<double>, DynList<double>>;

  tuple<const Unit*, const Unit*, DynList<TYPE>>
  iapply(const Correlation * corr_ptr) const
  {
    const string target_name = corr_ptr->target_name();
    const DynList<const VectorDesc*> target_vectors =
      search_vectors(target_name);
    if (target_vectors.is_empty())
      ZENTHROW(VarNameNotFound, "Correlation target " + target_name +
	       " not found in data set");

    auto first_sample_ptr = target_vectors.get_first();

    ParList pars; // parameter to be passed to correlation corr_ptr
    DynList<Sample> samples;
    for (auto it = corr_ptr->get_preconditions().get_it(); it.has_curr();
	 it.next())
      {
	auto & par = it.get_curr();
	auto * ptr = par.names().find_ptr([this] (auto & p)
					  { return names.has(p.first); });
	if (ptr == nullptr)
	  ZENTHROW(VarNameNotFound, "correlation parameter " + par.name +
		   " not found in data set");

	const string & name_in_data = ptr->first;
	if (name_in_data == "t" or name_in_data == "p" or
	    name_in_data == "pb" or name_in_data == "uod")
	  continue;

	const ConstDesc * const_ptr = search_const(name_in_data);
	if (const_ptr)
	  {
	    pars.insert(par.name, const_ptr->value, const_ptr->unit_ptr);
	    continue;
	  }

	auto s = get_pars(target_vectors, name_in_data);
	samples.append(s);
      }

    if (samples.is_empty())
      samples.append(get_pars(target_vectors, target_name));

    const Unit * punit = first_sample_ptr->punit;
    const Unit * yunit = first_sample_ptr->yunit;
  // Using TYPE = tuple<double, double, double const Unit*, DynList<double>,
  // 		      const Unit*, DynList<double>, DynList<double>>;
    DynList<TYPE> ret;
    for (auto it = zip_it(samples, target_vectors); it.has_curr(); it.next())
      {
	auto tt = it.get_curr();
	Sample & desc = get<0>(tt);
	const VectorDesc * ref_ptr = get<1>(tt);

	// if (s_punit(desc) != ref_ptr->punit)
	//   ZENTHROW(InvalidPressureUnit, "mismatch between pressure units " +
	// 	   s_punit(desc)->name + " != " + ref_ptr->punit->name);
	const Unit * src_punit = s_punit(desc);
	const double & temp = s_t(desc);
	const double & pb = s_pb(desc);
	const double & uod = s_uod(desc);
	const Unit * par_unit = s_yunit(desc);
	const string & par_name = s_yname(desc);
	pars.insert("t", temp, &Fahrenheit::get_instance());
	pars.insert("pb", pb, src_punit);
	pars.insert("uod", uod, &CP::get_instance());
	DynList<double> pvals = unit_convert(*src_punit, s_pvals(desc), *punit);
	DynList<double> yvals = s_yvals(desc);
	DynList<double> ycvals; // correlation output
	for (auto it = zip_it(pvals, yvals); it.has_curr(); it.next())
	  {
	    auto t = it.get_curr();
	    const double & p = get<0>(t); // pressure from desc
	    const double & parval = get<1>(t);
	    pars.insert("p", p, punit);
	    pars.insert(par_name, parval, par_unit);

	    // pars.tbl.for_each([] (auto p)
	    //  		      {
	    // 			cout << p.first << " = " << p.second.first << " " << p.second.second->name << " ";
	    // 		      });
	    // cout << endl;

	    VtlQuantity result =
	      VtlQuantity(*yunit, corr_ptr->compute_by_names(pars, false));
	    ycvals.append(result.raw());
	    //cout << target_name << " = " << result << endl;
	    pars.remove("p");
	    pars.remove(par_name);
	  }//                                 malo
	ret.append(TYPE(temp, pb, uod, pvals, to_dynlist(ref_ptr->y), ycvals));
	pars.remove("t");
	pars.remove("pb");
	pars.remove("uod");
      }

    return make_tuple(punit, yunit, ret);
  }

  using T =
    tuple<const Correlation*, DynList<double>, DynList<double>, CorrStat::Desc>;

  T istats(const Correlation * corr_ptr) const
  {
    cout << "ystats " << corr_ptr->target_name() << endl;
    using P = tuple<double, double, double>; // p, y, yc
    DynList<P> xy;
    auto vals = iapply(corr_ptr);
    for (auto it = get<2>(vals).get_it(); it.has_curr(); it.next())
      {
	auto & curr = it.get_curr();
	DynList<double> & p = get<3>(curr);
	DynList<double> & y = get<4>(curr);
	DynList<double> & yc = get<5>(curr);
	xy.append(zip_maps<P>([] (auto t)
			      {
				return P(move(get<0>(t)),
					 move(get<1>(t)),
					 move(get<2>(t)));
			      }, p, y, yc));
      }

    auto t = t_unzip(xy);
    // cout << "** " << corr_ptr->name << endl;
    // cout << "p ="; get<0>(t).for_each([] (auto v) { cout << " " << v; }); cout << endl;
    // cout << "y ="; get<1>(t).for_each([] (auto v) { cout << " " << v; }); cout << endl;
    // cout << "yc ="; get<2>(t).for_each([] (auto v) { cout << " " << v; }); cout << endl;

    auto stats = CorrStat(get<1>(t)).stats(get<2>(t));

    return make_tuple(corr_ptr, move(get<0>(t)), move(get<1>(t)), move(stats));
  }
};

# endif
