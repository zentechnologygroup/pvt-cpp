
# ifndef EMPIRICAL_DATA_H
# define EMPIRICAL_DATA_H 1

# include <tuple>

# include <parse-csv.H>
# include <ah-string-utils.H>
# include <ah-convert.H>
# include <ahSort.H>
# include <ah-comb.H>
# include <ah-zip.H>
# include <ahSort.H>

# include <units/pvt-units.H>
# include <correlations/pvt-correlations.H>
# include <correlations/defined-correlation.H>

# include <json.hpp>

using namespace std;
using namespace Aleph;

using json = nlohmann::json;

struct EmpiricalData
{
  string name = "Test de Alberto Valderrama";

  string desc = "Usado para pruebas, aparentemente de un pozo en Monagas";

  size_t num_const = 0;
  Array<string> const_names;
  Array<double> const_vals;
  Array<const Unit*> const_units;

  DynMapTree<string, pair<double, const Unit*>> const_table;

  struct VarSet
  {
    string name = "NO-NAME";
    string desc = "NO-DESC";

    VarSet() {}
    VarSet(const string & name, const string & desc)
      : name(name), desc(desc) {}

    size_t num_var = 0;
    Array<string> var_names; // pressure Rs bo uo
    Array<const Unit*> var_units; // of above names
    size_t num_samples = 0;
    Array<Array<double>> samples; // must have the same number of
                                  // columns than var_names

    Array<double> values(const string & name) const
    {
      auto i = var_names.find_index([&name] (const auto & n)
				    { return name == n; });
      if (i >= var_names.size())
	{
	  ostringstream s;
	  s << "var name " << name << " not found in data set name "
	    << this->name;
	  throw domain_error(s.str());
	}

      Array<double> ret(var_names.size());
      for (size_t j = 0; j < num_samples; ++j)
	ret.append(samples(j)(i));

      return ret;
    }

    bool contains_name(const string & name) const noexcept
    {
      return var_names.exists([&name] (const auto & s) { return s == name; });
    }

    size_t name_index(const string & name) const noexcept
    {
      return
	var_names.find_index([&name] (const auto & s) { return s == name; });
    }

    json to_json() const
    {
      json j;
      j["name"] = name;
      j["description"] = desc;
      j["num vars"] = num_var;
      j["num samples"] = num_samples;
      j["variables"] = to_vector(t_zip(var_names, var_units).
	maps<json>([] (const auto & t)
		  {
		    json ret;
		    ret["name"] = get<0>(t);
		    ret["unit"] = get<1>(t)->symbol;
		    return ret;
		  }));

      for (auto it = var_names.get_it(); it.has_curr(); it.next())
	{
	  const auto & var_name = it.get_curr();
	  j["samples"][var_name] = to_vector(values(var_name));
	}

      return j;
    }

    string to_string() const
    {
      ostringstream s;
      s << "Varset " << name << " " << desc << endl;
      auto mat = samples.maps<DynList<string>>([this] (const auto & row)
        {
	  return
	  row.template maps<string>([] (auto v) { return ::to_string(v); });
	});

      mat.insert(t_zip(var_names, var_units).maps<string>([] (auto t)
        {
	  return get<0>(t) + " " + get<1>(t)->symbol;
	}));

      s << Aleph::to_string(format_string(mat));

      return s.str();
    }

    void set_from_json(const json & j)
    {
      name = j["name"];
      desc = j["description"];
      num_var = j["num vars"];
      num_samples = j["num samples"];
      for (const auto & item : j["variables"])
	{
	  var_names.append(item["name"]);
	  var_units.append(Unit::search_by_symbol(item["unit"]));
	}

      for (size_t i = 0; i < num_samples; ++i)
	samples.append(Array<double>(rep<double>(num_var)));

      size_t col = 0;
      for (auto it = var_names.get_it(); it.has_curr(); it.next(), ++col)
	{
	  const auto & vname = it.get_curr();
	  size_t row = 0;
	  for (auto v : j["samples"][vname])
	    samples[row++][col] = v;
	}
    }

    /// Return samples array sorted by the column name `name`
    DynList<DynList<double>> sort_by_name(const string & name) const
    {
      size_t col =
	var_names.find_index([&name] (const auto & s) { return s == name; });
      if (col >= var_names.size())
	{
	  ostringstream s;
	  s << "VarSet::sort_by_name(" << name << "): name not found";
	  throw domain_error(s.str());
	}

      auto ret = sort(samples, [&col] (auto & row1, auto & row2)
		      {
			return row1[col] < row2[col];
		      });

      return ret.maps<DynList<double>>([] (const auto & row)
        {
	  return row.template maps<double>([] (auto v) { return v; });
	});
    }

    /// Return a copy of sample involving the rows concerned to
    /// correlation `ptr`. The matrix is sorted according to the col name
    tuple<DynList<DynList<double>>, DynList<double>>
    get_set_by_correlation(const Correlation * ptr, const string & name) const
    {
      size_t col =
	var_names.find_index([&name] (const auto & s) { return s == name; });
      if (col >= var_names.size())
	{
	  ostringstream s;
	  s << "VarSet::get_set_by_correlation(): name " << name
	    << " not found";
	  throw domain_error(s.str());
	}

      const string target_name = ptr->target_name();
      size_t target_col = var_names.find_index([&target_name] (const auto & s)
					       { return s == target_name; });
      if (target_col >= var_names.size())
	{
	  ostringstream s;
	  s << "VarSet::get_set_by_correlation(): correlation target name "
	    << target_name << " not found";
	  throw domain_error(s.str());
	}

      auto mat = sort(samples, [&col] (auto & row1, auto & row2)
		      {
			return row1[col] < row2[col];
		      });

      auto col_indexes =
	ptr->get_preconditions().maps<size_t>([this] (const auto & par)
          {
	    return var_names.find_index([&par] (const auto & s)
	      {
		return s == par.name;
	      });
	  });
      
      DynList<DynList<double>> smat; // samples matrix
      for (size_t i = 0; i < mat.size(); ++i)
	{
	  const auto & row = mat(i);
	  DynList<double> l =
	    col_indexes.maps<double>([&row] (auto i) { return row(i); });
	  smat.append(l);
	}

      DynList<double> y;
      for (size_t i = 0; i < mat.size(); ++i)
	y.append(mat(i)(target_col));

      return make_tuple(smat, y);
    }
  };

  Array<VarSet> var_sets;

  /// Returns all the names contained in this data set
  DynList<string> names() const
  {
    DynSetTree<string> ret = const_names;
    for (auto it = var_sets.get_it(); it.has_curr(); it.next())
      it.get_curr().var_names.for_each([&ret] (const auto & s)
				       { ret.append(s); });

    return ret.keys();
  }

  using ConstDef = tuple<string, double, const Unit *>;

  ConstDef find_const(const string & name) const
  {
    for (auto it = get_zip_it(const_names, const_vals, const_units);
	 it.has_curr(); it.next())
      {
	auto t = it.get_curr();
	if (get<0>(t) == name)
	  return t;
      }

    ostringstream s;
    s << "const name " << name << " not found in data set";
    throw domain_error(s.str());
  }

  tuple<bool, double, const Unit*> search_const(const string & name) const
  {
    auto p = const_table.search(name);
    if (p == nullptr)
      return make_tuple(false, 0.0, &Unit::null_unit);

    return make_tuple(true, p->second.first, p->second.second);
  }

  void check_constant(const string & name, const PhysicalQuantity & pq) const
  {
    auto p = const_table.search(name);
    if (p == nullptr)
      {
	ostringstream s;
	s << "EmpiricalData::check_constant: constant name " << name
	  << " not found";
	throw domain_error(s.str());
      }

    const auto & physical_quantity = p->second.second->physical_quantity;
    if (&physical_quantity != &pq)
      {
	ostringstream s;
	s << "EmpiricalData::check_constant: Physical quantity of name "
	  << name << " is " << physical_quantity << " but it must be "
	  << pq.name;
	throw domain_error(s.str());
      }

    try
      {
	VtlQuantity q(*p->second.second, p->second.first);
	return;
      }
    catch (exception &e)
      {
	ostringstream s;
	s << "EmpiricalData::check_constant: constant " << name << ": "
	  << e.what();
	throw domain_error(s.str());
      }
  }

  void check_variable(size_t set_i, const string & name,
		      const PhysicalQuantity & pq) const
  {
    const auto & vset = var_sets[set_i];
    for (auto it = get_zip_it(vset.var_names, vset.var_units); it.has_curr();
	 it.next())
      {
	auto t = it.get_curr();
	const string & vname = get<0>(t);
	if (vname != name)
	  continue;

	const auto & physical_quantity = get<1>(t)->physical_quantity;
	if (&physical_quantity != &pq)
	  {	    
	    ostringstream s;
	    s << "Physical quantity of name " << name << " is "
	      << physical_quantity << " but it must be " << pq.name;
	    throw domain_error(s.str());
	  }

	if (not vset.values(name).is_empty())
	  return;

	ostringstream s;
	s << "var name " << name << " in var set " << vset.name
	  << " does not contain any sample";
	throw domain_error(s.str());
      }

    ostringstream s;
    s << "var name " << name << " in data set " << set_i << " not found";
    throw domain_error(s.str());
  }

  void check_variable(const string && set_name, const string & name,
		      const PhysicalQuantity & pq) const
  {
    return check_variable(set_index(set_name), name, pq);
  }

  tuple<bool, Array<double>, const Unit*>
  search_variable(size_t seti, const string & name) const
  {
    const auto & vset = var_sets[seti];
    for (auto it = get_zip_it(vset.var_names, vset.var_units); it.has_curr();
	 it.next())
      {
	auto t = it.get_curr();
	const string & vname = get<0>(t);
	if (vname != name)
	  continue;

	auto vals = vset.values(name);
	if (vals.is_empty())
	  return make_tuple(true, vals, &Unit::null_unit);
	else
	  return make_tuple(true, move(vals), get<1>(t));
      }

    return make_tuple(true, Array<double>(), &Unit::null_unit);
  }

  tuple<bool, Array<double>, const Unit*>
  search_variable(const string & set_name, const string & name) const
  {
    return search_variable(set_index(set_name), name);
  }

  // return true if this data set contains the names stored in `names
  // as constants or variables
  bool contains_names(size_t set_idx, const DynList<string> & names) const
  {
    const auto & vset = var_sets[set_idx];
    return names.all([this, &vset] (const auto & name)
      {
	return
	  const_names.exists([&name] (const auto & s) { return s == name; }) or
	  vset.var_names.exists([&name] (const auto & s) { return s == name; });
      });
  }

  bool is_defined_as_constant(const string & name) const
  {
    return const_names.exists([&name] (const auto & s) { return s == name; });
  }

  /// Return true is name belongs to the constant set of to var name
  /// of varriabe set set_idex
  bool contains_name(size_t set_idx, const string & name) const
  {
    const auto & vset = var_sets[set_idx];
    return const_names.exists([&name] (const auto & s) { return s == name; }) or
      vset.var_names.exists([&name] (const auto & s) { return s == name; });
  }

  /// Return true if name is define as constant or variable name
  bool contains_name(const string & name) const
  {
    return const_names.exists([&name] (const auto & s) { return s == name; }) or
      var_sets.exists([&name] (const auto & vset)
		      { return vset.var_names.exists([&name] (const auto & s)
						     { return s == name; }); });
  }

  // return a list of stored values for the symbol name of the vars set set_idx
  Array<double> values(size_t set_idx, const string & name) const
  {
    for (size_t i = 0; i < const_names.size(); ++i)
      if (const_names(i) == name)
	{
	  DynList<double> ret;
	  ret.append(const_vals(i));
	  return ret;
	}

    const auto & var_set = var_sets[set_idx];
    const auto & var_names = var_set.var_names;
    for (size_t i = 0; i < var_names.size(); ++i)
      if (var_names(i) == name)
	{
	  Array<double> ret(var_set.num_samples);
	  for (size_t j = 0; j < var_set.num_samples; ++j)
	    ret.append(var_set.samples(j)(i));
	  return ret;
	}

    return Array<double>();
  }

  // return a list of stored values for the symbol name in the vars
  // sets contained in sets
  Array<double> values(const DynList<size_t> & sets, const string & name) const
  {
    Array<double> ret;
    sets.for_each([&] (size_t i)
      {
	auto a = this->values(i, name);
	a.for_each([&ret] (auto v) { ret.append(v); });
      });
    return ret;
  }

  // return a list of stored values for the symbol name of the vars set set_idx
  pair<Array<double>, const Unit *>
  quantities(size_t set_idx, const string & name) const
  {
    for (size_t i = 0; i < const_names.size(); ++i)
      if (const_names(i) == name)
	{
	  Array<double> ret;
	  ret.append(const_vals(i));
	  return make_pair(ret, const_units(i));
	}

    const auto & var_set = var_sets[set_idx];
    const auto & var_names = var_set.var_names;
    for (size_t i = 0; i < var_names.size(); ++i)
      if (var_names(i) == name)
	{
	  Array<double> ret(var_set.num_samples);
	  for (size_t j = 0; j < var_set.num_samples; ++j)
	    ret.append(var_set.samples(j)(i));
	  return make_pair(ret, var_set.var_units(i));
	}

    return make_pair(Array<double>(), nullptr);
  }

  pair<Array<double>, const Unit *>
  quantities(const string & set_name, const string & var_name) const
  {
    return quantities(set_index(set_name), var_name);
  }

  Array<double> values(const string & set_name, const string & name) const
  {
    return values(set_index(set_name), name);
  }

  string to_string() const
  {
    ostringstream s;
    s << "Empirical data set: " << name << endl
      << desc << endl
      << endl
      << "  Constant list:" << endl;

    if (const_names.is_empty())
      s << "    Empty" << endl;
    else
      {
	DynList<DynList<string>> const_list;
	enum_for_each(tzip(const_names, const_vals, const_units),
		      [&const_list] (auto t, size_t i)
         {
	   DynList<string> row = { get<0>(t), std::to_string(get<1>(t)),
				   get<2>(t)->symbol, std::to_string(i + 1)};
	   const_list.append(row);
	 });
	auto str = Aleph::to_string(format_string(const_list));
	s << shift_lines_to_left(str, 4) << endl
	  << endl;
      }

    for (auto it = var_sets.get_it(); it.has_curr(); it.next())
      {
	const auto vset = it.get_curr();

	s << "  Variable set " << vset.name << ": " << vset.desc << endl;

	DynList<DynList<string>> var_list;
	enum_for_each(zip(vset.var_names, vset.var_units),
		      [&var_list] (auto p, auto i)
          {
	    DynList<string> row = { p.first, p.second->symbol,
				    std::to_string(i + 1)};
	    var_list.append(row);
	  });
	auto str = Aleph::to_string(format_string(var_list));
	s << shift_lines_to_left(str, 4) << endl
	  << endl;
      }

    return s.str();
  }

  string full_desc() const
  {
    ostringstream s;
    s << to_string() << endl;

    for (size_t i = 0; i < var_sets.size(); ++i)
      {
	const auto & vset = var_sets(i);

	s << "Samples set " << i + 1 << " (" << vset.num_samples << "):"
	  << "  " << vset.name << endl
	  << "  " << vset.desc << endl;

	DynList<DynList<string>> str_samples =
	  vset.samples.maps<DynList<string>>([] (const auto & l)
            {
	      return
	      l.template maps<string>([] (auto v) { return std::to_string(v); });
	    });

	str_samples.insert(to_dynlist(vset.var_names));
	
	str_samples = format_string(str_samples);

	s << shift_lines_to_left(Aleph::to_string(str_samples), 4)
	  << endl;
      }

    return s.str();
  }

  void def_const(const string & name, double val, const Unit * unit)
  {
    if (contains_name(name))
      {
	ostringstream s;
	s << "EmpiricalData::def_const(): name " << name
	  << " is already defined";
	throw domain_error(s.str());
      }

    const_names.append(name);
    const_vals.append(val);
    const_units.append(unit);
    const_table.insert(name, make_pair(val, unit));
    num_const++;
  }

private:

  static const Unit * search_unit(const string & name)
  {
    auto uptr = Unit::search_by_name(name);
    if (uptr == nullptr)
      uptr = Unit::search_by_symbol(name);

    if (uptr == nullptr)
      {
	ostringstream s;
	s << "search_unit(" << name << "): unit not found";
	throw domain_error(s.str());
      }

    return uptr;
  }

public:

  void def_const(const string & name, double val, const string & symbol)
  {
    def_const(name, val, search_unit(symbol));
  }

  VarSet & def_var_set(const string & name, const string & desc)
  {
    VarSet varset = { name, desc };
    return var_sets.append(move(varset));
  }

  VarSet & copy_var_set(size_t seti, const string & name, const string & desc)
  {
    auto & varset = def_var_set(name, desc);
    varset = var_sets[seti];
    varset.name = name;
    varset.desc = desc;
    return varset;
  }    

  VarSet & def_var(size_t set_idx, const string & name, const Unit * unit)
  {
    auto & vset = var_sets[set_idx];
    if (vset.contains_name(name))
      {
	ostringstream s;
	s << "EmpiricalData::def_var(): name " << name
	  << " is already defined in the set " << vset.name;
	throw domain_error(s.str());
      }
    
    vset.var_names.append(name);
    vset.var_units.append(unit);
    vset.num_var++;

    return vset;
  }

  size_t set_index(const string & name) const noexcept
  {
    auto i =
      var_sets.find_index([&name] (const auto & s) { return s.name == name; });
    if (i < var_sets.size())
      return i;

    ostringstream s;
    s << "set name " << name << " not found";
    throw domain_error(s.str());
  }  

  VarSet & def_var(const string & set_name, const string & name,
		   const string & symbol)
  {
    return def_var(set_index(set_name), name, search_unit(symbol));
  }

  VarSet & def_var(const string & set_name, const string & name,
		   const string & symbol, const DynList<double> & col)
  {
    auto & varset = def_var(set_name, name, symbol);

    for (auto it = get_enum_zip_it(col); it.has_curr(); it.next())
      {
	auto t = it.get_curr();
	auto & row = varset.samples(get<1>(t));
	row.append(get<0>(t));
	row.for_each([] (auto v) { cout << " " << v; });
	cout << endl;
      }
    
    // TODO: hay que recorrer todas las filas y añadir la nueva
    // columna junto con la copia

    return varset;
  }

  void add_sample(size_t set_idx, DynList<double> && sample)
  {
    var_sets[set_idx].samples.append(move(sample));
  }

  void add_sample(const string & set_name, DynList<double> && sample)
  {
    add_sample(set_index(set_name), move(sample));
  }

  void add_samples(const string & set_name, istream & input)
  {
    auto & vset = var_sets[set_index(set_name)];
    if (not vset.var_names.is_empty())
      {
	const string s = "EmpiricalData::add_samples(istream&): "
	  "the data has already variables";
	throw domain_error(s);
      }	

    auto header = csv_read_row(input, ',');
    if (header.size() == 0)
      {
	ostringstream s;
	s << "EmpiricalData::add_samples(istream&): "
	  << "input stream header for data set name " 
	  << set_name << " apparently is empty";
	throw domain_error(s.str());
      }
    size_t i = 1;
    for (auto it = header.get_it(); it.has_curr(); it.next(), ++i)
      {
	const auto & s = it.get_curr();
	auto tokens = split_to_list(s, " ");
	string name, symbol;
	try
	  {
	    name = tokens.remove();
	    symbol = tokens.remove();
	    if (not tokens.is_empty())
	      {
		ostringstream ss;
		ss << "EmpiricalData::add_samples(istream&): header token "
		   << s << " has more of two tokens";
		throw domain_error(ss.str());
	      }
	  }
	catch (exception & e)
	  {
	    ostringstream ss;
	    ss << "EmpiricalData::add_samples(istream&): column number " << i
	       << " is incomplete";
	    throw domain_error(ss.str());
	  }

	auto unit_ptr = search_unit(symbol);

	vset.var_names.append(move(name));
	vset.var_units.append(unit_ptr);
	vset.num_var++;
      }

    for (size_t linenum = 2; input.good(); ++vset.num_samples, linenum++)
      {
	auto row = csv_read_row(input, ',');
	if (row.size() != vset.num_var)
	  {
	    ostringstream s;
	    s << "EmpiricalData::add_samples(istream&): row number "
	      << linenum << " has << " << row.size()
	      << " cols, which is not exactly the number of vars "
	      << vset.num_var;
	    throw domain_error(s.str());
	  }
	DynList<double> sample;
	enum_for_each(zip(row, vset.var_units),
		      [&sample, linenum] (auto & p, auto i)
	  {
	    if (not is_double(p.first))
	      {
		ostringstream s;
		s << "EmpiricalData::add_samples(istream&): in row " << linenum
		  << " column " << i + 1 << ": " << p.first
		  << " is not a number";
		throw domain_error(s.str());
	      }
	    try
	      {
		VtlQuantity v(*p.second, atof(p.first.c_str()));
		sample.append(v.raw());
	      }
	    catch (exception & e)
	      {
		ostringstream s;
		s << "EmpiricalData::add_samples(istream&): in row " << linenum
		  << " column " << i + 1 << ": " << e.what();
		throw domain_error(s.str());
	      }
	  });

	vset.samples.append(sample);
      }
    if (vset.samples.is_empty())
      {
	ostringstream s;
	s << "EmpiricalData::add_samples(istream&): input stream apparently "
	  << "does not contain samples";
	throw domain_error(s.str());
      }
  }

  //               var-name value
  using Par = pair<string, double>;

  void add_sample(size_t set_idx, DynList<Par> && pars)
  {
    auto vset = var_sets[set_idx];
    DynList<double> sample;
    for (auto it = vset.var_names.get_it(); it.has_curr(); it.next())
      {
	auto name = it.get_curr();
	sample.append(pars.remove([&name] (const auto & p)
				  { return p.first == name; }).second);
      }

    if (not pars.is_empty())
      {
	ostringstream s;	
	s << "EmpiricalData::add_sample(DynList<Par> && pars): pars size "
	  << "is greater than number of defined variables";
	throw domain_error(s.str());
      }

    vset.samples.append(sample);
  }

  void add_sample(const string & set_name, DynList<Par> && pars)
  {
    add_sample(set_index(set_name), move(pars));
  }

  bool exists_const_name(const string & name) const
  {
    return const_names.exists([&name] (const auto & s) { return name == s; });
  }

  bool exists_var_name(size_t set_idx, const string & name) const noexcept
  {
    return var_sets[set_idx].var_names.exists([&name] (const string & s)
					      { return s == name; });
  }

  bool exists_var_name(const string & set_name, const string & name)
    const noexcept
  {
    return exists_var_name(set_index(set_name), name);
  }

  void add_sample(const string & set_name, DynList<string> && pars)
  {
    auto set_idx = set_index(set_name);
    auto & vset = var_sets[set_idx];
    if (pars.size() != vset.num_var)
      {
	ostringstream s;
	s << "EmpiricalData::add_sample(DynList<string> &&): pars size "
	  << pars.size() << " is different than number of vars " << vset.num_var;
	throw domain_error(s.str());
      }

    DynList<Par> sample;
    size_t n = 0;
    for (; not pars.is_empty(); ++n)
      {
	auto tokens = split_to_list(pars.remove_first(), "=");
	string name = remove_spaces(tokens.remove_first());
	if (not exists_var_name(set_idx, name))
	  {
	    ostringstream s;
	    s << "EmpiricalData::add_sample(DynList<string>&): var name "
	      << name << " at pos " << n + 1 << " is not defined";
	    throw domain_error(s.str());
	  }

	string val = tokens.remove_first();
	if (not is_double(val))
	  {
	    ostringstream s;
	    s << "EmpiricalData::add_sample(DynList<string>&): var value "
	      << val << " at pos " << n + 1 << " is not a number";
	    throw domain_error(s.str());
	  }
	sample.append(make_pair(move(name), atof(val.c_str())));
      }

    if (n < vset.num_var or not pars.is_empty())
      {
	ostringstream s;
	s << "EmpiricalData::add_sample(DynList<string> &&): pars size "
	  << pars.size() << " is different than number of vars "
	  << vset.num_var;
	throw domain_error(s.str());
      }

    add_sample(set_idx, move(sample));
  }

  /// Given the correlation `ptr`, extract the samples matrix and
  /// correlated var from the data set `seti`. return a tuple whose
  /// first item is the matrix of samples and second item is
  /// therrelated value
  tuple<DynList<DynList<double>>, DynList<double>>
  get_set_by_correlation(size_t seti, const Correlation * ptr,
			 const string & name) const
  {
    return var_sets[seti].get_set_by_correlation(ptr, name);
  }

  tuple<DynList<DynList<double>>, DynList<double>>
  get_set_by_correlation(const string & set_name, const Correlation * ptr,
			 const string & name) const
  {
    return get_set_by_correlation(set_index(set_name), ptr, name);
  }

  // returns true if the correlation can be computed through the constants values
  bool can_be_applied(const Correlation * correlation_ptr) const
  {
    return correlation_ptr->names_and_synonyms().exists
      ([this] (const auto & signature)
      {
	return signature.all([this] (const auto & par_name)
          {
	    return this->is_defined_as_constant(par_name);
	  });
      });
  }

  /// return true if correlation correlation_ptr could be applied to
  /// the empirical data including contants and the variable set `seti`
  bool can_be_applied(size_t seti, const Correlation * correlation_ptr) const
  {
    return
      correlation_ptr->names_and_synonyms().exists([this, seti]
						   (const auto & sign)
      {
	return this->contains_names(seti, sign);
      });
  }

  bool can_be_applied(const string &set_name,
		      const Correlation * correlation_ptr) const
  {
    return can_be_applied(set_index(set_name), correlation_ptr);
  }

  /// Return a list of missing parameters needed for computing the
  /// correlation `correlation_ptr` according to the constants stored
  /// in all he data set and variables stored in the variables set `seti`
  DynList<string>
  missing_parameters(size_t seti,  const Correlation * correlation_ptr) const
  {
    DynList<string> ret;
    for (auto it = correlation_ptr->get_preconditions().get_it();
	 it.has_curr(); it.next())
      {
	const auto & par = it.get_curr();
	if (not (par.names().exists([this, seti] (const auto & s)
                     {
		       return this->exists_const_name(s.first) or
			 this->exists_var_name(seti, s.first);
		     })))
	  ret.append(par.name);
      }
    return ret;
  }

  // Return a list of list of correlation parameters belonging to the
  // data set that would be used in order to compute the all the value
  // of the correlation
  DynList<DynList<double>>
  correlation_values(size_t seti, const Correlation * correlation_ptr) const
  {
    DynList<DynList<double>> ret; // this is the return value

    // traverse all correlation parameter and get from the data set a
    // column of data
    for (auto it = correlation_ptr->get_preconditions().get_it(); it.has_curr();
	 it.next())
      {
	auto par = it.get_curr();
	DynList<double> vals = to_DynList(values(seti, par.name));
	if (not vals.is_empty())
	  {
	    ret.append(vals);
	    continue;
	  }

	// if vals is empty, the it could exist a synonym in the
	// correlation that matches with par_name. So we traverse the
	// par_name synonyms
	for (auto it_syn = par.get_synonyms().get_it(); it_syn.has_curr();
	     it.next())
	{
	  auto syn = it_syn.get_curr();
	  const auto & syn_name = syn.first;
	  vals = to_DynList(values(seti, syn_name)); // verify if
						     // there are
						     // values in data
						     // set
	  if (not vals.is_empty())
	    { // a synomym was found for par_name in the
	      // correlation. Now we convert the values to the synonym unit
	      const Unit * syn_unit = syn.second;
	      vals.mutable_for_each([syn_unit, &par] (auto & v)
				    {
				      VtlQuantity vsyn(*syn_unit, v);
				      v = VtlQuantity(par.unit, vsyn).raw();
				    });
	      break;
	    }
	}
	if (vals.is_empty())
	  { // here definitely there is no data for the parameter
	    // inside the data set
	    ostringstream s;
	    s << "EmpiricalData::correlation_values(): parameter " << par.name
	      << " of correlation " << correlation_ptr->name
	      << " does not match with any name of data set";
	    throw domain_error(s.str());
	  }

	ret.append(vals);
      }

    return ret;
  }

  // Return a list of permutations inputs for `correlation_ptr` built
  // according to column index `col`
  DynList<DynList<double>>
  correlation_perms(size_t seti, const Correlation * correlation_ptr,
		    const string & par_name) const
  {
    auto pos = correlation_ptr->parameter_pos(par_name);
    if (pos >= correlation_ptr->get_num_pars())
      {
	ostringstream s;
	s << "EmpiricalData::correlation_perms(): parameter name " << par_name
	  << " not found";
	throw domain_error(s.str());
      }

    const size_t & num_rows = var_sets[seti].samples.size();
    return transpose(correlation_values(seti, correlation_ptr).
      maps<DynList<double>>([&] (const auto & l)
        {
	  return l.is_unitarian() ? rep<double>(num_rows, l.get_first()) : l;
	}));
  }

  /// Returns true if the parameter contained in data set number
  /// `seti` fits the restrictions of correlation `correlation_ptr`
  bool fits_parameter_ranges(size_t seti,
			     const Correlation * correlation_ptr) const
  {
    auto vals = correlation_values(seti, correlation_ptr);
    vals.mutable_for_each([] (auto & l) { in_place_sort(l); });

    for (auto it = get_pair_it(vals, correlation_ptr->get_preconditions());
	 it.has_curr(); it.next())
      {
	auto p = it.get_curr();
	auto empirical_samples = p.first;
	const auto & par = p.second;
	double min_emp_sample = empirical_samples.get_first();
	double max_emp_sample = empirical_samples.get_last();
	if (not par.check(min_emp_sample) or not par.check(max_emp_sample))
	  return false;
      }

    return true;
  }

  bool fits_parameter_ranges(const string & set_name,
			     const Correlation * correlation_ptr) const
  {
    return fits_parameter_ranges(set_index(set_name), correlation_ptr);
  }

  using InvalidRange = pair<string, DynList<double>>;

  /// given `correlaion_ptr` and the var set `seti`, it returns a list
  /// of names and their values which are not inside in te correlation range
  DynList<InvalidRange>
  invalid_parameters_values(size_t seti,
			    const Correlation * correlation_ptr) const
  {
    auto vals = correlation_values(seti, correlation_ptr);
    vals.mutable_for_each([] (auto & l) { in_place_sort(l); });
    DynList<InvalidRange> ret;

    for (auto it = get_pair_it(vals, correlation_ptr->get_preconditions());
	 it.has_curr(); it.next())
      {
	auto p = it.get_curr();
	auto empirical_samples = p.first;
	const auto & par = p.second;
	double min_emp_sample = empirical_samples.get_first();
	if (empirical_samples.is_unitarian())
	  {
	    if (not par.check(min_emp_sample))
	      ret.append(make_pair(par.name,
				   DynList<double>({ min_emp_sample })));
	    continue;
	  }

	DynList<double> invalid_vals;
	while (not empirical_samples.is_empty() and
	       not par.check(empirical_samples.get_first()))
	  invalid_vals.append(empirical_samples.remove_first());

	if (empirical_samples.is_empty())
	  {
	    if (not invalid_vals.is_empty())
	      ret.append(make_pair(par.name, invalid_vals));
	    continue;
	  }

	empirical_samples.reverse();
	while (not empirical_samples.is_empty() and
	       not par.check(empirical_samples.get_first()))
	  invalid_vals.append(empirical_samples.remove_first());

	if (not invalid_vals.is_empty())
	  ret.append(make_pair(par.name, invalid_vals));
      }

    return ret;
  }

  DynList<InvalidRange>
  invalid_parameters_values(const string & set_name,
			    const Correlation * correlation_ptr) const
  {
    return invalid_parameters_values(set_index(set_name), correlation_ptr);
  }

  //   found, real name, value, unit
  tuple<bool, string, double, const Unit*>
  search_const_name(const string & name) const
  {
    auto result = t_zip(const_names, const_vals, const_units).
      find_item([&name] (const auto & t) { return get<0>(t) == name; });
    if (get<0>(result))
      return tuple_cat(make_tuple(true), get<1>(result));

    return tuple_cat(make_tuple(false), get<1>(result));
  }

  /// Returns all the correlations that compute `const_name` and
  /// matching with other constant parameters 
  DynList<const Correlation*>
  matching_const_correlations(const string & const_name) const
  {
    auto target_correlations =
      Correlation::array().filter([&const_name] (auto ptr)
      {
	return ptr->target_name() == const_name;
      });

    auto ret = target_correlations.filter([&] (auto ptr)
      {
	return ptr->get_preconditions().all([&](const auto & par)
          {
	    return par.names().exists([&](const auto & p)
              {
		return const_names.exists([&] (const auto &s)
                  {
		    return s == p.first;
		  });
	      });
	  });
      });

    return ret;
  }

  /// Return all the correlations that compute `const_name`, match
  /// with other constant parameters and their parameters ranges are
  /// valid respect to the constant stored in this data set
  DynList<const Correlation*>
  matching_const_valid_correlations(const string & const_name) const
  {
    return matching_const_correlations(const_name).
      filter([this, &const_name] (auto ptr)
      {
	return ptr->get_preconditions().all([this] (const auto & par)
	  {
	    return par.names().exists([this, &par] (auto p)
              {
		auto t = this->search_const_name(p.first);
		if (not get<0>(t))
		  return false;
		
		const double val = get<2>(t);
		const Unit * unit_ptr = get<3>(t);
		try
		  { // exception handler because val could be out of
		    // unit bounds
		    return par.check(VtlQuantity(*unit_ptr, val));
		  }
		catch (...)
		  {
		    return false; // this precondition is not matched
		  }
	      });
	  });
      });
  }

  /// Returns a list of correlation matching the names contained in
  /// the data set
  DynList<const Correlation*> matching_correlations(size_t seti) const
  {
    return Correlation::array().filter([this, seti] (auto ptr)
      {
	return this->can_be_applied(seti, ptr);
      });
  }

  DynList<const Correlation*>
  matching_correlations(const string & set_name) const
  {
    return matching_correlations(set_index(set_name));
  }

  DynList<const Correlation*> valid_correlations(size_t seti) const
  {
    return Correlation::array().filter([this, seti] (auto ptr)
      {
	return this->can_be_applied(seti, ptr) and
	  this->fits_parameter_ranges(seti, ptr);
      });
  }

  DynList<const Correlation*>
  valid_correlations(const string & set_name) const
  {
    return valid_correlations(set_index(set_name));
  }

  /// Computes `correlation_ptr` from the stored constant values 
 double compute(const Correlation * correlation_ptr, bool check = true) const
  {
    auto const_list = t_zip(const_names, const_vals, const_units);
    auto pars = correlation_ptr->get_preconditions().
      maps<VtlQuantity>([&] (const auto & par)
        {
	  auto ptr = const_list.find_ptr([&] (auto t)
            { return par.names().exists
	      ([&] (const auto & p) { return get<0>(p) == get<0>(t); });
	    });
	  
	  if (ptr == nullptr)
	    {
	      ostringstream s;
	      s << "Parameter " << par.name << " of correlation "
		<< correlation_ptr->name << " was not found in data set";
	      throw domain_error(s.str());
	    }
	  return VtlQuantity(*get<2>(*ptr), get<1>(*ptr));
	});
    return correlation_ptr->compute(pars, check).raw();
  }

  /// Aplies `correlation_ptr` to the var set seti restricted to the
  /// values of samples[col] are greater or equal than min_val and
  /// lesser than max_val
  DynList<DynList<Correlation::ParByName>>
  generate_samples_by_name(const Correlation * correlation_ptr,
			   size_t seti,
			   size_t col,
			   double min_val, double max_val) const
  {
    if (not (max_val >= min_val))
      {
	ostringstream s;
	s << "EmpiricalData::compute(" << correlation_ptr->name << ", "
	  << seti << ", " << col << ", " << min_val << ", " << max_val
	  << "): min_val is greater than max_val";
	throw domain_error(s.str());
      }

    const auto & var_set = var_sets[seti];

    if (col >= var_set.var_names.size())
      {
	ostringstream s;
	s << "EmpiricalData::compute(" << correlation_ptr->name << ", "
	  << seti << ", " << col << ", " << min_val << ", " << max_val
	  << "): column " << col << " out of range [0, "
	  << var_set.var_names.size() - 1 << "]";
	throw out_of_range(s.str());
      }

    const size_t n = correlation_ptr->get_num_pars();

	//        par-name  par-value par-unit
    Array<tuple<string, double, const Unit*>> pars(n);
    pars.putn(n);

    const string & col_name = var_set.var_names[col];
    bool col_name_seen = false;

    size_t num_seen = 0;
    size_t i = 0;
    // Set the constant values
    correlation_ptr->get_preconditions().for_each([&] (const auto & par)
      {
	auto & t = pars(i);
	get<0>(t) = par.name;
	get<2>(t) = &Unit::null_unit;

	auto names = par.names();
	for (auto it = names.get_it(); it.has_curr(); it.next())
	  {
	    auto alias = it.get_curr();
	    auto p = const_table.search(alias.first);
	    if (p == nullptr)
	      continue; // not found alias ==> look at next alias

	    if (alias.first == col_name)
	      col_name_seen = true;

	    VtlQuantity val = { *p->second.second, p->second.first };
	    VtlQuantity result = { par.unit, val };
	    get<1>(t) = result.raw();
	    get<2>(t) = &par.unit;
	    ++num_seen;
	    break;
	  }
	
	++i; // next index of correlation parameter
      });

    const bool col_is_constant = col_name_seen;

    //          index in par, index in samples col
    DynList<pair<size_t, size_t>> cols;

    // missing parameters indexes are the null_unit entries of
    // pars. These are set to the var_set
    for (i = 0; i < n and num_seen < n; ++i)
      {
	auto & t = pars(i);
	if (get<2>(t) != &Unit::null_unit)
	  continue;

	const auto & name = get<0>(t); // correlation parameter name
	const auto par_ptr = correlation_ptr->search_par_name(name);
	size_t col_idx = var_set.var_names.find_index([&] (const auto & vname)
          {
	    return par_ptr->names().exists([&] (const auto & p)
	      { return p.first == vname; });
	  });

	if (col_idx >= var_set.var_names.size())
	  continue; // this col name is not inside the correlation

	if (not col_is_constant and col_idx == col)
	  col_name_seen = true;

	// col name found. col_idx contains its column index
	get<2>(t) = &par_ptr->unit;
	++num_seen;
	cols.append(make_pair(i, col_idx));
      }

    const bool col_is_var = col_name_seen and not col_is_constant;

    if (num_seen < n)
      {
	ostringstream s;
	s << "EmpiricalData::compute(" << correlation_ptr->name << ", "
	  << seti << ") parameters";
	pars.filter([] (auto t) { return get<2>(t); }).
	  for_each([&s] (auto t) { s << " " << get<0>(t); });
	s << " not found in the data set";
	throw domain_error(s.str());
      }

    DynList<DynList<Correlation::ParByName>> ret;
    for (i = 0; i <= var_set.samples.size(); ++i)
      {
	const auto & row = var_set.samples(i);

	if (not (row(col) >= min_val and row(col) <= max_val))
	  continue;

	cols.for_each([&row, &pars, &var_set] (auto col)
          {
	    VtlQuantity q = { *var_set.var_units(col.second), row(col.second) };
	    VtlQuantity val = { *get<2>(pars(col.first)), q };
	    get<1>(pars(col.first)) = val.raw();
	  });

	auto pars_list = pars.maps<Correlation::ParByName>([] (auto t)
	  {
	    return make_pair(get<0>(t), get<1>(t));
	  });
	
	if (not col_is_var)
	  pars_list.insert(make_pair(col_name, row(col)));

	ret.append(move(pars_list));	  
      }

    return ret;
  }

  DynList<DynList<Correlation::ParByName>>
  generate_samples_by_name(const Correlation * correlation_ptr,
			   size_t seti,
			   const string & col_name,
			   double min_val, double max_val) const
  {
    auto var_set = var_sets[seti];
    auto col = var_set.var_names.find_index([&col_name] (const auto & s)
					    { return s == col_name; });
    return
      generate_samples_by_name(correlation_ptr, seti, col, min_val, max_val);
  }

  DynList<pair<const Correlation *, DynList<DynList<Correlation::ParByName>>>>
  generate_samples_by_name(const DefinedCorrelation & correlation,
			   const string & var_name, 
			   const DynList<size_t> & sets) const
  {
    // primero determinar los rangos de var_name según lo contenido en
    // sets
    //                 start    end       seti    col
    using SRange = tuple<double, double, size_t, size_t>;
    auto sets_ranges = sets.maps<SRange>([&var_name, this] (size_t i)
      {
	const auto & var_set = var_sets[i];
	auto col = var_set.name_index(var_name);

	if (col >= var_set.var_names.size())
	  {
	    ostringstream s;
	    s << "Variable name " << name << " was not found in var set " << i;
	    throw domain_error(s.str());
	  }
	// TODO: verificar que coluna esté ordenada
	return make_tuple(var_set.samples.get_first()(col),
			  var_set.samples.get_last()(col), i, col);
      });

    cout << "Set ranges:" << endl;
    sets_ranges.for_each([] (auto p) { cout << "[" << get<0>(p) << ", "
					    << get<1>(p) << "] "; });
    cout << endl;

    // Luego según los rangos determinar las correlaciones
    using CRange = tuple<double, double, const Correlation*>;
    auto corr_ranges =
      correlation.interval_list().maps<CRange>([] (const auto &i)
      {
	return make_tuple(i.start, i.end, i.correlation_ptr);
      });

    cout << "Corr ranges:" << endl;
    corr_ranges.for_each([] (auto p) { cout << "[" << get<0>(p) << ", "
					    << get<1>(p) << "] "; });
    cout << endl;

    if (not (get<0>(sets_ranges.get_first()) >= get<0>(corr_ranges.get_first())
	     and
	     get<1>(sets_ranges.get_last()) <= get<1>(corr_ranges.get_last())))
      {
	ostringstream s;
	s << "Empiricaldata::generate_samples_by_name(): data given in some "
	  " variable set is outside of correlation range";
	throw domain_error(s.str());
      }

    // Ahora debo hacer una especie de merge de los intervalos tal que
    // el resultado tenga la mínima cantidad de intervalos

    DynList<pair<const Correlation*,
		 DynList<DynList<Correlation::ParByName>>>> ret;
    DynList<DynList<Correlation::ParByName>> curr_pars;

    auto sets_it = sets_ranges.get_it(); 
    auto corr_it = corr_ranges.get_it();
    while (sets_it.has_curr() and corr_it.has_curr())
      {
	auto set_range = sets_it.get_curr();
	auto corr_range = corr_it.get_curr();

	auto set_min = get<0>(set_range);
	auto set_max = get<1>(set_range);
	auto corr_min = get<0>(corr_range);
	auto corr_max = get<1>(corr_range);

	// Verify that the intervals intersect
	if (set_max < corr_min or corr_max < set_min)
	  {
	    ostringstream s;
	    s << "Empiricaldata::generate_samples_by_name(): range of var set ["
	      << set_min << ", " << set_max
	      << "] does not intersect correlation range ["
	      << corr_min << ", " << corr_max << "]";
	    throw domain_error(s.str());
	  }

	const auto & seti = get<2>(set_range);
	const auto & col = get<3>(set_range);

	if (set_min >= corr_min and set_max == corr_max) // equal intervals
	  { // compute all values inside set, afterward advance to next 
	    curr_pars.append(generate_samples_by_name(get<2>(corr_range),
						      seti, col,
						      set_min, set_max));
	    ret.append(make_pair(get<2>(corr_range), curr_pars));
	    curr_pars.empty();
	    corr_it.next();
	    sets_it.next();
	    continue;
	  }

	if (set_min >= corr_min and set_max <= corr_max)
	  { 
	    curr_pars.append(generate_samples_by_name(get<2>(corr_range), seti,
						      col, set_min, set_max));
	    sets_it.next();
	    continue;
	  }

	if (set_min < corr_min)
	  {
	    ostringstream s;
	    s << "Empiricaldata::generate_samples_by_name(): values inside ["
	      << set_min << ", " << corr_min
	      << ") does not have any correlation";
	    throw domain_error(s.str());
	  }

	curr_pars.append(generate_samples_by_name(get<2>(corr_range), seti, col,
						  set_min, corr_max));
	ret.append(make_pair(get<2>(corr_range), curr_pars));
	curr_pars.empty();
	get<1>(set_range) = nextafter(corr_max, set_max);
	corr_it.next();
      }

    return ret;
  }

  DynList<DynList<Correlation::ParByName>>
  samples_by_name(const DefinedCorrelation & correlation,
		  const string & var_name, 
		  const DynList<size_t> & sets) const
  {
    auto samples = generate_samples_by_name(correlation, var_name, sets);
    DynList<DynList<Correlation::ParByName>> ret;
    samples.for_each([&ret] (auto p) { ret.append(p.second); });
    return ret;
  }

  // DynList<double> compute(const DefinedCorrelation & correlation,
  // 			   const string & var_name, 
  // 			   DynList<size_t> sets) const
  // {
  //   return generate_samples_by_name(correlation, var_name, sets).
  //     maps<double>([
			    
			  

  /// returns a column of correlation outputs for the values stored in
  /// this data set
  DynList<double> compute(size_t seti, const string & row_name,
			  const Correlation * correlation_ptr,
			  bool check = true) const
  {
    return
      correlation_perms(seti, correlation_ptr, row_name).
      maps<double>([&](const auto & l)
		   {
		     return correlation_ptr->compute(l, check);
		   });
    
    auto vals = correlation_values(seti, correlation_ptr);

    DynList<double> ret;

    for_each_perm(vals, [&ret, correlation_ptr, check] (const auto & pars)
      {
	ret.append(correlation_ptr->compute(pars, check));		       
      });
    
    return ret;
  }

  DynList<double> tuned_compute(size_t seti, const string & row_name,
				const Correlation * correlation_ptr,
				double c, double m,
				bool check = true) const
  {
    return
      correlation_perms(seti, correlation_ptr, row_name).
      maps<double>([&](const auto & l)
		   {
		     return correlation_ptr->tuned_compute(l, c, m, check);
		   });

    auto vals = correlation_values(seti, correlation_ptr);

    DynList<double> ret;

    for_each_perm(vals, [&ret, correlation_ptr, check, c, m] (const auto & pars)
      {
	ret.append(correlation_ptr->tuned_compute(pars, c, m, check));
      });
    
    return ret;
  }

  DynList<double> compute(const string & set_name, const string & row_name,
			  const Correlation * correlation_ptr,
			  bool check = true) const
  {
    return compute(set_index(set_name), row_name, correlation_ptr, check);
  }

  DynList<double> tuned_compute(const string & set_name,
				const string & row_name,
				const Correlation * correlation_ptr,
				double c, double m,
				bool check = true) const
  {
    return
      tuned_compute(set_index(set_name), row_name, correlation_ptr, c, m, check);
  }

  /// Builds a parameter list for correlation `corr_ptr` extracting the
  /// values from the var_set with index `seti` and the column number `col`
  DynList<Correlation::NamedPar>
  get_last_row_named_pars(const Correlation * corr_ptr, size_t seti) const
  {
    return corr_ptr->get_preconditions().
      maps<Correlation::NamedPar>([this, seti] (const auto & par)
      {
	auto par_alias = par.names();
	for (auto it = par_alias.get_it(); it.has_curr(); it.next())
	  {
	    auto syn = it.get_curr();
	    const auto & name = syn.first;
	    cout << "Searching " << name << " for " << par.name << endl;
	    try
	      {
		auto values = this->quantities(seti, name);
		if (values.first.is_empty())
		  continue;
		return make_tuple(true, par.name,
				  values.first.get_last(), syn.second);
	    }
	  catch (...) { /* ignore it! it could be in another synonym */ }
	}
      return make_tuple(false, par.name, 0.0, &Unit::null_unit);
    });
  }

  using CorrRow = tuple<DynList<double>, double>;
  using CorrMat =
    tuple<DynList<string>, DynList<DynList<double>>, DynList<double>>;
  
  /// Return a matrix of correlations inputs and its outputs
  CorrMat compute_mat(size_t seti, const Correlation * correlation_ptr,
		      bool check) const
  {
    DynList<string> names = correlation_ptr->get_preconditions().
      maps<string>([] (const auto & par) { return par.name; });
    names.append(correlation_ptr->target_name());

    auto vals = correlation_values(seti, correlation_ptr);

    DynList<DynList<double>> samples;
    DynList<double> y;

    for_each_perm(vals, [&samples, &y, correlation_ptr, check] (auto & row)
      {
	double result = correlation_ptr->compute(row, check);
	samples.append(row);
	y.append(result);
      });
    
    return make_tuple(move(names), move(samples), move(y));
  }

  CorrMat compute_mat(const string & set_name,
		      const Correlation * correlation_ptr,
		      bool check) const
  {
    return compute_mat(set_index(set_name), correlation_ptr, check);
  }

  /// Returns a pair with the names needed for computing correlation
  /// `correlation_ptr`. The first element is a list of names that are
  /// present between the const and the var_set-set_idx]. The second
  /// element is a list of names that are not present
  pair<DynList<string>, DynList<string>>
    matching_names(size_t set_idx, const Correlation * correlation_ptr) const
  {
    const auto & var_set = var_sets[set_idx];
    DynList<string> present, absent;
    for (auto it = correlation_ptr->get_preconditions().get_it(); it.has_curr();
	 it.next())
      {
	const auto & par = it.get_curr();
	const auto & name = par.name;
	auto exist = [&par] (const string & s)
	  {
	    return s == par.name or
	    par.get_synonyms().exists([&s] (const auto & syn)
	      { return s == syn.first; });
	  };
	if (const_names.exists(exist) or var_set.var_names.exists(exist))
	  present.append(name);
	else
	  absent.append(name);
      }

    return make_pair(present, absent);
  }

  /// Returns the set of correlations whose target name is
  /// `name`. That is, those correlations computing `name`
  DynList<const Correlation*>
  correlations(size_t seti, const string & name) const
  {
    const auto & var_set = var_sets[seti];
    if (var_set.contains_name(name))
      {
	ostringstream s;
	s << "var set " << var_set.name << " contains the name " << name;
	throw domain_error(s.str());
      }

    return Correlation::array().filter([&name] (auto ptr)
      { return ptr->target_name() == name; }).
      filter([this, i = seti] (auto ptr)
	     { return this->can_be_applied(i, ptr); });
    }

private:

  static json const_json(const tuple<string, double, const Unit *> & c)
  {
    json j;
    j["name"] = get<0>(c);
    j["value"] = get<1>(c);
    j["unit"] = get<2>(c)->name;

    return j;
  }

public:

  string to_json() const
  {
    json j;
    j["name"] = name;
    j["description"] = desc;
    auto const_list = t_zip(const_names, const_vals, const_units).
      maps<json>([] (const auto & c) { return const_json(c); });

    j["constants"] = to_vector(const_list);
    
    j["varsets"] = to_vector(var_sets.maps<json>([] (const auto & s)
      { return s.to_json(); }));

    return j.dump(2);
  }

  EmpiricalData() {}

  void set_from_json(const string & json_str)
  {
    json j = json::parse(json_str);
    name = j["name"];
    desc = j["description"];

    for (const auto & item : j["constants"])
      {
	const string name = item["name"];
	const double value = item["value"];
	const Unit * unit = search_unit(item["unit"]);
	const_names.append(name);
	const_table.insert(name, make_pair(value, unit));
	const_vals.append(value);
	const_units.append(unit);
      }

    for (const auto & json_set : j["varsets"])
      {
	VarSet vset;
	vset.set_from_json(json_set);
	var_sets.append(move(vset));
      }
  }

  EmpiricalData(const string & json_str)
  {
    set_from_json(json_str);
  }
};


# endif // EMPIRICAL_DATA_H

