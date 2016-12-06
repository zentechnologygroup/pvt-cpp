Quantity<Paxs>
UwMaoDuan__correlation__fct(const Quantity<Kelvin>& t,
                            const Quantity<Molality_NaCl>& nacl_concentration,
                            const Quantity<Gr_cm3>& ppw)
{
  return UwMaoDuan::get_instance()(t, nacl_concentration, ppw);
}

double
UwMaoDuan__correlation__valid__fct(double t, double nacl_concentration,
                                   double ppw)
{
  return UwMaoDuan::get_instance()(t, nacl_concentration, ppw).raw();
}

Quantity<CP>
UwVanWingen__correlation__fct(const Quantity<Fahrenheit>& t)
{
  return UwVanWingen::get_instance()(t);
}

double
UwVanWingen__correlation__valid__fct(double t)
{
  return UwVanWingen::get_instance()(t).raw();
}

Quantity<CP>
UwMatthewsRussel__correlation__fct(
  const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
  const Quantity<Dissolved_Salt_Percent>& nacl_concentration)
{
  return UwMatthewsRussel::get_instance()(t, p, nacl_concentration);
}

double
UwMatthewsRussel__correlation__valid__fct(double t, double p,
                                          double nacl_concentration)
{
  return UwMatthewsRussel::get_instance()(t, p, nacl_concentration).raw();
}

Quantity<CP>
UwMcCain__correlation__fct(
  const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
  const Quantity<Dissolved_Salt_Percent>& nacl_concentration)
{
  return UwMcCain::get_instance()(t, p, nacl_concentration);
}

double
UwMcCain__correlation__valid__fct(double t, double p, double nacl_concentration)
{
  return UwMcCain::get_instance()(t, p, nacl_concentration).raw();
}

Quantity<CP>
UwMcCoy__correlation__fct(
  const Quantity<Kelvin>& t,
  const Quantity<Dissolved_Salt_Percent>& nacl_concentration)
{
  return UwMcCoy::get_instance()(t, nacl_concentration);
}

double
UwMcCoy__correlation__valid__fct(double t, double nacl_concentration)
{
  return UwMcCoy::get_instance()(t, nacl_concentration).raw();
}
