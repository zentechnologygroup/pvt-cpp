Quantity<psia_1> CobMcCainEtAl__correlation__fct(const Quantity<Api>& api,
                                                 const Quantity<SCF_STB>& rsb,
                                                 const Quantity<Rankine>& t,
                                                 const Quantity<psia>& p,
                                                 const Quantity<psia>& pb) {
  return CobMcCainEtAl::get_instance()(api, rsb, t, p, pb);
}

double CobMcCainEtAl__correlation__valid__fct(const double& api,
                                              const double& rsb,
                                              const double& t, const double& p,
                                              const double& pb) {
  return CobMcCainEtAl::get_instance()(api, rsb, t, p, pb).raw();
}
