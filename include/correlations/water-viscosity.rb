
begin_correlation("UwMaoDuan", "WaterViscosity", "Paxs")
add_parameter("t", "Kelvin", "Aqui Neylith",  273, 623)
add_parameter("ppw", "Gr_cm3", "Aqui Neylith")
add_parameter("s", "Dissolved_Salt_PPM", "Aqui Neylith",  "Quantity<Dissolved_Salt_Percent>(0)",  "Quantity<Dissolved_Salt_Percent>(26)") 
add_author("Mao & Duan")
end_correlation()

################################################################

begin_correlation("UwVanWingen", "WaterViscosity", "CP", 0.1, 2)
add_parameter("t", "Fahrenheit", "Aqui Neylith",  0,  350)
add_author("Van Wingen")
end_correlation()

################################################################

begin_correlation("UwMatthewsRussel", "WaterViscosity", "CP")
add_parameter("t", "Fahrenheit", "Aqui Neylith",  60,  400)
add_parameter("p", "psia", "Aqui Neylith",  14.7,  10000)
add_parameter("s", "Dissolved_Salt_Percent", "Aqui Neylith",  0,  26)
add_author("Matthews & Russel")
end_correlation()

################################################################

begin_correlation("UwMcCain", "WaterViscosity", "CP")
add_parameter("t", "Fahrenheit", "Aqui Neylith",  100,  400)
add_parameter("p", "psia", "Aqui Neylith",  14.7,  10000)
add_parameter("s", "Dissolved_Salt_Percent", "Aqui Neylith",  0,  26)
add_author("McCain")
end_correlation()

################################################################

begin_correlation("UwMcCoy", "WaterViscosity", "CP")
add_parameter("t",  "Fahrenheit", "Aqui Neylith")
add_parameter("s",  "Dissolved_Salt_Percent", "Aqui Neylith")
add_author("McCoy")
end_correlation()
