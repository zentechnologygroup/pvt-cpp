
# coding: utf-8

# verificada con python!
begin_correlation("PoaBradley", "UndersaturatedOilDensity", "Lb_ft3")
add_title("CALCULATION OF OIL DENSITY")
add_parameter("yg", "Sgg", "Gas specific gravity")
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb")
add_parameter("bobp", "psia", "Oil formation volume factor")
add_parameter("yo", "Sgg", "Oil specific gravity")
add_parameter("p",  "psia", "Pressure")
add_parameter("pb",  "psia", "Bubble point pressure")
add_parameter("co",  "psia_1", "Coefficient of isothermal compressibility")
add_author("Bradley")
add_ref("beggs:1987")
end_correlation()

################################################################        
