Quantity<Paxs> UwMaoDuan__correlation__fct(const Quantity<Kelvin>& t,
                                           const Quantity<Molality_NaCl>& nacl,
                                           const Quantity<Gr_cm3>& ppw) {
  return UwMaoDuan::get_instance()(t, nacl, ppw);
}

double UwMaoDuan__correlation__valid__fct(const double& t, const double& nacl,
                                          const double& ppw) {
  return UwMaoDuan::get_instance()(t, nacl, ppw).raw();
}

Quantity<CP> UwVanWingen__correlation__fct(const Quantity<Fahrenheit>& t) {
  return UwVanWingen::get_instance()(t);
}

double UwVanWingen__correlation__valid__fct(const double& t) {
  return UwVanWingen::get_instance()(t).raw();
}

Quantity<CP> UwMatthewsRussel__correlation__fct(
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<Dissolved_Salt_Percent>& nacl) {
  return UwMatthewsRussel::get_instance()(t, p, nacl);
}

double UwMatthewsRussel__correlation__valid__fct(const double& t,
                                                 const double& p,
                                                 const double& nacl) {
  return UwMatthewsRussel::get_instance()(t, p, nacl).raw();
}

Quantity<CP> UwMcCain__correlation__fct(
    const Quantity<Fahrenheit>& t, const Quantity<psia>& p,
    const Quantity<Dissolved_Salt_Percent>& nacl) {
  return UwMcCain::get_instance()(t, p, nacl);
}

double UwMcCain__correlation__valid__fct(const double& t, const double& p,
                                         const double& nacl) {
  return UwMcCain::get_instance()(t, p, nacl).raw();
}

Quantity<CP> UwMcCoy__correlation__fct(
    const Quantity<Kelvin>& t, const Quantity<Dissolved_Salt_Percent>& nacl) {
  return UwMcCoy::get_instance()(t, nacl);
}

double UwMcCoy__correlation__valid__fct(const double& t, const double& nacl) {
  return UwMcCoy::get_instance()(t, nacl).raw();
}
