# ifndef WATER_SPECIFIC_GRAVITY_UNIT_H
# define WATER_SPECIFIC_GRAVITY_UNIT_H

# include <cmath>
# include "units.H"

// SOURCE: Effect of Salinity on the density of brine (Data from
// international Critical Tables ii 1927, 79)  

// SOURCE: McCain W.D, Jr: The properties of Petroleum Fluid 2nd. ed
// Tulsa, OK PennWell

// SOURCE: Carlos Banzer S, Correlaciones Numericas P.V.T. Instituto
// de Investigaciones Petroleras Universidad del Zulia   

Declare_Physical_Quantity(WaterSpecificGravity, "Sgw","Undefined",
	    R"DESC(density of a brine (in this case formation 
        water) in normal conditions based on total
        solid disolved, it is defined as the ratio
        of the density of the brine to the density
        pure water, both taken at the same pressure
        and temperature (usually atmopheric pressure
        to the seal level and 60 degF) )DESC");

Declare_Unit(Pwl_lb_ft3, "pwl_lb/ft3","lb/ft^{3}",
	     R"DESC(unit used to measure density of produce water (brine) 
         in normal conditions denoted as pound per cubic feet)DESC",
	     WaterSpecificGravity, 62.366389027, 76.96514502699999);

Declare_Unit(Dissolved_Salt_Percent, "dissolvedSaltPercent","wt\\%",
	     R"DESC(Unit used to measure salinity of a brine
	     in normal conditions in percentage of disolved salt)DESC",
	     WaterSpecificGravity, 0, 30);

Declare_Unit(Sgw_sg, "swsg","water=1\\ at\\ 60\\ \\degree{F}",
	     R"DESC(Specific gravity of a brine in normal conditions, it is
         the ratio of the density of a brine to the density of a
         pure water;)DESC", WaterSpecificGravity, 1, 1.2341063383784996);

Declare_Unit(Dissolved_Salt_PPM, "dissolvedSaltPPM","wt\\ ppm",
	     R"DESC(Total solid disolved in a brine
	     measure in Part Per Million PPM)DESC",
	     WaterSpecificGravity, 0, 300000);

Declare_Unit(Molality_NaCl, "mol_NaCl/Kg_H2O","mol\\ NaCl/kg\\ H_2O", "Molality of sodium chloride",
  	     WaterSpecificGravity, 0, 7.333177544050398);

Declare_Unit(CgL, "g_NaCl/L","g\\ NaCl/L\\ H_2O", R"DESC(Total solid disolved in a brine
       measure in grams of NaCl disolved in one liter of water)DESC",
	     WaterSpecificGravity, 0, 300.2306536197343);

Declare_Unit(Dissolved_Salt_Fraction, "dissolvedSaltFraction","Mass\\ fraction\\ NaCl", 
       R"DESC(Unit used to measure salinity of a brine
       in normal conditions in fraction of disolved salt)DESC",
	     WaterSpecificGravity, 0, 0.3);


// To pwl_lb/ft3
Declare_Conversion(Dissolved_Salt_Percent, Pwl_lb_ft3, v) { return 62.368 + 0.438603*v +  0.00160074*v*v ;}
Declare_Conversion(Sgw_sg, Pwl_lb_ft3, v) { return v * 62.366389027; } 
Declare_Conversion(Dissolved_Salt_PPM, Pwl_lb_ft3, v) { return 62.368 + 0.438603*(v/10000) +  0.00160074*(v/10000)*(v/10000);}
Declare_Conversion(Molality_NaCl, Pwl_lb_ft3, v) { return 62.368 + 0.438603*(5844.28*v / (58.4428*v + 1000)) +  0.00160074*(5844.28*v / (58.4428*v + 1000))*(5844.28*v / (58.4428*v + 1000)) ;}
Declare_Conversion(CgL, Pwl_lb_ft3, v) { return 62.368 + 0.438603*(v / 9.9923174527) + 0.00160074*(v / 9.9923174527)*(v / 9.9923174527) ;}
Declare_Conversion(Dissolved_Salt_Fraction, Pwl_lb_ft3, v)  { return 62.368 + 0.438603*(100*v) +  0.00160074*(100*v)*(100*v) ;}

// To dissolvedSaltPercent
Declare_Conversion( CgL,Dissolved_Salt_Percent, v) { return v / 9.9923174527;}
Declare_Conversion(Dissolved_Salt_PPM, Dissolved_Salt_Percent, v) { return v/10000; }
Declare_Conversion(Pwl_lb_ft3, Dissolved_Salt_Percent, v) { double n = -0.438603 + sqrt(0.19237259160900003 - 0.00640296*(62.368 - v));  return n / (0.00320148); }
Declare_Conversion(Sgw_sg, Dissolved_Salt_Percent, v) { double n = -0.438603 + sqrt(0.19237259160900003 - 0.00640296*(62.368 - v * 62.366389027)); return n/(0.00320148); }

Declare_Conversion(Molality_NaCl, Dissolved_Salt_Percent, v) { return 5844.28*v / (58.4428*v + 1000); }
Declare_Conversion(Dissolved_Salt_Fraction, Dissolved_Salt_Percent, v) { return v * 100; } 

// To swsg
Declare_Conversion(Dissolved_Salt_Percent, Sgw_sg, v)   { return (62.368 + 0.438603*v +  0.00160074*v*v) / 62.366389027 ;  }//62.303?
Declare_Conversion(Dissolved_Salt_PPM, Sgw_sg, v){ return (62.368 + 0.438603*(v/10000) + 0.00160074*(v/10000)*(v/10000)) / 62.366389027;}
Declare_Conversion(Pwl_lb_ft3, Sgw_sg, v) { return  v / 62.366389027;  }
Declare_Conversion(Molality_NaCl, Sgw_sg, v)  { return (62.368 + 0.438603*(5844.28*v / (58.4428*v + 1000)) +  0.00160074*(5844.28*v / (58.4428*v + 1000))*(5844.28*v / (58.4428*v + 1000))) / 62.366389027 ;  }
Declare_Conversion(CgL, Sgw_sg, v)  { return (62.368 + 0.438603*(v / 9.9923174527) +  0.00160074*(v / 9.9923174527)*(v / 9.9923174527)) / 62.366389027 ;  }
Declare_Conversion(Dissolved_Salt_Fraction, Sgw_sg, v)   { return (62.368 + 0.438603*(v*100) +  0.00160074*(v*100)*(v*100)) / 62.366389027 ;  }

// To dissolvedSaltPPM
Declare_Conversion(Pwl_lb_ft3, Dissolved_Salt_PPM, v) { return (-0.438603 + sqrt(0.19237259160900003 - 0.00640296*(62.368-v))) / (0.00320148) * 10000 ;}
Declare_Conversion(Sgw_sg, Dissolved_Salt_PPM, v) {  double n = -0.438603 + sqrt(0.19237259160900003 - 0.00640296*(62.368 -v*62.366389027));  return n / (0.00320148) * 10000 ;}
Declare_Conversion(Dissolved_Salt_Percent, Dissolved_Salt_PPM, v) { return  v * 10000  ; }
Declare_Conversion(Molality_NaCl, Dissolved_Salt_PPM, v) { return (5844.28*v / (58.4428*v + 1000))*10000; }
Declare_Conversion(CgL, Dissolved_Salt_PPM, v) { return  (v / 9.9923174527) * 10000; }   
Declare_Conversion(Dissolved_Salt_Fraction,  Dissolved_Salt_PPM, v) { return v * 1000000; } 


// To mol_NaCl/Kg_H2O
Declare_Conversion(Dissolved_Salt_Percent, Molality_NaCl, v) { return 1000*(v/100) / (58.4428*(1 - v/100));}
Declare_Conversion(Dissolved_Salt_PPM, Molality_NaCl, v) { return 1000*((v/10000)/100) / (58.4428*(1 - (v/10000)/100)) ; }
Declare_Conversion(Pwl_lb_ft3, Molality_NaCl, v) { return 1000*(((-0.438603 + sqrt(0.19237259160900003 - 0.00640296*(62.368 - v))) / (0.00320148))/100) / (58.4428*(1 - ((-0.438603 + sqrt(0.19237259160900003 - 0.00640296*(62.368 - v))) / (0.00320148))/100));}
Declare_Conversion(Sgw_sg, Molality_NaCl, v) { return 1000*(((-0.438603 + sqrt(0.19237259160900003 - 0.00640296*(62.368 - (v * 62.366389027)))) / (0.00320148))/100) / (58.4428*(1 - ((-0.438603 + sqrt(0.19237259160900003 - 0.00640296*(62.368 - (v * 62.366389027)))) / (0.00320148))/100));}
Declare_Conversion(CgL, Molality_NaCl, v)  { return 1000*((v / 9.9923174527)/100) / (58.4428*(1 - (v / 9.9923174527)/100));}
Declare_Conversion(Dissolved_Salt_Fraction, Molality_NaCl, v) { return 1000*((v * 100)/100) / (58.4428*(1 - (v * 100)/100));}


// To g_NaCl/L
Declare_Conversion(Dissolved_Salt_Percent, CgL, v) { return 9.9923174527 * v;}
Declare_Conversion(Molality_NaCl, CgL, v) { return (5844.28*v / (58.4428*v + 1000)) * 9.9923174527 ; }
Declare_Conversion(Sgw_sg, CgL, v) { return  ((-0.438603 + sqrt(0.19237259160900003 - 0.00640296*(62.368-v*62.366389027))) / (0.00320148)) * 9.9923174527 ;}
Declare_Conversion(Pwl_lb_ft3, CgL, v) {return (( -0.438603 + sqrt(0.19237259160900003 - 0.00640296*(62.368 - v))) / (0.00320148)) * 9.9923174527;}
Declare_Conversion(Dissolved_Salt_PPM, CgL, v) { return (v/10000) * 9.9923174527 ; }
Declare_Conversion(Dissolved_Salt_Fraction, CgL, v) { return 9.9923174527 * (v*100);}

// To Dissolved_Salt_Fraction
Declare_Conversion( CgL,Dissolved_Salt_Fraction, v) { return (v / 9.9923174527)/100;}
Declare_Conversion(Dissolved_Salt_PPM,Dissolved_Salt_Fraction, v) { return (v/10000)/100; }
Declare_Conversion(Pwl_lb_ft3,Dissolved_Salt_Fraction, v) { return  (( -0.438603 + sqrt(0.19237259160900003 - 0.00640296*(62.368 - v))) / (0.00320148))/100;}
Declare_Conversion(Sgw_sg,Dissolved_Salt_Fraction, v) {  return ((-0.438603 + sqrt(0.19237259160900003 - 0.00640296*(62.368-v*62.366389027))) / (0.00320148))/100;}
Declare_Conversion(Molality_NaCl, Dissolved_Salt_Fraction, v) { return (5844.28*v / (58.4428*v + 1000))/100; }
Declare_Conversion(Dissolved_Salt_Percent, Dissolved_Salt_Fraction, v) { return v / 100; } 



//-----------------------------------------------------------------------
//---------------------------------------------------------------
//S =  5844.28*v / (58.4428*v + 1000)
//mp= 1000*(v/100) / (58.4428*(1 - v/100))

//S =  5844.28*M / (58.4428*M + 1000)
//M= 1000*(S/100) / (58.4428*(1 - S/100))

// # -> Conversion of units
//   # Transformation from weight percent to [g NaCl/L]
//   ppw = 0.99923174527 # [g/cm³] Pure water density at oilfield standard conditions (60 °F and 14.7 psia): 62.38 [lb/ft3]
//   Cgcm3 = saltConcentration/ppw # [g/cm³]
//   CgL = Cgcm3 * 1000/100 # [g/L]
//  # <- Conversion of units
//  999.23174527


//Declare_Conversion(Dissolved_Salt_Percent, CgL, v) { return 9.9923174527 * v;}
//Declare_Conversion( CgL,Dissolved_Salt_Percent, v) { return v / 9.9923174527;}

//---------------------------------------------------------------
//---------------------------------------------------------------



# endif // WATER_SPECIFIC_GRAVITY_UNIT_H
