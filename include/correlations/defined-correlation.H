
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

    VtlQuantity
    compute_and_check(const DynList<VtlQuantity> & pars,
		      bool check = true) const
    {
      if (tuned)
	return correlation_ptr->tuned_compute_and_check(pars, c, m, check);

      return correlation_ptr->compute_and_check(pars, check);
    }

    double
    compute(const DynList<double> & pars, bool check = true) const
    {
      if (tuned)
	return correlation_ptr->tuned_compute(pars, c, m, check);

      return correlation_ptr->compute(pars, check);
    }

    VtlQuantity
    compute(const DynList<Correlation::ParByName> & pars,
	    bool check = true) const
    {
      if (tuned)
	return correlation_ptr->tuned_compute_by_names(pars, c, m, check);

      return correlation_ptr->compute_by_names(pars, check);
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

public:

  /// Returns a list union of all parameters
  DynList<string> parameter_list() const
  {
    DynSetTree<string> pars;
    intervals.for_each([&pars] (const auto & i)
      {
	i.correlation_ptr->get_preconditions().
	  for_each([&pars] (const auto & par)
          {
	    pars.insert(par.name);
	  });
      });
    return pars.keys();
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

  void add_correlation(const Correlation * correlation_ptr,
		       double start, double end)
  {
    auto result = intervals.insert(Interval(correlation_ptr, start, end));
    if (result)
      return;
    ostringstream s;
    s << "DefinedCorrelation::add_correlation: interval [" << start << ", "
      << end << "] is already inserted";
    throw domain_error(s.str());
  }
  
  void add_tuned_correlation(const Correlation * correlation_ptr,
			     double start, double end, double c, double m)
  {
    auto result = intervals.insert(Interval(correlation_ptr, start, end, c, m));
    if (result)
      return;
    ostringstream s;
    s << "DefinedCorrelation::add_correlation: interval [" << start << ", "
      << end << "] is already inserted";
    throw domain_error(s.str());
  }

  VtlQuantity
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

    return interval_ptr->correlation_ptr->compute_by_names(pars, check);
  }
};


# endif // DEFINED_CORRELATION_H
