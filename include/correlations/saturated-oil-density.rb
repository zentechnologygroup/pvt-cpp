
# coding: utf-8

# verificada con python!
begin_correlation("PobBradley", "SaturatedOilDensity", "Lb_ft3")
add_title("CALCULATION OF OIL DENSITY")
add_parameter("yg",  "Sgg", "Gas specific gravity")
add_parameter("rs",  "SCF_STB", "Solution GOR")
add_parameter("bob",  "RB_STB", "Oil formation volume factor")
add_parameter("yo",  "Sg_do", "Oil specific gravity")
add_author("Bradley")
add_ref("beggs:1987")
end_correlation()

################################################################        
