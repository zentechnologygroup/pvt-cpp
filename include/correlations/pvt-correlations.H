# ifndef PVT_CORRELATIONS_H
# define PVT_CORRELATIONS_H

# include "bubble-point-pressure.H"
# include "solution-gas-oil-relation.H"
# include "saturated-oil-formation-volume-factor.H"
# include "undersaturated-oil-formation-volume-factor.H"
# include "saturated-oil-compressibility.H"
# include "undersaturated-oil-compressibility.H"
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
# include "saturated-water-volume-factor.H"
# include "undersaturated-water-volume-factor.H"
# include "water-interfacial-tension.H"
# include "saturated-water-isothermal-compressibility.H"
# include "undersaturated-water-isothermal-compressibility.H"
# include "solution-gas-water-relation.H"
# include "spivey-dry.H"
# include "wet-gas-volume-factor.H"
# include "wet-gas-specific-gravity.H"
# include "equivalent-volume.H"
# include "additional-gas-produced.H"
# include "producing-gas-oil-ratio.H"

# include "bubble-point-pressure-impl.H"
# include "solution-gas-oil-relation-impl.H"
# include "saturated-oil-formation-volume-factor-impl.H"
# include "undersaturated-oil-formation-volume-factor-impl.H"
# include "saturated-oil-compressibility-impl.H"
# include "undersaturated-oil-compressibility-impl.H"
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
# include "saturated-water-volume-factor-impl.H"
# include "undersaturated-water-volume-factor-impl.H"
# include "water-interfacial-tension-impl.H"
# include "saturated-water-isothermal-compressibility-impl.H"
# include "undersaturated-water-isothermal-compressibility-impl.H"
# include "solution-gas-water-relation-impl.H"
# include "spivey-dry-impl.H"

# include "wet-gas-volume-factor-impl.H"
# include "wet-gas-specific-gravity-impl.H"
# include "equivalent-volume-impl.H"
# include "additional-gas-produced-impl.H"
# include "producing-gas-oil-ratio-impl.H"

struct CorrelationInstantiater
{
  CorrelationInstantiater()
  {
    UnitsInstancer::init();

        // Oil Bubble point
    PbAlMarhoun::get_instance();
    PbAlShammasi::get_instance();
    PbDeGhettoDetachedFromRs::get_instance();
    PbDeGhettoEtAl::get_instance();
    PbDindorukChristmanDetachedFromRs::get_instance();
    PbDindorukChristman::get_instance();
    PbDoklaOsman::get_instance();
    PbHanafyDetachedFromRs::get_instance();
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
    RsTotalCFPDetachedFromPb::get_instance();
    RsTotalCFP::get_instance();
    RsVasquezBeggs::get_instance();
    RsVelarde::get_instance();
    RsCegarra::get_instance();
    RsPerezML::get_instance();
    RsMillanArcia::get_instance();
    RsManucciRosales::get_instance();

    BobAlmarhoun::get_instance();
    BobAlShammasi::get_instance();
    BobDeGhetto::get_instance();
    BobDindorukChristman::get_instance();
    BobDoklaOsman::get_instance();
    BobGlaso::get_instance();
    BobHanafy::get_instance();
    BobKartoatmodjo::get_instance();
    BobPetroskyFarshad::get_instance();
    BobStanding::get_instance();
    BobTotalCFP::get_instance();
    BobVasquezBeggs::get_instance();
    BobPerezML::get_instance();
    BobVelardeMcCain::get_instance();
    BobMillanArcia::get_instance();
    BobManucciRosales::get_instance();
    BobLasater::get_instance();

    BoaMcCain::get_instance();
    BoaDeGhetto::get_instance();
    BoaHanafy::get_instance();
    BoaKartoatmodjoSchmidt::get_instance();
    BoaMillanArcia::get_instance();
    BoaPerezML::get_instance();
    BoaPetroskyFarshad::get_instance();
    BoaVasquezBeggs::get_instance();

    CobMcCainEtAl::get_instance();

    CoaDeGhetto::get_instance();
    CoaHanafy::get_instance();
    CoaKartoatmodjoSchmidt::get_instance();
    CoaMillanArcia::get_instance();
    CoaPerezML::get_instance();
    CoaPetroskyFarshad::get_instance();
    CoaVasquezBeggs::get_instance();

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

    YghcWichertAziz::get_instance();
    PpcmKayMixingRule::get_instance();
    AdjustedppcmWichertAziz::get_instance();
    TpcmKayMixingRule::get_instance();
    AdjustedtpcmWichertAziz::get_instance();

    PpchcStanding::get_instance();
    PpchcSutton::get_instance();
    PpchcGuoGhalambor::get_instance();
    PpcAhmed::get_instance();
    PpchcElsharkawyEtAl::get_instance();
    PpchcStandingHeavierFractions::get_instance();

    UgCarrKB::get_instance();
    UgLeeGE::get_instance();
    UgDeanStiel::get_instance();

    Pg::get_instance();

    Tpr::get_instance();
    Ppr::get_instance();

    ZfactorSarem::get_instance();
    ZfactorHallYarborough::get_instance();
    ZfactorDranchukPR::get_instance();
    ZfactorDranchukAK::get_instance();
    ZfactorGopal::get_instance();
    ZfactorBrillBeggs::get_instance();
    ZfactorPapay::get_instance();

    TpchcStanding::get_instance();
    TpchcStandingHeavierFractions::get_instance();
    TpchcSutton::get_instance();
    TpchcElsharkawyEtAl::get_instance();

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

    SgwJenningsNewman::get_instance();

    CwbSpiveyMN::get_instance();
    CwaSpiveyMN::get_instance();
    CwaDodsonStanding::get_instance();
    CwaOsif::get_instance();
    CwbMcCain::get_instance();

    RswSpiveyMN::get_instance();
    RswMcCoy::get_instance();
    RswCulbersonMcKetta::get_instance();

    PvSpiveyMN::get_instance();
    MwSpiveyMN::get_instance();
    CgfwSpiveyMN::get_instance();

    BwbSpiveyMN::get_instance();
    BwbMcCain::get_instance();
    BwbMcCoy::get_instance();
    BwaSpiveyMN::get_instance();
    BwaMcCain::get_instance();
    // Al no ejecutarse esta instancia ==> la correlación no se incluye en tabla
    BwSpiveyMNDry::get_instance();

    Bwg::get_instance();
    YwgrMcCain::get_instance();
    VeqspMcCain::get_instance();
    Veqsp2McCain::get_instance();
    GpaspMcCain::get_instance();
    Gpasp2McCain::get_instance();
    Rsp1::get_instance();
  }
};

# endif // PVT_CORRELATIONS_H

