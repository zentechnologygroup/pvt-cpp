# ifndef FLUID_H
# define FLUID_H

# include <json.hpp>

# include <pvt-exceptions.H>
# include <correlations/pvt-correlations.H>

using Json = nlohmann::json;

struct Fluid
{
  mutable Quantity<Api> api;
  mutable Quantity<Sgg> yg;
  mutable Quantity<SCF_STB> rsb;
  mutable Quantity<Dissolved_Salt_PPM> nacl;
  mutable Quantity<MolePercent> n2;
  mutable Quantity<VolumeFraction> h2s;
  mutable Quantity<Fahrenheit> tsep;
  mutable Quantity<psia> psep;
};

# endif
