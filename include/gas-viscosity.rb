
begin_correlation("UgCarrKB", "GasViscosity", "CP")
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
end_correlation()

################################################################

begin_correlation("UgLeeGE", "GasViscosity", "CP")
add_parameter("t", "Rankine",
              "Quantity<Fahrenheit>(100)", "Quantity<Fahrenheit>(340)")
add_parameter("p", "psia", 100, 8000)
add_parameter("yg", "Sgg")
add_parameter("z", "ZFactor")
add_author("Lee et al.")
end_correlation()

################################################################

begin_correlation("UgDeanStiel", "GasViscosity", "CP")
add_author("Dean & Stiel")
add_parameter("t", "Rankine")
add_parameter("p", "psia")
add_parameter("tsc", "Rankine")
add_parameter("psc", "psia")
add_parameter("yg", "Sgg")
add_parameter("z", "ZFactor")
end_correlation()
