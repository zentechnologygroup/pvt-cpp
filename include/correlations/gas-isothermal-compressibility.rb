
begin_correlation("CgSarem", "GasIsothermalCompressibility", "psia_1")
add_title("SAREM FIT METHOD, CALCULATION OF GAS ISOTHERMAL COMPRESSIBILITY")
add_parameter("tpr",  "PseudoReducedTemperature", "Gas pseudoreduced temperature", 1.05, 2.95)
add_parameter("ppr",  "PseudoReducedPressure", "Gas pseudoreduced pressure", 0.1, 14.9)
add_parameter("ppc",  "psia", "Gas pseudocritical pressure")
add_parameter("z",  "ZFactor", "Gas compressibility factor")
add_db("The correlation is based on the partial derivative of the Z-factor equation with respect to the pseudoreduced pressure.")
add_db("The Z-factor equation was obtained by using Legendre polynomials of up to five degree to fit the Standing-Katz curves.")
add_ref("sarem:1961")
add_ref("standing:1942")
add_ref("banzer:1996")
add_author("Sarem")
add_internal_note("The original reference is not available. The correlation was verified by using a secondary reference: Bánzer (1996). Date: September 26 2016.")
end_correlation()

################################################################

begin_correlation("CgHallYarborough", "GasIsothermalCompressibility", "psia_1")
add_title("HALL & YARBOROUGH FIT METHOD, CALCULATION OF GAS ISOTHERMAL COMPRESSIBILITY")
add_parameter("tpr",  "PseudoReducedTemperature", "Gas pseudoreduced temperature", 1.2, 3.0)
add_parameter("ppr",  "PseudoReducedPressure", "Gas pseudoreduced pressure", 0.1, 24.0)
add_parameter("ppc",  "psia", "Gas pseudocritical pressure")
add_parameter("z",  "ZFactor", "Gas compressibility factor")
add_author("Hall & Yarborough")
add_db("The correlation is based on the partial derivative of the Z-factor equation with respect to the pseudoreduced pressure.")
add_db("The Z-factor correlation is based on the equation of state of Starling-Carnahan. The coefficients of the equation were obtained by fitting the Standing-Katz curves.")
add_ref("hall:1973")
add_ref("yarborough:1974")
add_ref("standing:1942")
add_ref("banzer:1996")
add_db("The equation is solved by using the Newton-Raphson iteration method.")
add_internal_note("The original reference is not available. The correlation was verified by using a secondary reference: Bánzer (1996). Date: September 27 2016.")
add_internal_note("The application ranges and data bank information was obtained from Takacs (1989).")
end_correlation()

################################################################

begin_correlation("CgMattarBA", "GasIsothermalCompressibility", "psia_1")
add_title("MATTAR, BRAR & AZIZ FIT METHOD, CALCULATION OF GAS ISOTHERMAL COMPRESSIBILITY")
add_parameter("tpr",  "PseudoReducedTemperature", "Gas pseudoreduced temperature")
add_parameter("ppr",  "PseudoReducedPressure", "Gas pseudoreduced pressure")
add_parameter("ppc",  "psia", "Gas pseudocritical pressure")
add_parameter("z",  "ZFactor", "Gas compressibility factor")
add_author("Mattar, Brar & Aziz")
end_correlation()

################################################################

begin_correlation("CgGopal", "GasIsothermalCompressibility", "psia_1")
add_parameter("tpr",  "PseudoReducedTemperature", "Gas pseudoreduced temperature")
add_parameter("ppr",  "PseudoReducedPressure", "Gas pseudoreduced pressure")
add_parameter("ppc",  "psia", "Gas pseudocritical pressure")
add_parameter("z",  "ZFactor", "Gas compressibility factor")
add_author("Gopal")
end_correlation()

################################################################

begin_correlation("CgBrillBeggs", "GasIsothermalCompressibility", "psia_1")
add_parameter("tpr",  "PseudoReducedTemperature", "Gas pseudoreduced temperature")
add_parameter("ppr",  "PseudoReducedPressure", "Gas pseudoreduced pressure")
add_parameter("ppc",  "psia", "Gas pseudocritical pressure")
add_parameter("z",  "ZFactor", "Gas compressibility factor")
add_author("Brill & Beggs")
end_correlation()

################################################################

begin_correlation("CgPapay", "GasIsothermalCompressibility", "psia_1") 
add_parameter("tpr",  "PseudoReducedTemperature", "Gas pseudoreduced temperature")
add_parameter("ppr",  "PseudoReducedPressure", "Gas pseudoreduced pressure")
add_parameter("ppc",  "psia", "Gas pseudocritical pressure")
add_parameter("z",  "ZFactor", "Gas compressibility factor")
add_author("Pápay")
end_correlation()

