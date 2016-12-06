Quantity<psia>
PscHCStanding__correlation__fct(const Quantity<Sgg>& yghc)
{
  return PscHCStanding::get_instance()(yghc);
}

double
PscHCStanding__correlation__valid__fct(double yghc)
{
  return PscHCStanding::get_instance()(yghc).raw();
}

Quantity<psia>
PscHcSutton__correlation__fct(const Quantity<Sgg>& yghc)
{
  return PscHcSutton::get_instance()(yghc);
}

double
PscHcSutton__correlation__valid__fct(double yghc)
{
  return PscHcSutton::get_instance()(yghc).raw();
}

Quantity<psia>
PscHCGuoGhalambor__correlation__fct(
  const Quantity<Sgg>& yghc, const Quantity<MoleFraction>& n2_concentration,
  const Quantity<MoleFraction>& h2s_concentration)
{
  return PscHCGuoGhalambor::get_instance()(yghc, n2_concentration,
                                           h2s_concentration);
}

double
PscHCGuoGhalambor__correlation__valid__fct(double yghc, double n2_concentration,
                                           double h2s_concentration)
{
  return PscHCGuoGhalambor::get_instance()(yghc, n2_concentration,
                                           h2s_concentration)
    .raw();
}

Quantity<psia>
PscAhmed__correlation__fct(const Quantity<Sgg>& yghc,
                           const Quantity<MoleFraction>& n2_concentration,
                           const Quantity<MoleFraction>& co2_concentration,
                           const Quantity<MoleFraction>& h2s_concentration)
{
  return PscAhmed::get_instance()(yghc, n2_concentration, co2_concentration,
                                  h2s_concentration);
}

double
PscAhmed__correlation__valid__fct(double yghc, double n2_concentration,
                                  double co2_concentration,
                                  double h2s_concentration)
{
  return PscAhmed::get_instance()(yghc, n2_concentration, co2_concentration,
                                  h2s_concentration)
    .raw();
}

Quantity<psia>
PscHCStandingHeavierFractions__correlation__fct(const Quantity<Sgg>& yghc)
{
  return PscHCStandingHeavierFractions::get_instance()(yghc);
}

double
PscHCStandingHeavierFractions__correlation__valid__fct(double yghc)
{
  return PscHCStandingHeavierFractions::get_instance()(yghc).raw();
}

Quantity<psia>
PscHCElsharkawyEtAl__correlation__fct(const Quantity<Sgg>& yghc)
{
  return PscHCElsharkawyEtAl::get_instance()(yghc);
}

double
PscHCElsharkawyEtAl__correlation__valid__fct(double yghc)
{
  return PscHCElsharkawyEtAl::get_instance()(yghc).raw();
}
