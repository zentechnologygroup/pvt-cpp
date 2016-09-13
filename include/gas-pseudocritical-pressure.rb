# coding: utf-8

begin_correlation("YgHCWichertAziz", "GasPseudoCriticalPressure", "Sgg", 0.55)
add_title("METHOD OF WICHERT & AZIZ FOR THE CALCULATION OF THE GAS GRAVITY OF THE HYDROCARBON PORTION")
add_ref("banzer:1996")
add_note("The original reference is not available. The correlation was verified by using secondary references: Standing (1977) and Bánzer (1996). Date: September 12 2016.")
add_parameter("yg", "Sgg")
add_parameter("n2_concentration", "MolePercent")
add_parameter("co2_concentration", "MolePercent")
add_parameter("h2s_concentration", "MolePercent")
end_correlation()

################################################################

begin_correlation("PscMKay", "GasPseudoCriticalPressure", "psia")
add_title("KAY'S MIXING RULE, CALCULATION OF PSEUDOCRITICAL PRESSURE OF THE WHOLE GAS MIXTURE")
add_note("The value of the pseudocritical pressure of the gas hydrocarbon portion (PscHC) is adjusted for nonhydrocarbon content on the basis of Kay's mixing rule.")
add_parameter("pschc", "psia")
add_parameter("n2_concentration", "MolePercent")
add_parameter("co2_concentration", "MolePercent")
add_parameter("h2s_concentration", "MolePercent")
end_correlation()

################################################################

begin_correlation("AdjustedPscWichertAziz", "GasPseudoCriticalPressure", "psia")
add_title("WICHERT & AZIZ CORRELATION, CALCULATION OF PSEUDOCRITICAL PRESSURE OF THE WHOLE GAS MIXTURE")
add_note("The pseudocritical pressure is adjusted by using the Wichert & Aziz correlation when the gas contains significant fractions of acid gases, specifically carbon dioxide (CO2) and hydrogen sulfide (H2S).")
add_note("The original reference is not available. The correlation was verified by using a secondary reference: Standing (1977). Date: September 12 2016.")
add_note("The development ranges were taken from PetroWiki (http://petrowiki.org/Real_gases).")
add_parameter("pscm", "psia", 154, 7026)
add_parameter("tscm", "Fahrenheit", 40, 300)
add_parameter("n2_concentration", "MolePercent")
add_parameter("co2_concentration", "MolePercent", 0, 54.4)
add_parameter("h2s_concentration", "MolePercent", 0, 73.8)
end_correlation()

################################################################

begin_correlation("PscBrownKOA" , "GasPseudoCriticalPressure", "psia", 580, 680)
add_parameter("yghc", "Sgg", 0.55, 1.7)
add_parameter("n2_concentration", "MolePercent", 0, 5)
add_parameter("co2_concentration", "MolePercent", 0, 2)
add_parameter("h2s_concentration", "MolePercent", 0, 2)
add_title("BROWN ET AL. CORRELATION, CALCULATION OF PSEUDOCRITICAL PRESSURE OF THE NATURAL GAS HYDROCARBON PORTION")
add_db("Based on 71 natural gases from California, which represent surface trap gases and stock-tank vapors whose high gravity results from relatively high proportions of ethane, propane, and butanes.")
add_ref("banzer:1996")
add_eq("{P_{scHC}}\\,=\\,{677}\\,+\\,{15\\cdot{\\gamma_{gHC}}}\\,-\\,{37.5\\cdot{{\\gamma_{gHC}}^{\\,2}}}")
end_correlation()
