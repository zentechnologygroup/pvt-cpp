Quantity<Gr_cm3> PwSpiveyMN__correlation__fct(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p,
    const Quantity<Molality_NaCl>& nacl) {
  return PwSpiveyMN::get_instance()(t, p, nacl);
}

double PwSpiveyMN__correlation__valid__fct(const double& t, const double& p,
                                           const double& nacl) {
  return PwSpiveyMN::get_instance()(t, p, nacl).raw();
}

Quantity<Gr_cm3> PwSpiveyMNGasFree__correlation__fct(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p,
    const Quantity<Molality_NaCl>& nacl) {
  return PwSpiveyMNGasFree::get_instance()(t, p, nacl);
}

double PwSpiveyMNGasFree__correlation__valid__fct(const double& t,
                                                  const double& p,
                                                  const double& nacl) {
  return PwSpiveyMNGasFree::get_instance()(t, p, nacl).raw();
}

Quantity<Lb_ft3> PwMcCain__correlation__fct(
    const Quantity<Dissolved_Salt_Percent>& nacl, const Quantity<RB_STB>& bw) {
  return PwMcCain::get_instance()(nacl, bw);
}

double PwMcCain__correlation__valid__fct(const double& nacl, const double& bw) {
  return PwMcCain::get_instance()(nacl, bw).raw();
}

Quantity<Gr_cm3> PpwSpiveyMN__correlation__fct(const Quantity<Celsius>& t,
                                               const Quantity<mPascal>& p) {
  return PpwSpiveyMN::get_instance()(t, p);
}

double PpwSpiveyMN__correlation__valid__fct(const double& t, const double& p) {
  return PpwSpiveyMN::get_instance()(t, p).raw();
}
