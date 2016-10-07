
begin_correlation("PwSpiveyMN", "WaterDensity", "Gr_cm3")
add_parameter("t", "Celsius", "Aqui Neylith",  "Quantity<Fahrenheit>(32)",  "Quantity<Fahrenheit>(530)") 
add_parameter("pr", "Pascal", "Aqui Neylith",  "Quantity<psia>(14.5)",  "Quantity<psia>(29000)")
add_parameter("s", "Disolved_Salt_Percent", "Aqui Neylith", 0, 26) 
add_author("Spivey et al.")
end_correlation()

################################################################

begin_correlation("PwSpiveyMNGasFree", "WaterDensity", "Gr_cm3")
add_parameter("t", "Celsius", "Aqui Neylith",  "Quantity<Fahrenheit>(32)",  "Quantity<Fahrenheit>(530)") 
add_parameter("pr", "Pascal", "Aqui Neylith",  "Quantity<psia>(14.5)",  "Quantity<psia>(29000)")
add_parameter("s", "Disolved_Salt_Percent", "Aqui Neylith", 0, 26)
add_author("Spivey et al. (free Gas)")
end_correlation()

################################################################

begin_correlation("PwMcCain", "WaterDensity", "Lb_ft3")
add_parameter("s", "Disolved_Salt_Percent", "Aqui Neylith", 0, 26) 
add_parameter("bw",  "RB_STB", "Aqui Neylith")
add_author("McCain")
end_correlation()
