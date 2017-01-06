# coding: utf-8

begin_correlation("CwaSpiveyMN", "UndersaturatedWaterIsothermalCompressibility", "mPa_1")
add_title("SPIVEY, McCAIN & NORTH, CALCULATION OF WATER ISOTHERMAL COMPRESSIBILITY (FOR PRESSURES ABOVE THE BUBBLE POINT)")
add_db("The volumetric properties of oilfield brines were obtained by combining the correlations developed by Spivey, McCain & North (2004), for density of methane-free sodium chloride brine and solubility of methane in sodium chloride brines, with published correlations for specific volume and partial molar volume of methane in brine.")
add_parameter("t", "Celsius", "Temperature", 0, 275) 
add_parameter("p", "mPascal", "Pressure", 0.1, 200)
add_parameter("nacl_concentration", "Molality_NaCl", "Dissolved salt concentration", 0, 5.7)
add_author("Spivey, McCain & North")
add_ref("spivey:2004")
add_ref("mcCain:2011")
add_note("This correlation provides an accurate and consistent estimate of the coefficient of isothermal compressibility at pressures above the bubble point.")
add_internal_note("The correlation was verified by using the original reference and McCain et al. (2011) as a secondary reference. Date: October 19 2016.")
end_correlation()

################################################################

begin_correlation("CwaDodsonStanding",
                  "UndersaturatedWaterIsothermalCompressibility", "psia_1")
add_title("DODSON & STANDING CORRELATION, CALCULATION OF WATER ISOTHERMAL COMPRESSIBILITY (FOR PRESSURES ABOVE THE BUBBLE POINT)")
add_db("Based on PVT and solubility data for mixtures of natural gas and water, and for mixtures of natural gas and two typical oil-field brines.")
add_parameter("t", "Fahrenheit", "Temperature", 100, 250) 
add_parameter("p", "psia", "Pressure", 500, 5000)
add_parameter("rsw", "SCF_STB", "Solution gas-water ratio")
add_parameter("nacl_concentration", "Dissolved_Salt_Percent", "Dissolved salt concentration", 0.08, 3.5)
add_author("Dodson & Standing")
add_ref("dodson:1944")
add_ref("meehan:1980")
add_ref("numbere:1977")
# add_ref("banzer:1996") Secondary reference
add_note("This correlation is for calculating the isothermal compressibility of water at pressures above the bubble point.")
add_internal_note("The original reference (Meehan, 1980) is not available. The correlation was verified by using Bánzer (1996) as a secondary reference. Date: October 21 2016.")
end_correlation()

################################################################

begin_correlation("CwaOsif", "UndersaturatedWaterIsothermalCompressibility", "psia_1")
add_title("OSIF CORRELATION, CALCULATION OF WATER ISOTHERMAL COMPRESSIBILITY (FOR PRESSURES ABOVE THE BUBBLE POINT)")
add_db("Based on 420 observations.")
add_parameter("t", "Fahrenheit", "Temperature", 200, 270) 
add_parameter("p", "psia", "Pressure", 1000, 20000)
add_parameter("nacl_concentration", "CgL", "Dissolved salt concentration", 0, 200)
add_note("This correlation is for calculating the isothermal compressibility of water at pressures above the bubble point.")
add_internal_note("The correlation was verified by using the original reference and Bánzer (1996) as a secondary reference. Date: October 20 2016.")
add_internal_note("A difference was found between the conversion of units suggested by Bánzer (1996) and the conversion generally expressed in the petroleum unit converters.")
add_internal_note("Pure water density at oilfield standard conditions (60 °F and 14.7 psia): http://www.engineeringtoolbox.com/water-properties-d_1258.html")
add_author("Osif")
add_ref("osif:1988")
# add_ref("banzer:1996") Secondary reference
end_correlation()

################################################################

