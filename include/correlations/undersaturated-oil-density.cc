Quantity<Lb_ft3>
PoaBradley__correlation__fct(const Quantity<Lb_ft3>& pobp,
                             const Quantity<psia>& p, const Quantity<psia>& pb,
                             const Quantity<psia_1>& co)
{
  return PoaBradley::get_instance()(pobp, p, pb, co);
}

double
PoaBradley__correlation__valid__fct(double pobp, double p, double pb, double co)
{
  return PoaBradley::get_instance()(pobp, p, pb, co).raw();
}
