# coding: utf-8
begin_correlation("BoaMcCain", "UndersaturatedOilFormationVolumeFactor", "RB_STB")
add_title("McCain CORRELATION, CALCULATION OF UNDERSATURATED OIL FORMATION VOLUME FACTOR")
add_db("Based on the equation presented by McCain (1990).")
add_db("The volume factor at bubble point pressure is adjusted to higher pressures through the use of the coefficient of isothermal compresibility.")
add_ref("mcCain:1990")  
add_ref("banzer:1996")                 
add_parameter("bobp", "RB_STB", "Oil formation volume factor at pb")
add_parameter("p", "psia", "Pressure")
add_parameter("pb", "psia", "Bubble point pressure")
add_parameter("co",  "psia_1", "Coefficient of isothermal compressibility")
add_author("McCain")
add_note("This correlation determines the oil formation volume factor for pressures above the bubble point pressure.")
add_internal_note("The correlation was verified by using the original reference and Bánzer (1996) as a secondary reference. Date: October 26 2016")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoaDeGhetto", "OilFormationVolumeFactor", "RB_STB")
add_title("DE GHETTO, PAONE & VILLA CORRELATION FOR EXTRA-HEAVY AND HEAVY OILS (VAZQUEZ & BEGGS CORRELATION), CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Based on 1200 measured data points of 63 heavy and extra-heavy oil samples obtained from the Mediterranean Basin, Africa and the Persian Gulf.")
add_db("Oil samples have been divided in two different API gravity classes: extra-heavy oils for °API<=10, heavy oils for 10<°API<=22.3.")
add_parameter("bobp", "RB_STB", "Formation volume factor at pb") 
add_parameter("yg", "Sgg", "Gas specific gravity",  0.623,  1.517)
add_parameter("api",  "Api", "API oil gravity",  6,  22.3)
add_parameter("rsb",  "SCF_STB", "Solution GOR at Pb")
add_parameter("t", "Fahrenheit", "Temperature",  131.4,  250.7)
add_parameter("tsep", "Fahrenheit", "Separator temperature",  59,  177.8)
add_parameter("p", "psia", "Pressure",  1038.49,  7411.54)
add_parameter("pb", "psia", "Bubble point pressure",  208.86,  4021.96)
add_parameter("psep", "psia", "Separator pressure",  14.5,  752.2)
add_author("De Ghetto, Paone & Villa")
add_ref("deGhetto:1995")
add_internal_note("De Ghetto et al. recommend the use of the Vazquez & Beggs correlation for the estimation of Bo. The values for the coefficients c1, c2, and c3 are selected only for API<=30.")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoaHanafy", "OilFormationVolumeFactor", "RB_STB")
add_title("HANAFY ET AL. CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Based on experimental PVT data of 324 fluid samples taken from 176 wells located in 75 fields. This data represents 15 productive zones of 123 reservoirs distributed along three different regions of Egypt, including the Gulf of Suez, Western Desert, and Sinai.")
add_parameter("bobp", "RB_STB", "Formation volume factor at pb")
add_parameter("p",  "psia", "Pressure")
add_parameter("pb", "psia", "Bubble point pressure",  36,  5003)
add_author("Hanafy et al.")
add_ref("hanafy:1997")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoaKartoatmodjo", "OilFormationVolumeFactor", "RB_STB")
add_title("KARTOATMODJO & SCHMIDT CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Based on a set of 5392 data points, which represent 740 different crude oil samples.")
add_db("The data bank was collected from PVT reports and literature. The first major source was from South East Asia, mainly Indonesia. The second source was North America, including the offshore area. The rest came from the Middle East and Latin America.")
add_parameter("bobp", "RB_STB", "Formation volume factor at pb")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.4824,  1.668)
add_parameter("yo", "Sg_do", "Oil specific gravity", "Quantity<Api>(59)", "Quantity<Api>(14.4)")
add_synonym("yo", "api", "Api")
add_parameter("rsb",  "SCF_STB", "Solution GOR at Pb")
add_parameter("t", "Fahrenheit", "Temperature",  75,  320)
add_parameter("tsep", "Fahrenheit", "Separator temperature",  38,  294)
add_parameter("p",  "psia", "Pressure")
add_parameter("pb", "psia", "Bubble point pressure",  14.7,  6054.7)
add_parameter("psep", "psia", "Separator pressure",  14.7,  1414.7)
add_author("Kartoatmodjo & Schmidt")
add_ref("kartoatmodjo:1991")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoaPetroskyFarshad", "OilFormationVolumeFactor", "RB_STB")
add_title("PETROSKY & FARSHAD CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Based on 81 laboratory PVT analyses. Fluid samples were obtained from reservoirs located offshore Texas and Louisiana.")
add_db("Producing areas from Galveston Island, eastward, through Main Pass are represented.")
add_db("Many Gulf of Mexico crudes can be characterized as naphthenic or biodegraded oils.")
add_parameter("bobp", "RB_STB", "Formation volume factor at pb")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.5781,  0.8519)
add_parameter("yo", "Sg_do", "Oil specific gravity",  "Quantity<Api>(45)",  "Quantity<Api>(16.3)")
add_synonym("yo", "api", "Api")
add_parameter("rsb",  "SCF_STB", "Solution GOR at Pb")
add_parameter("t", "Fahrenheit", "Temperature",  114,  288)
add_parameter("p",  "psia", "Pressure")
add_parameter("pb", "psia", "Bubble point pressure",  1574,  6523)
add_author("Petrosky & Farshad")
add_ref("petrosky:1993")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoaVasquezBeggs", "OilFormationVolumeFactor", "RB_STB")
add_title("VAZQUEZ & BEGGS CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Based on more than 600 laboratory PVT analyses from fields all over the world.")
add_parameter("bobp", "RB_STB", "Formation volume factor at pb")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.511,  1.35)
add_parameter("api",  "Api", "API oil gravity",  15.3,  59.3)
add_parameter("rsb",  "SCF_STB", "Solution GOR at Pb")
add_parameter("t", "Fahrenheit", "Temperature",  75,  294)
add_parameter("tsep", "Fahrenheit", "Separator temperature",  76,  150)
add_parameter("p",  "psia", "Pressure")
add_parameter("pb",  "psia", "Bubble point pressure")
add_parameter("psep", "psia", "Separator pressure",  60,  565)
add_author("Vazquez & Beggs")
add_ref("vazquez:1980")
add_ref("alShammasi:2001")
add_ref("banzer:1996")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoaPerezML", "OilFormationVolumeFactor", "RB_STB")
add_title("PÉREZ, HENY & LAGO CORRELATION (EZEKWE-QUERIN-HUMPHREY CORRELATION USED BY PÉREZ, HENY & LAGO), CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_note("The correlation could not be verified because the original reference is not available. Date: August 22 2016.")
add_parameter("bobp", "RB_STB", "Formation volume factor at pb")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.571,  0.981)
add_parameter("yo", "Sg_do", "Oil specific gravity",  "Quantity<Api>(12.9)",  "Quantity<Api>(6.4)")
add_synonym("yo", "api", "Api")
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  38,  121)
add_parameter("t", "Fahrenheit", "Temperature",  112,  300)
add_parameter("p",  "psia", "Pressure")
add_parameter("pb", "psia", "Bubble point pressure",  405,  1335)
add_author("Pérez, Heny & Lago")
add_ref("perez:2001")
add_internal_note("The correlation could not be verified because the original reference is not available. Date: August 22 2016.")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoaMillanArcia", "OilFormationVolumeFactor", "RB_STB")
add_title("MILLÁN-ARCIA CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Venezuelan heavy crudes were correlated.")
add_parameter("bobp", "RB_STB", "Formation volume factor at pb")
add_parameter("api",  "Api", "API oil gravity",  9,  20.2)
add_parameter("rs", "SCF_STB", "Solution GOR",  53,  459)
add_parameter("rsb",  "SCF_STB", "Solution GOR at Pb")
add_parameter("t", "Fahrenheit", "Temperature",  87,  195)
add_parameter("p",  "psia", "Pressure")
add_parameter("pb", "psia", "Bubble point pressure",  222.0,  3432.7)
add_author("Millán-Arcia")
add_ref("millan:1984")
add_ref("perez:2001")
add_internal_note("The correlation could not be verified because the original reference is not available. Date: August 22 2016.")
end_correlation()


