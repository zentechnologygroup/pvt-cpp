
# ifndef CORRELATION_ANALYZER_H
# define CORRELATION_ANALYZER_H 1

# include <correlations/correlation-stats.H>
# include "empirical-data.H"

struct CorrelationMatcher
{
  DynList<const Correlation *> correlation_list; 

  // The available data set for analysing 
  const EmpiricalData data_set;

  // the target variable name which we want to analyze
  string target_var;

  // variables to be considerated in the analysis
  DynList<string> vars;

  CorrelationMatcher(EmpiricalData && ed) : data_set(move(ed)) {}

  CorrelationMatcher(const EmpiricalData & ed) : data_set(ed) {}

  // return true if correlation correlation_ptr could be applied to
  // the empirical data
  bool can_be_applied(const Correlation * correlation_ptr) const
  {
    return data_set.contains_names(correlation_ptr->names()); 
  }

  // Returns the target correlations matching the target_var name
  DynList<const Correlation * const>
  target_correlations() const
  {
    return Correlation::list().filter([this] (const Correlation * ptr)
      {
	return is_prefix(Aleph::to_lower(ptr->name), target_var);
      });
  }
};

# endif // CORRELATION_ANALYZER_H
