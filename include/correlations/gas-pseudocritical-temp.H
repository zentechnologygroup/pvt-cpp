#ifndef GAS_PSEUDOCRITICAL_TEMP_H
#define GAS_PSEUDOCRITICAL_TEMP_H

#include <pvt-exceptions.H>
#include <correlations/correlation.H>

/** TpchcStanding

    Generated at 2017-10-30 16:49:05 -0400

    Zen Technologies Group

Standing (1977) presented two temperature correlations: one for \"condensate\"
gases and one for \"California\" gases. Whitson & Brulé (2000) explained that
the correlation for \"condensate\" gases applies to wet or high molecular weight
gases, and the other one corresponds to dry hydrocarbon gases. Standing
recommends using the \"California\" curves for surface gases.
Brown et al. (1948) presented the graphical correlation. Standing (1977)
expressed it in a mathematical form.

The values of the development ranges are taken from the graphs presented by
Standing (1977) and Ahmed (1989).

Brown et al.'s reference is not available. The correlation was verified by using
these references: Standing (1977), Ahmed (1989), and Bánzer (1996). Date:
September 14 2016.

References:

  - standing:1977

  - brown:1948

  - ahmed:1989

  - whitson:2000

@param[in] yghc Gas gravity of the hydrocarbon portion. Range in [0.55, 1.67]
@return inside [345, 670]
@ingroup Correlations
*/
class TpchcStanding : public HydrocarbonGasPseudocriticalTemperature {
  Correlation_Singleton(TpchcStanding);

  TpchcStanding()
      : HydrocarbonGasPseudocriticalTemperature(
            "TpchcStanding", Rankine::get_instance(), 345, 670) {
    set_author("Standing");
    set_title("STANDING CORRELATION, CALCULATION OF PSEUDOCRITICAL TEMPERATURE "
              "OF THE DRY GAS HYDROCARBON PORTION");
    add_parameter("yghc", Sgg::get_instance(),
                  "Gas gravity of the hydrocarbon portion", 0.55, 1.67);
    def_parameter_latex_symbol("yghc", "\\gamma_{gHC}");
    add_db("Calculation of pseudocritical temperature of the dry gas "
           "hydrocarbon portion.");
    add_db("Based on 71 natural gases from California, which represent surface "
           "trap gases and stock-tank vapors whose high gravity results from "
           "relatively high proportions of ethane, propane, and butanes. The "
           "correlation was developed with samples containing concentrations "
           "of nitrogen, carbon dioxide, and hydrogen sulfide, up to 5 mol %, "
           "2 mol %, and 2 mol %, respectively.");
    add_note("Standing (1977) presented two temperature correlations: one for "
             "\"condensate\" gases and one for \"California\" gases. Whitson & "
             "Brulé (2000) explained that the correlation for \"condensate\" "
             "gases applies to wet or high molecular weight gases, and the "
             "other one corresponds to dry hydrocarbon gases. Standing "
             "recommends using the \"California\" curves for surface gases.");
    add_note("Brown et al. (1948) presented the graphical correlation. "
             "Standing (1977) expressed it in a mathematical form.");
    add_ref("standing:1977");
    add_ref("brown:1948");
    add_ref("ahmed:1989");
    add_ref("whitson:2000");
  }

public:
  static inline double impl(const double& yghc) noexcept;

  Quantity<Rankine> call(const Quantity<Sgg>& yghc) const {
    return Quantity<Rankine>(impl(yghc.raw()));
  }

  Quantity<Rankine> validate_and_call(const Quantity<Sgg>& yghc) const {
    if (not(yghc >= 0.55))
      ZENTHROW(OutOfParameterRange, "parameter yghc is less than 0.55");
    if (not(yghc <= 1.67))
      ZENTHROW(OutOfParameterRange, "parameter yghc is greater than 1.67");

    return call(yghc);
  }

  Quantity<Rankine> operator()(const Quantity<Sgg>& yghc) const {
    return Quantity<Rankine>(call(yghc));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yghc = it.get_curr();

    return call(yghc);
  }
};

extern Quantity<Rankine> TpchcStanding__correlation__fct(
    const Quantity<Sgg>& yghc);
extern double TpchcStanding__correlation__valid__fct(const double& yghc);

/** TpchcStandingHeavierFractions

    Generated at 2017-10-30 16:49:05 -0400

    Zen Technologies Group

Standing (1977) presented two temperature correlations: one for \"condensate\"
gases and one for \"California\" gases. Whitson & Brulé (2000) explained that
the correlation for \"condensate\" gases applies to wet or high molecular weight
gases, and the other one corresponds to dry hydrocarbon gases. Standing
recommends using the \"condensate\" curves in calculations that involve gases in
equilibrium with crude oil condensate in petroleum reservoirs.
Brown et al. (1948) presented the graphical correlation. Standing (1977)
expressed it in a mathematical form.

The values of the development ranges are taken from the graphs presented by
Standing (1977) and Ahmed (1989).

Brown et al.'s reference is not available. The correlation was verified by using
these references: Standing (1977), Ahmed (1989), and Bánzer (1996). Date:
September 14 2016.

References:

  - standing:1977

  - brown:1948

  - ahmed:1989

@param[in] yghc Gas gravity of the hydrocarbon portion. Range in [0.58, 1.3]
@return inside [350, 495]
@ingroup Correlations
*/
class TpchcStandingHeavierFractions
    : public HydrocarbonGasPseudocriticalTemperature {
  Correlation_Singleton(TpchcStandingHeavierFractions);

  TpchcStandingHeavierFractions()
      : HydrocarbonGasPseudocriticalTemperature("TpchcStandingHeavierFractions",
                                                Rankine::get_instance(), 350,
                                                495) {
    set_author("Standing (Heavier Fractions)");
    set_title("STANDING CORRELATION, CALCULATION OF PSEUDOCRITICAL TEMPERATURE "
              "OF THE WET OR \"CONDENSATE\" GAS HYDROCARBON PORTION");
    add_parameter("yghc", Sgg::get_instance(),
                  "Gas gravity of the hydrocarbon portion", 0.58, 1.3);
    def_parameter_latex_symbol("yghc", "\\gamma_{gHC}");
    add_db("Calculation of pseudocritical temperature of the wet or "
           "\"condensate\" gas hydrocarbon portion.");
    add_db("Based on laboratory generated gases in equilibrium with crude oil "
           "at high pressures (1000 to 8000 psia). Their high gravities come "
           "from relatively large quantities of heptanes and heavier "
           "compounds. The correlation was developed with samples containing "
           "concentrations of nitrogen, carbon dioxide, and hydrogen sulfide, "
           "up to 5 mol %, 2 mol %, and 2 mol %, respectively.");
    add_note("Standing (1977) presented two temperature correlations: one for "
             "\"condensate\" gases and one for \"California\" gases. Whitson & "
             "Brulé (2000) explained that the correlation for \"condensate\" "
             "gases applies to wet or high molecular weight gases, and the "
             "other one corresponds to dry hydrocarbon gases. Standing "
             "recommends using the \"condensate\" curves in calculations that "
             "involve gases in equilibrium with crude oil condensate in "
             "petroleum reservoirs.");
    add_note("Brown et al. (1948) presented the graphical correlation. "
             "Standing (1977) expressed it in a mathematical form.");
    add_ref("standing:1977");
    add_ref("brown:1948");
    add_ref("ahmed:1989");
  }

public:
  static inline double impl(const double& yghc) noexcept;

  Quantity<Rankine> call(const Quantity<Sgg>& yghc) const {
    return Quantity<Rankine>(impl(yghc.raw()));
  }

  Quantity<Rankine> validate_and_call(const Quantity<Sgg>& yghc) const {
    if (not(yghc >= 0.58))
      ZENTHROW(OutOfParameterRange, "parameter yghc is less than 0.58");
    if (not(yghc <= 1.3))
      ZENTHROW(OutOfParameterRange, "parameter yghc is greater than 1.3");

    return call(yghc);
  }

  Quantity<Rankine> operator()(const Quantity<Sgg>& yghc) const {
    return Quantity<Rankine>(call(yghc));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yghc = it.get_curr();

    return call(yghc);
  }
};

extern Quantity<Rankine> TpchcStandingHeavierFractions__correlation__fct(
    const Quantity<Sgg>& yghc);
extern double TpchcStandingHeavierFractions__correlation__valid__fct(
    const double& yghc);

/** TpchcSutton

    Generated at 2017-10-30 16:49:05 -0400

    Zen Technologies Group

Sutton's correlation was developed based on samples of gases with no hydrogen
sulfide content. The samples contained nitrogen (0 mol % to 2.86 mol %), carbon
dioxide (0.01 mol % to 11.86 mol %), and heptanes-plus (0.02 mol % to 14.27 mol
%).

The ranges for Yg, n2Concentration, and co2Concentration, are presented as the
ranges of data covered by the total number of PVT reports. The range for Psc was
taken from the graph of the variation of pseudocritical pressure with gas
gravity, presented by Sutton.

References:

  - sutton:1985

@param[in] yghc Gas gravity of the hydrocarbon portion. Range in [0.571, 1.679]
@return inside [330, 575]
@ingroup Correlations
*/
class TpchcSutton : public HydrocarbonGasPseudocriticalTemperature {
  Correlation_Singleton(TpchcSutton);

  TpchcSutton()
      : HydrocarbonGasPseudocriticalTemperature(
            "TpchcSutton", Rankine::get_instance(), 330, 575) {
    set_author("Sutton");
    set_title("SUTTON CORRELATION, CALCULATION OF PSEUDOCRITICAL TEMPERATURE "
              "OF THE NATURAL GAS HYDROCARBON PORTION");
    add_parameter("yghc", Sgg::get_instance(),
                  "Gas gravity of the hydrocarbon portion", 0.571, 1.679);
    def_parameter_latex_symbol("yghc", "\\gamma_{gHC}");
    add_db("Calculation of pseudocritical temperature of the natural gas "
           "hydrocarbon portion.");
    add_db("Based on 264 samples of natural gases with high molecular weight, "
           "rich in heptanes, with minor amounts of nitrogen and carbon "
           "dioxide, and without hydrogen sulfide. These 264 compositions were "
           "obtained from a data bank of 275 PVT reports and 634 compositions "
           "in total. The producing areas represented by this data bank are: "
           "Gulf of Mexico, Lousiana, Texas, and others.");
    add_note("Sutton's correlation was developed based on samples of gases "
             "with no hydrogen sulfide content. The samples contained nitrogen "
             "(0 mol % to 2.86 mol %), carbon dioxide (0.01 mol % to 11.86 mol "
             "%), and heptanes-plus (0.02 mol % to 14.27 mol %).");
    add_ref("sutton:1985");
  }

public:
  static inline double impl(const double& yghc) noexcept;

  Quantity<Rankine> call(const Quantity<Sgg>& yghc) const {
    return Quantity<Rankine>(impl(yghc.raw()));
  }

  Quantity<Rankine> validate_and_call(const Quantity<Sgg>& yghc) const {
    if (not(yghc >= 0.571))
      ZENTHROW(OutOfParameterRange, "parameter yghc is less than 0.571");
    if (not(yghc <= 1.679))
      ZENTHROW(OutOfParameterRange, "parameter yghc is greater than 1.679");

    return call(yghc);
  }

  Quantity<Rankine> operator()(const Quantity<Sgg>& yghc) const {
    return Quantity<Rankine>(call(yghc));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yghc = it.get_curr();

    return call(yghc);
  }
};

extern Quantity<Rankine> TpchcSutton__correlation__fct(
    const Quantity<Sgg>& yghc);
extern double TpchcSutton__correlation__valid__fct(const double& yghc);

/** TpchcElsharkawyEtAl

    Generated at 2017-10-30 16:49:05 -0400

    Zen Technologies Group

The correlation was developed with samples containing concentrations of
nitrogen, carbon dioxide, and hydrogen sulfide, up to 12.84 mol %, 67.12 mol %,
and 31.82 mol %, respectively. The samples also contained heptanes-plus in the
range from 0.22 mol % to 17.2 mol %.

The correlation was verified by using the original reference and a secondary
one: Elsharkawy & Elkamel (2000). Date: September 22 2016.

References:

  - elsharkawy:2000:1

  - elsharkawy:2000:2

@param[in] yghc Gas gravity of the hydrocarbon portion. Range in [0.61, 1.89]
@return inside [, ]
@ingroup Correlations
*/
class TpchcElsharkawyEtAl : public HydrocarbonGasPseudocriticalTemperature {
  Correlation_Singleton(TpchcElsharkawyEtAl);

  TpchcElsharkawyEtAl()
      : HydrocarbonGasPseudocriticalTemperature("TpchcElsharkawyEtAl",
                                                Rankine::get_instance()) {
    set_author("Elsharkawy et al.");
    set_title("ELSHARKAWY ET AL. CORRELATION, CALCULATION OF PSEUDOCRITICAL "
              "TEMPERATURE OF THE CONDENSATE GAS HYDROCARBON PORTION");
    add_parameter("yghc", Sgg::get_instance(),
                  "Gas gravity of the hydrocarbon portion", 0.61, 1.89);
    def_parameter_latex_symbol("yghc", "\\gamma_{gHC}");
    add_db("Calculation of pseudocritical temperature of the \"condensate\" "
           "gas hydrocarbon portion.");
    add_db("Based on compositional analysis of 1200 compositions of gas "
           "condensates collected worldwide.");
    add_note("The correlation was developed with samples containing "
             "concentrations of nitrogen, carbon dioxide, and hydrogen "
             "sulfide, up to 12.84 mol %, 67.12 mol %, and 31.82 mol %, "
             "respectively. The samples also contained heptanes-plus in the "
             "range from 0.22 mol % to 17.2 mol %.");
    add_ref("elsharkawy:2000:1");
    add_ref("elsharkawy:2000:2");
  }

public:
  static inline double impl(const double& yghc) noexcept;

  Quantity<Rankine> call(const Quantity<Sgg>& yghc) const {
    return Quantity<Rankine>(impl(yghc.raw()));
  }

  Quantity<Rankine> validate_and_call(const Quantity<Sgg>& yghc) const {
    if (not(yghc >= 0.61))
      ZENTHROW(OutOfParameterRange, "parameter yghc is less than 0.61");
    if (not(yghc <= 1.89))
      ZENTHROW(OutOfParameterRange, "parameter yghc is greater than 1.89");

    return call(yghc);
  }

  Quantity<Rankine> operator()(const Quantity<Sgg>& yghc) const {
    return Quantity<Rankine>(call(yghc));
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& pars,
                              bool check = true) const {
    if (check)
      verify_preconditions(pars);

    auto it = pars.get_it();
    const auto& yghc = it.get_curr();

    return call(yghc);
  }
};

extern Quantity<Rankine> TpchcElsharkawyEtAl__correlation__fct(
    const Quantity<Sgg>& yghc);
extern double TpchcElsharkawyEtAl__correlation__valid__fct(const double& yghc);

#endif // GAS_PSEUDOCRITICAL_TEMP_H
