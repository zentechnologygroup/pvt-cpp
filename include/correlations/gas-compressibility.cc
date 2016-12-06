Quantity<ZFactor>
ZFactorSarem__correlation__fct(const Quantity<PseudoReducedTemperature>& tpr,
                               const Quantity<PseudoReducedPressure>& ppr)
{
  return ZFactorSarem::get_instance()(tpr, ppr);
}

double
ZFactorSarem__correlation__valid__fct(double tpr, double ppr)
{
  return ZFactorSarem::get_instance()(tpr, ppr).raw();
}

Quantity<ZFactor>
ZFactorHallYarborough__correlation__fct(
  const Quantity<PseudoReducedTemperature>& tpr,
  const Quantity<PseudoReducedPressure>& ppr)
{
  return ZFactorHallYarborough::get_instance()(tpr, ppr);
}

double
ZFactorHallYarborough__correlation__valid__fct(double tpr, double ppr)
{
  return ZFactorHallYarborough::get_instance()(tpr, ppr).raw();
}

Quantity<ZFactor>
ZFactorDranchukPR__correlation__fct(
  const Quantity<PseudoReducedTemperature>& tpr,
  const Quantity<PseudoReducedPressure>& ppr)
{
  return ZFactorDranchukPR::get_instance()(tpr, ppr);
}

double
ZFactorDranchukPR__correlation__valid__fct(double tpr, double ppr)
{
  return ZFactorDranchukPR::get_instance()(tpr, ppr).raw();
}

Quantity<ZFactor>
ZFactorDranchukAK__correlation__fct(
  const Quantity<PseudoReducedTemperature>& tpr,
  const Quantity<PseudoReducedPressure>& ppr)
{
  return ZFactorDranchukAK::get_instance()(tpr, ppr);
}

double
ZFactorDranchukAK__correlation__valid__fct(double tpr, double ppr)
{
  return ZFactorDranchukAK::get_instance()(tpr, ppr).raw();
}

Quantity<ZFactor>
ZFactorGopal__correlation__fct(const Quantity<PseudoReducedTemperature>& tpr,
                               const Quantity<PseudoReducedPressure>& ppr)
{
  return ZFactorGopal::get_instance()(tpr, ppr);
}

double
ZFactorGopal__correlation__valid__fct(double tpr, double ppr)
{
  return ZFactorGopal::get_instance()(tpr, ppr).raw();
}

Quantity<ZFactor>
ZFactorBrillBeggs__correlation__fct(
  const Quantity<PseudoReducedTemperature>& tpr,
  const Quantity<PseudoReducedPressure>& ppr)
{
  return ZFactorBrillBeggs::get_instance()(tpr, ppr);
}

double
ZFactorBrillBeggs__correlation__valid__fct(double tpr, double ppr)
{
  return ZFactorBrillBeggs::get_instance()(tpr, ppr).raw();
}

Quantity<ZFactor>
ZFactorPapay__correlation__fct(const Quantity<PseudoReducedTemperature>& tpr,
                               const Quantity<PseudoReducedPressure>& ppr)
{
  return ZFactorPapay::get_instance()(tpr, ppr);
}

double
ZFactorPapay__correlation__valid__fct(double tpr, double ppr)
{
  return ZFactorPapay::get_instance()(tpr, ppr).raw();
}
