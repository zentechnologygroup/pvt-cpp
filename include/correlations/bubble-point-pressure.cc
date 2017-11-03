Quantity<psia> PbAlMarhoun__correlation__fct(const Quantity<Sgg>& yg,
                                             const Quantity<Sg_do>& yo,
                                             const Quantity<SCF_STB>& rsb,
                                             const Quantity<Rankine>& t) {
  return PbAlMarhoun::get_instance()(yg, yo, rsb, t);
}

double PbAlMarhoun__correlation__valid__fct(const double& yg, const double& yo,
                                            const double& rsb,
                                            const double& t) {
  return PbAlMarhoun::get_instance()(yg, yo, rsb, t).raw();
}

Quantity<psia> PbAlShammasi__correlation__fct(const Quantity<Sgg>& yg,
                                              const Quantity<Sg_do>& yo,
                                              const Quantity<SCF_STB>& rsb,
                                              const Quantity<Fahrenheit>& t) {
  return PbAlShammasi::get_instance()(yg, yo, rsb, t);
}

double PbAlShammasi__correlation__valid__fct(const double& yg, const double& yo,
                                             const double& rsb,
                                             const double& t) {
  return PbAlShammasi::get_instance()(yg, yo, rsb, t).raw();
}

Quantity<psia> PbDeGhettoDetachedFromRs__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t) {
  return PbDeGhettoDetachedFromRs::get_instance()(yg, rsb, api, t);
}

double PbDeGhettoDetachedFromRs__correlation__valid__fct(const double& yg,
                                                         const double& rsb,
                                                         const double& api,
                                                         const double& t) {
  return PbDeGhettoDetachedFromRs::get_instance()(yg, rsb, api, t).raw();
}

Quantity<psia> PbDeGhettoEtAl__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t,
    const Quantity<Fahrenheit>& tsep, const Quantity<psia>& psep) {
  return PbDeGhettoEtAl::get_instance()(yg, rsb, api, t, tsep, psep);
}

double PbDeGhettoEtAl__correlation__valid__fct(
    const double& yg, const double& rsb, const double& api, const double& t,
    const double& tsep, const double& psep) {
  return PbDeGhettoEtAl::get_instance()(yg, rsb, api, t, tsep, psep).raw();
}

Quantity<psia> PbDindorukChristmanDetachedFromRs__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t) {
  return PbDindorukChristmanDetachedFromRs::get_instance()(yg, rsb, api, t);
}

double PbDindorukChristmanDetachedFromRs__correlation__valid__fct(
    const double& yg, const double& rsb, const double& api, const double& t) {
  return PbDindorukChristmanDetachedFromRs::get_instance()(yg, rsb, api, t)
      .raw();
}

Quantity<psia> PbDindorukChristman__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t) {
  return PbDindorukChristman::get_instance()(yg, rsb, api, t);
}

double PbDindorukChristman__correlation__valid__fct(const double& yg,
                                                    const double& rsb,
                                                    const double& api,
                                                    const double& t) {
  return PbDindorukChristman::get_instance()(yg, rsb, api, t).raw();
}

Quantity<psia> PbDoklaOsman__correlation__fct(const Quantity<Sgg>& yg,
                                              const Quantity<SCF_STB>& rsb,
                                              const Quantity<Sg_do>& yo,
                                              const Quantity<Rankine>& t) {
  return PbDoklaOsman::get_instance()(yg, rsb, yo, t);
}

double PbDoklaOsman__correlation__valid__fct(const double& yg,
                                             const double& rsb,
                                             const double& yo,
                                             const double& t) {
  return PbDoklaOsman::get_instance()(yg, rsb, yo, t).raw();
}

Quantity<psia> PbHanafyDetachedFromRs__correlation__fct(
    const Quantity<SCF_STB>& rsb) {
  return PbHanafyDetachedFromRs::get_instance()(rsb);
}

double PbHanafyDetachedFromRs__correlation__valid__fct(const double& rsb) {
  return PbHanafyDetachedFromRs::get_instance()(rsb).raw();
}

Quantity<psia> PbHanafy__correlation__fct(const Quantity<SCF_STB>& rsb) {
  return PbHanafy::get_instance()(rsb);
}

double PbHanafy__correlation__valid__fct(const double& rsb) {
  return PbHanafy::get_instance()(rsb).raw();
}

Quantity<psia> PbKartoatmodjoSchmidt__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t,
    const Quantity<Fahrenheit>& tsep, const Quantity<psia>& psep) {
  return PbKartoatmodjoSchmidt::get_instance()(yg, rsb, api, t, tsep, psep);
}

double PbKartoatmodjoSchmidt__correlation__valid__fct(
    const double& yg, const double& rsb, const double& api, const double& t,
    const double& tsep, const double& psep) {
  return PbKartoatmodjoSchmidt::get_instance()(yg, rsb, api, t, tsep, psep)
      .raw();
}

Quantity<psia> PbLasater__correlation__fct(const Quantity<Sgg>& yg,
                                           const Quantity<SCF_STB>& rsb,
                                           const Quantity<Sg_do>& yo,
                                           const Quantity<Fahrenheit>& t) {
  return PbLasater::get_instance()(yg, rsb, yo, t);
}

double PbLasater__correlation__valid__fct(const double& yg, const double& rsb,
                                          const double& yo, const double& t) {
  return PbLasater::get_instance()(yg, rsb, yo, t).raw();
}

Quantity<psia> PbPetroskyFarshad__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t) {
  return PbPetroskyFarshad::get_instance()(yg, rsb, api, t);
}

double PbPetroskyFarshad__correlation__valid__fct(const double& yg,
                                                  const double& rsb,
                                                  const double& api,
                                                  const double& t) {
  return PbPetroskyFarshad::get_instance()(yg, rsb, api, t).raw();
}

Quantity<psia> PbStanding__correlation__fct(const Quantity<Sgg>& yg,
                                            const Quantity<SCF_STB>& rsb,
                                            const Quantity<Api>& api,
                                            const Quantity<Fahrenheit>& t) {
  return PbStanding::get_instance()(yg, rsb, api, t);
}

double PbStanding__correlation__valid__fct(const double& yg, const double& rsb,
                                           const double& api, const double& t) {
  return PbStanding::get_instance()(yg, rsb, api, t).raw();
}

Quantity<psia> PbTotalCFP__correlation__fct(const Quantity<Sgg>& yg,
                                            const Quantity<SCF_STB>& rsb,
                                            const Quantity<Api>& api,
                                            const Quantity<Fahrenheit>& t) {
  return PbTotalCFP::get_instance()(yg, rsb, api, t);
}

double PbTotalCFP__correlation__valid__fct(const double& yg, const double& rsb,
                                           const double& api, const double& t) {
  return PbTotalCFP::get_instance()(yg, rsb, api, t).raw();
}

Quantity<psia> PbVasquezBeggs__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t,
    const Quantity<Fahrenheit>& tsep, const Quantity<psia>& psep) {
  return PbVasquezBeggs::get_instance()(yg, rsb, api, t, tsep, psep);
}

double PbVasquezBeggs__correlation__valid__fct(
    const double& yg, const double& rsb, const double& api, const double& t,
    const double& tsep, const double& psep) {
  return PbVasquezBeggs::get_instance()(yg, rsb, api, t, tsep, psep).raw();
}

Quantity<psia> PbVelarde__correlation__fct(const Quantity<Sgg>& yg,
                                           const Quantity<SCF_STB>& rsb,
                                           const Quantity<Api>& api,
                                           const Quantity<Fahrenheit>& t) {
  return PbVelarde::get_instance()(yg, rsb, api, t);
}

double PbVelarde__correlation__valid__fct(const double& yg, const double& rsb,
                                          const double& api, const double& t) {
  return PbVelarde::get_instance()(yg, rsb, api, t).raw();
}

Quantity<psia> PbCegarra__correlation__fct(const Quantity<Sgg>& yg,
                                           const Quantity<SCF_STB>& rsb,
                                           const Quantity<Api>& api,
                                           const Quantity<Fahrenheit>& t) {
  return PbCegarra::get_instance()(yg, rsb, api, t);
}

double PbCegarra__correlation__valid__fct(const double& yg, const double& rsb,
                                          const double& api, const double& t) {
  return PbCegarra::get_instance()(yg, rsb, api, t).raw();
}

Quantity<psia> PbPerezML__correlation__fct(const Quantity<Sgg>& yg,
                                           const Quantity<SCF_STB>& rsb,
                                           const Quantity<Api>& api,
                                           const Quantity<Fahrenheit>& t) {
  return PbPerezML::get_instance()(yg, rsb, api, t);
}

double PbPerezML__correlation__valid__fct(const double& yg, const double& rsb,
                                          const double& api, const double& t) {
  return PbPerezML::get_instance()(yg, rsb, api, t).raw();
}

Quantity<psia> PbMillanArcia__correlation__fct(const Quantity<Sgg>& yg,
                                               const Quantity<SCF_STB>& rsb,
                                               const Quantity<Api>& api,
                                               const Quantity<Fahrenheit>& t) {
  return PbMillanArcia::get_instance()(yg, rsb, api, t);
}

double PbMillanArcia__correlation__valid__fct(const double& yg,
                                              const double& rsb,
                                              const double& api,
                                              const double& t) {
  return PbMillanArcia::get_instance()(yg, rsb, api, t).raw();
}

Quantity<psia> PbManucciRosales__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t) {
  return PbManucciRosales::get_instance()(yg, rsb, api, t);
}

double PbManucciRosales__correlation__valid__fct(const double& yg,
                                                 const double& rsb,
                                                 const double& api,
                                                 const double& t) {
  return PbManucciRosales::get_instance()(yg, rsb, api, t).raw();
}

Quantity<psia> PbGlaso__correlation__fct(const Quantity<Sgg>& yg,
                                         const Quantity<SCF_STB>& rsb,
                                         const Quantity<Api>& api,
                                         const Quantity<Fahrenheit>& t,
                                         const Quantity<MoleFraction>& n2,
                                         const Quantity<MoleFraction>& co2,
                                         const Quantity<MoleFraction>& h2s) {
  return PbGlaso::get_instance()(yg, rsb, api, t, n2, co2, h2s);
}

double PbGlaso__correlation__valid__fct(const double& yg, const double& rsb,
                                        const double& api, const double& t,
                                        const double& n2, const double& co2,
                                        const double& h2s) {
  return PbGlaso::get_instance()(yg, rsb, api, t, n2, co2, h2s).raw();
}

Quantity<psia> PbLopezCR__correlation__fct(const Quantity<Sgg>& yg,
                                           const Quantity<SCF_STB>& rsb,
                                           const Quantity<Api>& api,
                                           const Quantity<Fahrenheit>& t) {
  return PbLopezCR::get_instance()(yg, rsb, api, t);
}

double PbLopezCR__correlation__valid__fct(const double& yg, const double& rsb,
                                          const double& api, const double& t) {
  return PbLopezCR::get_instance()(yg, rsb, api, t).raw();
}

Quantity<psia> PbLopezCRDetachedFromRs__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
    const Quantity<Api>& api, const Quantity<Fahrenheit>& t) {
  return PbLopezCRDetachedFromRs::get_instance()(yg, rsb, api, t);
}

double PbLopezCRDetachedFromRs__correlation__valid__fct(const double& yg,
                                                        const double& rsb,
                                                        const double& api,
                                                        const double& t) {
  return PbLopezCRDetachedFromRs::get_instance()(yg, rsb, api, t).raw();
}

Quantity<psia> PbSalazar__correlation__fct(const Quantity<Sgg>& yg,
                                           const Quantity<SCF_STB>& rsb,
                                           const Quantity<Api>& api,
                                           const Quantity<Fahrenheit>& t) {
  return PbSalazar::get_instance()(yg, rsb, api, t);
}

double PbSalazar__correlation__valid__fct(const double& yg, const double& rsb,
                                          const double& api, const double& t) {
  return PbSalazar::get_instance()(yg, rsb, api, t).raw();
}
