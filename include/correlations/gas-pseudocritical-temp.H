#ifndef GAS_PSEUDOCRITICAL_TEMP_H
#define GAS_PSEUDOCRITICAL_TEMP_H

#include <correlations/correlation.H>

/** TscHCStanding

The values of the development ranges are taken from the graphs presented by
Standing (1977) and Ahmed (1989).

Brown et al.'s reference is not available. The correlation was verified by using
these references: Standing (1977), Ahmed (1989), and Bánzer (1996). Date:
September 14 2016.


*/
class TscHCStanding : public GasPseudocriticalTemperature
{
  Correlation_Singleton(TscHCStanding);

  TscHCStanding()
    : GasPseudocriticalTemperature("TscHCStanding", Rankine::get_instance(),
                                   345, 670)
  {
    set_author("Standing (HC Portion)");
    set_title("STANDING CORRELATION, CALCULATION OF PSEUDOCRITICAL TEMPERATURE "
              "OF THE DRY GAS HYDROCARBON PORTION");
    add_parameter("yghc", Sgg::get_instance(),
                  "Gas gravity of the hydrocarbon portion", 0.55, 1.67);
    def_parameter_latex_symbol("yghc", "\\gamma_{gHC}");
    add_db("Calculation of pseudocritical temperature of the dry gas "
           "hydrocarbon portion.");
    add_db("Based on 71 natural gases from California, which represent surface "
           "trap gases and stock-tank vapors whose high gravity results from "
           "relatively high proportions of ethane, propane, and butanes.");
    add_db(
      "Standing recommends using the \"California\" curves for surface gases.");
    add_note("Brown et al. (1948) presented the graphical correlation. "
             "Standing (1977) expressed it in a mathematical form.");
    add_note("The correlation was developed with samples containing "
             "concentrations of nitrogen, carbon dioxide, and hydrogen "
             "sulfide, up to 5 mol %, 2 mol %, and 2 mol %, respectively.");
    add_note("Standing (1977) presented two pressure correlations: one for "
             "\"condensate\" gases and one for \"California\" gases. Whitson & "
             "Brulé (2000) explained that the correlation for \"condensate\" "
             "gases applies to wet or high molecular weight gases, and the "
             "other one corresponds to dry hydrocarbon gases.");
    add_ref("standing:1977");
    add_ref("brown:1948");
    add_ref("ahmed:1989");
    add_ref("whitson:2000");
  }

public:
  inline Quantity<Rankine> impl(const Quantity<Sgg>& yghc) const;

  Quantity<Rankine> operator()(const Quantity<Sgg>& yghc) const
  {
    return impl(yghc);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yghc = it.get_curr();

      return impl(yghc);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yghc = it.get_curr();

    return impl(yghc);
  }
};

extern Quantity<Rankine> TscHCStanding__correlation__fct(
  const Quantity<Sgg>& yghc);
extern double TscHCStanding__correlation__valid__fct(double yghc);

/** TscHCStandingHeavierFractions

The values of the development ranges are taken from the graphs presented by
Standing (1977) and Ahmed (1989).

Brown et al.'s reference is not available. The correlation was verified by using
these references: Standing (1977), Ahmed (1989), and Bánzer (1996). Date:
September 14 2016.


*/
class TscHCStandingHeavierFractions : public GasPseudocriticalTemperature
{
  Correlation_Singleton(TscHCStandingHeavierFractions);

  TscHCStandingHeavierFractions()
    : GasPseudocriticalTemperature("TscHCStandingHeavierFractions",
                                   Rankine::get_instance(), 350, 495)
  {
    set_author("Standing (HC Portion for Heavier Fractions)");
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
           "compounds.");
    add_db("Standing recommends using the \"condensate\" curves in "
           "calculations that involve gases in equilibrium with crude oil "
           "condensate in petroleum reservoirs.");
    add_note("Brown et al. (1948) presented the graphical correlation. "
             "Standing (1977) expressed it in a mathematical form.");
    add_note("Standing (1977) presented two pressure correlations: one for "
             "\"condensate\" gases and one for \"California\" gases. Whitson & "
             "Brulé (2000) explained that the correlation for \"condensate\" "
             "gases applies to wet or high molecular weight gases, and the "
             "other one corresponds to dry hydrocarbon gases.");
    add_note("The correlation was developed with samples containing "
             "concentrations of nitrogen, carbon dioxide, and hydrogen "
             "sulfide, up to 5 mol %, 2 mol %, and 2 mol %, respectively.");
    add_ref("standing:1977");
    add_ref("brown:1948");
    add_ref("ahmed:1989");
  }

public:
  inline Quantity<Rankine> impl(const Quantity<Sgg>& yghc) const;

  Quantity<Rankine> operator()(const Quantity<Sgg>& yghc) const
  {
    return impl(yghc);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yghc = it.get_curr();

      return impl(yghc);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yghc = it.get_curr();

    return impl(yghc);
  }
};

extern Quantity<Rankine> TscHCStandingHeavierFractions__correlation__fct(
  const Quantity<Sgg>& yghc);
extern double TscHCStandingHeavierFractions__correlation__valid__fct(
  double yghc);

/** TscHCSutton

The ranges for Yg, n2Concentration, and co2Concentration, are presented as the
ranges of data covered by the total number of PVT reports. The range for Psc was
taken from the graph of the variation of pseudocritical pressure with gas
gravity, presented by Sutton.


*/
class TscHCSutton : public GasPseudocriticalTemperature
{
  Correlation_Singleton(TscHCSutton);

  TscHCSutton()
    : GasPseudocriticalTemperature("TscHCSutton", Rankine::get_instance(), 330,
                                   575)
  {
    set_author("Sutton (HC Portion)");
    set_title("SUTTON CORRELATION, CALCULATION OF PSEUDOCRITICAL TEMPERATURE "
              "OF THE NATURAL GAS HYDROCARBON PORTION");
    add_parameter("yghc", Sgg::get_instance(),
                  "Gas gravity of the hydrocarbon portion", 0.571, 1.679);
    def_parameter_latex_symbol("yghc", "\\gamma_{gHC}");
    add_db("Calculation of pseudocritical temperature of the natural gas "
           "hydrocarbon portion.");
    add_db("Based on 264 samples of natural gases with high molecular weight, "
           "rich in heptanes, with minor amounts of nitrogen and carbon "
           "dioxide, and without hydrogen sulfide.");
    add_db("These 264 compositions were obtained from a data bank of 275 PVT "
           "reports and 634 compositions in total. The producing areas "
           "represented by this data bank are: Gulf of Mexico, Lousiana, "
           "Texas, and others.");
    add_note("Sutton's correlation was developed based on samples of gases "
             "with no hydrogen sulfide content.");
    add_note("The samples contained nitrogen (0 mol % to 2.86 mol %), carbon "
             "dioxide (0.01 mol % to 11.86 mol %), and heptanes-plus (0.02 mol "
             "% to 14.27 mol %).");
    add_ref("sutton:1985");
  }

public:
  inline Quantity<Rankine> impl(const Quantity<Sgg>& yghc) const;

  Quantity<Rankine> operator()(const Quantity<Sgg>& yghc) const
  {
    return impl(yghc);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yghc = it.get_curr();

      return impl(yghc);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yghc = it.get_curr();

    return impl(yghc);
  }
};

extern Quantity<Rankine> TscHCSutton__correlation__fct(
  const Quantity<Sgg>& yghc);
extern double TscHCSutton__correlation__valid__fct(double yghc);

/** TpcHCElsharkawyEtAl

The correlation was verified by using the original reference and a secondary
one: Elsharkawy & Elkamel (2000). Date: September 22 2016.


*/
class TpcHCElsharkawyEtAl : public GasPseudocriticalTemperature
{
  Correlation_Singleton(TpcHCElsharkawyEtAl);

  TpcHCElsharkawyEtAl()
    : GasPseudocriticalTemperature("TpcHCElsharkawyEtAl",
                                   Rankine::get_instance())
  {
    set_author("Elsharkawy et al. (HC Portion)");
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
             "respectively.");
    add_note("The samples also contained heptanes-plus in the range from 0.22 "
             "mol % to 17.2 mol %.");
    add_ref("elsharkawy:2000:1");
    add_ref("elsharkawy:2000:2");
  }

public:
  inline Quantity<Rankine> impl(const Quantity<Sgg>& yghc) const;

  Quantity<Rankine> operator()(const Quantity<Sgg>& yghc) const
  {
    return impl(yghc);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yghc = it.get_curr();

      return impl(yghc);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yghc = it.get_curr();

    return impl(yghc);
  }
};

extern Quantity<Rankine> TpcHCElsharkawyEtAl__correlation__fct(
  const Quantity<Sgg>& yghc);
extern double TpcHCElsharkawyEtAl__correlation__valid__fct(double yghc);

#endif // GAS_PSEUDOCRITICAL_TEMP_H
