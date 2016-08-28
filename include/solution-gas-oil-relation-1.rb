# coding: utf-8

## verificada con python
begin_correlation("RsStanding", "SolutionGasOilRelation", "SCF_STB")
add_doc(%Q[    STANDING CORRELATION, CALCULATION OF SOLUTION GOR

    Based on 105 experimentally determined bubble point pressures
    from 22 different Californian crude-oil-natural-gas mixtures. 
              
    @see: M.B. Standing. "A Pressure-Volume-Temperature
    Correlation for Mixtures of California Oil and Gases , " Drill. &
    Prod. Prac.,Beal C. The Viscosity-Temperature Chart 0361-39
    Nat. Bur. Stds Circular C461-1947"  

    @see: C. Bánzer. "Correlaciones Numéricas P.V.T.", Universidad
    del Zulia, 1996. ])
add_parameter("yg", "Sgg", 0.59, 0.95)
add_parameter("pb", "psia", 130, 700)
add_parameter("p", "psia")
add_parameter("api", "Api", 16.5, 63.8)
add_parameter("t", "Fahrenheit", 100, 258)
add_parameter("rsb", "SCF_STB", 20, 1425)
end_correlation()


## verificada con python
begin_correlation("RsTotalCFP", "SolutionGasOilRelation", "SCF_STB")
add_doc(%Q[    TOTAL CFP CORRELATION, CALCULATION OF SOLUTION GOR
 
    Based on 336 PVT analyses of crude oil from eastern Venezuela.
        
    @see: TOTAL Compagnie Francaise Des Petroles CFP: " Proyectos de
    Inyeccion de Fluidos Correlaciones PVT para Crudos del Oriente de
    Venezuela" S.A. MENEVEN Sep. 1983. 

    @see: C. Bánzer. "Correlaciones Numéricas P.V.T.", Universidad del Zulia, 1996.
    ])
add_parameter("yg", "Sgg")
add_parameter("pb", "psia")
add_parameter("p", "psia")
add_parameter("api", "Api", 0, 45)
add_parameter("t", "Fahrenheit")
add_parameter("rsb", "SCF_STB")
end_correlation()


## verificada con python
begin_correlation("RsVasquezBeggs", "SolutionGasOilRelation", "SCF_STB")
add_doc(%Q[    VASQUEZ & BEGGS CORRELATION, CALCULATION OF SOLUTION GOR

    Based on more than 600 laboratory PVT analyses from fields all
    over the world.

    The equation for solution GOR was obtained by regression analysis
    using 5008 measured data points.

    @see: M.E. Vasquez and H.D. Beggs. "Correlations for Fluid
    Physical Property Prediction," JPT 968-70, June 1980.

    @see: Al-Shammasi, A.A.: "A Review of Bubblepoint Pressure and Oil
    Formation Volume Factor Correlations", SPE 71302, 2001.

    @see: C. Bánzer. "Correlaciones Numéricas P.V.T.", Universidad del
    Zulia, 1996.

    @note: The gas gravity is corrected if it resulted from a
    first-stage separation at a pressure other than 100 psig
    (approximately 114.7 psia). 
  ])

add_parameter("yg", "Sgg", 0.511, 1.35)
add_parameter("pb", "psia", 15, 6055)
add_parameter("p", "psia")
add_parameter("api", "Api", 15.3, 59.3)
add_parameter("t", "Fahrenheit", 75, 294)
add_parameter("tsep", "Fahrenheit", 76, 150)
add_parameter("psep", "psia", 60, 595) 
add_parameter("rsb", "SCF_STB", 0, 2199)
end_correlation()


begin_correlation("RsVelarde", "SolutionGasOilRelation", "SCF_STB")
add_doc(%Q[    VELARDE CORRELATION, CALCULATION OF SOLUTION GOR

    Based on 195 PVT reports, including 2097 data sets.
                     
     @see: J. Velarde, T.A. Blasingame and W.D. McCain, Jr. 
    "Correlation of Black Oil Properties at Pressures below
     Bubble Point Pressure - A New Approach," 
    The Petroleum Society 97-93, 1997.
  ])

add_parameter("yg", "Sgg", 0.561, 1.101)
add_parameter("pb", "psig", "Quantity<psia>(106)", "Quantity<psia>(5312)")
add_parameter("p", "psig")
add_parameter("api", "Api", 11.6, 53.4)
add_parameter("t", "Fahrenheit", 70, 307)
add_parameter("rsb", "SCF_STB", 102, 1808)
end_correlation()


