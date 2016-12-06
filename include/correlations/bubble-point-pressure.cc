Quantity<psia>
PbAlMarhoun__correlation__fct(const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Rankine>& t)
{
  return PbAlMarhoun::get_instance()(yg, yo, rsb, t);
}

double
PbAlMarhoun__correlation__valid__fct(double yg, double yo, double rsb, double t)
{
  return PbAlMarhoun::get_instance()(yg, yo, rsb, t).raw();
}

Quantity<psia>
PbAlShammasi__correlation__fct(const Quantity<Sgg>& yg,
                               const Quantity<Sg_do>& yo,
                               const Quantity<SCF_STB>& rsb,
                               const Quantity<Fahrenheit>& t)
{
  return PbAlShammasi::get_instance()(yg, yo, rsb, t);
}

double
PbAlShammasi__correlation__valid__fct(double yg, double yo, double rsb,
                                      double t)
{
  return PbAlShammasi::get_instance()(yg, yo, rsb, t).raw();
}

Quantity<psia>
PbDeGhetto__correlation__fct(const Quantity<Sgg>& yg,
                             const Quantity<SCF_STB>& rsb,
                             const Quantity<Api>& api,
                             const Quantity<Fahrenheit>& t)
{
  return PbDeGhetto::get_instance()(yg, rsb, api, t);
}

double
PbDeGhetto__correlation__valid__fct(double yg, double rsb, double api, double t)
{
  return PbDeGhetto::get_instance()(yg, rsb, api, t).raw();
}

Quantity<psia>
PbDindorukChristman__correlation__fct(const Quantity<Sgg>& yg,
                                      const Quantity<SCF_STB>& rsb,
                                      const Quantity<Api>& api,
                                      const Quantity<Fahrenheit>& t)
{
  return PbDindorukChristman::get_instance()(yg, rsb, api, t);
}

double
PbDindorukChristman__correlation__valid__fct(double yg, double rsb, double api,
                                             double t)
{
  return PbDindorukChristman::get_instance()(yg, rsb, api, t).raw();
}

Quantity<psia>
PbDoklaOsman__correlation__fct(const Quantity<Sgg>& yg,
                               const Quantity<SCF_STB>& rsb,
                               const Quantity<Sg_do>& yo,
                               const Quantity<Rankine>& t)
{
  return PbDoklaOsman::get_instance()(yg, rsb, yo, t);
}

double
PbDoklaOsman__correlation__valid__fct(double yg, double rsb, double yo,
                                      double t)
{
  return PbDoklaOsman::get_instance()(yg, rsb, yo, t).raw();
}

Quantity<psia>
PbHanafy__correlation__fct(const Quantity<SCF_STB>& rsb)
{
  return PbHanafy::get_instance()(rsb);
}

double
PbHanafy__correlation__valid__fct(double rsb)
{
  return PbHanafy::get_instance()(rsb).raw();
}

Quantity<psia>
PbKartoatmodjoSchmidt__correlation__fct(const Quantity<Sgg>& yg,
                                        const Quantity<SCF_STB>& rsb,
                                        const Quantity<Api>& api,
                                        const Quantity<Fahrenheit>& t,
                                        const Quantity<Fahrenheit>& tsep,
                                        const Quantity<psia>& psep)
{
  return PbKartoatmodjoSchmidt::get_instance()(yg, rsb, api, t, tsep, psep);
}

double
PbKartoatmodjoSchmidt__correlation__valid__fct(double yg, double rsb,
                                               double api, double t,
                                               double tsep, double psep)
{
  return PbKartoatmodjoSchmidt::get_instance()(yg, rsb, api, t, tsep, psep)
    .raw();
}

Quantity<psia>
PbLasater__correlation__fct(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Sg_do>& yo,
                            const Quantity<Fahrenheit>& t)
{
  return PbLasater::get_instance()(yg, rsb, yo, t);
}

double
PbLasater__correlation__valid__fct(double yg, double rsb, double yo, double t)
{
  return PbLasater::get_instance()(yg, rsb, yo, t).raw();
}

Quantity<psia>
PbPetroskyFarshad__correlation__fct(const Quantity<Sgg>& yg,
                                    const Quantity<SCF_STB>& rsb,
                                    const Quantity<Api>& api,
                                    const Quantity<Fahrenheit>& t)
{
  return PbPetroskyFarshad::get_instance()(yg, rsb, api, t);
}

double
PbPetroskyFarshad__correlation__valid__fct(double yg, double rsb, double api,
                                           double t)
{
  return PbPetroskyFarshad::get_instance()(yg, rsb, api, t).raw();
}

Quantity<psia>
PbStanding__correlation__fct(const Quantity<Sgg>& yg,
                             const Quantity<SCF_STB>& rsb,
                             const Quantity<Api>& api,
                             const Quantity<Fahrenheit>& t)
{
  return PbStanding::get_instance()(yg, rsb, api, t);
}

double
PbStanding__correlation__valid__fct(double yg, double rsb, double api, double t)
{
  return PbStanding::get_instance()(yg, rsb, api, t).raw();
}

Quantity<psia>
PbTotalCFP__correlation__fct(const Quantity<Sgg>& yg,
                             const Quantity<SCF_STB>& rsb,
                             const Quantity<Api>& api,
                             const Quantity<Fahrenheit>& t)
{
  return PbTotalCFP::get_instance()(yg, rsb, api, t);
}

double
PbTotalCFP__correlation__valid__fct(double yg, double rsb, double api, double t)
{
  return PbTotalCFP::get_instance()(yg, rsb, api, t).raw();
}

Quantity<psia>
PbVasquezBeggs__correlation__fct(const Quantity<Sgg>& yg,
                                 const Quantity<SCF_STB>& rsb,
                                 const Quantity<Api>& api,
                                 const Quantity<Fahrenheit>& t,
                                 const Quantity<Fahrenheit>& tsep,
                                 const Quantity<psia>& psep)
{
  return PbVasquezBeggs::get_instance()(yg, rsb, api, t, tsep, psep);
}

double
PbVasquezBeggs__correlation__valid__fct(double yg, double rsb, double api,
                                        double t, double tsep, double psep)
{
  return PbVasquezBeggs::get_instance()(yg, rsb, api, t, tsep, psep).raw();
}

Quantity<psia>
PbVelarde__correlation__fct(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t)
{
  return PbVelarde::get_instance()(yg, rsb, api, t);
}

double
PbVelarde__correlation__valid__fct(double yg, double rsb, double api, double t)
{
  return PbVelarde::get_instance()(yg, rsb, api, t).raw();
}

Quantity<psia>
PbCegarra__correlation__fct(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t)
{
  return PbCegarra::get_instance()(yg, rsb, api, t);
}

double
PbCegarra__correlation__valid__fct(double yg, double rsb, double api, double t)
{
  return PbCegarra::get_instance()(yg, rsb, api, t).raw();
}

Quantity<psia>
PbPerezML__correlation__fct(const Quantity<Sgg>& yg,
                            const Quantity<SCF_STB>& rsb,
                            const Quantity<Api>& api,
                            const Quantity<Fahrenheit>& t)
{
  return PbPerezML::get_instance()(yg, rsb, api, t);
}

double
PbPerezML__correlation__valid__fct(double yg, double rsb, double api, double t)
{
  return PbPerezML::get_instance()(yg, rsb, api, t).raw();
}

Quantity<psia>
PbMillanArcia__correlation__fct(const Quantity<Sgg>& yg,
                                const Quantity<SCF_STB>& rsb,
                                const Quantity<Api>& api,
                                const Quantity<Fahrenheit>& t)
{
  return PbMillanArcia::get_instance()(yg, rsb, api, t);
}

double
PbMillanArcia__correlation__valid__fct(double yg, double rsb, double api,
                                       double t)
{
  return PbMillanArcia::get_instance()(yg, rsb, api, t).raw();
}

Quantity<psia>
PbManucciRosales__correlation__fct(const Quantity<Sgg>& yg,
                                   const Quantity<SCF_STB>& rsb,
                                   const Quantity<Api>& api,
                                   const Quantity<Fahrenheit>& t)
{
  return PbManucciRosales::get_instance()(yg, rsb, api, t);
}

double
PbManucciRosales__correlation__valid__fct(double yg, double rsb, double api,
                                          double t)
{
  return PbManucciRosales::get_instance()(yg, rsb, api, t).raw();
}

Quantity<psia>
PbGlaso__correlation__fct(const Quantity<Sgg>& yg, const Quantity<SCF_STB>& rsb,
                          const Quantity<Api>& api,
                          const Quantity<Fahrenheit>& t,
                          const Quantity<MoleFraction>& n2_concentration,
                          const Quantity<MoleFraction>& co2_concentration,
                          const Quantity<MoleFraction>& h2s_concentration)
{
  return PbGlaso::get_instance()(yg, rsb, api, t, n2_concentration,
                                 co2_concentration, h2s_concentration);
}

double
PbGlaso__correlation__valid__fct(double yg, double rsb, double api, double t,
                                 double n2_concentration,
                                 double co2_concentration,
                                 double h2s_concentration)
{
  return PbGlaso::get_instance()(yg, rsb, api, t, n2_concentration,
                                 co2_concentration, h2s_concentration)
    .raw();
}
