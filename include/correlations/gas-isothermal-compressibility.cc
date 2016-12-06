Quantity<psia_1>
CgSarem__correlation__fct(const Quantity<PseudoReducedTemperature>& tpr,
                          const Quantity<PseudoReducedPressure>& ppr,
                          const Quantity<psia>& ppc, const Quantity<ZFactor>& z)
{
  return CgSarem::get_instance()(tpr, ppr, ppc, z);
}

double
CgSarem__correlation__valid__fct(double tpr, double ppr, double ppc, double z)
{
  return CgSarem::get_instance()(tpr, ppr, ppc, z).raw();
}

Quantity<psia_1>
CgHallYarborough__correlation__fct(
  const Quantity<PseudoReducedTemperature>& tpr,
  const Quantity<PseudoReducedPressure>& ppr, const Quantity<psia>& ppc,
  const Quantity<ZFactor>& z)
{
  return CgHallYarborough::get_instance()(tpr, ppr, ppc, z);
}

double
CgHallYarborough__correlation__valid__fct(double tpr, double ppr, double ppc,
                                          double z)
{
  return CgHallYarborough::get_instance()(tpr, ppr, ppc, z).raw();
}

Quantity<psia_1>
CgMattarBA__correlation__fct(const Quantity<PseudoReducedTemperature>& tpr,
                             const Quantity<PseudoReducedPressure>& ppr,
                             const Quantity<psia>& ppc,
                             const Quantity<ZFactor>& z)
{
  return CgMattarBA::get_instance()(tpr, ppr, ppc, z);
}

double
CgMattarBA__correlation__valid__fct(double tpr, double ppr, double ppc,
                                    double z)
{
  return CgMattarBA::get_instance()(tpr, ppr, ppc, z).raw();
}

Quantity<psia_1>
CgGopal__correlation__fct(const Quantity<PseudoReducedTemperature>& tpr,
                          const Quantity<PseudoReducedPressure>& ppr,
                          const Quantity<psia>& ppc, const Quantity<ZFactor>& z)
{
  return CgGopal::get_instance()(tpr, ppr, ppc, z);
}

double
CgGopal__correlation__valid__fct(double tpr, double ppr, double ppc, double z)
{
  return CgGopal::get_instance()(tpr, ppr, ppc, z).raw();
}

Quantity<psia_1>
CgBrillBeggs__correlation__fct(const Quantity<PseudoReducedTemperature>& tpr,
                               const Quantity<PseudoReducedPressure>& ppr,
                               const Quantity<psia>& ppc,
                               const Quantity<ZFactor>& z)
{
  return CgBrillBeggs::get_instance()(tpr, ppr, ppc, z);
}

double
CgBrillBeggs__correlation__valid__fct(double tpr, double ppr, double ppc,
                                      double z)
{
  return CgBrillBeggs::get_instance()(tpr, ppr, ppc, z).raw();
}

Quantity<psia_1>
CgPapay__correlation__fct(const Quantity<PseudoReducedTemperature>& tpr,
                          const Quantity<PseudoReducedPressure>& ppr,
                          const Quantity<psia>& ppc, const Quantity<ZFactor>& z)
{
  return CgPapay::get_instance()(tpr, ppr, ppc, z);
}

double
CgPapay__correlation__valid__fct(double tpr, double ppr, double ppc, double z)
{
  return CgPapay::get_instance()(tpr, ppr, ppc, z).raw();
}
