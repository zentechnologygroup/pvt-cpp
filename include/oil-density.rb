
# coding: utf-8

# verificada con python!
begin_correlation("PoBradley", "OilDensity", "Lb_ft3")
add_doc(%Q[    CALCULATION OF OIL DENSITY

    @see: H.B. Bradley, Petroleum Engineering Handbook.: Capitulo 22"
    Oil System Correlations ".SPE International.

 ])
add_parameter("yg",  "Sgg", "Aqui Neylith")
add_parameter("rs",  "SCF_STB", "Aqui Neylith")
add_parameter("bo",  "RB_STB", "Aqui Neylith")
add_parameter("yo",  "Sg_do", "Aqui Neylith")
add_parameter("p",  "psia", "Aqui Neylith")
add_parameter("pb",  "psia", "Aqui Neylith")
add_parameter("co",  "psia_1", "Aqui Neylith")
add_author("Bradley")
end_correlation()

################################################################        
