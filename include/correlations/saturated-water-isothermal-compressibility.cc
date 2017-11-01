Quantity<mPa_1> CwbSpiveyMN__correlation__fct(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p,
    const Quantity<Molality_NaCl>& nacl, const Quantity<Zfactor>& z) {
  return CwbSpiveyMN::get_instance()(t, p, nacl, z);
}

double CwbSpiveyMN__correlation__valid__fct(const double& t, const double& p,
                                            const double& nacl,
                                            const double& z) {
  return CwbSpiveyMN::get_instance()(t, p, nacl, z).raw();
}

Quantity<psia_1> CwbMcCain__correlation__fct(
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<Dissolved_Salt_Percent>& nacl, const Quantity<RB_SCF>& bg,
    const Quantity<RB_STB>& bw, const Quantity<psia_1>& cwa) {
  return CwbMcCain::get_instance()(t, p, nacl, bg, bw, cwa);
}

double CwbMcCain__correlation__valid__fct(const double& t, const double& p,
                                          const double& nacl, const double& bg,
                                          const double& bw, const double& cwa) {
  return CwbMcCain::get_instance()(t, p, nacl, bg, bw, cwa).raw();
}

Quantity<psia_1> CwbMcCainOsif__correlation__fct(
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<Dissolved_Salt_Percent>& nacl, const Quantity<RB_SCF>& bg,
    const Quantity<RB_STB>& bw) {
  return CwbMcCainOsif::get_instance()(t, p, nacl, bg, bw);
}

double CwbMcCainOsif__correlation__valid__fct(const double& t, const double& p,
                                              const double& nacl,
                                              const double& bg,
                                              const double& bw) {
  return CwbMcCainOsif::get_instance()(t, p, nacl, bg, bw).raw();
}
