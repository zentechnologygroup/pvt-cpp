
begin_correlation("CwSpiveyMN", "WaterIsothermalCompressibility", "mPa_1")
add_parameter("t", "Celsius", "Aqui Neylith",  "Quantity<Fahrenheit>(32)",  "Quantity<Fahrenheit>(530)") 
add_parameter("p", "mPascal", "Aqui Neylith",  "Quantity<psia>(14.5)",  "Quantity<psia>(29000)")
add_parameter("pb",  "mPascal", "Aqui Neylith")
add_parameter("s", "Disolved_Salt_Percent", "Aqui Neylith",  0,  26)
add_parameter("z",  "ZFactor", "Aqui Neylith")
add_author("Spivey")
end_correlation()
