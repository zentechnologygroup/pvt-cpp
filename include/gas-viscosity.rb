
begin_correlation("UgCarrKB", "GasViscosity", "CP")
<<<<<<< HEAD
add_parameter("t", "Rankine",
              "Quantity<Fahrenheit>(100)", "Quantity<Fahrenheit>(300)")
add_parameter("p", "psia")
add_parameter("tsc", "Rankine")
add_parameter("psc", "psia")
add_parameter("yg", "Sgg", 0.55, 1.55)
add_parameter("n2_concentration", "MolePercent")
add_parameter("co2_concentration", "MolePercent")
add_parameter("h2s_concentration", "MolePercent")
add_author("Carr et al.")
=======
add_parameter("t", "Rankine", "Aqui Neylith",  "Quantity<Fahrenheit>(100)",  "Quantity<Fahrenheit>(300)") 
add_parameter("p",  "psia", "Aqui Neylith")
add_parameter("tsc",  "Rankine", "Aqui Neylith")
add_parameter("psc",  "psia", "Aqui Neylith")
add_parameter("yg", "Sgg", "Aqui Neylith",  0.55,  1.55)
add_parameter("n2_concentration",  "MolePercent", "Aqui Neylith")
add_parameter("co2_concentration",  "MolePercent", "Aqui Neylith")
add_parameter("h2s_concentration",  "MolePercent", "Aqui Neylith")
>>>>>>> 93f903d2b033c387105a9a25568497e787e57306
end_correlation()

################################################################

begin_correlation("UgLeeGE", "GasViscosity", "CP")
<<<<<<< HEAD
add_parameter("t", "Rankine",
              "Quantity<Fahrenheit>(100)", "Quantity<Fahrenheit>(340)")
add_parameter("p", "psia", 100, 8000)
add_parameter("yg", "Sgg")
add_parameter("z", "ZFactor")
add_author("Lee et al.")
=======
add_parameter("t", "Rankine", "Aqui Neylith",  "Quantity<Fahrenheit>(100)",  "Quantity<Fahrenheit>(340)") 
add_parameter("p", "psia", "Aqui Neylith",  100,  8000)
add_parameter("yg",  "Sgg", "Aqui Neylith")
add_parameter("z",  "ZFactor", "Aqui Neylith")
>>>>>>> 93f903d2b033c387105a9a25568497e787e57306
end_correlation()

################################################################

begin_correlation("UgDeanStiel", "GasViscosity", "CP")
add_author("Dean & Stiel")
add_parameter("t",  "Rankine", "Aqui Neylith")
add_parameter("p",  "psia", "Aqui Neylith")
add_parameter("tsc",  "Rankine", "Aqui Neylith")
add_parameter("psc",  "psia", "Aqui Neylith")
add_parameter("yg",  "Sgg", "Aqui Neylith")
add_parameter("z",  "ZFactor", "Aqui Neylith")
end_correlation()
