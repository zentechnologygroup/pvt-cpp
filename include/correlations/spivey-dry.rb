# coding: utf-8

begin_correlation("PvSpiveyMN", "VaporPressure", "mPascal")
add_title("SPIVEY, McCAIN & NORTH, CALCULATION OF VAPOR PRESSURE")
add_db("Vapor pressure of pure water, calculated from the IAWPS-95 formulation.")
add_parameter("t", "Kelvin", "Temperature") 
add_author("Spivey, McCain & North")
add_ref("spivey:2004")
add_ref("mcCain:2011")
add_internal_note("The correlation was verified by using the original reference and McCain et al. (2011) as a secondary reference. Date: October 19 2016.")
set_hidden_blackoil_grid()
set_hidden_drygas_grid()
set_hidden_wetgas_grid()
end_correlation()

################################################################

begin_correlation("MwSpiveyMN", "TmpCorrelation", "Molality_NaCl")
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

begin_correlation("CgfwSpiveyMN", "TmpCorrelation", "mPa_1")
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

begin_correlation("BwSpiveyMNDry", "TmpCorrelation", "Rm3_Sm3") # TODO: la unidad debe ser Scm3/Scm3 
add_title("SPIVEY, McCAIN & NORTH CORRELATION, CALCULATION OF WATER FORMATION VOLUME FACTOR ")
add_db("The volumetric properties of oilfield brines were obtained by combining the correlations developed by Spivey, McCain & North (2004), for density of methane-free sodium chloride brine and solubility of methane in sodium chloride brines, with published correlations for specific volume and partial molar volume of methane in brine.")
add_parameter("t", "Celsius", "Temperature",  0,  275) 
add_parameter("p", "mPascal", "Pressure",  0.1,  200)
add_parameter("pv", "mPascal", "Vapor pressure",  0, 200)
add_parameter("nacl_concentration", "Molality_NaCl", "Dissolved salt concentration",  0,  5.7)
add_precondition("p", "pv")
add_author("Spivey, McCain & North")
add_ref("spivey:2004")
add_ref("mcCain:2011")
add_note("The formation volume factor is found by dividing the volume at reservoir conditions by the volume at stock tank conditions.")
add_note("This correlation provides an accurate and consistent estimate of formation volume factor at pressures above and below the bubble point pressure.")
add_internal_note("The correlation was verified by using the original reference and McCain et al. (2011) as a secondary reference. Date: October 18 2016.")
set_hidden()
end_correlation()
