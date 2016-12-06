Quantity<Rankine>
TscHCStanding__correlation__fct(const Quantity<Sgg>& yghc)
{
  return TscHCStanding::get_instance()(yghc);
}

double
TscHCStanding__correlation__valid__fct(double yghc)
{
  return TscHCStanding::get_instance()(yghc).raw();
}

Quantity<Rankine>
TscHCStandingHeavierFractions__correlation__fct(const Quantity<Sgg>& yghc)
{
  return TscHCStandingHeavierFractions::get_instance()(yghc);
}

double
TscHCStandingHeavierFractions__correlation__valid__fct(double yghc)
{
  return TscHCStandingHeavierFractions::get_instance()(yghc).raw();
}

Quantity<Rankine>
TscHCSutton__correlation__fct(const Quantity<Sgg>& yghc)
{
  return TscHCSutton::get_instance()(yghc);
}

double
TscHCSutton__correlation__valid__fct(double yghc)
{
  return TscHCSutton::get_instance()(yghc).raw();
}

Quantity<Rankine>
TpcHCElsharkawyEtAl__correlation__fct(const Quantity<Sgg>& yghc)
{
  return TpcHCElsharkawyEtAl::get_instance()(yghc);
}

double
TpcHCElsharkawyEtAl__correlation__valid__fct(double yghc)
{
  return TpcHCElsharkawyEtAl::get_instance()(yghc).raw();
}
