
begin_correlation("CwSpiveyMN", "WaterIsothermalCompressibility", "mPa_1")
add_parameter("t", "Celsius", "Aqui Neylith",  "Quantity<Fahrenheit>(32)",  "Quantity<Fahrenheit>(530)") 
add_parameter("p", "mPascal", "Aqui Neylith",  "Quantity<psia>(14.5)",  "Quantity<psia>(29000)")
add_parameter("pb",  "mPascal", "Aqui Neylith")
add_parameter("s", "Disolved_Salt_Percent", "Aqui Neylith",  0,  26)
add_parameter("z",  "ZFactor", "Aqui Neylith")
add_author("Spivey")
end_correlation()

################################################################

begin_correlation("CwDodsonStanding", "WaterIsothermalCompressibility",
                  "psia_1")
add_parameter("t", "Fahrenheit", "Aqui Neylith", 100, 250) 
add_parameter("p", "psia", "Aqui Neylith", 500, 5000)
add_parameter("pb",  "psia", "Aqui Neylith")
add_parameter("rsw", "SCF_STB", "Aqui Neylith")
add_parameter("rswb", "SCF_STB", "Aqui Neylith")
add_parameter("s", "Disolved_Salt_Percent", "Aqui Neylith",  0, 3.5)
add_parameter("bg", "RCF_SCF", "Aqui Neylith")
add_parameter("bw", "RB_STB", "Aqui Neylith")
end_correlation()

################################################################

begin_correlation("CwOsif", "WaterIsothermalCompressibility", "psia_1")
add_parameter("t", "Fahrenheit", "Aqui Neylith", 200, 270) 
add_parameter("p", "psia", "Aqui Neylith", 1000, 20000)
add_parameter("pb",  "psia", "Aqui Neylith")
add_parameter("s", "Disolved_Salt_Percent", "Aqui Neylith",  0, 20)
add_parameter("bg", "RCF_SCF", "Aqui Neylith")
add_parameter("bw", "RB_STB", "Aqui Neylith")
end_correlation()
