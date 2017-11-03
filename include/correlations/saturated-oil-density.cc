Quantity<Lb_ft3> PobBradley__correlation__fct(const Quantity<Sgg>& yg,
                                              const Quantity<SCF_STB>& rs,
                                              const Quantity<RB_STB>& bob,
                                              const Quantity<Sg_do>& yo) {
  return PobBradley::get_instance()(yg, rs, bob, yo);
}

double PobBradley__correlation__valid__fct(const double& yg, const double& rs,
                                           const double& bob,
                                           const double& yo) {
  return PobBradley::get_instance()(yg, rs, bob, yo).raw();
}
