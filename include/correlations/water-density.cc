Quantity<Gr_cm3>
PwSpiveyMN__correlation__fct(const Quantity<Celsius>& t,
                             const Quantity<mPascal>& p,
                             const Quantity<Molality_NaCl>& nacl_concentration)
{
  return PwSpiveyMN::get_instance()(t, p, nacl_concentration);
}

double
PwSpiveyMN__correlation__valid__fct(double t, double p,
                                    double nacl_concentration)
{
  return PwSpiveyMN::get_instance()(t, p, nacl_concentration).raw();
}

Quantity<Gr_cm3>
PwSpiveyMNGasFree__correlation__fct(
  const Quantity<Celsius>& t, const Quantity<mPascal>& p,
  const Quantity<Molality_NaCl>& nacl_concentration)
{
  return PwSpiveyMNGasFree::get_instance()(t, p, nacl_concentration);
}

double
PwSpiveyMNGasFree__correlation__valid__fct(double t, double p,
                                           double nacl_concentration)
{
  return PwSpiveyMNGasFree::get_instance()(t, p, nacl_concentration).raw();
}

Quantity<Lb_ft3>
PwMcCain__correlation__fct(
  const Quantity<Dissolved_Salt_Percent>& nacl_concentration,
  const Quantity<RB_STB>& bw)
{
  return PwMcCain::get_instance()(nacl_concentration, bw);
}

double
PwMcCain__correlation__valid__fct(double nacl_concentration, double bw)
{
  return PwMcCain::get_instance()(nacl_concentration, bw).raw();
}

Quantity<Gr_cm3>
PpwSpiveyMN__correlation__fct(const Quantity<Celsius>& t,
                              const Quantity<mPascal>& p)
{
  return PpwSpiveyMN::get_instance()(t, p);
}

double
PpwSpiveyMN__correlation__valid__fct(double t, double p)
{
  return PpwSpiveyMN::get_instance()(t, p).raw();
}
