
begin_correlation("CwSpiveyMN", "WaterIsothermalCompressibility", "mPa_1")
add_parameter("t", "Celsius",
              "Quantity<Fahrenheit>(32)", "Quantity<Fahrenheit>(530)")
add_parameter("p", "mPascal", "Quantity<psia>(14.5)", "Quantity<psia>(29000)")
add_parameter("pb", "mPascal")
add_parameter("s", "Disolved_Salt_Percent", 0, 26)
add_parameter("z", "ZFactor")
end_correlation()
