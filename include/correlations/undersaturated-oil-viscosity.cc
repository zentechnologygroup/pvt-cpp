Quantity<CP>
UoaKartoatmodjoSchmidt__correlation__fct(const Quantity<CP>& uobp,
                                         const Quantity<psia>& pb,
                                         const Quantity<psia>& p)
{
  return UoaKartoatmodjoSchmidt::get_instance()(uobp, pb, p);
}

double
UoaKartoatmodjoSchmidt__correlation__valid__fct(double uobp, double pb,
                                                double p)
{
  return UoaKartoatmodjoSchmidt::get_instance()(uobp, pb, p).raw();
}

Quantity<CP>
UoaDeGhettoEtAl__correlation__fct(const Quantity<CP>& uobp,
                                  const Quantity<psia>& p,
                                  const Quantity<psia>& pb,
                                  const Quantity<CP>& uod,
                                  const Quantity<Api>& api)
{
  return UoaDeGhettoEtAl::get_instance()(uobp, p, pb, uod, api);
}

double
UoaDeGhettoEtAl__correlation__valid__fct(double uobp, double p, double pb,
                                         double uod, double api)
{
  return UoaDeGhettoEtAl::get_instance()(uobp, p, pb, uod, api).raw();
}

Quantity<CP>
UoaBeal__correlation__fct(const Quantity<CP>& uobp, const Quantity<psia>& p,
                          const Quantity<psia>& pb)
{
  return UoaBeal::get_instance()(uobp, p, pb);
}

double
UoaBeal__correlation__valid__fct(double uobp, double p, double pb)
{
  return UoaBeal::get_instance()(uobp, p, pb).raw();
}

Quantity<CP>
UoaVasquezBeggs__correlation__fct(const Quantity<CP>& uobp,
                                  const Quantity<psia>& p,
                                  const Quantity<psia>& pb)
{
  return UoaVasquezBeggs::get_instance()(uobp, p, pb);
}

double
UoaVasquezBeggs__correlation__valid__fct(double uobp, double p, double pb)
{
  return UoaVasquezBeggs::get_instance()(uobp, p, pb).raw();
}

Quantity<CP>
UoaKhan__correlation__fct(const Quantity<CP>& uobp, const Quantity<psia>& p,
                          const Quantity<psia>& pb)
{
  return UoaKhan::get_instance()(uobp, p, pb);
}

double
UoaKhan__correlation__valid__fct(double uobp, double p, double pb)
{
  return UoaKhan::get_instance()(uobp, p, pb).raw();
}

Quantity<CP>
UoaPetroskyFarshad__correlation__fct(const Quantity<CP>& uobp,
                                     const Quantity<psia>& p,
                                     const Quantity<psia>& pb)
{
  return UoaPetroskyFarshad::get_instance()(uobp, p, pb);
}

double
UoaPetroskyFarshad__correlation__valid__fct(double uobp, double p, double pb)
{
  return UoaPetroskyFarshad::get_instance()(uobp, p, pb).raw();
}

Quantity<CP>
UoaAbedini__correlation__fct(const Quantity<CP>& uobp, const Quantity<psia>& p,
                             const Quantity<psia>& pb)
{
  return UoaAbedini::get_instance()(uobp, p, pb);
}

double
UoaAbedini__correlation__valid__fct(double uobp, double p, double pb)
{
  return UoaAbedini::get_instance()(uobp, p, pb).raw();
}

Quantity<CP>
UoaPerezML__correlation__fct(const Quantity<CP>& uobp, const Quantity<psia>& p,
                             const Quantity<psia>& pb)
{
  return UoaPerezML::get_instance()(uobp, p, pb);
}

double
UoaPerezML__correlation__valid__fct(double uobp, double p, double pb)
{
  return UoaPerezML::get_instance()(uobp, p, pb).raw();
}

Quantity<CP>
UoaDindorukChristman__correlation__fct(const Quantity<CP>& uobp,
                                       const Quantity<psia>& p,
                                       const Quantity<psia>& pb,
                                       const Quantity<SCF_STB>& rsb)
{
  return UoaDindorukChristman::get_instance()(uobp, p, pb, rsb);
}

double
UoaDindorukChristman__correlation__valid__fct(double uobp, double p, double pb,
                                              double rsb)
{
  return UoaDindorukChristman::get_instance()(uobp, p, pb, rsb).raw();
}
