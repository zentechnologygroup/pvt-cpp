Quantity<mPa_1> CwaSpiveyMN__correlation__fct(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p,
    const Quantity<Molality_NaCl>& nacl) {
  return CwaSpiveyMN::get_instance()(t, p, nacl);
}

double CwaSpiveyMN__correlation__valid__fct(const double& t, const double& p,
                                            const double& nacl) {
  return CwaSpiveyMN::get_instance()(t, p, nacl).raw();
}

Quantity<psia_1> CwaDodsonStanding__correlation__fct(
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<SCF_STB>& rsw,
    const Quantity<Dissolved_Salt_Percent>& nacl) {
  return CwaDodsonStanding::get_instance()(t, p, rsw, nacl);
}

double CwaDodsonStanding__correlation__valid__fct(const double& t,
                                                  const double& p,
                                                  const double& rsw,
                                                  const double& nacl) {
  return CwaDodsonStanding::get_instance()(t, p, rsw, nacl).raw();
}

Quantity<psia_1> CwaOsif__correlation__fct(const Quantity<Fahrenheit>& t,
                                           const Quantity<psia>& p,
                                           const Quantity<CgL>& nacl) {
  return CwaOsif::get_instance()(t, p, nacl);
}

double CwaOsif__correlation__valid__fct(const double& t, const double& p,
                                        const double& nacl) {
  return CwaOsif::get_instance()(t, p, nacl).raw();
}
