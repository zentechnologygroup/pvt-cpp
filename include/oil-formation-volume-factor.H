#ifndef OIL_FORMATION_VOLUME_FACTOR_H
#define OIL_FORMATION_VOLUME_FACTOR_H

#include <correlation.H>

/** BoAlmarhoun

    AL-MARHOUN CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR

    Based on 69 bottomhole fluid samples from 69 Middle East oil reservoirs.

    @see: Al-Marhoun, M.A.: "PVT Correlation for Middle East Crude
    Oils", Journal Petroleum Technology, May 1988, pp. 650-666.

    @see: Bánzer, C.: "Correlaciones Numéricas P.V.T.", Universidad
    del Zulia, 1996.

    @see: McCain, W.D., Jr. "The Properties of Petroleum Fluids", 2nd
    ed. Tulsa, OK: PennWell Books, 1990.


*/
class BoAlmarhoun : public OilFormationVolumeFactor
{
  Correlation_Singleton(BoAlmarhoun);

  BoAlmarhoun()
    : OilFormationVolumeFactor("BoAlmarhoun", RB_STB::get_instance(), 1.032,
                               1.997)
  {
    add_parameter("yg", Sgg::get_instance(), 0.752, 1.367);
    add_parameter("yo", Sg_do::get_instance(), Quantity<Api>(44.6),
                  Quantity<Api>(19.4));
    add_parameter("rs", SCF_STB::get_instance(), 26, 1602);
    add_parameter("rsb", SCF_STB::get_instance());
    add_parameter("t", Rankine::get_instance(), Quantity<Fahrenheit>(100),
                  Quantity<Fahrenheit>(258));
    add_parameter("p", psia::get_instance());
    add_parameter("pb", psia::get_instance(), 130, 3573);
    add_parameter("co", psia_1::get_instance());
  }

public:
  inline Quantity<RB_STB> impl(
    const Quantity<Sgg>& yg, const Quantity<Sg_do>& yo,
    const Quantity<SCF_STB>& rs, const Quantity<SCF_STB>& rsb,
    const Quantity<Rankine>& t, const Quantity<psia>& p,
    const Quantity<psia>& pb, const Quantity<psia_1>& co) const;

  Quantity<RB_STB> operator()(const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rs,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Rankine>& t,
                              const Quantity<psia>& p, const Quantity<psia>& pb,
                              const Quantity<psia_1>& co) const
  {
    return compute({ yg, yo, rs, rsb, t, p, pb, co });
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars) const
  {
    auto pars = verify_preconditions(__pars);

    auto it = pars.get_it();
    const auto& yg = it.get_curr();
    it.next();
    const auto& yo = it.get_curr();
    it.next();
    const auto& rs = it.get_curr();
    it.next();
    const auto& rsb = it.get_curr();
    it.next();
    const auto& t = it.get_curr();
    it.next();
    const auto& p = it.get_curr();
    it.next();
    const auto& pb = it.get_curr();
    it.next();
    const auto& co = it.get_curr();

    return impl(yg, yo, rs, rsb, t, p, pb, co);
  }
};

#endif // OIL_FORMATION_VOLUME_FACTOR_H
