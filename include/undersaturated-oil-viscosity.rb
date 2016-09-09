# coding: utf-8

## verificada con python
begin_correlation("UoaKartoatmodjoSchmidt", "UndersaturatedOilViscosity", "CP",
                 0.168, 517.03)
add_doc(%Q[    KARTOATMODJO & SCHMIDT CORRELATION FOR HEAVY, MEDIUM
    AND LIGHT OILS, CALCULATION OF UNDERSATURATED OIL VISCOSITY

    Based on a total of 3588 data points collected from 661 different
    crude oil samples. 

    The first major source was from South East Asia, mainly
    Indonesia. The second source was North America, including the
    offshore area. The rest came from the Middle East and Latin
    America. 

    Depending on the API gravity, the samples can cover three
    different classes of crude oils: heavy oils for
    10<°API<=22.3, medium oils for 22.3<°API<=31.1, and
    light oils for °API>31.1. 
                
    @see: Kartoatmodjo, R.S. and Schmidt, Z.: "New Correlations For
    Crude Oil Physical Properties", University of Tulsa, 1991. SPE 23556.  
   
 ])
add_parameter("uob", "CP", 0.168, 184.86)
add_parameter("pb", "psia", 24.7, 4764.7)
add_parameter("p", "psia", 24.7, 6014.7)
end_correlation()

################################################################        

## verificada con python
begin_correlation("UoaDeGhettoEtAl", "UndersaturatedOilViscosity",
                  "CP", 2.4, 354.6)
add_doc(%Q[    DE GHETTO ET AL. CORRELATION FOR EXTRA-HEAVY (MODIFIED
    LABEDI CORRELATION) AND HEAVY (MODIFIED KARTOATMODJO & SCHMIDT
    CORRELATION) OILS, CALCULATION OF UNDERSATURATED OIL VISCOSITY 
                              
    Based on 1200 measured data points of 63 heavy and extra-heavy oil
    samples obtained from the Mediterranean Basin, Africa and the Persian Gulf. 

    Oil samples have been divided in two different API gravity
    classes: extra-heavy oils for °API<=10, heavy oils for
    10<°API<=22.3. 
 
    @see: De Ghetto, G., Paone, F., and Villa, M.:
    "Pressure-Volume-Temperature Correlations for Heavy
    and Extra Heavy Oils", Canada, 1995. SPE 30316.  
    
 ])
add_parameter("uob", "CP", 2.1, 295.9)
add_parameter("p", "psia", 1038.49, 7411.54)
add_parameter("pb", "psia", 1038.49, 7411.54)
add_parameter("uod", "CP", 7.7, 1386.9)
add_parameter("api", "Api", 6, 22.3)
end_correlation()


################################################################

## verificada con python
begin_correlation("UoaBeal", "UndersaturatedOilViscosity", "CP", 0.16, 315)
add_doc(%Q[    BEAL ET AL. CORRELATION, CALCULATION OF UNDERSATURATED OIL VISCOSITY
                              
    @see: Beal, C.: The Viscosity of Air, Water, Natural Gas, Crude
    Oil and Its Associated Gases at Oil Field Temperatures and
    Pressures. Trans., AIME (1946), 94-115.  

    @see: Standing, M.B.: "Volumetric and Phase Behavior of Oil Field
    Hydrocarbon Systems", Society of Petroleum Engineers of AIME,
    Dallas, 1977. 

    @see: Bánzer, C.: "Correlaciones Numéricas P.V.T.", Universidad
    del Zulia, 1996. 
 
    Based on 52 viscosity observations taken from 26 crude oil
    samples, representing 20 individual oil pools, 11 of which are in
    California.  

    Half of the observations include viscosity data for the
    undersaturated crude oil above the bubble point, and the other half
    are viscosity observations at the bubble point. 
   
    @note: Beal developed the graphical correlation and Standing
    presented the equation 
 ])
add_parameter("uob", "CP", 0.142, 127)
add_parameter("p", "psia", 1500, 5500)
add_parameter("pb", "psia", 125, 4120)
end_correlation()

################################################################

## verificada con python
begin_correlation("UoaVasquezBeggs", "UndersaturatedOilViscosity", "CP")
add_doc(%Q[    VASQUEZ & BEGGS CORRELATION, CALCULATION OF
    UNDERSATURATED OIL VISCOSITY 
                
    @see: Vasquez, M.E. and Beggs, H.D.: "Correlations for Fluid
    Physical Property Prediction", JPT 968-70, June 1980. 
        
    Based on more than 600 laboratory PVT analyses from fields all
    over the world, including more than 6000 measurements of
    undersaturated oil viscosity at various pressures. 
        
    @note: The development ranges are presented by Bánzer, who does
    not specify if the range for viscosity corresponds to uoBubble or uoa.  
   
 ])
add_parameter("uob", "CP", 0.117, 148)
add_parameter("p", "psia", 141, 9515)
add_parameter("pb", "psia")
end_correlation()

################################################################

## verificada con python
begin_correlation("UoaKhan", "UndersaturatedOilViscosity", "CP", 0.13, 71.0)
add_doc(%Q[    KHAN ET AL. CORRELATION, CALCULATION OF UNDERSATURATED OIL VISCOSITY
                
    @see: Khan, S.A., Al-Marhoun, M.A., Duffuaa, S.O., and
    Abu-Khamsin, S.A.: "Viscosity Correlations for Saudi Arabian Crude
    Oils", SPE 15720, 1987. 
        
    Based on 75 bottom hole samples taken from 62 Saudi Arabian oil reservoirs.

    A total of 1503 data points were used for oil viscosity above the
   bubble point pressure. 
   
 ])
add_parameter("uob", "CP", 0.13, 17.9)
add_parameter("p", "psia", 4.7, 5015)
add_parameter("pb", "psia", 4.7, 5015)

end_correlation()

################################################################

## verificada con python
begin_correlation("UoaPetroskyFarshad", "UndersaturatedOilViscosity", "CP",
                  0.224, 4.090)
add_doc(%Q[    PETROSKY & FARSHAD CORRELATION, CALCULATION OF
    UNDERSATURATED OIL VISCOSITY 
                              
    @see: Petrosky, G.E. and Farshad, F.F.: "Viscosity Correlations
    for Golf of Mexico Crude Oils", April 1995. SPE 29468. 

    Based on a set of 404 data points from 126 laboratory PVT analyses
    of Gulf of Mexico crude oils. For the ranges of carbon dioxide and
    nitrogen concentration in the mixture, 126 data points were
    considered. 

    Fluid samples were obtained from reservoirs located offshore Texas
    and Louisiana. Producing areas from Galveston Island, eastward,
    through Main Pass are represented. 
   
 ])
add_parameter("uob", "CP", 0.211, 3.546)
add_parameter("p", "psia", 1600, 10250)
add_parameter("pb", "psia", 1574, 9552)
end_correlation()

################################################################

## verificada con python
begin_correlation("UoaAbedini", "UndersaturatedOilViscosity", "CP", 0.316, 18.435)
add_doc(%Q[    ABEDINI CORRELATION, CALCULATION OF UNDERSATURATED OIL VISCOSITY  
                
    @see: R. Abedini, A. Abedini, N. Eslami Yakhfrozan, "A New
    Correlation for Prediction of Undersaturated Crude Oil Viscosity",
    Petroleum & Coal 52(1) 50-55, 2010. 
        
    Based on PVT experimental data of five sample oils from Iranian
    oil reservoirs. 
   
 ])
add_parameter("uob", "CP")
add_parameter("p", "psia", )
add_parameter("pb", "psia", 1340, 4513)
end_correlation()

################################################################

begin_correlation("UoaPerezML", "UndersaturatedOilViscosity", "CP")
add_doc(%Q[    PÉREZ ET AL. CORRELATION (MODIFIED LABEDI CORRELATION),
    CALCULATION OF UNDERSATURATED OIL VISCOSITY 
                
    @see: C.A. Heny, V.L. Pérez, M.E. Lago, 2001: Evaluación y
    generación de correlaciones PVT para crudos extrapesados
    venezolanos. Informe Técnico INT-8713,2001. Proyecto: Estudio y
    Modelaje de Procesos de Recuperación de Crudos Pesados. Los Teques,
    Octubre de 2001 
   
 ])
add_parameter("uob", "CP")
add_parameter("p", "psia")
add_parameter("pb", "psia", 405, 1335)
end_correlation()
