
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

  size_t num_var = 0;
  Array<string> var_names; // pressure Rs bo uo
  Array<const Unit*> var_units; // of above names

  size_t num_samples = 0;
  Array<Array<double>> samples; // must have the same number of
				// columns than var_names

  DynList<string> names() const
  {
    DynList<string> ret = to_dynlist(const_names);
    ret.append(to_dynlist(var_names));
    return ret;
  }

  // return true if this data set contains the names as constants or variables
  bool contains_names(const DynList<string> & names) const
  {
    return names.all([this] (const auto & name)
      {
	return
	  const_names.exists([&name] (const auto & s) { return s == name; }) or
	  var_names.exists([&name] (const auto & s) { return s == name; });
      });
  }

  bool contains_name(const string & name) const
  {
    return const_names.exists([&name] (const auto & s) { return s == name; }) or
      var_names.exists([&name] (const auto & s) { return s == name; });
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

    for (size_t i = 0; i < var_names.size(); ++i)
      if (var_names(i) == name)
	{
	  for (size_t j = 0; j < num_samples; ++j)
	    ret.append(samples(i)(j));

	  break;
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
      << endl
      << "  Variables list:" << endl;

    DynList<DynList<string>> var_list;
    enum_for_each(zip(var_names, var_units), [&var_list] (auto p, auto i)
      {
    	DynList<string> row = { p.first, p.second->symbol,
				std::to_string(i + 1)};
    	var_list.append(row);
      });
    str = Aleph::to_string(format_string(var_list));
    s << shift_lines_to_left(str, 4) << endl;

    return s.str();
  }

  string full_desc() const
  {
    ostringstream s;
    s << to_string() << endl
      << endl
      << "Samples (" << num_samples << "):" << endl;

    DynList<DynList<string>> str_samples =
      samples.map<DynList<string>>([] (const auto & l)
      {
	return l.template map<string>([] (auto v) { return std::to_string(v); });
      });

    str_samples.insert(to_dynlist(var_names));

    str_samples = format_string(str_samples);

    s << shift_lines_to_left(Aleph::to_string(str_samples), 4);

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

  void def_var(const string & name, const Unit * unit)
  {
    if (contains_name(name))
      {
	ostringstream s;
	s << "EmpiricalData::def_var(): name " << name << " is already defined";
	throw domain_error(s.str());
      }
    var_names.append(name);
    var_units.append(unit);
    num_var++;
  }

  void def_var(const string & name, const string & symbol)
  {
    def_var(name, search_unit(symbol));
  }

  void add_sample(const DynList<double> & sample)
  {
    samples.append(sample);
  }

  void add_samples(istream & input)
  {
    if (not var_names.is_empty())
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

	var_names.append(move(name));
	var_units.append(unit_ptr);
	num_var++;
      }

    for (size_t linenum = 2; input.good(); ++num_samples, linenum++)
      {
	auto row = csv_read_row(input, ',');
	if (row.size() != num_var)
	  {
	    ostringstream s;
	    s << "EmpiricalData::add_samples(istream&): row number "
	      << linenum << " has << " << row.size()
	      << " cols, which is not exactly the number of vars " << num_var;
	    throw domain_error(s.str());
	  }
	DynList<double> sample;
	enum_for_each(zip(row, var_units), [&sample, linenum] (auto & p, auto i)
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
	samples.append(sample);
      }
  }

  //               var-name value
  using Par = pair<string, double>;

  void add_sample(DynList<Par> && pars)
  {
    DynList<double> sample;
    for (auto it = var_names.get_it(); it.has_curr(); it.next())
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

    samples.append(sample);
  }
  
  bool exists_var_name(const string & name) const noexcept
  {
    return var_names.exists([&name] (const string & s) { return s == name; });
  }

  void add_sample(DynList<string> && pars)
  {
    if (pars.size() != num_var)
      {
	ostringstream s;
	s << "EmpiricalData::add_sample(DynList<string> &&): pars size "
	  << pars.size() << " is different than number of vars " << num_var;
	throw domain_error(s.str());
      }

    DynList<Par> sample;
    size_t n = 0;
    for (; not pars.is_empty(); ++n)
      {
	auto tokens = split_to_list(pars.remove_first(), "=");
	string name = remove_spaces(tokens.remove_first());
	if (not exists_var_name(name))
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

    if (n < num_var or not pars.is_empty())
      {
	ostringstream s;
	s << "EmpiricalData::add_sample(DynList<string> &&): pars size "
	  << pars.size() << " is different than number of vars " << num_var;
	throw domain_error(s.str());
      }

    add_sample(move(sample));
  }

  /// return true if correlation correlation_ptr could be applied to
  /// the empirical data 
  bool can_be_applied(const Correlation * correlation_ptr) const
  {
    return correlation_ptr->names_and_synonyms().exists([this] (const auto & sign)
      {
	return this->contains_names(sign);
      });
  }

  /// Returns a list of correlation matching the names contaned in
  /// the data set
  DynList<const Correlation* const> matching_correlations()
  {
    return Correlation::list().filter([this] (auto ptr)
				      { return this->can_be_applied(ptr); });
  }

  // Return a list of list of correlation parameter belonging to the
  // data set that would be used in order to compute the all the value
  // of the correlation
  DynList<DynList<double>>
  correlation_values(const Correlation * correlation_ptr) const
  {
    DynList<DynList<double>> ret; // this is the return value

    // traverse all correlation parameter and get from the data set a
    // column of data
    for (auto it = correlation_ptr->get_preconditions().get_it(); it.has_curr();
	 it.next())
      {
	auto par = it.get_curr();
	DynList<double> vals = values(par.name); // extract value of data set
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
	  vals = values(syn_name); // verify if there are values in data set
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

  DynList<double> execute_correlation(const Correlation * correlation_ptr) const
  {
    auto vals = correlation_values(correlation_ptr);
    DynList<double> ret;

    for_each_perm(vals, [&ret, correlation_ptr] (const auto & pars)
      {
	pars.for_each([] (auto v) { cout << v << " "; }); cout << endl;
	ret.append(correlation_ptr->compute(pars));		       
      });
    return ret;
  }
};


# endif // EMPIRICAL_DATA_H
