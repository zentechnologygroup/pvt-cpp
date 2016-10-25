# coding: utf-8

# verificada con python!
begin_correlation("CoMcCainEtAl", "OilCompressibility", "psia_1",
                  31e-6, 6600e-6)
add_title("McCAIN, ROLLINS & VILLENA CORRELATION FOR SATURATED OILS (PRESSURES BELOW THE BUBBLE POINT), CALCULATION OF OIL ISOTHERMAL COMPRESSIBILITY")
add_db("Based on 2500 experimental data obtained from different crude oils.")
add_db("The equation was developed with data from reservoir-fluid studies of 260 oil fields worldwide.")
add_parameter("api", "Api", "API oil gravity",  18,  52)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  15,  1947)
add_parameter("t", "Rankine", "Temperature",  "Quantity<Fahrenheit>(78)",  "Quantity<Fahrenheit>(330)") 
add_parameter("p", "psia", "Pressure",  500,  5300)
add_parameter("pb", "psia", "Bubble point pressure",  763,  5300)
add_author("McCain, Rollins & Villena")
add_ref("mcCain:1988")
add_ref("banzer:1996")
end_correlation()

################################################################        

# verificada con python!
begin_correlation("CoDeGhetto", "OilCompressibility", "psia_1", 3.02e-6, 42.9e-6)
add_title("DE GHETTO, PAONE & VILLA CORRELATION FOR EXTRA-HEAVY AND HEAVY OILS (MODIFIED VASQUEZ & BEGGS CORRELATION), CALCULATION OF OIL ISOTHERMAL COMPRESSIBILITY")
add_db("Based on 1200 measured data points of 63 heavy and extra-heavy oil samples obtained from the Mediterranean Basin, Africa and the Persian Gulf.")
add_db("Oil samples have been divided in two different API gravity classes: extra-heavy oils for °API<=10, heavy oils for 10<°API<=22.3.")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.623,  1.517)
add_parameter("api", "Api", "API oil gravity",  6,  22.3)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  17.21,  640.25)
add_parameter("t", "Fahrenheit", "Temperature",  131.4,  250.7)
add_parameter("tsep", "Fahrenheit", "Separator temperature",  59,  177.8)
add_parameter("p", "psia", "Pressure",  1038.49,  7411.54)
add_parameter("pb", "psia", "Bubble point pressure",  208.86,  4021.96)
add_parameter("psep", "psia", "Separator pressure",  14.5,  752.2)
add_author("De Ghetto, Paone & Villa")
add_ref("deGhetto:1995")
add_internal_note("The range for Rsb is presented by De Ghetto et al. as the range for Solution GOR.")
end_correlation()

################################################################        

# verificada con python!
begin_correlation("CoHanafy", "OilCompressibility", "psia_1",
                  4.46e-6, 62.68e-6)
add_title("HANAFY ET AL. CORRELATION, CALCULATION OF OIL ISOTHERMAL COMPRESSIBILITY")
add_db("Based on experimental PVT data of 324 fluid samples taken from 176 wells located in 75 fields. This data represents 15 productive zones of 123 reservoirs distributed along three different regions of Egypt, including the Gulf of Suez, Western Desert, and Sinai.")
add_parameter("api", "Api", "API oil gravity",  17.8,  47.7)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  7,  4272)
add_parameter("t", "Fahrenheit", "Temperature",  107,  327)
add_parameter("p",  "psia", "Pressure")
add_parameter("pb", "psia", "Bubble point pressure",  36,  5003)
add_author("Hanafy et al.")
add_ref("hanafy:1997")
add_internal_note("The range for Rsb is presented by Hanafy et al. as the range for Rs.")
end_correlation()


################################################################        

# verificada con python!
begin_correlation("CoKartoatmodjoSchmidt", "OilCompressibility", "psia_1")
add_title("KARTOATMODJO & SCHMIDT CORRELATION, CALCULATION OF OIL ISOTHERMAL COMPRESSIBILITY")
add_db("Based on a total of 3588 data points collected from 661 different crude oil samples.")
add_db("The data bank was collected from PVT reports and literature. The first major source was from South East Asia, mainly Indonesia. The second source was North America, including the offshore area. The rest came from the Middle East and Latin America.")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.4824,  1.668)
add_parameter("api", "Api", "API oil gravity",  14.4,  59)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  0,  2890)
add_parameter("t", "Fahrenheit", "Temperature",  75,  320)
add_parameter("tsep", "Fahrenheit", "Separator temperature",  38,  294)
add_parameter("p", "psia", "Pressure",  24.7,  6014.7)
add_parameter("pb", "psia", "Bubble point pressure",  24.7,  4764.7)
add_parameter("psep", "psia", "Separator pressure",  14.7,  1414.7)
add_author("Kartoatmodjo & Schmidt")
add_ref("kartoatmodjo:1991")
add_internal_note("The range for Rsb is presented by Kartoatmodjo and Schmidt as the range for Rsf (flash gas-oil ratio expressed as cubic feet of separator and stock tank gas at any pressure less than the bubble point pressure in one stock tank barrel oil).")
end_correlation()

################################################################

# verificada con python!
begin_correlation("CoPetroskyFarshad", "OilCompressibility", "psia_1",
                  3.507e-6, 24.64e-6)
add_title("PETROSKY & FARSHAD CORRELATION, CALCULATION OF OIL ISOTHERMAL COMPRESSIBILITY")
add_db("Based on 81 laboratory PVT analyses, using a total of 304 data points. Fluid samples were obtained from reservoirs located offshore Texas and Louisiana.")
add_db("Producing areas from Galveston Island, eastward, through Main Pass are represented.")
add_db("Many Gulf of Mexico crudes can be characterized as naphthenic or biodegraded oils.")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.5781,  0.8519)
add_parameter("api", "Api", "API oil gravity",  16.3,  45)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  217,  1406)
add_parameter("t", "Fahrenheit", "Temperature",  114,  288)
add_parameter("p", "psia", "Pressure",  1700,  10692)
add_parameter("pb", "psia", "Bubble point pressure",  1574,  6523)
add_author("Petrosky & Farshad")
add_ref("petrosky:1993")
add_internal_note("The range for Rsb is presented by Petrosky & Farshad as the range for solution GOR (Rs).")
end_correlation()

################################################################

# verificada con python!
begin_correlation("CoVasquezBeggs", "OilCompressibility", "psia_1")
add_title("VAZQUEZ & BEGGS CORRELATION, CALCULATION OF OIL ISOTHERMAL COMPRESSIBILITY")
add_db("Based on more than 600 laboratory PVT analyses from fields all over the world.")
add_db("The correlation was developed using a total of 4036 data points.")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.511,  1.351)
add_parameter("api", "Api", "API oil gravity",  15.3,  59.5)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  9.3,  2199)
add_parameter("t",  "Fahrenheit", "Temperature")
add_parameter("tsep", "Fahrenheit", "Separator temperature",  76,  150)
add_parameter("p", "psia", "Pressure",  141,  9515)
add_parameter("pb",  "psia", "Bubble point pressure")
add_parameter("psep", "psia", "Separator pressure",  60,  565)
add_author("Vazquez & Beggs")
add_ref("vazquez:1980")
add_ref("banzer:1996")
add_internal_note("The development ranges are presented by Bánzer (1996).")
end_correlation()

################################################################

# verificada con python!
begin_correlation("CoPerezML", "OilCompressibility", "psia_1")
add_title("PÉREZ, HENY & LAGO CORRELATION (MODIFIED PETROSKY & FARSHAD CORRELATION), CALCULATION OF OIL ISOTHERMAL COMPRESSIBILITY")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.571,  0.981)
add_parameter("api", "Api", "API oil gravity",  6.4,  12.9)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  38,  121)
add_parameter("t", "Fahrenheit", "Temperature",  112,  300)
add_parameter("p",  "psia", "Pressure")
add_parameter("pb", "psia", "Bubble point pressure",  405,  1335)
add_author("Pérez, Heny & Lago")
add_ref("perez:2001")
add_internal_note("The correlation could not be verified because the original reference is not available. Date: August 29 2016.")
end_correlation()

################################################################

# verificada con python!
begin_correlation("CoMillanArcia", "OilCompressibility", "psia_1")
add_title("MILLÁN-ARCIA CORRELATION, CALCULATION OF OIL ISOTHERMAL COMPRESSIBILITY")
add_db("Venezuelan heavy crudes were correlated.")
add_parameter("api", "Api", "API oil gravity",  9.0,  20.2)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  53,  459)
add_parameter("t", "Fahrenheit", "Temperature",  87,  195)
add_parameter("p", "psia", "Pressure")
add_parameter("pb", "psia", "Bubble point pressure",  222.0,  3432.7)
add_author("Millán-Arcia")
add_ref("millan:1984")
add_ref("perez:2001")
add_internal_note("The correlation could not be verified because the original reference is not available. Date: August 29 2016.")
end_correlation()
