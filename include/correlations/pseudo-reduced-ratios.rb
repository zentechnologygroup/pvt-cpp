
begin_correlation("Tpr", "GasPseudoreducedTemperature", "PseudoReducedTemperature") 
add_title("CALCULATION OF THE PSEUDOREDUCED TEMPERATURE")
add_ref("ahmed:1989")
add_ref("banzer:1996")
add_parameter("t",  "Rankine", "Temperature")
add_parameter("tpc",  "Rankine", "Gas pseudocritical temperature")
add_author("Standard Equation")
end_correlation()

################################################################

begin_correlation("Ppr", "GasPseudoreducedPressure", "PseudoReducedPressure") 
add_title("CALCULATION OF THE PSEUDOREDUCED PRESSURE")
add_ref("ahmed:1989")
add_ref("banzer:1996")
add_parameter("p",  "psia", "Pressure")
add_parameter("ppc",  "psia", "Gas pseudocritical pressure")
add_author("Standard Equation")
end_correlation()
