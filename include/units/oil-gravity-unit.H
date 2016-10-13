
# ifndef OIL_GRAVITY_UNIT_H
# define OIL_GRAVITY_UNIT_H

# include <units.H>


Declare_Physical_Quantity(OilGravity, "rho_o",
			  R"DESC(The density of a substance (oil) is its mass per
                            unit volume. NOTE: The official density of water at
                            60 °F according to the 2008 edition of ASTM D1250 is
                            999.016 kg/m3)DESC");

Declare_Unit(Api, "api",
	     R"DESC(The American Petroleum Institute gravity, or API gravity,
	            is a measure of how heavy or light a petroleum liquid is
	            compared to water)DESC", OilGravity, 4, 110);


Declare_Unit(Gr_cm3, "gr/cm3",
	     R"DESC(unit of density declared as gr (mass in gram) per cm3 (unit
                    volume in cubic centimeter))DESC",
	     OilGravity, 0.58592, 1.04428);

Declare_Unit(Kg_m3, "kg/m3",
	     R"DESC(unit of density declared as kg (mass in kilogram) per m3
                     (unit volume in cubic meter))DESC",
	     OilGravity, 585.92, 1044.28044);

Declare_Unit(Gr_L, "gr/L",
	     R"DESC(unit of density declared as gr (mass in gram ) per L 
                    (unit volume in liter))DESC",
	     OilGravity, 0.58592, 1.04428044);

Declare_Unit(Lb_ft3, "lb/ft3",
	     R"DESC(unit of density declared as lb (mass in pound ) per ft3
	            (unit volume in cubic feet))DESC",
	     OilGravity, 36.577, 65.1921);

Declare_Unit(Sg, "sg",
	     R"DESC(Specific gravity is the ratio of the density of a substance
                    to the density of a reference substance water)DESC",
	     OilGravity, 0.58592, 1.04428);


// Oil Density conversions        
// To API
       
Declare_Conversion(gr, Api, v) { return  (141.5 / v) - 131.5; }
Declare_Conversion(kilogramPerCubicMeter, Api, v) { return  (141.5 * 1000/v) - 131.5; } 
Declare_Conversion(kilogramPerLiter, Api, v) { return  (141.5 / v) - 131.5; }
Declare_Conversion(poundPerCubicFeet, Api, v) { return  (141.5/ (v/ 62.427818)) - 131.5; }
Declare_Conversion(specificGravity, Api, v) { return  141.5/v - 131.5; }
       

// gramPerCubicCentimeter     

Declare_Conversion(api, GramPerCubicCentimeter, v) { return  (141.5 / (v + 131.5)); }
Declare_Conversion(kilogramPerCubicMeter, GramPerCubicCentimeter, v) { return  v * 0.001; }
Declare_Conversion(kilogramPerLiter, GramPerCubicCentimeter, v) { return  v; }
Declare_Conversion(poundPerCubicFeet, GramPerCubicCentimeter, v) { return  v * 0.0160184999578233; }
Declare_Conversion(specificGravity, GramPerCubicCentimeter, v) { return  v ; }       

// To KilogramPerCubicMeter

Declare_Conversion(api, KilogramPerCubicMeter, v) { return  1000* (141.5 / (v + 131.5)); }
Declare_Conversion(gramPerCubicCentimeter, KilogramPerCubicMeter, v) { return  v * 1000; }
Declare_Conversion(kilogramPerLiter, KilogramPerCubicMeter, v) { return  v *1000; }
Declare_Conversion(poundPerCubicFeet, KilogramPerCubicMeter, v) { return  v * 16.0184999578233; }
Declare_Conversion(specificGravity, KilogramPerCubicMeter, v) { return  v * 1000; }
       
     
// To KilogramPerLiter  
 
Declare_Conversion(api, KilogramPerLiter, v) { return  (141.5 / (v + 131.5)); }
Declare_Conversion(gramPerCubicCentimeter, KilogramPerLiter, v) { return  v * 1000; }
Declare_Conversion(kilogramPerCubicMeter, KilogramPerLiter, v) { return  v * 0.001; }
Declare_Conversion(poundPerCubicFeet, KilogramPerLiter, v) { return  v * 0.016018499957823; }
Declare_Conversion(specificGravity, KilogramPerLiter, v) { return  v; }
       
            
// To Specific Gravity sg

Declare_Conversion(api, SpecificGravity, v) { return  141.5 / (v + 131.5); }
Declare_Conversion(gramPerCubicCentimeter, SpecificGravity, v) { return  v; }
Declare_Conversion(kilogramPerCubicMeter, SpecificGravity, v) { return  v/1000; }
Declare_Conversion(poundPerCubicFeet, SpecificGravity, v) { return v/ 62.427818; }
Declare_Conversion(kilogramPerLiter, SpecificGravity, v) { return  v ; }
       
       
// To Specific pound Per Cubic Feet

Declare_Conversion(gramPerCubicCentimeter, PoundPerCubicFeet, v) { return  v *  62.427818; }
Declare_Conversion(kilogramPerCubicMeter, PoundPerCubicFeet, v) { return  v *   0.062427818; }
Declare_Conversion(api, PoundPerCubicFoot, v) { return  62.3678575608*(141.5 / (v + 131.5)); }
Declare_Conversion(kilogramPerLiter, PoundPerCubicFoot, v) { return  v * 62.4278180000; }      
Declare_Conversion(specificGravity, PoundPerCubicFoot, v) { return  v * 62.4278180000; }


# endif // OIL_GRAVITY_UNIT_H
