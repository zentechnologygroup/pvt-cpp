 # coding: utf-8

begin_correlation("RsAlShammasi", "SolutionGasOilRelation", "SCF_STB")
add_title("AL-SHAMMASI CORRELATION, CALCULATION OF SOLUTION GOR")
add_parameter("yg", "Sgg", "Gas specific gravity", 0.51, 3.44)
add_parameter("pb", "psia", "Bubble point pressure", 31.7, 7127)
add_parameter("p", "psia", "Pressure", "Quantity<Atmosphere>(1)")
add_parameter("yo", "Sg_do", "Oil specific gravity", "Quantity<Api>(63.7)", "Quantity<Api>(6)")
add_parameter("t", "Fahrenheit", "Temperature", 74, 341.6)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb", 6, 3298.6)
add_db("Based on a total of 1709 data sets (1661 data sets from 13 different published literature papers and 48 data sets originated from Kuwait reservoirs from unpublished sources).")
add_author("Al Shammasi")
end_correlation()

################################################################

begin_correlation("RsAlMarhoun", "SolutionGasOilRelation", "SCF_STB")
add_title("AL-MARHOUN CORRELATION, CALCULATION OF SOLUTION GOR")
add_parameter("yg", "Sgg", "Gas specific gravity", 0.752, 1.367)
add_parameter("pb", "psia", "Bubble point pressure", 130, 3573)
add_parameter("p", "psia", "Pressure", 20, 3573)
add_parameter("yo", "Sg_do", "Oil specific gravity", "Quantity<Api>(44.6)", "Quantity<Api>(19.4)")
add_parameter("t", "Rankine", "Temperature", "Quantity<Fahrenheit>(74)", "Quantity<Fahrenheit>(240)") 
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb", 26, 1602)
add_db("Based on 69 bottomhole fluid samples from 69 Middle East oil reservoirs.")
add_ref("alMarhoun:1988")
add_author("Al Marhoun")
end_correlation()

################################################################

begin_correlation("RsDeGhetto", "SolutionGasOilRelation", "SCF_STB")
add_title("DE GHETTO CORRELATION FOR EXTRA-HEAVY (MODIFIED STANDING CORRELATION) AND HEAVY (MODIFIED VASQUEZ-BEGGS CORRELATION) OILS, CALCULATION OF SOLUTION GOR")
add_parameter("yg", "Sgg", "Gas specific gravity", 0.623, 1.517)
add_parameter("pb", "psia", "Bubble point pressure", 208.6, 4021.96)
add_parameter("p", "psia", "Pressure", 1038.49, 7411.54)
add_parameter("api", "Api", "API oil gravity", 6, 22.3)
add_parameter("t", "Fahrenheit", "Temperature", 131.4, 250.7)
add_parameter("tsep", "Fahrenheit", "Separator temperature", 59, 177.8);
add_parameter("psep", "psia", "Separator pressure", 14.5, 752.2); 
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb", 26, 1602)
add_db("DE GHETTO, PAONE & VILLA CORRELATION FOR EXTRA-HEAVY (MODIFIED STANDING CORRELATION) AND HEAVY (MODIFIED VASQUEZ-BEGGS CORRELATION) OILS, CALCULATION OF SOLUTION GOR")
add_db("Based on 1200 measured data points of 63 heavy and extra-heavy oil samples obtained from the Mediterranean Basin, Africa and the Persian Gulf.")
add_db("Oil samples have been divided in two different API gravity classes: extra-heavy oils for API<=10, heavy oils for 10<API<=22.3.")
add_internal_note("De Ghetto uses Yg as the average specific gravity of total surface gases, while the original correlation (Standing) uses it as the gravity of dissolved gas. Since Standing doesn't specify if Yg is the gravity of total surface gases or of the separator, it is assumed that the data is from a one stage separation with average gas gravity.")
add_author("De Ghetto, Paone & Villa")
end_correlation()

################################################################

begin_correlation("RsDindorukChristman", "SolutionGasOilRelation", "SCF_STB")
add_title("DINDORUK & CHRISTMAN CORRELATION, CALCULATION OF SOLUTION GOR")
add_parameter("yg", "Sgg", "Gas specific gravity", 0.6017, 1.027)
add_parameter("pb", "psia", "Bubble point pressure", 926, 12230)
add_parameter("p", "psia", "Pressure")
add_parameter("api", "Api", "API oil gravity", 14.7, 40)
add_parameter("t", "Fahrenheit", "Temperature", 117, 276)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb", 133, 3050)
add_author("Dindoruk & Christman")
end_correlation()

################################################################

begin_correlation("RsDoklaOsman", "SolutionGasOilRelation", "SCF_STB")
add_title("DOKLA & OSMAN CORRELATION, CALCULATION OF SOLUTION GOR")
add_parameter("yg", "Sgg", "Gas specific gravity", 0.798, 1.29)
add_parameter("pb", "psia", "Bubble point pressure", 590, 4640)
add_parameter("p", "psia", "Pressure")
add_parameter("yo", "Sg_do", "Oil specific gravity",  0.8236, 0.886)
add_parameter("t", "Rankine", "Temperature", "Quantity<Fahrenheit>(190)", "Quantity<Fahrenheit>(275)") 
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb", 181, 2266)
add_db("Based on 51 bottomhole samples from UAE reservoirs.")
add_author("Dokla & Osman")
end_correlation()

################################################################

begin_correlation("RsGlaso", "SolutionGasOilRelation", "SCF_STB")
add_title("GLASO CORRELATION, CALCULATION OF SOLUTION GOR")
add_parameter("yg", "Sgg", "Gas specific gravity", 0.65, 1.276)
add_parameter("pb", "psia", "Bubble point pressure", 150, 7127)
add_parameter("p", "psia", "Pressure")
add_parameter("api", "Api", "API oil gravity", 22.3, 48.1)
add_parameter("t", "Fahrenheit", "Temperature", 80, 280)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb", 90, 2637)
add_db("Based on 26 samples from the North Sea (collected from wells in the region 56 to 62°N) and 19 samples from the Middle East, Algeria, and several areas in the U.S. cg")
add_ref("banzer:1996")
add_author("Glaso")
end_correlation()

################################################################

begin_correlation("RsHanafy", "SolutionGasOilRelation", "SCF_STB")
add_title("HANAFY CORRELATION, CALCULATION OF SOLUTION GOR")
add_parameter("pb", "psia", "Bubble point pressure", 36, 5003)
add_parameter("p", "psia", "Pressure")
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb", 97, 4272)
add_db("Based on experimental PVT data of 324 fluid samples taken from 176 wells located in 75 fields. This data represents 15 productive zones of 123 reservoirs distributed along three different regions of Egypt, including the Gulf of Suez, Western Desert, and Sinai.")
add_author("Hanafy")
end_correlation()

################################################################

begin_correlation("RsKartoatmodjoSchmidt", "SolutionGasOilRelation", "SCF_STB")
add_title("KARTOATMODJO & SCHMIDT CORRELATION, CALCULATION OF SOLUTION GOR")
add_parameter("yg", "Sgg", "Gas specific gravity", 0.28, 1.668)
add_parameter("pb", "psia", "Bubble point pressure", 14.7, 6054.7)
add_parameter("p", "psia", "Pressure")
add_parameter("api", "Api", "API oil gravity", 14.4, 59)
add_parameter("t", "Fahrenheit", "Temperature", 75, 320)
add_parameter("tsep", "Fahrenheit", "Separator temperature", 38, 294);
add_parameter("psep", "psia", "Separator pressure", 14.7, 1414.7); 
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb", 0, 2890)
add_db("Based on a set of 5392 data points, which represent 740 different crude oil samples.")
add_db("The data bank was collected from PVT reports and literature. The first major source was from South East Asia, mainly Indonesia. The second source was North America, including the offshore area. The rest came from the Middle East and Latin America.")
add_author("Kartoatmodjo & Schmidt")
end_correlation()

################################################################

begin_correlation("RsLasater", "SolutionGasOilRelation", "SCF_STB")
add_title("LASATER CORRELATION, CALCULATION OF SOLUTION GOR")
add_parameter("yg", "Sgg", "Gas specific gravity", 0.574, 1.223)
add_parameter("pb", "psia", "Bubble point pressure", 48, 5780)
add_parameter("p", "psia", "Pressure")
add_parameter("yo", "Sg_do", "Oil specific gravity", "Quantity<Api>(51.1)", "Quantity<Api>(17.9)")
add_parameter("t", "Rankine", "Temperature", "Quantity<Fahrenheit>(82)", "Quantity<Fahrenheit>(272)") 
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb", 3, 2905)
add_db("Based on 158 experimentally measured bubble point pressures of 137 independent systems.")
add_db("The data were obtained from Canada, Western and Mid-Continental United States, and South America.")
add_author("Lasater")
end_correlation()

################################################################

begin_correlation("RsPetroskyFarshad", "SolutionGasOilRelation", "SCF_STB")
add_title("PETROSKY & FARSHAD CORRELATION, CALCULATION OF SOLUTION GOR")
add_parameter("yg", "Sgg", "Gas specific gravity", 0.5781, 0.8519)
add_parameter("pb", "psia", "Bubble point pressure", 1574, 6523)
add_parameter("p", "psia", "Pressure")
add_parameter("api", "Api", "API oil gravity", 16.3, 45)
add_parameter("t", "Fahrenheit", "Temperature", 114, 288)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb", 217, 1406)
add_db("Based on 81 laboratory PVT analyses.")
add_db("Fluid samples were obtained from reservoirs located offshore Texas and Louisiana. Producing areas from Galveston Island, eastward, through Main Pass are represented.")
add_author("Petrosky & Farshad")
end_correlation()


################################################################

## verificada con python
begin_correlation("RsStanding", "SolutionGasOilRelation", "SCF_STB")
add_title("STANDING CORRELATION, CALCULATION OF SOLUTION GOR")
add_parameter("yg", "Sgg", "Gas specific gravity", 0.59, 0.95)
add_parameter("pb", "psia", "Bubble point pressure", 130, 700)
add_parameter("p", "psia", "Pressure")
add_parameter("api", "Api", "API oil gravity", 16.5, 63.8)
add_parameter("t", "Fahrenheit", "Temperature", 100, 258)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb", 20, 1425)
add_db("Based on 105 experimentally determined bubble point pressures from 22 different Californian crude-oil-natural-gas mixtures.")
add_ref("banzer:1996")
add_author("Standing")
end_correlation()

################################################################    

## verificada con python
begin_correlation("RsTotalCFP", "SolutionGasOilRelation", "SCF_STB")
add_title("TOTAL CFP CORRELATION, CALCULATION OF SOLUTION GOR")
add_eq("R_s = \\gamma_g \\cdot \\left[ \\frac{P}{C_1} \\cdot 10 ^ {\\left( C_2 \\cdot \\gamma_{API} - C_3 \\cdot T \\right) } \\right] ^ {C_4}")
add_parameter("yg", "Sgg", "Gas specific gravity")
add_parameter("pb", "psia", "Bubble point pressure")
add_parameter("p", "psia", "Pressure")
add_parameter("api", "Api", "API oil gravity", 0, 45)
add_parameter("t", "Fahrenheit", "Temperature")
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb")
add_db("Based on 336 PVT analyses of crude oil from eastern Venezuela.")
add_ref("banzer:1996")
add_author("Total CFP")
end_correlation()

################################################################

## verificada con python
begin_correlation("RsVasquezBeggs", "SolutionGasOilRelation", "SCF_STB")
add_title("VASQUEZ & BEGGS CORRELATION, CALCULATION OF SOLUTION GOR")
add_db("Based on more than 600 laboratory PVT analyses from fields all over the world.")
add_db("The equation for solution GOR was obtained by regression analysis using 5008 measured data points.")
add_note("The gas gravity is corrected if it resulted from a first-stage separation at a pressure other than 100 psig (approximately 114.7 psia).")
add_parameter("yg", "Sgg", "Gas specific gravity", 0.511, 1.35)
add_parameter("pb", "psia", "Bubble point pressure", 15, 6055)
add_parameter("p", "psia", "Pressure")
add_parameter("api", "Api", "API oil gravity", 15.3, 59.3)
add_parameter("t", "Fahrenheit", "Temperature", 75, 294)
add_parameter("tsep", "Fahrenheit", "Separator temperature", 76, 150)
add_parameter("psep", "psia", "Separator pressure", 60, 595) 
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb", 0, 2199)
add_author("Vasquez & Beggs")
end_correlation()

################################################################<

## verificada con python
begin_correlation("RsVelarde", "SolutionGasOilRelation", "SCF_STB")
add_title("VELARDE CORRELATION, CALCULATION OF SOLUTION GO")
add_db("Based on 195 PVT reports, including 2097 data sets.")
add_parameter("yg", "Sgg", "Gas specific gravity", 0.561, 1.101)
add_parameter("pb", "psig", "Bubble point pressure", "Quantity<psia>(106)", "Quantity<psia>(5312)")
add_parameter("p", "psig", "Pressure")
add_parameter("api", "Api", "API oil gravity", 11.6, 53.4)
add_parameter("t", "Fahrenheit", "Temperature", 70, 307)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb", 102, 1808)
add_author("Velarde")
end_correlation()


################################################################

## verificada con python
begin_correlation("RsCegarra", "SolutionGasOilRelation", "SCF_STB")
add_title("CEGARRA CORRELATION (MODIFIED STANDING CORRELATION), CALCULATION OF SOLUTION GOR")
add_db("Based on 375 consistent PVT analyses. The data bank was compiled from 8394 fields of eastern Venezuela subdivided into 5192 fields of light oil and 3202 fields of medium oil. The correlation takes the functional form of Standing.")
add_parameter("yg", "Sgg", "Gas specific gravity", 0.654, 1.337)
add_parameter("pb", "psia", "Bubble point pressure", 735, 7303)
add_parameter("p", "psia", "Pressure")
add_parameter("api", "Api", "API oil gravity", 22, 41.9)
add_parameter("t", "Fahrenheit", "Temperature", 117, 354)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb", 135, 4290)
add_author("Cegarra")
end_correlation()

################################################################

## verificada con python
begin_correlation("RsPerezML", "SolutionGasOilRelation", "SCF_STB")
add_title("PEREZ ET AL. CORRELATION (MODIFIED MILLÁN-ARCIA CORRELATION), CALCULATION OF SOLUTION GOR")
add_parameter("yg", "Sgg", "Gas specific gravity")
add_parameter("pb", "psia", "Bubble point pressure", 405, 1335)
add_parameter("p", "psia", "Pressure")
add_parameter("api", "Api", "API oil gravity", 6.4, 12.9)
add_parameter("t", "Fahrenheit", "Temperature", 112, 300)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb", 38, 121)
add_author("Pérez et al.")
end_correlation()


################################################################

## verificada con python
begin_correlation("RsMillanArcia", "SolutionGasOilRelation", "SCF_STB")
add_doc(%Q[  MILLÁN-ARCIA CORRELATION, CALCULATION OF SOLUTION GOR 

  Venezuelan heavy crudes,
    
  @see: E.A. Millán-Arcia, 1984: Correlaciones para estimar el
  comportamiento PVT de crudos pesados venezolanos. I Jornadas
  Técnicas Corpoven 

  @see: C.A. Heny, V.L. Pérez, M.E. Lago, 2001: Evaluación y
  generación de correlaciones PVT para crudos extrapesados
  venezolanos. Informe Técnico INT-8713,2001. Proyecto: Estudio y
  Modelaje de Procesos de Recuperación de Crudos Pesados. Los Teques,
  Octubre de 2001 
 ])

add_parameter("yg", "Sgg", "Gas specific gravity")
add_parameter("pb", "psia", "Bubble point pressure", 222, 3432.7)
add_parameter("p", "psia", "Pressure")
add_parameter("api", "Api", "API oil gravity", 9, 20.2)
add_parameter("t", "Fahrenheit", "Temperature", 87, 195)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb", 53, 459)
add_author("Millán Arcia")
end_correlation()


################################################################

## verificada con python
begin_correlation("RsManucciRosales", "SolutionGasOilRelation", "SCF_STB")
add_title("MANUCCI-ROSALES CORRELATION, CALCULATION OF SOLUTION GOR")
add_db("Crude oils of eastern Venezuela were correlated, by using differential separation data from laboratory PVT analysis.")
add_parameter("yg", "Sgg", "Gas specific gravity")
add_parameter("pb", "psia", "Bubble point pressure", 1230, 5035)
add_parameter("p", "psia", "Pressure")
add_parameter("api", "Api", "API oil gravity", 14, 40)
add_parameter("t", "Fahrenheit", "Temperature", 165, 304)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb", 180, 2500)
add_author("Manucci Rosales")
end_correlation()
