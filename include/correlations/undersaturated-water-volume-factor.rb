
begin_correlation("BwaSpiveyMN", "UndersaturatedWaterVolumeFactor", "Sm3_Sm3") # TODO: la unidad debe ser Scm3/Scm3
add_title("SPIVEY, McCAIN & NORTH CORRELATION, CALCULATION OF WATER FORMATION VOLUME FACTOR ")
add_db("The volumetric properties of oilfield brines were obtained by combining the correlations developed by Spivey, McCain & North (2004), for density of methane-free sodium chloride brine and solubility of methane in sodium chloride brines, with published correlations for specific volume and partial molar volume of methane in brine.")
add_parameter("t", "Celsius", "Temperature",  0,  275) 
add_parameter("p", "mPascal", "Pressure",  0.1,  200)
add_parameter("nacl_concentration", "Molality_NaCl", "Dissolved salt concentration",  0,  5.7)
add_author("Spivey, McCain & North")
add_ref("spivey:2004")
add_ref("mcCain:2011")
add_note("The formation volume factor is found by dividing the volume at reservoir conditions by the volume at stock tank conditions.")
add_note("This correlation provides an accurate and consistent estimate of formation volume factor at pressures above the bubble point pressure.")
add_internal_note("The correlation was verified by using the original reference and McCain et al. (2011) as a secondary reference. Date: October 18 2016.")
end_correlation()

################################################################

begin_correlation("BwaMcCain", "UndersaturatedWaterVolumeFactor", "RB_STB")
add_title("McCain CORRELATION, CALCULATION OF WATER FORMATION VOLUME FACTOR (FOR PRESSURES ABOVE THE BUBBLE POINT)")
add_db("Based on the equation presented by McCain (1990).")
add_parameter("p", "psia", "Pressure")
add_parameter("pb", "psia", "Bubble point pressure")
add_parameter("bwbp", "RB_STB", "Water formation volume factor at Pb")
add_parameter("cw",  "psia_1", "Water isothermal compressibility")
add_author("McCain")
add_ref("mcCain:1990")
add_ref("banzer:1996")
add_note("This correlation determines the water formation volume factor for pressures above the bubble point pressure.")
add_internal_note("The correlation was verified by using the original reference and Bánzer (1996) as a secondary reference. Date: October 26 2016.")
end_correlation()

################################################################
