Quantity<Sm3_Sm3>
BwaSpiveyMN__correlation__fct(const Quantity<Celsius>& t,
                              const Quantity<mPascal>& p,
                              const Quantity<Molality_NaCl>& nacl_concentration)
{
  return BwaSpiveyMN::get_instance()(t, p, nacl_concentration);
}

double
BwaSpiveyMN__correlation__valid__fct(double t, double p,
                                     double nacl_concentration)
{
  return BwaSpiveyMN::get_instance()(t, p, nacl_concentration).raw();
}

Quantity<RB_STB>
BwaMcCain__correlation__fct(const Quantity<psia>& p, const Quantity<psia>& pb,
                            const Quantity<RB_STB>& bwbp,
                            const Quantity<psia_1>& cw)
{
  return BwaMcCain::get_instance()(p, pb, bwbp, cw);
}

double
BwaMcCain__correlation__valid__fct(double p, double pb, double bwbp, double cw)
{
  return BwaMcCain::get_instance()(p, pb, bwbp, cw).raw();
}
