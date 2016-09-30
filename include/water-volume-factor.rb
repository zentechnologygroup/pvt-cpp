
begin_correlation("BwSpiveyMN", "WaterVolumeFactor", "RB_STB")
add_parameter("t", "Celsius",
              "Quantity<Fahrenheit>(32)", "Quantity<Fahrenheit>(530)")
add_parameter("p", "mPascal", "Quantity<psia>(14.5)", "Quantity<psia>(29000)")
add_parameter("s", "Disolved_Salt_Percent", 0, 26)
end_correlation()
