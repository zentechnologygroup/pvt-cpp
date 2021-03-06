# verificada con python!
begin_correlation("CobMcCainEtAl", "SaturatedOilCompressibility", "psia_1",
                  31e-6, 6600e-6)
add_title("McCAIN, ROLLINS & VILLENA CORRELATION, CALCULATION OF SATURATED OIL ISOTHERMAL COMPRESSIBILITY")
add_db("Based on 2500 experimental data obtained from different crude oils.")
add_db("The equation was developed with data from reservoir-fluid studies of 260 oil fields worldwide.")
add_parameter("api", "Api", "API oil gravity",  18,  52)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  15,  1947)
add_parameter("t", "Rankine", "Temperature",  "Quantity<Fahrenheit>(78)",  "Quantity<Fahrenheit>(330)") 
add_parameter("p", "psia", "Pressure",  500,  5300)
add_parameter("pb", "psia", "Bubble point pressure",  763,  5300)
add_precondition("p", "pb")
add_author("McCain, Rollins & Villena")
add_ref("mcCain:1988")
# add_ref("banzer:1996") Secondary reference
end_correlation()
