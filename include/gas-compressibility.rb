
begin_correlation("ZFactorSarem", "GasCompressibility", "ZFactor")
add_parameter("t", "Rankine")
add_parameter("p", "psia")
add_parameter("tsc", "Rankine") # TODO falta cotas
add_parameter("psc", "psia")
end_correlation()

################################################################

begin_correlation("ZFactorHallYarborough", "GasCompressibility", "ZFactor")
add_parameter("t", "Rankine")
add_parameter("p", "psia")
add_parameter("tsc", "Rankine") # TODO falta cotas
add_parameter("psc", "psia")
end_correlation()

################################################################

begin_correlation("ZFactorDranchukPR", "GasCompressibility", "ZFactor")
add_parameter("t", "Rankine")
add_parameter("p", "psia")
add_parameter("tsc", "Rankine") # TODO falta cotas
add_parameter("psc", "psia")
end_correlation()

################################################################

begin_correlation("ZFactorDranchukAK", "GasCompressibility", "ZFactor")
add_parameter("t", "Rankine")
add_parameter("p", "psia")
add_parameter("tsc", "Rankine") # TODO falta cotas
add_parameter("psc", "psia")
end_correlation()
