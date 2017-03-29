# ifndef PVT_DATA_H
# define PVT_DATA_H

# include <tpl_array.H>

# include <json.hpp>

# include <tpl_array.H>

# include <correlations/correlation-stats.H>
# include "metadata-exceptions.H"

struct VectorDesc
{
  const string xname = "no-name";
  const string yname = "no-name";
  const Unit * xunit = nullptr;
  const Unit * yunit = nullptr;

  const Array<double> x, y;

  VectorDesc(const string xname, const string yname,
	     const Unit * xunit, const Unit * yunit,
	     const Array<double> & x, const Array<double> & y) noexcept
    : xname(xname), yname(yname), xunit(xunit), yunit(yunit), x(x), y(y)
  {
    assert(is_sorted(x));
  }

  VectorDesc(const string & name = "no-name") : yname(name) {}

  CorrStat::Desc stats(const Array<double> & yc, const Unit * unit_ptr) const
  {
    CorrStat s = y;
    return
      s.stats(yunit == unit_ptr ? yc : unit_convert(*unit_ptr, yc, *yunit));
  }

  bool operator < (const VectorDesc & rhs) const noexcept
  {
    return yname < rhs.yname;
  }

  size_t get_yindex(double yval) const noexcept
  {
    return y.find_index([yval] (auto v) { return v == yval; });
  }

  double gety(double xval) const
  {
    size_t i = x.find_index([xval] (auto x) { return x == xval; });
    if (i >= x.size())
      ZENTHROW(ValueNotFound, ::to_string(xval) + " not found in vector");
    return y(i);
  }

  DynList<DynList<string>> to_dynlist() const
  {
    DynList<DynList<string>> ret = zip_maps<DynList<string>>([] (auto t)
      {
	return build_dynlist<string>(::to_string(get<0>(t)),
				     ::to_string(get<1>(t)));
			       
       }, x, y);
    ret.insert(build_dynlist<string>(xname + " " + xunit->name,
				     yname + " " + yunit->name));
    return ret;
  }

  string to_string() const
  {
    return Aleph::to_string(format_string(to_dynlist()));
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

  void
  add_vector(const string xname, const Unit & xunit, const Array<double> & x,
	     const string yname, const Unit & yunit, const Array<double> & y)
  {
    if (not is_sorted(x))
      ZENTHROW(SamplesUnsorted, "pressure values are not sorted");
    vectors.emplace(xname, yname, &xunit, &yunit, x, y);
    names.append(yname);
  }

  const ConstDesc * search_const(const string & name) const noexcept
  {
    return const_values.search(ConstDesc(name));
  }

  const VectorDesc * search_vector(const string & name) const noexcept
  {
    return vectors.search(VectorDesc(name));
  }

  DynList<double> get_samples(const string & target_name) const
  {
    auto desc_ptr = search_vector(target_name);
    if (desc_ptr == nullptr)
      ZENTHROW(VarNameNotFound, target_name + " not found");
    return desc_ptr->y.to_dynlist();
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
	if (name_in_data == "t" or name_in_data == "p")
	  continue;

	const ConstDesc * const_ptr = search_const(name_in_data);
	if (const_ptr) // is name_in_data as constant?
	  {
	    if ( (not par.check(VtlQuantity(*const_ptr->unit_ptr,
					    const_ptr->value))))
	      return false;
	    continue;
	  }

	const VectorDesc * vector_ptr = search_vector(name_in_data);
	if (vector_ptr == nullptr)
	  return false; // name_in_data not found as constant neither vector

	if (not vector_ptr->y.all([&par, unit_ptr = vector_ptr->yunit] (auto v)
             { return par.check(VtlQuantity(*unit_ptr, v)); }))
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

  //        pressure, target_name
  pair<DynList<double>, DynList<double>> apply(const Correlation * corr_ptr) const
  {
    using Ptype = tuple<string, double, const Unit*>;

    const string target_name = corr_ptr->target_name();
    
    const VectorDesc * vector_ptr = search_vector(target_name);
    if (vector_ptr == nullptr)
      ZENTHROW(VarNameNotFound, target_name + " target name not found");

    assert(vector_ptr->yname == target_name and vector_ptr->xname == "p");

    const DynList<Ptype> pressures =
      vector_ptr->x.maps<Ptype>([vector_ptr] (auto v)
				{
				  return Ptype("p", v, vector_ptr->xunit);
				});

    //            var name, pressures,    values,           unit
    DynList<tuple<string, DynList<double>, DynList<double>, const Unit*>> vars;
    ParList pars;
    for (auto it = corr_ptr->get_preconditions().get_it(); it.has_curr();
	 it.next())
      {
	auto & par = it.get_curr();
	auto * ptr = par.names().find_ptr([this] (auto & p)
					  {
					    return names.has(p.first);
					  });
	if (ptr == nullptr)
	  ZENTHROW(VarNameNotFound, "correlation parameter " + par.name +
		   " not found in data set");

	const string & name_in_data = ptr->first;

	const ConstDesc * const_ptr = search_const(name_in_data);
	if (const_ptr)
	  {
	    pars.insert(par.name, const_ptr->value, const_ptr->unit_ptr);
	    continue;
	  }

	const VectorDesc * vector_ptr = search_vector(name_in_data);
	if (vector_ptr == nullptr)
	  ZENTHROW(VarNameNotFound, name_in_data + " not found in data set");
	vars.append(make_tuple(name_in_data, vector_ptr->x.to_dynlist(),
			       vector_ptr->y.to_dynlist(),
			       vector_ptr->yunit));
      }

    cout << "vars:";
    vars.for_each([] (auto &t)
		  {
		    cout << " " << get<0>(t) << ":";
		    get<2>(t).for_each([] (auto v) { cout << " " << v; });
		  });
    cout << endl;

    DynList<DynList<Ptype>> vpars =
      transpose(vars.maps<DynList<Ptype>>([&pressures] (auto & t)
      { // verify that pressures coincide
	zip_for_each([&t] (auto tt)
          {
	    if (get<1>(get<0>(tt)) != get<1>(tt))
	      ZENTHROW(PressureMismatch, "Pressure mismatch in property " +
	     	       get<0>(t) + ": " + ::to_string(get<1>(get<0>(tt))) +
		       " != " + ::to_string(get<1>(tt)));
	  }, pressures, get<1>(t));	  

	return get<2>(t).template maps<Ptype>([&t] (auto & v)
          { return Ptype(get<0>(t), v, get<3>(t)); });
      }));

    const Unit * expected_unit = vector_ptr->yunit;
    DynList<double> vret;
    for (auto it = vpars.get_it(); it.has_curr(); it.next())
      {
	const auto & l = it.get_curr();
	l.for_each([&pars] (auto & v) { pars.insert(v); });

	VtlQuantity result = VtlQuantity(*expected_unit,
					 corr_ptr->compute_by_names(pars));
	vret.append(result.raw());

	l.for_each([&pars] (auto & v) { pars.remove(v); });
      }

    return make_pair(pressures.maps<double>([] (auto & t) { return get<1>(t); }),
		     move(vret));
  }
};

# endif
