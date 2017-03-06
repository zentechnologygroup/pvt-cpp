# coding: utf-8

begin_correlation("Bwg", "WetGasVolumeFactor", "RB_STB")
add_title("CALCULATION OF WET GAS FORMATION VOLUME FACTOR")
add_parameter("t", "Rankine", "Temperature")
add_parameter("p", "psia", "Pressure")
add_parameter("z", "Zfactor", "Gas compressibility factor")
add_parameter("rsp1", "SCF_STB", "Primary separator producing GOR")
add_parameter("veq", "SCF_STB", "Equivalent volume")
add_author("Standard Equation")
add_note("The formation volume factor of a wet gas is defined as the volume of reservoir gas required to produce one stock-tank barrel of liquid at the surface.")
add_ref("mcCain:1990")
add_internal_note("The equation was verified by using as reference: McCain (1990). Date: March 06 2017.")
end_correlation()
