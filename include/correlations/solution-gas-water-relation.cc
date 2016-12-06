Quantity<Sm3_Sm3>
RswSpiveyMN__correlation__fct(const Quantity<Celsius>& t,
                              const Quantity<mPascal>& p,
                              const Quantity<Molality_NaCl>& nacl_concentration)
{
  return RswSpiveyMN::get_instance()(t, p, nacl_concentration);
}

double
RswSpiveyMN__correlation__valid__fct(double t, double p,
                                     double nacl_concentration)
{
  return RswSpiveyMN::get_instance()(t, p, nacl_concentration).raw();
}

Quantity<SCF_STB>
RswCulbersonMcKetta__correlation__fct(
  const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
  const Quantity<Dissolved_Salt_Percent>& nacl_concentration)
{
  return RswCulbersonMcKetta::get_instance()(t, p, nacl_concentration);
}

double
RswCulbersonMcKetta__correlation__valid__fct(double t, double p,
                                             double nacl_concentration)
{
  return RswCulbersonMcKetta::get_instance()(t, p, nacl_concentration).raw();
}

Quantity<SCF_STB>
RswMcCoy__correlation__fct(
  const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
  const Quantity<Dissolved_Salt_Percent>& nacl_concentration)
{
  return RswMcCoy::get_instance()(t, p, nacl_concentration);
}

double
RswMcCoy__correlation__valid__fct(double t, double p, double nacl_concentration)
{
  return RswMcCoy::get_instance()(t, p, nacl_concentration).raw();
}
