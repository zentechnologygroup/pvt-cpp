Quantity<mPascal>
PvSpiveyMN__correlation__fct(const Quantity<Kelvin>& t)
{
  return PvSpiveyMN::get_instance()(t);
}

double
PvSpiveyMN__correlation__valid__fct(double t)
{
  return PvSpiveyMN::get_instance()(t).raw();
}

Quantity<Molality_NaCl>
MwSpiveyMN__correlation__fct(const Quantity<Celsius>& t,
                             const Quantity<mPascal>& p,
                             const Quantity<mPascal>& pv,
                             const Quantity<Molality_NaCl>& nacl_concentration)
{
  return MwSpiveyMN::get_instance()(t, p, pv, nacl_concentration);
}

double
MwSpiveyMN__correlation__valid__fct(double t, double p, double pv,
                                    double nacl_concentration)
{
  return MwSpiveyMN::get_instance()(t, p, pv, nacl_concentration).raw();
}

Quantity<mPa_1>
CgfwSpiveyMN__correlation__fct(
  const Quantity<Celsius>& t, const Quantity<mPascal>& p,
  const Quantity<Molality_NaCl>& nacl_concentration)
{
  return CgfwSpiveyMN::get_instance()(t, p, nacl_concentration);
}

double
CgfwSpiveyMN__correlation__valid__fct(double t, double p,
                                      double nacl_concentration)
{
  return CgfwSpiveyMN::get_instance()(t, p, nacl_concentration).raw();
}

Quantity<Sm3_Sm3>
BwSpiveyMNDry__correlation__fct(
  const Quantity<Celsius>& t, const Quantity<mPascal>& p,
  const Quantity<mPascal>& pv,
  const Quantity<Molality_NaCl>& nacl_concentration)
{
  return BwSpiveyMNDry::get_instance()(t, p, pv, nacl_concentration);
}

double
BwSpiveyMNDry__correlation__valid__fct(double t, double p, double pv,
                                       double nacl_concentration)
{
  return BwSpiveyMNDry::get_instance()(t, p, pv, nacl_concentration).raw();
}
