Quantity<Lb_ft3> Pg__correlation__fct(const Quantity<Sgg>& yg,
                                      const Quantity<Rankine>& t,
                                      const Quantity<psia>& p,
                                      const Quantity<Zfactor>& z) {
  return Pg::get_instance()(yg, t, p, z);
}

double Pg__correlation__valid__fct(const double& yg, const double& t,
                                   const double& p, const double& z) {
  return Pg::get_instance()(yg, t, p, z).raw();
}
