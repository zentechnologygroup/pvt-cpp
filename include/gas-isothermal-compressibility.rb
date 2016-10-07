
begin_correlation("CgSarem", "GasIsothermalCompressibility", "psia_1")
add_parameter("tpr",  "PseudoReducedTemperature", "Aqui Neylith")
add_parameter("ppr",  "PseudoReducedPressure", "Aqui Neylith")
add_parameter("ppc",  "psia", "Aqui Neylith")
add_parameter("z",  "ZFactor", "Aqui Neylith")
add_ref('sarem:1961')
add_author("Sarem")
end_correlation()

################################################################

begin_correlation("CgHallYarborough", "GasIsothermalCompressibility", "psia_1")
add_parameter("__tpr",  "PseudoReducedTemperature", "Aqui Neylith")
add_parameter("__ppr",  "PseudoReducedPressure", "Aqui Neylith")
add_parameter("ppc",  "psia", "Aqui Neylith")
add_parameter("z",  "ZFactor", "Aqui Neylith")
add_author("Hall & Yarborough")
end_correlation()

################################################################

begin_correlation("CgMattarBA", "GasIsothermalCompressibility", "psia_1")
add_parameter("__tpr",  "PseudoReducedTemperature", "Aqui Neylith")
add_parameter("__ppr",  "PseudoReducedPressure", "Aqui Neylith")
add_parameter("ppc",  "psia", "Aqui Neylith")
add_parameter("z",  "ZFactor", "Aqui Neylith")
add_author("Mattar et al.")
end_correlation()

################################################################

begin_correlation("CgGopal", "GasIsothermalCompressibility", "psia_1")
add_parameter("__tpr",  "PseudoReducedTemperature", "Aqui Neylith")
add_parameter("__ppr",  "PseudoReducedPressure", "Aqui Neylith")
add_parameter("ppc",  "psia", "Aqui Neylith")
add_parameter("z",  "ZFactor", "Aqui Neylith")
add_author("Gopal")
end_correlation()

################################################################

begin_correlation("CgBrillBeggs", "GasIsothermalCompressibility", "psia_1")
add_parameter("__tpr",  "PseudoReducedTemperature", "Aqui Neylith")
add_parameter("__ppr",  "PseudoReducedPressure", "Aqui Neylith")
add_parameter("ppc",  "psia", "Aqui Neylith")
add_parameter("z",  "ZFactor", "Aqui Neylith")
add_author("Brill & Beggs")
end_correlation()

################################################################

begin_correlation("CgPapay", "GasIsothermalCompressibility", "psia_1") 
add_parameter("__tpr",  "PseudoReducedTemperature", "Aqui Neylith")
add_parameter("__ppr",  "PseudoReducedPressure", "Aqui Neylith")
add_parameter("ppc",  "psia", "Aqui Neylith")
add_parameter("z",  "ZFactor", "Aqui Neylith")
add_author("Papay")
end_correlation()

