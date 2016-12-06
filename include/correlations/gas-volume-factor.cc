Quantity<RCF_SCF>
Bg__correlation__fct(const Quantity<Rankine>& t, const Quantity<psia>& p,
                     const Quantity<ZFactor>& z)
{
  return Bg::get_instance()(t, p, z);
}

double
Bg__correlation__valid__fct(double t, double p, double z)
{
  return Bg::get_instance()(t, p, z).raw();
}
