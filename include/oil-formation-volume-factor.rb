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


################################################################

## verificada con python
begin_correlation("BoPetroskyFarshad", "OilFormationVolumeFactor", "RB_STB",
                 1.1178, 1.6229)
add_doc(%Q[     PETROSKY & FARSHAD CORRELATION, CALCULATION OF OIL
    FORMATION VOLUME FACTOR

    @see: G.E. Petrosky Jr. and
    F.F. Farshad. "Pressure-Volume-Temperature Correlations for
    Gulf of Mexico Crude Oils," SPE 26644, 1993 
        
    Based on 81 laboratory PVT analyses. Fluid samples were obtained
    from reservoirs located offshore Texas and Louisiana.  

    Producing areas from Galveston Island, eastward, through Main Pass
    are represented.         

    Many Gulf of Mexico crudes can be characterized as naphthenic or
    biodegraded oils. 
 ])

add_parameter("yg", "Sgg", 0.5781, 0.8519)
add_parameter("yo", "Sg_do", "Quantity<Api>(45)", "Quantity<Api>(16.3)")
add_parameter("rs", "SCF_STB", 217, 1406)
add_parameter("rsb", "SCF_STB")
add_parameter("t", "Fahrenheit", 114, 288)
add_parameter("p", "psia")
add_parameter("pb", "psia", 1574, 6523)
add_parameter("co", "psia_1")

end_correlation()


################################################################

## verificada con python
begin_correlation("BoStanding", "OilFormationVolumeFactor", "RB_STB",
                 1.024, 2.15)
add_doc(%Q[     STANDING CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR  
             
    @see: M.B. Standing. "A Pressure-Volume-Temperature Correlation for
    Mixtures of California Oil and Gases" Drill. & Prod. Prac.,Beal C.
    The Viscosity-Temperature Chart 0361-39 Nat. Bur . Stds Circular C461-1947 

    @see: Al-Shammasi, A.A.: "A Review of Bubblepoint Pressure and Oil
    Formation Volume Factor Correlations", SPE 71302, 2001. 

    @see: McCain, W.D., Jr.: "The Properties of Petroleum Fluids", 2nd
    ed. Tulsa, OK: PennWell Books, 1990. 

    Based on 105 experimentally determined bubble point pressures from
    22 different Californian crude-oil-natural-gas mixtures. 
        
 ])

add_parameter("yg", "Sgg", 0.59, 0.95)
add_parameter("yo", "Sg_do", "Quantity<Api>(63.8)", "Quantity<Api>(16.5)")
add_parameter("rs", "SCF_STB", 20, 1425)
add_parameter("rsb", "SCF_STB")
add_parameter("t", "Fahrenheit", 100, 258)
add_parameter("p", "psia")
add_parameter("pb", "psia")
add_parameter("co", "psia_1")

end_correlation()


################################################################

## verificada con python
begin_correlation("BoTotalCFP", "OilFormationVolumeFactor", "RB_STB")
add_doc(%Q[     TOTAL CFP CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR
             
    @see: TOTAL Compaigne Francaise des Petroles: "Proyectos de
    Inyección de Fluidos - Correlaciones PVT para Crudos del Oriente de
    Venezuela", S.A., MENEVEN. Septiembre de 1996 

    @see: Bánzer, C.: "Correlaciones Numéricas P.V.T.", Universidad
    del Zulia, 1996.         
 ])

add_parameter("yg", "Sgg")
add_parameter("api", "Api")
add_parameter("rs", "SCF_STB")
add_parameter("rsb", "SCF_STB")
add_parameter("t", "Fahrenheit")
add_parameter("p", "psia")
add_parameter("pb", "psia")
add_parameter("co", "psia_1")

end_correlation()


################################################################

## verificada con python
begin_correlation("BoVasquezBeggs", "OilFormationVolumeFactor", "RB_STB",
                  1.028, 2.226)
add_doc(%Q[     VASQUEZ & BEGGS CORRELATION, CALCULATION OF OIL
    FORMATION VOLUME FACTOR 

    @see: M.E. Vasquez and H.D. Beggs. "Correlations for Fluid
    Physical Property Prediction," JPT 968-70, June 1980 

    @see: Al-Shammasi, A.A.: "A Review of Bubblepoint Pressure and Oil
    Formation Volume Factor Correlations", SPE 71302, 2001. 

    @see: C. Bánzer. "Correlaciones Numéricas P.V.T.", Universidad del
    Zulia, 1996. 
 ])

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

end_correlation()

################################################################

## verificada con python
begin_correlation("BoCegarra", "OilFormationVolumeFactor", "RB_STB")
add_doc(%Q[     CEGARRA CORRELATION (STANDING CORRELATION APPLIED FOR
    CEGARRA), CALCULATION OF OIL FORMATION VOLUME FACTOR 

    @see: Cegarra, L., 2007: "Correlación para el cálculo de la
    presión de burbuja de los yacimientos de crudo liviano y mediano
    del Oriente de Venezuela". Trabajo especial de grado. Universidad
    del Zulia

    @see: Standing, M.B.: "A Pressure-Volume-Temperature Correlation
    for Mixtures of California Oil and Gases" Drill. &
    Prod. Prac.,Beal C. The Viscosity-Temperature Chart 0361-39
    Nat. Bur . Stds Circular C461-1947 

    @see: McCain, W.D., Jr.: "The Properties of Petroleum Fluids", 2nd
    ed. Tulsa, OK: PennWell Books, 1990. 

    Based on 375 consistent PVT analyses. The data bank was compiled
    from 8394 fields of eastern Venezuela subdivided into 5192 fields
    of light oil and 3202 fields of medium oil.  

    The correlation takes the functional form of Standing correlation,
    which is based on 105 experimental data of 22 different
    crude-oil-natural-gas mixtures from California (USA). 

    @note: The isothermal compressibility of crude oils (Co) used in
    this equation is obtained from the Vasquez & Beggs correlation. 

    @note: The correlation's development ranges could not be verified
    because the original reference is not available. Date: August 22
    2016.  
 ])

add_parameter("yg", "Sgg", 0.654, 1.337)
add_parameter("yo", "Sg_do", "Quantity<Api>(41.8)", "Quantity<Api>(22)")
add_parameter("rs", "SCF_STB", 135, 4290)
add_parameter("rsb", "SCF_STB")
add_parameter("t", "Fahrenheit", 117, 354)
add_parameter("p", "psia")
add_parameter("pb", "psia", 735, 7303)
add_parameter("co", "psia_1")

end_correlation()

################################################################

## verificada con python
begin_correlation("BoPerezML", "OilFormationVolumeFactor", "RB_STB",
                  1.032, 1.183)
add_doc(%Q[     PEREZ ET AL. CORRELATION (EZEKWE-QUERIN-HUMPHREY
    CORRELATION USED BY PEREZ ET AL.), CALCULATION OF OIL FORMATION VOLUME
    FACTOR  

    @see: C.A. Heny, V.L. Pérez, M.E. Lago, 2001: Evaluación y
    generación de correlaciones PVT para crudos extrapesados
    venezolanos. Informe Técnico INT-8713,2001. Proyecto: Estudio y
    Modelaje de Procesos de Recuperación de Crudos Pesados. Los
    Teques, Octubre de 2001 

    @note: The correlation could not be verified because the original
    reference is not available. Date: August 22 2016. 

    @note: <param API for C++> :param Yo: Oil specific gravity [ratio
    water=1]       
 ])

add_parameter("yg", "Sgg", 0.571, 0.981)
add_parameter("yo", "Sg_do", "Quantity<Api>(12.9)", "Quantity<Api>(6.4)")
add_parameter("rsb", "SCF_STB", 38, 121)
add_parameter("t", "Fahrenheit", 112, 300)
add_parameter("p", "psia")
add_parameter("pb", "psia", 405, 1335)
add_parameter("co", "psia_1")

end_correlation()


################################################################

## verificada con python
begin_correlation("BoVelardeMcCain", "OilFormationVolumeFactor", "RB_STB",
                  1.040, 2.082)
add_doc(%Q[     VELARDE, BLASINGAME & McCAIN CORRELATION, CALCULATION
    OF OIL FORMATION VOLUME FACTOR 
        
    @see: J. Velarde, T.A. Blasingame and W.D. McCain,
    Jr. "Correlation of Black Oil Properties at Pressures Below Bubble
    Point Pressure: A New Approach," The Petroleum Society 97-93, 1997 

    @see: McCain, W.D., Jr.: "The Properties of Petroleum Fluids", 2nd
    ed. Tulsa, OK: PennWell Books, 1990. 

    Based on 195 PVT reports, including 2097 data sets.
 ])

add_parameter("yg", "Sgg", 0.561, 1.101)
add_parameter("yo", "Sg_do", "Quantity<Api>(53.4)", "Quantity<Api>(11.6)")
add_parameter("rs", "SCF_STB")
add_parameter("rsb", "SCF_STB", 102, 1808)
add_parameter("t", "Fahrenheit", 70, 307)
add_parameter("p", "psia")
add_parameter("pb", "psia", 106, 5312)
add_parameter("co", "psia_1")

end_correlation()


################################################################

## verificada con python
begin_correlation("BoMillanArcia", "OilFormationVolumeFactor", "RB_STB",
                  1.014, 1.248)
add_doc(%Q[     MILLÁN-ARCIA CORRELATION, CALCULATION OF OIL FORMATION
    VOLUME FACTOR

    Venezuelan heavy crudes were correlated.
        
    @see: E.A. Millán-Arcia, 1984: Correlaciones para estimar el
    comportamiento PVT de crudos pesados venezolanos. I Jornadas
    Técnicas Corpoven 

    @see: C.A. Heny, V.L. Pérez, M.E. Lago, 2001: Evaluación y
    generación de correlaciones PVT para crudos extrapesados
    venezolanos. Informe Técnico INT-8713,2001. Proyecto: Estudio y
    Modelaje de Procesos de Recuperación de Crudos Pesados. Los Teques,
    Octubre de 2001 
 ])

add_parameter("yg", "Sgg")
add_parameter("api", "Api", 9, 20.2)
add_parameter("rs", "SCF_STB", 53, 459)
add_parameter("rsb", "SCF_STB")
add_parameter("t", "Fahrenheit", 87, 195)
add_parameter("p", "psia")
add_parameter("pb", "psia", 222.0, 3432.7)
add_parameter("co", "psia_1")

end_correlation()


################################################################

## verificada con python
begin_correlation("BoManucciRosales", "OilFormationVolumeFactor", "RB_STB",
                  1.050, 2.400)
add_doc(%Q[     MANUCCI-ROSALES CORRELATION, CALCULATION OF OIL
    FORMATION VOLUME FACTOR 

    Crude oils of eastern Venezuela were correlated, by using differential
    separation data from laboratory PVT analysis. 
                
    @see: J.E. Manucci, E.E. Rosales, 1968: "Correlaciones de presión
    de burbujeo y factor volumétrico del petróleo para crudos el
    oriente de Venezuela". Corporación Venezolana del Petróleo, II
    Jornadas técnicas de petróleo, marzo 1968 

    @see: C.A. Heny, V.L. Pérez, M.E. Lago, 2001: Evaluación y
    generación de correlaciones PVT para crudos extrapesados
    venezolanos. Informe Técnico INT-8713,2001. Proyecto: Estudio y
    Modelaje de Procesos de Recuperación de Crudos Pesados. Los
    Teques, Octubre de 2001 

 ])

add_parameter("yg", "Sgg")
add_parameter("api", "Api", 14, 40)
add_parameter("rs", "SCF_STB", 180, 2500)
add_parameter("rsb", "SCF_STB")
add_parameter("t", "Fahrenheit", 165, 304)
add_parameter("p", "psia")
add_parameter("pb", "psia", 1230, 5035)
add_parameter("co", "psia_1")

end_correlation()
