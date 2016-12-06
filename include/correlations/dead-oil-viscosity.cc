Quantity<CP>
UodBeal__correlation__fct(const Quantity<Api>& api,
                          const Quantity<Fahrenheit>& t)
{
  return UodBeal::get_instance()(api, t);
}

double
UodBeal__correlation__valid__fct(double api, double t)
{
  return UodBeal::get_instance()(api, t).raw();
}

Quantity<CP>
UodBeggsRobinson__correlation__fct(const Quantity<Api>& api,
                                   const Quantity<Fahrenheit>& t)
{
  return UodBeggsRobinson::get_instance()(api, t);
}

double
UodBeggsRobinson__correlation__valid__fct(double api, double t)
{
  return UodBeggsRobinson::get_instance()(api, t).raw();
}

Quantity<CP>
UodEgbogahNg__correlation__fct(const Quantity<Api>& api,
                               const Quantity<Fahrenheit>& t)
{
  return UodEgbogahNg::get_instance()(api, t);
}

double
UodEgbogahNg__correlation__valid__fct(double api, double t)
{
  return UodEgbogahNg::get_instance()(api, t).raw();
}

Quantity<CP>
UodGlaso__correlation__fct(const Quantity<Api>& api,
                           const Quantity<Fahrenheit>& t)
{
  return UodGlaso::get_instance()(api, t);
}

double
UodGlaso__correlation__valid__fct(double api, double t)
{
  return UodGlaso::get_instance()(api, t).raw();
}

Quantity<CP>
UodKartoatmodjoSchmidt__correlation__fct(const Quantity<Api>& api,
                                         const Quantity<Fahrenheit>& t)
{
  return UodKartoatmodjoSchmidt::get_instance()(api, t);
}

double
UodKartoatmodjoSchmidt__correlation__valid__fct(double api, double t)
{
  return UodKartoatmodjoSchmidt::get_instance()(api, t).raw();
}

Quantity<CP>
UodSattarinEtAl__correlation__fct(const Quantity<Api>& api,
                                  const Quantity<Kelvin>& t)
{
  return UodSattarinEtAl::get_instance()(api, t);
}

double
UodSattarinEtAl__correlation__valid__fct(double api, double t)
{
  return UodSattarinEtAl::get_instance()(api, t).raw();
}

Quantity<CP>
UodNaseri__correlation__fct(const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t)
{
  return UodNaseri::get_instance()(api, t);
}

double
UodNaseri__correlation__valid__fct(double api, double t)
{
  return UodNaseri::get_instance()(api, t).raw();
}

Quantity<CP>
UodPetroskyFarshad__correlation__fct(const Quantity<Api>& api,
                                     const Quantity<Fahrenheit>& t)
{
  return UodPetroskyFarshad::get_instance()(api, t);
}

double
UodPetroskyFarshad__correlation__valid__fct(double api, double t)
{
  return UodPetroskyFarshad::get_instance()(api, t).raw();
}

Quantity<CP>
UodDeGhettoEtAl__correlation__fct(const Quantity<Api>& api,
                                  const Quantity<Fahrenheit>& t)
{
  return UodDeGhettoEtAl::get_instance()(api, t);
}

double
UodDeGhettoEtAl__correlation__valid__fct(double api, double t)
{
  return UodDeGhettoEtAl::get_instance()(api, t).raw();
}

Quantity<CP>
UodPerezML__correlation__fct(const Quantity<Api>& api,
                             const Quantity<Fahrenheit>& t)
{
  return UodPerezML::get_instance()(api, t);
}

double
UodPerezML__correlation__valid__fct(double api, double t)
{
  return UodPerezML::get_instance()(api, t).raw();
}

Quantity<CP>
UodDindorukChristman__correlation__fct(const Quantity<Api>& api,
                                       const Quantity<Fahrenheit>& t,
                                       const Quantity<psia>& pb,
                                       const Quantity<SCF_STB>& rsb)
{
  return UodDindorukChristman::get_instance()(api, t, pb, rsb);
}

double
UodDindorukChristman__correlation__valid__fct(double api, double t, double pb,
                                              double rsb)
{
  return UodDindorukChristman::get_instance()(api, t, pb, rsb).raw();
}
