
begin_correlation("Tpr", "GasPseudoReducedRatio", "PseudoReducedTemperature") 
add_title("CALCULATION OF THE PSEUDOREDUCED TEMPERATURE")
add_ref("banzer:1996")
add_parameter("t",  "Rankine", "Aqui Neylith")
add_parameter("tpcm",  "Rankine", "Aqui Neylith")
end_correlation()

################################################################

begin_correlation("Ppr", "GasPseudoReducedRatio", "PseudoReducedPressure") 
add_title("CALCULATION OF THE PSEUDOREDUCED PRESSURE")
add_ref("banzer:1996")
add_parameter("p",  "psia", "Aqui Neylith")
add_parameter("ppcm",  "psia", "Aqui Neylith")
end_correlation()
