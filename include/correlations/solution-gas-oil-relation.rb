 # coding: utf-8

begin_correlation("RsAlShammasi", "SolutionGasOilRelation", "SCF_STB", 6, 3298.6)
add_parameter("yg", "Sgg", "Gas specific gravity", 0.51, 3.44)
add_parameter("p", "psia", "Pressure", "Quantity<Atmosphere>(1)")
add_parameter("yo", "Sg_do", "Oil specific gravity", "Quantity<Api>(63.7)", "Quantity<Api>(6)")
add_synonym("yo", "api", "Api")
add_parameter("t", "Fahrenheit", "Temperature", 74, 341.6)
add_title("AL-SHAMMASI CORRELATION, CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE POINT)")
add_db("Based on a total of 1709 data sets (1661 data sets from 13 different published literature papers and 48 data sets originated from Kuwait reservoirs from unpublished sources).")
add_author("Al-Shammasi")
add_ref("alShammasi:1999")
end_correlation()

################################################################

begin_correlation("RsAlShammasiCalibration", "SolutionGasOilRelation", "SCF_STB", 6, 3298.6)
add_parameter("yg", "Sgg", "Gas specific gravity", 0.51, 3.44)
add_parameter("p", "psia", "Pressure", "Quantity<Atmosphere>(1)")
add_parameter("yo", "Sg_do", "Oil specific gravity", "Quantity<Api>(63.7)", "Quantity<Api>(6)")
add_synonym("yo", "api", "Api")
add_parameter("t", "Fahrenheit", "Temperature", 74, 341.6)
add_parameter("c", "Dim_Less", "c")
add_parameter("m", "Dim_Less", "m")
add_title("AL-SHAMMASI CORRELATION, CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE POINT)")
add_db("Based on a total of 1709 data sets (1661 data sets from 13 different published literature papers and 48 data sets originated from Kuwait reservoirs from unpublished sources).")
add_author("Al-Shammasi")
add_ref("alShammasi:1999")
set_hidden()
end_correlation()

################################################################

begin_correlation("RsAlMarhoun", "SolutionGasOilRelation", "SCF_STB", 26, 1602)
add_title("AL-MARHOUN CORRELATION, CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE POINT)")
add_parameter("yg", "Sgg", "Gas specific gravity", 0.752, 1.367)
add_parameter("p", "psia", "Pressure", 20, 3573)
add_parameter("yo", "Sg_do", "Oil specific gravity", "Quantity<Api>(44.6)", "Quantity<Api>(19.4)")
add_synonym("yo", "api", "Api")
add_parameter("t", "Rankine", "Temperature", "Quantity<Fahrenheit>(74)", "Quantity<Fahrenheit>(240)") 
add_db("Based on 69 bottomhole fluid samples from 69 Middle East oil reservoirs.")
add_ref("alMarhoun:1988")
add_author("Al-Marhoun")
end_correlation()

################################################################

begin_correlation("RsDeGhetto", "SolutionGasOilRelation", "SCF_STB", 17.21, 640.25)
add_title("DE GHETTO, PAONE & VILLA CORRELATION FOR EXTRA-HEAVY (MODIFIED STANDING CORRELATION) AND HEAVY (MODIFIED VAZQUEZ & BEGGS CORRELATION) OILS, CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE POINT)")
add_parameter("yg", "Sgg", "Gas specific gravity", 0.623, 1.517)
add_parameter("p", "psia", "Pressure", 1038.49, 7411.54)
add_parameter("api", "Api", "API oil gravity", 6, 22.3)
add_parameter("t", "Fahrenheit", "Temperature", 131.4, 250.7)
add_parameter("tsep", "Fahrenheit", "Separator temperature", 59, 177.8);
add_parameter("psep", "psia", "Separator pressure", 14.5, 752.2); 
add_db("Based on 1200 measured data points of 63 heavy and extra-heavy oil samples obtained from the Mediterranean Basin, Africa and the Persian Gulf.")
add_db("Oil samples have been divided in two different API gravity classes: extra-heavy oils for API<=10, heavy oils for 10<API<=22.3.")
add_internal_note("De Ghetto et al. use Yg as the average specific gravity of total surface gases, while the original correlation (Standing) uses it as the gravity of dissolved gas. Since Standing doesn't specify if Yg is the gravity of total surface gases or of the separator, it is assumed that the data is from a one stage separation with average gas gravity.")
add_author("De Ghetto, Paone & Villa")
add_ref("deGhetto:1995")
add_ref("standing:1947")
end_correlation()

################################################################

begin_correlation("RsDindorukChristman", "SolutionGasOilRelation", "SCF_STB")
add_title("DINDORUK & CHRISTMAN CORRELATION, CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE POINT)")
add_parameter("yg", "Sgg", "Gas specific gravity", 0.6017, 1.027)
add_parameter("p", "psia", "Pressure")
add_parameter("api", "Api", "API oil gravity", 14.7, 40)
add_parameter("t", "Fahrenheit", "Temperature", 117, 276)
add_author("Dindoruk & Christman")
add_ref("dindoruk:2004")
end_correlation()

################################################################

begin_correlation("RsDoklaOsman", "SolutionGasOilRelation", "SCF_STB")
add_title("DOKLA & OSMAN CORRELATION, CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE POINT)")
add_parameter("yg", "Sgg", "Gas specific gravity", 0.798, 1.29)
add_parameter("p", "psia", "Pressure")
add_parameter("yo", "Sg_do", "Oil specific gravity",  0.8236, 0.886)
add_synonym("yo", "api", "Api")
add_parameter("t", "Rankine", "Temperature", "Quantity<Fahrenheit>(190)", "Quantity<Fahrenheit>(275)") 
add_db("Based on 51 bottomhole samples from UAE reservoirs.")
add_author("Dokla & Osman")
add_ref("dokla:1992")
end_correlation()

################################################################

begin_correlation("RsGlaso", "SolutionGasOilRelation", "SCF_STB", 90, 2637)
add_title("GLASO CORRELATION, CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE POINT)")
add_parameter("yg", "Sgg", "Gas specific gravity", 0.65, 1.276)
add_parameter("p", "psia", "Pressure")
add_parameter("api", "Api", "API oil gravity", 22.3, 48.1)
add_parameter("t", "Fahrenheit", "Temperature", 80, 280)
add_parameter("n2", "MoleFraction", "N2 concentration", "Quantity<MolePercent>(0)",  "Quantity<MolePercent>(26)")
add_parameter("co2","MoleFraction", "CO2 concentration", "Quantity<MolePercent>(0)", "Quantity<MolePercent>(26)")
add_parameter("h2s", "MoleFraction", "H2S concentration", "Quantity<MolePercent>(0)", "Quantity<MolePercent>(50)")
add_precondition("n2", "co2", "h2s")
add_db("Based on 26 samples from the North Sea (collected from wells in the region 56 to 62°N) and 19 samples from the Middle East, Algeria, and several areas in the U.S.")
add_internal_note("P must be strictly greater than zero for the calculation of the logarithm.")
add_author("Glaso")
add_ref("glaso:1980")
# add_ref("banzer:1996") Secondary reference
end_correlation()

################################################################

begin_correlation("RsHanafy", "SolutionGasOilRelation", "SCF_STB", 7, 4272)
add_title("HANAFY ET AL. CORRELATION, CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE POINT)")
add_parameter("p", "psia", "Pressure")
add_db("Based on experimental PVT data of 324 fluid samples taken from 176 wells located in 75 fields. This data represents 15 productive zones of 123 reservoirs distributed along three different regions of Egypt, including the Gulf of Suez, Western Desert, and Sinai.")
add_note("Rs is determined by rearranging the bubble point pressure correlation to solve for the solution gas-oil ratio.")
add_author("Hanafy et al.")
add_ref("hanafy:1997")
set_hidden()
end_correlation()

################################################################

begin_correlation("RsKartoatmodjoSchmidt", "SolutionGasOilRelation", "SCF_STB", 0, 2890)
add_title("KARTOATMODJO & SCHMIDT CORRELATION, CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE POINT)")
add_parameter("yg", "Sgg", "Gas specific gravity", 0.4824, 1.668)
add_parameter("p", "psia", "Pressure")
add_parameter("api", "Api", "API oil gravity", 14.4, 59)
add_parameter("t", "Fahrenheit", "Temperature", 75, 320)
add_parameter("tsep", "Fahrenheit", "Separator temperature", 38, 294);
add_parameter("psep", "psia", "Separator pressure", 14.7, 1414.7); 
add_db("Based on a set of 5392 data points, which represent 740 different crude oil samples.")
add_db("The data bank was collected from PVT reports and literature. The first major source was from South East Asia, mainly Indonesia. The second source was North America, including the offshore area. The rest came from the Middle East and Latin America.")
add_author("Kartoatmodjo & Schmidt")
add_ref("kartoatmodjo:1991")
end_correlation()

################################################################

begin_correlation("RsLasater", "SolutionGasOilRelation", "SCF_STB")
add_title("LASATER CORRELATION, CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE POINT)")
add_parameter("yg", "Sgg", "Gas specific gravity", 0.574, 1.223)
add_parameter("p", "psia", "Pressure")
add_parameter("yo", "Sg_do", "Oil specific gravity", "Quantity<Api>(51.1)", "Quantity<Api>(17.9)")
add_synonym("yo", "api", "Api")
add_parameter("t", "Rankine", "Temperature", "Quantity<Fahrenheit>(82)", "Quantity<Fahrenheit>(272)") 
add_db("Based on 158 experimentally measured bubble point pressures of 137 independent systems.")
add_db("The data were obtained from Canada, Western and Mid-Continental United States, and South America.")
add_note("This correlation is best suited for oils with gravity up to 30 °API.")
add_author("Lasater")
add_ref("lasater:1958")
add_ref("beggs:1987")
add_ref("baker:2015")
end_correlation()

################################################################

begin_correlation("RsPetroskyFarshad", "SolutionGasOilRelation", "SCF_STB", 217, 1406)
add_title("PETROSKY & FARSHAD CORRELATION, CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE POINT)")
add_parameter("yg", "Sgg", "Gas specific gravity", 0.5781, 0.8519)
add_parameter("p", "psia", "Pressure")
add_parameter("api", "Api", "API oil gravity", 16.3, 45)
add_parameter("t", "Fahrenheit", "Temperature", 114, 288)
add_db("Based on 81 laboratory PVT analyses.")
add_db("Fluid samples were obtained from reservoirs located offshore Texas and Louisiana. Producing areas from Galveston Island, eastward, through Main Pass are represented.")
add_author("Petrosky & Farshad")
add_ref("petrosky:1993")
end_correlation()


################################################################

## verificada con python
begin_correlation("RsStanding", "SolutionGasOilRelation", "SCF_STB", 20, 1425)
add_title("STANDING CORRELATION, CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE POINT)")
add_parameter("yg", "Sgg", "Gas specific gravity", 0.59, 0.95)
add_parameter("p", "psia", "Pressure")
add_parameter("api", "Api", "API oil gravity", 16.5, 63.8)
add_parameter("t", "Fahrenheit", "Temperature", 100, 258)
add_db("Based on 105 experimentally determined bubble point pressures from 22 different Californian crude-oil-natural-gas mixtures.")
add_author("Standing")
add_ref("standing:1947")
# add_ref("banzer:1996") Secondary reference
end_correlation()

################################################################    

## verificada con python
begin_correlation("RsTotalCFPDetachedFromPb", "SolutionGasOilRelation", "SCF_STB")
add_title("TOTAL COMPAGNIE FRANÇAISE DES PÉTROLES (CFP) CORRELATION, CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE POINT)")
add_eq("R_s = \\gamma_g \\cdot \\left[ \\frac{P}{C_1} \\cdot 10 ^ {\\left( C_2 \\cdot \\gamma_{API} - C_3 \\cdot T \\right) } \\right] ^ {C_4}")
add_parameter("yg", "Sgg", "Gas specific gravity")
add_parameter("p", "psia", "Pressure")
add_parameter("api", "Api", "API oil gravity", "Api::get_instance().min_val", 45)
add_parameter("t", "Fahrenheit", "Temperature")
add_db("Based on 336 PVT analyses of crude oil from eastern Venezuela.")
add_author("Total CFP")
add_ref("totalCFP:1983")
add_ref("banzer:1996")
set_hidden()
end_correlation()

################################################################

begin_correlation("RsTotalCFP", "SolutionGasOilRelation", "SCF_STB")
add_title("TOTAL COMPAGNIE FRANÇAISE DES PÉTROLES (CFP) CORRELATION, CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE POINT)")
add_eq("R_s = \\gamma_g \\cdot \\left[ \\frac{P}{C_1} \\cdot 10 ^ {\\left( C_2 \\cdot \\gamma_{API} - C_3 \\cdot T \\right) } \\right] ^ {C_4}")
add_parameter("yg", "Sgg", "Gas specific gravity")
add_parameter("p", "psia", "Pressure")
add_parameter("api", "Api", "API oil gravity", "Api::get_instance().min_val", 45)
add_parameter("t", "Fahrenheit", "Temperature")
add_db("Based on 336 PVT analyses of crude oil from eastern Venezuela.")
add_note("Total CFP presented separate equations for solution gas oil ratio (Rs) and bubble point pressure (Pb). The Pb equation provides more realistic results; therefore, Rs is determined by rearranging the bubble point pressure correlation to solve for the solution gas-oil ratio.")
add_author("Total CFP")
add_ref("totalCFP:1983")
add_ref("banzer:1996")
add_ref("lopez:2014")
end_correlation()

################################################################

## verificada con python
begin_correlation("RsVasquezBeggs", "SolutionGasOilRelation", "SCF_STB", 0, 2199)
add_title("VAZQUEZ & BEGGS CORRELATION, CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE POINT)")
add_db("Based on more than 600 laboratory PVT analyses from fields all over the world.")
add_db("The equation for solution GOR was obtained by regression analysis using 5008 measured data points.")
add_internal_note("The gas gravity is corrected if it resulted from a first-stage separation at a pressure other than 100 psig (approximately 114.7 psia).")
add_parameter("yg", "Sgg", "Gas specific gravity", 0.511, 1.35)
add_parameter("p", "psia", "Pressure")
add_parameter("api", "Api", "API oil gravity", 15.3, 59.3)
add_parameter("t", "Fahrenheit", "Temperature", 75, 294)
add_parameter("tsep", "Fahrenheit", "Separator temperature", 76, 150)
add_parameter("psep", "psia", "Separator pressure", 60, 565) 
add_author("Vazquez & Beggs")
add_ref("vazquez:1980")
add_ref("alShammasi:2001")
# add_ref("banzer:1996") Secondary reference
end_correlation()

################################################################<

## verificada con python
begin_correlation("RsVelarde", "SolutionGasOilRelation", "SCF_STB")
add_title("VELARDE, BLASINGAME & McCAIN CORRELATION, CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE POINT)")
add_db("Based on 195 PVT reports, including 2097 data sets.")
add_parameter("yg", "Sgg", "Gas specific gravity", 0.561, 1.101)
add_parameter("pb", "psig", "Bubble point pressure", "Quantity<psia>(106)", "Quantity<psia>(5312)")
add_parameter("p", "psig", "Pressure")
add_parameter("api", "Api", "API oil gravity", 11.6, 53.4)
add_parameter("t", "Fahrenheit", "Temperature", 70, 307)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb", 102, 1808)
add_author("Velarde, Blasingame & McCain")
add_ref("velarde:1997")
end_correlation()


################################################################

## verificada con python
begin_correlation("RsCegarra", "SolutionGasOilRelation", "SCF_STB", 135, 4290)
add_title("CEGARRA CORRELATION (MODIFIED STANDING CORRELATION), CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE POINT)")
add_db("Based on 375 consistent PVT analyses. The data bank was compiled from 8394 fields of eastern Venezuela subdivided into 5192 fields of light oil and 3202 fields of medium oil.")
add_db("The correlation takes the functional form of Standing.")
add_parameter("yg", "Sgg", "Gas specific gravity", 0.654, 1.337)
add_parameter("p", "psia", "Pressure")
add_parameter("api", "Api", "API oil gravity", 22, 41.9)
add_parameter("t", "Fahrenheit", "Temperature", 117, 354)
add_author("Cegarra")
add_ref("cegarra:2007")
add_ref("standing:1947")
add_internal_note("The correlation could not be verified because the original reference is not available. Date: August 16 2016.")
#set_hidden()
end_correlation()

################################################################

## verificada con python
begin_correlation("RsPerezML", "SolutionGasOilRelation", "SCF_STB", 38, 121)
add_title("PÉREZ, HENY & LAGO CORRELATION (MODIFIED MILLÁN-ARCIA CORRELATION), CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE POINT)")
add_db("Based on 35 laboratory PVT analyses of extra-heavy oil samples from the Orinoco Oil Belt, specifically: 16 from Hamaca, 14 from Zuata, three from Cerro Negro, one from Melones, and one from Boscán.")
add_parameter("pb", "psia", "Bubble point pressure", 405, 1335)
add_parameter("p", "psia", "Pressure")
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb", 38, 121)
add_author("Pérez, Heny & Lago")
add_ref("perez:2001")
add_internal_note("The correlation was verified by using the original reference (Pérez, Heny & Lago, 2001). Date: October 02 2017.")
#set_hidden()
end_correlation()


################################################################

## verificada con python
begin_correlation("RsMillanArciaDetachedFromPb", "SolutionGasOilRelation", "SCF_STB", 53, 459)
add_title("MILLÁN-ARCIA CORRELATION, CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE POINT)")
add_db("Venezuelan heavy crudes were correlated.")
add_parameter("pb", "psia", "Bubble point pressure", 222, 3432.7)
add_parameter("p", "psia", "Pressure")
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb")
add_author("Millán-Arcia")
add_ref("millan:1984")
add_ref("perez:2001")
add_internal_note("The correlation could not be verified because the original reference is not available. Date: August 16 2016.")
set_hidden()
end_correlation()


################################################################

begin_correlation("RsMillanArcia", "SolutionGasOilRelation", "SCF_STB", 53,  459)
add_title("MILLÁN-ARCIA CORRELATION, CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE POINT)")
add_db("Venezuelan heavy crudes were correlated.")
add_parameter("yg",  "Sgg", "Gas specific gravity")
add_parameter("p", "psia", "Pressure",  222, 3432.7)
add_parameter("api", "Api", "API oil gravity",  9,  20.2)
add_parameter("t", "Fahrenheit", "Temperature",  87,  195)
add_author("Millán-Arcia")
add_ref("millan:1984")
add_ref("perez:2001")
add_internal_note("The correlation was verified by using a secondary reference (Salazar, 2008). Date: July 27 2017.")
add_note("Millán-Arcia presented separate equations for solution gas oil ratio (Rs) and bubble point pressure (Pb). The correlation for Pb is used to calculate Rs by rearranging the equation.")
#set_hidden()
end_correlation()


################################################################

## verificada con python
begin_correlation("RsManucciRosales", "SolutionGasOilRelation", "SCF_STB", 180, 2500)
add_title("MANUCCI & ROSALES CORRELATION, CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE POINT)")
add_db("Crude oils of eastern Venezuela were correlated, by using differential separation data from laboratory PVT analysis.")
add_parameter("yg", "Sgg", "Gas specific gravity")
add_parameter("p", "psia", "Pressure")
add_parameter("api", "Api", "API oil gravity", 14, 40)
add_parameter("t", "Fahrenheit", "Temperature", 165, 304)
add_author("Manucci & Rosales")
add_ref("manucci:1968")
add_ref("perez:2001")
add_internal_note("The correlation could not be verified because the original reference is not available. Date: August 16 2016.")
#set_hidden()
end_correlation()

################################################################

begin_correlation("RsLopezCR", "SolutionGasOilRelation", "SCF_STB", 12, 110)
add_title("LÓPEZ, CASTILLO & ROJAS CORRELATION, CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE POINT)")
add_db("Based on 48 PVT tests from saturated oil reservoirs located in Bare and Arecuna fields, from the Ayacucho Area of the Faja Petrolífera del Orinoco.")
add_parameter("yg", "Sgg", "Gas specific gravity", 0.5086, 0.75)
add_parameter("p", "psia", "Pressure", 100, 1315)
add_parameter("api", "Api", "API oil gravity", 7.4, 14.5)
add_parameter("t", "Fahrenheit", "Temperature", 124, 220)
add_author("López, Castillo & Rojas")
add_ref("lopez:2014")
add_internal_note("The correlation was verified by using the original reference. Date: April 27 2017.")
#set_hidden()
end_correlation()

################################################################

begin_correlation("RsSalazar", "SolutionGasOilRelation", "SCF_STB", 26, 149)
add_title("SALAZAR CORRELATION, CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE POINT)")
add_db("Based on 136 PVT reports from different areas of the Faja Petrolífera del Orinoco: 60 from Ayacucho, 48 from Junín, 26 from Carabobo, and 2 from Boyacá.")
add_parameter("yg", "Sgg", "Gas specific gravity", 0.5086, 0.75)
add_parameter("p", "psia", "Pressure", 253, 1564)
add_parameter("api", "Api", "API oil gravity", 7.1, 18.9)
add_parameter("t", "Fahrenheit", "Temperature", 108, 169)
add_author("Salazar")
add_ref("salazar:2008")
add_internal_note("The correlation was verified by using the original reference. Date: April 28 2017.")
add_note("Salazar presented separate equations for solution gas oil ratio (Rs) and bubble point pressure (Pb). The best Pb correlation provides a lower average error than the different ones proposed for Rs; therefore, this correlation is used to calculate Rs by rearranging the equation.")
#set_hidden()
end_correlation()

################################################################

begin_correlation("RsSalazarDetachedFromPb", "SolutionGasOilRelation", "SCF_STB", 25, 154)
add_title("SALAZAR CORRELATION, CALCULATION OF SOLUTION GOR (FOR PRESSURES BELOW THE BUBBLE POINT)")
add_db("Based on 136 PVT reports from different areas of the Faja Petrolífera del Orinoco: 60 from Ayacucho, 48 from Junín, 26 from Carabobo, and 2 from Boyacá.")
add_parameter("yg", "Sgg", "Gas specific gravity", 0.5692, 0.6930)
add_parameter("p", "psia", "Pressure", 65, 1940)
add_parameter("api", "Api", "API oil gravity", 7.8, 14.9)
add_parameter("t", "Fahrenheit", "Temperature", 118, 220)
add_author("Salazar")
add_ref("salazar:2008")
add_note("Salazar (2008) presented the correlation as model 1.")
add_internal_note("The correlation was verified by using the original reference. Date: April 28 2017.")
set_hidden()
end_correlation()

################################################################

begin_correlation("RsAbovePb", "SolutionGasOilRelation", "SCF_STB")
add_title("Faked constant for internal computations above bubble point")
add_parameter("pb", "psia", "Bubble point pressure")
add_parameter("p", "psia", "Pressure")
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb")
add_precondition("p", "pb")
set_hidden()
end_correlation()

################################################################


