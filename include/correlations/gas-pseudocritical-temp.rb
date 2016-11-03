# coding: utf-8

begin_correlation("TscHCStanding", "GasPseudoCriticalTemperature", "Rankine",
                  345, 670)
add_title("STANDING CORRELATION, CALCULATION OF PSEUDOCRITICAL TEMPERATURE OF THE DRY GAS HYDROCARBON PORTION")
add_db("Based on 71 natural gases from California, which represent surface trap gases and stock-tank vapors whose high gravity results from relatively high proportions of ethane, propane, and butanes.")
add_db("Standing recommends using the \\\"California\\\" curves for surface gases.")
add_note('Brown et al. (1948) presented the graphical correlation. Standing (1977) expressed it in a mathematical form.')
add_internal_note('The values of the development ranges are taken from the graphs presented by Standing (1977) and Ahmed (1989).')
add_note("Standing (1977) presented two pressure correlations: one for \\\"condensate\\\" gases and one for \\\"California\\\" gases. Whitson & Brulé (2000) explained that the correlation for \\\"condensate\\\" gases applies to wet or high molecular weight gases, and the other one corresponds to dry hydrocarbon gases.")
add_internal_note("Brown et al.'s reference is not available. The correlation was verified by using these references: Standing (1977), Ahmed (1989), and Bánzer (1996). Date: September 14 2016.")
add_ref('standing:1977')
add_ref('brown:1948')
add_ref('ahmed:1989')
add_ref('banzer:1996')
add_ref("whitson:2000")
add_parameter('yghc', 'Sgg', "Gas gravity of the hydrocarbon portion",  0.55,  1.67)
add_parameter('n2_concentration', 'MoleFraction', "N2 concentration",  "Quantity<MolePercent>(0)",  "Quantity<MolePercent>(5)") 
add_parameter('co2_concentration', 'MoleFraction', "CO2 concentration",  "Quantity<MolePercent>(0)",  "Quantity<MolePercent>(2)") 
add_parameter('h2s_concentration', 'MoleFraction', "H2S concentration",  "Quantity<MolePercent>(0)",  "Quantity<MolePercent>(2)") 
add_author("Standing")
end_correlation()

################################################################

begin_correlation("TscHCStandingHeavierFractions",
                  "GasPseudoCriticalTemperature", "Rankine", 350, 495)
add_title("STANDING CORRELATION, CALCULATION OF PSEUDOCRITICAL TEMPERATURE OF THE WET OR \\\"CONDENSATE\\\" GAS HYDROCARBON PORTION")
add_db("Based on laboratory generated gases in equilibrium with crude oil at high pressures (1000 to 8000 psia). Their high gravities come from relatively large quantities of heptanes and heavier compounds.")
add_db("Standing recommends using the \\\"condensate\\\" curves in calculations that involve gases in equilibrium with crude oil condensate in petroleum reservoirs.")
add_note('Brown et al. (1948) presented the graphical correlation. Standing (1977) expressed it in a mathematical form.')
add_internal_note('The values of the development ranges are taken from the graphs presented by Standing (1977) and Ahmed (1989).')
add_note("Standing (1977) presented two pressure correlations: one for \\\"condensate\\\" gases and one for \\\"California\\\" gases. Whitson & Brulé (2000) explained that the correlation for \\\"condensate\\\" gases applies to wet or high molecular weight gases, and the other one corresponds to dry hydrocarbon gases.")
add_internal_note("Brown et al.'s reference is not available. The correlation was verified by using these references: Standing (1977), Ahmed (1989), and Bánzer (1996). Date: September 14 2016.")
add_ref('standing:1977')
add_ref('brown:1948')
add_ref('ahmed:1989')
add_ref('banzer:1996')
add_parameter('yghc', 'Sgg', "Gas gravity of the hydrocarbon portion",  0.58,  1.3)
add_parameter('n2_concentration', 'MoleFraction', "N2 concentration",  "Quantity<MolePercent>(0)",  "Quantity<MolePercent>(5)") 
add_parameter('co2_concentration', 'MoleFraction', "CO2 concentration",  "Quantity<MolePercent>(0)",  "Quantity<MolePercent>(2)") 
add_parameter('h2s_concentration', 'MoleFraction', "H2S concentration",  "Quantity<MolePercent>(0)",  "Quantity<MolePercent>(2)") 
add_author("Standing (Heavier Fractions)")
end_correlation()

################################################################

begin_correlation("TscHCSutton",
                  "GasPseudoCriticalTemperature", "Rankine", 330 , 575)
add_title("SUTTON CORRELATION, CALCULATION OF PSEUDOCRITICAL TEMPERATURE OF THE NATURAL GAS HYDROCARBON PORTION")
add_db("Based on 264 samples of natural gases with high molecular weight, rich in heptanes, with minor amounts of nitrogen and carbon dioxide, and without hydrogen sulfide.")
add_db("These 264 compositions were obtained from a data bank of 275 PVT reports and 634 compositions in total. The producing areas represented by this data bank are: Gulf of Mexico, Lousiana, Texas, and others.")
add_internal_note("The ranges for Yg, n2Concentration, and co2Concentration, are presented as the ranges of data covered by the total number of PVT reports. The range for Psc was taken from the graph of the variation of pseudocritical pressure with gas gravity, presented by Sutton.")
add_note("Sutton's correlation was developed based on samples of gases with no hydrogen sulfide content.")
add_ref('sutton:1985')
add_parameter('yghc', 'Sgg', "Gas gravity of the hydrocarbon portion",  0.571,  1.679)
add_parameter('n2_concentration', 'MoleFraction', "N2 concentration",  "Quantity<MolePercent>(0)",  "Quantity<MolePercent>(2.86)") 
add_parameter('co2_concentration', 'MoleFraction', "CO2 concentration",  "Quantity<MolePercent>(0.01)",  "Quantity<MolePercent>(11.86)") 
add_parameter('h2s_concentration', 'MoleFraction', "H2S concentration",  "Quantity<MolePercent>(0)",  "Quantity<MolePercent>(0)") 
add_author("Sutton")
end_correlation()

################################################################

begin_correlation("TpcHCElsharkawyEtAl",
                  "GasPseudoCriticalTemperature", "Rankine")
add_title("ELSHARKAWY ET AL. CORRELATION, CALCULATION OF PSEUDOCRITICAL TEMPERATURE OF THE CONDENSATE GAS HYDROCARBON PORTION")
add_db("Based on compositional analysis of 1200 compositions of gas condensates collected worldwide.")
add_ref('elsharkawy:2000:1')
add_ref('elsharkawy:2000:2')
add_parameter("yghc", "Sgg", "Gas gravity of the hydrocarbon portion",  0.61,  1.89)
add_parameter('n2_concentration',  'MoleFraction', "N2 concentration")
add_parameter('co2_concentration',  'MoleFraction', "CO2 concentration")
add_parameter('h2s_concentration',  'MoleFraction', "H2S concentration")
add_internal_note("The correlation was verified by using the original reference and a secondary one: Elsharkawy & Elkamel (2000). Date: September 22 2016.")
add_author("Elsharkawy et al.")
end_correlation()

