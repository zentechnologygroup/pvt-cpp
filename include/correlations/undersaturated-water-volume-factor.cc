Quantity<Rm3_Sm3> BwaSpiveyMN__correlation__fct(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p,
    const Quantity<Molality_NaCl>& nacl) {
  return BwaSpiveyMN::get_instance()(t, p, nacl);
}

double BwaSpiveyMN__correlation__valid__fct(const double& t, const double& p,
                                            const double& nacl) {
  return BwaSpiveyMN::get_instance()(t, p, nacl).raw();
}

Quantity<RB_STB> BwaMcCain__correlation__fct(const Quantity<psia>& p,
                                             const Quantity<psia>& pb,
                                             const Quantity<RB_STB>& bwbp,
                                             const Quantity<psia_1>& cwa) {
  return BwaMcCain::get_instance()(p, pb, bwbp, cwa);
}

double BwaMcCain__correlation__valid__fct(const double& p, const double& pb,
                                          const double& bwbp,
                                          const double& cwa) {
  return BwaMcCain::get_instance()(p, pb, bwbp, cwa).raw();
}
