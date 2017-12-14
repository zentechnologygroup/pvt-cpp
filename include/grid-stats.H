# ifndef GRID_STATS_H
# define GRID_STATS_H

# include <simple-stats.H>
# include <pvt-grid-compute.H>

class PvtStats
{
  PvtGrid grid;

  struct ExperimentalCol
  {
    mutable string name; // name of property as shown in the grid
    mutable double t; // always in Fahrenheit
    const Unit * tunit = nullptr;
    const Array<double> p;
    const Unit * punit = nullptr;
    const Array<double> y;
    const Unit * yunit = nullptr;

    ExperimentalCol(const string & __name, double __t, const Unit * __tunit,
		    const DynList<double> & __p, const Unit * __punit,
		    const DynList<double> & __y, const Unit * __yunit)
      : name(__name), t(__t), tunit(__tunit), p(__p), punit(__punit),
	y(__y), yunit(__yunit) {}
  };

  DynMapTree<string, 

};

# endif
