Quantity<mPa_1>
CwaSpiveyMN__correlation__fct(const Quantity<Celsius>& t,
                              const Quantity<mPascal>& p,
                              const Quantity<Molality_NaCl>& nacl_concentration)
{
  return CwaSpiveyMN::get_instance()(t, p, nacl_concentration);
}

double
CwaSpiveyMN__correlation__valid__fct(double t, double p,
                                     double nacl_concentration)
{
  return CwaSpiveyMN::get_instance()(t, p, nacl_concentration).raw();
}

Quantity<psia_1>
CwaDodsonStanding__correlation__fct(
  const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
  const Quantity<SCF_STB>& rsw,
  const Quantity<Dissolved_Salt_Percent>& nacl_concentration)
{
  return CwaDodsonStanding::get_instance()(t, p, rsw, nacl_concentration);
}

double
CwaDodsonStanding__correlation__valid__fct(double t, double p, double rsw,
                                           double nacl_concentration)
{
  return CwaDodsonStanding::get_instance()(t, p, rsw, nacl_concentration).raw();
}

Quantity<psia_1>
CwaOsif__correlation__fct(const Quantity<Fahrenheit>& t,
                          const Quantity<psia>& p,
                          const Quantity<CgL>& nacl_concentration)
{
  return CwaOsif::get_instance()(t, p, nacl_concentration);
}

double
CwaOsif__correlation__valid__fct(double t, double p, double nacl_concentration)
{
  return CwaOsif::get_instance()(t, p, nacl_concentration).raw();
}
