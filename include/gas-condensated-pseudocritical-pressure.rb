# coding: utf-8

begin_correlation("CondensatePscHCStanding",
                  "GasCondensatePseudoCriticalPressure", "psia", 620, 675)
add_parameter("yghc", "Sgg", 0.58, 1.3)
add_parameter("n2_concentration", "MolePercent", 0, 5)
add_parameter("co2_concentration", "MolePercent", 0, 2)
add_parameter("h2s_concentration", "MolePercent", 0, 2)
add_title("STANDING CORRELATION, CALCULATION OF PSEUDOCRITICAL PRESSURE OF THE CONDENSATE GAS HYDROCARBON PORTION")
add_db("Based on laboratory generated gases in equilibrium with crude oil at high pressures (1000 to 8000 psia).")
add_note("Brown et al. (1948) presented the graphical correlation. Standing (1977) expressed it in a mathematical form.")
add_note("The values of the development ranges are taken from the graphs presented by Standing (1977) and Ahmed (1989).")
add_note("Brown et al.'s reference is not available. The correlation was verified by using these references: Standing (1977), Ahmed (1989), and Bánzer (1996). Date: September 9 2016.")
end_correlation()

################################################################

begin_correlation("CondensatePscHCElsharkawyEtAl",
                  "GasCondensatePseudoCriticalPressure", "psia")
add_parameter("yghc", "Sgg")
add_title("Based on compositional analysis of 1200 compositions of gas condensates collected worldwide.")
add_note("The original reference is not available. The correlation was verified by using a secondary reference: Elsharkawy & Elkamel (2000). Date: September 13 2016.")

end_correlation()
