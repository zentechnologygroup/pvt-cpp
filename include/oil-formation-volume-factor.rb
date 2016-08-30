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

