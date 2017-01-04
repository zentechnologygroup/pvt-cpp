
# ifndef DEFINED_CORRELATION_H
# define DEFINED_CORRELATION_H 1

# include "correlation.H"

class DefinedCorrelation
{
  struct Interval
  {
    const Correlation * correlation_ptr = nullptr;
    double start, end;

    double c, m; // set if the correlation is tuned
    bool tuned = false;

    const Unit * result_unit = nullptr;

    Interval() {}

    Interval(const Correlation * ptr, double start, double end)
      : correlation_ptr(ptr), start(start), end(end)
    {
      if (start <= end)
	return;
      ostringstream s;
      s << "start val " << start << " is greater than end val " << end;
      throw domain_error(s.str());
    }

    Interval(const Correlation * ptr, double start, double end,
	     double c, double m) : Interval(ptr, start, end)
    {
      tuned = true;
      this->c = c;
      this->m = m;
    }

    VtlQuantity to_result_unit(const VtlQuantity & val) const
    {
      if (result_unit == &val.unit)
	return val;
      return VtlQuantity(*result_unit, val);
    }

    VtlQuantity
    compute_and_check(const DynList<VtlQuantity> & pars,
		      bool check = true) const
    {
      auto val = tuned ?
	correlation_ptr->tuned_compute_and_check(pars, c, m, check) :
	correlation_ptr->compute_and_check(pars, check);
      return to_result_unit(val);
    }

    VtlQuantity compute(const DynList<Correlation::ParByName> & pars,
			bool check = true) const
    {
      auto val = tuned ?
	correlation_ptr->tuned_compute_by_names(pars, c, m, check) :
	correlation_ptr->compute_by_names(pars, check);
      return to_result_unit(val);
    }

    VtlQuantity compute(const DynList<Correlation::NamedPar> & pars,
			bool check = true) const
    {
      auto val = tuned ?
	correlation_ptr->tuned_compute_by_names(pars, c, m, check) :
	correlation_ptr->compute_by_names(pars, check);
      return to_result_unit(val);
    }

    VtlQuantity compute(const ParList & pars, bool check = true) const
    {
      auto val = tuned ?
	correlation_ptr->tuned_compute_by_names(pars, c, m, check) :
	correlation_ptr->compute_by_names(pars, check);
      return to_result_unit(val);
    }
  };

  struct Cmp
  {
    bool operator () (const Interval & i1, const Interval & i2) const
    {
      return i1.end < i2.start;
    }
  };

  DynSetTree<Interval, Avl_Tree, Cmp> intervals;
  string main_par_name;
      // store all the parameter names for all correlations
  DynSetTree<string> par_names;

  double min_val = 0;
  double max_val = numeric_limits<double>::max();

public:

  DynList<const Correlation*> correlations() const
  {
    return intervals.maps<const Correlation*> ([] (const auto & i)
					       { return i.correlation_ptr; });
  }

  const Unit * result_unit = nullptr; // it will be the result unit of
				      // the first defined correlation

  /// Returns a list union of all parameters names. Each item is a
  /// pair with the parameter name and a list of parameter synonyms
  DynList<pair<string, DynList<string>>> parameter_list() const
  {
    using P = pair<string, DynList<string>>;
    DynMapTree<string, DynList<string>> pars;
    intervals.for_each([&pars] (const auto & i)
      {
	i.correlation_ptr->get_preconditions().
	  for_each([&pars] (const auto & par)
          {
	    auto ptr = pars.insert(P(par.name, DynList<string>()));
	    if (ptr)
	      par.get_synonyms().for_each([ptr] (const auto & p)
                {
		  ptr->second.append(p.first);
		});	    
	  });
      });
    return pars.items_ptr().maps<P>([] (auto ptr)
      {
	return P(ptr->first, ptr->second);
      });
  }

    /// Returns a list union of all parameters. Each item is a pair with
  /// the parameter name and a list of parameter synonyms
  static DynList<pair<string, DynList<string>>>
    parameter_list(const DynList<const Correlation*> & l)
  {
    using P = pair<string, DynList<string>>;
    DynMapTree<string, DynList<string>> pars;
    l.for_each([&pars] (const auto & p)
      {
	p->get_preconditions().for_each([&pars] (const auto & par)
          {
	    auto ptr = pars.insert(P(par.name, DynList<string>()));
	    if (ptr)
	      par.get_synonyms().for_each([ptr] (const auto & p)
                {
		  ptr->second.append(p.first);
		});
	  });
      });
    return pars.items_ptr().maps<P>([] (auto ptr)
      {
	return P(ptr->first, ptr->second);
      });
  }

  Interval * search_interval(double val) const
  {
    Interval i(nullptr, val, val);
    return intervals.search(i);
  }

  bool contains(double val) const noexcept { return search_interval(val); }

  DynList<Interval> interval_list() const { return intervals.keys(); }

  DefinedCorrelation(const string & main_par_name)
    : main_par_name(main_par_name) {}

  void add_tuned_correlation(const Correlation * correlation_ptr,
			     double start, double end, double c, double m)
  {
    auto result = intervals.insert(Interval(correlation_ptr, start, end, c, m));
    if (result)
      {
	if (result_unit == nullptr)
	  result_unit = &correlation_ptr->unit;
	result->result_unit = result_unit;
	correlation_ptr->get_preconditions().for_each([this] (const auto & par)
          {
	    par_names.insert(par.name);
	  });
	return;
      }
    ostringstream s;
    s << "DefinedCorrelation::add_correlation: interval [" << start << ", "
      << end << "] is already inserted";
    throw domain_error(s.str());
  }

  void add_correlation(const Correlation * correlation_ptr,
		       double start, double end)
  {
    intervals.insert(Interval(correlation_ptr, start, end, 0, 1));
  }

  void set_min(double min_val) { this->min_val = min_val; }

  void set_max(double max_val) { this->max_val = max_val; } 

  double
  compute_by_names(const DynList<Correlation::ParByName> & pars,
		   bool check = true) const
  {
    auto main_par_ptr =
      pars.find_ptr([this] (auto p) { return p.first == main_par_name; });
    if (main_par_ptr == nullptr)
      {
	ostringstream s;
	s << "DefinedCorrelation: main parameter name " << main_par_name
	  << " was not found in parameter list";
	throw domain_error(s.str());
      }

    double main_val = main_par_ptr->second;

    Interval * interval_ptr = search_interval(main_val);
    if (interval_ptr == nullptr)
      {
	ostringstream s;
	s << "DefinedCorrelation: value " << main_val
	  << " was not found in any interval";
	throw domain_error(s.str());
      }

    double val = interval_ptr->compute(pars, check).raw();
    if (val < min_val)
      return min_val;
    if (val > max_val)
      return max_val;
    return val;
  }

  double compute_by_names(const DynList<Correlation::NamedPar> & pars,
			  bool check = true) const
  {
    auto main_par_ptr =
      pars.find_ptr([this] (auto t) { return get<1>(t) == main_par_name; });
    if (main_par_ptr == nullptr)
      {
	ostringstream s;
	s << "DefinedCorrelation: main parameter name " << main_par_name
	  << " was not found in parameter list";
	throw domain_error(s.str());
      }

    double main_val = get<2>(*main_par_ptr);

    Interval * interval_ptr = search_interval(main_val);
    if (interval_ptr == nullptr)
      {
	ostringstream s;
	s << "DefinedCorrelation: value " << main_val
	  << " was not found in any interval";
	throw domain_error(s.str());
      }

    double val = interval_ptr->compute(pars, check).raw();
    if (val < min_val)
      return min_val;
    if (val > max_val)
      return max_val;
    return val;
  }

  double compute_by_names(const ParList & pars, bool check = true) const
  {
    VtlQuantity main_val = pars(main_par_name);
    Interval * interval_ptr = search_interval(main_val.raw());
    if (interval_ptr == nullptr)
      {
	ostringstream s;
	s << "DefinedCorrelation: value " << main_val
	  << " was not found in any interval";
	throw domain_error(s.str());
      }

    double val = interval_ptr->compute(pars, check).raw();
    if (val < min_val)
      return min_val;
    if (val > max_val)
      return max_val;
    return val;
  }
};


# endif // DEFINED_CORRELATION_H
