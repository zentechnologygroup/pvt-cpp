Quantity<Lb_ft3>
Pg__correlation__fct(const Quantity<Sgg>& yg, const Quantity<psia>& pb,
                     const Quantity<Rankine>& t, const Quantity<psia>& p,
                     const Quantity<ZFactor>& z)
{
  return Pg::get_instance()(yg, pb, t, p, z);
}

double
Pg__correlation__valid__fct(double yg, double pb, double t, double p, double z)
{
  return Pg::get_instance()(yg, pb, t, p, z).raw();
}
