Quantity<CP>
UobBeggsRobinson__correlation__fct(const Quantity<CP>& uod,
                                   const Quantity<SCF_STB>& rs)
{
  return UobBeggsRobinson::get_instance()(uod, rs);
}

double
UobBeggsRobinson__correlation__valid__fct(double uod, double rs)
{
  return UobBeggsRobinson::get_instance()(uod, rs).raw();
}

Quantity<CP>
UobChewConnally__correlation__fct(const Quantity<CP>& uod,
                                  const Quantity<SCF_STB>& rs)
{
  return UobChewConnally::get_instance()(uod, rs);
}

double
UobChewConnally__correlation__valid__fct(double uod, double rs)
{
  return UobChewConnally::get_instance()(uod, rs).raw();
}

Quantity<CP>
UobKhan__correlation__fct(const Quantity<SCF_STB>& rsb,
                          const Quantity<Sg_do>& yo, const Quantity<Sgg>& yg,
                          const Quantity<Fahrenheit>& t,
                          const Quantity<psia>& p, const Quantity<psia>& pb)
{
  return UobKhan::get_instance()(rsb, yo, yg, t, p, pb);
}

double
UobKhan__correlation__valid__fct(double rsb, double yo, double yg, double t,
                                 double p, double pb)
{
  return UobKhan::get_instance()(rsb, yo, yg, t, p, pb).raw();
}

Quantity<CP>
UobKartoatmodjoSchmidt__correlation__fct(const Quantity<CP>& uod,
                                         const Quantity<SCF_STB>& rs)
{
  return UobKartoatmodjoSchmidt::get_instance()(uod, rs);
}

double
UobKartoatmodjoSchmidt__correlation__valid__fct(double uod, double rs)
{
  return UobKartoatmodjoSchmidt::get_instance()(uod, rs).raw();
}

Quantity<CP>
UobPetroskyFarshad__correlation__fct(const Quantity<CP>& uod,
                                     const Quantity<SCF_STB>& rs)
{
  return UobPetroskyFarshad::get_instance()(uod, rs);
}

double
UobPetroskyFarshad__correlation__valid__fct(double uod, double rs)
{
  return UobPetroskyFarshad::get_instance()(uod, rs).raw();
}

Quantity<CP>
UobPerezML__correlation__fct(const Quantity<CP>& uod,
                             const Quantity<SCF_STB>& rs)
{
  return UobPerezML::get_instance()(uod, rs);
}

double
UobPerezML__correlation__valid__fct(double uod, double rs)
{
  return UobPerezML::get_instance()(uod, rs).raw();
}

Quantity<CP>
UobGilFonseca__correlation__fct(const Quantity<CP>& uod,
                                const Quantity<SCF_STB>& rs)
{
  return UobGilFonseca::get_instance()(uod, rs);
}

double
UobGilFonseca__correlation__valid__fct(double uod, double rs)
{
  return UobGilFonseca::get_instance()(uod, rs).raw();
}

Quantity<CP>
UobDeGhettoEtAl__correlation__fct(const Quantity<CP>& uod,
                                  const Quantity<SCF_STB>& rs,
                                  const Quantity<Api>& api)
{
  return UobDeGhettoEtAl::get_instance()(uod, rs, api);
}

double
UobDeGhettoEtAl__correlation__valid__fct(double uod, double rs, double api)
{
  return UobDeGhettoEtAl::get_instance()(uod, rs, api).raw();
}

Quantity<CP>
UobDindorukChristman__correlation__fct(const Quantity<CP>& uod,
                                       const Quantity<SCF_STB>& rs)
{
  return UobDindorukChristman::get_instance()(uod, rs);
}

double
UobDindorukChristman__correlation__valid__fct(double uod, double rs)
{
  return UobDindorukChristman::get_instance()(uod, rs).raw();
}
