# ifndef PVT_CORRELATIONS_H
# define PVT_CORRELATIONS_H

# include <bubble-point-pressure.H>

struct CorrelationInstantiater
{
  CorrelationInstantiater()
  {
    PbAlMarhoun::get_instance();
    PbAlShammasi::get_instance();
    PbDeGhettoEHOil::get_instance();
    PbDeGhettoHOil::get_instance();
    PbDindorukChristman::get_instance();
    PbDoklaOsman::get_instance();
    PbHanafy::get_instance();
    PbKartoatmodjoSchmidt::get_instance();
    PbLasater::get_instance();
    PbPetroskyFarshad::get_instance();
    PbStanding::get_instance();
  }
};

# endif // PVT_CORRELATIONS_H
