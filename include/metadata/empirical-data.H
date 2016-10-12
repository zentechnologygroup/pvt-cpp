
# ifndef EMPIRICAL_DATA_H
# define EMPIRICAL_DATA_H 1

# include <tuple>

# include <parse-csv.H>
# include <ah-string-utils.H>

# include <units/pvt-units.H>
# include <correlations/correlation.H>

using namespace std;
using namespace Aleph;

struct EmpiricalData
{
  size_t num_const = 0;
  DynList<string> const_names;
  DynList<double> const_vals;
  DynList<const Unit*> const_units;

  size_t num_var = 0;
  DynList<string> var_names; // pressure Rs bo uo
  DynList<const Unit*> var_units; // of above names

  size_t num_samples = 0;
  DynList<DynList<double>> samples; // must have the same number of
				    // columns than var_names

  string to_string() const
  {
    ostringstream s;
    s << "Empirical data set" << endl
      << "  Constant list:" << endl;

    DynList<DynList<string>> const_list;
    enum_for_each(zip(const_names, const_units), [&const_list] (auto p, size_t i)
      {
	DynList<string> row = { p.first, p.second->symbol, std::to_string(i + 1)};
	const_list.append(row);
      });
    auto str = Aleph::to_string(format_string(const_list));
    s << shift_lines_to_left(str, 4) << endl
      << endl
      << "  Variables list:" << endl;

    DynList<DynList<string>> var_list;
    enum_for_each(zip(var_names, var_units), [&var_list] (auto p, auto i)
      {
    	DynList<string> row = { p.first, p.second->symbol, std::to_string(i + 1)};
    	var_list.append(row);
      });
    str = Aleph::to_string(format_string(var_list));
    s << shift_lines_to_left(str, 4) << endl
      << endl;

    return s.str();
  }

  void def_const(const string & name, const Unit * unit)
  {
    const_names.append(name);
    const_units.append(unit);
    num_const++;
  }

  void def_const(const string & name, const string & symbol)
  {
    def_const(name, Unit::search_by_symbol(symbol));
  }

  void def_var(const string & name, const Unit * unit)
  {
    var_names.append(name);
    var_units.append(unit);
    num_var++;
  }

  void def_var(const string & name, const string & symbol)
  {
    def_var(name, Unit::search_by_symbol(symbol));
  }

  void add_sample(const DynList<double> & sample)
  {
    samples.append(sample);
  }

private:

  static const Unit * search_unit(const string & symbol)
  {
    const Unit * ptr = Unit::search_by_symbol(symbol);
    if (ptr)
      return ptr;

    ptr = Unit::search_by_name(symbol);
    if (ptr)
      return ptr;

    ostringstream s;
    s << "EmpiricalData::search_unit: unit symbol or name " << symbol
      << " not found";
    throw domain_error(s.str());
  }

public:

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

    for (; input.good(); ++num_samples)
      {
	auto row = csv_read_row(input, ',');
	if (row.size() != num_var)
	  {
	    ostringstream s;
	    s << "EmpiricalData::add_samples(istream&): row number "
	      << num_samples << " has << " << row.size()
	      << " cols, which is not exactly the number of vars " << num_var;
	    throw domain_error(s.str());
	  }
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
				  { return p.first == name; }));
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
};


# endif // EMPIRICAL_DATA_H
