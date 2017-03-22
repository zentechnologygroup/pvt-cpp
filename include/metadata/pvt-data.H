# ifndef PVT_DATA_H
# define PVT_DATA_H

# include <tpl_array.H>

# include <json.hpp>

# include <correlations/correlation-stats.H>
# include "metadata-exceptions.H"

struct DataArray
{
  const string xname = "no-name";
  const string yname = "no-name";
  const Unit * xunit = nullptr;
  const Unit * yunit = nullptr;

  const Array<double> x, y;

  DataArray(const string xname, const string yname,
	     const Unit * xunit, const Unit * yunit,
	     const Array<double> & x, const Array<double> & y)
    : xname(xname), yname(yname), xunit(xunit), yunit(yunit), x(x), y(y)
  {
    assert(is_sorted(x));
  }

  CorrStat::Desc stats(const Array<double> & yc, const Unit * unit_ptr) const
  {
    return CorrStat(
  }
};

# endif
