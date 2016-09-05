# coding: utf-8

# verificada con python!
begin_correlation("SgoBakerSwerdloff", "OilInterfacialTension", "Lb_ft3")
add_doc(%Q[    BAKER & SWERDLOFF CORRELATION, CALCULATION OF OIL
    INTERFACIAL TENSION
                              
    @see: Baker, O. and Swerdloff, W.: Finding Surface Tension of
    Hydrocarbon Liquids. OGJ Jan 2. 1956. 

    @see: Beggs, H.D.: "Oil System Correlations", Petroleum
    Engineering Handbook, H.C. SPE, Richardson, TX (1987). Cap 22. Pag
    22-6.  

    @see: Bánzer, C.: "Correlaciones Numéricas P.V.T.", Universidad
    del Zulia, 1996.

 ])
add_parameter("yg", "Sgg")
add_parameter("rs", "SCF_STB")
add_parameter("bo", "RB_STB")
add_parameter("yo", "Sg_do")
add_parameter("p", "psia")
add_parameter("pb", "psia")
add_parameter("co", "psia_1")
end_correlation()
