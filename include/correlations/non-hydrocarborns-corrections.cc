Quantity<Sgg>
YgHCWichertAziz__correlation__fct(
  const Quantity<Sgg>& yg, const Quantity<MoleFraction>& n2_concentration,
  const Quantity<MoleFraction>& co2_concentration,
  const Quantity<MoleFraction>& h2s_concentration)
{
  return YgHCWichertAziz::get_instance()(yg, n2_concentration,
                                         co2_concentration, h2s_concentration);
}

double
YgHCWichertAziz__correlation__valid__fct(double yg, double n2_concentration,
                                         double co2_concentration,
                                         double h2s_concentration)
{
  return YgHCWichertAziz::get_instance()(yg, n2_concentration,
                                         co2_concentration, h2s_concentration)
    .raw();
}

Quantity<psia>
PscMKayMixingRule__correlation__fct(
  const Quantity<psia>& ppchc, const Quantity<MoleFraction>& n2_concentration,
  const Quantity<MoleFraction>& co2_concentration,
  const Quantity<MoleFraction>& h2s_concentration)
{
  return PscMKayMixingRule::get_instance()(
    ppchc, n2_concentration, co2_concentration, h2s_concentration);
}

double
PscMKayMixingRule__correlation__valid__fct(double ppchc,
                                           double n2_concentration,
                                           double co2_concentration,
                                           double h2s_concentration)
{
  return PscMKayMixingRule::get_instance()(ppchc, n2_concentration,
                                           co2_concentration, h2s_concentration)
    .raw();
}

Quantity<psia>
AdjustedPscMWichertAziz__correlation__fct(
  const Quantity<psia>& ppcm, const Quantity<Fahrenheit>& tpcm,
  const Quantity<MoleFraction>& co2_concentration,
  const Quantity<MoleFraction>& h2s_concentration)
{
  return AdjustedPscMWichertAziz::get_instance()(ppcm, tpcm, co2_concentration,
                                                 h2s_concentration);
}

double
AdjustedPscMWichertAziz__correlation__valid__fct(double ppcm, double tpcm,
                                                 double co2_concentration,
                                                 double h2s_concentration)
{
  return AdjustedPscMWichertAziz::get_instance()(ppcm, tpcm, co2_concentration,
                                                 h2s_concentration)
    .raw();
}

Quantity<Rankine>
TscMKayMixingRule__correlation__fct(
  const Quantity<Rankine>& tpchc,
  const Quantity<MoleFraction>& n2_concentration,
  const Quantity<MoleFraction>& co2_concentration,
  const Quantity<MoleFraction>& h2s_concentration)
{
  return TscMKayMixingRule::get_instance()(
    tpchc, n2_concentration, co2_concentration, h2s_concentration);
}

double
TscMKayMixingRule__correlation__valid__fct(double tpchc,
                                           double n2_concentration,
                                           double co2_concentration,
                                           double h2s_concentration)
{
  return TscMKayMixingRule::get_instance()(tpchc, n2_concentration,
                                           co2_concentration, h2s_concentration)
    .raw();
}

Quantity<Rankine>
AdjustedTscMWichertAziz__correlation__fct(
  const Quantity<Rankine>& tpcm,
  const Quantity<MoleFraction>& co2_concentration,
  const Quantity<MoleFraction>& h2s_concentration)
{
  return AdjustedTscMWichertAziz::get_instance()(tpcm, co2_concentration,
                                                 h2s_concentration);
}

double
AdjustedTscMWichertAziz__correlation__valid__fct(double tpcm,
                                                 double co2_concentration,
                                                 double h2s_concentration)
{
  return AdjustedTscMWichertAziz::get_instance()(tpcm, co2_concentration,
                                                 h2s_concentration)
    .raw();
}
