Quantity<Sgg> YghcWichertAziz__correlation__fct(
    const Quantity<Sgg>& yg, const Quantity<MoleFraction>& n2,
    const Quantity<MoleFraction>& co2, const Quantity<MoleFraction>& h2s) {
  return YghcWichertAziz::get_instance()(yg, n2, co2, h2s);
}

double YghcWichertAziz__correlation__valid__fct(const double& yg,
                                                const double& n2,
                                                const double& co2,
                                                const double& h2s) {
  return YghcWichertAziz::get_instance()(yg, n2, co2, h2s).raw();
}

Quantity<psia> PpcmKayMixingRule__correlation__fct(
    const Quantity<psia>& ppchc, const Quantity<MoleFraction>& n2,
    const Quantity<MoleFraction>& co2, const Quantity<MoleFraction>& h2s) {
  return PpcmKayMixingRule::get_instance()(ppchc, n2, co2, h2s);
}

double PpcmKayMixingRule__correlation__valid__fct(const double& ppchc,
                                                  const double& n2,
                                                  const double& co2,
                                                  const double& h2s) {
  return PpcmKayMixingRule::get_instance()(ppchc, n2, co2, h2s).raw();
}

Quantity<psia> AdjustedppcmWichertAziz__correlation__fct(
    const Quantity<psia>& ppcm, const Quantity<Rankine>& tpcm,
    const Quantity<MoleFraction>& co2, const Quantity<MoleFraction>& h2s) {
  return AdjustedppcmWichertAziz::get_instance()(ppcm, tpcm, co2, h2s);
}

double AdjustedppcmWichertAziz__correlation__valid__fct(const double& ppcm,
                                                        const double& tpcm,
                                                        const double& co2,
                                                        const double& h2s) {
  return AdjustedppcmWichertAziz::get_instance()(ppcm, tpcm, co2, h2s).raw();
}

Quantity<Rankine> TpcmKayMixingRule__correlation__fct(
    const Quantity<Rankine>& tpchc, const Quantity<MoleFraction>& n2,
    const Quantity<MoleFraction>& co2, const Quantity<MoleFraction>& h2s) {
  return TpcmKayMixingRule::get_instance()(tpchc, n2, co2, h2s);
}

double TpcmKayMixingRule__correlation__valid__fct(const double& tpchc,
                                                  const double& n2,
                                                  const double& co2,
                                                  const double& h2s) {
  return TpcmKayMixingRule::get_instance()(tpchc, n2, co2, h2s).raw();
}

Quantity<Rankine> AdjustedtpcmWichertAziz__correlation__fct(
    const Quantity<Rankine>& tpcm, const Quantity<MoleFraction>& co2,
    const Quantity<MoleFraction>& h2s) {
  return AdjustedtpcmWichertAziz::get_instance()(tpcm, co2, h2s);
}

double AdjustedtpcmWichertAziz__correlation__valid__fct(const double& tpcm,
                                                        const double& co2,
                                                        const double& h2s) {
  return AdjustedtpcmWichertAziz::get_instance()(tpcm, co2, h2s).raw();
}
