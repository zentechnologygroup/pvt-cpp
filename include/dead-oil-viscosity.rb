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
add_db("Based on 98 samples of gas-free crude oil measured at temperatures above 100 degF and 655 samples measured at 100 degF obtained from 492 different oil fields, of which 358 are in the United States")
add_note(" Beal presented a graphical correlation showing the effects of both oil gravity and temperature on dead-oil viscosity. The equation is presented by Standing")
add_ref("banzer:1996")
add_eq("\\mu_{od}\\,=\\,{\\left[{0.32}+{\\frac{1.8\\cdot{10^{7}}}{{\\gamma_{API}}^{4.53}}} \\right]}\\cdot{{\\left[\\frac{360}{T+200}\\right]}^{a}}")
add_eq("a\\,=\\,antilog{\\left({0.43}+{\\frac{8.33}{\\gamma_{API}}}\\right)}")
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

################################################################

begin_correlation("UodSattarinEtAl", "DeadOilViscosity", "CP", 2, 570)
add_doc(%Q[     SATTARIN ET AL. CORRELATION, CALCULATION OF DEAD OIL VISCOSITY

    Based on 438 data points from Iranian off and on-shore dead
    crude oil samples, collected in Crude Oil Evaluation
    department at RIPI. Standard test method of ASTM D-445 was
    used for kinematic viscosity measurements. 

    Data were divided into two major groups, depending on the API
    gravity. The first group included 85 Iranian heavy crude oil
    (17<=°API<28) data-points and the second one contained 353 Iranian
    light crude oil (28<=°API<45) data-points. 

    @see: Sattarin et al. New Viscosity Correlations for Dead Crude
    Oils. Petroleum & Coal ISSN 1335-3055. 2007.         

 ])

add_parameter("api", "Api", 17, 45)
add_parameter("t", "Kelvin", "Quantity<Celsius>(9.85)", "Quantity<Celsius>(40)")
end_correlation()

################################################################

## verificada con python
begin_correlation("UodNaseri", "DeadOilViscosity", "CP", 0.75, 54)
add_doc(%Q[     NASERI ET AL. CORRELATION, CALCULATION OF DEAD OIL VISCOSITY
        
    Based on PVT experimental data of 472 series of Iranian oil
    reservoirs. About 250 series of PVT and viscosity data have been used
    in developing a new empirical correlation.   

    Validity and accuracy of the proposed correlations (dead oil
    viscosity, saturated and undersaturated viscosity) have been
    checked by their application to 222 samples. 

    @see: Naseri et al. A Correlation Approach for Prediction of Crude
    Oil Viscosities. Journal of Petroleum Science and Engineering 47
    (2005) 163-174. Science Direct. 

 ])

add_parameter("api", "Api", 17, 44)
add_parameter("t", "Fahrenheit", 105, 295)
end_correlation()

################################################################

## verificada con python
begin_correlation("UodPetroskyFarshad", "DeadOilViscosity", "CP", 0.725, 10.249)
add_doc(%Q[     PETROSKY & FARSHAD CORRELATION, CALCULATION OF DEAD OIL VISCOSITY
        
    Based on a set of 118 data points from 126 laboratory PVT analyses
    of Gulf of Mexico crude oils. 

    Fluid samples were obtained from reservoirs located offshore Texas
    and Louisiana. Producing areas from Galveston Island, eastward,
    through Main Pass are represented. 

    @see: Petrosky, G.E. Jr., and Farshad F.F.: Viscosity Correlations
    for Gulf of Mexico Crude Oils. Production Operation Symposium,
    Oklahoma City, 1995. Society of Petroleum Engineers paper - SPE
    29468. 

 ])

add_parameter("api", "Api",  25.4, 46.1)
add_parameter("t", "Fahrenheit", 114, 288)
end_correlation()

################################################################

## verificada con python
begin_correlation("UodDeGhettoEtAl", "DeadOilViscosity", "CP",  7.7, 1386.9)
add_doc(%Q[     DE GHETTO ET AL. CORRELATION FOR EXTRA-HEAVY AND HEAVY
    OILS (MODIFIED EGBOGAH-JACK CORRELATION), CALCULATION OF DEAD OIL
    VISCOSITY 

    Based on 1200 measured data points of 63 heavy and extra-heavy oil
    samples obtained from the Mediterranean Basin, Africa and the
    Persian Gulf. 

    Oil samples have been divided in two different API gravity
    classes: extra-heavy oils for °API<=10, heavy oils for
    10<°API<=22.3. 
 
    @see: De Ghetto, G., Paone, F., and Villa, M.:
    "Pressure-Volume-Temperature Correlations for Heavy and Extra
    Heavy Oils", Canada, 1995. SPE 30316.  

 ])

add_parameter("api", "Api", 6, 22.3)
add_parameter("t", "Fahrenheit", 131.4, 250.7)
end_correlation()

################################################################

## verificada con python
begin_correlation("UodPerezML", "DeadOilViscosity", "CP")
add_doc(%Q[     PEREZ ET AL. CORRELATION (MODIFIED BEGGS & ROBINSON
    CORRELATION), CALCULATION OF DEAD OIL VISCOSITY 
                      
    @see: C.A. Heny, V.L. Pérez, M.E. Lago, 2001: Evaluación y
    generación de correlaciones PVT para crudos extrapesados
    venezolanos. Informe Técnico INT-8713,2001. Proyecto: Estudio y
    Modelaje de Procesos de Recuperación de Crudos Pesados. Los Teques,
    Octubre de 2001 

 ])

add_parameter("api", "Api", 6.4, 12.9)
add_parameter("t", "Fahrenheit", 112, 300)
end_correlation()
