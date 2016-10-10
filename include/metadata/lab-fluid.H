
# ifndef LAB_FLUID_H
# define LAB_FLUID_H 1

# include <tuple>

# include <units/pvt-units.H>
# include <correlations/correlation.H>

using namespace std;

struct LabFluidData
{
  size_t num_cols = 0;
  DynList<string> names; // pressure Rs RSB viscosity
  DynList<string> unit_symbols; // of above names
  DynList<const Unit*> units;
  DynList<DynList<double>> samples;

  // Lab data
  Quantity<Fahrenheit> t; // temperature
  Quantity<psia> pb; // bubble point

  // stock tank parameters
  
  Quantity<SCF_STB> rsb; // solution GOR at pb
  Quantity<Api> api; // api
  Quantity<Sgg> yg; // gas gravity
  Quantity<Disolved_Salt_PPM> s; // water salinity
  Quantity<Disolved_Salt_Percent> wcut; // water cut
  Quantity<Fahrenheit> tsep; // separator temp
  Quantity<psia> psep; // separator pressure

  void add_sample(double p, double rs, double rsb, double uo)
  {
    samples.append(make_tuple(p, rs, rsb, uo));
  }

  void add_samples(iostream & in)
  {

  }
};


# endif // LAB_FLUID_H
