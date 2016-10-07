
begin_correlation("UgCarrKB", "GasViscosity", "CP")
add_parameter("t", "Rankine", "Aqui Neylith",  "Quantity<Fahrenheit>(100)",  "Quantity<Fahrenheit>(300)") 
add_parameter("p",  "psia", "Aqui Neylith")
add_parameter("tsc",  "Rankine", "Aqui Neylith")
add_parameter("psc",  "psia", "Aqui Neylith")
add_parameter("yg", "Sgg", "Aqui Neylith",  0.55,  1.55)
add_parameter("n2_concentration",  "MolePercent", "Aqui Neylith")
add_parameter("co2_concentration",  "MolePercent", "Aqui Neylith")
add_parameter("h2s_concentration",  "MolePercent", "Aqui Neylith")
add_author("Carr et al.")
end_correlation()

################################################################

begin_correlation("UgLeeGE", "GasViscosity", "CP")
add_parameter("t", "Rankine", "Aqui Neylith",  "Quantity<Fahrenheit>(100)",  "Quantity<Fahrenheit>(340)") 
add_parameter("p", "psia", "Aqui Neylith",  100,  8000)
add_parameter("yg",  "Sgg", "Aqui Neylith")
add_parameter("z",  "ZFactor", "Aqui Neylith")
add_author("Lee et al.")
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
