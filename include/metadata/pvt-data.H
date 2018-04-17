# ifndef PVT_DATA_H
# define PVT_DATA_H

# include <tpl_array.H>

# include <json.hpp>

# include <tpl_array.H>

# include <correlations/correlation-stats.H>
# include "metadata-exceptions.H"

struct VectorDesc
{
  const double t = 0; //  unit will always be in Fahrenheit
  const double uod = -1; // dead oil viscosity; unit is the same than yunit
  const double pb = 0; // bubble point pressure in the same unit that p
  const Array<double> p;
  const Unit * punit = nullptr;

  const string yname = "no-name";
  const Unit * yunit = nullptr;
  const Array<double> y;

  void validate_p_sorted()
  {
    if (not is_sorted(p))
      ALEPHTHROW(SamplesUnsorted, "pressures are not sorted");
  }

  VectorDesc(double t, double pb, const Array<double> & p, const Unit * punit,
	     const string & yname, const Unit * yunit, const Array<double> & y)
    noexcept
    : t(t), pb(pb), p(p), punit(punit), yname(yname), yunit(yunit), y(y)
  {
    validate_p_sorted();
  }

  VectorDesc(double t, double uod, const Unit * vunit, double pb, 
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
      ALEPHTHROW(ValueNotFound, ::to_string(pval) + " not found in data set");

    const double pentry = p(i);
    if (pentry == pval)
      return y(i);

    if (pval < pentry)
      --i;

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

  DynList<const VectorDesc*> search_vector(const string & name) const noexcept
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
    auto v = search_vector(target_name);
    if (v.is_empty())
      ALEPHTHROW(VarNameNotFound, target_name + " not found");

    auto & desc_ptr = v.get_first();
    return make_pair(desc_ptr->punit, desc_ptr->yunit);
  }

  void
  add_vector(double t, double pb, const Array<double> & p, const Unit & punit,
	     const string yname, const Array<double> & y, const Unit & yunit)
  {
    if (&punit.physical_quantity != &Pressure::get_instance())
      ALEPHTHROW(InvalidPressureUnit, punit.name + " is not for pressure");

    if (not is_sorted(p))
      ALEPHTHROW(SamplesUnsorted, "pressure values are not sorted");

    auto sibling = search_vector(yname);
    if (not sibling.is_empty() and not sibling.all([&yunit] (auto & p)
        { return p->yunit->is_sibling(yunit); }))
      ALEPHTHROW(InvalidTargetUnit, yunit.name + " is not sibling");

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

  using SampleDesc = tuple<double, // t in degF
			   const Unit*, DynList<double>, // pressure unit &values
			   const Unit*, DynList<double>>; // target unit & values

  DynList<SampleDesc> get_samples(const string & target_name) const
  {
    DynList<SampleDesc> ret;

    auto samples = search_vector(target_name);
    if (samples.is_empty())
      return ret;

    auto ptr = samples.remove_first();
    const Unit * punit = ptr->punit;
    const Unit * yunit = ptr->yunit;

    ret.append(SampleDesc(ptr->t, punit, to_dynlist(ptr->p),
			  yunit, to_dynlist(ptr->y)));

    for (auto it = samples.get_it(); it.has_curr(); it.next())
      {
	ptr = it.get_curr();
	DynList<double> p =
	  to_dynlist(unit_convert(*ptr->punit, ptr->p, *punit));
	DynList<double> y =
	  to_dynlist(unit_convert(*ptr->yunit, ptr->y, *yunit));
	ret.append(SampleDesc(ptr->t, punit, move(p), yunit, move(y)));
      }
				
    return ret;
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
  DynList<Sample> get_pars(const string & target_name) const
  {
    DynList<Sample> ret;
    auto samples = search_vector(target_name);
    if (samples.is_empty())
      return ret;

    auto ptr = samples.remove_first();
    const Unit * punit = ptr->punit;
    const Unit * yunit = ptr->yunit;

    ret.append(Sample(ptr->t, ptr->pb, ptr->uod, punit, to_dynlist(ptr->p),
		      target_name, yunit, to_dynlist(ptr->y)));

    for (auto it = samples.get_it(); it.has_curr(); it.next())
      {
	ptr = it.get_curr();
	DynList<double> p =
	  to_dynlist(unit_convert(*ptr->punit, ptr->p, *punit));
	DynList<double> y =
	  to_dynlist(unit_convert(*ptr->yunit, ptr->y, *yunit));
	ret.append(Sample(ptr->t, ptr->pb, ptr->uod, punit, move(p),
			  target_name, yunit, move(y)));
      }
				
    return ret;
  }

  DynList<Sample> get_pars(const DynList<const VectorDesc*> & ref_vectors,
			   const string & target_name) const
  {
    cout << "get_pars(" << target_name << ")" << endl;
    DynList<Sample> ret;
    auto samples = search_vector(target_name);
    if (samples.is_empty())
      return ret;

    const VectorDesc * fst_vector = ref_vectors.get_first();

    auto ptr = samples.get_first();
    const Unit * punit = ptr->punit;
    const Unit * yunit = ptr->yunit;

    ret.append(Sample(ptr->t, ptr->pb, ptr->uod, punit,
		      to_dynlist(fst_vector->p),
		      target_name, yunit, ptr->gety(fst_vector->p)));

    cout << "p ="; fst_vector->p.for_each([] (auto v) { cout << " " << v; }); cout << endl;
    cout << target_name << " ="; ptr->gety(fst_vector->p).for_each([] (auto v) { cout << " " << v; }); cout << endl;

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
	ret.append(Sample(ptr->t, ptr->pb, ptr->uod, punit, move(p),
			  target_name, yunit, move(y)));
      }

    return ret;
  }

  tuple<DynList<double>, const Unit*, DynList<double>, const Unit*>
  get_target_samples(const string & target_name) const
  {
    auto samples = search_vector(target_name);
    if (samples.is_empty())
      ALEPHTHROW(VarNameNotFound, target_name + " not found in data set");

    auto ptr = samples.remove_first();
    DynList<double> p = to_dynlist(ptr->p);
    const Unit * punit = ptr->punit;
    DynList<double> y = to_dynlist(ptr->y);
    const Unit * yunit = ptr->yunit;
    for (auto it = samples.get_it(); it.has_curr(); it.next())
      {
	ptr = it.get_curr();
	p.append(to_dynlist(unit_convert(*ptr->punit, ptr->p, *punit)));
	y.append(to_dynlist(unit_convert(*ptr->yunit, ptr->y, *yunit)));
      }
				
    return make_tuple(p, punit, y, yunit);
  }

  using Ptype = tuple<string, double, const Unit*>;

  DynList<DynList<Ptype>> get_parameters(const string & target_name) const
  {
    auto samples = search_vector(target_name);
    if (samples.is_empty())
      ALEPHTHROW(VarNameNotFound, target_name + " not found");

    auto ptr = samples.remove_first();
    DynList<size_t> sz = { ptr->p.size() };
    const Unit * punit = ptr->punit;
    Ptype tpar = Ptype("t", ptr->t, &Fahrenheit::get_instance());
    Ptype pbpar = Ptype("pb", ptr->pb, punit);
    const Unit * yunit = ptr->yunit;
    DynList<DynList<Ptype>> ret = zip_maps<DynList<Ptype>>([&] (auto t)
        {
	  return build_dynlist<Ptype>(tpar, pbpar,
				      Ptype("p", get<0>(t), punit),
				      Ptype(target_name, get<1>(t), yunit));
	}, ptr->p, ptr->y);

    for (auto it = samples.get_it(); it.has_curr(); it.next())
      {
	ptr = it.get_curr();
	get<1>(tpar) = ptr->t;
	get<1>(pbpar) = ptr->pb;
	ret.append(zip_maps<DynList<Ptype>>([&] (auto t)
          {
	    return build_dynlist<Ptype>(tpar, pbpar,
					Ptype("p", get<0>(t), punit),
					Ptype(target_name, get<1>(t), yunit));
	  }, unit_convert(*ptr->punit, ptr->p, *punit),
             unit_convert(*ptr->yunit, ptr->y, *yunit))); 
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

	auto pars_list = get_parameters(name_in_data);
	if (not pars_list.all([&par] (auto & l)
             {
	       auto & curr_par = l.get_last();
	       return par.check(VtlQuantity(*get<2>(curr_par),
					    get<1>(curr_par)));
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

  // pressure values, pressure unit,  lab values, target values, target unit
  tuple<DynList<double>, const Unit*, DynList<double>, DynList<double>, const Unit*>
  apply(const Correlation * corr_ptr) const
  {
    const string target_name = corr_ptr->target_name();

    ParList pars; // parameter to be passed to correlation corr_ptr
    DynList<DynList<DynList<Ptype>>> samples;
    for (auto it = corr_ptr->get_preconditions().get_it(); it.has_curr();
	 it.next())
      {
	auto & par = it.get_curr();
	auto * ptr =
	  par.names().find_ptr([this] (auto & p) { return names.has(p.first); });
	if (ptr == nullptr)
	  ALEPHTHROW(VarNameNotFound, "correlation parameter " + par.name +
		   " not found in data set");

	const string & name_in_data = ptr->first;
	if (name_in_data == "t" or name_in_data == "p" or name_in_data == "pb")
	  continue;

	const ConstDesc * const_ptr = search_const(name_in_data);
	if (const_ptr)
	  {
	    pars.insert(par.name, const_ptr->value, const_ptr->unit_ptr);
	    continue;
	  }

	auto s = get_parameters(name_in_data);
	samples.append(s);
      }

    auto target_samples = get_target_samples(target_name);
    const DynList<double> & pressures = get<0>(target_samples);
    const DynList<double> & y = get<2>(target_samples);
    const Unit * punit = get<1>(target_samples);
    const Unit * yunit = get<3>(target_samples);
    if (samples.is_empty())
      samples.append(get_parameters(target_name));

    DynList<double> values;
    for (auto it = samples.get_it(); it.has_curr(); it.next())
      it.get_curr().for_each([&] (auto & l)
        {
	  l.for_each([&] (auto & p) { pars.insert(p); });

	  VtlQuantity result = corr_ptr->compute_by_names(pars, false);
	  values.append(VtlQuantity(*yunit, result).raw());

	  l.for_each([&pars] (auto & v) { pars.remove(v); });
	});

    auto pp = t_unzip(sort(t_zip(pressures, y, values), [] (auto p1, auto p2)
      { return get<0>(p1) < get<0>(p2); }));

    return make_tuple(move(get<0>(pp)), punit, move(get<1>(pp)),
		      move(get<2>(pp)), yunit);
  }

  // t, pb, punit, pval, target unit, target sample, correlation value
  using ResultType = tuple<double, double, const Unit*, double,
			   const Unit*, double, double>;
  DynList<ResultType> tapply(const Correlation * corr_ptr) const
  {
    const string target_name = corr_ptr->target_name();

    ParList pars; // parameter to be passed to correlation corr_ptr
    DynList<Sample> samples;
    for (auto it = corr_ptr->get_preconditions().get_it(); it.has_curr();
	 it.next())
      {
	auto & par = it.get_curr();
	auto * ptr =
	  par.names().find_ptr([this] (auto & p) { return names.has(p.first); });
	if (ptr == nullptr)
	  ALEPHTHROW(VarNameNotFound, "correlation parameter " + par.name +
		   " not found in data set");

	const string & name_in_data = ptr->first;
	if (name_in_data == "t" or name_in_data == "p" or name_in_data == "pb")
	  continue;

	const ConstDesc * const_ptr = search_const(name_in_data);
	if (const_ptr)
	  {
	    pars.insert(par.name, const_ptr->value, const_ptr->unit_ptr);
	    continue;
	  }

	auto s = get_pars(name_in_data);
	samples.append(s);
      }

    auto target_samples = get_pars(target_name);
    auto & first_sample = target_samples.get_first();
    const Unit * yunit = s_yunit(first_sample);
    if (samples.is_empty())
      samples.append(target_samples);

    // using ResultType = tuple<double, double, const Unit*, double,
    // 			   const Unit*, double, double>;
    DynList<ResultType> ret;
    for (auto it = zip_it(samples, target_samples); it.has_curr(); it.next())
      {
	auto tt = it.get_curr();
	Sample & desc = get<0>(tt);
	Sample & ref = get<1>(tt);

	if (s_punit(desc) != s_punit(ref))
	  ALEPHTHROW(InvalidPressureUnit, "mismatch between pressure units " +
		   s_punit(desc)->name + " != " + s_punit(ref)->name);
 
	const Unit * punit = s_punit(desc);
	const double & temp = s_t(desc);
	const double & pb = s_pb(desc);
	const Unit * par_unit = s_yunit(desc);
	const string & par_name = s_yname(desc);
	pars.insert("t", temp, &Fahrenheit::get_instance());
	pars.insert("pb", pb, &psia::get_instance());
	for (auto it = zip_it(s_pvals(desc), s_pvals(ref), s_yvals(desc),
			      s_yvals(ref)); it.has_curr(); it.next())
	  {
	    auto t = it.get_curr();
	    const double & p = get<0>(t); // pressure from desc
	    if (p != get<1>(t))
	      ALEPHTHROW(PressureMismatch, "pressure mismatch between samples " +
		       ::to_string(p) + " != " + ::to_string(get<1>(t)));

	    const double & parval = get<2>(t);
	    const double & yval = get<3>(t);
	    pars.insert("p", p, punit);
	    pars.insert(par_name, parval, par_unit);

	    double result =
	      VtlQuantity(*yunit, corr_ptr->compute_by_names(pars, false)).raw();
	    ret.append(ResultType(temp, pb, punit, p, yunit, yval, result));
	    pars.remove("p");
	    pars.remove(par_name);
	  }
	pars.remove("t");
	pars.remove("pb");
      }

    return ret;
  }

  using Rtype = tuple<double, double, const Unit*, DynList<double>,
		      const Unit*, DynList<double>, DynList<double>>;
  DynList<Rtype> rapply(const Correlation * corr_ptr) const
  {
    const string target_name = corr_ptr->target_name();
    auto target_samples = get_pars(target_name);
    if (target_samples.is_empty())
      ALEPHTHROW(VarNameNotFound, "Correlation target " + target_name +
	       " not found in data set");

    auto & first_sample = target_samples.get_first();
    const Unit * yunit = s_yunit(first_sample);

    ParList pars; // parameter to be passed to correlation corr_ptr
    DynList<Sample> samples;
    for (auto it = corr_ptr->get_preconditions().get_it(); it.has_curr();
	 it.next())
      {
	auto & par = it.get_curr();
	auto * ptr =
	  par.names().find_ptr([this] (auto & p) { return names.has(p.first); });
	if (ptr == nullptr)
	  ALEPHTHROW(VarNameNotFound, "correlation parameter " + par.name +
		   " not found in data set");

	const string & name_in_data = ptr->first;
	if (name_in_data == "t" or name_in_data == "p" or name_in_data == "pb")
	  continue;

	const ConstDesc * const_ptr = search_const(name_in_data);
	if (const_ptr)
	  {
	    pars.insert(par.name, const_ptr->value, const_ptr->unit_ptr);
	    continue;
	  }

	auto s = get_pars(name_in_data);
	samples.append(s);
      }

    if (samples.is_empty())
      samples.append(target_samples);

  // using Rtype = tuple<double, double, const Unit*, DynList<double>,
  // 		      const Unit*, DynList<double>, DynList<double>>;
    DynList<Rtype> ret;
    for (auto it = zip_it(samples, target_samples); it.has_curr(); it.next())
      {
	auto tt = it.get_curr();
	Sample & desc = get<0>(tt);
	Sample & ref = get<1>(tt);

	if (s_punit(desc) != s_punit(ref))
	  ALEPHTHROW(InvalidPressureUnit, "mismatch between pressure units " +
		   s_punit(desc)->name + " != " + s_punit(ref)->name);
 
	const Unit * punit = s_punit(desc);
	const double & temp = s_t(desc);
	const double & pb = s_pb(desc);
	const Unit * par_unit = s_yunit(desc);
	const string & par_name = s_yname(desc);
	pars.insert("t", temp, &Fahrenheit::get_instance());
	pars.insert("pb", pb, &psia::get_instance());
	DynList<double> & pvals = s_pvals(desc); // pressures
	DynList<double> & yvals = s_yvals(desc) ; // lab data
	DynList<double> ycvals; // correlation output
	for (auto it = zip_it(pvals, s_pvals(ref), yvals);
	     it.has_curr(); it.next())
	  {
	    auto t = it.get_curr();
	    const double & p = get<0>(t); // pressure from desc
	    if (p != get<1>(t))
	      ALEPHTHROW(PressureMismatch, "pressure mismatch between samples " +
		       ::to_string(p) + " != " + ::to_string(get<1>(t)));

	    const double & parval = get<2>(t);
	    pars.insert("p", p, punit);
	    pars.insert(par_name, parval, par_unit);

	    double result =
	      VtlQuantity(*yunit, corr_ptr->compute_by_names(pars, false)).raw();
	    ycvals.append(result);
	    pars.remove("p");
	    pars.remove(par_name);
	  }
	ret.append(Rtype(temp, pb, punit, pvals, yunit, yvals, ycvals));
	pars.remove("t");
	pars.remove("pb");
      }

    return ret;
  }

  using TYPE = tuple<double, double, const Unit*, DynList<double>,
		     const Unit*, DynList<double>, DynList<double>>;
  DynList<TYPE> iapply(const Correlation * corr_ptr) const
  {
    const string target_name = corr_ptr->target_name();
    const DynList<const VectorDesc*> target_vectors = search_vector(target_name);
    if (target_vectors.is_empty())
      ALEPHTHROW(VarNameNotFound, "Correlation target " + target_name +
	       " not found in data set");

    auto first_sample_ptr = target_vectors.get_first();
    const Unit * yunit = first_sample_ptr->yunit;

    ParList pars; // parameter to be passed to correlation corr_ptr
    DynList<Sample> samples;
    for (auto it = corr_ptr->get_preconditions().get_it(); it.has_curr();
	 it.next())
      {
	auto & par = it.get_curr();
	auto * ptr =
	  par.names().find_ptr([this] (auto & p) { return names.has(p.first); });
	if (ptr == nullptr)
	  ALEPHTHROW(VarNameNotFound, "correlation parameter " + par.name +
		   " not found in data set");

	const string & name_in_data = ptr->first;
	if (name_in_data == "t" or name_in_data == "p" or name_in_data == "pb")
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
      samples.append(get_pars(target_name));

    // TODO : falta uod en TYPE
  // using TYPE = tuple<double, double, const Unit*, DynList<double>,
  // 		      const Unit*, DynList<double>, DynList<double>>;
    DynList<TYPE> ret;
    for (auto it = zip_it(samples, target_vectors); it.has_curr(); it.next())
      {
	auto tt = it.get_curr();
	Sample & desc = get<0>(tt);
	const VectorDesc * ref_ptr = get<1>(tt);

	// if (s_punit(desc) != ref_ptr->punit)
	//   ALEPHTHROW(InvalidPressureUnit, "mismatch between pressure units " +
	// 	   s_punit(desc)->name + " != " + ref_ptr->punit->name);
 
	const Unit * punit = s_punit(desc);
	const double & temp = s_t(desc);
	const double & pb = s_pb(desc);
	const Unit * par_unit = s_yunit(desc);
	const string & par_name = s_yname(desc);
	pars.insert("t", temp, &Fahrenheit::get_instance());
	pars.insert("pb", pb, punit);
	// TODO uod
	DynList<double> pvals = s_pvals(desc); // pressures
	DynList<double> yvals = ref_ptr->gety(pvals);
	DynList<double> ycvals; // correlation output
	for (auto it = zip_it(pvals, yvals); it.has_curr(); it.next())
	  {
	    auto t = it.get_curr();
	    const double & p = get<0>(t); // pressure from desc
	    const double & parval = get<1>(t);
	    pars.insert("p", p, punit);
	    pars.insert(par_name, parval, par_unit);

	    pars.tbl.for_each([] (auto p)
	     		      {
				cout << p.first << " = " << p.second.first << " " << p.second.second->name << " ";
			      });
	    cout << endl;

	    VtlQuantity result =
	      VtlQuantity(*yunit, corr_ptr->compute_by_names(pars, false));
	    ycvals.append(result.raw());
	    cout << target_name << " = " << result << endl;
	    pars.remove("p");
	    pars.remove(par_name);
	  }
	ret.append(Rtype(temp, pb, punit, pvals, yunit, yvals, ycvals));
	pars.remove("t");
	pars.remove("pb");
      }

    return ret;
  }

  //                             pressure vals, lab vals, target vals, statistics
  using T =
    tuple<const Correlation*, DynList<double>, DynList<double>, CorrStat::Desc>;

  T stats(const Correlation * corr_ptr) const
  {
    auto vals = apply(corr_ptr);
    auto & y = get<2>(vals);
    auto & yc = get<3>(vals);    
    auto stats = CorrStat(y).stats(yc);

    return make_tuple(corr_ptr, move(get<0>(vals)), move(y), move(stats));
  }

  T tstats(const Correlation * corr_ptr) const
  {
    auto vals = tapply(corr_ptr);
    DynList<double> p, y, yc;
    vals.for_each([&p, &y, &yc] (auto & v)
		  {
		    p.append(get<3>(v));
		    y.append(get<5>(v));
		    yc.append(get<6>(v));
		  });
    // TODO: 
    auto stats = CorrStat(y).stats(yc);

    return make_tuple(corr_ptr, move(p), move(y), move(stats));
  }

  T rstats(const Correlation * corr_ptr) const
  {
    cout << "rstats " << corr_ptr->target_name() << endl;
    // using Rtype = tuple<double, double, const Unit*, DynList<double>,
  // 		      const Unit*, DynList<double>, DynList<double>>;
    
    // TODO: unicidad y prdenamiento por y. A ponerlo en tree ya queda ordenado 
    DynList<Rtype> vals = iapply(corr_ptr);
    //          yc,          p,       y
    DynMapTree<double, pair<double, double>> ptbl;
    for (auto it = vals.get_it(); it.has_curr(); it.next())
      {
    	const auto & t = it.get_curr();
    	const DynList<double> & p = get<3>(t);
    	const DynList<double> & y = get<5>(t);
    	const DynList<double> & yc = get<6>(t);
    cout << "p ="; p.for_each([] (auto v) { cout << " " << v; }); cout << endl;
    cout << "y ="; y.for_each([] (auto v) { cout << " " << v; }); cout << endl;
    cout << "yc ="; yc.for_each([] (auto v) { cout << " " << v; }); cout << endl;	for (auto it = zip_it(p, y, yc); it.has_curr(); it.next())
    	  {
    	    auto t = it.get_curr();
    	    const double & p = get<0>(t);
    	    const double & y = get<1>(t);
    	    const double & yc = get<2>(t);
    	    auto ptr = ptbl.search(yc);
    	    if (ptr == nullptr)
    	      ptbl.append(yc, make_pair(p, y));
    	    else if (yc > ptr->second.second)
    	      ptr->second = make_pair(p, y);
    	  }
      }

    DynList<double> p, y, yc;
    for (auto it = ptbl.get_it(); it.has_curr(); it.next())
      {
    	auto pp = it.get_curr();
    	auto & second = pp.second;
    	p.append(second.first);
    	y.append(second.second);
    	yc.append(pp.first);
      }

    cout << "**" << endl;
    cout << "p ="; p.for_each([] (auto v) { cout << " " << v; }); cout << endl;
    cout << "y ="; y.for_each([] (auto v) { cout << " " << v; }); cout << endl;
    cout << "yc ="; yc.for_each([] (auto v) { cout << " " << v; }); cout << endl;

    auto stats = CorrStat(yc).stats(y);

    return make_tuple(corr_ptr, move(p), move(y), move(stats));
  }
};

# endif
