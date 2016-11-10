# coding: utf-8

begin_correlation("PvSpiveyMN", "WaterIsothermalCompressibility", "mPascal")
add_title("SPIVEY, McCAIN & NORTH, CALCULATION OF VAPOR PRESSURE")
add_db("Vapor pressure of pure water, calculated from the IAWPS-95 formulation.")
add_parameter("tk", "Kelvin", "Temperature") 
add_author("Spivey, McCain & North")
add_ref("spivey:2004")
add_ref("mcCain:2011")
add_internal_note("The correlation was verified by using the original reference and McCain et al. (2011) as a secondary reference. Date: October 19 2016.")
set_hidden()
end_correlation()

################################################################

begin_correlation("MwSpiveyMN", "WaterIsothermalCompressibility", "Molality_NaCl")
add_title("SPIVEY, McCAIN & NORTH, CALCULATION OF SOLUBILITY OF METHANE IN BRINE")
add_db("Calculation of solubility of methane in brine.")
add_parameter("t", "Celsius", "Temperature") 
add_parameter("p", "mPascal", "Pressure")
add_parameter("pv", "mPascal", "Vapor pressure")
add_parameter("nacl_concentration", "Molality_NaCl", "Dissolved salt concentration")
add_author("Spivey, McCain & North")
add_ref("spivey:2004")
add_ref("mcCain:2011")
add_internal_note("The correlation was verified by using the original reference and McCain et al. (2011) as a secondary reference. Date: October 19 2016.")
set_hidden()
end_correlation()

################################################################

begin_correlation("CgfwSpiveyMN", "WaterIsothermalCompressibility", "mPa_1")
add_title("SPIVEY, McCAIN & NORTH, CALCULATION OF COMPRESSIBILITY OF METHANE-FREE BRINE")
add_db("Calculation of compressibility of methane-free brine.")
add_parameter("t", "Celsius", "Temperature") 
add_parameter("p", "mPascal", "Pressure")
add_parameter("nacl_concentration", "Molality_NaCl", "Dissolved salt concentration")
add_author("Spivey, McCain & North")
add_ref("spivey:2004")
add_ref("mcCain:2011")
add_internal_note("The correlation was verified by using the original reference and McCain et al. (2011) as a secondary reference. Date: October 19 2016.")
set_hidden()
end_correlation()

################################################################
