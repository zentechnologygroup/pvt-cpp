
begin_correlation("UwMaoDuan", "WaterViscosity", "Paxs")
add_title("MAO & DUAN CORRELATION, CALCULATION OF WATER VISCOSITY")
add_db("Based on more than 2200 experimental data points of sodium chloride brines.")
add_parameter("t", "Kelvin", "Temperature",  273, 623)
add_parameter("nacl_concentration", "Molality_NaCl", "Dissolved salt concentration",  0, 6) 
add_parameter("ppw", "Gr_cm3", "Pure water density")
add_author("Mao & Duan")
add_ref("mao:2009")
add_ref("mcCain:2011")
add_internal_note("The application range for P is: 1 - 1000 [bar]. P is used to calculate the density of pure water, which was obtained from the IAPWS-97 international standard equation of state.")
add_internal_note("The upper limit of the development range for the pressure is 350 bar and for the temperature is 573 °K.")
add_internal_note("The authors indicated that the model can be extrapolated beyond the range of data to predict the viscosity up to 1000 bar, because the viscosity of the alkali-chloride solutions increases only very slightly with pressure. The valid temperature range is from 273 °K to 623 °K.")
end_correlation()

################################################################

begin_correlation("UwVanWingen", "WaterViscosity", "CP", 0.1, 2)
add_title("VAN WINGEN CORRELATION, CALCULATION OF WATER VISCOSITY")
add_parameter("t", "Fahrenheit", "Temperature",  0,  350)
add_author("Van Wingen")
add_ref("vanWingen:1950")
add_ref("brill:1974")
add_ref("banzer:1996")
add_internal_note("The original reference is not available. The correlation was verified by using a secondary reference: Bánzer (1996). Date: October 6 2016.")
end_correlation()

################################################################

begin_correlation("UwMatthewsRussel", "WaterViscosity", "CP", 0.1, 2.1)
add_title("MATTHEWS & RUSSELL CORRELATION, CALCULATION OF WATER VISCOSITY")
add_parameter("t", "Fahrenheit", "Temperature",  40,  400)
add_parameter("p", "psia", "Pressure",  14.7,  10000)
add_parameter("nacl_concentration", "Dissolved_Salt_Percent", "Dissolved salt concentration",  0,  26)
add_author("Matthews & Russell")
add_ref("matthews:1967")
add_ref("meehan:1980")
add_ref("banzer:1996")
add_internal_note("The original reference (Meehan, 1980) is not available. The correlation was verified by using a secondary reference: Bánzer (1996). Date: October 6 2016.")
add_internal_note("Matthews & Russell (1967) presented the graphical correlation (from Chesnut, unpublished, Shell Development Co.).")
add_internal_note("The ranges were taken from the graphical correlation.")
end_correlation()

################################################################

begin_correlation("UwMcCain", "WaterViscosity", "CP")
add_title("McCAIN CORRELATION, CALCULATION OF WATER VISCOSITY")
add_parameter("t", "Fahrenheit", "Temperature",  100,  400)
add_parameter("p", "psia", "Pressure",  14.7,  14000)
add_parameter("nacl_concentration", "Dissolved_Salt_Percent", "Dissolved salt concentration",  0,  26)
add_author("McCain")
add_ref("mcCain:1990")
add_internal_note("McCain expressed that the viscosity equation fits the graphical correlation presented by Matthews & Russell (from Chesnut, unpublished, Shell Development Co.) to within 5 % at temperatures between 100 and 400 °F and salinities to 26 %.")
add_internal_note("The equation for the viscosity correction fits data at 86.5 to 167 °F and pressures below 10000 psia to within 4 %. At pressures between 10000 and 14000 psia, the fit is to within 7 %.")
end_correlation()

################################################################

begin_correlation("UwMcCoy", "WaterViscosity", "CP")
add_title("McCOY CORRELATION, CALCULATION OF WATER VISCOSITY")
add_parameter("t",  "Kelvin", "Temperature")
add_parameter("nacl_concentration",  "Dissolved_Salt_Percent", "Dissolved salt concentration")
add_author("McCoy")
add_ref("mcCoy:1983")
add_ref("banzer:1996")
add_internal_note("The original reference is not available. The correlation was verified by using a secondary reference: Bánzer (1996). Date: October 7 2016.")
end_correlation()
