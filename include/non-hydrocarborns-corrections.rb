# coding: utf-8

begin_correlation("YgHCWichertAziz", "GasPseudoCriticalPressure", "Sgg", 0.55)
add_title("METHOD OF WICHERT & AZIZ FOR THE CALCULATION OF THE GAS GRAVITY OF THE HYDROCARBON PORTION")
add_ref("banzer:1996")
add_note("The original reference is not available. The correlation was verified by using secondary references: Standing (1977) and Bánzer (1996). Date: September 12 2016.")
add_parameter("yg", "Sgg")
add_parameter("n2_concentration", "MoleFraction")
add_parameter("co2_concentration", "MoleFraction")
add_parameter("h2s_concentration", "MoleFraction")
end_correlation()

################################################################

begin_correlation("PscMKayMixingRule", "GasPseudoCriticalPressure", "psia")
add_title("KAY'S MIXING RULE, CALCULATION OF PSEUDOCRITICAL PRESSURE OF THE WHOLE GAS MIXTURE")
add_note("The value of the pseudocritical pressure of the gas hydrocarbon portion (PscHC) is adjusted for nonhydrocarbon content on the basis of Kay's mixing rule.")
add_parameter("pschc", "psia")
add_parameter("n2_concentration", "MoleFraction")
add_parameter("co2_concentration", "MoleFraction")
add_parameter("h2s_concentration", "MoleFraction")
end_correlation()

################################################################

begin_correlation("AdjustedPscMWichertAziz", "GasPseudoCriticalPressure", "psia")
add_title("WICHERT & AZIZ CORRELATION, CALCULATION OF PSEUDOCRITICAL PRESSURE OF THE WHOLE GAS MIXTURE")
add_note("The pseudocritical pressure is adjusted by using the Wichert & Aziz correlation when the gas contains significant fractions of acid gases, specifically carbon dioxide (CO2) and hydrogen sulfide (H2S).")
add_note("The original reference is not available. The correlation was verified by using a secondary reference: Standing (1977). Date: September 12 2016.")
add_note("The development ranges were taken from PetroWiki (http://petrowiki.org/Real_gases).")
add_parameter("pscm", "psia", 154, 7026)
add_parameter("tscm", "Fahrenheit", 40, 300)
add_parameter("co2_concentration", "MoleFraction",
              "Quantity<MolePercent>(0)", "Quantity<MolePercent>(54.4)")
add_parameter("h2s_concentration", "MoleFraction",
              "Quantity<MolePercent>(0)", "Quantity<MolePercent>(73.8)")
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
add_parameter("tschc", "psia")
add_parameter("n2_concentration", "MoleFraction")
add_parameter("co2_concentration", "MoleFraction")
add_parameter("h2s_concentration", "MoleFraction")
end_correlation()
