Quantity<RB_STB>
BwSpiveyMN__correlation__fct(const Quantity<Celsius>& t,
                             const Quantity<mPascal>& p,
                             const Quantity<Dissolved_Salt_Percent>& s)
{
  return BwSpiveyMN::get_instance()(t, p, s);
}

double
BwSpiveyMN__correlation__valid__fct(double t, double p, double s)
{
  return BwSpiveyMN::get_instance()(t, p, s).raw();
}

Quantity<RB_STB>
BwMcCain__correlation__fct(const Quantity<Fahrenheit>& t,
                           const Quantity<psia>& p)
{
  return BwMcCain::get_instance()(t, p);
}

double
BwMcCain__correlation__valid__fct(double t, double p)
{
  return BwMcCain::get_instance()(t, p).raw();
}

Quantity<RB_STB>
BwMcCoy__correlation__fct(const Quantity<Fahrenheit>& t,
                          const Quantity<psia>& p, const Quantity<psia>& pb,
                          const Quantity<Dissolved_Salt_Percent>& s,
                          const Quantity<psia_1>& cw)
{
  return BwMcCoy::get_instance()(t, p, pb, s, cw);
}

double
BwMcCoy__correlation__valid__fct(double t, double p, double pb, double s,
                                 double cw)
{
  return BwMcCoy::get_instance()(t, p, pb, s, cw).raw();
}
