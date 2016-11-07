# coding: utf-8

begin_correlation("RswSpiveyMN", "SolutionGasWaterRelation", "Sm3_Sm3")
add_title("SPIVEY, McCAIN & NORTH CORRELATION, CALCULATION OF THE SOLUTION GAS-WATER RATIO")
add_db("The volumetric properties of oilfield brines were obtained by combining the correlations developed by Spivey et al., for density of methane-free sodium chloride brine and solubility of methane in sodium chloride brines, with published correlations for specific volume and partial molar volume of methane in brine.")
add_parameter("t", "Celsius", "Temperature",  0,  275) 
add_parameter("p", "mPascal", "Pressure",  0.1,  200)
add_parameter("nacl_concentration", "Molality_NaCl", "Dissolved salt concentration", 0, 5.7) 
add_author("Spivey, McCain & North")
add_ref("spivey:2004")
add_ref("mcCain:2011")
add_note("The solution gas-water ratio is obtained by dividing the volume of the gas at standard conditions by the volume of brine at stock-tank conditions.")
add_internal_note("The correlation was verified by using the original reference and McCain et al. (2011) as a secondary reference. Date: October 17 2016.")
add_internal_note("The original reference expresses the output unit in Scm3/Scm3")
end_correlation()

################################################################

begin_correlation("RswCulbersonMcKetta", "SolutionGasWaterRelation", "SCF_STB")
add_title("CULBERSON & McKETTA CORRELATION, CALCULATION OF THE SOLUTION GAS-WATER RATIO")
add_db("Based on experimental and smoothed data for the solubility of methane in water for temperatures of 77, 100, 160, 220, 280, and 340 °F at pressures to 10000 psia.")
add_parameter("t", "Fahrenheit", "Temperature",  77,  340) 
add_parameter("p", "psia", "Pressure",  1000,  10000)
add_parameter("nacl_concentration", "Dissolved_Salt_Percent", "Dissolved salt concentration", 0, 26) # TODO: valor de referencia es 30 pero 26 es el límite de la unidad
add_author("Culberson & McKetta")
add_ref("culberson:1951")
add_ref("mcCain:1990")
add_ref("banzer:1996")
add_note("Culberson & McKetta (1951) developed the graphical correlation and McCain (1990) presented it in a mathematical form.")
add_internal_note("The data bank information was obtained from the original reference.")
add_internal_note("The equation was verified by using McCain (1990) and Bánzer (1996) as secondary references. Date: October 17 2016.")
end_correlation()

################################################################

begin_correlation("RswMcCoy", "SolutionGasWaterRelation", "SCF_STB")
add_title("McCOY CORRELATION, CALCULATION OF THE SOLUTION GAS-WATER RATIO")
add_parameter("t", "Fahrenheit", "Temperature") 
add_parameter("p", "psia", "Pressure")
add_parameter("nacl_concentration", "Dissolved_Salt_Percent", "Dissolved salt concentration")
add_author("McCoy")
add_ref("mcCoy:1983")
add_ref("banzer:1996")
add_internal_note("The original reference is not available. The correlation was verified by using Bánzer (1996) as a secondary reference. Date: October 17 2016.")
end_correlation()


