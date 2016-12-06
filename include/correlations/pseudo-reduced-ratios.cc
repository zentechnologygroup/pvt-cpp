Quantity<PseudoReducedTemperature>
Tpr__correlation__fct(const Quantity<Rankine>& t, const Quantity<Rankine>& tpc)
{
  return Tpr::get_instance()(t, tpc);
}

double
Tpr__correlation__valid__fct(double t, double tpc)
{
  return Tpr::get_instance()(t, tpc).raw();
}

Quantity<PseudoReducedPressure>
Ppr__correlation__fct(const Quantity<psia>& p, const Quantity<psia>& ppc)
{
  return Ppr::get_instance()(p, ppc);
}

double
Ppr__correlation__valid__fct(double p, double ppc)
{
  return Ppr::get_instance()(p, ppc).raw();
}
