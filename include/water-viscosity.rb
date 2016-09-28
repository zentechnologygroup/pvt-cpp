
begin_correlation("UwMaoDuan", "WaterDensity", "Paxs")
add_parameter("t", "Fahrenheit", 32, 662)
add_parameter("p", "psia", 14.7, 14500)
add_parameter("s", "Disolved_Salt_PPM", "Quantity<Disolved_Salt_Percent>(0)",
              "Quantity<Disolved_Salt_Percent>(26)")
end_correlation()
