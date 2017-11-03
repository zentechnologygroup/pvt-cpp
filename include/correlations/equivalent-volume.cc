Quantity<SCF_STB> VeqspMcCain__correlation__fct(
    const Quantity<Fahrenheit>& tsep, const Quantity<psia>& psep,
    const Quantity<Sgg>& yg, const Quantity<Api>& api) {
  return VeqspMcCain::get_instance()(tsep, psep, yg, api);
}

double VeqspMcCain__correlation__valid__fct(const double& tsep,
                                            const double& psep,
                                            const double& yg,
                                            const double& api) {
  return VeqspMcCain::get_instance()(tsep, psep, yg, api).raw();
}

Quantity<SCF_STB> Veqsp2McCain__correlation__fct(
    const Quantity<Fahrenheit>& tsep, const Quantity<Fahrenheit>& tsep2,
    const Quantity<psia>& psep, const Quantity<Sgg>& yg,
    const Quantity<Api>& api) {
  return Veqsp2McCain::get_instance()(tsep, tsep2, psep, yg, api);
}

double Veqsp2McCain__correlation__valid__fct(const double& tsep,
                                             const double& tsep2,
                                             const double& psep,
                                             const double& yg,
                                             const double& api) {
  return Veqsp2McCain::get_instance()(tsep, tsep2, psep, yg, api).raw();
}
