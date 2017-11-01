Quantity<Lb_ft3> PoaBradley__correlation__fct(const Quantity<Lb_ft3>& pobp,
                                              const Quantity<psia>& p,
                                              const Quantity<psia>& pb,
                                              const Quantity<psia_1>& coa) {
  return PoaBradley::get_instance()(pobp, p, pb, coa);
}

double PoaBradley__correlation__valid__fct(const double& pobp, const double& p,
                                           const double& pb,
                                           const double& coa) {
  return PoaBradley::get_instance()(pobp, p, pb, coa).raw();
}
