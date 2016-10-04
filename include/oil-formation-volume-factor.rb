# coding: utf-8

## verificada con python
begin_correlation("BoAlmarhoun", "OilFormationVolumeFactor", "RB_STB",
                  1.032, 1.997)
add_title("AL-MARHOUN CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Based on 69 bottomhole fluid samples from 69 Middle East oil reservoirs.")
add_ref("alMarhoun:1988")
add_ref("banzer:1996")                   
add_parameter("yg", "Sgg", 0.752, 1.367)
add_parameter("yo", "Sg_do", "Quantity<Api>(44.6)", "Quantity<Api>(19.4)")
add_parameter("rs", "SCF_STB", 26, 1602)
add_parameter("rsb", "SCF_STB")
add_parameter("t", "Rankine",
              "Quantity<Fahrenheit>(100)", "Quantity<Fahrenheit>(258)")
add_parameter("p", "psia")
add_parameter("pb", "psia", 130, 3573)
add_parameter("co", "psia_1")
add_author("Al Marhoun")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoAlShammasi", "OilFormationVolumeFactor", "RB_STB",
                  1.02, 2.916)
add_title("Al-SHAMMASI CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Based on a total of 1709 data sets (1661 data sets from 13 different published literature papers and 48 data sets originated from Kuwait reservoirs from unpublished sources).")
add_db("The data available after excluding the duplicates and the repeated data sets for formation volume factor are 1345 data sets.")

add_parameter("yg", "Sgg", 0.51, 3.44)
add_parameter("yo", "Sg_do", "Quantity<Api>(63.7)", "Quantity<Api>(6)")
add_parameter("rs", "SCF_STB", 6, 3298.6)
add_parameter("rsb", "SCF_STB")
add_parameter("t", "Fahrenheit", 74, 341.6)
add_parameter("p", "psia")
add_parameter("pb", "psia", 31.7, 7127)
add_parameter("co", "psia_1")
add_author("Al Shammasi")
end_correlation()

###############################################################

## verificada con python
begin_correlation("BoDeGhetto", "OilFormationVolumeFactor", "RB_STB",
                  1.057, 1.362)
add_title("DE GHETTO CORRELATION FOR EXTRA-HEAVY AND HEAVY OILS (VASQUEZ & BEGGS CORRELATION), CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Based on 1200 measured data points of 63 heavy and extra-heavy oil samples obtained from the Mediterranean Basin, Africa and the Persian Gulf.")
add_db("Oil samples have been divided in two different API gravity classes: extra-heavy oils for °API<=10, heavy oils for 10<°API<=22.3.")
add_parameter("yg", "Sgg", 0.623, 1.517)
add_parameter("api", "Api", 6, 22.3)
add_parameter("rs", "SCF_STB", 17.21, 640.25)
add_parameter("rsb", "SCF_STB")
add_parameter("t", "Fahrenheit", 131.4, 250.7)
add_parameter("tsep", "Fahrenheit", 59, 177.8)
add_parameter("p", "psia", 1038.49, 7411.54)
add_parameter("pb", "psia", 208.86, 4021.96)
add_parameter("psep", "psia", 14.5, 752.2)
add_parameter("co", "psia_1", 1.057, 1.362)
add_author("De Guetto et al.")
end_correlation()


################################################################

begin_correlation("BoDindorukChristman", "OilFormationVolumeFactor", "RB_STB",
                  1.0845, 2.7381)
add_title("DINDORUK & CHRISTMAN CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Based on approximately 100 PVT reports from the Gulf of Mexico.")
add_parameter("yg", "Sgg", 0.6017, 1.027)
add_parameter("api", "Api", 14.7, 40)
add_parameter("rs", "SCF_STB")
add_parameter("rsb", "SCF_STB", 133, 3050)
add_parameter("t", "Fahrenheit", 117, 276)
add_parameter("tsep", "Fahrenheit")
add_parameter("p", "psia")
add_parameter("pb", "psia")
add_parameter("co", "psia_1")
add_author("Dindoruk & Christman")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoDoklaOsman", "OilFormationVolumeFactor", "RB_STB",
                  1.216, 2.493)
add_title("DOKLA & OSMAN CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_ref("banzer:1996")
add_db("Based on 51 bottomhole samples from UAE reservoirs.")
add_parameter("yg", "Sgg", 0.789, 1.29)
add_parameter("yo", "Sg_do", 0.8236, 0.886)
add_parameter("rs", "SCF_STB")
add_parameter("rsb", "SCF_STB", 181, 2266)
add_parameter("t", "Rankine",
              "Quantity<Fahrenheit>(190)", "Quantity<Fahrenheit>(275)")
add_parameter("p", "psia")
add_parameter("pb", "psia", 590, 4640)
add_parameter("co", "psia_1")
add_author("Dokla & Osman")
end_correlation()


################################################################

## verificada con python
begin_correlation("BoGlaso", "OilFormationVolumeFactor", "RB_STB",
                  1.087, 2.588)
add_title("GLASO CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_ref("banzer:1996")                  
add_db("Based on 26 samples from the North Sea (collected from wells in the region 56 to 62°N) and 19 samples from the Middle East, Algeria, and several areas in the U.S.")
add_parameter("yg", "Sgg", 0.650, 1.276)
add_parameter("yo", "Sg_do", "Quantity<Api>(48.1)", "Quantity<Api>(22.3)")
add_parameter("rs", "SCF_STB", 90, 2637)
add_parameter("rsb", "SCF_STB")
add_parameter("t", "Fahrenheit", 80, 280)
add_parameter("p", "psia")
add_parameter("pb", "psia", 150, 7127)
add_parameter("co", "psia_1")
add_author("Glaso")
end_correlation()


################################################################

## verificada con python
begin_correlation("BoHanafy", "OilFormationVolumeFactor", "RB_STB",
                  1.032, 4.35)
add_title("HANAFY CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Based on experimental PVT data of 324 fluid samples taken from 176 wells located in 75 fields. This data represents 15 productive zones of 123 reservoirs distributed along three different regions of Egypt, including the Gulf of Suez, Western Desert, and Sinai.")
add_parameter("rs", "SCF_STB", 7, 4272)
add_parameter("rsb", "SCF_STB")
add_parameter("p", "psia")
add_parameter("pb", "psia", 36, 5003)
add_parameter("co", "psia_1", 1.032e-6, 4.35e-6)
add_author("Hanafy")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoKartoatmodjo", "OilFormationVolumeFactor", "RB_STB",
                  1.007, 2.144)
add_title("KARTOATMODJO & SCHMIDT CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Based on a set of 5392 data points, which represent 740 different crude oil samples.")
add_db("The data bank was collected from PVT reports and literature. The first major source was from South East Asia, mainly Indonesia. The second source was North America, including the offshore area. The rest came from the Middle East and Latin America.")
add_parameter("yg", "Sgg", 0.4824, 1.668)
add_parameter("yo", "Sg_do", "Quantity<Api>(59)", "Quantity<Api>(14.4)")
add_parameter("rs", "SCF_STB", 0, 2890)
add_parameter("rsb", "SCF_STB")
add_parameter("t", "Fahrenheit", 75, 320)
add_parameter("tsep", "Fahrenheit", 38, 294)
add_parameter("p", "psia")
add_parameter("pb", "psia", 14.7, 6054.7)
add_parameter("psep", "psia", 14.7, 1414.7)
add_parameter("co", "psia_1")
add_author("Kartoatmodjo & Schmidt")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoLasater", "OilFormationVolumeFactor", "RB_STB")
add_title("LASATER CORRELATION (STANDING CORRELATION APPLIED FOR LASATER), CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Lasater correlation is based on data obtained from Canada, Western and Mid-Continental United States, and South America.")
add_db("Standing correlation for formation volumes of bubble point liquids is based on data collected from 22 different Californian crude-oil-natural-gas mixtures (Western United States).")
add_parameter("yg", "Sgg")
add_parameter("yo", "Sg_do", "Quantity<Api>(51.1)", "Quantity<Api>(17.9)")
add_parameter("rs", "SCF_STB")
add_parameter("rsb", "SCF_STB", 3, 2905)
add_parameter("t", "Fahrenheit", 82, 272)
add_parameter("p", "psia")
add_parameter("pb", "psia", 48, 5780)
add_parameter("co", "psia_1")
add_author("Lasater")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoPetroskyFarshad", "OilFormationVolumeFactor", "RB_STB",
                 1.1178, 1.6229)
add_title("PETROSKY & FARSHAD CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Based on 81 laboratory PVT analyses. Fluid samples were obtained from reservoirs located offshore Texas and Louisiana.")
add_db("Producing areas from Galveston Island, eastward, through Main Pass are represented.")
add_db("Many Gulf of Mexico crudes can be characterized as naphthenic or biodegraded oils.")
add_parameter("yg", "Sgg", 0.5781, 0.8519)
add_parameter("yo", "Sg_do", "Quantity<Api>(45)", "Quantity<Api>(16.3)")
add_parameter("rs", "SCF_STB", 217, 1406)
add_parameter("rsb", "SCF_STB")
add_parameter("t", "Fahrenheit", 114, 288)
add_parameter("p", "psia")
add_parameter("pb", "psia", 1574, 6523)
add_parameter("co", "psia_1")
add_author("Petrosky & Farshad")
end_correlation()


################################################################

## verificada con python
begin_correlation("BoStanding", "OilFormationVolumeFactor", "RB_STB",
                 1.024, 2.15)
add_title("STANDING CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Based on 105 experimentally determined bubble point pressures from 22 different Californian crude-oil-natural-gas mixtures.")
add_parameter("yg", "Sgg", 0.59, 0.95)
add_parameter("yo", "Sg_do", "Quantity<Api>(63.8)", "Quantity<Api>(16.5)")
add_parameter("rs", "SCF_STB", 20, 1425)
add_parameter("rsb", "SCF_STB")
add_parameter("t", "Fahrenheit", 100, 258)
add_parameter("p", "psia")
add_parameter("pb", "psia")
add_parameter("co", "psia_1")
add_author("Standing")
end_correlation()


################################################################

## verificada con python
begin_correlation("BoTotalCFP", "OilFormationVolumeFactor", "RB_STB")
add_title("TOTAL CFP CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_ref("banzer:1996")             
add_parameter("yg", "Sgg")
add_parameter("api", "Api")
add_parameter("rs", "SCF_STB")
add_parameter("rsb", "SCF_STB")
add_parameter("t", "Fahrenheit")
add_parameter("p", "psia")
add_parameter("pb", "psia")
add_parameter("co", "psia_1")
add_author("Total CFP")
end_correlation()


################################################################

## verificada con python
begin_correlation("BoVasquezBeggs", "OilFormationVolumeFactor", "RB_STB",
                  1.028, 2.226)
add_title("VASQUEZ & BEGGS CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_ref("banzer:1996")
add_parameter("yg", "Sgg", 0.511, 1.35)
add_parameter("api", "Api", 15.3, 59.3)
add_parameter("rs", "SCF_STB", 0, 2199)
add_parameter("rsb", "SCF_STB")
add_parameter("t", "Fahrenheit", 75, 294)
add_parameter("tsep", "Fahrenheit", 76, 150)
add_parameter("p", "psia")
add_parameter("pb", "psia")
add_parameter("psep", "psia", 60, 565)
add_parameter("co", "psia_1")
add_author("Vasquez & Beggs")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoCegarra", "OilFormationVolumeFactor", "RB_STB")
add_title("CEGARRA CORRELATION (STANDING CORRELATION APPLIED FOR CEGARRA), CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Based on 375 consistent PVT analyses. The data bank was compiled from 8394 fields of eastern Venezuela subdivided into 5192 fields of light oil and 3202 fields of medium oil.")
add_db("The correlation takes the functional form of Standing correlation, which is based on 105 experimental data of 22 different crude-oil-natural-gas mixtures from California (USA).")
add_note("The isothermal compressibility of crude oils (Co) used in this equation is obtained from the Vasquez & Beggs correlation.")
add_note("The correlation's development ranges could not be verified because the original reference is not available. Date: August 22 2016.")
add_parameter("yg", "Sgg", 0.654, 1.337)
add_parameter("yo", "Sg_do", "Quantity<Api>(41.8)", "Quantity<Api>(22)")
add_parameter("rs", "SCF_STB", 135, 4290)
add_parameter("rsb", "SCF_STB")
add_parameter("t", "Fahrenheit", 117, 354)
add_parameter("p", "psia")
add_parameter("pb", "psia", 735, 7303)
add_parameter("co", "psia_1")
add_author("Cegarra")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoPerezML", "OilFormationVolumeFactor", "RB_STB",
                  1.032, 1.183)
add_title("PEREZ ET AL. CORRELATION (EZEKWE-QUERIN-HUMPHREY CORRELATION USED BY PEREZ ET AL.), CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_note("The correlation could not be verified because the original reference is not available. Date: August 22 2016.")
add_parameter("yg", "Sgg", 0.571, 0.981)
add_parameter("yo", "Sg_do", "Quantity<Api>(12.9)", "Quantity<Api>(6.4)")
add_parameter("rsb", "SCF_STB", 38, 121)
add_parameter("t", "Fahrenheit", 112, 300)
add_parameter("p", "psia")
add_parameter("pb", "psia", 405, 1335)
add_parameter("co", "psia_1")
add_author("Pérez et al.")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoVelardeMcCain", "OilFormationVolumeFactor", "RB_STB",
                  1.040, 2.082)
add_title("VELARDE, BLASINGAME & McCAIN CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Based on 195 PVT reports, including 2097 data sets.")
add_parameter("yg", "Sgg", 0.561, 1.101)
add_parameter("yo", "Sg_do", "Quantity<Api>(53.4)", "Quantity<Api>(11.6)")
add_parameter("rs", "SCF_STB")
add_parameter("rsb", "SCF_STB", 102, 1808)
add_parameter("t", "Fahrenheit", 70, 307)
add_parameter("p", "psia")
add_parameter("pb", "psia", 106, 5312)
add_parameter("co", "psia_1")
add_author("Velarde et al.")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoMillanArcia", "OilFormationVolumeFactor", "RB_STB",
                  1.014, 1.248)
add_title("MILLÁN-ARCIA CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Venezuelan heavy crudes were correlated.")
add_parameter("yg", "Sgg")
add_parameter("api", "Api", 9, 20.2)
add_parameter("rs", "SCF_STB", 53, 459)
add_parameter("rsb", "SCF_STB")
add_parameter("t", "Fahrenheit", 87, 195)
add_parameter("p", "psia")
add_parameter("pb", "psia", 222.0, 3432.7)
add_parameter("co", "psia_1")
add_author("Millán Arcia")
end_correlation()


################################################################

## verificada con python
begin_correlation("BoManucciRosales", "OilFormationVolumeFactor", "RB_STB",
                  1.050, 2.400)
add_title("MANUCCI-ROSALES CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Crude oils of eastern Venezuela were correlated, by using differential separation data from laboratory PVT analysis.")
add_parameter("yg", "Sgg")
add_parameter("api", "Api", 14, 40)
add_parameter("rs", "SCF_STB", 180, 2500)
add_parameter("rsb", "SCF_STB")
add_parameter("t", "Fahrenheit", 165, 304)
add_parameter("p", "psia")
add_parameter("pb", "psia", 1230, 5035)
add_parameter("co", "psia_1")
add_author("Manucci Rosales")
end_correlation()
