# coding: utf-8

# coding: utf-8

## verificada con python
begin_correlation("UobBeggsRobinson", "SaturatedOilViscosity", "CP")
add_doc(%Q[    BEGGS & ROBINSON CORRELATION, CALCULATION OF SATURATED OIL VISCOSITY
                
    @see: Beggs H.D. and Robinson J.R.: Estimating the Viscosity of
    Crude Oil Systems. Journal Petroleum Technology (Sep. 1975) 
        
    Based on 2073 live oil observations. Beggs and Robinson developed
    correlations for dead and live oil viscosities with samples obtained
    from 600 oil systems. 

 ])
add_parameter("uod", "CP")
add_parameter("rs", "SCF_STB", 20, 2070)
end_correlation()

################################################################        

## verificada con python
begin_correlation("UobChewConnally", "SaturatedOilViscosity", "CP", 0.1, 100)
add_doc(%Q[    CHEW & CONNALLY CORRELATION, CALCULATION OF SATURATED OIL VISCOSITY

    Based on 457 crude oil samples obtained from all the important
    producing areas of the U.S., as well as from Canada and South
    America. 

    @see: Chew Ju-Nam, & Connally Carl: A Viscosity Correlation for
    Gas-Saturated Crude Oils. Magnolia Petroleum Co. Dallas, TX,
    1959. SPE 1092-G. 

    @see: Bánzer, C.: "Correlaciones Numéricas P.V.T.", Universidad
    del Zulia, 1996. 

    @see: Ahmed, T.: "Reservoir Engineering Handbook", 4th edition,
    Gulf Professional Publishing, U.S.A., 2010. 

    @note: Chew & Connally presented the graphical correlation and the
    general form of the equation. The values of A and b of the equation
    are presented by Bánzer (1996) and Ahmed (2010). 
  
 ])
add_parameter("uod", "CP", 0.377, 100)
add_parameter("rs", "SCF_STB", 0, 1600)
end_correlation()

################################################################

## verificada con python
begin_correlation("UobKhan", "SaturatedOilViscosity", "CP", 0.13, 77.4)
add_doc(%Q[     KHAN ET AL. CORRELATION, CALCULATION OF SATURATED OIL VISCOSITY

    Based on 75 bottom hole samples taken from 62 Saudi Arabian oil reservoirs.
    A total of 150 data points were used for bubble point oil
    viscosity in 1691 for oil viscosity below the bubble point
    pressure.  
                              
    @see: Khan, S.A. et al.: Viscosity Correlations for Saudi Arabian
    Crude Oils - SPE 15720 
        
    Based on 75 bottom hole samples taken from 62 Saudi Arabian oil reservoirs.

 ])

add_parameter("rs", "SCF_STB", 24, 1901)
add_parameter("api", "Api", 14.3, 44.6)
add_parameter("yg", "Sgg", 0.752, 1.367)
add_parameter("t", "Fahrenheit", 75, 240)
add_parameter("p" , "psia", 75, 240)
add_parameter("pb" , "psia", 107, 4315)
end_correlation()

################################################################

## verificada con python
begin_correlation("UobKartoatmodjoSchmidt", "SaturatedOilViscosity",
                  "CP", 0.097, 586)
add_doc(%Q[     KARTOATMODJO & SCHMIDT CORRELATION FOR HEAVY, MEDIUM
    AND LIGHT OILS, CALCULATION OF SATURATED OIL VISCOSITY 

    Based on 5321 data points of heavy, medium and light oil samples. The
    data bank was collected from PVT reports and literature.  

    The first major source was from South East Asia, mainly
    Indonesia. The second source was North America, including the
    offshore area. The rest came from the Middle East and Latin
    America. 

    Depending on the API gravity, the samples can cover three
    different classes of crude oils: heavy oils for 10<°API<=22.3,
    medium oils for 22.3<°API<=31.1, and light oils for °API>31.1. 

    @see: R.S. Kartoatmodjo and Z. Schmidt. "New Correlations For
    Crude Oil Physical Properties", University of Tulsa, 1991. SPE 23556.

 ])

add_parameter("uod", "CP", 0.506, 682.0)
add_parameter("rs", "SCF_STB", 0, 2890)
end_correlation()

################################################################

## verificada con python
begin_correlation("UobPetroskyFarshad", "SaturatedOilViscosity", "CP", 
                  0.211, 7.403)
add_doc(%Q[    PETROSKY & FARSHAD CORRELATION, CALCULATION OF
    SATURATED OIL VISCOSITY 
         
    Based on a set of 864 data points from 126 laboratory PVT analyses of
    Gulf of Mexico crude oils. For the ranges of carbon dioxide and
    nitrogen concentration in the mixture, 88 data points were
    considered. 

    Fluid samples were obtained from reservoirs located offshore Texas
    and Louisiana. Producing areas from Galveston Island,
    eastward, through Main Pass are represented. 
        
    @see: Petrosky, G.E. and Farshad, F.F.: "Viscosity Correlations
    for Golf of Mexico Crude Oils", April 1995. SPE 29468.         

 ])

add_parameter("uod", "CP", 0.725, 11.69)
add_parameter("rs", "SCF_STB", 21, 1885)
end_correlation()


################################################################

## verificada con python
begin_correlation("UobPerezML", "SaturatedOilViscosity", "CP")
add_doc(%Q[    PÉREZ ET AL. CORRELATION, CALCULATION OF SATURATED OIL VISCOSITY
                              
    :see: C.A. Heny, V.L. Pérez, M.E. Lago, 2001: Evaluación y
    generación de correlaciones PVT para crudos extrapesados
    venezolanos. Informe Técnico INT-8713,2001. Proyecto: Estudio y
    Modelaje de Procesos de Recuperación de Crudos Pesados. Los Teques,
    Octubre de 2001 

 ])

add_parameter("uod", "CP")
add_parameter("rs", "SCF_STB", 38, 121)
end_correlation()


################################################################

## verificada con python
begin_correlation("UobGilFonseca", "SaturatedOilViscosity", "CP")
add_doc(%Q[    GIL-FONSECA CORRELATION, CALCULATION OF SATURATED OIL VISCOSITY  
         
    @see: C.A. Heny, V.L. Pérez, M.E. Lago, 2001: Evaluación y
    generación de correlaciones PVT para crudos extrapesados
    venezolanos. Informe Técnico INT-8713,2001. Proyecto: Estudio y
    Modelaje de Procesos de Recuperación de Crudos Pesados. Los Teques,
    Octubre de 2001  

 ])

add_parameter("uod", "CP")
add_parameter("rs", "SCF_STB", )
end_correlation()


################################################################

## verificada con python
begin_correlation("UobDeGhettoEtAl", "SaturatedOilViscosity", "CP", 2.1, 295.9)
add_doc(%Q[    DE GHETTO ET AL. CORRELATION FOR EXTRA-HEAVY AND HEAVY
    OILS (MODIFIED KARTOATMODJO CORRELATION), CALCULATION OF SATURATED OIL
    VISCOSITY 
                              
    Based on 1200 measured data points of 63 heavy and extra-heavy oil
    samples obtained from the Mediterranean Basin, Africa and the Persian
    Gulf.  

    Oil samples have been divided in two different API gravity
    classes: extra-heavy oils for °API<=10, heavy oils for
    10<°API<=22.3. 

    @see: De Ghetto, G., Paone, F., and Villa, M.:
    "Pressure-Volume-Temperature Correlations for Heavy and Extra
    Heavy Oils", Canada, 1995. SPE 30316. 

 ])

add_parameter("uod", "CP", 7.7, 1386.9)
add_parameter("rs", "SCF_STB", 17.21, 640.25)
add_parameter("api", "Api", 6, 22.3)
end_correlation()

