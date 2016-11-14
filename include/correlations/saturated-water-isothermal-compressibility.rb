# coding: utf-8

begin_correlation("CwbSpiveyMN", "SaturatedWaterIsothermalCompressibility", "mPa_1")
add_title("SPIVEY, McCAIN & NORTH, CALCULATION OF WATER ISOTHERMAL COMPRESSIBILITY (FOR PRESSURES BELOW THE BUBBLE POINT)")
add_db("The volumetric properties of oilfield brines were obtained by combining the correlations developed by Spivey et al., for density of methane-free sodium chloride brine and solubility of methane in sodium chloride brines, with published correlations for specific volume and partial molar volume of methane in brine.")
add_parameter("t", "Celsius", "Temperature",  0,  275) 
add_parameter("p", "mPascal", "Pressure",  0.1, 200)
add_parameter("nacl_concentration", "Molality_NaCl", "Dissolved salt concentration",  0,  5.7)
add_parameter("z",  "ZFactor", "Gas compressibility factor")
add_author("Spivey, McCain & North")
add_ref("spivey:2004")
add_ref("mcCain:2011")
add_note("This correlation provides an accurate and consistent estimate of the coefficient of isothermal compressibility at pressures below the bubble point.")
add_internal_note("The correlation was verified by using the original reference and McCain et al. (2011) as a secondary reference. Date: October 19 2016.")
end_correlation()

################################################################

begin_correlation("CwbMcCain", "SaturatedWaterIsothermalCompressibility", "psia_1")
add_title("McCAIN CORRELATION, CALCULATION OF WATER ISOTHERMAL COMPRESSIBILITY (FOR PRESSURES BELOW THE BUBBLE POINT)")
add_db("Based on differentiated gas-in-water solubility data. Ramey (1964) presented the pressure differential of gas in solution for a single natural gas containing 88.51 % methane and 6.02 % ethane.")
add_parameter("t", "Fahrenheit", "Temperature", 100, 250) 
add_parameter("p", "psia", "Pressure", 500, 5000)
add_parameter("nacl_concentration", "Dissolved_Salt_Percent", "Dissolved salt concentration",  0, 26) # TODO: el valor debería ser 30
add_parameter("bg", "RB_SCF", "Gas formation volume factor")
add_parameter("bw", "RB_STB", "Water formation volume factor")
add_parameter("cwa", "psia_1", "Water isothermal compressibility above Pb")
add_note("This correlation is for calculating the isothermal compressibility of water at pressures below or equal to the bubble point.")
add_note("Ramey (1964) developed the graphical correlation for the derivative of the solution gas-water ratio with respect to the pressure, and McCain (1990) presented it in a mathematical form.")
add_internal_note("The development ranges for the temperature and pressure were taken from the graph presented by Ramey (1964). The development range for the salt concentration was taken from the graph suggested by McCain (1990).")
add_internal_note("The correlation was verified by using the original reference and Bánzer (1996) as a secondary reference. Date: October 21 2016.")
add_note("The undersaturated water isothermal compressibility is estimated at P and T as if pressure were above bubble point.")
end_correlation()
