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
  const double pb = 0; // in the same unit that p
  const Array<double> p;
  const Unit * punit = nullptr;

  const string yname = "no-name";
  const Unit * yunit = nullptr;
  const Array<double> y;

  VectorDesc(double t, double pb, const Array<double> & p, const Unit * punit,
	     const string & yname, const Unit * yunit, const Array<double> & y)
    noexcept
    : t(t), pb(pb), p(p), punit(punit), yname(yname), yunit(yunit), y(y)
  {
    assert(is_sorted(p));
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
    size_t i = p.find_index([pval] (auto p) { return p == pval; });
    if (i >= p.size())
      ZENTHROW(ValueNotFound, ::to_string(pval) + " not found in vector");
    return y(i);
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
      << pb << " " << punit->name << " :" << endl
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

    auto sibling = search_vector(yname);
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
	DynList<double> p = to_dynlist(unit_convert(*ptr->punit, ptr->p, *punit));
	DynList<double> y = to_dynlist(unit_convert(*ptr->yunit, ptr->y, *yunit));
	ret.append(SampleDesc(ptr->t, punit, p, yunit, y));
      }
				
    return ret;
  }

  tuple<DynList<double>, const Unit*, DynList<double>, const Unit*>
  get_target_samples(const string & target_name) const
  {
    auto samples = search_vector(target_name);
    if (samples.is_empty())
      ZENTHROW(VarNameNotFound, target_name + " not found in data set");

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
      ZENTHROW(VarNameNotFound, target_name + " not found");

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
	if (name_in_data == "t" or name_in_data == "p" or name_in_data == "pb")
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

  // pressure values, pressure unit, target values, target unit
  tuple<DynList<double>, const Unit*, DynList<double>, const Unit*>
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
	  ZENTHROW(VarNameNotFound, "correlation parameter " + par.name +
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

    if (samples.is_empty())
      samples.append(get_parameters(target_name));

    auto units = search_units(target_name);
    const Unit * punit = units.first;
    const Unit * yunit = units.second;
    DynList<double> pressures;
    DynList<double> values;
    for (auto it = samples.get_it(); it.has_curr(); it.next())
      it.get_curr().for_each([&] (auto & l)
        {
	  l.for_each([&] (auto & p)
            {
	      if (get<0>(p) == "p")
		pressures.append(unit_convert(*get<2>(p), get<1>(p), *punit));
	      pars.insert(p);
	    });

	  VtlQuantity result = corr_ptr->compute_by_names(pars, false);
	  values.append(VtlQuantity(*yunit, result).raw());

	  l.for_each([&pars] (auto & v) { pars.remove(v); });
	});

    auto pp = t_unzip(sort(t_zip(pressures, values), [] (auto p1, auto p2)
      { return get<0>(p1) < get<0>(p2); }));

    return make_tuple(move(get<0>(pp)), punit, move(get<1>(pp)), yunit);
  }

  //                                  pressure vals,   target vals,   statistics
  using T =
    tuple<const Correlation*, DynList<double>, DynList<double>, CorrStat::Desc>;

  T stats(const Correlation * corr_ptr) const
  {
    auto samples = get_target_samples(corr_ptr->target_name());
    auto & y = get<2>(samples);
    auto yunit = get<3>(samples);
       
    auto vals = apply(corr_ptr);
    auto yc_unit = get<3>(vals);
    auto yc = unit_convert(*yc_unit, get<2>(vals), *yunit);
    
    auto stats = CorrStat(y).stats(yc);

    return make_tuple(corr_ptr, move(get<0>(vals)), move(yc), move(stats));
  }
};

# endif
