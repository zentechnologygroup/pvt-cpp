
begin_correlation("Tpr", "GasPseudoReducedRatio", "PseudoReducedTemperature") 
add_title("CALCULATION OF THE PSEUDOREDUCED TEMPERATURE")
add_ref("banzer:1996")
add_parameter("t", "Rankine")
add_parameter("tpcm", "Rankine")
end_correlation()

################################################################

begin_correlation("Ppr", "GasPseudoReducedRatio", "PseudoReducedPressure") 
add_title("CALCULATION OF THE PSEUDOREDUCED PRESSURE")
add_ref("banzer:1996")
add_parameter("p", "psia")
add_parameter("ppcm", "psia")
end_correlation()
