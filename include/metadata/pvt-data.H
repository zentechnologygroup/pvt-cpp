# ifndef PVT_DATA_H
# define PVT_DATA_H

# include <tpl_array.H>

# include <json.hpp>

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
};

struct PvtData
{
  DynSetTree<ConstDesc> const_values; // indexed and sorted by name

  DynSetTree<VectorDesc> vectors;

  DynSetTree<string> names;

  void add_const(const string & name, double v, const Unit & unit)
  {
    const_values.emplace(name, v, &unit);
    names.append(name);
  }

  void add_vector(const string xname, const string yname,
	     const Unit & xunit, const Unit & yunit,
	     const Array<double> & x, const Array<double> & y)
  {
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

  // Return true if corr_ptr can be applied; that is, if the values in
  // this data set matches with correlation development ranges
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

	const string & name_in_data = ptr->first; // can be a name or an alias
	const ConstDesc * const_ptr = search_const(name_in_data);
	if (const_ptr)
	  {
	    if (name_in_data == "t" or name_in_data == "p")
	      continue;
	    if (not par.check(VtlQuantity(*const_ptr->unit_ptr,
					  const_ptr->value)))
	      return false;
	    continue;
	  }

	//const VectorDesc * vector_ptr = 
       }

    return true;
  }

  // DynList<const Correlation*> target_correlations(const VectorDesc & v) const
  // {
  //   const string & tgt_name = v.yname;
  //   return Correlation::
  // }
};

# endif
