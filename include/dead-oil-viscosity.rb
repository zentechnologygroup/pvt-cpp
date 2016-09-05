# coding: utf-8

## verificada con python
begin_correlation("UodBeal", "DeadOilViscosity", "CP", 0.865, 1550)
add_doc(%Q[     BEAL CORRELATION, CALCULATION OF DEAD OIL VISCOSITY

    Based on 98 samples of gas-free crude oil measured at temperatures
    above 100 °F and 655 samples measured at 100 °F obtained from
    492 different oil fields, of which 358 are in the United
    States.  
        
    @see: Beal, C.: The Viscosity of Air, Water, Natural Gas, Crude
    Oil and Its Associated Gases at Oil Field Temperatures and
    Pressures. Trans., AIME (1946) 165, 94-115. 

    @see: Standing, M.B.: "Volumetric and Phase Behavior of Oil Field
    Hydrocarbon Systems", Society of Petroleum Engineers of AIME,
    Dallas, 1977. 

    @see: Bánzer, C.: "Correlaciones Numéricas P.V.T.", Universidad
    del Zulia, 1996. 

    @note: Beal presented a graphical correlation showing the effects
    of both oil gravity and temperature on dead-oil viscosity. The
    equation is presented by Standing. 

 ])

add_parameter("api", "Api", 10.1, 52.5)
add_parameter("t", "Fahrenheit", 100, 220)
end_correlation()

################################################################

## verificada con python
begin_correlation("UodBeggsRobinson", "DeadOilViscosity", "CP")
add_doc(%Q[     BEGGS & ROBINSON CORRELATION, CALCULATION OF DEAD OIL VISCOSITY

    Based on 460 dead oil observations. Beggs and Robinson developed
    correlations for dead and live oil viscosities with samples
    obtained from 600 oil systems. 
        
    @see: H.D. Beggs y J.R. Robinson. "Estimating the Viscosity of
    Crude Oil Systems," JPT 1140-41, Septiembre 1975.         

 ])

add_parameter("api", "Api", 16, 58)
add_parameter("t", "Fahrenheit", 70, 295)
end_correlation()

################################################################

## verificada con python
begin_correlation("UodEgbogahNg", "DeadOilViscosity", "CP")
add_doc(%Q[     EGBOGAH & NG CORRELATION (MODIFIED BEGGS & ROBINSON
CORRELATION), CALCULATION OF DEAD OIL VISCOSITY

    Based on viscosity data from the Reservoir Fluids Analysis
    Laboratory of AGAT Engineering Utd. The total number of
    oil systems used is 394.   
             
    @see: Egbogah, E.O. and Ng, J.T.H.: An Improved
    Temperature-Viscosity Correlation for Crude Oil Systems. Paper
    83-34-32 presented at the 1983 Annual Technical Meeting of the
    Petroleum Society of CIM. Alberta May 10-13 - 1983.      

 ])

add_parameter("api", "Api", 5, 58)
add_parameter("t", "Fahrenheit", 15, 80)
end_correlation()

################################################################

## verificada con python
begin_correlation("UodGlaso", "DeadOilViscosity", "CP", 0.3, 39.1)
add_doc(%Q[      GLASO CORRELATION, CALCULATION OF DEAD OIL VISCOSITY

    Based on 26 samples from the North Sea (collected from wells in
    the region 56 to 62°N). The relation between dead oil viscosity, API
    gravity, and temperature, was developed specifically for North Sea
    oils (for oils with a paraffinicity characterization factor of
    11.9), which have a factor almost equal to California oils, as well
    as for oil samples from fields in various regions of the world. 
  
    @see: Oistein Glaso. "Generalized Pressure-Volume-Temperature
    Correlations," Journal of Petroleum Technology , 1980. 

    @see: Bánzer, C.: "Correlaciones Numéricas P.V.T.", Universidad
    del Zulia, 1996. 

 ])

add_parameter("api", "Api", 20, 48)
add_parameter("t", "Fahrenheit", 50, 300)
end_correlation()

################################################################

## verificada con python
begin_correlation("UodKartoatmodjoSchmidt", "DeadOilViscosity", "CP", 0.506, 682)
add_doc(%Q[      KARTOATMODJO & SCHMIDT CORRELATION, CALCULATION OF
    DEAD OIL VISCOSITY 
        
    Based on a total of 661 different crude oil samples. The data bank
    was collected from PVT reports and literature.  

    The first major source was from South East Asia, mainly
    Indonesia. The second source was North America, including the
    offshore area. The rest came from the Middle East and Latin
    America. 

    @see: R.S. Kartoatmodjo and Z. Schmidt. "New Correlations For
    Crude Oil Physical Properties", University of Tulsa, 1991. SPE
    23556. 

 ])

add_parameter("api", "Api",  14.4 , 59)
add_parameter("t", "Fahrenheit", 80, 320)
end_correlation()
