Quantity<dynes_cm> SgoBakerSwerdloff__correlation__fct(
    const Quantity<Fahrenheit>& t, const Quantity<Api>& api,
    const Quantity<psia>& p) {
  return SgoBakerSwerdloff::get_instance()(t, api, p);
}

double SgoBakerSwerdloff__correlation__valid__fct(const double& t,
                                                  const double& api,
                                                  const double& p) {
  return SgoBakerSwerdloff::get_instance()(t, api, p).raw();
}
