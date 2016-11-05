
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
  };

  struct Cmp
  {
    bool operator () (const Interval & i1, const Interval & i2) const
    {
      return i1.end < i2.start;
    }
  };

  DynSetTree<Interval> intervals;

public:

};


# endif // DEFINED_CORRELATION_H
