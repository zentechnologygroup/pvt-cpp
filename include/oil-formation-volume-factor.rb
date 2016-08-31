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


################################################################

## verificada con python
begin_correlation("BoHanafy", "OilFormationVolumeFactor", "RB_STB",
                  1.032, 4.35)
add_doc(%Q[     HANAFY CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR 
                    
    @see: H.H. Hanafy, S.M. Macary, Y.M. ElNady, A.A. Bayomi and
    M.H. El Batanony. "A New Approach for Predicting the Crude Oil
    Properties," SPE 37439, 1997   
        
    Based on experimental PVT data of 324 fluid samples taken from 176
    wells located in 75 fields. This data represents 15 productive
    zones of 123 reservoirs distributed along three different regions
    of Egypt, including the Gulf of Suez, Western Desert, and Sinai. 
        
 ])

add_parameter("rs", "SCF_STB", 7, 4272)
add_parameter("rsb", "SCF_STB")
add_parameter("p", "psia")
add_parameter("pb", "psia", 36, 5003)
add_parameter("co", "psia_1", 1.032e-6, 4.35e-6)

end_correlation()

################################################################

## verificada con python
begin_correlation("BoKartoatmodjo", "OilFormationVolumeFactor", "RB_STB",
                  1.007, 2.144)
add_doc(%Q[     KARTOATMODJO & SCHMIDT CORRELATION, CALCULATION OF OIL
    FORMATION VOLUME FACTOR  
                          
    @see: R.S. Kartoatmodjo and Z. Schmidt. "New Correlations For
    Crude Oil Physical Properties", SPE 23556, 1991. 
        
    Based on a set of 5392 data points, which represent 740 different
    crude oil samples.  

    The data bank was collected from PVT reports and literature. The
    first major source was from South East Asia, mainly
    Indonesia. The second source was North America, including the
    offshore area. The rest came from the Middle East and Latin America. 
 ])

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

end_correlation()

################################################################

## verificada con python
begin_correlation("BoLasater", "OilFormationVolumeFactor", "RB_STB")
add_doc(%Q[     LASATER CORRELATION (STANDING CORRELATION APPLIED FOR
    LASATER), CALCULATION OF OIL FORMATION VOLUME FACTOR
                
    @see: Lasater, J.A.: "Bubble Point Pressure Correlation", Trans
    AIME (1958) 213, 379-381 

    @see: Standing, M.B.: "A Pressure-Volume-Temperature Correlation
    for Mixtures of California Oil and Gases", American Petroleum
    Institute, 47-275 API, 1947. 

    @see: Beggs, H.D. "Oil System Correlations", Petroleum Engineering
    Handbook, H.C. SPE, Richardson, TX (1987). Cap 22. Pag 22-6.  

    @see: McCain, W.D., Jr.: "The Properties of Petroleum Fluids", 2nd
    ed. Tulsa, OK: PennWell Books, 1990. 
 
    Lasater correlation is based on data obtained from Canada, Western
    and Mid-Continental United States, and South America. 

    Standing correlation for formation volumes of bubble point liquids
    is based on data collected from 22 different Californian
    crude-oil-natural-gas mixtures (Western United States). 
 ])

add_parameter("yg", "Sgg")
add_parameter("yo", "Sg_do", "Quantity<Api>(51.1)", "Quantity<Api>(17.9)")
add_parameter("rs", "SCF_STB")
add_parameter("rsb", "SCF_STB", 3, 2905)
add_parameter("t", "Fahrenheit", 82, 272)
add_parameter("p", "psia")
add_parameter("pb", "psia", 48, 5780)
add_parameter("co", "psia_1")

end_correlation()
