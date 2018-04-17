# ifndef GRID_STATS_H
# define GRID_STATS_H

# include <simple-stats.H>
# include <pvt-grid-compute.H>
# include <pvt-units.H>

DEFINE_ALEPH_EXCEPTION(DuplicatedTemperature, "Duplicated temperature");

class PvtStats
{
  PvtGrid grid;

  struct ExperimentalSample
  {
    mutable string name; // name of property as shown in the grid
    mutable double t; // always in Fahrenheit
    const DynList<double> p;
    const Unit * punit = nullptr;
    const DynList<double> y;
    const Unit * yunit = nullptr;

    ExperimentalSample(const string & __name, double __t, const Unit * __tunit,
		       const DynList<double> & __p, const Unit * __punit,
		       const DynList<double> & __y, const Unit * __yunit)
      : name(__name), t(__t), p(__p), punit(__punit), y(__y), yunit(__yunit)
    {
      if (not __tunit->is_sibling(Fahrenheit::get_instance()))
	ALEPHTHROW(InvalidUnit, __tunit->name + " is not a temperature unit");
      if (not punit->is_sibling(psia::get_instance()))
	ALEPHTHROW(InvalidUnit, punit->name + " is not a pressure unit");
      if (__tunit != &Fahrenheit::get_instance())
	t = unit_convert(*__tunit, t, Fahrenheit::get_instance());
    }
  };

  DynMapTree<string, DynList<ExperimentalSample>> exp_samples;

  void add_sample(const string & name, double t, const Unit * tunit,
		  const DynList<double> & p, const Unit * punit,
		  const DynList<double> & y, const Unit * yunit)
  {
    auto samples_list = exp_samples[name];
    if (samples_list.exists([&t] (auto & s) { return s.t == t; }))
      ALEPHTHROW(DuplicatedTemperature, to_str(t) + " is already in the set");

    exp_samples.emplace_end(name, t, tunit, p, punit, y, yunit);
  }

  DynMapTree<string, SimpleStats> stats() const
  {
    DynMapTree<string, SimpleStats> ret;

    for (auto it = exp_samples.get_it(); it.has_curr(); it.next())
      {
	auto pp = it.get_curr();
	const string & name = pp.first;
	const DynList<ExperimentalSample> & samples_list = pp.second;
	const ExperimentalSample & fst_s = samples_list.get_first();
	const Unit * yunit = fst_s.yunit;
	DynList<double> y = fst_s.y;
	for (auto it = samples_list.get_it(1); it.has_curr(); it.next())
	  {
	    const ExperimentalSample & s = it.get_curr();
	    y.append(unit_convert(*s.yunit, s.p, *yunit));
	  }
	ret.insert(name, SimpleStats(y));
      }

    return ret;
  }
};

# endif
