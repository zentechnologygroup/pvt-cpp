# coding: utf-8

begin_correlation("PwSpiveyMN", "WaterDensity", "Gr_cm3")
add_parameter("t", "Celsius", "Temperature", 0, 275) 
add_parameter("p", "mPascal", "Pressure", 0.9, 200)
add_parameter("nacl_concentration", "Molality_NaCl", "Dissolved salt concentration", 0, 5.7)
add_precondition("t", "p")
add_title("SPIVEY, McCAIN & NORTH CORRELATION, CALCULATION OF WATER DENSITY")
add_db("The brine density correlation was obtained by combining the correlations developed by Spivey, McCain & North (2004), for density of methane-free sodium chloride brine and solubility of methane in sodium chloride brines, with published correlations for specific volume and partial molar volume of methane in brine.")
add_db("The methane solubility correlation is based on 1039 data points from a database of volumetric properties of sodium chloride brine.")
add_author("Spivey, McCain & North")
add_ref("spivey:2004")
add_ref("mcCain:2011")
add_note("The authors developed an empirical correlation for the solubility of methane in pure water, which is then corrected for salinity using a modification of Duan's method.")
add_internal_note("The ranges were established from the intersection of the ranges of the methane-free water density correlation and the methane solubility correlation.")
add_internal_note("The correlation was verified by using the original reference and McCain et al. (2011) as a secondary reference. Date: October 12 2016.")
end_correlation()

################################################################

begin_correlation("PwSpiveyMNGasFree", "WaterDensity", "Gr_cm3")
add_parameter("t", "Celsius", "Temperature", 0, 275) 
add_parameter("p", "mPascal", "Pressure", 0.1, 200)
add_parameter("nacl_concentration", "Molality_NaCl",
       "Dissolved salt concentration", 0, 5.7)
add_title("SPIVEY, McCAIN & NORTH CORRELATION, CALCULATION OF METHANE-FREE WATER DENSITY")
add_db("Based on 1461 data points from a database of volumetric properties of methane-free sodium chloride brine.")
add_author("Spivey, McCain & North (Methane-Free)")
add_ref("spivey:2004")
add_ref("mcCain:2011")
add_note("Spivey, McCain & North (2004) used the IAPWS-95 international standard equation of state for water to determine the coefficients of the correlation for pure water density.")
add_internal_note("The correlation was verified by using the original reference and McCain et al. (2011) as a secondary reference. Date: October 12 2016.")
set_hidden()
end_correlation()

################################################################

begin_correlation("PwMcCain", "WaterDensity", "Lb_ft3")
add_parameter("nacl_concentration", "Dissolved_Salt_Percent", "Dissolved salt concentration", 0, 26) 
add_parameter("bw", "RB_STB", "Water formation volume factor")
add_title("McCAIN CORRELATION, CALCULATION OF WATER DENSITY")
add_db("The brine density is expressed as a function of the total percentage of the dissolved solids weight and the water formation volume factor.")
add_author("McCain")
add_ref("mcCain:1990")
# add_ref("banzer:1996") Secondary reference
add_internal_note("The range for the dissolved salt concentration was taken from the graph presented by McCain (1990).")
add_internal_note("The correlation was verified by using the original reference and Bánzer (1996) as a secondary reference. Date: October 11 2016.")
end_correlation()

################################################################

begin_correlation("PpwSpiveyMN", "PureWaterDensity", "Gr_cm3")
add_parameter("t", "Celsius", "Temperature", 0, 275)
add_parameter("p", "mPascal", "Pressure", 0.1, 200)
add_title("SPIVEY, McCAIN & NORTH CORRELATION, CALCULATION OF PURE WATER DENSITY")
add_author("Spivey, McCain & North")
add_ref("spivey:2004")
add_note("Spivey, McCain & North (2004) used the IAPWS-95 international standard equation of state for water to determine the coefficients of the correlation for pure water.")
add_internal_note("The correlation was verified by using the original reference.")
end_correlation()
