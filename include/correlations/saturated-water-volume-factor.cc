Quantity<Rm3_Sm3> BwbSpiveyMN__correlation__fct(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p,
    const Quantity<Molality_NaCl>& nacl) {
  return BwbSpiveyMN::get_instance()(t, p, nacl);
}

double BwbSpiveyMN__correlation__valid__fct(const double& t, const double& p,
                                            const double& nacl) {
  return BwbSpiveyMN::get_instance()(t, p, nacl).raw();
}

Quantity<RB_STB> BwbMcCain__correlation__fct(const Quantity<Fahrenheit>& t,
                                             const Quantity<psia>& p) {
  return BwbMcCain::get_instance()(t, p);
}

double BwbMcCain__correlation__valid__fct(const double& t, const double& p) {
  return BwbMcCain::get_instance()(t, p).raw();
}

Quantity<RB_STB> BwbMcCoy__correlation__fct(
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<Dissolved_Salt_Percent>& nacl) {
  return BwbMcCoy::get_instance()(t, p, nacl);
}

double BwbMcCoy__correlation__valid__fct(const double& t, const double& p,
                                         const double& nacl) {
  return BwbMcCoy::get_instance()(t, p, nacl).raw();
}
