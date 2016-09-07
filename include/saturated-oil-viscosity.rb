
# coding: utf-8

## verificada con python
begin_correlation("UobBeggsRobinson", "SaturatedOilViscosity", "CP")
add_doc(%Q[    BEGGS & ROBINSON CORRELATION, CALCULATION OF SATURATED OIL VISCOSITY
                
    @see: Beggs H.D. and Robinson J.R.: Estimating the Viscosity of
    Crude Oil Systems. Journal Petroleum Technology (Sep. 1975) 
        
    Based on 2073 live oil observations. Beggs and Robinson developed
    correlations for dead and live oil viscosities with samples obtained
    from 600 oil systems. 

 ])
add_parameter("uod", "CP")
add_parameter("rs", "SCF_STB", 20, 2070)
end_correlation()

################################################################        
