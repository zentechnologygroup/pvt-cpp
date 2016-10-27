# coding: utf-8

# verificada con python!
begin_correlation("SgoBakerSwerdloff", "OilInterfacialTension", "dynes_cm", 0, 35)
add_parameter("t",  "Fahrenheit", "Temperature")
add_parameter("api",  "Api" , "API oil gravity")
add_parameter("p",  "psia", "Pressure")
add_title("BAKER & SWERDLOFF CORRELATION, CALCULATION OF OIL INTERFACIAL TENSION")
add_internal_note("The correlation was verified by using secondary references (Beggs, 1987 and Bánzer, 1996) because the original reference is not available. Date: August 30 2016.")
add_author("Baker & Swerdloff")
add_ref("baker:1956")
add_ref("beggs:1987")
add_ref("banzer:1996")
end_correlation()
