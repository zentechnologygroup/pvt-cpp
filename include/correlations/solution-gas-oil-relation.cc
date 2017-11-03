Quantity<SCF_STB> RsAlShammasi__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Sg_do>& yo,
    const Quantity<Fahrenheit>& t) {
  return RsAlShammasi::get_instance()(yg, p, yo, t);
}

double RsAlShammasi__correlation__valid__fct(const double& yg, const double& p,
                                             const double& yo,
                                             const double& t) {
  return RsAlShammasi::get_instance()(yg, p, yo, t).raw();
}

Quantity<SCF_STB> RsAlShammasiCalibration__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Sg_do>& yo,
    const Quantity<Fahrenheit>& t, const Quantity<Dim_Less>& c,
    const Quantity<Dim_Less>& m) {
  return RsAlShammasiCalibration::get_instance()(yg, p, yo, t, c, m);
}

double RsAlShammasiCalibration__correlation__valid__fct(
    const double& yg, const double& p, const double& yo, const double& t,
    const double& c, const double& m) {
  return RsAlShammasiCalibration::get_instance()(yg, p, yo, t, c, m).raw();
}

Quantity<SCF_STB> RsAlMarhoun__correlation__fct(const Quantity<Sgg>& yg,
                                                const Quantity<psia>& p,
                                                const Quantity<Sg_do>& yo,
                                                const Quantity<Rankine>& t) {
  return RsAlMarhoun::get_instance()(yg, p, yo, t);
}

double RsAlMarhoun__correlation__valid__fct(const double& yg, const double& p,
                                            const double& yo, const double& t) {
  return RsAlMarhoun::get_instance()(yg, p, yo, t).raw();
}

Quantity<SCF_STB> RsDeGhetto__correlation__fct(const Quantity<Sgg>& yg,
                                               const Quantity<psia>& p,
                                               const Quantity<Api>& api,
                                               const Quantity<Fahrenheit>& t,
                                               const Quantity<Fahrenheit>& tsep,
                                               const Quantity<psia>& psep) {
  return RsDeGhetto::get_instance()(yg, p, api, t, tsep, psep);
}

double RsDeGhetto__correlation__valid__fct(const double& yg, const double& p,
                                           const double& api, const double& t,
                                           const double& tsep,
                                           const double& psep) {
  return RsDeGhetto::get_instance()(yg, p, api, t, tsep, psep).raw();
}

Quantity<SCF_STB> RsDindorukChristman__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
    const Quantity<Fahrenheit>& t) {
  return RsDindorukChristman::get_instance()(yg, p, api, t);
}

double RsDindorukChristman__correlation__valid__fct(const double& yg,
                                                    const double& p,
                                                    const double& api,
                                                    const double& t) {
  return RsDindorukChristman::get_instance()(yg, p, api, t).raw();
}

Quantity<SCF_STB> RsDoklaOsman__correlation__fct(const Quantity<Sgg>& yg,
                                                 const Quantity<psia>& p,
                                                 const Quantity<Sg_do>& yo,
                                                 const Quantity<Rankine>& t) {
  return RsDoklaOsman::get_instance()(yg, p, yo, t);
}

double RsDoklaOsman__correlation__valid__fct(const double& yg, const double& p,
                                             const double& yo,
                                             const double& t) {
  return RsDoklaOsman::get_instance()(yg, p, yo, t).raw();
}

Quantity<SCF_STB> RsGlaso__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
    const Quantity<Fahrenheit>& t, const Quantity<MoleFraction>& n2,
    const Quantity<MoleFraction>& co2, const Quantity<MoleFraction>& h2s) {
  return RsGlaso::get_instance()(yg, p, api, t, n2, co2, h2s);
}

double RsGlaso__correlation__valid__fct(const double& yg, const double& p,
                                        const double& api, const double& t,
                                        const double& n2, const double& co2,
                                        const double& h2s) {
  return RsGlaso::get_instance()(yg, p, api, t, n2, co2, h2s).raw();
}

Quantity<SCF_STB> RsHanafy__correlation__fct(const Quantity<psia>& p) {
  return RsHanafy::get_instance()(p);
}

double RsHanafy__correlation__valid__fct(const double& p) {
  return RsHanafy::get_instance()(p).raw();
}

Quantity<SCF_STB> RsKartoatmodjoSchmidt__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
    const Quantity<Fahrenheit>& t, const Quantity<Fahrenheit>& tsep,
    const Quantity<psia>& psep) {
  return RsKartoatmodjoSchmidt::get_instance()(yg, p, api, t, tsep, psep);
}

double RsKartoatmodjoSchmidt__correlation__valid__fct(
    const double& yg, const double& p, const double& api, const double& t,
    const double& tsep, const double& psep) {
  return RsKartoatmodjoSchmidt::get_instance()(yg, p, api, t, tsep, psep).raw();
}

Quantity<SCF_STB> RsLasater__correlation__fct(const Quantity<Sgg>& yg,
                                              const Quantity<psia>& p,
                                              const Quantity<Sg_do>& yo,
                                              const Quantity<Rankine>& t) {
  return RsLasater::get_instance()(yg, p, yo, t);
}

double RsLasater__correlation__valid__fct(const double& yg, const double& p,
                                          const double& yo, const double& t) {
  return RsLasater::get_instance()(yg, p, yo, t).raw();
}

Quantity<SCF_STB> RsPetroskyFarshad__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
    const Quantity<Fahrenheit>& t) {
  return RsPetroskyFarshad::get_instance()(yg, p, api, t);
}

double RsPetroskyFarshad__correlation__valid__fct(const double& yg,
                                                  const double& p,
                                                  const double& api,
                                                  const double& t) {
  return RsPetroskyFarshad::get_instance()(yg, p, api, t).raw();
}

Quantity<SCF_STB> RsStanding__correlation__fct(const Quantity<Sgg>& yg,
                                               const Quantity<psia>& p,
                                               const Quantity<Api>& api,
                                               const Quantity<Fahrenheit>& t) {
  return RsStanding::get_instance()(yg, p, api, t);
}

double RsStanding__correlation__valid__fct(const double& yg, const double& p,
                                           const double& api, const double& t) {
  return RsStanding::get_instance()(yg, p, api, t).raw();
}

Quantity<SCF_STB> RsTotalCFPDetachedFromPb__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
    const Quantity<Fahrenheit>& t) {
  return RsTotalCFPDetachedFromPb::get_instance()(yg, p, api, t);
}

double RsTotalCFPDetachedFromPb__correlation__valid__fct(const double& yg,
                                                         const double& p,
                                                         const double& api,
                                                         const double& t) {
  return RsTotalCFPDetachedFromPb::get_instance()(yg, p, api, t).raw();
}

Quantity<SCF_STB> RsTotalCFP__correlation__fct(const Quantity<Sgg>& yg,
                                               const Quantity<psia>& p,
                                               const Quantity<Api>& api,
                                               const Quantity<Fahrenheit>& t) {
  return RsTotalCFP::get_instance()(yg, p, api, t);
}

double RsTotalCFP__correlation__valid__fct(const double& yg, const double& p,
                                           const double& api, const double& t) {
  return RsTotalCFP::get_instance()(yg, p, api, t).raw();
}

Quantity<SCF_STB> RsVasquezBeggs__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
    const Quantity<Fahrenheit>& t, const Quantity<Fahrenheit>& tsep,
    const Quantity<psia>& psep) {
  return RsVasquezBeggs::get_instance()(yg, p, api, t, tsep, psep);
}

double RsVasquezBeggs__correlation__valid__fct(
    const double& yg, const double& p, const double& api, const double& t,
    const double& tsep, const double& psep) {
  return RsVasquezBeggs::get_instance()(yg, p, api, t, tsep, psep).raw();
}

Quantity<SCF_STB> RsVelarde__correlation__fct(const Quantity<Sgg>& yg,
                                              const Quantity<psig>& pb,
                                              const Quantity<psig>& p,
                                              const Quantity<Api>& api,
                                              const Quantity<Fahrenheit>& t,
                                              const Quantity<SCF_STB>& rsb) {
  return RsVelarde::get_instance()(yg, pb, p, api, t, rsb);
}

double RsVelarde__correlation__valid__fct(const double& yg, const double& pb,
                                          const double& p, const double& api,
                                          const double& t, const double& rsb) {
  return RsVelarde::get_instance()(yg, pb, p, api, t, rsb).raw();
}

Quantity<SCF_STB> RsCegarra__correlation__fct(const Quantity<Sgg>& yg,
                                              const Quantity<psia>& p,
                                              const Quantity<Api>& api,
                                              const Quantity<Fahrenheit>& t) {
  return RsCegarra::get_instance()(yg, p, api, t);
}

double RsCegarra__correlation__valid__fct(const double& yg, const double& p,
                                          const double& api, const double& t) {
  return RsCegarra::get_instance()(yg, p, api, t).raw();
}

Quantity<SCF_STB> RsPerezML__correlation__fct(const Quantity<psia>& pb,
                                              const Quantity<psia>& p,
                                              const Quantity<SCF_STB>& rsb) {
  return RsPerezML::get_instance()(pb, p, rsb);
}

double RsPerezML__correlation__valid__fct(const double& pb, const double& p,
                                          const double& rsb) {
  return RsPerezML::get_instance()(pb, p, rsb).raw();
}

Quantity<SCF_STB> RsMillanArciaDetachedFromPb__correlation__fct(
    const Quantity<psia>& pb, const Quantity<psia>& p,
    const Quantity<SCF_STB>& rsb) {
  return RsMillanArciaDetachedFromPb::get_instance()(pb, p, rsb);
}

double RsMillanArciaDetachedFromPb__correlation__valid__fct(const double& pb,
                                                            const double& p,
                                                            const double& rsb) {
  return RsMillanArciaDetachedFromPb::get_instance()(pb, p, rsb).raw();
}

Quantity<SCF_STB> RsMillanArcia__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
    const Quantity<Fahrenheit>& t) {
  return RsMillanArcia::get_instance()(yg, p, api, t);
}

double RsMillanArcia__correlation__valid__fct(const double& yg, const double& p,
                                              const double& api,
                                              const double& t) {
  return RsMillanArcia::get_instance()(yg, p, api, t).raw();
}

Quantity<SCF_STB> RsManucciRosales__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
    const Quantity<Fahrenheit>& t) {
  return RsManucciRosales::get_instance()(yg, p, api, t);
}

double RsManucciRosales__correlation__valid__fct(const double& yg,
                                                 const double& p,
                                                 const double& api,
                                                 const double& t) {
  return RsManucciRosales::get_instance()(yg, p, api, t).raw();
}

Quantity<SCF_STB> RsLopezCR__correlation__fct(const Quantity<Sgg>& yg,
                                              const Quantity<psia>& p,
                                              const Quantity<Api>& api,
                                              const Quantity<Fahrenheit>& t) {
  return RsLopezCR::get_instance()(yg, p, api, t);
}

double RsLopezCR__correlation__valid__fct(const double& yg, const double& p,
                                          const double& api, const double& t) {
  return RsLopezCR::get_instance()(yg, p, api, t).raw();
}

Quantity<SCF_STB> RsSalazar__correlation__fct(const Quantity<Sgg>& yg,
                                              const Quantity<psia>& p,
                                              const Quantity<Api>& api,
                                              const Quantity<Fahrenheit>& t) {
  return RsSalazar::get_instance()(yg, p, api, t);
}

double RsSalazar__correlation__valid__fct(const double& yg, const double& p,
                                          const double& api, const double& t) {
  return RsSalazar::get_instance()(yg, p, api, t).raw();
}

Quantity<SCF_STB> RsSalazarDetachedFromPb__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<psia>& p, const Quantity<Api>& api,
    const Quantity<Fahrenheit>& t) {
  return RsSalazarDetachedFromPb::get_instance()(yg, p, api, t);
}

double RsSalazarDetachedFromPb__correlation__valid__fct(const double& yg,
                                                        const double& p,
                                                        const double& api,
                                                        const double& t) {
  return RsSalazarDetachedFromPb::get_instance()(yg, p, api, t).raw();
}

Quantity<SCF_STB> RsAbovePb__correlation__fct(const Quantity<psia>& pb,
                                              const Quantity<psia>& p,
                                              const Quantity<SCF_STB>& rsb) {
  return RsAbovePb::get_instance()(pb, p, rsb);
}

double RsAbovePb__correlation__valid__fct(const double& pb, const double& p,
                                          const double& rsb) {
  return RsAbovePb::get_instance()(pb, p, rsb).raw();
}
