# coding: utf-8

begin_correlation("YgHCWichertAziz", "GasCorrelation", "psia")
add_parameter("yg", "Sgg")
add_parameter("n2_concentration", "Molar_Percent")
add_parameter("co2_concentration", "Molar_Percent")
add_parameter("h2s_concentration", "Molar_Percent")

begin_correlation("PscMKay", "GasCorrelation", "psia", )
add_parameter("pschc", "psia")
add_parameter("n2_concentration", "Molar_Percent")
add_parameter("co2_concentration", "Molar_Percent")
add_parameter("h2s_concentration", "Molar_Percent")


begin_correlation("PscBrownKOA" , "GasCorrelation", "psia", 580, 680)
add_parameter("yghc", "Sgg", 0.55, 1.7)
add_parameter("n2_concentration", "Molar_Percent", 0, 5)
add_parameter("co2_concentration", "Molar_Percent", 0, 2)
add_parameter("h2s_concentration", "Molar_Percent", 0, 2)
add_db("BROWN ET AL. CORRELATION, CALCULATION OF PSEUDOCRITICAL PRESSURE OF THE NATURAL GAS HYDROCARBON PORTION")
add_db("Based on 71 natural gases from California, which represent surface trap gases and stock-tank vapors whose high gravity results from relatively high proportions of ethane, propane, and butanes.")
add_ref("banzer:1996")
add_eq("{P_{scHC}}\\,=\\,{677}\\,+\\,{15\\cdot{\\gamma_{gHC}}}\\,-\\,{37.5\\cdot{{\\gamma_{gHC}}^{\\,2}}}")
end_correlation()
