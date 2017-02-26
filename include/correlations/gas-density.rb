# coding: utf-8
begin_correlation("Pg", "GasDensity", "Lb_ft3")
add_title("CALCULATION OF GAS DENSITY")
add_parameter("yg", "Sgg", "Gas specific gravity")
add_parameter("t", "Rankine", "Temperature")
add_parameter("p", "psia", "Pressure")
add_parameter("z", "Zfactor", "Gas compressibility factor")
add_author("Standard Equation")
add_db("The calculation is based on the Real Gas Law.")
add_ref("takacs:2005")
add_ref("ahmed:2010")
# add_ref("banzer:1996") Secondary reference
add_internal_note("The equation was verified by using secondary references: Ahmed (2010) and Bánzer (1996). Date: September 28 2016.")
end_correlation()
