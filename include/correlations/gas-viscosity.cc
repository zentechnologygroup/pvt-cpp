Quantity<CP> UgCarrKB__correlation__fct(
    const Quantity<Fahrenheit>& t,
    const Quantity<PseudoReducedTemperature>& tpr,
    const Quantity<PseudoReducedPressure>& ppr, const Quantity<Sgg>& yg,
    const Quantity<MoleFraction>& n2, const Quantity<MoleFraction>& co2,
    const Quantity<MoleFraction>& h2s) {
  return UgCarrKB::get_instance()(t, tpr, ppr, yg, n2, co2, h2s);
}

double UgCarrKB__correlation__valid__fct(const double& t, const double& tpr,
                                         const double& ppr, const double& yg,
                                         const double& n2, const double& co2,
                                         const double& h2s) {
  return UgCarrKB::get_instance()(t, tpr, ppr, yg, n2, co2, h2s).raw();
}

Quantity<mP> UgLeeGE__correlation__fct(const Quantity<Rankine>& t,
                                       const Quantity<psia>& p,
                                       const Quantity<Sgg>& yg,
                                       const Quantity<Zfactor>& z) {
  return UgLeeGE::get_instance()(t, p, yg, z);
}

double UgLeeGE__correlation__valid__fct(const double& t, const double& p,
                                        const double& yg, const double& z) {
  return UgLeeGE::get_instance()(t, p, yg, z).raw();
}

Quantity<CP> UgDeanStiel__correlation__fct(const Quantity<Rankine>& t,
                                           const Quantity<psia>& p,
                                           const Quantity<Rankine>& tpc,
                                           const Quantity<psia>& ppc,
                                           const Quantity<Sgg>& yg,
                                           const Quantity<Zfactor>& z) {
  return UgDeanStiel::get_instance()(t, p, tpc, ppc, yg, z);
}

double UgDeanStiel__correlation__valid__fct(const double& t, const double& p,
                                            const double& tpc,
                                            const double& ppc, const double& yg,
                                            const double& z) {
  return UgDeanStiel::get_instance()(t, p, tpc, ppc, yg, z).raw();
}
