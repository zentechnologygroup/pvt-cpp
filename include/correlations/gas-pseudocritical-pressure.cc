Quantity<psia> PpchcStanding__correlation__fct(const Quantity<Sgg>& yghc) {
  return PpchcStanding::get_instance()(yghc);
}

double PpchcStanding__correlation__valid__fct(const double& yghc) {
  return PpchcStanding::get_instance()(yghc).raw();
}

Quantity<psia> PpchcSutton__correlation__fct(const Quantity<Sgg>& yghc) {
  return PpchcSutton::get_instance()(yghc);
}

double PpchcSutton__correlation__valid__fct(const double& yghc) {
  return PpchcSutton::get_instance()(yghc).raw();
}

Quantity<psia> PpchcStandingHeavierFractions__correlation__fct(
    const Quantity<Sgg>& yghc) {
  return PpchcStandingHeavierFractions::get_instance()(yghc);
}

double PpchcStandingHeavierFractions__correlation__valid__fct(
    const double& yghc) {
  return PpchcStandingHeavierFractions::get_instance()(yghc).raw();
}

Quantity<psia> PpchcElsharkawyEtAl__correlation__fct(
    const Quantity<Sgg>& yghc) {
  return PpchcElsharkawyEtAl::get_instance()(yghc);
}

double PpchcElsharkawyEtAl__correlation__valid__fct(const double& yghc) {
  return PpchcElsharkawyEtAl::get_instance()(yghc).raw();
}
