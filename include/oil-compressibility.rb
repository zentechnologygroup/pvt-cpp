# coding: utf-8

# verificada con python!
begin_correlation("CoMcCainEtAl", "OilCompressibility", "psia_1",
                  31e-6, 6600e-6)
add_doc(%Q[    McCAIN ET AL. CORRELATION FOR SATURATED OILS (PRESSURES
    BELOW THE BUBBLE POINT), CALCULATION OF OIL ISOTHERMAL COMPRESSIBILITY 
       
    @see: McCain, W.D., Jr., Rollins, J.B, and Villena-Lanzi, A.J.:
    "The Coefficient of Isothermal Compressibility of Black Oils at
    Pressures Below the BubblePoint", Trans., AIME (1988) 285, 659-662. 

    @see: Bánzer, C.: "Correlaciones Numéricas P.V.T.", Universidad
    del Zulia, 1996. 

    Based on 2500 experimental data obtained from different crude
    oils. The equation was developed with data from reservoir-fluid
    studies of 260 oil fields worldwide. 

 ])
add_parameter("api", "Api", 18, 52)
add_parameter("rsb", "SCF_STB", 15, 1947)
add_parameter("t", "Rankine",
              "Quantity<Fahrenheit>(78)", "Quantity<Fahrenheit>(330)")
add_parameter("p", "psia", 500, 5300)
add_parameter("pb", "psia", 763, 5300)
end_correlation()

################################################################        

# verificada con python!
begin_correlation("CoDeGhetto", "OilCompressibility", "psia_1", 3.02e-6, 42.9e-6)
add_doc(%Q[    DE GHETTO CORRELATION FOR EXTRA-HEAVY AND HEAVY OILS
    (MODIFIED VASQUEZ-BEGGS CORRELATION), CALCULATION OF OIL ISOTHERMAL
    COMPRESSIBILITY 
             
    @see: Giambattista De Ghetto, Francesco Paone and Marco
    Villa. "Pressure-Volume-Temperature Correlations for Heavy and Extra
    Heavy Oils," SPE 30316, 1995 
        
    Based on 1200 measured data points of 63 heavy and extra-heavy oil
    samples obtained from the Mediterranean Basin, Africa and the
    Persian Gulf.  

    Oil samples have been divided in two different API gravity
    classes: extra-heavy oils for °API<=10, heavy oils for
    10<°API<=22.3. 

 ])
add_parameter("yg", "Sgg", 0.623, 1.517)
add_parameter("api", "Api", 6, 22.3)
add_parameter("rsb", "SCF_STB", 17.21, 640.25)
add_parameter("t", "Fahrenheit", 131.4, 250.7)
add_parameter("tsep", "Fahrenheit", 59, 177.8)
add_parameter("p", "psia", 1038.49, 7411.54)
add_parameter("pb", "psia", 208.86, 4021.96)
add_parameter("psep", "psia", 14.5, 752.2)
end_correlation()

################################################################        

# verificada con python!
begin_correlation("CoHanafy", "OilCompressibility", "psia_1",
                  4.46e-6, 62.68e-6)
add_doc(%Q[     HANAFY CORRELATION, CALCULATION OF OIL ISOTHERMAL COMPRESSIBILITY
             
    @see: H.H. Hanafy, S.M. Macary, Y.M. ElNady, A.A. Bayomi and
    M.H. El Batanony. "A New Approach for Predicting the Crude Oil
    Properties," SPE 37439, 1997 
        
    Based on experimental PVT data of 324 fluid samples taken from 176
    wells located in 75 fields. This data represents 15
    productive zones of 123 reservoirs distributed along
    three different regions of Egypt, including the Gulf
    of Suez, Western Desert, and Sinai. 

 ])

add_parameter("api", "Api", 17.8, 47.7)
add_parameter("rsb", "SCF_STB", 7, 4272)
add_parameter("t", "Fahrenheit", 107, 327)
add_parameter("p", "psia")
add_parameter("pb", "psia", 36, 5003)
end_correlation()


################################################################        

# verificada con python!
begin_correlation("CoKartoatmodjoSchmidt", "OilCompressibility", "psia_1")
add_doc(%Q[    KARTOATMODJO & SCHMIDT CORRELATION, CALCULATION OF OIL
    ISOTHERMAL COMPRESSIBILITY
	
    @see: R.S. Kartoatmodjo and Z. Schmidt. "New Correlations For
    Crude Oil Physical Properties", SPE 23556, 1991. 
 
    Based on a total of 3588 data points collected from 661 different
crude oil samples. 

    The data bank was collected from PVT reports and literature. The
    first major source was from South East Asia, mainly Indonesia. The
    second source was North America, including the offshore area. The
    rest came from the Middle East and Latin America. 

 ])

add_parameter("yg", "Sgg", 0.4824, 1.668)
add_parameter("api", "Api", 14.4, 59)
add_parameter("rsb", "SCF_STB", 0, 2890)
add_parameter("t", "Fahrenheit", 75, 320)
add_parameter("tsep", "Fahrenheit", 38, 294)
add_parameter("p", "psia", 24.7, 4764.7)
add_parameter("pb", "psia", 24.7, 6014.7)
add_parameter("psep", "psia", 14.7, 1414.7)
end_correlation()

################################################################

# verificada con python!
begin_correlation("CoPetroskyFarshad", "OilCompressibility", "psia_1",
                  3.507e-6, 24.64e-6)
add_doc(%Q[    PETROSKY & FARSHAD CORRELATION, CALCULATION OF OIL
    ISOTHERMAL COMPRESSIBILITY   
	
    @see: G.E. Petrosky Jr. and F.F. Farshad. "Pressure-Volume-Temperature
    Correlations for Gulf of Mexico Crude Oils," SPE 26644, 1993  
        
    Based on 81 laboratory PVT analyses, using a total of 304 data
    points. Fluid samples were obtained from reservoirs located offshore
    Texas and Louisiana.  

    Producing areas from Galveston Island, eastward, through Main Pass
    are represented. 

    Many Gulf of Mexico crudes can be characterized as naphthenic or
    biodegraded oils.
 ])
add_parameter("yg", "Sgg", 0.5781, 0.8519)
add_parameter("api", "Api", 16.3, 45)
add_parameter("rsb", "SCF_STB", 217, 1406)
add_parameter("t", "Fahrenheit", 114, 288)
add_parameter("p", "psia", 1700, 10692)
add_parameter("pb", "psia", 1574, 6523)
end_correlation()

################################################################

# verificada con python!
begin_correlation("CoVasquezBeggs", "OilCompressibility", "psia_1",)
add_doc(%Q[    VASQUEZ & BEGGS CORRELATION, CALCULATION OF OIL
    ISOTHERMAL COMPRESSIBILITY
             
    @see: M.E. Vasquez and H.D. Beggs. "Correlations for Fluid
    Physical Property Prediction," JPT 968-70, June 1980 

    @see: Bánzer, C.: "Correlaciones Numéricas P.V.T.", Universidad
    del Zulia, 1996. 
        
    Based on more than 600 laboratory PVT analyses from fields all
    over the world. The correlation was developed using a total of
    4036 data points. 

 ])
add_parameter("yg", "Sgg", 0.511, 1.351)
add_parameter("api", "Api", 15.3, 59.5)
add_parameter("rsb", "SCF_STB", 9.3, 2199)
add_parameter("t", "Fahrenheit")
add_parameter("tsep", "Fahrenheit", 76, 150)
add_parameter("p", "psia", 141, 9515)
add_parameter("pb", "psia")
add_parameter("psep", "psia", 60, 565)
end_correlation()

################################################################

# verificada con python!
begin_correlation("CoPerezML", "OilCompressibility", "psia_1")
add_doc(%Q[    PEREZ ET AL. CORRELATION (MODIFIED PETROSKY-FARSHAD
    CORRELATION), CALCULATION OF OIL ISOTHERMAL COMPRESSIBILITY 
        
    @see: C.A. Heny, V.L. Pérez, M.E. Lago, 2001: Evaluación y
    generación de correlaciones PVT para crudos extrapesados
    venezolanos. Informe Técnico INT-8713,2001. Proyecto: Estudio y
    Modelaje de Procesos de Recuperación de Crudos Pesados. Los Teques,
    Octubre de 2001 

 ])
add_parameter("yg", "Sgg", 0.571, 0.981)
add_parameter("api", "Api", 6.4, 12.9)
add_parameter("rsb", "SCF_STB", 38, 121)
add_parameter("t", "Fahrenheit", 112, 300)
add_parameter("p", "psia")
add_parameter("pb", "psia", 405, 1335)
end_correlation()

################################################################

# verificada con python!
begin_correlation("CoMillanArcia", "OilCompressibility", "psia_1",)
add_doc(%Q[    MILLÁN-ARCIA CORRELATION, CALCULATION OF OIL ISOTHERMAL
    COMPRESSIBILITY 
	
    @see: E.A. Millán-Arcia, 1984: Correlaciones para estimar el
    comportamiento PVT de crudos pesados venezolanos. I Jornadas Técnicas
    Corpoven 

    @see: C.A. Heny, V.L. Pérez, M.E. Lago, 2001: Evaluación y
    generación de correlaciones PVT para crudos extrapesados
    venezolanos. Informe Técnico INT-8713,2001. Proyecto: Estudio y
    Modelaje de Procesos de Recuperación de Crudos Pesados. Los
    Teques, Octubre de 2001 

 ])

add_parameter("api", "Api", 9.0, 20.2)
add_parameter("rsb", "SCF_STB", 53, 459)
add_parameter("t", "Fahrenheit", 87, 195)
add_parameter("p", "psia",)
add_parameter("pb", "psia", 222.0, 3432.7)
end_correlation()
