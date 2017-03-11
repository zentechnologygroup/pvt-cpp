# ifndef DENSITY_UNIT_H
# define DENSITY_UNIT_H

# include "units.H"

Declare_Physical_Quantity(Density, "rho", "\\rho",
		R"DESC(The density of a substance (oil) is its mass
        per unit volume. NOTE: The official density of 
        water at 60 °F according to the 2008 edition of
        ASTM D1250 is 999.016 kg/m3)DESC");

Declare_Unit(Gr_cm3, "gr/cm3","g/cm^{3}",
	    R"DESC(unit of density declared as gr (mass in gram) per cm3
        (unit volume in cubic centimeter))DESC",
	     Density, 0.0001, 3.30);

Declare_Unit(Kg_m3, "kg/m3","kg/m^{3}",
	     R"DESC(unit of density declared as kg (mass in kilogram) per m3 
        (unit volume in cubic meter))DESC",
	     Density, 0.1, 3300);

Declare_Unit(Kg_L, "kg/L","kg/L",
	     R"DESC(unit of density declared as kg (mass in kilogram ) per L
        (unit volume in liter))DESC",
	     Density, 0.0001, 3.30);

Declare_Unit(Lb_ft3, "lb/ft3","lb/ft^{3}",
	     R"DESC(unit of density declared as lb (mass in pound ) per ft3
        (unit volume in cubic feet))DESC",
	     Density, 0.006242782, 206.0117994);

Declare_Unit(Lb_Gal, "lb/gal","lb/gal",
	     R"DESC(unit of density declared as lb (mass in pound ) per Gal
        (unit volume in Gallons))DESC",
	     Density, 0.000834538, 27.5397507);

Declare_Unit(Lb_Inch3, "lb/inch3","lb/in^{3}",
	     R"DESC(unit of density declared as lb (mass in pound ) per inch3
        (unit volume in cubic inches))DESC",
	     Density, 0.000003613, 0.119219793);

Declare_Unit(Sg, "sg","water=1\\ at\\ 60\\ \\degree{F}",
	     R"DESC(Specific gravity is the ratio of the density of a substance
            to the density of a reference substance water)DESC",
	     Density, 0.00010009849692097025, 3.303250398392018);

// to gramPerCubicCentimeter     

Declare_Conversion(Kg_m3, Gr_cm3, v) { return 0.001 * v; }
Declare_Conversion(Kg_L, Gr_cm3, v) { return v; }
Declare_Conversion(Lb_ft3, Gr_cm3, v) { return  0.01601846337396014 * v ; }
Declare_Conversion(Sg, Gr_cm3, v) { return  0.999016 * v ; }    
Declare_Conversion(Lb_Gal, Gr_cm3, v) { return  0.119826793 * v  ; }    
Declare_Conversion(Lb_Inch3, Gr_cm3, v) { return  27.679967537 * v  ; }    

// To KilogramPerCubicMeter

Declare_Conversion(Gr_cm3, Kg_m3, v) { return 1000 * v ; }
Declare_Conversion(Kg_L, Kg_m3, v) { return 1000 * v ; }
Declare_Conversion(Lb_ft3, Kg_m3, v) { return 16.018463374 * v; }
Declare_Conversion(Sg, Kg_m3, v) { return 999.016 * v; }
Declare_Conversion(Lb_Gal, Kg_m3, v) { return  119.826792768 * v; }
Declare_Conversion(Lb_Inch3, Kg_m3, v) { return 27679.898580851597 * v; }

// To KilogramPerLiter  
 
Declare_Conversion(Gr_cm3, Kg_L, v) { return v; }
Declare_Conversion(Kg_m3, Kg_L, v) { return 0.001 * v ; }
Declare_Conversion(Lb_ft3, Kg_L, v) { return  0.01601846337396014 * v ; }
Declare_Conversion(Sg, Kg_L, v) { return 0.999016 * v; }
Declare_Conversion(Lb_Gal, Kg_L, v) { return  0.119826792768 * v  ; }    
Declare_Conversion(Lb_Inch3, Kg_L, v) { return  27.679967537 * v  ; }    

                  
// To Specific Gravity sg

Declare_Conversion(Gr_cm3, Sg, v) { return  v / 0.999016; }
Declare_Conversion(Kg_m3, Sg, v) { return  v / 999.016; }
Declare_Conversion(Lb_ft3, Sg, v) { return v / 62.366389027; }
Declare_Conversion(Kg_L, Sg, v) { return  v / 0.999016; }
Declare_Conversion(Lb_Gal, Sg, v) { return  v / 8.337167147 ; }    
Declare_Conversion(Lb_Inch3, Sg, v) { return  v / 0.036091661 ; }    

// To Pound Per Cubic Feet

Declare_Conversion(Gr_cm3, Lb_ft3, v) { return 62.4279605761446 * v ; }
Declare_Conversion(Kg_m3, Lb_ft3, v) { return  0.0624279606 * v ; }
Declare_Conversion(Kg_L, Lb_ft3, v) { return   62.4279605761446 * v ; }  
Declare_Conversion(Sg, Lb_ft3, v) { return  62.366389027 * v ; }
Declare_Conversion(Lb_Gal, Lb_ft3, v) { return   7.480525210 * v ; }      
Declare_Conversion(Lb_Inch3, Lb_ft3, v) { return   1727.999975642 * v ; }

// To Pound per Gallon

Declare_Conversion( Kg_m3, Lb_Gal, v)  { return 0.0083453790 * v ; }
Declare_Conversion( Gr_cm3, Lb_Gal, v) { return  8.3453790000 * v ; }
Declare_Conversion( Kg_L, Lb_Gal, v) { return  8.3453790000 * v ; }
Declare_Conversion( Lb_ft3, Lb_Gal, v) { return  0.1336804532 * v ; }
Declare_Conversion( Sg, Lb_Gal, v) { return   8.3371671471* v ; }
Declare_Conversion( Lb_Inch3, Lb_Gal, v) { return   230.9998198034 * v ; }


// To Pound per cubic inches

Declare_Conversion( Kg_m3, Lb_Inch3, v) { return  0.0000361273 * v; }
Declare_Conversion( Gr_cm3, Lb_Inch3, v) { return 0.0361272100 * v ; }
Declare_Conversion( Kg_L, Lb_Inch3, v) { return   0.0361272100 * v ; }
Declare_Conversion( Lb_ft3, Lb_Inch3, v) { return   0.0005787037 * v ; }
Declare_Conversion( Sg,Lb_Inch3, v) { return   0.0360910828 * v ; }
Declare_Conversion( Lb_Gal, Lb_Inch3, v) { return   0.0043290077 * v ; }


# endif // DENSITY_UNIT_H









