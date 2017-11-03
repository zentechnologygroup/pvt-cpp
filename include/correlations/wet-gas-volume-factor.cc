Quantity<RCFGas_STB> Bwg__correlation__fct(const Quantity<Rankine>& t,
                                           const Quantity<psia>& p,
                                           const Quantity<Zfactor>& z,
                                           const Quantity<SCF_STB>& rsp1,
                                           const Quantity<SCF_STB>& veq) {
  return Bwg::get_instance()(t, p, z, rsp1, veq);
}

double Bwg__correlation__valid__fct(const double& t, const double& p,
                                    const double& z, const double& rsp1,
                                    const double& veq) {
  return Bwg::get_instance()(t, p, z, rsp1, veq).raw();
}
