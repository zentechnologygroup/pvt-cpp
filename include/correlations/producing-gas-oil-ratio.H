#ifndef PRODUCING_GAS_OIL_RATIO_H
#define PRODUCING_GAS_OIL_RATIO_H

#include <pvt-exceptions.H>
#include <correlations/correlation.H>

/** Rsp1

    Generated at 2017-10-30 16:49:06 -0400

    Zen Technologies Group

Calculation of producing Gas Oil Ratio (GOR).

Producing GOR is the inverse of Condensate gas ratio.

References:

@param[in] ogr Primary sparator condensate gas ratio. Range in [, ]
@return inside [, ]
@ingroup Correlations
*/
class Rsp1 : public ProducingGasOilRatio {
  Correlation_Singleton(Rsp1);

  Rsp1() : ProducingGasOilRatio("Rsp1", SCF_STB::get_instance()) {
    set_author("Standard Equation");
    set_title("CALCULATION OF PRODUCING GAS OIL RATIO (GOR)");
    set_hidden();
    add_parameter("ogr", STB_MMscf::get_instance(),
                  "Primary sparator condensate gas ratio");
    def_parameter_latex_symbol("ogr", "r_{s-sp1}");
    add_note("Calculation of producing Gas Oil Ratio (GOR).");
  }

public:
  static inline double impl(const double& ogr) noexcept;

  Quantity<SCF_STB> call(const Quantity<STB_MMscf>& ogr) const {
    return Quantity<SCF_STB>(impl(ogr.raw()));
  }

  Quantity<SCF_STB> validate_and_call(const Quantity<STB_MMscf>& ogr) const {

    return call(ogr);
  }

  Quantity<SCF_STB> operator()(const Quantity<STB_MMscf>& ogr) const {
    return Quantity<SCF_STB>(call(ogr));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& ogr = it.get_curr();

    return call(ogr);
  }
};

extern Quantity<SCF_STB> Rsp1__correlation__fct(const Quantity<STB_MMscf>& ogr);
extern double Rsp1__correlation__valid__fct(const double& ogr);

#endif // PRODUCING_GAS_OIL_RATIO_H
