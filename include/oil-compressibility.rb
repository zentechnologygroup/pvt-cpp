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
