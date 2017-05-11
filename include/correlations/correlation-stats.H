
# ifndef CORRELATION_STATS_H
# define CORRELATION_STATS_H 1

# include <cmath>

# include <gsl/gsl_statistics_double.h>
# include <gsl/gsl_fit.h>

# include <ah-zip.H>
# include <tpl_array.H>

# include "correlation.H"

using namespace std;

class CorrStat
{
  double mean;
  double var;
  size_t n = 0;

  const Array<double> y; // experimental data

  long double ss_tot = 0;

public:

  double r2(const Array<double> & yc) const
  {
    auto pearson_corr = gsl_stats_correlation(&y.base(), 1,
					      &yc.base(), 1, y.size());
    if (std::isnan(pearson_corr))
      return 1;
    
    return pearson_corr*pearson_corr;
  }

  double sigma_distance(const Array<double> & yc) const
  {
    double sum = 0;
    Array<double> error;
    for (auto it = get_zip_it(y, yc); it.has_curr(); it.next())
      {
	auto t = it.get_curr();
	auto e = fabs(get<0>(t) - get<1>(t));
	error.append(e);
	sum += e;
      }

    auto var = error.foldl<double>(0.0, [ym = sum/n] (auto acu, auto y)
				   {
				     double diff = y - ym;
				     return acu + diff*diff;
				   }) / n;

    return sqrt(var);
  }

  double mse(const Array<double> & yc) const
  {
    double sum = 0;
    for (auto it = get_zip_it(y, yc); it.has_curr(); it.next())
      {
	auto t = it.get_curr();
	auto diff = get<0>(t) - get<1>(t);
	sum += diff*diff;
      }

    return sqrt(sum/n);
  }

  typedef double (CorrStat::*Statistical)(const Array<double>&) const;

  Array<pair<string, Statistical>> statisticals;

  void set_statistical(const string & name, Statistical statistical)
  {
    statisticals.append(make_pair(name, statistical));
  }

  Statistical statistical(const string & name) const
  {
    auto ptr = statisticals.find_ptr([&name] (const auto & p)
				     { return p.first == name; });
    if (ptr)
      return ptr->second;

    ostringstream s;
    s << "CorrStat::statistical(" << name << ") : statistica name not found";
    throw domain_error(s.str());
  }

  void compute_base()
  {
    if (y.is_empty())
      ZENTHROW(SizeMismatch, "experimental data is empty");
    
    set_statistical("r2", &CorrStat::r2);
    set_statistical("mse", &CorrStat::mse);
    set_statistical("sigma_distance", &CorrStat::sigma_distance);
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

  struct LFit
  {
    double c = 0;
    double m = 1;
    double cov00 = 0;
    double cov01 = 0;
    double cov11 = 0;
    double sumsq = 0;

    LFit() noexcept {}

    LFit(const Array<double> & y, const Array<double> & yc)
    {
      if (y.size() != yc.size())
	ZENTHROW(SizeMismatch, "y and yc have different sizes");

      if (y.size() == 1)
	{
	  c = yc(0) - y(0);
	  return;
	}

      // cout << "y ="; y.for_each([] (auto v) { cout << " " << v; }); cout << endl;
      // cout << "yc ="; yc.for_each([] (auto v) { cout << " " << v; }); cout << endl;
      gsl_fit_linear(&yc.base(), 1, &y.base(), 1, yc.size(), &c, &m,
		     &cov00, &cov01, &cov11, &sumsq);
      //      cout << to_string() << endl;
      // if (std::isnan(c))
      // 	c = 0;
      // if (std::isnan(m))
      // 	m = 1;
      // if (std::isnan(sumsq))
      // 	sumsq = 0;
    }

    string to_string() const
    {
      ostringstream s;
      s << "Line y = " << c << " + " << m << " x" << endl
	<< "cov00 = " << cov00 << endl
	<< "cov01 = " << cov01 << endl
	<< "cov11 = " << cov11 << endl
	<< "sumsq = " << sumsq;
      return s.str();
    }

    bool operator == (const LFit & rhs) const noexcept
    {
      return c == rhs.c and m == rhs.m and sumsq == rhs.sumsq;
    }
  };

  LFit linear_fit(const Array<double> & yc) const
  {
    return LFit(y, yc);
  }

  //                  r2       mse   sigma
  using Desc = tuple<double, double, double, LFit>;

  static double r2(const Desc & d) { return get<0>(d); }
  static double mse(const Desc & d) { return get<1>(d); }
  static double sigma(const Desc & d) { return get<2>(d); }
  static double sumsq(const Desc & d) { return get<3>(d).sumsq; }
  static double c(const Desc & d) { return get<3>(d).c; }
  static double m(const Desc & d) { return get<3>(d).m; }
  static bool is_valid(const Desc & d)
  {
    bool invalid = std::isnan(r2(d)) or std::isnan(mse(d)) or std::isnan(sigma(d)) or
      std::isnan(sumsq(d)) or std::isnan(c(d)) or std::isnan(m(d));
    return not invalid;
  }

  Desc stats(const Array<double> & yc) const
  {
    if (yc.size() != y.size())
      ZENTHROW(SizeMismatch,
	       "experimental and correlation data do not have the same size");
    double r2 = this->r2(yc);
    double mse = this->mse(yc);
    double sigma = sigma_distance(yc);
    LFit fit = linear_fit(yc);

    return make_tuple(r2, mse, sigma, fit);
  }

  static DynList<string> desc_to_dynlist(const Desc & d)
  {
    return build_dynlist<string>(::to_string(r2(d)), ::to_string(mse(d)),
				 ::to_string(sigma(d)), ::to_string(sumsq(d)),
				 ::to_string(c(d)), ::to_string(m(d)),
				 is_valid(d) ? "valid" : "invalid");
  }

  static DynList<string> invalid_desc_to_dynlist()
  {
    return build_dynlist<string>("-", "-", "-", "-", "-", "-", "invalid");
  }

  static DynList<string> stats_header()
  {
    return
      build_dynlist<string>("r2", "mse", "sigma", "sumsq", "c", "m", "status");
  }

  // each sample contains a row of parameters plus the value
  CorrStat(const Array<double> & y) : y(y) { compute_base(); }

  CorrStat(Array<double> && y) : y(move(y)) { compute_base(); }

  CorrStat(const DynList<double> & y) : y(y) { compute_base(); }
};

		 

# endif // CORRELATION_STATS_H

