
# coding: utf-8

# verificada con python!
begin_correlation("PoBradley", "OilDensity", "Lb_ft3")
add_doc(%Q[    CALCULATION OF OIL DENSITY

    @see: H.B. Bradley, Petroleum Engineering Handbook.: Capitulo 22"
    Oil System Correlations ".SPE International.

 ])
add_parameter("yg", "Sgg")
add_parameter("rs", "SCF_STB")
add_parameter("bo", "RB_STB")
add_parameter("yo", "Sg_do")
add_parameter("p", "psia")
add_parameter("pb", "psia")
add_parameter("co", "psia_1")
end_correlation()

################################################################        
