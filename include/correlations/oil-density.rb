
# coding: utf-8

# verificada con python!
begin_correlation("PoBradley", "OilDensity", "Lb_ft3")
add_title("CALCULATION OF OIL DENSITY")
add_parameter("yg",  "Sgg", "Gas specific gravity")
add_parameter("rs",  "SCF_STB", "Solution GOR at Pb")
add_parameter("bo",  "RB_STB", "Oil formation volume factor")
add_parameter("yo",  "Sg_do", "Oil specific gravity")
add_parameter("p",  "psia", "Pressure")
add_parameter("pb",  "psia", "Bubble point pressure")
add_parameter("co",  "psia_1", "Coefficient of isothermal compressibility")
add_author("Bradley")
add_ref("beggs:1987")
end_correlation()

################################################################        
