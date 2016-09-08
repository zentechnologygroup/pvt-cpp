# ifndef PVT_CORRELATIONS_H
# define PVT_CORRELATIONS_H

# include <bubble-point-pressure.H>
# include <solution-gas-oil-relation.H>
# include <solution-gas-oil-relation-1.H>
# include <oil-formation-volume-factor.H>
# include <oil-compressibility.H>
# include <oil-density.H>
# include <dead-oil-viscosity.H>
# include <saturated-oil-viscosity.H>

# include <solution-gas-oil-relation-1-impl.H>
# include <oil-formation-volume-factor-impl.H>
# include <oil-compressibility-impl.H>
# include <oil-density-impl.H>
# include <dead-oil-viscosity-impl.H>
# include <saturated-oil-viscosity-impl.H>

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
    RsAlMarhoun::get_instance();
    RsDeGhetto::get_instance();
    RsDindorukChristman::get_instance();
    RsDoklaOsman::get_instance();
    RsGlaso::get_instance();
    RsHanafy::get_instance();
    RsKartoatmodjoSchmidt::get_instance();
    RsLasater::get_instance();
    RsPetroskyFarshad::get_instance();
    RsStanding::get_instance();
    RsTotalCFP::get_instance();
    RsVasquezBeggs::get_instance();
    RsVelarde::get_instance();
    RsCegarra::get_instance();
    RsPerezML::get_instance();
    RsMillanArcia::get_instance();
    RsManucciRosales::get_instance();

    BoAlmarhoun::get_instance();
    BoAlShammasi::get_instance();
    BoDeGhetto::get_instance();
    BoDindorukChristman::get_instance();
    BoDoklaOsman::get_instance();
    BoGlaso::get_instance();
    BoHanafy::get_instance();
    BoKartoatmodjo::get_instance();
    BoLasater::get_instance();
    BoPetroskyFarshad::get_instance();
    BoStanding::get_instance();
    BoTotalCFP::get_instance();
    BoVasquezBeggs::get_instance();
    BoCegarra::get_instance();
    BoPerezML::get_instance();
    BoVelardeMcCain::get_instance();
    BoMillanArcia::get_instance();
    BoManucciRosales::get_instance();

    CoMcCainEtAl::get_instance();
    CoDeGhetto::get_instance();
    CoHanafy::get_instance();
    CoKartoatmodjoSchmidt::get_instance();
    CoPetroskyFarshad::get_instance();
    CoVasquezBeggs::get_instance();
    CoPerezML::get_instance();
    CoMillanArcia::get_instance();

    PoBradley::get_instance();

    UodBeal::get_instance();
    UodBeggsRobinson::get_instance();
    UodEgbogahNg::get_instance();
    UodGlaso::get_instance();
    UodKartoatmodjoSchmidt::get_instance();
    UodSattarinEtAl::get_instance();
    UodNaseri::get_instance();
    UodPetroskyFarshad::get_instance();
    UodDeGhettoEtAl::get_instance();
    UodPerezML::get_instance();

    UobBeggsRobinson::get_instance();
    UobChewConnally::get_instance();
    UobKhan::get_instance();
    UobKartoatmodjoSchmidt::get_instance();
    UobPetroskyFarshad::get_instance();
    UobPerezML::get_instance();
    UobGilFonseca::get_instance();
    UobDeGhettoEtAl::get_instance();
  }
};

# endif // PVT_CORRELATIONS_H
