# coding: utf-8

## verificada con python
begin_correlation("UoaKartoatmodjoSchmidt", "UndersaturatedOilViscosity", "CP",
                 0.168, 517.03)
add_title("KARTOATMODJO & SCHMIDT CORRELATION FOR HEAVY, MEDIUM AND LIGHT OILS, CALCULATION OF UNDERSATURATED OIL VISCOSITY")
add_db("Based on a total of 3588 data points collected from 661 different crude oil samples.")
add_db("The first major source was from South East Asia, mainly Indonesia. The second source was North America, including the offshore area. The rest came from the Middle East and Latin America.")
add_internal_note("Depending on the API gravity, the samples can cover three different classes of crude oils: heavy oils for 10<°API<=22.3, medium oils for 22.3<°API<=31.1, and light oils for °API>31.1.")
add_parameter("uob", "CP", "Saturated oil viscosity at Pb",  0.168,  184.86)
add_parameter("pb", "psia", "Bubble point pressure",  24.7,  4764.7)
add_parameter("p", "psia", "Pressure",  24.7,  6014.7)
add_author("Kartoatmodjo & Schmidt")
add_ref("kartoatmodjo:1991")
end_correlation()

################################################################        

## verificada con python
begin_correlation("UoaDeGhettoEtAl", "UndersaturatedOilViscosity",
                  "CP", 2.4, 354.6)
add_title("DE GHETTO, PAONE & VILLA CORRELATION FOR EXTRA-HEAVY (MODIFIED LABEDI CORRELATION) AND HEAVY (MODIFIED KARTOATMODJO & SCHMIDT CORRELATION) OILS, CALCULATION OF UNDERSATURATED OIL VISCOSITY")
add_db("Based on 1200 measured data points of 63 heavy and extra-heavy oil samples obtained from the Mediterranean Basin, Africa and the Persian Gulf.")
add_db("Oil samples have been divided in two different API gravity classes: extra-heavy oils for °API<=10, heavy oils for 10<°API<=22.3.")
add_parameter("uob", "CP", "Saturated oil viscosity at Pb",  2.1,  295.9)
add_parameter("p", "psia", "Pressure",  1038.49,  7411.54)
add_parameter("pb", "psia", "Bubble point pressure",  208.86,  4021.96)
add_parameter("uod", "CP", "Dead oil viscosity",  7.7,  1386.9)
add_parameter("api", "Api", "API oil gravity",  6,  22.3)
add_author("De Ghetto, Paone & Villa")
add_ref("deGhetto:1995")
end_correlation()

################################################################

## verificada con python
begin_correlation("UoaBeal", "UndersaturatedOilViscosity", "CP", 0.16, 315)
add_title("BEAL CORRELATION, CALCULATION OF UNDERSATURATED OIL VISCOSITY")
add_db("Based on 52 viscosity observations taken from 26 crude oil samples, representing 20 individual oil pools, 11 of which are in California.")
add_db("Half of the observations include viscosity data for the undersaturated crude oil above the bubble point, and the other half are viscosity observations at the bubble point.")
add_internal_note("Beal developed the graphical correlation and Standing presented the equation")
add_parameter("uob", "CP", "Saturated oil viscosity at Pb",  0.142,  127)
add_parameter("p", "psia", "Pressure",  "Quantity<psig>(1500)",  "Quantity<psig>(5500)")
add_parameter("pb", "psia", "Bubble point pressure",  "Quantity<psig>(125)",  "Quantity<psig>(4120)")
add_author("Beal")
add_ref("beal:1946")
add_ref("standing:1977")
add_ref("banzer:1996")
end_correlation()

################################################################

## verificada con python
begin_correlation("UoaVasquezBeggs", "UndersaturatedOilViscosity", "CP",  0.117,  148)
add_title("VAZQUEZ & BEGGS CORRELATION, CALCULATION OF UNDERSATURATED OIL VISCOSITY")
add_db("Based on more than 600 laboratory PVT analyses from fields all over the world, including more than 6000 measurements of undersaturated oil viscosity at various pressures.")
add_internal_note("The development ranges are presented by Bánzer, who does not specify if the range for viscosity corresponds to uoBubble or uoa.")
add_parameter("uob", "CP", "Saturated oil viscosity at Pb")
add_parameter("p", "psia", "Pressure",  141,  9515)
add_parameter("pb",  "psia", "Bubble point pressure")
add_author("Vazquez & Beggs")
add_ref("vazquez:1980")
add_ref("banzer:1996")
end_correlation()

################################################################

## verificada con python
begin_correlation("UoaKhan", "UndersaturatedOilViscosity", "CP", 0.13, 71.0)
add_title("KHAN ET AL. CORRELATION, CALCULATION OF UNDERSATURATED OIL VISCOSITY")
add_db("Based on 75 bottom hole samples taken from 62 Saudi Arabian oil reservoirs.")
add_db("A total of 1503 data points were used for oil viscosity above the bubble point pressure.")
add_parameter("uob", "CP", "Saturated oil viscosity at Pb",  0.13,  17.9)
add_parameter("p", "psia", "Pressure",  14.7,  5015)
add_parameter("pb", "psia", "Bubble point pressure",  107,  4315)
add_author("Khan et al.")
add_ref("khan:1987")
end_correlation()

################################################################

## verificada con python
begin_correlation("UoaPetroskyFarshad", "UndersaturatedOilViscosity", "CP",
                  0.224, 4.090)
add_title("PETROSKY & FARSHAD CORRELATION, CALCULATION OF UNDERSATURATED OIL VISCOSITY")
add_db("Based on a set of 404 data points from 126 laboratory PVT analyses of Gulf of Mexico crude oils. For the ranges of carbon dioxide and nitrogen concentration in the mixture, 126 data points were considered.")
add_db("Fluid samples were obtained from reservoirs located offshore Texas and Louisiana. Producing areas from Galveston Island, eastward, through Main Pass are represented.")
add_parameter("uob", "CP", "Saturated oil viscosity at Pb",  0.211,  3.546)
add_parameter("p", "psia", "Pressure",  1600,  10250)
add_parameter("pb", "psia", "Bubble point pressure",  1574,  9552)
add_author("Petrosky & Farshad")
add_ref("petrosky:1995")
end_correlation()

################################################################

## verificada con python
begin_correlation("UoaAbedini", "UndersaturatedOilViscosity", "CP", 0.316, 18.435)
add_title("ABEDINI, ABEDINI & ESLAMI CORRELATION, CALCULATION OF UNDERSATURATED OIL VISCOSITY")
add_db("Based on PVT experimental data of five sample oils from Iranian oil reservoirs.")
add_parameter("uob",  "CP", "Saturated oil viscosity at Pb")
add_parameter("p", "psia", "Pressure")
add_parameter("pb", "psia", "Bubble point pressure",  1340,  4513)
add_author("Abedini, Abedini & Eslami")
add_ref("abedini:2010")
end_correlation()

################################################################

begin_correlation("UoaPerezML", "UndersaturatedOilViscosity", "CP")
add_title("PÉREZ, HENY & LAGO CORRELATION (MODIFIED LABEDI CORRELATION), CALCULATION OF UNDERSATURATED OIL VISCOSITY")
add_parameter("uob",  "CP", "Saturated oil viscosity at Pb")
add_parameter("p",  "psia", "Pressure")
add_parameter("pb", "psia", "Bubble point pressure",  405,  1335)
add_author("Pérez, Heny & Lago")
add_ref("perez:2001")
add_internal_note("The correlation could not be verified because the original reference is not available. Date: September 8 2016.")
end_correlation()

################################################################

begin_correlation("UoaDindorukChristman", "UndersaturatedOilViscosity", "CP",
                  0.211, 10.6)
add_title("DINDORUK & CHRISTMAN CORRELATION, CALCULATION OF UNDERSATURATED OIL VISCOSITY")
add_db("Based on more than 90 PVT reports from the Gulf of Mexico.")
add_parameter("uob", "CP", "Saturated oil viscosity at Pb",  0.161,  8.7)
add_parameter("p", "psia", "Pressure",  1128,  22370)
add_parameter("pb", "psia", "Bubble point pressure",  926,  12230)
add_parameter("rs",  "SCF_STB", "Solution GOR")
add_author("Dindoruk & Christman")
add_ref("dindoruk:2004")
end_correlation()
