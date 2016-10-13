# coding: utf-8

begin_correlation("YgHCWichertAziz", "GasPseudoCriticalPressure", "Sgg", 0.55)
add_title("METHOD OF WICHERT & AZIZ FOR THE CALCULATION OF THE GAS GRAVITY OF THE HYDROCARBON PORTION")
add_ref("banzer:1996")
add_note("The original reference is not available. The correlation was verified by using secondary references: Standing (1977) and Bánzer (1996). Date: September 12 2016.")
add_parameter("yg",  "Sgg", "Gas specific gravity")
add_parameter("n2_concentration",  "MoleFraction", "Aqui Neylith")
add_parameter("co2_concentration",  "MoleFraction", "Aqui Neylith")
add_parameter("h2s_concentration",  "MoleFraction", "Aqui Neylith")
add_author("Correction Yg Wichert & Aziz")
end_correlation()

################################################################

begin_correlation("PscMKayMixingRule", "GasPseudoCriticalPressure", "psia")
add_title("KAY'S MIXING RULE, CALCULATION OF PSEUDOCRITICAL PRESSURE OF THE WHOLE GAS MIXTURE")
add_note("The value of the pseudocritical pressure of the gas hydrocarbon portion (PscHC) is adjusted for nonhydrocarbon content on the basis of Kay's mixing rule.")
add_parameter("pschc",  "psia", "Aqui Neylith")
add_parameter("n2_concentration",  "MoleFraction", "Aqui Neylith")
add_parameter("co2_concentration",  "MoleFraction", "Aqui Neylith")
add_parameter("h2s_concentration",  "MoleFraction", "Aqui Neylith")
add_author("Correction Psc MKayMixingRule")
end_correlation()

################################################################

begin_correlation("AdjustedPscMWichertAziz", "GasPseudoCriticalPressure", "psia")
add_title("WICHERT & AZIZ CORRELATION, CALCULATION OF PSEUDOCRITICAL PRESSURE OF THE WHOLE GAS MIXTURE")
add_note("The pseudocritical pressure is adjusted by using the Wichert & Aziz correlation when the gas contains significant fractions of acid gases, specifically carbon dioxide (CO2) and hydrogen sulfide (H2S).")
add_note("The original reference is not available. The correlation was verified by using a secondary reference: Standing (1977). Date: September 12 2016.")
add_note("The development ranges were taken from PetroWiki (http://petrowiki.org/Real_gases).")
add_parameter("pscm", "psia", "Aqui Neylith",  154,  7026)
add_parameter("tscm", "Fahrenheit", "Aqui Neylith",  40,  300)
add_parameter("co2_concentration", "MoleFraction", "Aqui Neylith",  "Quantity<MolePercent>(0)",  "Quantity<MolePercent>(54.4)") 
add_parameter("h2s_concentration", "MoleFraction", "Aqui Neylith",  "Quantity<MolePercent>(0)",  "Quantity<MolePercent>(73.8)") 
add_author("Correction Psc MWichertAziz")
end_correlation()

################################################################

begin_correlation("TscMKayMixingRule", "GasPseudoCriticalTemperature", "psia")
add_title("KAY'S MIXING RULE, CALCULATION OF PSEUDOCRITICAL TEMPERATURE OF THE WHOLE GAS MIXTURE")
add_note("The value of the pseudocritical temperature of the gas hydrocarbon portion (PscHC) is adjusted for nonhydrocarbon content on the basis of Kay's mixing rule.")
add_note("The original reference is not available. The correlation was verified by using secondary references: Whitson & Brulé (2000), Standing (1977) and Bánzer (1996). Date: September 12 2016.")
add_ref("kay:1936")
add_ref("whitson:2000")
add_ref("standing:1977")
add_ref("banzer:1996")
add_parameter("tschc",  "psia", "Aqui Neylith")
add_parameter("n2_concentration",  "MoleFraction", "Aqui Neylith")
add_parameter("co2_concentration",  "MoleFraction", "Aqui Neylith")
add_parameter("h2s_concentration",  "MoleFraction", "Aqui Neylith")
add_author("Correction Tsc MKayMixingRule")
end_correlation()

################################################################

begin_correlation("AdjustedTscMWichertAziz", "GasPseudoCriticalTemperature",
                  "psia")
add_title("WICHERT & AZIZ CORRELATION, CALCULATION OF PSEUDOCRITICAL TEMPERATURE OF THE WHOLE GAS MIXTURE")
add_note("The pseudocritical temperature is adjusted by using the Wichert & Aziz correlation when the gas contains significant fractions of acid gases, specifically carbon dioxide (CO2) and hydrogen sulfide (H2S).")
add_ref("wichert:1972")
add_ref("standing:1977")
add_parameter("tscm", "Rankine", "Aqui Neylith",  "Quantity<Fahrenheit>(40)",  "Quantity<Fahrenheit>(300)") 
add_parameter("co2_concentration", "MoleFraction", "Aqui Neylith",  "Quantity<MolePercent>(0)",  "Quantity<MolePercent>(54.4)") 
add_parameter("h2s_concentration", "MoleFraction", "Aqui Neylith",  "Quantity<MolePercent>(0)",  "Quantity<MolePercent>(73.8)") 
add_note("The original reference is not available. The correlation was verified by using a secondary reference: Standing (1977). Date: September 12 2016.")
add_note("The development ranges were taken from PetroWiki (http://petrowiki.org/Real_gases).")
add_author("Correction Tsc MWichertAziz")
end_correlation()
