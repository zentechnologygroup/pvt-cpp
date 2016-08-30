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

################################################################

## verificada con python
begin_correlation("BoDoklaOsman", "OilFormationVolumeFactor", "RB_STB",
                  1.216, 2.493)
add_doc(%Q[    DOKLA & OSMAN CORRELATION, CALCULATION OF OIL FORMATION
    VOLUME FACTOR    

    @see: Dokla, M.E. y Osman, M.E.: "Correlation of PVT Properties
    for UAE Crudes",Trans AIME (1992) 293, 41-46 

    @see: Bánzer, C.: "Correlaciones Numéricas P.V.T.", Universidad del
    Zulia, 1996. 

    @see: McCain, W.D., Jr.: "The Properties of Petroleum Fluids", 2nd
    ed. Tulsa, OK: PennWell Books, 1990. 
        
        Based on 51 bottomhole samples from UAE reservoirs.
        
 ])

add_parameter("yg", "Sgg", 0.789, 1.29)
add_parameter("yo", "Sg_do", 0.8236, 0.886)
add_parameter("rs", "SCF_STB")
add_parameter("rsb", "SCF_STB", 181, 2266)
add_parameter("t", "Rankine",
              "Quantity<Fahrenheit>(190)", "Quantity<Fahrenheit>(275)")
add_parameter("p", "psia")
add_parameter("pb", "psia", 590, 4640)
add_parameter("co", "psia_1")

end_correlation()


################################################################

## verificada con python
begin_correlation("BoGlaso", "OilFormationVolumeFactor", "RB_STB",
                  1.087, 2.588)
add_doc(%Q[     GLASO CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR  
                  
    @see: Glaso, O.: "Generalized Pressure-Volume-Temperature
    Correlations," Journal of Petroleum Technology, 1980 

    @see: Bánzer, C.: "Correlaciones Numéricas P.V.T.", Universidad
    del Zulia, 1996. 

    @see: McCain, W.D., Jr.: "The Properties of Petroleum Fluids", 2nd
    ed. Tulsa, OK: PennWell Books, 1990. 
        
    Based on 26 samples from the North Sea (collected from wells in
    the region 56 to 62°N) and 19 samples from the Middle East,
    Algeria, and several areas in the U.S.  
        
 ])

add_parameter("yg", "Sgg", 0.650, 1.276)
add_parameter("yo", "Sg_do", "Quantity<Api>(48.1)", "Quantity<Api>(22.3)")
add_parameter("rs", "SCF_STB", 90, 2637)
add_parameter("rsb", "SCF_STB")
add_parameter("t", "Fahrenheit", 80, 280)
add_parameter("p", "psia")
add_parameter("pb", "psia", 150, 7127)
add_parameter("co", "psia_1")

end_correlation()
