
# ifndef CORRELATION_STATS_H
# define CORRELATION_STATS_H 1

# include "correlation.H"

class CorrStat
{
  double mean;
  double var;
  size_t n = 0;

  const DynList<DynList<double>> samples;

  long double ss_tot = 0;

public:

  // each sample contains a row of parameters plus the value
  CorrStat(const DynList<DynList<double>> & samples)
    : samples(samples)
  {
    long double sum = 0;
    for (auto it = samples.get_it(); it.has_curr(); it.next(), ++n)
      {
	const auto & sample = it.get_curr();
	const auto & y = sample.get_last();
	sum += y;
      }
    mean = sum/n;

    long double sum2 = 0;
    for (auto it = samples.get_it(); it.has_curr(); it.next())
      {
	const auto & sample = it.get_curr();
	const auto & y = sample.get_last();
	const double diff = y - mean;
	sum2 += diff*diff;
      }
    var = sum2/n;
    ss_tot = sum2;
  }

  CorrStat(const DynList<double> & y)
  {
    long double sum = 0;
    for (auto it = y.get_it(); it.has_curr(); it.next(), ++n)
      sum += it.get_curr();
    mean = sum/n;

    long double sum2 = 0;
    for (auto it = y.get_it(); it.has_curr(); it.next())
      {
	const double diff = it.get_curr() - mean;
	sum2 += diff*diff;
      }
    var = sum2/n;
    ss_tot = sum2;
  }

  CorrStat(ifstream & input)
  {

  }

  double r2(const Correlation * correlation)
  {
    if (samples.is_empty())
      throw domain_error("CorrStat::r2: stat object was built without samples");

    long double ss_res = 0;
    const size_t num_pars = correlation->get_num_pars();

    for (auto it = samples.get_it(); it.has_curr(); it.next())
      {
	const auto & row = it.get_curr();
	const double & y_i = row.get_last();
	const double f_i = correlation->compute(row.take(num_pars));
	const double diff = y_i - f_i;
	ss_res += diff*diff;
      }

    return 1 - ss_res/ss_tot;
  }
};

# endif // CORRELATION_STATS_H

