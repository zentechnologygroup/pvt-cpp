Quantity<Sm3_Sm3>
BwbSpiveyMN__correlation__fct(const Quantity<Celsius>& t,
                              const Quantity<mPascal>& p,
                              const Quantity<Molality_NaCl>& nacl_concentration)
{
  return BwbSpiveyMN::get_instance()(t, p, nacl_concentration);
}

double
BwbSpiveyMN__correlation__valid__fct(double t, double p,
                                     double nacl_concentration)
{
  return BwbSpiveyMN::get_instance()(t, p, nacl_concentration).raw();
}

Quantity<RB_STB>
BwbMcCain__correlation__fct(const Quantity<Fahrenheit>& t,
                            const Quantity<psia>& p)
{
  return BwbMcCain::get_instance()(t, p);
}

double
BwbMcCain__correlation__valid__fct(double t, double p)
{
  return BwbMcCain::get_instance()(t, p).raw();
}

Quantity<RB_STB>
BwbMcCoy__correlation__fct(
  const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
  const Quantity<Dissolved_Salt_Percent>& nacl_concentration)
{
  return BwbMcCoy::get_instance()(t, p, nacl_concentration);
}

double
BwbMcCoy__correlation__valid__fct(double t, double p, double nacl_concentration)
{
  return BwbMcCoy::get_instance()(t, p, nacl_concentration).raw();
}
