Quantity<mPa_1>
CwbSpiveyMN__correlation__fct(const Quantity<Celsius>& t,
                              const Quantity<mPascal>& p,
                              const Quantity<Molality_NaCl>& nacl_concentration,
                              const Quantity<ZFactor>& z)
{
  return CwbSpiveyMN::get_instance()(t, p, nacl_concentration, z);
}

double
CwbSpiveyMN__correlation__valid__fct(double t, double p,
                                     double nacl_concentration, double z)
{
  return CwbSpiveyMN::get_instance()(t, p, nacl_concentration, z).raw();
}

Quantity<psia_1>
CwbMcCain__correlation__fct(
  const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
  const Quantity<Dissolved_Salt_Percent>& nacl_concentration,
  const Quantity<RB_SCF>& bg, const Quantity<RB_STB>& bw,
  const Quantity<psia_1>& cwa)
{
  return CwbMcCain::get_instance()(t, p, nacl_concentration, bg, bw, cwa);
}

double
CwbMcCain__correlation__valid__fct(double t, double p,
                                   double nacl_concentration, double bg,
                                   double bw, double cwa)
{
  return CwbMcCain::get_instance()(t, p, nacl_concentration, bg, bw, cwa).raw();
}
