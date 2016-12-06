Quantity<mPa_1>
CwSpiveyMN__correlation__fct(const Quantity<Celsius>& t,
                             const Quantity<mPascal>& p,
                             const Quantity<mPascal>& pb,
                             const Quantity<Molality_NaCl>& nacl_concentration,
                             const Quantity<ZFactor>& z)
{
  return CwSpiveyMN::get_instance()(t, p, pb, nacl_concentration, z);
}

double
CwSpiveyMN__correlation__valid__fct(double t, double p, double pb,
                                    double nacl_concentration, double z)
{
  return CwSpiveyMN::get_instance()(t, p, pb, nacl_concentration, z).raw();
}

Quantity<psia_1>
CwDodsonStanding__correlation__fct(
  const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
  const Quantity<SCF_STB>& rsw,
  const Quantity<Dissolved_Salt_Percent>& nacl_concentration)
{
  return CwDodsonStanding::get_instance()(t, p, rsw, nacl_concentration);
}

double
CwDodsonStanding__correlation__valid__fct(double t, double p, double rsw,
                                          double nacl_concentration)
{
  return CwDodsonStanding::get_instance()(t, p, rsw, nacl_concentration).raw();
}

Quantity<psia_1>
CwOsif__correlation__fct(const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
                         const Quantity<CgL>& nacl_concentration)
{
  return CwOsif::get_instance()(t, p, nacl_concentration);
}

double
CwOsif__correlation__valid__fct(double t, double p, double nacl_concentration)
{
  return CwOsif::get_instance()(t, p, nacl_concentration).raw();
}

Quantity<psia_1>
CwMcCain__correlation__fct(
  const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
  const Quantity<Dissolved_Salt_Percent>& nacl_concentration,
  const Quantity<RB_SCF>& bg, const Quantity<RB_STB>& bw,
  const Quantity<psia_1>& cwa)
{
  return CwMcCain::get_instance()(t, p, nacl_concentration, bg, bw, cwa);
}

double
CwMcCain__correlation__valid__fct(double t, double p, double nacl_concentration,
                                  double bg, double bw, double cwa)
{
  return CwMcCain::get_instance()(t, p, nacl_concentration, bg, bw, cwa).raw();
}
