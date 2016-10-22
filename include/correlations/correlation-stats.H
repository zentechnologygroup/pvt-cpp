
# ifndef CORRELATION_STATS_H
# define CORRELATION_STATS_H 1

# include <ah-zip.H>
# include "correlation.H"

class CorrStat
{
  double mean;
  double var;
  size_t n = 0;

  const DynList<double> y;

  long double ss_tot = 0;

  void compute_base()
  {
    long double sum = y.foldl(0.0, [this] (const auto & acu, const auto & yi)
			      {
				++n;
				return acu + yi;
			      });
    mean = sum/n;

    long double sum2 =
      y.foldl(0.0, [m = mean] (const auto & acu, const auto & yi)
	      {
		const double diff = yi - m;
		return acu + diff*diff;
	      });
    var = sum2/n;
    ss_tot = sum2;
  }

public:

  // each sample contains a row of parameters plus the value
  CorrStat(const DynList<double> & y) : y(y) { compute_base(); }

  CorrStat(DynList<double> && y) : y(move(y)) { compute_base(); }

  double r2(const DynList<double> & yc)
  {
    long double ss_res = 0;

    for (auto it = get_zip_it(y, yc); it.has_curr(); it.next())
      {
	const auto & t = it.get_curr();
	const double diff = get<0>(t) - get<1>(t);
	ss_res += diff*diff;
      }

    return 1 - ss_res/ss_tot;
  }
};

		 

# endif // CORRELATION_STATS_H

