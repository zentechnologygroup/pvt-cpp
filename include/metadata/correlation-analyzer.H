
# ifndef CORRELATION_ANALYZER_H
# define CORRELATION_ANALYZER_H 1

# include <correlations/correlation-stats.H>
# include "empirical-data.H"

struct CorrelationAnalyser
{
  DynList<const Correlation *> correlation_list; 

  // The available data set for analysing 
  const EmpiricalData data_set;

  // the target variable name which we want to analyze
  string target_var;

  // variables to be considerated in the analysis
  DynList<string> vars;

  CorrelationAnalyser(EmpiricalData && ed) : data_set(move(ed)) {}

  CorrelationAnalyser(const EmpiricalData & ed) : data_set(ed) {}

  // return true if correlation correlation_ptr could be applied to
  // the empirical data
  bool can_be_applied(const Correlation * correlation_ptr) const
  {
    auto corr_signatures = correlation_ptr->names_and_synonyms();

    return corr_signatures.exists([this] (const auto & sign)
      {
	return data_set.contains_names(sign);
      });
  }

  // Returns the target correlations matching the target_var name
  DynList<const Correlation * const> target_correlations() const
  {
    return Correlation::list().filter([this] (const Correlation * ptr)
      {
	return is_prefix(Aleph::to_lower(ptr->name), target_var);
      });
  }

  

  // TODO: función que retoorne las correlciones factibles

  // TODO función que calcule los valores de las correaciones
};

# endif // CORRELATION_ANALYZER_H
