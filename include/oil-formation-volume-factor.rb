# coding: utf-8

## verificada con python
begin_correlation("BoAlmarhoun", "OilFormationVolumeFactor", "RB_STB",
                  1.032, 1.997)
add_doc(%Q[    AL-MARHOUN CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR

    Based on 69 bottomhole fluid samples from 69 Middle East oil reservoirs.
                   
    @see: Al-Marhoun, M.A.: "PVT Correlation for Middle East Crude
    Oils", Journal Petroleum Technology, May 1988, pp. 650-666.   

    @see: Bánzer, C.: "Correlaciones Numéricas P.V.T.", Universidad
    del Zulia, 1996. 

    @see: McCain, W.D., Jr. "The Properties of Petroleum Fluids", 2nd
    ed. Tulsa, OK: PennWell Books, 1990. 

 ])

add_parameter("yg", "Sgg", 0.752, 1.367)
add_parameter("yo", "Sg_do", "Quantity<Api>(44.6)", "Quantity<Api>(19.4)")
add_parameter("rs", "SCF_STB", 26, 1602)
add_parameter("rsb", "SCF_STB")
add_parameter("t", "Rankine",
              "Quantity<Fahrenheit>(100)", "Quantity<Fahrenheit>(258)")
add_parameter("p", "psia")
add_parameter("pb", "psia", 130, 3573)
add_parameter("co", "psia_1")

end_correlation()

################################################################

## verificada con python
begin_correlation("BoAlShammasi", "OilFormationVolumeFactor", "RB_STB",
                  1.02, 2.916)
add_doc(%Q[   Al-SHAMMASI CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR

    @see: Al-Shammasi, A.A.: "Bubble Point Pressure and Oil Formation
    Volume Factor Correlations", SPE 53185, 1999  

    @see: McCain, W.D., Jr. "The Properties of Petroleum Fluids", 2nd
    ed. Tulsa, OK: PennWell Books, 1990. 
        
    Based on a total of 1709 data sets (1661 data sets from 13
    different published literature papers and 48 data sets originated
    from Kuwait reservoirs from unpublished sources). 

    The data available after excluding the duplicates and the repeated
    data sets for formation volume factor are 1345 data sets. 
 ])

add_parameter("yg", "Sgg", 0.51, 3.44)
add_parameter("yo", "Sg_do", "Quantity<Api>(63.7)", "Quantity<Api>(6)")
add_parameter("rs", "SCF_STB", 6, 3298.6)
add_parameter("rsb", "SCF_STB")
add_parameter("t", "Fahrenheit", 74, 341.6)
add_parameter("p", "psia")
add_parameter("pb", "psia", 31.7, 7127)
add_parameter("co", "psia_1")

end_correlation()

###############################################################

## verificada con python
begin_correlation("BoDeGhetto", "OilFormationVolumeFactor", "RB_STB",
                  1.057, 1.362)
add_doc(%Q[    DE GHETTO CORRELATION FOR EXTRA-HEAVY AND HEAVY OILS
    (VASQUEZ & BEGGS CORRELATION), CALCULATION OF OIL FORMATION VOLUME FACTOR 

    @see: Giambattista De Ghetto, Francesco Paone and Marco
    Villa. "Pressure-Volume-Temperature Correlations for Heavy and
    Extra Heavy Oils," SPE 30316, 1995 
        
    Based on 1200 measured data points of 63 heavy and extra-heavy oil
    samples obtained from the Mediterranean Basin, Africa and the
    Persian Gulf.  

    Oil samples have been divided in two different API gravity
    classes: extra-heavy oils for °API<=10, heavy oils for
    10<°API<=22.3. 
 ])

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

end_correlation()


################################################################

begin_correlation("BoDindorukChristman", "OilFormationVolumeFactor", "RB_STB",
                  1.0845, 2.7381)
add_doc(%Q[    DINDORUK & CHRISTMAN CORRELATION, CALCULATION OF OIL
    FORMATION VOLUME FACTOR 
        
    @see: Dindoruk, B. and Christman, P.G.: "Properties and Viscosity
    Correlations for Gulf of Mexico Oils", SPE 89030, 2004 

    @see: McCain, W.D., Jr.: "The Properties of Petroleum Fluids", 2nd
    ed. Tulsa, OK: PennWell Books, 1990. 
                
    Based on approximately 100 PVT reports from the Gulf of Mexico.
 ])

add_parameter("yg", "Sgg", 0.6017, 1.027)
add_parameter("api", "Api", 14.7, 40)
add_parameter("rs", "SCF_STB")
add_parameter("rsb", "SCF_STB", 133, 3050)
add_parameter("t", "Fahrenheit", 117, 276)
add_parameter("tsep", "Fahrenheit")
add_parameter("p", "psia")
add_parameter("pb", "psia")
add_parameter("co", "psia_1")

end_correlation()
