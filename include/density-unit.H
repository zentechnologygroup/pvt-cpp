
# ifndef DENSITY_UNIT_H
# define DENSITY_UNIT_H

# include <units.H>

Declare_Physical_Quantity(Density, "rho",
			  R"DESC(The density of a substance (oil) is its mass
                                 per unit volume. NOTE: The official density of 
                                 water at 60 °F according to the 2008 edition of
                                 ASTM D1250 is 999.016 kg/m3)DESC");

Declare_Unit(Gr_cm3, "gr/cm3",
	     R"DESC(unit of density declared as gr (mass in gram) per cm3
                    (unit volume in cubic centimeter))DESC",
	     Density, 0.58592, 1.04428);

Declare_Unit(Kg_m3, "kg/m3",
	     R"DESC(unit of density declared as kg (mass in kilogram) per m3 
                    (unit volume in cubic meter))DESC",
	     Density, 585.92, 1044.28044);

Declare_Unit(Kg_L, "kg/L",
	     R"DESC(unit of density declared as kg (mass in kilogram ) per L
                    (unit volume in liter))DESC",
	     Density, 0.58592, 1.04428044);

Declare_Unit(Lb_ft3, "lb/ft3",
	     R"DESC(unit of density declared as lb (mass in pound ) per ft3
                    (unit volume in cubic feet))DESC",
	     Density, 36.52, 65.1921);

Declare_Unit(Sg, "sg",
	     R"DESC(Specific gravity is the ratio of the density of a substance
                    to the density of a reference substance water)DESC",
	     Density, 0.58592, 1.04428);

// to gramPerCubicCentimeter     

Declare_Conversion(Kg_m3, Gr_cm3, v) { return  v * 0.001; }
Declare_Conversion(Kg_L, Gr_cm3, v) { return v; }
Declare_Conversion(Lb_ft3, Gr_cm3, v) { return  v * 0.016018463374; }
Declare_Conversion(Sg, Gr_cm3, v) { return  v * 0.9990167 ; }       

// To KilogramPerCubicMeter

Declare_Conversion(Gr_cm3, Kg_m3, v) { return  v * 1000; }
Declare_Conversion(Kg_L, Kg_m3, v) { return  v * 1000; }
Declare_Conversion(Lb_ft3, Kg_m3, v) { return  16.018499958 * v; }
Declare_Conversion(Sg, Kg_m3, v) { return  999.039523707 * v; }
       
     
// To KilogramPerLiter  
 
Declare_Conversion(Gr_cm3, Kg_L, v) { return  1000 * v ; }
Declare_Conversion(Kg_m3, Kg_L, v) { return  0.001 * v ; }
Declare_Conversion(Lb_ft3, Kg_L, v) { return  1 / 62.427818000 * v; }
Declare_Conversion(Sg, Kg_L, v) { return  0.9990167 * v; }
       
            
// To Specific Gravity sg

Declare_Conversion(Gr_cm3, Sg, v) { return  v / 0.9990167; }
Declare_Conversion(Kg_m3, Sg, v) { return  v / 999.039523707; }
Declare_Conversion(Lb_ft3, Sg, v) { return v / 62.366389027; }
Declare_Conversion(Kg_L, Sg, v) { return  v / 0.9990167; }
     
// To Specific pound Per Cubic Feet

Declare_Conversion(Gr_cm3, Lb_ft3, v) { return  v * 62.427818000; }
Declare_Conversion(Kg_m3, Lb_ft3, v) { return  v * 0.062427818000; }
Declare_Conversion(Kg_L, Lb_ft3, v) { return  v * 62.427818000; }      
Declare_Conversion(Sg, Lb_ft3, v) { return  v * 62.366389027; }



# endif // DENSITY_UNIT_H





