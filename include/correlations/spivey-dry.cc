Quantity<mPascal> PvSpiveyMN__correlation__fct(const Quantity<Kelvin>& t) {
  return PvSpiveyMN::get_instance()(t);
}

double PvSpiveyMN__correlation__valid__fct(const double& t) {
  return PvSpiveyMN::get_instance()(t).raw();
}

Quantity<Molality_NaCl> MwSpiveyMN__correlation__fct(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p,
    const Quantity<mPascal>& pv, const Quantity<Molality_NaCl>& nacl) {
  return MwSpiveyMN::get_instance()(t, p, pv, nacl);
}

double MwSpiveyMN__correlation__valid__fct(const double& t, const double& p,
                                           const double& pv,
                                           const double& nacl) {
  return MwSpiveyMN::get_instance()(t, p, pv, nacl).raw();
}

Quantity<mPa_1> CgfwSpiveyMN__correlation__fct(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p,
    const Quantity<Molality_NaCl>& nacl) {
  return CgfwSpiveyMN::get_instance()(t, p, nacl);
}

double CgfwSpiveyMN__correlation__valid__fct(const double& t, const double& p,
                                             const double& nacl) {
  return CgfwSpiveyMN::get_instance()(t, p, nacl).raw();
}

Quantity<Rm3_Sm3> BwSpiveyMNDry__correlation__fct(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p,
    const Quantity<mPascal>& pv, const Quantity<Molality_NaCl>& nacl) {
  return BwSpiveyMNDry::get_instance()(t, p, pv, nacl);
}

double BwSpiveyMNDry__correlation__valid__fct(const double& t, const double& p,
                                              const double& pv,
                                              const double& nacl) {
  return BwSpiveyMNDry::get_instance()(t, p, pv, nacl).raw();
}
