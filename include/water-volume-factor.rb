
begin_correlation("BwSpiveyMN", "WaterVolumeFactor", "RB_STB")
add_parameter("t", "Celsius",
              "Quantity<Fahrenheit>(32)", "Quantity<Fahrenheit>(530)")
add_parameter("p", "mPascal", "Quantity<psia>(14.5)", "Quantity<psia>(29000)")
add_parameter("s", "Disolved_Salt_Percent", 0, 26)
end_correlation()

################################################################

begin_correlation("BwMcCain", "WaterVolumeFactor", "RB_STB")
add_parameter("t", "Fahrenheit", 70, 250)
add_parameter("p", "psia", 1000, 5000)
end_correlation()

################################################################

begin_correlation("BwMcCoy", "WaterVolumeFactor", "RB_STB")
add_parameter("t", "Fahrenheit")
add_parameter("p", "psia")
add_parameter("pb", "psia")
add_parameter("s", "Disolved_Salt_Percent")
add_parameter("cw", "psia_1")
end_correlation()
