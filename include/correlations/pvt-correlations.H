# ifndef PVT_CORRELATIONS_H
# define PVT_CORRELATIONS_H

# include "bubble-point-pressure.H"
# include "solution-gas-oil-relation.H"
# include "oil-formation-volume-factor.H"
# include "saturated-oil-formation-volume-factor.H"
# include "undersaturated-oil-formation-volume-factor.H"
# include "oil-compressibility.H"
# include "saturated-oil-compressibility.H"
# include "undersaturated-oil-compressibility.H"
# include "oil-density.H"
# include "undersaturated-oil-density.H"
# include "saturated-oil-density.H"
# include "dead-oil-viscosity.H"
# include "saturated-oil-viscosity.H"
# include "undersaturated-oil-viscosity.H"
# include "oil-interfacial-tension.H"
# include "non-hydrocarborns-corrections.H"
# include "pseudo-reduced-ratios.H"
# include "gas-density.H"
# include "gas-pseudocritical-pressure.H"
# include "gas-viscosity.H"
# include "gas-compressibility.H"
# include "gas-pseudocritical-temp.H"
# include "gas-isothermal-compressibility.H"
# include "gas-volume-factor.H"
# include "water-density.H"
# include "water-viscosity.H"
# include "water-volume-factor.H"
# include "water-interfacial-tension.H"
# include "water-isothermal-compressibility.H"
# include "solution-gas-water-relation.H"
# include "spivey-dry.H"

# include "bubble-point-pressure-impl.H"
# include "solution-gas-oil-relation-impl.H"
# include "oil-formation-volume-factor-impl.H"
# include "saturated-oil-formation-volume-factor-impl.H"
# include "undersaturated-oil-formation-volume-factor-impl.H"
# include "oil-compressibility-impl.H"
# include "saturated-oil-compressibility-impl.H"
# include "undersaturated-oil-compressibility-impl.H"
# include "oil-density-impl.H"
# include "undersaturated-oil-density-impl.H"
# include "saturated-oil-density-impl.H"
# include "dead-oil-viscosity-impl.H"
# include "saturated-oil-viscosity-impl.H"
# include "undersaturated-oil-viscosity-impl.H"
# include "oil-interfacial-tension-impl.H"
# include "non-hydrocarborns-corrections-impl.H"
# include "pseudo-reduced-ratios-impl.H"
# include "gas-density-impl.H"
# include "gas-pseudocritical-pressure-impl.H"
# include "gas-viscosity-impl.H"
# include "gas-compressibility-impl.H"
# include "gas-pseudocritical-temp-impl.H"
# include "gas-isothermal-compressibility-impl.H"
# include "gas-volume-factor-impl.H"
# include "water-density-impl.H"
# include "water-viscosity-impl.H"
# include "water-volume-factor-impl.H"
# include "water-interfacial-tension-impl.H"
# include "water-isothermal-compressibility-impl.H"
# include "solution-gas-water-relation-impl.H"
# include "spivey-dry-impl.H"

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
    PbGlaso::get_instance();

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

    BobAlmarhoun::get_instance();
    BobAlShammasi::get_instance();
    BobDeGhetto::get_instance();
    BobDindorukChristman::get_instance();
    BobDoklaOsman::get_instance();
    BoGlaso::get_instance();
    BobHanafy::get_instance();
    BobKartoatmodjo::get_instance();
    BobPetroskyFarshad::get_instance();
    BobStanding::get_instance();
    BobTotalCFP::get_instance();
    BobVasquezBeggs::get_instance();
    BobPerezML::get_instance();
    BobPerezML::get_instance();
    BobVelardeMcCain::get_instance();
    BobMillanArcia::get_instance();
    BobManucciRosales::get_instance();

    BoaMcCain::get_instance();
    BobMillanArcia::get_instance();
    BoaDeGhetto::get_instance();
    BoaHanafy::get_instance();
    BoaKartoatmodjo::get_instance();
    BoaPetroskyFarshad::get_instance();
    BoaVasquezBeggs::get_instance();

    CoMcCainEtAl::get_instance();
    CoDeGhetto::get_instance();
    CoHanafy::get_instance();
    CoKartoatmodjoSchmidt::get_instance();
    CoPetroskyFarshad::get_instance();
    CoVasquezBeggs::get_instance();
    CoPerezML::get_instance();
    CoMillanArcia::get_instance();

    CobMcCainEtAl::get_instance();

    CoaDeGhetto::get_instance();
    CoaHanafy::get_instance();
    CoaKartoatmodjoSchmidt::get_instance();
    CoaMillanArcia::get_instance();
    CoaPerezML::get_instance();
    CoaPetroskyFarshad::get_instance();
    CoaVasquezBeggs::get_instance();

    PoBradley::get_instance();
    PobBradley::get_instance();
    PoaBradley::get_instance();

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
    UodDindorukChristman::get_instance();

    UobBeggsRobinson::get_instance();
    UobChewConnally::get_instance();
    UobKhan::get_instance();
    UobKartoatmodjoSchmidt::get_instance();
    UobPetroskyFarshad::get_instance();
    UobPerezML::get_instance();
    UobGilFonseca::get_instance();
    UobDeGhettoEtAl::get_instance();
    UobDindorukChristman::get_instance();

    UoaKartoatmodjoSchmidt::get_instance();
    UoaDeGhettoEtAl::get_instance();
    UoaBeal::get_instance();
    UoaVasquezBeggs::get_instance();
    UoaKhan::get_instance();
    UoaPetroskyFarshad::get_instance();
    UoaAbedini::get_instance();
    UoaPerezML::get_instance();
    UoaDindorukChristman::get_instance();

    SgoBakerSwerdloff::get_instance();

    YgHCWichertAziz::get_instance();
    PscMKayMixingRule::get_instance();
    AdjustedPscMWichertAziz::get_instance();
    TscMKayMixingRule::get_instance();
    AdjustedTscMWichertAziz::get_instance();

    PscHCStanding::get_instance();
    PscHcSutton::get_instance();
    PscHCGuoGhalambor::get_instance();
    PscAhmed::get_instance();
    PscHCElsharkawyEtAl::get_instance();
    PscHCStandingHeavierFractions::get_instance();

    UgCarrKB::get_instance();
    UgLeeGE::get_instance();
    UgDeanStiel::get_instance();

    Pg::get_instance();

    Tpr::get_instance();
    Ppr::get_instance();

    ZFactorSarem::get_instance();
    ZFactorHallYarborough::get_instance();
    ZFactorDranchukPR::get_instance();
    ZFactorDranchukAK::get_instance();
    ZFactorGopal::get_instance();
    ZFactorBrillBeggs::get_instance();
    ZFactorPapay::get_instance();

    TscHCStanding::get_instance();
    TscHCStandingHeavierFractions::get_instance();
    TscHCSutton::get_instance();
    TpcHCElsharkawyEtAl::get_instance();

    CgSarem::get_instance();
    CgHallYarborough::get_instance();
    CgMattarBA::get_instance();
    CgGopal::get_instance();
    CgBrillBeggs::get_instance();
    CgPapay::get_instance();

    Bg::get_instance();

    PwSpiveyMN::get_instance();
    PwSpiveyMNGasFree::get_instance();
    PwMcCain::get_instance();
    PpwSpiveyMN::get_instance();

    UwMaoDuan::get_instance();
    UwVanWingen::get_instance();
    UwMatthewsRussel::get_instance();
    UwMcCain::get_instance();
    UwMcCoy::get_instance();

    BwSpiveyMN::get_instance();
    BwMcCain::get_instance();
    BwMcCoy::get_instance();

    SgwJenningsNewman::get_instance();

    CwSpiveyMN::get_instance();
    CwDodsonStanding::get_instance();
    CwOsif::get_instance();
    CwbMcCain::get_instance();

    RswSpiveyMN::get_instance();
    RswMcCoy::get_instance();
    RswCulbersonMcKetta::get_instance();

    PvSpiveyMN::get_instance();
    MwSpiveyMN::get_instance();
    CgfwSpiveyMN::get_instance();

  }
};

# endif // PVT_CORRELATIONS_H

