# ifndef PVT_GRID_COMPUTE_H
# define PVT_GRID_COMPUTE_H

# include <parse-csv.H>
# include <tpl_array.H>
# include <tpl_sort_utils.H>
# include <tpl_dynMapTree.H>
# include <utils.H>
# include <units.H>

DEFINE_ZEN_EXCEPTION(MismatchInPressureValues, "pressure values does not match");
DEFINE_ZEN_EXCEPTION(UnsortedPressureValues, "pressure values are not sorted");

class PvtGrid
{
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

  Array<double> p;

  using T = tuple<double, Array<double>, Array<Array<double>>>;

  //          t,          matrix of values associated to temperature t
  // columns in second are ordered by name in var_names array
  Array<T> temps; 

  void process_row(const Array<string> & row,
		   DynMapTree<double, Desc> & tmap,
		   const Array<size_t> & col_indexes,
		   size_t row_idx, size_t ncol, size_t tidx, size_t pidx)
  {
    assert(col_indexes.size() == ncol);
    if (row.size() != ncol)
      ZENTHROW(InvalidCsvRow, "invalid size of " + to_string(row_idx) +
	       "-th row");
    if (not row.all([] (auto &s) { return s.size() == 0 or is_double(s); }))
      ZENTHROW(InvalidConversion, "a value in row " + to_string(row_idx) +
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

  PvtGrid(istream & in)
  {
    Array<string> header = csv_read_row(in);
    DynList<pair<string, size_t>> names_to_idx;
    size_t i = 0, tidx = 0, pidx = 0;
    for (auto it = header.get_it(); it.has_curr(); it.next(), ++i)
      {
	auto & s = it.get_curr();
	auto parts = split(s, ' ');
	if (parts.size() != 2)
	  ZENTHROW(InvalidCsvHeader, "Invalid format in column " + to_string(i));
	const string & var_name = parts[0];
	const string & unit_name = parts[1];
	const Unit * unit_ptr = Unit::search(unit_name);
	names_to_idx.append(make_pair(var_name, i));
	if (unit_ptr == nullptr)
	  ZENTHROW(UnitNotFound, "unit " + unit_name + " not found");
	if (var_name == "t")
	  {
	    tunit_ptr = unit_ptr;
	    tidx = i;
	  }
	else if (var_name == "p")
	  {
	    punit_ptr = unit_ptr;
	    pidx = i;
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
    DynMapTree<double, Desc> tmap;
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
	
	if (not is_sorted(desc.p))
	  ZENTHROW(UnsortedPressureValues,
		   "pressure values associated to temp " + to_string(t) +
		   " are not sorted");
	
	temps.append(T(t, move(desc.p), move(desc.vals)));
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
	const T & tt = tit.get_curr();
	const double & t = get<0>(tt);

	const Array<double> & p = get<1>(tt);
	const Array<Array<double>> & vals = get<2>(tt);
	for (auto rit = zip_it(p, vals); rit.has_curr(); rit.next())
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

  pair<size_t, size_t> search_temperature(const double t) const
  {
    const double & t1 = get<0>(temps.get_first());
    const double & t2 = get<0>(temps.get_last());
    if (not (t >= t1 and t <= t2))
      ZENTHROW(OutOfRange, "temperature value " + to_string(t) +
	       " is out of range [" + to_string(t1) + ", " +
	       to_string(t2) + "]");

    T t_t;
    get<0>(t_t) = t;
    const long i = Aleph::binary_search(temps, t_t, [] (auto & t1, auto & t2)
					{ return get<0>(t1) < get<0>(t2); });
    assert(i >= 0 and size_t(i) < temps.size());
    const T & found_entry = temps(i);
    const double & found_t = get<0>(found_entry);
    if (found_t == t)
      return pair<size_t, size_t>(i, i);
    else if (t < found_t)
      {
	assert(i > 0);
	return pair<size_t, size_t>(i - 1, i);
      }
    else
      {
	assert(size_t(i) < temps.size() - 1);
	return pair<size_t, size_t>(i, i + 1);
      }
  }

  pair<size_t, size_t> search_presure(const T & desc, const double & p) const
  {
    const Array<double> & pvals = get<1>(desc);
    const double & fst_p = pvals.get_first();
    const double & lst_p = pvals.get_last();
    if (p < fst_p or p > lst_p)
      ZENTHROW(OutOfMemory, "pressure value " + to_string(p) +
	       "is out of range [" + to_string(fst_p) + ", " +
	       to_string(lst_p) + "]");
    
    const long i = binary_search(pvals, p);

    assert(i >= 0 and size_t(i) < pvals.size());
    const double & found_p = pvals(i);
    if (found_p == p)
      return pair<size_t, size_t>(i, i);
    else if (p < found_p)
      {
	assert(i > 0);
	return pair<size_t, size_t>(i - 1, i);
      }
    else
      {
	assert(size_t(i) < pvals.size() - 1);
	return pair<size_t, size_t>(i, i + 1);
      }
  }

public:

  size_t property_index(const string & name) const
  {
    pair<string, const Unit*> p;
    p.first = name;
    long i = binary_search(var_names, p, [] (auto & p1, auto & p2)
			   { return p1.first < p2.first; });
    if (i < 0 or size_t(i) >= var_names.size() or var_names(i).first != name)
      ZENTHROW(NameNotFound, "var name " + name + " not found");
    return i;
  }

private:

  double interpolate_p(const T & desc, const double p, size_t name_idx) const
  {
    const Array<double> & pvals = get<1>(desc);
    const Array<Array<double>> & vals = get<2>(desc);
    const auto p_idx = search_presure(desc, p);
    
    const double & p1 = pvals(p_idx.first);
    const Array<double> & vals1 = vals(p_idx.first);    
    const double & y1 = vals1(name_idx);
    if (y1 == Unit::Invalid_Value)
      ZENTHROW(OutOfRange, "for t = " + to_string(get<0>(desc)) + " p = " +
	       to_str(p) + " : value of " + var_names(name_idx).first +
	       " out of grid range"); 
    if (p == p1)
      return y1;

    const double & p2 = pvals(p_idx.second);
    const Array<double> & vals2 = vals(p_idx.second);
    const double & y2 = vals2(name_idx);
    if (y2 == Unit::Invalid_Value)
      ZENTHROW(OutOfRange, "for t = " + to_string(get<0>(desc)) + " p = " +
	       to_str(p) + " : value of " + var_names(name_idx).first +
	       " out of grid range");    
    if (p == p2)
      return y2;

    return ::interpolate(p1, p2, y1, y2, p);
  }
  
public:

  VtlQuantity
  compute(const size_t name_idx,
	  const VtlQuantity & temp, const VtlQuantity & pressure) const
  {
    assert(name_idx < var_names.size());

    const double & t = temp.raw();
    const double & p = pressure.raw();
    const Unit * unit_ptr = var_names(name_idx).second;

    auto t_idx = search_temperature(t);
    const T & desc1 = temps(t_idx.first);
    const double & t1 = get<0>(desc1);
    if (t == t1)
      return VtlQuantity(*unit_ptr, interpolate_p(desc1, p, name_idx));

    const T & desc2 = temps(t_idx.second);
    const double & t2 = get<0>(desc2);
    if (t == t2)
      return VtlQuantity(*unit_ptr, interpolate_p(desc1, p, name_idx));

    const double y1 = interpolate_p(desc1, p, name_idx);
    const double y2 = interpolate_p(desc2, p, name_idx);

    return VtlQuantity(*unit_ptr, ::interpolate(t1, t2, y1, y2, t));
  }

  VtlQuantity compute(const string & name,
		      const VtlQuantity & temp,
		      const VtlQuantity & pressure) const
  {
    return compute(property_index(name), temp, pressure);
  }

  VtlQuantity
  operator () (const string & name,
	       const VtlQuantity & temp,
	       const VtlQuantity & pressure) const
  {
    return compute(name, temp, pressure);
  }

  VtlQuantity
  operator () (const size_t name_idx, 
	       const VtlQuantity & temp,
	       const VtlQuantity & pressure) const
  {
    return compute(name_idx, temp, pressure);
  }
};

# endif
