Quantity<RB_STB> BoaMcCain__correlation__fct(const Quantity<RB_STB>& bobp,
                                             const Quantity<psia>& p,
                                             const Quantity<psia>& pb,
                                             const Quantity<psia_1>& coa) {
  return BoaMcCain::get_instance()(bobp, p, pb, coa);
}

double BoaMcCain__correlation__valid__fct(const double& bobp, const double& p,
                                          const double& pb, const double& coa) {
  return BoaMcCain::get_instance()(bobp, p, pb, coa).raw();
}

Quantity<RB_STB> BoaDeGhetto__correlation__fct(
    const Quantity<RB_STB>& bobp, const Quantity<Sgg>& yg,
    const Quantity<Api>& api, const Quantity<SCF_STB>& rsb,
    const Quantity<Fahrenheit>& t, const Quantity<Fahrenheit>& tsep,
    const Quantity<psia>& p, const Quantity<psia>& pb,
    const Quantity<psia>& psep) {
  return BoaDeGhetto::get_instance()(bobp, yg, api, rsb, t, tsep, p, pb, psep);
}

double BoaDeGhetto__correlation__valid__fct(const double& bobp,
                                            const double& yg, const double& api,
                                            const double& rsb, const double& t,
                                            const double& tsep, const double& p,
                                            const double& pb,
                                            const double& psep) {
  return BoaDeGhetto::get_instance()(bobp, yg, api, rsb, t, tsep, p, pb, psep)
      .raw();
}

Quantity<RB_STB> BoaHanafy__correlation__fct(const Quantity<RB_STB>& bobp,
                                             const Quantity<psia>& p,
                                             const Quantity<psia>& pb) {
  return BoaHanafy::get_instance()(bobp, p, pb);
}

double BoaHanafy__correlation__valid__fct(const double& bobp, const double& p,
                                          const double& pb) {
  return BoaHanafy::get_instance()(bobp, p, pb).raw();
}

Quantity<RB_STB> BoaKartoatmodjoSchmidt__correlation__fct(
    const Quantity<RB_STB>& bobp, const Quantity<Sgg>& yg,
    const Quantity<Sg_do>& yo, const Quantity<SCF_STB>& rsb,
    const Quantity<Fahrenheit>& t, const Quantity<Fahrenheit>& tsep,
    const Quantity<psia>& p, const Quantity<psia>& pb,
    const Quantity<psia>& psep) {
  return BoaKartoatmodjoSchmidt::get_instance()(bobp, yg, yo, rsb, t, tsep, p,
                                                pb, psep);
}

double BoaKartoatmodjoSchmidt__correlation__valid__fct(
    const double& bobp, const double& yg, const double& yo, const double& rsb,
    const double& t, const double& tsep, const double& p, const double& pb,
    const double& psep) {
  return BoaKartoatmodjoSchmidt::get_instance()(bobp, yg, yo, rsb, t, tsep, p,
                                                pb, psep).raw();
}

Quantity<RB_STB> BoaPetroskyFarshad__correlation__fct(
    const Quantity<RB_STB>& bobp, const Quantity<Sgg>& yg,
    const Quantity<Sg_do>& yo, const Quantity<SCF_STB>& rsb,
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<psia>& pb) {
  return BoaPetroskyFarshad::get_instance()(bobp, yg, yo, rsb, t, p, pb);
}

double BoaPetroskyFarshad__correlation__valid__fct(
    const double& bobp, const double& yg, const double& yo, const double& rsb,
    const double& t, const double& p, const double& pb) {
  return BoaPetroskyFarshad::get_instance()(bobp, yg, yo, rsb, t, p, pb).raw();
}

Quantity<RB_STB> BoaVasquezBeggs__correlation__fct(
    const Quantity<RB_STB>& bobp, const Quantity<Sgg>& yg,
    const Quantity<Api>& api, const Quantity<SCF_STB>& rsb,
    const Quantity<Fahrenheit>& t, const Quantity<Fahrenheit>& tsep,
    const Quantity<psia>& p, const Quantity<psia>& pb,
    const Quantity<psia>& psep) {
  return BoaVasquezBeggs::get_instance()(bobp, yg, api, rsb, t, tsep, p, pb,
                                         psep);
}

double BoaVasquezBeggs__correlation__valid__fct(
    const double& bobp, const double& yg, const double& api, const double& rsb,
    const double& t, const double& tsep, const double& p, const double& pb,
    const double& psep) {
  return BoaVasquezBeggs::get_instance()(bobp, yg, api, rsb, t, tsep, p, pb,
                                         psep).raw();
}

Quantity<RB_STB> BoaPerezML__correlation__fct(const Quantity<RB_STB>& bobp,
                                              const Quantity<Sgg>& yg,
                                              const Quantity<Sg_do>& yo,
                                              const Quantity<SCF_STB>& rsb,
                                              const Quantity<Fahrenheit>& t,
                                              const Quantity<psia>& p,
                                              const Quantity<psia>& pb) {
  return BoaPerezML::get_instance()(bobp, yg, yo, rsb, t, p, pb);
}

double BoaPerezML__correlation__valid__fct(const double& bobp, const double& yg,
                                           const double& yo, const double& rsb,
                                           const double& t, const double& p,
                                           const double& pb) {
  return BoaPerezML::get_instance()(bobp, yg, yo, rsb, t, p, pb).raw();
}

Quantity<RB_STB> BoaMillanArcia__correlation__fct(const Quantity<RB_STB>& bobp,
                                                  const Quantity<Api>& api,
                                                  const Quantity<SCF_STB>& rsb,
                                                  const Quantity<Fahrenheit>& t,
                                                  const Quantity<psia>& p,
                                                  const Quantity<psia>& pb) {
  return BoaMillanArcia::get_instance()(bobp, api, rsb, t, p, pb);
}

double BoaMillanArcia__correlation__valid__fct(const double& bobp,
                                               const double& api,
                                               const double& rsb,
                                               const double& t, const double& p,
                                               const double& pb) {
  return BoaMillanArcia::get_instance()(bobp, api, rsb, t, p, pb).raw();
}
