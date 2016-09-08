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
