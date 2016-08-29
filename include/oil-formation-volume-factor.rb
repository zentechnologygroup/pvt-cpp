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

