
# ifndef EMPIRICAL_DATA_H
# define EMPIRICAL_DATA_H 1

# include <tuple>

# include <parse-csv.H>
# include <ah-string-utils.H>
# include <ah-comb.H>

# include <units/pvt-units.H>
# include <correlations/pvt-correlations.H>

using namespace std;
using namespace Aleph;

struct EmpiricalData
{
  size_t num_const = 0;
  Array<string> const_names;
  Array<double> const_vals;
  Array<const Unit*> const_units;

  struct VarSet
  {
    string name = "NO-NAME";
    string desc = "NO-DESC";

    VarSet() {}
    VarSet(const string & name, const string & desc) : name(name), desc(desc) {}

    size_t num_var = 0;
    Array<string> var_names; // pressure Rs bo uo
    Array<const Unit*> var_units; // of above names
    size_t num_samples = 0;
    Array<Array<double>> samples; // must have the same number of
                                  // columns than var_names

    bool contains_name(const string & name) const noexcept
    {
      return var_names.exists([&name] (const auto & s) { return s == name; });
    }
  };

  Array<VarSet> var_sets;

  DynList<string> names() const
  {
    DynSetTree<string> ret = const_names;
    for (auto it = var_sets.get_it(); it.has_curr(); it.next())
      it.get_curr().var_names.for_each([&ret] (const auto & s) { ret.append(s); });

    return ret.keys();
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

  // return a list of stored values for the symbol name
  DynList<double> values(const string & name) const
  {
    DynList<double> ret;

    for (size_t i = 0; i < const_names.size(); ++i)
      if (const_names(i) == name)
	{
	  ret.append(const_vals(i));
	  return ret;
	}

    for (size_t k = 0; k < var_sets.size(); ++k)
      {
	const auto & var_set = var_sets(k);
	const auto & var_names = var_set.var_names;
	for (size_t i = 0; i < var_names.size(); ++i)
	  if (var_names(i) == name)
	    {
	      const auto & samples = var_set.samples;
	      const auto & num_samples = var_set.num_samples;
	      for (size_t j = 0; j < num_samples; ++j)
		ret.append(samples(i)(j));

	      return ret;
	    }
	}

    return ret;
  }

  // return a list of stored values for the symbol name
  DynList<double> values(size_t set_idx, const string & name) const
  {
    DynList<double> ret;

    for (size_t i = 0; i < const_names.size(); ++i)
      if (const_names(i) == name)
	{
	  ret.append(const_vals(i));
	  return ret;
	}

    const auto & var_set = var_sets[set_idx];
    const auto & var_names = var_set.var_names;
    for (size_t i = 0; i < var_names.size(); ++i)
      if (var_names(i) == name)
	for (size_t j = 0; j < var_set.num_samples; ++j)
	  {
	    ret.append(var_set.samples(i)(j));
	    return ret;
	  }

    return ret;
  }

  string to_string() const
  {
    ostringstream s;
    s << "Empirical data set" << endl
      << endl
      << "  Constant list:" << endl;

    DynList<DynList<string>> const_list;
    enum_for_each(tzip(const_names, const_vals, const_units),
		  [&const_list] (auto t, size_t i)
      {
	DynList<string> row = { get<0>(t), std::to_string(get<1>(t)),
				get<2>(t)->symbol, std::to_string(i + 1)};
	row.for_each([] (auto s) { cout << s << " "; }); cout << endl;
	const_list.append(row);
      });
    auto str = Aleph::to_string(format_string(const_list));
    s << shift_lines_to_left(str, 4) << endl
      << endl;

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
	str = Aleph::to_string(format_string(var_list));
	s << shift_lines_to_left(str, 4) << endl
	  << endl;
      }

    return s.str();
  }

  string full_desc() const
  {
    ostringstream s;
    
    for (size_t i = 0; i < var_sets.size(); ++i)
      {
	const auto & vset = var_sets(i);
	s << to_string() << endl
	  << endl
	  << "Samples set " << i + 1 << " (" << vset.num_samples << "):" << endl;

	DynList<DynList<string>> str_samples =
	  vset.samples.map<DynList<string>>([] (const auto & l)
            {
	      return
	      l.template map<string>([] (auto v) { return std::to_string(v); });
	    });

	str_samples.insert(to_dynlist(vset.var_names));
	
	str_samples = format_string(str_samples);

	s << shift_lines_to_left(Aleph::to_string(str_samples), 4);
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
    num_const++;
  }

private:

  static const Unit * search_unit(const string & symbol)
  {
    auto uptr = Unit::search_by_name(symbol);
    if (uptr == nullptr)
      uptr = Unit::search_by_symbol(symbol);

    if (uptr == nullptr)
      {
	ostringstream s;
	s << "search_unit(" << symbol << "): unit not found";
	throw domain_error(s.str());
      }

    return uptr;
  }

public:

  void def_const(const string & name, double val, const string & symbol)
  {
    def_const(name, val, search_unit(symbol));
  }

  void def_var_set(const string & name, const string & desc)
  {
    var_sets.emplace(name, desc);
  }

  void def_var(size_t set_idx, const string & name, const Unit * unit)
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
  }

private:

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

public:

  void def_var(const string & set_name,  string & name, const string & symbol)
  {
    def_var(set_index(set_name), name, search_unit(symbol));
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
	      << " cols, which is not exactly the number of vars " << vset.num_var;
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
	  << pars.size() << " is different than number of vars " << vset.num_var;
	throw domain_error(s.str());
      }

    add_sample(set_idx, move(sample));
  }

  /// return true if correlation correlation_ptr could be applied to
  /// the empirical data 
  bool can_be_applied(size_t seti, const Correlation * correlation_ptr) const
  {
    return
      correlation_ptr->names_and_synonyms().exists([this, seti] (const auto & sign)
      {
	return this->contains_names(seti, sign);
      });
  }

  bool can_be_applied(const string &set_name,
		      const Correlation * correlation_ptr) const
  {
    return can_be_applied(set_index(set_name), correlation_ptr);
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
	DynList<double> vals = values(seti, par.name); // values of data set
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
	  vals = values(seti, syn_name); // verify if there are values in data set
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
	      ret.append(make_pair(par.name, DynList<double>({ min_emp_sample })));
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
  
  /// Returns a list of correlation matching the names contaned in
  /// the data set
  DynList<const Correlation*> matching_correlations(size_t seti) const
  {
    return Correlation::list().filter([this, seti] (auto ptr)
				      { return this->can_be_applied(seti, ptr); });
  }

  DynList<const Correlation*> matching_correlations(const string & set_name)
    const
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

  DynList<const Correlation*> valid_correlations(const string & set_name) const
  {
    return valid_correlations(set_index(set_name));
  }

  /// returns a column of correlation outputs for the values stored in
  /// this data set
  DynList<double> compute(size_t seti, const Correlation * correlation_ptr,
			  bool check = true) const
  {
    auto vals = correlation_values(seti, correlation_ptr);
    DynList<double> ret;

    for_each_perm(vals, [&ret, correlation_ptr, check] (const auto & pars)
      {
	pars.for_each([] (auto v) { cout << v << " "; }); cout << endl;
	ret.append(correlation_ptr->compute(pars, check));		       
      });
    return ret;
  }

  DynList<double> compute(const string & set_name,
			  const Correlation * correlation_ptr,
			  bool check = true) const
  {
    return compute(set_index(set_name), correlation_ptr, check);
  }

  DynList<DynList<double>> compute_mat(size_t seti,
				       const Correlation * correlation_ptr,
				       bool check) const
  {
    auto vals = correlation_values(seti, correlation_ptr);
    DynList<DynList<double>> ret;

    for_each_perm(vals, [&ret, correlation_ptr, check] (const auto & pars)
      {
	DynList<double> row = pars;
	row.append(correlation_ptr->compute(pars, check));
	ret.append(row); 
      });
    return ret;
  }

  DynList<DynList<double>> compute_mat(const string & set_name,
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

  // TODO: funcion que dado un name que no está en el data set_set
  // determine las correlaciones que calculan su valor a partir de
  // datos que si están en el data set
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
      filter([this, i = seti] (auto ptr) { return this->can_be_applied(i, ptr); });
    }

  // TODO: luego función que calcule cuáles parámetros están ausentes

  // TODO: luego funcione que calcule cuales correlaciones pueden
  // calcular los datos ausentes con los datos del var_set y las constantes
};


# endif // EMPIRICAL_DATA_H
