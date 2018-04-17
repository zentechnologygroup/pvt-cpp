/** This file contains the base machinery needed for smoothing bo and
   uo properties of a grid pvt 
   
   Aleph-w Leandro Rabindranath Leon
 */

# ifndef PVT_GRID_COMPUTE_H
# define PVT_GRID_COMPUTE_H

# include <parse-csv.H>
# include <tpl_array.H>
# include <tpl_sort_utils.H>
# include <tpl_dynMapTree.H>
# include <pvt-units.H>

DEFINE_ALEPH_EXCEPTION(MismatchInPressureValues,
		       "pressure values does not match");
DEFINE_ALEPH_EXCEPTION(UnsortedPressureValues,
		       "pressure values are not sorted");
DEFINE_ALEPH_EXCEPTION(PropertyNotFound, "Essential property not found");

/**
   
   @ingroup Correlations
   @author Leandro Rabindranath Leon
*/
struct PvtGrid
{
  mutable bool valid = false;
  const Unit * tunit_ptr = nullptr;
  const Unit * punit_ptr = nullptr;

  // names and units alphabetically ordered by name as read in the
  // csv's header except t and p
  Array<pair<string, const Unit*>> var_names; 

  struct Desc
  {
    Array<string> names;
    Array<double> p;
    Array<Array<double>> vals;
  };

  struct Tdesc
  {
    double t;
    Array<double> p;
    Array<Array<double>> vals;
    Tdesc() {}
    Tdesc(double __t, Array<double> && __p, Array<Array<double>> && __vals)
      : t(__t), p(move(__p)), vals(move(__vals)) {}
    double val(const size_t prop_idx, const size_t p_idx) const
    {
      return vals[p_idx][prop_idx];
    }
    double val(const size_t prop_idx, const double pressure) const
    {
      long p_idx = binary_search(p, pressure, 0, p.size() - 1);
    }
  };

  Array<Tdesc> temps; 

  //                   t,     pb
  mutable DynMapTree<double, double> pb_table;

  void process_row(const Array<string> & row,
		   DynMapTree<double, Desc> & tmap,
		   const Array<size_t> & col_indexes,
		   size_t row_idx, size_t ncol, size_t tidx, size_t pidx)
  {
    assert(col_indexes.size() == ncol);
    if (row.size() != ncol)
      ALEPHTHROW(InvalidCsvRow, "invalid size of " + to_string(row_idx) +
	       "-th row");
    if (not row.all([] (auto &s) { return s.size() == 0 or is_double(s); }))
      ALEPHTHROW(InvalidConversion, "a value in row " + to_string(row_idx) +
	       " cannot be converted to double");

    double t = atof(row(tidx));
    Desc & desc = tmap[t];
    Array<double> vals;
    for (auto it = col_indexes.get_it(); it.has_curr(); it.next())
      {
	const size_t i = it.get_curr();
	const string & v = row(i);
	double val = v.size() ? atof(row(i)) : Unit::Invalid_Value;
	if (i == pidx)
	  desc.p.append(val);
	else if (i != tidx)
	  vals.append(val);
      }
    desc.vals.append(move(vals));
  }

public:

  bool is_valid() const noexcept { return valid; }

  PvtGrid() {}

  PvtGrid(PvtGrid && grid)
    : valid(true), tunit_ptr(grid.tunit_ptr), punit_ptr(grid.punit_ptr),
      var_names(move(grid.var_names)), temps(move(grid.temps)) {}

  PvtGrid & operator = (PvtGrid && grid)
  {
    swap(valid, grid.valid);
    swap(tunit_ptr, grid.tunit_ptr);
    swap(punit_ptr, grid.punit_ptr);
    swap(var_names, grid.var_names);
    swap(temps, grid.temps);
    return *this;
  }

  PvtGrid(istream & in) : valid(true)
  {
    Array<string> header = csv_read_row(in);
    if (not header.exists([] (auto & s)
			  { return split_to_list(s, " ")[0] == "t"; }))
      ALEPHTHROW(InvalidCsvHeader, "csv header does not contain t field");
    
    if (not header.exists([] (auto & s)
			  { return split_to_list(s, " ")[0] == "p"; }))
      ALEPHTHROW(InvalidCsvHeader, "csv header does not contain p field");
    
    DynList<pair<string, size_t>> names_to_idx;
    size_t i = 0, tidx = 0, pidx = 0;
    for (auto it = header.get_it(); it.has_curr(); it.next(), ++i)
      {
	auto & s = it.get_curr();
	auto parts = split(s, ' ');
	if (parts.size() != 2)
	  ALEPHTHROW(InvalidCsvHeader, "Invalid format in column " +
		   to_string(i));
	const string & var_name = parts[0];
	const string & unit_name = parts[1];
	const Unit * unit_ptr = Unit::search(unit_name);
	names_to_idx.append(make_pair(var_name, i));
	if (unit_ptr == nullptr)
	  ALEPHTHROW(UnitNotFound, "unit " + unit_name + " not found");
	if (var_name == "t")
	  {
	    tunit_ptr = unit_ptr;
	    tidx = i; // temperature column index in csv
	  }
	else if (var_name == "p")
	  {
	    punit_ptr = unit_ptr;
	    pidx = i; // pressure column index in csv
	  }
	var_names.append(make_pair(var_name, unit_ptr));
      }

    in_place_sort(var_names, [] (auto & p1, auto & p2)
		  { return p1.first < p2.first; });
    in_place_sort(names_to_idx, [] (auto & p1, auto & p2)
		  { return p1.first < p2.first; });

    Array<size_t> col_indexes =
      names_to_idx.maps<size_t>([] (auto p) { return p.second; });

    const size_t & ncol = header.size();
    DynMapTree<double, Desc> tmap; // temperature to Desc mapping 
    for (size_t i = 1; in.good(); ++i)
      {
	Array<string> row = csv_read_row(in);
	if (row.size() == 0)
	  break;
	process_row(row, tmap, col_indexes, i, ncol, tidx, pidx);
      }

    for (auto it = tmap.get_it(); it.has_curr(); it.next())
      {
	auto & curr = it.get_curr();
	const double & t = curr.first;
	Desc & desc = curr.second;
	
	if (not is_sorted(desc.p, std::less_equal<double>()))
	  {
	    size_t i = search_inversion(desc.p).second;
	    ALEPHTHROW(UnsortedPressureValues,
		     "pressure values associated to temp " + to_string(t) +
		     " are not sorted " +
		     join(desc.p.take(i)) + " * " + join(desc.p.drop(i)));
	  }
	
	temps.append(Tdesc(t, move(desc.p), move(desc.vals)));
      }

    var_names = var_names.filter([] (auto & p)
				 { return p.first != "t" and p.first != "p"; });
  }

  friend ostream & operator << (ostream & out, const PvtGrid & grid)
  {
    out << "t " << grid.tunit_ptr->name << ", p " << grid.punit_ptr->name;
    for (auto it = grid.var_names.get_it(); it.has_curr(); it.next())
      {
	auto & p =it.get_curr();
	out << ", " << p.first << " " << p.second->name;
      }
    out << endl;

    for (auto tit = grid.temps.get_it(); tit.has_curr(); tit.next())
      {
	const Tdesc & d = tit.get_curr();
	const double & t = d.t;
	for (auto rit = zip_it(d.p, d.vals); rit.has_curr(); rit.next())
	  {
	    auto tt = rit.get_curr();
	    const double & p = get<0>(tt);
	    const Array<double> & vals = get<1>(tt);
	    out << t << "," << p;
	    for (auto it = vals.get_it(); it.has_curr(); it.next())
	      {
		const auto & val = it.get_curr();
		out << ",";
		if (val != Unit::Invalid_Value)
		  out << val;
	      }
	    out << endl;
	  }
      }

    return out;
  }

private:

  struct RangeDesc
  {
    size_t first, second;
    enum class Type { Left, Internal, Right, Equal };
    Type type;

    RangeDesc(const size_t first, const size_t second, const Type & type)
      noexcept
      : first(first), second(second), type(type)
    {
      assert(first <= second);
    }
  };

  // return first and second indexes of temperature
  RangeDesc search_temperature(const double t) const
  {
    const double & t1 = temps.get_first().t;
    const double & t2 = temps.get_last().t;

    assert(t1 < t2);
    
    if (t < t1)
      return RangeDesc(0, 1, RangeDesc::Type::Left);
    
    if (t > t2)
      {
	const size_t & n = temps.size();
	return RangeDesc(n - 2, n - 1, RangeDesc::Type::Right);
      }

    Tdesc d;
    d.t = t;
    const long i = Aleph::binary_search(temps, d, [] (auto & d1, auto & d2)
					{ return d1.t < d2.t; });
    assert(i >= 0 and size_t(i) < temps.size());
    const Tdesc & found_entry = temps(i);
    const double & found_t = found_entry.t;
    if (found_t == t)
      return RangeDesc(i, i, RangeDesc::Type::Equal);
    else if (t < found_t)
      {
	assert(i > 0);
	return RangeDesc(i - 1, i, RangeDesc::Type::Internal);
      }
    else
      {
	assert(size_t(i) < temps.size() - 1);
	return RangeDesc(i, i + 1, RangeDesc::Type::Internal);
      }
  }

  RangeDesc search_presure(const Tdesc & desc, const double & p) const
  {
    const Array<double> & pvals = desc.p; // sorted pressure
    const double & fst_p = pvals.get_first(); // minimum pressure
    const double & lst_p = pvals.get_last();  // maximum pressure

    assert(fst_p < lst_p);

    if (p < fst_p)
      return RangeDesc(0, 1, RangeDesc::Type::Left);

    if (p > lst_p)
      {
	const size_t & n = pvals.size();
	return RangeDesc(n - 2, n - 1, RangeDesc::Type::Right);
      }
    
    const long i = binary_search(pvals, p);

    assert(i >= 0 and size_t(i) < pvals.size());
    const double & found_p = pvals(i);
    if (found_p == p)
      return RangeDesc(i, i, RangeDesc::Type::Equal);
    else if (p < found_p)
      {
	assert(i > 0);
	return RangeDesc(i - 1, i, RangeDesc::Type::Internal);
      }
    else
      {
	assert(size_t(i) < pvals.size() - 1);
	return RangeDesc(i, i + 1, RangeDesc::Type::Internal);
      }
  }

public:

  pair<size_t, const Unit*> property_pair(const string & name) const
  {
    pair<string, const Unit*> p;
    p.first = name;
    long i = binary_search(var_names, p, [] (auto & p1, auto & p2)
			   { return p1.first < p2.first; });
    if (i < 0 or size_t(i) >= var_names.size() or var_names(i).first != name)
      ALEPHTHROW(NameNotFound, "var name " + name + " not found");
    assert(var_names(i).first == name);
    return make_pair(i, var_names(i).second);
  }

  size_t property_index(const string & name) const
  {
    return property_pair(name).first;
  }

  bool has_name(const string & name) const
  {
    pair<string, const Unit*> p;
    p.first = name;
    long i = binary_search(var_names, p, [] (auto & p1, auto & p2)
			   { return p1.first < p2.first; });
    return i >= 0 and size_t(i) < var_names.size() and
				  var_names(i).first == name;
  }

  void make_pb_table() const
  {
    if (not pb_table.is_empty())
      return;
    
    if (not has_name("pb"))
      ALEPHTHROW(PropertyNotFound, "pb property not found in data set");

    const size_t pb_idx = property_index("pb");
    for (auto it = temps.get_it(); it.has_curr(); it.next())
      {
	const Tdesc & d = it.get_curr();
	const double & t = d.t;
	const double & pb = d.vals[0][pb_idx];
	assert(d.vals.size() == d.p.size());
	if (not d.vals.all([pb, pb_idx] (auto & a) { return a[pb_idx] == pb; }))
	  ALEPHTHROW(InvariantError, "found inconsistency with pb");
	pb_table.insert(t, pb);
      }
  }

  DynList<pair<double, double>> t_pb_list() const
  {
    return pb_table.items();
  }

  void remove(const string & name)
  {
    const size_t idx = property_index(name);
    close_gap(&var_names.base(), var_names.size(), idx);
    var_names.remove_last();
    close_gap(&temps.base(), temps.size(), idx);
    temps.remove_last();
  }

private:

  double interpolate_p(const Tdesc & d, const double p, size_t name_idx) const
  {
    const Array<double> & pvals = d.p;
    const Array<Array<double>> & vals = d.vals;
    const RangeDesc p_idx = search_presure(d, p);

    const double & p1 = pvals(p_idx.first);
    const Array<double> & vals1 = vals(p_idx.first);    
    const double & y1 = vals1(name_idx);
    if (y1 == Unit::Invalid_Value)
      ALEPHTHROW(OutOfRange, "for t = " + to_string(d.t) + " p = " +
	       to_str(p) + " : value of " + var_names(name_idx).first +
	       " out of grid range"); 

    const double & p2 = pvals(p_idx.second);
    const Array<double> & vals2 = vals(p_idx.second);
    const double & y2 = vals2(name_idx);

    assert(p1 <= p2);

    double y = 0;
    switch (p_idx.type)
      {
      case RangeDesc::Type::Equal:
	assert(p == p1);
	y = y1;
	break;
      case RangeDesc::Type::Internal:
	if (y2 == Unit::Invalid_Value)
	  ALEPHTHROW(OutOfRange, "for t = " + to_string(d.t) + " p = " +
		   to_str(p) + " : value of " + var_names(name_idx).first +
		   " out of grid range");    
	y = interpolate(p1, p2, y1, y2, p);
	break;
      case RangeDesc::Type::Left:
	if (y2 == Unit::Invalid_Value)
	  ALEPHTHROW(OutOfRange, "for t = " + to_string(d.t) + " p = " +
		   to_str(p) + " : value of " + var_names(name_idx).first +
		   " out of grid range");    
	y = extrapolate_left(p1, p2, y1, y2, p);
	break;
      case RangeDesc::Type::Right:
	if (y2 == Unit::Invalid_Value)
	  ALEPHTHROW(OutOfRange, "for t = " + to_string(d.t) + " p = " +
		   to_str(p) + " : value of " + var_names(name_idx).first +
		   " out of grid range");    
	y = extrapolate_right(p1, p2, y1, y2, p);
	break;
      }
    return y;
  }
  
public:

  VtlQuantity compute(const size_t name_idx,
		      const VtlQuantity & temp,
		      const VtlQuantity & pressure) const
  {
    assert(name_idx < var_names.size());

    const double & t = temp.raw();
    const double & p = pressure.raw();
    const Unit * unit_ptr = var_names(name_idx).second;

    const RangeDesc t_idx = search_temperature(t);
    
    const Tdesc & desc1 = temps(t_idx.first);
    const double & t1 = desc1.t;
    
    const Tdesc & desc2 = temps(t_idx.second);
    const double & t2 = desc2.t;

    assert(t1 <= t2);

    const double y1 = interpolate_p(desc1, p, name_idx);

    VtlQuantity ret = VtlQuantity::null_quantity;
    switch (t_idx.type)
      {
      case RangeDesc::Type::Equal:
	ret = VtlQuantity(*unit_ptr, y1);
	break;
      case RangeDesc::Type::Internal:
	{
	  const double y2 = interpolate_p(desc2, p, name_idx);
	  ret = VtlQuantity(*unit_ptr, interpolate(t1, t2, y1, y2, t));
	}
	break;
      case RangeDesc::Type::Left:
	{
	  const double y2 = interpolate_p(desc2, p, name_idx);
	  ret = VtlQuantity(*unit_ptr, extrapolate_left(t1, t2, y1, y2, t));
	}
	break;
      case RangeDesc::Type::Right:
	{
	  const double y2 = interpolate_p(desc2, p, name_idx);
	  ret = VtlQuantity(*unit_ptr, extrapolate_right(t1, t2, y1, y2, t));
	}
	break;
      }
    return ret;
  }

  struct Property
  {
    double t; // in Fahrenheit
    double pb = Unit::Invalid_Value; // in the same unit than grid pressure unit
    double y_in_pb = Unit::Invalid_Value; // value of property at pb (when applies)
    DynList<double> p;
    DynList<double> y;
    size_t n = 0;
  };

  struct PropertyList
  {
    const Unit * tunit = nullptr;
    const Unit * punit = nullptr;
    const Unit * yunit = nullptr;
    mutable string name = "INVALID";
    DynList<Property> l;
  };

  PropertyList get_full_property(const string & name) const
  {
    const pair<size_t, const Unit*> name_pair = property_pair(name);
    const size_t & name_idx = name_pair.first;
    
    const pair<size_t, const Unit*> pb_pair = property_pair("pb");
    const size_t & pb_idx = pb_pair.first;
    const Unit * pb_unit = pb_pair.second;

    cout << "pb_idx = " << pb_idx << endl;

    PropertyList ret;
    ret.tunit = tunit_ptr;
    ret.punit = punit_ptr;
    ret.yunit = name_pair.second;
    ret.name = name;

    for (auto it = temps.get_it(); it.has_curr(); it.next())
      {
	const Tdesc & d = it.get_curr();
	assert(d.p.size() == d.vals.size());
	Property pro;
	pro.t = d.t;
	pro.p = d.p.to_dynlist();
	pro.y =
	  d.vals.maps<double>([name_idx] (auto & a) { return a(name_idx); });
	pro.n = d.p.size();
	const double & stored_pb = d.vals[0][pb_idx];
	if (stored_pb != Unit::Invalid_Value)
	  {
	    pro.pb = punit_ptr == pb_unit ? stored_pb :
	      unit_convert(*pb_unit, stored_pb, *punit_ptr);
	    const size_t pb_pos = zip_find_index([pb = pro.pb] (auto t)
						 { return get<0>(t) == pb; },
						 pro.p, pro.y);
	    cout << join(var_names.maps<string>([] (auto & p)
						{ return p.first; })) << endl
	      << "vals size = " << d.vals.size() << endl
	      << "name_idx = " << name_idx << endl
	      << "d.vals[name_idx].size() = " << d.vals[name_idx].size() << endl;
	    pro.y_in_pb = d.vals[name_idx][pb_pos];
	  }
	ret.l.append(move(pro));
      }
    return ret;
  }

  VtlQuantity compute(const string & name,
		      const VtlQuantity & temp,
		      const VtlQuantity & pressure) const
  {
    return compute(property_index(name), temp, pressure);
  }

  VtlQuantity operator () (const string & name,
			   const VtlQuantity & temp,
			   const VtlQuantity & pressure) const
  {
    return compute(name, temp, pressure);
  }

  VtlQuantity operator () (const size_t name_idx, 
			   const VtlQuantity & temp,
			   const VtlQuantity & pressure) const
  {
    return compute(name_idx, temp, pressure);
  }
};

# endif
