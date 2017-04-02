# coding: utf-8

begin_correlation("BwbSpiveyMN", "SaturatedWaterVolumeFactor", "Rm3_Sm3") # TODO: la unidad debe ser Scm3/Scm3
add_title("SPIVEY, McCAIN & NORTH CORRELATION, CALCULATION OF WATER FORMATION VOLUME FACTOR ")
add_db("The volumetric properties of oilfield brines were obtained by combining the correlations developed by Spivey, McCain & North (2004), for density of methane-free sodium chloride brine and solubility of methane in sodium chloride brines, with published correlations for specific volume and partial molar volume of methane in brine.")
add_parameter("t", "Celsius", "Temperature", 0, 275) 
add_parameter("p", "mPascal", "Pressure", 0.1, 200)
add_parameter("nacl", "Molality_NaCl", "Dissolved salt concentration", 0, 5.7)
add_precondition("t", "p")
add_author("Spivey, McCain & North")
add_ref("spivey:2004")
add_ref("mcCain:2011")
add_note("The formation volume factor is found by dividing the volume at reservoir conditions by the volume at stock tank conditions.")
add_note("This correlation provides an accurate and consistent estimate of formation volume factor at pressures below or equal to the bubble point pressure.")
add_internal_note("The correlation was verified by using the original reference and McCain et al. (2011) as a secondary reference. Date: October 18 2016.")
end_correlation()

################################################################

begin_correlation("BwbMcCain", "SaturatedWaterVolumeFactor", "RB_STB")
add_title("McCain CORRELATION, CALCULATION OF WATER FORMATION VOLUME FACTOR (FOR PRESSURES BELOW OR EQUAL TO THE BUBBLE POINT)")
add_db("Based on graphs, presented by McCain (1990), of the change in volume as a function of reservoir temperature and pressure.")
add_parameter("t", "Fahrenheit", "Temperature", 90, 260)
add_parameter("p", "psia", "Pressure", 1000, 5000)
add_author("McCain")
add_ref("mcCain:1990")
# add_ref("banzer:1996") Secondary reference
add_note("This correlation determines the water formation volume factor for pressures below or equal to bubble point pressure, and it is valid for oilfield waters with widely varying brine concentrations.")
add_internal_note("The correlation was verified by using the original reference and Bánzer (1996) as a secondary reference. Date: October 18 2016.")
end_correlation()

################################################################

begin_correlation("BwbMcCoy", "SaturatedWaterVolumeFactor", "RB_STB")
add_title("McCOY CORRELATION, CALCULATION OF WATER FORMATION VOLUME FACTOR (FOR PRESSURES BELOW OR EQUAL TO THE BUBBLE POINT)")
add_parameter("t", "Fahrenheit", "Temperature")
add_parameter("p", "psia", "Pressure")
add_parameter("nacl", "Dissolved_Salt_Percent", "Dissolved salt concentration")
add_author("McCoy")
add_ref("mcCoy:1983")
# add_ref("banzer:1996") Secondary reference
add_note("This correlation determines the formation volume factor for gas-saturated pure water, for pressures below or equal to bubble point pressure.")
add_internal_note("The original reference is not available. The correlation was verified by using Bánzer (1996) as a secondary reference. Date: October 18 2016.")
end_correlation()
