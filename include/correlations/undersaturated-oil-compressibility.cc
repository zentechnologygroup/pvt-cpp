Quantity<psia_1>
CoaDeGhetto__correlation__fct(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<Fahrenheit>& tsep,
                              const Quantity<psia>& p,
                              const Quantity<psia>& psep)
{
  return CoaDeGhetto::get_instance()(yg, api, rsb, t, tsep, p, psep);
}

double
CoaDeGhetto__correlation__valid__fct(double yg, double api, double rsb,
                                     double t, double tsep, double p,
                                     double psep)
{
  return CoaDeGhetto::get_instance()(yg, api, rsb, t, tsep, p, psep).raw();
}

Quantity<psia_1>
CoaHanafy__correlation__fct(const Quantity<SCF_STB>& rsb)
{
  return CoaHanafy::get_instance()(rsb);
}

double
CoaHanafy__correlation__valid__fct(double rsb)
{
  return CoaHanafy::get_instance()(rsb).raw();
}

Quantity<psia_1>
CoaKartoatmodjoSchmidt__correlation__fct(const Quantity<Sgg>& yg,
                                         const Quantity<Api>& api,
                                         const Quantity<SCF_STB>& rsb,
                                         const Quantity<Fahrenheit>& t,
                                         const Quantity<Fahrenheit>& tsep,
                                         const Quantity<psia>& p,
                                         const Quantity<psia>& psep)
{
  return CoaKartoatmodjoSchmidt::get_instance()(yg, api, rsb, t, tsep, p, psep);
}

double
CoaKartoatmodjoSchmidt__correlation__valid__fct(double yg, double api,
                                                double rsb, double t,
                                                double tsep, double p,
                                                double psep)
{
  return CoaKartoatmodjoSchmidt::get_instance()(yg, api, rsb, t, tsep, p, psep)
    .raw();
}

Quantity<psia_1>
CoaPetroskyFarshad__correlation__fct(const Quantity<Sgg>& yg,
                                     const Quantity<Api>& api,
                                     const Quantity<SCF_STB>& rsb,
                                     const Quantity<Fahrenheit>& t,
                                     const Quantity<psia>& p)
{
  return CoaPetroskyFarshad::get_instance()(yg, api, rsb, t, p);
}

double
CoaPetroskyFarshad__correlation__valid__fct(double yg, double api, double rsb,
                                            double t, double p)
{
  return CoaPetroskyFarshad::get_instance()(yg, api, rsb, t, p).raw();
}

Quantity<psia_1>
CoaVasquezBeggs__correlation__fct(const Quantity<Sgg>& yg,
                                  const Quantity<Api>& api,
                                  const Quantity<SCF_STB>& rsb,
                                  const Quantity<Fahrenheit>& t,
                                  const Quantity<Fahrenheit>& tsep,
                                  const Quantity<psia>& p,
                                  const Quantity<psia>& psep)
{
  return CoaVasquezBeggs::get_instance()(yg, api, rsb, t, tsep, p, psep);
}

double
CoaVasquezBeggs__correlation__valid__fct(double yg, double api, double rsb,
                                         double t, double tsep, double p,
                                         double psep)
{
  return CoaVasquezBeggs::get_instance()(yg, api, rsb, t, tsep, p, psep).raw();
}

Quantity<psia_1>
CoaPerezML__correlation__fct(const Quantity<Sgg>& yg, const Quantity<Api>& api,
                             const Quantity<SCF_STB>& rsb,
                             const Quantity<Fahrenheit>& t,
                             const Quantity<psia>& p, const Quantity<psia>& pb)
{
  return CoaPerezML::get_instance()(yg, api, rsb, t, p, pb);
}

double
CoaPerezML__correlation__valid__fct(double yg, double api, double rsb, double t,
                                    double p, double pb)
{
  return CoaPerezML::get_instance()(yg, api, rsb, t, p, pb).raw();
}

Quantity<psia_1>
CoaMillanArcia__correlation__fct(const Quantity<Api>& api,
                                 const Quantity<SCF_STB>& rsb,
                                 const Quantity<Fahrenheit>& t,
                                 const Quantity<psia>& p,
                                 const Quantity<psia>& pb)
{
  return CoaMillanArcia::get_instance()(api, rsb, t, p, pb);
}

double
CoaMillanArcia__correlation__valid__fct(double api, double rsb, double t,
                                        double p, double pb)
{
  return CoaMillanArcia::get_instance()(api, rsb, t, p, pb).raw();
}
