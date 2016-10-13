
begin_correlation("CgSarem", "GasIsothermalCompressibility", "psia_1")
add_parameter("tpr",  "PseudoReducedTemperature", "Gas pseudoreduced temperature")
add_parameter("ppr",  "PseudoReducedPressure", "Gas pseudoreduced pressure")
add_parameter("ppc",  "psia", "Gas pseudocritical pressure")
add_parameter("z",  "ZFactor", "Gas compressibility factor")
add_ref('sarem:1961')
add_author("Sarem")
end_correlation()

################################################################

begin_correlation("CgHallYarborough", "GasIsothermalCompressibility", "psia_1")
add_parameter("tpr",  "PseudoReducedTemperature", "Gas pseudoreduced temperature")
add_parameter("ppr",  "PseudoReducedPressure", "Gas pseudoreduced pressure")
add_parameter("ppc",  "psia", "Gas pseudocritical pressure")
add_parameter("z",  "ZFactor", "Gas compressibility factor")
add_author("Hall & Yarborough")
end_correlation()

################################################################

begin_correlation("CgMattarBA", "GasIsothermalCompressibility", "psia_1")
add_parameter("tpr",  "PseudoReducedTemperature", "Gas pseudoreduced temperature")
add_parameter("ppr",  "PseudoReducedPressure", "Gas pseudoreduced pressure")
add_parameter("ppc",  "psia", "Gas pseudocritical pressure")
add_parameter("z",  "ZFactor", "Gas compressibility factor")
add_author("Mattar et al.")
end_correlation()

################################################################

begin_correlation("CgGopal", "GasIsothermalCompressibility", "psia_1")
add_parameter("tpr",  "PseudoReducedTemperature", "Gas pseudoreduced temperature")
add_parameter("ppr",  "PseudoReducedPressure", "Gas pseudoreduced pressure")
add_parameter("ppc",  "psia", "Gas pseudocritical pressure")
add_parameter("z",  "ZFactor", "Gas compressibility factor")
add_author("Gopal")
end_correlation()

################################################################

begin_correlation("CgBrillBeggs", "GasIsothermalCompressibility", "psia_1")
add_parameter("tpr",  "PseudoReducedTemperature", "Gas pseudoreduced temperature")
add_parameter("ppr",  "PseudoReducedPressure", "Gas pseudoreduced pressure")
add_parameter("ppc",  "psia", "Gas pseudocritical pressure")
add_parameter("z",  "ZFactor", "Gas compressibility factor")
add_author("Brill & Beggs")
end_correlation()

################################################################

begin_correlation("CgPapay", "GasIsothermalCompressibility", "psia_1") 
add_parameter("tpr",  "PseudoReducedTemperature", "Gas pseudoreduced temperature")
add_parameter("ppr",  "PseudoReducedPressure", "Gas pseudoreduced pressure")
add_parameter("ppc",  "psia", "Gas pseudocritical pressure")
add_parameter("z",  "ZFactor", "Gas compressibility factor")
add_author("Papay")
end_correlation()

