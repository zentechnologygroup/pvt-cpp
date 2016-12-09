# coding: utf-8

begin_correlation("PbAlMarhoun", "BubblePointPressure", "psia", 130, 3573)
add_title("AL-MARHOUN CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE")
add_author("Al-Marhoun")
add_parameter("yg", "Sgg", "Gas specific gravity",   0.752,  1.367)
add_parameter("yo", "Sg_do", "Oil specific gravity",  "Quantity<Api>(44.6)",  "Quantity<Api>(19.4)")
add_synonym("yo", "api", "api")
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  26,  1602)
add_parameter("t", "Rankine", "Temperature",  "Quantity<Fahrenheit>(74)",  "Quantity<Fahrenheit>(240)") 
add_db("Based on 69 bottomhole fluid samples from 69 Middle East oil reservoirs.")
add_ref("alMarhoun:1988")
add_eq("P_b = 5.38088 \\cdot {10}^{-3} \\cdot {R_s}^{0.715082} \\cdot {\\gamma_g}^{-1.877840} \\cdot {\\gamma_0}^{3.143700} \\cdot T^{1.326570}")
end_correlation()


################################################################

begin_correlation("PbAlShammasi", "BubblePointPressure", "psia", 31.7, 7127)
add_title("AL-SHAMMASI CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.51,  3.44)
add_parameter("yo", "Sg_do", "Oil specific gravity", "Quantity<Api>(63.7)",  "Quantity<Api>(6)")
add_synonym("yo", "api", "api")
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  6,  3298.6)
add_parameter("t", "Fahrenheit", "Temperature",  74,  341.6)
add_db("Based on a total of 1709 data sets (1661 data sets from 13 different published literature papers and 48 data sets originated from Kuwait reservoirs from unpublished sources).")
add_ref("alShammasi:1999")
add_author("Al-Shammasi")
add_eq("P_b={\\gamma_o}^{5.527215}\\cdot\\,\\exp^{\\left({-1.841408\\,\\cdot\\,[\\gamma_o*\\gamma_g]}\\right)}\\cdot{\\left[R_s\\cdot(T + 460)\\cdot\\gamma_g\\right]}^{0.783716}")
end_correlation()

################################################################

begin_correlation("PbDeGhetto", "BubblePointPressure", "psia", 208.86, 4021.96)
add_title("DE GHETTO, PAONE & VILLA CORRELATION FOR EXTRA-HEAVY AND HEAVY OILS (MODIFIED STANDING CORRELATION), CALCULATION OF BUBBLE POINT PRESSURE")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.623,  1.517)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  17.21,  640.25)
add_parameter("api", "Api", "API oil gravity",  6,  22.3)
add_parameter("t", "Fahrenheit", "Temperature",  131.4,  250.7)
add_db("Based on 1200 measured data points of 63 heavy and extra-heavy oil samples obtained from the Mediterranean Basin, Africa and the Persian Gulf.")
add_db("Oil samples have been divided in two different API gravity classes: extra-heavy oils for °API<=10, heavy oils for 10<°API<=22.3.")
add_author("De Ghetto, Paone & Villa")
add_ref("deGhetto:1995")
add_ref("standing:1947")
add_eq("P_b=\\left(\\frac{R_s}{\\gamma_g}\\right)^{(\\frac{1}{1.1128})}\\cdot{\\frac{10.7025}{10^{(0.0169\\cdot{API}-0.00156\\cdot{T})}}}")
add_eq("P_b=15.7286 \\cdot \\left[ \\left(\\frac{{R_s}}{\\gamma_{g}}\\right)^{0.7885} \\cdot \\frac{10^{0.0020 \\cdot T}}{10^{0.0142 \\cdot API}}\\right]")
add_internal_note("De Ghetto, Paone & Villa use Yg as the average specific gravity of total surface gases, while the original correlation (Standing) uses it as the gravity of dissolved gas. Since Standing doesn't specify if Yg is the gravity of total surface gases or of the separator, it is assumed that the data is from a one stage separation with average gas gravity.")
end_correlation()

################################################################

begin_correlation("PbDindorukChristman", "BubblePointPressure", "psia", 926, 12230)
add_title("DINDORUK & CHRISTMAN CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.623,  1.517)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  133,  3050)
add_parameter("api", "Api", "API oil gravity",  14.7,  40)
add_parameter("t", "Fahrenheit", "Temperature",  117,  276)
add_db("Based on more than 100 PVT reports from the Gulf of Mexico.")
add_author("Dindoruk & Christman")
add_ref("dindoruk:2004")
add_eq("P_b = a_8\\cdot\\left(\\frac{(R_s)\\,^{a_9}}{(\\gamma_g)\\,^{a_{10}}}\\cdot(10)^A+a_{11}\\right)")
add_eq("A = \\frac{a_1 \\cdot (T)^{a_2}+a_3 \\cdot (API)^{a_4} }{{\\left(a_{5}+ \\frac{2\\cdot(R_s)^{a_6}}{(\\gamma_g)^{a7}} \\right)}^{2}}")
end_correlation()

################################################################

begin_correlation("PbDoklaOsman", "BubblePointPressure", "psia", 590, 4640)
add_title("DOKLA & OSMAN CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.798,  1.290)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  181,  2266)
add_parameter("yo", "Sg_do", "Oil specific gravity",  0.8236,  0.886)
add_synonym("yo", "api", "api")
add_parameter("t", "Rankine", "Temperature",  "Quantity<Fahrenheit>(190)",  "Quantity<Fahrenheit>(275)") 
add_db("Based on 51 bottomhole samples from UAE reservoirs.")
add_author("Dokla & Osman")
add_ref("dokla:1992")
add_eq("{P_{b}}\\,=\\,{0.836386\\cdot{{\\,{10^{4}}}\\cdot{{\\,{{\\gamma_{g}}^{\\,-1.01049}}}\\cdot{{\\,{{\\gamma_{o}}^{\\,0.107991}}}\\cdot{{{\\,{{T}}^{\\,-0.952584}}}\\cdot{\,{{R_{s}}^{\\,0.724047}}}}}}}}")
end_correlation()

################################################################

begin_correlation("PbHanafy", "BubblePointPressure", "psia", 36, 5003)
add_title("HANAFY ET AL. CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE")
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  7,  4272)
add_db("Based on experimental PVT data of 324 fluid samples taken from 176 wells located in 75 fields. This data represents 15 productive zones of 123 reservoirs distributed along three different regions of Egypt, including the Gulf of Suez, Western Desert, and Sinai.")
add_author("Hanafy et al.")
add_ref("hanafy:1997")
add_eq("{R_{sft}}\\,=\\,{{69}\\,+\\,{1.071\\cdot{\\,R_{sb}}}}")
add_eq("{R_{si}}\\,=\\,{{23.94}\\,+\\,{1.101\\cdot{\\,R_{sft}}}}")
add_eq("{P_{b}}\\,=\\,{{157.27}\\,+\\,{3.205\\cdot{\\,R_{si}}}}")
end_correlation()

################################################################

begin_correlation("PbKartoatmodjoSchmidt", "BubblePointPressure", "psia", 14.7, 6054.7)
add_title("KARTOATMODJO & SCHMIDT CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.4824,  1.668)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  0,  2890)
add_parameter("api", "Api", "API oil gravity",  14.4,  59)
add_parameter("t", "Fahrenheit", "Temperature",  75,  320)
add_parameter("tsep", "Fahrenheit", "Separator temperature",  38,  294)
add_parameter("psep", "psia", "Separator pressure",  14.7,  1414.7)
add_db("Based on a set of 5392 data points, which represent 740 different crude oil samples.")
add_db("The data bank was collected from PVT reports and literature. The first major source was from South East Asia, mainly Indonesia. The second source was North America, including the offshore area. The rest came from the Middle East and Latin America.")
add_author("Kartoatmodjo & Schmidt")
add_ref("kartoatmodjo:1991")
add_eq("P_b = \\left(\\frac{Rs}{0.05958\\cdot{(\\gamma_{gcorr})^{0.7972}}\\cdot{10^{13.1405\\cdot{\\frac{API}{(T+460)}}}}}\\right)^{0.9986}")
add_eq("P_b = \\left(\\frac{Rs}{0.03150\\cdot{(\\gamma_{gcorr})^{0.7587}}\\cdot{10^{11.289\\cdot{\\frac{API}{(T+460)}}}}}\\right)^{0.9143}")
add_eq("\\gamma_{gcorr}=\\gamma_{gPsep}\\cdot{\\left[{1.00+0.1595\\cdot{API^{0.4078}\\cdot{(T_{sep})^{-0.2466}\\cdot{\\log{\\left(\\frac{P_{sep}}{114.7}\\right)}}}}}\\right]}")
end_correlation()

################################################################

begin_correlation("PbLasater", "BubblePointPressure", "psia", 48, 5780)
add_title("LASATER CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.574,  1.223)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  3,  2905)
add_parameter("yo", "Sg_do", "Oil specific gravity",  "Quantity<Api>(51.1)",  "Quantity<Api>(17.9)")
add_parameter("t", "Fahrenheit", "Temperature",  82,  272)
add_db("Based on 158 experimentally measured bubble point pressures of 137 independent systems.")
add_db("The data were obtained from Canada, Western and Mid-Continental United States, and South America.")
add_author("Lasater")
add_ref("lasater:1958")
add_ref("beggs:1987")
end_correlation()

################################################################

begin_correlation("PbPetroskyFarshad", "BubblePointPressure", "psia", 1574, 6523)
add_title("PETROSKY & FARSHAD CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.5781,  0.8519)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  217,  1406)
add_parameter("api", "Api", "API oil gravity",  16.3,  45)
add_parameter("t", "Fahrenheit", "Temperature",  114,  288)
add_db("Based on 81 laboratory PVT analyses.")
add_db("Fluid samples were obtained from reservoirs located offshore Texas and Louisiana. Producing areas from Galveston Island, eastward, through Main Pass are represented.")
add_author("Petrosky & Farshad")
add_ref("petrosky:1993")
end_correlation()

################################################################

begin_correlation("PbStanding", "BubblePointPressure", "psia", 130, 7000)
add_title("STANDING CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.59,  0.95)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  20,  1425)
add_parameter("api", "Api", "API oil gravity",  16.5,  63.8)
add_parameter("t", "Fahrenheit", "Temperature",  100,  258)
add_db("Based on 105 experimentally determined bubble point pressures from 22 different Californian crude-oil-natural-gas mixtures.")
add_author("Standing")
add_ref("standing:1947")
add_ref("alShammasi:2001")
end_correlation()

################################################################

begin_correlation("PbTotalCFP", "BubblePointPressure", "psia")
add_title("TOTAL COMPAGNIE FRANÇAISE DES PÉTROLES (CFP) CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE")
add_parameter("yg",  "Sgg", "Gas specific gravity")
add_parameter("rsb",  "SCF_STB", "Solution GOR at Pb")
add_parameter("api", "Api", "API oil gravity",  "Api::get_instance().min_val",  45)
add_parameter("t",  "Fahrenheit", "Temperature")
add_db("Based on 336 PVT analyses of crude oil from eastern Venezuela.")
add_author("Total CFP")
add_ref("totalCFP:1983")
add_ref("banzer:1996")
end_correlation()

################################################################

begin_correlation("PbVasquezBeggs", "BubblePointPressure", "psia", 15, 6055)
add_title("VAZQUEZ & BEGGS CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.511,  1.35)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  0,  2199)
add_parameter("api", "Api", "API oil gravity",  15.3,  59.3)
add_parameter("t", "Fahrenheit", "Temperature", 75,  294)
add_parameter("tsep",  "Fahrenheit", "Separator temperature", 76, 150)
add_parameter("psep",  "psia", "Separator pressure", 20, 565) # TODO mínima es 60
add_db("Based on more than 600 laboratory PVT analyses from fields all over the world.")
add_author("Vazquez & Beggs")
add_ref("vazquez:1980")
add_ref("beggs:1987")
add_ref("alShammasi:2001")
# add_ref("banzer:1996") Secondary reference
add_internal_note("The gas gravity is corrected if it resulted from a first-stage separation at a pressure other than 100 psig (approximately 114.7 psia).")
end_correlation()

################################################################

begin_correlation("PbVelarde", "BubblePointPressure", "psia", 70, 6700)
add_title("VELARDE, BLASINGAME & McCAIN CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.556,  1.367)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  10,  1870)
add_parameter("api", "Api", "API oil gravity",  12,  55)
add_parameter("t", "Fahrenheit", "Temperature",  74,  327)
add_db("Based on 728 laboratory measured bubble point pressures.")
add_author("Velarde, Blasingame & McCain")
add_ref("velarde:1997")
end_correlation()

################################################################

begin_correlation("PbCegarra", "BubblePointPressure", "psia", 735, 7303)
add_title("CEGARRA CORRELATION (MODIFIED STANDING CORRELATION), CALCULATION OF BUBBLE POINT PRESSURE")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.654,  1.337)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  135,  4290)
add_parameter("api", "Api", "API oil gravity",  22,  41.9)
add_parameter("t", "Fahrenheit", "Temperature",  117,  354)
add_db("Based on 375 consistent PVT analyses. The data bank was compiled from 8394 fields of eastern Venezuela subdivided into 5192 fields of light oil and 3202 fields of medium oil. The correlation takes the functional form of Standing.")
add_author("Cegarra")
add_ref("cegarra:2007")
add_ref("standing:1947")
add_internal_note("The correlation could not be verified because the original reference is not available. Date: August 10 2016.")
set_hidden()
end_correlation()

################################################################

begin_correlation("PbPerezML", "BubblePointPressure", "psia", 405, 1335)
add_title("PÉREZ, HENY & LAGO CORRELATION (MODIFIED TOTAL CFP CORRELATION), CALCULATION OF BUBBLE POINT PRESSURE")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.571,  0.981)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  38,  121)
add_parameter("api", "Api", "API oil gravity",  6.4,  12.9)
add_parameter("t", "Fahrenheit", "Temperature",  112,  300)
add_author("Pérez, Heny & Lago")
add_ref("perez:2001")
add_internal_note("The correlation could not be verified because the original reference is not available. Date: August 10 2016.")
set_hidden()
end_correlation()

################################################################

begin_correlation("PbMillanArcia", "BubblePointPressure", "psia", 222, 3432.7)
add_title("MILLÁN-ARCIA CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE")
add_parameter("yg",  "Sgg", "Gas specific gravity")
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  53,  459)
add_parameter("api", "Api", "API oil gravity",  9,  20.2)
add_parameter("t", "Fahrenheit", "Temperature",  87,  195)
add_db("Venezuelan heavy crudes were correlated.")
add_author("Millán-Arcia")
add_ref("millan:1984")
add_ref("perez:2001")
add_internal_note("The correlation could not be verified because the original reference is not available. Date: August 10 2016.")
set_hidden()
end_correlation()

################################################################

begin_correlation("PbManucciRosales", "BubblePointPressure", "psia", 1230, 5035)
add_title("MANUCCI & ROSALES CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE")
add_parameter("yg",  "Sgg", "Gas specific gravity")
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  180,  2500)
add_parameter("api", "Api", "API oil gravity",  14,  40)
add_parameter("t", "Fahrenheit", "Temperature",  165,  304)
add_db("Crude oils of eastern Venezuela were correlated, by using differential separation data from laboratory PVT analysis.")
add_author("Manucci & Rosales")
add_ref("manucci:1968")
add_ref("perez:2001")
add_internal_note("The correlation could not be verified because the original reference is not available. Date: August 10 2016.")
set_hidden()
end_correlation()

################################################################

begin_correlation("PbGlaso", "BubblePointPressure", "psia", 135.304, 7112.3)
add_title("GLASO CORRELATION, CALCULATION OF BUBBLE POINT PRESSURE")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.650,  1.276)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  90,  2637)
add_parameter("api", "Api", "API oil gravity",  22.3,  48.1)
add_parameter("t", "Fahrenheit", "Temperature",  80,  280)
add_parameter("n2_concentration", "MoleFraction", "N2 concentration", "Quantity<MolePercent>(0)",  "Quantity<MolePercent>(26)")
add_parameter("co2_concentration","MoleFraction", "CO2 concentration", "Quantity<MolePercent>(0)", "Quantity<MolePercent>(26)")
add_parameter("h2s_concentration", "MoleFraction", "H2S concentration", "Quantity<MolePercent>(0)", "Quantity<MolePercent>(50)")
add_db("Based on 26 samples from the North Sea (collected from wells in the region 56 to 62°N) and 19 samples from the Middle East, Algeria, and several areas in the U.S.")
add_db("The equation was derived from laboratory data, exclusively sampling North Sea oils. However, the author claims that the correlation should be valid for all types of gas/oil mixtures after correcting for non-hydrocarbons in the surface gases and paraffinicity of the oil.")
add_author("Glaso")
add_ref("glaso:1980")
add_eq("P_b=10^{(1.7669+{1.7447\\cdot{\\log{P_{b}^{\\ast}}}}-{0.30218\\cdot{(\\log{P_{b}^{\\ast}})^{2}}})}")
add_eq("{P_b}^{\\ast}={{\\left(\\frac{R_s}{\\gamma_g}\\right)}^{0.816}}\\cdot{\\frac{T^{0.172}}{{\\gamma_{API}}^{0.989}}}")
end_correlation()
