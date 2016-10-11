
# ifndef EMPIRICAL_DATA_H
# define EMPIRICAL_DATA_H 1

# include <tuple>

# include <parse-csv.H>

# include <units/pvt-units.H>
# include <correlations/correlation.H>

using namespace std;

struct EmpiricalData
{
  size_t num_const = 0;
  DynList<string> const_names;
  DynList<const Unit*> const_units;

  size_t num_var = 0;
  DynList<string> var_names; // pressure Rs bo uo
  DynList<const Unit*> var_units; // of above names

  DynList<DynList<double>> samples; // must have the same number of
				    // columns than var_names 

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
    auto header = csv_read_row(input, ',');
    header.for_each([] (const auto & s) { cout << s << " "; }); cout << endl;

    for (auto it = header.get_it(); it.has_curr(); it.next())
      {
	const auto & s = it.get_curr();
	auto tokens = split_to_list(s, " ");
	tokens.for_each([] (const auto & t) { cout << "  " << t << endl; });
	cout << endl;
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
};


# endif // EMPIRICAL_DATA_H
