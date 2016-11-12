# coding: utf-8

# coding: utf-8

## verificada con python
begin_correlation("UobBeggsRobinson", "SaturatedOilViscosity", "CP")
add_title("BEGGS & ROBINSON CORRELATION, CALCULATION OF SATURATED OIL VISCOSITY")
add_db("Based on 2073 live oil observations. Beggs & Robinson developed correlations for dead and live oil viscosities with samples obtained from 600 oil systems.")
add_parameter("uod",  "CP", "Dead oil viscosity")
add_parameter("rs", "SCF_STB", "Solution GOR",  20,  2070)
add_author("Beggs & Robinson")
add_ref("beggs:1975")
end_correlation()

################################################################        

## verificada con python
begin_correlation("UobChewConnally", "SaturatedOilViscosity", "CP", 0.1, 100)
add_title("CHEW & CONNALLY CORRELATION, CALCULATION OF SATURATED OIL VISCOSITY")
add_db("Based on 457 crude oil samples obtained from all the important producing areas of the U.S., as well as from Canada and South America.")
add_note("Chew & Connally presented the graphical correlation and the general form of the equation.")
add_internal_note("The values of A and b of the equation are presented by Bánzer (1996) and Ahmed (2010).")
add_parameter("uod", "CP", "Dead oil viscosity",  0.377,  100)
add_parameter("rs", "SCF_STB", "Solution GOR",  0,  1600)
add_author("Chew & Connally")
add_ref("chew:1959")
add_ref("banzer:1996")
add_ref("ahmed:2010")
end_correlation()

################################################################

## verificada con python
begin_correlation("UobKhan", "SaturatedOilViscosity", "CP", 0.13, 77.4)
add_title("KHAN ET AL. CORRELATION, CALCULATION OF SATURATED OIL VISCOSITY")
add_db("Based on 75 bottom hole samples taken from 62 Saudi Arabian oil reservoirs. A total of 150 data points were used for bubble point oil viscosity in 1691 for oil viscosity below the bubble point pressure.")
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  24,  1901)
add_parameter("yo", "Sg_do", "Oil specific gravity",  "Quantity<Api>(44.6)",  "Quantity<Api>(14.3)")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.752,  1.367)
add_parameter("t", "Fahrenheit", "Temperature",  75,  240)
add_parameter("p" , "psia", "Pressure",  14.7,  5015)
add_parameter("pb" , "psia", "Bubble point pressure",  107,  4315)
add_synonym("yo", "api", "api")
add_author("Khan et al.")
add_ref("khan:1987")
end_correlation()

################################################################

## verificada con python
begin_correlation("UobKartoatmodjoSchmidt", "SaturatedOilViscosity",
                  "CP", 0.097, 586)
add_title("KARTOATMODJO & SCHMIDT CORRELATION FOR HEAVY, MEDIUM AND LIGHT OILS, CALCULATION OF SATURATED OIL VISCOSITY")
add_db("Based on 5321 data points of heavy, medium and light oil samples. The data bank was collected from PVT reports and literature.")
add_db("The first major source was from South East Asia, mainly Indonesia. The second source was North America, including the offshore area. The rest came from the Middle East and Latin America.")
add_internal_note("Depending on the API gravity, the samples can cover three different classes of crude oils: heavy oils for 10<°API<=22.3, medium oils for 22.3<°API<=31.1, and light oils for °API>31.1.")
add_parameter("uod", "CP", "Dead oil viscosity",  0.506,  682.0)
add_parameter("rs", "SCF_STB", "Solution GOR",  0,  2890)
add_author("Kartoatmodjo & Schmidt")
add_ref("kartoatmodjo:1991")
end_correlation()

################################################################

## verificada con python
begin_correlation("UobPetroskyFarshad", "SaturatedOilViscosity", "CP", 
                  0.211, 7.403)
add_title("PETROSKY & FARSHAD CORRELATION, CALCULATION OF SATURATED OIL VISCOSITY")
add_db("Based on a set of 864 data points from 126 laboratory PVT analyses of Gulf of Mexico crude oils. For the ranges of carbon dioxide and nitrogen concentration in the mixture, 88 data points were considered.")
add_db("Fluid samples were obtained from reservoirs located offshore Texas and Louisiana. Producing areas from Galveston Island, eastward, through Main Pass are represented.")
add_parameter("uod", "CP", "Dead oil viscosity",  0.725,  11.69)
add_parameter("rs", "SCF_STB", "Solution GOR",  21,  1885)
add_author("Petrosky & Farshad")
add_ref("petrosky:1995")
end_correlation()


################################################################

## verificada con python
begin_correlation("UobPerezML", "SaturatedOilViscosity", "CP")
add_title("PÉREZ, HENY & LAGO CORRELATION, CALCULATION OF SATURATED OIL VISCOSITY")
add_parameter("uod",  "CP", "Dead oil viscosity")
add_parameter("rs", "SCF_STB", "Solution GOR",  38,  121)
add_author("Pérez, Heny & Lago")
add_ref("perez:2001")
add_internal_note("The correlation could not be verified because the original reference is not available. Date: September 6 2016.")
set_hidden()
end_correlation()


################################################################

## verificada con python
begin_correlation("UobGilFonseca", "SaturatedOilViscosity", "CP")
add_title("GIL & FONSECA CORRELATION, CALCULATION OF SATURATED OIL VISCOSITY")
add_parameter("uod",  "CP", "Dead oil viscosity")
add_parameter("rs", "SCF_STB", "Solution GOR")
add_author("Gil & Fonseca")
add_ref("perez:2001")
add_internal_note("The correlation could not be verified because the original and secondary references are not available. Date: September 6 2016.")
set_hidden()
end_correlation()


################################################################

## verificada con python
begin_correlation("UobDeGhettoEtAl", "SaturatedOilViscosity", "CP", 2.1, 295.9)
add_title("DE GHETTO, PAONE & VILLA CORRELATION FOR EXTRA-HEAVY AND HEAVY OILS (MODIFIED KARTOATMODJO CORRELATION), CALCULATION OF SATURATED OIL VISCOSITY")
add_db("Based on 1200 measured data points of 63 heavy and extra-heavy oil samples obtained from the Mediterranean Basin, Africa and the Persian Gulf.")
add_db("Oil samples have been divided in two different API gravity classes: extra-heavy oils for °API<=10, heavy oils for 10<°API<=22.3.")
add_parameter("uod", "CP", "Dead oil viscosity",  7.7,  1386.9)
add_parameter("rs", "SCF_STB", "Solution GOR",  17.21,  640.25)
add_parameter("api", "Api", "API oil gravity",  6,  22.3)
add_author("De Ghetto, Paone & Villa")
add_ref("deGhetto:1995")
end_correlation()

################################################################

begin_correlation("UobDindorukChristman", "SaturatedOilViscosity", "CP", 0.161, 8.7)
add_title("DINDORUK & CHRISTMAN CORRELATION, CALCULATION OF SATURATED OIL VISCOSITY")
add_db("Based on more than 90 PVT reports from the Gulf of Mexico.")
add_parameter("uod", "CP", "Dead oil viscosity",  0.896,  62.63)
add_parameter("rs",  "SCF_STB", "Solution GOR", 133, 3050)
add_author("Dindoruk & Christman")
add_ref("dindoruk:2004")
add_internal_note("The range assigned for Rs was taken from the range expressed by the author for Rsb.")
end_correlation()
