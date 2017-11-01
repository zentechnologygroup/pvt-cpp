Quantity<Sm3_Sm3> RswSpiveyMN__correlation__fct(
    const Quantity<Celsius>& t, const Quantity<mPascal>& p,
    const Quantity<Molality_NaCl>& nacl) {
  return RswSpiveyMN::get_instance()(t, p, nacl);
}

double RswSpiveyMN__correlation__valid__fct(const double& t, const double& p,
                                            const double& nacl) {
  return RswSpiveyMN::get_instance()(t, p, nacl).raw();
}

Quantity<SCF_STB> RswCulbersonMcKetta__correlation__fct(
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<Dissolved_Salt_Percent>& nacl) {
  return RswCulbersonMcKetta::get_instance()(t, p, nacl);
}

double RswCulbersonMcKetta__correlation__valid__fct(const double& t,
                                                    const double& p,
                                                    const double& nacl) {
  return RswCulbersonMcKetta::get_instance()(t, p, nacl).raw();
}

Quantity<SCF_STB> RswMcCoy__correlation__fct(
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<Dissolved_Salt_Percent>& nacl) {
  return RswMcCoy::get_instance()(t, p, nacl);
}

double RswMcCoy__correlation__valid__fct(const double& t, const double& p,
                                         const double& nacl) {
  return RswMcCoy::get_instance()(t, p, nacl).raw();
}
