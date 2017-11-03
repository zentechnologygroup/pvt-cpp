Quantity<SCF_STB> GpaspMcCain__correlation__fct(
    const Quantity<Fahrenheit>& tsep, const Quantity<psia>& psep,
    const Quantity<Sgg>& yg, const Quantity<Api>& api) {
  return GpaspMcCain::get_instance()(tsep, psep, yg, api);
}

double GpaspMcCain__correlation__valid__fct(const double& tsep,
                                            const double& psep,
                                            const double& yg,
                                            const double& api) {
  return GpaspMcCain::get_instance()(tsep, psep, yg, api).raw();
}

Quantity<SCF_STB> Gpasp2McCain__correlation__fct(
    const Quantity<Fahrenheit>& tsep, const Quantity<Fahrenheit>& tsep2,
    const Quantity<psia>& psep, const Quantity<Sgg>& yg,
    const Quantity<Api>& api) {
  return Gpasp2McCain::get_instance()(tsep, tsep2, psep, yg, api);
}

double Gpasp2McCain__correlation__valid__fct(const double& tsep,
                                             const double& tsep2,
                                             const double& psep,
                                             const double& yg,
                                             const double& api) {
  return Gpasp2McCain::get_instance()(tsep, tsep2, psep, yg, api).raw();
}
