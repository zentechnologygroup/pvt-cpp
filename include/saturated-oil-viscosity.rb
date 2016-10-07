# coding: utf-8

# coding: utf-8

## verificada con python
begin_correlation("UobBeggsRobinson", "SaturatedOilViscosity", "CP")
add_title("BEGGS & ROBINSON CORRELATION, CALCULATION OF SATURATED OIL VISCOSITY")
add_db("Based on 2073 live oil observations. Beggs and Robinson developed correlations for dead and live oil viscosities with samples obtained from 600 oil systems.")
add_parameter("uod",  "CP", "Aqui Neylith")
add_parameter("rs", "SCF_STB", "Aqui Neylith",  20,  2070)
add_author("Beggs & Robinson")
end_correlation()

################################################################        

## verificada con python
begin_correlation("UobChewConnally", "SaturatedOilViscosity", "CP", 0.1, 100)
add_title("CHEW & CONNALLY CORRELATION, CALCULATION OF SATURATED OIL VISCOSITY")
add_db("Based on 457 crude oil samples obtained from all the important producing areas of the U.S., as well as from Canada and South America.")
add_ref("banzer:1996")
add_note("Chew & Connally presented the graphical correlation and the general form of the equation. The values of A and b of the equation are presented by Bánzer (1996) and Ahmed (2010).")
add_parameter("uod", "CP", "Aqui Neylith",  0.377,  100)
add_parameter("rs", "SCF_STB", "Aqui Neylith",  0,  1600)
add_author("Chew & Connally")
end_correlation()

################################################################

## verificada con python
begin_correlation("UobKhan", "SaturatedOilViscosity", "CP", 0.13, 77.4)
add_title("KHAN ET AL. CORRELATION, CALCULATION OF SATURATED OIL VISCOSITY")
add_db("Based on 75 bottom hole samples taken from 62 Saudi Arabian oil reservoirs. A total of 150 data points were used for bubble point oil viscosity in 1691 for oil viscosity below the bubble point pressure.")
add_parameter("rsb", "SCF_STB", "Aqui Neylith",  24,  1901)
add_parameter("api", "Api", "Aqui Neylith",  14.3,  44.6)
add_parameter("yg", "Sgg", "Aqui Neylith",  0.752,  1.367)
add_parameter("t", "Fahrenheit", "Aqui Neylith",  75,  240)
add_parameter("p" , "psia", "Aqui Neylith",  75,  240)
add_parameter("pb" , "psia", "Aqui Neylith",  107,  4315)
add_author("Khan")
end_correlation()

################################################################

## verificada con python
begin_correlation("UobKartoatmodjoSchmidt", "SaturatedOilViscosity",
                  "CP", 0.097, 586)
add_title("KARTOATMODJO & SCHMIDT CORRELATION FOR HEAVY, MEDIUM AND LIGHT OILS, CALCULATION OF SATURATED OIL VISCOSITY")
add_db("Based on 5321 data points of heavy, medium and light oil samples. The data bank was collected from PVT reports and literature.")
add_db("The first major source was from South East Asia, mainly Indonesia. The second source was North America, including the offshore area. The rest came from the Middle East and Latin America.")
add_db("Depending on the API gravity, the samples can cover three different classes of crude oils: heavy oils for 10<°API<=22.3, medium oils for 22.3<°API<=31.1, and light oils for °API>31.1.")
add_parameter("uod", "CP", "Aqui Neylith",  0.506,  682.0)
add_parameter("rs", "SCF_STB", "Aqui Neylith",  0,  2890)
add_author("Kartoatmodjo & Schmidt")
end_correlation()

################################################################

## verificada con python
begin_correlation("UobPetroskyFarshad", "SaturatedOilViscosity", "CP", 
                  0.211, 7.403)
add_title("PETROSKY & FARSHAD CORRELATION, CALCULATION OF SATURATED OIL VISCOSITY")
add_db("Based on a set of 864 data points from 126 laboratory PVT analyses of Gulf of Mexico crude oils. For the ranges of carbon dioxide and nitrogen concentration in the mixture, 88 data points were considered.")
add_db("Fluid samples were obtained from reservoirs located offshore Texas and Louisiana. Producing areas from Galveston Island, eastward, through Main Pass are represented.")
add_parameter("uod", "CP", "Aqui Neylith",  0.725,  11.69)
add_parameter("rs", "SCF_STB", "Aqui Neylith",  21,  1885)
add_author("Petrosky & Farshad")
end_correlation()


################################################################

## verificada con python
begin_correlation("UobPerezML", "SaturatedOilViscosity", "CP")
add_title("PÉREZ ET AL. CORRELATION, CALCULATION OF SATURATED OIL VISCOSITY")
add_parameter("uod",  "CP", "Aqui Neylith")
add_parameter("rs", "SCF_STB", "Aqui Neylith",  38,  121)
add_author("Pérez et al.")
end_correlation()


################################################################

## verificada con python
begin_correlation("UobGilFonseca", "SaturatedOilViscosity", "CP")
add_title("GIL-FONSECA CORRELATION, CALCULATION OF SATURATED OIL VISCOSITY")
add_parameter("uod",  "CP", "Aqui Neylith")
add_parameter("rs", "SCF_STB", "Aqui Neylith",  )
add_author("Gil Fonseca")
end_correlation()


################################################################

## verificada con python
begin_correlation("UobDeGhettoEtAl", "SaturatedOilViscosity", "CP", 2.1, 295.9)
add_title("DE GHETTO ET AL. CORRELATION FOR EXTRA-HEAVY AND HEAVY OILS (MODIFIED KARTOATMODJO CORRELATION), CALCULATION OF SATURATED OIL VISCOSITY")
add_db("Based on 1200 measured data points of 63 heavy and extra-heavy oil samples obtained from the Mediterranean Basin, Africa and the Persian Gulf.")
add_db("Oil samples have been divided in two different API gravity classes: extra-heavy oils for °API<=10, heavy oils for 10<°API<=22.3.")
add_parameter("uod", "CP", "Aqui Neylith",  7.7,  1386.9)
add_parameter("rs", "SCF_STB", "Aqui Neylith",  17.21,  640.25)
add_parameter("api", "Api", "Aqui Neylith",  6,  22.3)
add_author("De Ghetto et al.")
end_correlation()

################################################################

begin_correlation("UobDindorukChristman", "SaturatedOilViscosity", "CP", 0.161, 8.7)
add_title("DINDORUK & CHRISTMAN CORRELATION, CALCULATION OF SATURATED OIL VISCOSITY")
add_db("Based on more than 90 PVT reports from the Gulf of Mexico.")
add_parameter("uod", "CP", "Aqui Neylith",  0.896,  62.63)
add_parameter("rs",  "SCF_STB", "Aqui Neylith")
add_author("Dindoruk & Christman")
end_correlation()
