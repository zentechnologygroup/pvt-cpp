# coding: utf-8

begin_correlation("Gor", "ProducingGasOilRatio", "SCF_STB")
add_title("CALCULATION OF PRODUCING GAS OIL RATIO (GOR)")
add_parameter("ogr", "STB_MMscf", "Primary sparator condensate gas ratio")
add_author("Standard Equation")
add_note("Calculation of producing Gas Oil Ratio (GOR).")
add_internal_note("Producing GOR is the inverse of Condensate gas ratio.")
set_hidden()
end_correlation()
