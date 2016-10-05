
begin_correlation("BwSpiveyMN", "WaterVolumeFactor", "RB_STB")
add_parameter("t", "Celsius", "Aqui Neylith",  "Quantity<Fahrenheit>(32)",  "Quantity<Fahrenheit>(530)") 
add_parameter("p", "mPascal", "Aqui Neylith",  "Quantity<psia>(14.5)",  "Quantity<psia>(29000)")
add_parameter("s", "Disolved_Salt_Percent", "Aqui Neylith",  0,  26)
add_author("Spivey et al.")
end_correlation()

################################################################

begin_correlation("BwMcCain", "WaterVolumeFactor", "RB_STB")
add_parameter("t", "Fahrenheit", "Aqui Neylith",  70,  250)
add_parameter("p", "psia", "Aqui Neylith",  1000,  5000)
add_author("McCain")
end_correlation()

################################################################

begin_correlation("BwMcCoy", "WaterVolumeFactor", "RB_STB")
add_parameter("t",  "Fahrenheit", "Aqui Neylith")
add_parameter("p",  "psia", "Aqui Neylith")
add_parameter("pb",  "psia", "Aqui Neylith")
add_parameter("s",  "Disolved_Salt_Percent", "Aqui Neylith")
add_parameter("cw",  "psia_1", "Aqui Neylith")
add_author("McCoy")
end_correlation()
