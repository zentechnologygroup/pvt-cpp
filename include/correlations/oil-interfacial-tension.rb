# coding: utf-8

# verificada con python!
begin_correlation("SgoBakerSwerdloff", "OilInterfacialTension", "dynes_cm", 0, 40)
add_parameter("t",  "Fahrenheit", "Temperature")
add_parameter("api",  "Api" , "API oil gravity", 4, 75)
add_parameter("p",  "psia", "Pressure", 14.7, 5000)
add_title("BAKER & SWERDLOFF CORRELATION, CALCULATION OF OIL INTERFACIAL TENSION")
add_internal_note("The correlation was verified by using secondary references (Beggs, 1987 and Bánzer, 1996) because the original reference is not available. Date: August 30 2016.")
add_author("Baker & Swerdloff")
add_ref("baker:1956")
add_ref("beggs:1987")
add_ref("petroWiki:2016:4")
# add_ref("banzer:1996") Secondary reference
end_correlation()
