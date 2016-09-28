
begin_correlation("PwSpiveyMN", "WaterDensity", "Gr_cm3")
add_parameter("t", "Celsius",
              "Quantity<Fahrenheit>(32)", "Quantity<Fahrenheit>(530)")
add_parameter("pr", "Pascal", "Quantity<psia>(14.5)", "Quantity<psia>(29000)")
add_parameter("s", "Disolved_Salt_PPM", "Quantity<Disolved_Salt_Percent>(0)",
              "Quantity<Disolved_Salt_Percent>(26")
end_correlation()

################################################################

begin_correlation("PwSpiveyMNGasFree", "WaterDensity", "Gr_cm3")
add_parameter("t", "Celsius",
              "Quantity<Fahrenheit>(32)", "Quantity<Fahrenheit>(530)")
add_parameter("pr", "Pascal", "Quantity<psia>(14.5)", "Quantity<psia>(29000)")
add_parameter("s", "Disolved_Salt_PPM", "Quantity<Disolved_Salt_Percent>(0)",
              "Quantity<Disolved_Salt_Percent>(26)")
end_correlation()

################################################################

begin_correlation("PwMcCain", "WaterDensity", "Lb_ft3")
add_parameter("s", "Disolved_Salt_PPM", "Quantity<Disolved_Salt_Percent>(0)",
              "Quantity<Disolved_Salt_Percent>(26)")
add_parameter("bw", "RB_STB")
end_correlation()
