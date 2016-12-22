# coding: utf-8

begin_correlation("ZfactorSarem", "GasCompressibility", "Zfactor")
add_title("SAREM FIT METHOD, CALCULATION OF THE GAS COMPRESSIBILITY FACTOR")
add_parameter("tpr", "PseudoReducedTemperature", "Gas pseudoreduced temperature", 1.05, 2.95)
add_parameter("ppr", "PseudoReducedPressure", "Gas pseudoreduced pressure", 0.1, 14.9)
add_ref("sarem:1961")
add_ref("standing:1942")
# add_ref("banzer:1996") Secondary reference
add_db("The correlation was obtained by using Legendre polynomials of up to five degree to fit the Standing-Katz curves for the gas compressibility factor (Z).")
add_internal_note("The original reference is not available. The correlation was verified by using a secondary reference: Bánzer (1996). Date: September 22 2016.")
add_author("Sarem")
end_correlation()

################################################################

begin_correlation("ZfactorHallYarborough", "GasCompressibility", "Zfactor")
add_title("HALL & YARBOROUGH FIT METHOD, CALCULATION OF THE GAS COMPRESSIBILITY FACTOR")
add_parameter("tpr", "PseudoReducedTemperature", "Gas pseudoreduced temperature", 1.2, 3)
add_parameter("ppr", "PseudoReducedPressure", "Gas pseudoreduced pressure", 0.1, 24)
add_ref("hall:1973")
add_ref("yarborough:1974")
add_ref("standing:1942")
# add_ref("banzer:1996") Secondary reference
add_db("Based on the equation of state of Starling-Carnahan for the calculation of Z-factor. The coefficients of the equation were obtained by fitting the Standing-Katz curves.")
add_db("The equation is solved by using the Newton-Raphson iteration method.")
add_internal_note("The original reference is not available. The correlation was verified by using a secondary reference: Bánzer (1996). Date: September 23 2016.")
add_internal_note("The application ranges and data bank information was obtained from Takacs (1989).")
add_author("Hall & Yarborough")
end_correlation()

################################################################

begin_correlation("ZfactorDranchukPR", "GasCompressibility", "Zfactor")
add_title("DRANCHUK, PURVIS & ROBINSON FIT METHOD, CALCULATION OF THE GAS COMPRESSIBILITY FACTOR")
add_parameter("tpr", "PseudoReducedTemperature", "Gas pseudoreduced temperature", 1.05, 3)
add_parameter("ppr", "PseudoReducedPressure", "Gas pseudoreduced pressure", 0.2, 30)
add_ref("dranchuk:1973")
add_ref("standing:1942")
add_ref("takacs:1989")
# add_ref("banzer:1996") Secondary reference
add_db("This method is a result of fitting the Benedict-Webb-Rubin (BWR) equation of state for the Z-factor. 1500 data points from the original Standing-Katz chart were used.")
add_db("The equation is solved by using the Newton-Raphson iteration method.")
add_internal_note("The correlation was verified by using the original reference and a secondary one: Bánzer (1996). Date: September 28 2016.")
add_internal_note("The application ranges and data bank information was obtained from Takacs (1989).")
add_author("Dranchuk, Purvis & Robinson")
end_correlation()

################################################################

begin_correlation("ZfactorDranchukAK", "GasCompressibility", "Zfactor")
add_title("DRANCHUK & ABOU-KASSEM FIT METHOD, CALCULATION OF THE GAS COMPRESSIBILITY FACTOR")
add_parameter("tpr", "PseudoReducedTemperature", "Gas pseudoreduced temperature", 0.7, 3)
add_parameter("ppr", "PseudoReducedPressure", "Gas pseudoreduced pressure", 0, 30)
add_ref("dranchuk:1975")
add_ref("standing:1942")
add_ref("takacs:1989")
# add_ref("banzer:1996") Secondary reference
add_db("This method is a result of fitting the Starling-Carnahan equation of state for the Z-factor. 1500 data points from the original Standing-Katz chart were used.")
add_db("The equation is solved by using the Newton-Raphson iteration method.")
add_note("The ranges of application are: 1 <= Tpr <= 3 for 0.2 <= Ppr <= 30 and 0.7 <= Tpr <= 1 for 0 <= Ppr <= 1.")
add_internal_note("The original reference is not available. The correlation was verified by using a secondary reference: Bánzer (1996). Date: September 23 2016.")
add_internal_note("The application ranges and data bank information was obtained from Takacs (1989).")
add_internal_note("The lower limit for Ppr (when Tpr's range is from 0.7 to 1.0) was taken from the development ranges of the correlation presented by Standing & Katz (1942).")
add_author("Dranchuk & Abou-Kassem")
end_correlation()

################################################################

begin_correlation("ZfactorGopal", "GasCompressibility", "Zfactor")
add_title("GOPAL FIT METHOD, CALCULATION OF THE GAS COMPRESSIBILITY FACTOR")
add_parameter("tpr", "PseudoReducedTemperature", "Gas pseudoreduced temperature", 1.05, 3)
add_parameter("ppr", "PseudoReducedPressure", "Gas pseudoreduced pressure", 0.2, 15)
add_ref("gopal:1977")
add_ref("standing:1942")
add_ref("takacs:1989")
# add_ref("banzer:1996") Secondary reference
add_db("Gopal presented a non-iterative method for calculating the Z-factor. The coefficients of the equations were obtained by fitting the Standing-Katz curves.")
add_internal_note("The original reference is not available. The correlation was verified by using a secondary reference: Bánzer (1996). Date: September 23 2016.")
add_internal_note("The application ranges and data bank information was obtained from Takacs (1989).")
add_author("Gopal")
end_correlation()

################################################################

begin_correlation("ZfactorBrillBeggs", "GasCompressibility", "Zfactor")
add_title("BRILL & BEGGS FIT METHOD, CALCULATION OF THE GAS COMPRESSIBILITY FACTOR")
add_parameter("tpr", "PseudoReducedTemperature", "Gas pseudoreduced temperature", 1.2, 2.4)
add_parameter("ppr", "PseudoReducedPressure", "Gas pseudoreduced pressure", 0, 13)
add_ref("brill:1974")
add_ref("standing:1942")
# add_ref("banzer:1996") Secondary reference
add_db("The equation was obtained by applying adjustment methods to the Standing-Katz curves for the gas compressibility factor (Z).")
add_internal_note("The original reference is not available. The correlation was verified by using a secondary reference: Bánzer (1996). Date: September 26 2016.")
add_author("Brill & Beggs")
end_correlation()

################################################################

begin_correlation("ZfactorPapay", "GasCompressibility", "Zfactor")
add_title("PÁPAY FIT METHOD, CALCULATION OF THE GAS COMPRESSIBILITY FACTOR")
add_parameter("tpr", "PseudoReducedTemperature", "Gas pseudoreduced temperature", 1.2, 3)
add_parameter("ppr", "PseudoReducedPressure", "Gas pseudoreduced pressure", 0.2, 15)
add_ref("papay:1968")
add_ref("standing:1942")
add_ref("takacs:1989")
# add_ref("banzer:1996") Secondary reference
add_db("Pápay presented an equation for the calculation of Z-factors, resulting from applying methods of adjustment to the Standing-Katz curves. The equation is widely used in Hungary.")
add_internal_note("The original reference is not available. The correlation was verified by using secondary references: Bánzer (1996) and Takacs (1989). Date: September 23 2016.")
add_internal_note("The data bank information was obtained from Takacs (1989).")
add_internal_note("The application ranges were presented by Bánzer (1996).")
add_author("Pápay")
end_correlation()

