Quantity<RCF_SCF> Bg__correlation__fct(const Quantity<Rankine>& t,
                                       const Quantity<psia>& p,
                                       const Quantity<Zfactor>& z) {
  return Bg::get_instance()(t, p, z);
}

double Bg__correlation__valid__fct(const double& t, const double& p,
                                   const double& z) {
  return Bg::get_instance()(t, p, z).raw();
}
