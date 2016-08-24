# ifndef PVT_CORRELATIONS_H
# define PVT_CORRELATIONS_H

# include <bubble-point-pressure.H>
# include <solution-gas-oil-relation.H>

struct CorrelationInstantiater
{
  CorrelationInstantiater()
  {
        // Oil Bubble point
    PbAlMarhoun::get_instance();
    PbAlShammasi::get_instance();
    PbDeGhetto::get_instance();
    PbDindorukChristman::get_instance();
    PbDoklaOsman::get_instance();
    PbHanafy::get_instance();
    PbKartoatmodjoSchmidt::get_instance();
    PbLasater::get_instance();
    PbPetroskyFarshad::get_instance();
    PbStanding::get_instance();
    PbTotalCFP::get_instance();
    PbVasquezBeggs::get_instance();
    PbVelarde::get_instance();
    PbCegarra::get_instance();
    PbPerezML::get_instance();
    PbMillanArcia::get_instance();
    PbManucciRosales::get_instance();

    RsAlShammasi::get_instance();
  }
};

# endif // PVT_CORRELATIONS_H
