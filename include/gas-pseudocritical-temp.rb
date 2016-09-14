# coding: utf-8

begin_correlation("PscHCStanding" , "GasPseudoCriticalPressure", "psia", 580, 680)
add_parameter("yghc", "Sgg", 0.55, 1.7)
add_parameter("n2_concentration", "MolePercent", 0, 5)
add_parameter("co2_concentration", "MolePercent", 0, 2)
add_parameter("h2s_concentration", "MolePercent", 0, 2)
add_title("STANDING CORRELATION, CALCULATION OF PSEUDOCRITICAL PRESSURE OF THE NATURAL GAS HYDROCARBON PORTION")
add_db("Based on 71 natural gases from California, which represent surface trap gases and stock-tank vapors whose high gravity results from relatively high proportions of ethane, propane, and butanes.")
add_ref("banzer:1996")
add_eq("{P_{scHC}}\\,=\\,{677}\\,+\\,{15\\cdot{\\gamma_{gHC}}}\\,-\\,{37.5\\cdot{{\\gamma_{gHC}}^{\\,2}}}")
add_note("Brown et al. (1948) presented the graphical correlation. Standing (1977) expressed it in a mathematical form.")
add_internal_note("The values of the development ranges are taken from the graphs presented by Standing (1977) and Ahmed (1989).")
add_internal_note("Brown et al.'s reference is not available. The correlation was verified by using these references: Standing (1977), Ahmed (1989), and Bánzer (1996). Date: September 9 2016.")
end_correlation()

################################################################

begin_correlation("PscHcSutton" , "GasPseudoCriticalPressure", "psia", 500, 700)
add_parameter("yghc", "Sgg", 0.571, 1.679)
add_parameter("n2_concentration", "MolePercent", 0, 2.86)
add_parameter("co2_concentration", "MolePercent", 0.01, 11.86)
add_title("SUTTON CORRELATION, CALCULATION OF PSEUDOCRITICAL PRESSURE OF THE NATURAL GAS HYDROCARBON PORTION")
add_db("Based on 264 samples of natural gases with high molecular weight, rich in heptanes, with minor amounts of nitrogen and carbon dioxide, and without hydrogen sulfide.")
add_db("These 264 compositions were obtained from a data bank of 275 PVT reports and 634 compositions in total. The producing areas represented by this data bank are: Gulf of Mexico, Lousiana, Texas, and others.")
add_note("The ranges for Yg, n2Concentration, and co2Concentration, are presented as the ranges of data covered by the total number of PVT reports. The range for Psc was taken from the graph of the variation of pseudocritical pressure with gas gravity, presented by Sutton.")
end_correlation()

################################################################

begin_correlation("PscHCGuoGhalambor", "GasPseudoCriticalPressure", "psia")
add_parameter("yghc", "Sgg")
add_parameter("n2_concentration", "MolePercent", 0, 5)
add_parameter("co2_concentration", "MolePercent")
add_parameter("h2s_concentration", "MolePercent", 0, 3)
add_title("GUO & GHALAMBOR CORRELATION, CALCULATION OF PSEUDOCRITICAL PRESSURE OF THE NATURAL GAS HYDROCARBON PORTION")
add_note("The book is available at: https://books.google.co.ve/books?id=KXmjAQAAQBAJ&pg=PA13&hl=es&source=gbs_toc_r&cad=4#v=onepage&q&f=false")
end_correlation()

################################################################

begin_correlation("PscAhmed", "GasPseudoCriticalPressure", "psia")
add_parameter("yghc", "Sgg")
add_parameter("n2_concentration", "MolePercent")
add_parameter("co2_concentration", "MolePercent")
add_parameter("h2s_concentration", "MolePercent")
add_title("AHMED CORRELATION, CALCULATION OF PSEUDOCRITICAL PRESSURE OF THE NATURAL GAS MIXTURE")
add_note("The correlation already has impurity corrections for mixture pseudocriticals.")
end_correlation()
