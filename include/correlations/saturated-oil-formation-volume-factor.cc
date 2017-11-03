Quantity<RB_STB> BobAlmarhoun__correlation__fct(const Quantity<Sgg>& yg,
                                                const Quantity<Sg_do>& yo,
                                                const Quantity<SCF_STB>& rs,
                                                const Quantity<Rankine>& t) {
  return BobAlmarhoun::get_instance()(yg, yo, rs, t);
}

double BobAlmarhoun__correlation__valid__fct(const double& yg, const double& yo,
                                             const double& rs,
                                             const double& t) {
  return BobAlmarhoun::get_instance()(yg, yo, rs, t).raw();
}

Quantity<RB_STB> BobAlShammasi__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
    const Quantity<SCF_STB>& rs, const Quantity<Fahrenheit>& t) {
  return BobAlShammasi::get_instance()(yg, yo, rs, t);
}

double BobAlShammasi__correlation__valid__fct(const double& yg,
                                              const double& yo,
                                              const double& rs,
                                              const double& t) {
  return BobAlShammasi::get_instance()(yg, yo, rs, t).raw();
}

Quantity<RB_STB> BobCegarra__correlation__fct(const Quantity<Sgg>& yg,
                                              const Quantity<Sg_do>& yo,
                                              const Quantity<SCF_STB>& rs,
                                              const Quantity<Fahrenheit>& t) {
  return BobCegarra::get_instance()(yg, yo, rs, t);
}

double BobCegarra__correlation__valid__fct(const double& yg, const double& yo,
                                           const double& rs, const double& t) {
  return BobCegarra::get_instance()(yg, yo, rs, t).raw();
}

Quantity<RB_STB> BobDeGhetto__correlation__fct(const Quantity<Sgg>& yg,
                                               const Quantity<Api>& api,
                                               const Quantity<SCF_STB>& rs,
                                               const Quantity<Fahrenheit>& t,
                                               const Quantity<Fahrenheit>& tsep,
                                               const Quantity<psia>& psep) {
  return BobDeGhetto::get_instance()(yg, api, rs, t, tsep, psep);
}

double BobDeGhetto__correlation__valid__fct(const double& yg, const double& api,
                                            const double& rs, const double& t,
                                            const double& tsep,
                                            const double& psep) {
  return BobDeGhetto::get_instance()(yg, api, rs, t, tsep, psep).raw();
}

Quantity<RB_STB> BobDindorukChristman__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<Api>& api,
    const Quantity<SCF_STB>& rs, const Quantity<Fahrenheit>& t,
    const Quantity<Fahrenheit>& tsep) {
  return BobDindorukChristman::get_instance()(yg, api, rs, t, tsep);
}

double BobDindorukChristman__correlation__valid__fct(const double& yg,
                                                     const double& api,
                                                     const double& rs,
                                                     const double& t,
                                                     const double& tsep) {
  return BobDindorukChristman::get_instance()(yg, api, rs, t, tsep).raw();
}

Quantity<RB_STB> BobDoklaOsman__correlation__fct(const Quantity<Sgg>& yg,
                                                 const Quantity<Sg_do>& yo,
                                                 const Quantity<SCF_STB>& rs,
                                                 const Quantity<Rankine>& t) {
  return BobDoklaOsman::get_instance()(yg, yo, rs, t);
}

double BobDoklaOsman__correlation__valid__fct(const double& yg,
                                              const double& yo,
                                              const double& rs,
                                              const double& t) {
  return BobDoklaOsman::get_instance()(yg, yo, rs, t).raw();
}

Quantity<RB_STB> BobGlaso__correlation__fct(const Quantity<Sgg>& yg,
                                            const Quantity<Sg_do>& yo,
                                            const Quantity<SCF_STB>& rs,
                                            const Quantity<Fahrenheit>& t) {
  return BobGlaso::get_instance()(yg, yo, rs, t);
}

double BobGlaso__correlation__valid__fct(const double& yg, const double& yo,
                                         const double& rs, const double& t) {
  return BobGlaso::get_instance()(yg, yo, rs, t).raw();
}

Quantity<RB_STB> BobHanafy__correlation__fct(const Quantity<SCF_STB>& rs) {
  return BobHanafy::get_instance()(rs);
}

double BobHanafy__correlation__valid__fct(const double& rs) {
  return BobHanafy::get_instance()(rs).raw();
}

Quantity<RB_STB> BobKartoatmodjo__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
    const Quantity<SCF_STB>& rs, const Quantity<Fahrenheit>& t,
    const Quantity<Fahrenheit>& tsep, const Quantity<psia>& psep) {
  return BobKartoatmodjo::get_instance()(yg, yo, rs, t, tsep, psep);
}

double BobKartoatmodjo__correlation__valid__fct(
    const double& yg, const double& yo, const double& rs, const double& t,
    const double& tsep, const double& psep) {
  return BobKartoatmodjo::get_instance()(yg, yo, rs, t, tsep, psep).raw();
}

Quantity<RB_STB> BobStanding__correlation__fct(const Quantity<Sgg>& yg,
                                               const Quantity<Sg_do>& yo,
                                               const Quantity<SCF_STB>& rs,
                                               const Quantity<Fahrenheit>& t) {
  return BobStanding::get_instance()(yg, yo, rs, t);
}

double BobStanding__correlation__valid__fct(const double& yg, const double& yo,
                                            const double& rs, const double& t) {
  return BobStanding::get_instance()(yg, yo, rs, t).raw();
}

Quantity<RB_STB> BobLasater__correlation__fct(const Quantity<Sgg>& yg,
                                              const Quantity<Sg_do>& yo,
                                              const Quantity<SCF_STB>& rs,
                                              const Quantity<Fahrenheit>& t) {
  return BobLasater::get_instance()(yg, yo, rs, t);
}

double BobLasater__correlation__valid__fct(const double& yg, const double& yo,
                                           const double& rs, const double& t) {
  return BobLasater::get_instance()(yg, yo, rs, t).raw();
}

Quantity<RB_STB> BobPetroskyFarshad__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
    const Quantity<SCF_STB>& rs, const Quantity<Fahrenheit>& t) {
  return BobPetroskyFarshad::get_instance()(yg, yo, rs, t);
}

double BobPetroskyFarshad__correlation__valid__fct(const double& yg,
                                                   const double& yo,
                                                   const double& rs,
                                                   const double& t) {
  return BobPetroskyFarshad::get_instance()(yg, yo, rs, t).raw();
}

Quantity<RB_STB> BobTotalCFP__correlation__fct(const Quantity<Sgg>& yg,
                                               const Quantity<Api>& api,
                                               const Quantity<SCF_STB>& rs,
                                               const Quantity<Fahrenheit>& t) {
  return BobTotalCFP::get_instance()(yg, api, rs, t);
}

double BobTotalCFP__correlation__valid__fct(const double& yg, const double& api,
                                            const double& rs, const double& t) {
  return BobTotalCFP::get_instance()(yg, api, rs, t).raw();
}

Quantity<RB_STB> BobVasquezBeggs__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<Api>& api,
    const Quantity<SCF_STB>& rs, const Quantity<Fahrenheit>& t,
    const Quantity<Fahrenheit>& tsep, const Quantity<psia>& psep) {
  return BobVasquezBeggs::get_instance()(yg, api, rs, t, tsep, psep);
}

double BobVasquezBeggs__correlation__valid__fct(
    const double& yg, const double& api, const double& rs, const double& t,
    const double& tsep, const double& psep) {
  return BobVasquezBeggs::get_instance()(yg, api, rs, t, tsep, psep).raw();
}

Quantity<RB_STB> BobPerezML__correlation__fct(const Quantity<Sgg>& yg,
                                              const Quantity<Sg_do>& yo,
                                              const Quantity<SCF_STB>& rsb,
                                              const Quantity<Fahrenheit>& t,
                                              const Quantity<psia>& p,
                                              const Quantity<psia>& pb) {
  return BobPerezML::get_instance()(yg, yo, rsb, t, p, pb);
}

double BobPerezML__correlation__valid__fct(const double& yg, const double& yo,
                                           const double& rsb, const double& t,
                                           const double& p, const double& pb) {
  return BobPerezML::get_instance()(yg, yo, rsb, t, p, pb).raw();
}

Quantity<RB_STB> BobVelardeMcCain__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
    const Quantity<SCF_STB>& rs, const Quantity<SCF_STB>& rsb,
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p) {
  return BobVelardeMcCain::get_instance()(yg, yo, rs, rsb, t, p);
}

double BobVelardeMcCain__correlation__valid__fct(
    const double& yg, const double& yo, const double& rs, const double& rsb,
    const double& t, const double& p) {
  return BobVelardeMcCain::get_instance()(yg, yo, rs, rsb, t, p).raw();
}

Quantity<RB_STB> BobMillanArcia__correlation__fct(const Quantity<Api>& api,
                                                  const Quantity<SCF_STB>& rsb,
                                                  const Quantity<psia>& p,
                                                  const Quantity<psia>& pb) {
  return BobMillanArcia::get_instance()(api, rsb, p, pb);
}

double BobMillanArcia__correlation__valid__fct(const double& api,
                                               const double& rsb,
                                               const double& p,
                                               const double& pb) {
  return BobMillanArcia::get_instance()(api, rsb, p, pb).raw();
}

Quantity<RB_STB> BobManucciRosales__correlation__fct(
    const Quantity<Api>& api, const Quantity<SCF_STB>& rs,
    const Quantity<Fahrenheit>& t) {
  return BobManucciRosales::get_instance()(api, rs, t);
}

double BobManucciRosales__correlation__valid__fct(const double& api,
                                                  const double& rs,
                                                  const double& t) {
  return BobManucciRosales::get_instance()(api, rs, t).raw();
}

Quantity<RB_STB> BobLopezCR__correlation__fct(const Quantity<Sgg>& yg,
                                              const Quantity<Api>& api,
                                              const Quantity<SCF_STB>& rs,
                                              const Quantity<Fahrenheit>& t) {
  return BobLopezCR::get_instance()(yg, api, rs, t);
}

double BobLopezCR__correlation__valid__fct(const double& yg, const double& api,
                                           const double& rs, const double& t) {
  return BobLopezCR::get_instance()(yg, api, rs, t).raw();
}

Quantity<RB_STB> BobSalazar__correlation__fct(const Quantity<Sgg>& yg,
                                              const Quantity<Api>& api,
                                              const Quantity<psia>& p) {
  return BobSalazar::get_instance()(yg, api, p);
}

double BobSalazar__correlation__valid__fct(const double& yg, const double& api,
                                           const double& p) {
  return BobSalazar::get_instance()(yg, api, p).raw();
}
