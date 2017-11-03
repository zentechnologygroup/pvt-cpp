Quantity<Zfactor> ZfactorSarem__correlation__fct(
    const Quantity<PseudoReducedTemperature>& tpr,
    const Quantity<PseudoReducedPressure>& ppr) {
  return ZfactorSarem::get_instance()(tpr, ppr);
}

double ZfactorSarem__correlation__valid__fct(const double& tpr,
                                             const double& ppr) {
  return ZfactorSarem::get_instance()(tpr, ppr).raw();
}

Quantity<Zfactor> ZfactorHallYarborough__correlation__fct(
    const Quantity<PseudoReducedTemperature>& tpr,
    const Quantity<PseudoReducedPressure>& ppr) {
  return ZfactorHallYarborough::get_instance()(tpr, ppr);
}

double ZfactorHallYarborough__correlation__valid__fct(const double& tpr,
                                                      const double& ppr) {
  return ZfactorHallYarborough::get_instance()(tpr, ppr).raw();
}

Quantity<Zfactor> ZfactorDranchukPR__correlation__fct(
    const Quantity<PseudoReducedTemperature>& tpr,
    const Quantity<PseudoReducedPressure>& ppr) {
  return ZfactorDranchukPR::get_instance()(tpr, ppr);
}

double ZfactorDranchukPR__correlation__valid__fct(const double& tpr,
                                                  const double& ppr) {
  return ZfactorDranchukPR::get_instance()(tpr, ppr).raw();
}

Quantity<Zfactor> ZfactorDranchukAK__correlation__fct(
    const Quantity<PseudoReducedTemperature>& tpr,
    const Quantity<PseudoReducedPressure>& ppr) {
  return ZfactorDranchukAK::get_instance()(tpr, ppr);
}

double ZfactorDranchukAK__correlation__valid__fct(const double& tpr,
                                                  const double& ppr) {
  return ZfactorDranchukAK::get_instance()(tpr, ppr).raw();
}

Quantity<Zfactor> ZfactorGopal__correlation__fct(
    const Quantity<PseudoReducedTemperature>& tpr,
    const Quantity<PseudoReducedPressure>& ppr) {
  return ZfactorGopal::get_instance()(tpr, ppr);
}

double ZfactorGopal__correlation__valid__fct(const double& tpr,
                                             const double& ppr) {
  return ZfactorGopal::get_instance()(tpr, ppr).raw();
}

Quantity<Zfactor> ZfactorBrillBeggs__correlation__fct(
    const Quantity<PseudoReducedTemperature>& tpr,
    const Quantity<PseudoReducedPressure>& ppr) {
  return ZfactorBrillBeggs::get_instance()(tpr, ppr);
}

double ZfactorBrillBeggs__correlation__valid__fct(const double& tpr,
                                                  const double& ppr) {
  return ZfactorBrillBeggs::get_instance()(tpr, ppr).raw();
}

Quantity<Zfactor> ZfactorPapay__correlation__fct(
    const Quantity<PseudoReducedTemperature>& tpr,
    const Quantity<PseudoReducedPressure>& ppr) {
  return ZfactorPapay::get_instance()(tpr, ppr);
}

double ZfactorPapay__correlation__valid__fct(const double& tpr,
                                             const double& ppr) {
  return ZfactorPapay::get_instance()(tpr, ppr).raw();
}
