Quantity<CP>
UgCarrKB__correlation__fct(const Quantity<Fahrenheit>& t,
                           const Quantity<PseudoReducedTemperature>& tpr,
                           const Quantity<PseudoReducedPressure>& ppr,
                           const Quantity<Sgg>& yg,
                           const Quantity<MolePercent>& n2_concentration,
                           const Quantity<MolePercent>& co2_concentration,
                           const Quantity<MolePercent>& h2s_concentration)
{
  return UgCarrKB::get_instance()(t, tpr, ppr, yg, n2_concentration,
                                  co2_concentration, h2s_concentration);
}

double
UgCarrKB__correlation__valid__fct(double t, double tpr, double ppr, double yg,
                                  double n2_concentration,
                                  double co2_concentration,
                                  double h2s_concentration)
{
  return UgCarrKB::get_instance()(t, tpr, ppr, yg, n2_concentration,
                                  co2_concentration, h2s_concentration)
    .raw();
}

Quantity<mP>
UgLeeGE__correlation__fct(const Quantity<Rankine>& t, const Quantity<psia>& p,
                          const Quantity<Sgg>& yg, const Quantity<ZFactor>& z)
{
  return UgLeeGE::get_instance()(t, p, yg, z);
}

double
UgLeeGE__correlation__valid__fct(double t, double p, double yg, double z)
{
  return UgLeeGE::get_instance()(t, p, yg, z).raw();
}

Quantity<CP>
UgDeanStiel__correlation__fct(const Quantity<Rankine>& t,
                              const Quantity<psia>& p,
                              const Quantity<Rankine>& tpc,
                              const Quantity<psia>& ppc,
                              const Quantity<Sgg>& yg,
                              const Quantity<ZFactor>& z)
{
  return UgDeanStiel::get_instance()(t, p, tpc, ppc, yg, z);
}

double
UgDeanStiel__correlation__valid__fct(double t, double p, double tpc, double ppc,
                                     double yg, double z)
{
  return UgDeanStiel::get_instance()(t, p, tpc, ppc, yg, z).raw();
}
