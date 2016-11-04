# coding: utf-8

## verificada con python
begin_correlation("UodBeal", "DeadOilViscosity", "CP", 0.865, 1550)
add_title("BEAL CORRELATION, CALCULATION OF DEAD OIL VISCOSITY")
add_db("Based on 98 samples of gas-free crude oil measured at temperatures above 100 °F and 655 samples measured at 100 °F obtained from 492 different oil fields, of which 358 are in the United States.")
add_ref("beal:1946")
add_ref("standing:1977")
add_ref("banzer:1996")
add_note("Beal (1946) presented a graphical correlation showing the effects of both oil gravity and temperature on dead-oil viscosity. The equation is presented by Standing (1977).")
add_eq("\\mu_{od}\\,=\\,{\\left[{0.32}+{\\frac{1.8\\cdot{10^{7}}}{{\\gamma_{API}}^{4.53}}} \\right]}\\cdot{{\\left[\\frac{360}{T+200}\\right]}^{a}}")
add_eq("a\\,=\\,antilog{\\left({0.43}+{\\frac{8.33}{\\gamma_{API}}}\\right)}")
add_parameter("api", "Api", "API oil gravity",  10.1,  52.5)
add_parameter("t", "Fahrenheit", "Temperature",  100,  220)
add_author("Beal")
end_correlation()

################################################################

## verificada con python
begin_correlation("UodBeggsRobinson", "DeadOilViscosity", "CP")
add_title("BEGGS & ROBINSON CORRELATION, CALCULATION OF DEAD OIL VISCOSITY")
add_db("Based on 460 dead oil observations. Beggs & Robinson (1975) developed correlations for dead and live oil viscosities with samples obtained from 600 oil systems.")
add_parameter("api", "Api", "API oil gravity",  16,  58)
add_parameter("t", "Fahrenheit", "Temperature",  70,  295)
add_author("Beggs & Robinson")
add_ref("beggs:1975")
end_correlation()

################################################################

## verificada con python
begin_correlation("UodEgbogahNg", "DeadOilViscosity", "CP")
add_title("NG & EGBOGAH CORRELATION (MODIFIED BEGGS & ROBINSON CORRELATION), CALCULATION OF DEAD OIL VISCOSITY")
add_db("Based on viscosity data from the Reservoir Fluids Analysis Laboratory of AGAT Engineering Utd. The total number of oil systems used is 394.")
add_parameter("api", "Api", "API oil gravity",  5,  58)
add_parameter("t", "Fahrenheit", "Temperature", "Quantity<Celsius>(15)", "Quantity<Celsius>(80)")
add_author("Ng & Egbogah")
add_ref("ng:1983")
end_correlation()

################################################################

## verificada con python
begin_correlation("UodGlaso", "DeadOilViscosity", "CP", 0.2, 80)
add_title("GLASO CORRELATION, CALCULATION OF DEAD OIL VISCOSITY")
add_db("Based on 26 samples from the North Sea (collected from wells in the region 56 to 62°N).")
add_db("The relation between dead oil viscosity, API gravity, and temperature, was developed specifically for North Sea oils (for oils with a paraffinicity characterization factor of 11.9), which have a factor almost equal to California oils, as well as for oil samples from fields in various regions of the world.")
add_parameter("api", "Api", "API oil gravity",  20,  48)
add_parameter("t", "Fahrenheit", "Temperature",  50,  300)
add_author("Glaso")
add_ref("glaso:1980")
add_internal_note("The ranges are taken from the curves presented by Glaso.")
end_correlation()

################################################################

## verificada con python
begin_correlation("UodKartoatmodjoSchmidt", "DeadOilViscosity", "CP", 0.506, 682)
add_title("KARTOATMODJO & SCHMIDT CORRELATION, CALCULATION OF DEAD OIL VISCOSITY")
add_db("Based on a total of 661 different crude oil samples. The data bank was collected from PVT reports and literature.")
add_db("The first major source was from South East Asia, mainly Indonesia. The second source was North America, including the offshore area. The rest came from the Middle East and Latin America.")
add_parameter("api", "Api", "API oil gravity",   14.4 ,  59)
add_parameter("t", "Fahrenheit", "Temperature",  80,  320)
add_author("Kartoatmodjo & Schmidt")
add_ref("kartoatmodjo:1991")
end_correlation()

################################################################

begin_correlation("UodSattarinEtAl", "DeadOilViscosity", "CP", 2, 570)
add_title("SATTARIN ET AL. CORRELATION, CALCULATION OF DEAD OIL VISCOSITY")
add_db("Based on 438 data points from Iranian off and on-shore dead crude oil samples, collected in Crude Oil Evaluation department at RIPI. Standard test method of ASTM D-445 was used for kinematic viscosity measurements.")
add_db("Data were divided into two major groups, depending on the API gravity. The first group included 85 Iranian heavy crude oil (17<=°API<28) data-points and the second one contained 353 Iranian light crude oil (28<=°API<45) data-points.")
add_internal_note("Sattarin confirmed that the correlation has an error in the published article. In order to correct it, -5.9836 was replaced by -5.8936. Furthermore, all the decimals indicated by Sattarin were used, whereas in the article the numbers were rounded.")
add_internal_note("All crude oil viscosities were measured at temperatures 10, 20, and 40 °C.")
add_parameter("api", "Api", "API oil gravity",  17,  45)
add_parameter("t", "Kelvin", "Temperature",  "Quantity<Celsius>(10)",  "Quantity<Celsius>(40)")
add_author("Sattarin et al.")
add_ref("sattarin:2007")
end_correlation()

################################################################

## verificada con python
begin_correlation("UodNaseri", "DeadOilViscosity", "CP", 0.75, 54)
add_title("NASERI, NIKAZAR & MOUSAVI CORRELATION, CALCULATION OF DEAD OIL VISCOSITY")
add_db("Based on PVT experimental data of 472 series of Iranian oil reservoirs. About 250 series of PVT and viscosity data have been used in developing a new empirical correlation.")
add_db("Validity and accuracy of the proposed correlations (dead oil viscosity, saturated and undersaturated viscosity) have been checked by their application to 222 samples.")
add_parameter("api", "Api", "API oil gravity",  17,  44)
add_parameter("t", "Fahrenheit", "Temperature",  105,  295)
add_author("Naseri, Nikazar & Mousavi")
add_ref("naseri:2005")
end_correlation()

################################################################

## verificada con python
begin_correlation("UodPetroskyFarshad", "DeadOilViscosity", "CP", 0.725, 10.249)
add_title("PETROSKY & FARSHAD CORRELATION, CALCULATION OF DEAD OIL VISCOSITY")
add_db("Based on a set of 118 data points from 126 laboratory PVT analyses of Gulf of Mexico crude oils.")
add_db("Fluid samples were obtained from reservoirs located offshore Texas and Louisiana. Producing areas from Galveston Island, eastward, through Main Pass are represented.")
add_internal_note("The independent variables were obtained from laboratory differential data. The API oil gravity is the residual oil gravity.")
add_parameter("api", "Api", "API oil gravity",   25.4,  46.1)
add_parameter("t", "Fahrenheit", "Temperature",  114,  288)
add_author("Petrosky & Farshad")
add_ref("petrosky:1995")
end_correlation()

################################################################

## verificada con python
begin_correlation("UodDeGhettoEtAl", "DeadOilViscosity", "CP",  7.7, 1386.9)
add_title("DE GHETTO, PAONE & VILLA CORRELATION FOR EXTRA-HEAVY AND HEAVY OILS (MODIFIED EGBOGAH-JACK CORRELATION), CALCULATION OF DEAD OIL VISCOSITY")
add_db("Based on 1200 measured data points of 63 heavy and extra-heavy oil samples obtained from the Mediterranean Basin, Africa and the Persian Gulf.")
add_db("Oil samples have been divided in two different API gravity classes: extra-heavy oils for °API<=10, heavy oils for 10<°API<=22.3.")
add_parameter("api", "Api", "API oil gravity",  6,  22.3)
add_parameter("t", "Fahrenheit", "Temperature",  131.4,  250.7)
add_author("De Ghetto, Paone & Villa")
add_ref("deGhetto:1995")
end_correlation()

################################################################

## verificada con python
begin_correlation("UodPerezML", "DeadOilViscosity", "CP")
add_title("PÉREZ, HENY & LAGO CORRELATION (MODIFIED BEGGS & ROBINSON CORRELATION), CALCULATION OF DEAD OIL VISCOSITY")
add_parameter("api", "Api", "API oil gravity",  6.4,  12.9)
add_parameter("t", "Fahrenheit", "Temperature",  112,  300)
add_author("Pérez, Heny & Lago")
add_ref("perez:2001")
add_internal_note("The correlation could not be verified because the original reference is not available. Date: September 1 2016.")
set_hidden()
end_correlation()

################################################################

begin_correlation("UodDindorukChristman", "DeadOilViscosity", "CP", 0.896, 62.63)
add_title("DINDORUK & CHRISTMAN CORRELATION, CALCULATION OF DEAD OIL VISCOSITY")
add_parameter("api", "Api", "API oil gravity",  17.4,  40)
add_parameter("t", "Fahrenheit", "Temperature",  121,  276)
add_parameter("pb", "psia", "Bubble point pressure",  926,  12230)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  133,  3050)
add_db("Based on more than 90 PVT reports from the Gulf of Mexico.")
add_author("Dindoruk & Christman")
add_ref("dindoruk:2004")
end_correlation()

