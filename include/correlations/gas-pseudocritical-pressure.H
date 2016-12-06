#ifndef GAS_PSEUDOCRITICAL_PRESSURE_H
#define GAS_PSEUDOCRITICAL_PRESSURE_H

#include <correlations/correlation.H>

/** PscHCStanding

The values of the development ranges are taken from the graphs presented by
Standing (1977) and Ahmed (1989).

Brown et al.'s reference is not available. The correlation was verified by using
these references: Standing (1977), Ahmed (1989), and Bánzer (1996). Date:
September 9 2016.


*/
class PscHCStanding : public GasPseudocriticalPressure
{
  Correlation_Singleton(PscHCStanding);

  PscHCStanding()
    : GasPseudocriticalPressure("PscHCStanding", psia::get_instance(), 586, 678)
  {
    set_author("Standing (HC Portion)");
    set_title("STANDING CORRELATION, CALCULATION OF PSEUDOCRITICAL PRESSURE OF "
              "THE DRY GAS HYDROCARBON PORTION");
    add_parameter("yghc", Sgg::get_instance(),
                  "Gas gravity of the hydrocarbon portion", 0.55, 1.67);
    def_parameter_latex_symbol("yghc", "\\gamma_{gHC}");
    add_db("Calculation of pseudocritical pressure of the dry gas hydrocarbon "
           "portion.");
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
  inline Quantity<psia> impl(const Quantity<Sgg>& yghc) const;

  Quantity<psia> operator()(const Quantity<Sgg>& yghc) const
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

extern Quantity<psia> PscHCStanding__correlation__fct(
  const Quantity<Sgg>& yghc);
extern double PscHCStanding__correlation__valid__fct(double yghc);

/** PscHcSutton

The ranges for Yg, n2Concentration, and co2Concentration, are presented as the
ranges of data covered by the total number of PVT reports. The range for Psc was
taken from the graph of the variation of pseudocritical pressure with gas
gravity, presented by Sutton.

Nonhydrocarbon adjustment methods should be applied to calculated
pseudo-critical properties of gases to account for the presence of nitrogen,
carbon dioxide, and hydrogen sulfide.


*/
class PscHcSutton : public GasPseudocriticalPressure
{
  Correlation_Singleton(PscHcSutton);

  PscHcSutton()
    : GasPseudocriticalPressure("PscHcSutton", psia::get_instance(), 500, 700)
  {
    set_author("Sutton (HC Portion)");
    set_title("SUTTON CORRELATION, CALCULATION OF PSEUDOCRITICAL PRESSURE OF "
              "THE NATURAL GAS HYDROCARBON PORTION");
    add_parameter("yghc", Sgg::get_instance(),
                  "Gas gravity of the hydrocarbon portion", 0.571, 1.679);
    def_parameter_latex_symbol("yghc", "\\gamma_{gHC}");
    add_db("Calculation of pseudocritical pressure of the natural gas "
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
  inline Quantity<psia> impl(const Quantity<Sgg>& yghc) const;

  Quantity<psia> operator()(const Quantity<Sgg>& yghc) const
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

extern Quantity<psia> PscHcSutton__correlation__fct(const Quantity<Sgg>& yghc);
extern double PscHcSutton__correlation__valid__fct(double yghc);

/** PscHCGuoGhalambor

It was decided to eliminate this correlation.


*/
class PscHCGuoGhalambor : public GasPseudocriticalPressure
{
  Correlation_Singleton(PscHCGuoGhalambor);

  PscHCGuoGhalambor()
    : GasPseudocriticalPressure("PscHCGuoGhalambor", psia::get_instance())
  {
    set_author("Guo & Ghalambor");
    set_title("GUO & GHALAMBOR CORRELATION, CALCULATION OF PSEUDOCRITICAL "
              "PRESSURE OF THE NATURAL GAS HYDROCARBON PORTION");
    set_hidden();
    add_parameter("yghc", Sgg::get_instance(),
                  "Gas gravity of the hydrocarbon portion");
    def_parameter_latex_symbol("yghc", "\\gamma_{gHC}");
    add_parameter("n2_concentration", MoleFraction::get_instance(),
                  "N2 concentration", Quantity<MolePercent>(0),
                  Quantity<MolePercent>(5));
    def_parameter_latex_symbol("n2_concentration", "C_{N_2}");
    add_parameter("h2s_concentration", MoleFraction::get_instance(),
                  "H2S concentration", Quantity<MolePercent>(0),
                  Quantity<MolePercent>(3));
    def_parameter_latex_symbol("h2s_concentration", "C_{H_2S}");
    add_note("The book is available at: "
             "https://books.google.co.ve/"
             "books?id=KXmjAQAAQBAJ&pg=PA13&hl=es&source=gbs_toc_r&cad=4#v="
             "onepage&q&f=false");
  }

public:
  inline Quantity<psia> impl(
    const Quantity<Sgg>& yghc, const Quantity<MoleFraction>& n2_concentration,
    const Quantity<MoleFraction>& h2s_concentration) const;

  Quantity<psia> operator()(
    const Quantity<Sgg>& yghc, const Quantity<MoleFraction>& n2_concentration,
    const Quantity<MoleFraction>& h2s_concentration) const
  {
    return impl(yghc, n2_concentration, h2s_concentration);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yghc = it.get_curr();
      it.next();
      const auto& n2_concentration = it.get_curr();
      it.next();
      const auto& h2s_concentration = it.get_curr();

      return impl(yghc, n2_concentration, h2s_concentration);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yghc = it.get_curr();
    it.next();
    const auto& n2_concentration = it.get_curr();
    it.next();
    const auto& h2s_concentration = it.get_curr();

    return impl(yghc, n2_concentration, h2s_concentration);
  }
};

extern Quantity<psia> PscHCGuoGhalambor__correlation__fct(
  const Quantity<Sgg>& yghc, const Quantity<MoleFraction>& n2_concentration,
  const Quantity<MoleFraction>& h2s_concentration);
extern double PscHCGuoGhalambor__correlation__valid__fct(
  double yghc, double n2_concentration, double h2s_concentration);

/** PscAhmed

It was decided to eliminate this correlation.


*/
class PscAhmed : public GasPseudocriticalPressure
{
  Correlation_Singleton(PscAhmed);

  PscAhmed()
    : GasPseudocriticalPressure("PscAhmed", psia::get_instance())
  {
    set_author("Ahmed");
    set_title("AHMED CORRELATION, CALCULATION OF PSEUDOCRITICAL PRESSURE OF "
              "THE NATURAL GAS MIXTURE");
    set_hidden();
    add_parameter("yghc", Sgg::get_instance(),
                  "Gas gravity of the hydrocarbon portion");
    def_parameter_latex_symbol("yghc", "\\gamma_{gHC}");
    add_parameter("n2_concentration", MoleFraction::get_instance(),
                  "N2 concentration");
    def_parameter_latex_symbol("n2_concentration", "C_{N_2}");
    add_parameter("co2_concentration", MoleFraction::get_instance(),
                  "CO2 concentration");
    def_parameter_latex_symbol("co2_concentration", "C_{CO_2}");
    add_parameter("h2s_concentration", MoleFraction::get_instance(),
                  "H2S concentration");
    def_parameter_latex_symbol("h2s_concentration", "C_{H_2S}");
    add_note("The correlation already has impurity corrections for mixture "
             "pseudocriticals.");
  }

public:
  inline Quantity<psia> impl(
    const Quantity<Sgg>& yghc, const Quantity<MoleFraction>& n2_concentration,
    const Quantity<MoleFraction>& co2_concentration,
    const Quantity<MoleFraction>& h2s_concentration) const;

  Quantity<psia> operator()(
    const Quantity<Sgg>& yghc, const Quantity<MoleFraction>& n2_concentration,
    const Quantity<MoleFraction>& co2_concentration,
    const Quantity<MoleFraction>& h2s_concentration) const
  {
    return impl(yghc, n2_concentration, co2_concentration, h2s_concentration);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity>& __pars,
                              bool check = true) const
  {
    if (check) {
      auto pars = verify_preconditions(__pars);
      auto it = pars.get_it();
      const auto& yghc = it.get_curr();
      it.next();
      const auto& n2_concentration = it.get_curr();
      it.next();
      const auto& co2_concentration = it.get_curr();
      it.next();
      const auto& h2s_concentration = it.get_curr();

      return impl(yghc, n2_concentration, co2_concentration, h2s_concentration);
    }

    const DynList<VtlQuantity>& pars = __pars;
    auto it = pars.get_it();
    const auto& yghc = it.get_curr();
    it.next();
    const auto& n2_concentration = it.get_curr();
    it.next();
    const auto& co2_concentration = it.get_curr();
    it.next();
    const auto& h2s_concentration = it.get_curr();

    return impl(yghc, n2_concentration, co2_concentration, h2s_concentration);
  }
};

extern Quantity<psia> PscAhmed__correlation__fct(
  const Quantity<Sgg>& yghc, const Quantity<MoleFraction>& n2_concentration,
  const Quantity<MoleFraction>& co2_concentration,
  const Quantity<MoleFraction>& h2s_concentration);
extern double PscAhmed__correlation__valid__fct(double yghc,
                                                double n2_concentration,
                                                double co2_concentration,
                                                double h2s_concentration);

/** PscHCStandingHeavierFractions

The values of the development ranges are taken from the graphs presented by
Standing (1977) and Ahmed (1989).

Brown et al.'s reference is not available. The correlation was verified by using
these references: Standing (1977), Ahmed (1989), and Bánzer (1996). Date:
September 9 2016.


*/
class PscHCStandingHeavierFractions : public GasPseudocriticalPressure
{
  Correlation_Singleton(PscHCStandingHeavierFractions);

  PscHCStandingHeavierFractions()
    : GasPseudocriticalPressure("PscHCStandingHeavierFractions",
                                psia::get_instance(), 620, 675)
  {
    set_author("Standing (HC Portion for Heavier Fractions)");
    set_title("STANDING CORRELATION, CALCULATION OF PSEUDOCRITICAL PRESSURE OF "
              "THE WET OR \"CONDENSATE\" GAS HYDROCARBON PORTION");
    add_parameter("yghc", Sgg::get_instance(),
                  "Gas gravity of the hydrocarbon portion", 0.58, 1.3);
    def_parameter_latex_symbol("yghc", "\\gamma_{gHC}");
    add_db("Calculation of pseudocritical pressure of the wet or "
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
    add_note("The correlation was developed with samples containing "
             "concentrations of nitrogen, carbon dioxide, and hydrogen "
             "sulfide, up to 5 mol %, 2 mol %, and 2 mol %, respectively.");
  }

public:
  inline Quantity<psia> impl(const Quantity<Sgg>& yghc) const;

  Quantity<psia> operator()(const Quantity<Sgg>& yghc) const
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

extern Quantity<psia> PscHCStandingHeavierFractions__correlation__fct(
  const Quantity<Sgg>& yghc);
extern double PscHCStandingHeavierFractions__correlation__valid__fct(
  double yghc);

/** PscHCElsharkawyEtAl

The correlation was verified by using the original reference and a secondary
one: Elsharkawy & Elkamel (2000). Date: September 22 2016.


*/
class PscHCElsharkawyEtAl : public GasPseudocriticalPressure
{
  Correlation_Singleton(PscHCElsharkawyEtAl);

  PscHCElsharkawyEtAl()
    : GasPseudocriticalPressure("PscHCElsharkawyEtAl", psia::get_instance())
  {
    set_author("Elsharkawy et al. (HC Portion)");
    set_title("ELSHARKAWY ET AL. CORRELATION, CALCULATION OF PSEUDOCRITICAL "
              "PRESSURE OF THE CONDENSATE GAS HYDROCARBON PORTION");
    add_parameter("yghc", Sgg::get_instance(),
                  "Gas gravity of the hydrocarbon portion", 0.61, 1.89);
    def_parameter_latex_symbol("yghc", "\\gamma_{gHC}");
    add_db("Calculation of pseudocritical pressure of the \"condensate\" gas "
           "hydrocarbon portion.");
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
  inline Quantity<psia> impl(const Quantity<Sgg>& yghc) const;

  Quantity<psia> operator()(const Quantity<Sgg>& yghc) const
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

extern Quantity<psia> PscHCElsharkawyEtAl__correlation__fct(
  const Quantity<Sgg>& yghc);
extern double PscHCElsharkawyEtAl__correlation__valid__fct(double yghc);

#endif // GAS_PSEUDOCRITICAL_PRESSURE_H
