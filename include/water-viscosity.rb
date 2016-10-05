
begin_correlation("UwMaoDuan", "WaterDensity", "Paxs")
add_parameter("t", "Fahrenheit", "Aqui Neylith",  32,  662)
add_parameter("p", "mPascal", "Aqui Neylith", "Quantity<psia>(14.7)",  "Quantity<psia>(14500)")
add_parameter("s", "Disolved_Salt_PPM", "Aqui Neylith",  "Quantity<Disolved_Salt_Percent>(0)",  "Quantity<Disolved_Salt_Percent>(26)") 
add_author("Mao & Duan")
end_correlation()

################################################################

begin_correlation("UwVanWingen", "WaterDensity", "CP", 0.1, 2)
add_parameter("t", "Fahrenheit", "Aqui Neylith",  0,  350)
add_author("Van Wingen")
end_correlation()

################################################################

begin_correlation("UwMatthewsRussel", "WaterDensity", "CP")
add_parameter("t", "Fahrenheit", "Aqui Neylith",  60,  400)
add_parameter("p", "psia", "Aqui Neylith",  14.7,  10000)
add_parameter("s", "Disolved_Salt_Percent", "Aqui Neylith",  0,  26)
add_author("Matthews & Russel")
end_correlation()

################################################################

begin_correlation("UwMcCain", "WaterDensity", "CP")
add_parameter("t", "Fahrenheit", "Aqui Neylith",  100,  400)
add_parameter("p", "psia", "Aqui Neylith",  14.7,  10000)
add_parameter("s", "Disolved_Salt_Percent", "Aqui Neylith",  0,  26)
add_author("McCain")
end_correlation()

################################################################

begin_correlation("UwMcCoy", "WaterDensity", "CP")
add_parameter("t",  "Fahrenheit", "Aqui Neylith")
add_parameter("s",  "Disolved_Salt_Percent", "Aqui Neylith")
add_author("McCoy")
end_correlation()
