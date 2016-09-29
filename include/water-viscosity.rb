
begin_correlation("UwMaoDuan", "WaterDensity", "Paxs")
add_parameter("t", "Fahrenheit", 32, 662)
add_parameter("p", "mPascal","Quantity<psia>(14.7)", "Quantity<psia>(14500)")
add_parameter("s", "Disolved_Salt_PPM", "Quantity<Disolved_Salt_Percent>(0)",
              "Quantity<Disolved_Salt_Percent>(26)")
end_correlation()

################################################################

begin_correlation("UwVanWingen", "WaterDensity", "CP", 0.1, 2)
add_parameter("t", "Fahrenheit", 0, 350)
end_correlation()
