# coding: utf-8

begin_correlation("YgHCWichertAziz", "GasSpecificGravity", "Sgg", 0.55)
add_title("METHOD OF WICHERT & AZIZ FOR THE CALCULATION OF THE GAS GRAVITY OF THE HYDROCARBON PORTION")
add_note("Method of Wichert & Aziz for the calculation of the gas gravity of the hydrocarbon portion.")
add_internal_note("The original reference is not available. The correlation was verified by using secondary references: Standing (1977) and Bánzer (1996). Date: September 12 2016.")
add_parameter("yg",  "Sgg", "Gas specific gravity")
add_parameter("n2_concentration",  "MoleFraction", "N2 concentration")
add_parameter("co2_concentration",  "MoleFraction", "CO2 concentration")
add_parameter("h2s_concentration",  "MoleFraction", "H2S concentration")
add_author("Wichert & Aziz (Gas Gravity Correction)")
add_ref("wichert:1972")
add_ref("standing:1977")
# add_ref("banzer:1996") Secondary reference
end_correlation()

################################################################

begin_correlation("PscMKayMixingRule", "GasPseudocriticalPressure", "psia")
add_title("KAY'S MIXING RULE, CALCULATION OF PSEUDOCRITICAL PRESSURE OF THE WHOLE GAS MIXTURE")
add_note("The value of the pseudocritical pressure of the gas hydrocarbon portion is adjusted for nonhydrocarbon content on the basis of Kay's mixing rule.")
add_parameter("ppchc",  "psia", "Gas pseudocritical pressure of the hydrocarbon portion")
add_parameter("n2_concentration",  "MoleFraction", "N2 concentration")
add_parameter("co2_concentration",  "MoleFraction", "CO2 concentration")
add_parameter("h2s_concentration",  "MoleFraction", "H2S concentration")
add_author("Kay (Mixing Rule)")
add_ref("kay:1936")
add_ref("whitson:2000")
add_ref("standing:1977")
# add_ref("banzer:1996") Secondary reference
add_internal_note("The original reference is not available. The correlation was verified by using secondary references: Whitson & Brulé (2000), Standing (1977) and Bánzer (1996). Date: September 12 2016.")
end_correlation()

################################################################

begin_correlation("AdjustedPscMWichertAziz", "GasPseudocriticalPressure", "psia")
add_title("WICHERT & AZIZ CORRELATION, CALCULATION OF PSEUDOCRITICAL PRESSURE OF THE WHOLE GAS MIXTURE")
add_note("The pseudocritical pressure of the mixture is adjusted by using the Wichert & Aziz correlation when the gas contains significant fractions of acid gases, specifically carbon dioxide and hydrogen sulfide.")
add_internal_note("The original reference is not available. The correlation was verified by using a secondary reference: Standing (1977). Date: September 12 2016.")
add_internal_note("The development ranges were taken from PetroWiki (http://petrowiki.org/Real_gases).")
add_parameter("ppcm", "psia", "Gas pseudocritical pressure of the mixture",  154,  7026)
add_parameter("tpcm", "Fahrenheit", "Gas pseudocritical temperature of the mixture",  40,  300)
add_parameter("co2_concentration", "MoleFraction", "CO2 concentration",  "Quantity<MolePercent>(0)",  "Quantity<MolePercent>(54.4)") 
add_parameter("h2s_concentration", "MoleFraction", "H2S concentration",  "Quantity<MolePercent>(0)",  "Quantity<MolePercent>(73.8)") 
add_author("Wichert & Aziz (Mixture Correction)")
add_ref("wichert:1972")
add_ref("standing:1977")
end_correlation()

################################################################

begin_correlation("TscMKayMixingRule", "GasPseudocriticalTemperature", "psia")
add_title("KAY'S MIXING RULE, CALCULATION OF PSEUDOCRITICAL TEMPERATURE OF THE WHOLE GAS MIXTURE")
add_note("The value of the pseudocritical temperature of the gas hydrocarbon portion is adjusted for nonhydrocarbon content on the basis of Kay's mixing rule.")
add_internal_note("The original reference is not available. The correlation was verified by using secondary references: Whitson & Brulé (2000), Standing (1977) and Bánzer (1996). Date: September 12 2016.")
add_ref("kay:1936")
add_ref("whitson:2000")
add_ref("standing:1977")
# add_ref("banzer:1996") Secondary reference
add_parameter("tpchc",  "psia", "Gas pseudocritical temperature of the hydrocarbon portion")
add_parameter("n2_concentration",  "MoleFraction", "N2 concentration")
add_parameter("co2_concentration",  "MoleFraction", "CO2 concentration")
add_parameter("h2s_concentration",  "MoleFraction", "H2S concentration")
add_author("Kay (Mixing Rule)")
end_correlation()

################################################################

begin_correlation("AdjustedTscMWichertAziz", "GasPseudocriticalTemperature",
                  "psia")
add_title("WICHERT & AZIZ CORRELATION, CALCULATION OF PSEUDOCRITICAL TEMPERATURE OF THE WHOLE GAS MIXTURE")
add_note("The pseudocritical temperature of the mixture is adjusted by using the Wichert & Aziz correlation when the gas contains significant fractions of acid gases, specifically carbon dioxide and hydrogen sulfide.")
add_ref("wichert:1972")
add_ref("standing:1977")
add_parameter("tpcm", "Rankine", "Gas pseudocritical temperature of the mixture",  "Quantity<Fahrenheit>(40)",  "Quantity<Fahrenheit>(300)") 
add_parameter("co2_concentration", "MoleFraction", "CO2 concentration",  "Quantity<MolePercent>(0)",  "Quantity<MolePercent>(54.4)") 
add_parameter("h2s_concentration", "MoleFraction", "H2S concentration",  "Quantity<MolePercent>(0)",  "Quantity<MolePercent>(73.8)") 
add_internal_note("The original reference is not available. The correlation was verified by using a secondary reference: Standing (1977). Date: September 12 2016.")
add_internal_note("The development ranges were taken from PetroWiki (http://petrowiki.org/Real_gases).")
add_author("Wichert & Aziz (Mixture Correction)")
end_correlation()
