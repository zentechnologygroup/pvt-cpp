Quantity<RB_STB>
BoaMcCain__correlation__fct(const Quantity<RB_STB>& bobp,
                            const Quantity<psia>& p, const Quantity<psia>& pb,
                            const Quantity<psia_1>& co)
{
  return BoaMcCain::get_instance()(bobp, p, pb, co);
}

double
BoaMcCain__correlation__valid__fct(double bobp, double p, double pb, double co)
{
  return BoaMcCain::get_instance()(bobp, p, pb, co).raw();
}

Quantity<RB_STB>
BoaDeGhetto__correlation__fct(const Quantity<RB_STB>& bobp,
                              const Quantity<Sgg>& yg, const Quantity<Api>& api,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Fahrenheit>& t,
                              const Quantity<Fahrenheit>& tsep,
                              const Quantity<psia>& p, const Quantity<psia>& pb,
                              const Quantity<psia>& psep)
{
  return BoaDeGhetto::get_instance()(bobp, yg, api, rsb, t, tsep, p, pb, psep);
}

double
BoaDeGhetto__correlation__valid__fct(double bobp, double yg, double api,
                                     double rsb, double t, double tsep,
                                     double p, double pb, double psep)
{
  return BoaDeGhetto::get_instance()(bobp, yg, api, rsb, t, tsep, p, pb, psep)
    .raw();
}

Quantity<RB_STB>
BoaHanafy__correlation__fct(const Quantity<RB_STB>& bobp,
                            const Quantity<psia>& p, const Quantity<psia>& pb)
{
  return BoaHanafy::get_instance()(bobp, p, pb);
}

double
BoaHanafy__correlation__valid__fct(double bobp, double p, double pb)
{
  return BoaHanafy::get_instance()(bobp, p, pb).raw();
}

Quantity<RB_STB>
BoaKartoatmodjo__correlation__fct(
  const Quantity<RB_STB>& bobp, const Quantity<Sgg>& yg,
  const Quantity<Sg_do>& yo, const Quantity<SCF_STB>& rsb,
  const Quantity<Fahrenheit>& t, const Quantity<Fahrenheit>& tsep,
  const Quantity<psia>& p, const Quantity<psia>& pb, const Quantity<psia>& psep)
{
  return BoaKartoatmodjo::get_instance()(bobp, yg, yo, rsb, t, tsep, p, pb,
                                         psep);
}

double
BoaKartoatmodjo__correlation__valid__fct(double bobp, double yg, double yo,
                                         double rsb, double t, double tsep,
                                         double p, double pb, double psep)
{
  return BoaKartoatmodjo::get_instance()(bobp, yg, yo, rsb, t, tsep, p, pb,
                                         psep)
    .raw();
}

Quantity<RB_STB>
BoaPetroskyFarshad__correlation__fct(const Quantity<RB_STB>& bobp,
                                     const Quantity<Sgg>& yg,
                                     const Quantity<Sg_do>& yo,
                                     const Quantity<SCF_STB>& rsb,
                                     const Quantity<Fahrenheit>& t,
                                     const Quantity<psia>& p,
                                     const Quantity<psia>& pb)
{
  return BoaPetroskyFarshad::get_instance()(bobp, yg, yo, rsb, t, p, pb);
}

double
BoaPetroskyFarshad__correlation__valid__fct(double bobp, double yg, double yo,
                                            double rsb, double t, double p,
                                            double pb)
{
  return BoaPetroskyFarshad::get_instance()(bobp, yg, yo, rsb, t, p, pb).raw();
}

Quantity<RB_STB>
BoaVasquezBeggs__correlation__fct(
  const Quantity<RB_STB>& bobp, const Quantity<Sgg>& yg,
  const Quantity<Api>& api, const Quantity<SCF_STB>& rsb,
  const Quantity<Fahrenheit>& t, const Quantity<Fahrenheit>& tsep,
  const Quantity<psia>& p, const Quantity<psia>& pb, const Quantity<psia>& psep)
{
  return BoaVasquezBeggs::get_instance()(bobp, yg, api, rsb, t, tsep, p, pb,
                                         psep);
}

double
BoaVasquezBeggs__correlation__valid__fct(double bobp, double yg, double api,
                                         double rsb, double t, double tsep,
                                         double p, double pb, double psep)
{
  return BoaVasquezBeggs::get_instance()(bobp, yg, api, rsb, t, tsep, p, pb,
                                         psep)
    .raw();
}

Quantity<RB_STB>
BoaPerezML__correlation__fct(const Quantity<RB_STB>& bobp,
                             const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
                             const Quantity<SCF_STB>& rsb,
                             const Quantity<Fahrenheit>& t,
                             const Quantity<psia>& p, const Quantity<psia>& pb)
{
  return BoaPerezML::get_instance()(bobp, yg, yo, rsb, t, p, pb);
}

double
BoaPerezML__correlation__valid__fct(double bobp, double yg, double yo,
                                    double rsb, double t, double p, double pb)
{
  return BoaPerezML::get_instance()(bobp, yg, yo, rsb, t, p, pb).raw();
}

Quantity<RB_STB>
BoaMillanArcia__correlation__fct(const Quantity<RB_STB>& bobp,
                                 const Quantity<Api>& api,
                                 const Quantity<SCF_STB>& rsb,
                                 const Quantity<Fahrenheit>& t,
                                 const Quantity<psia>& p,
                                 const Quantity<psia>& pb)
{
  return BoaMillanArcia::get_instance()(bobp, api, rsb, t, p, pb);
}

double
BoaMillanArcia__correlation__valid__fct(double bobp, double api, double rsb,
                                        double t, double p, double pb)
{
  return BoaMillanArcia::get_instance()(bobp, api, rsb, t, p, pb).raw();
}
