Quantity<Rankine> TpchcStanding__correlation__fct(const Quantity<Sgg>& yghc) {
  return TpchcStanding::get_instance()(yghc);
}

double TpchcStanding__correlation__valid__fct(const double& yghc) {
  return TpchcStanding::get_instance()(yghc).raw();
}

Quantity<Rankine> TpchcStandingHeavierFractions__correlation__fct(
    const Quantity<Sgg>& yghc) {
  return TpchcStandingHeavierFractions::get_instance()(yghc);
}

double TpchcStandingHeavierFractions__correlation__valid__fct(
    const double& yghc) {
  return TpchcStandingHeavierFractions::get_instance()(yghc).raw();
}

Quantity<Rankine> TpchcSutton__correlation__fct(const Quantity<Sgg>& yghc) {
  return TpchcSutton::get_instance()(yghc);
}

double TpchcSutton__correlation__valid__fct(const double& yghc) {
  return TpchcSutton::get_instance()(yghc).raw();
}

Quantity<Rankine> TpchcElsharkawyEtAl__correlation__fct(
    const Quantity<Sgg>& yghc) {
  return TpchcElsharkawyEtAl::get_instance()(yghc);
}

double TpchcElsharkawyEtAl__correlation__valid__fct(const double& yghc) {
  return TpchcElsharkawyEtAl::get_instance()(yghc).raw();
}
