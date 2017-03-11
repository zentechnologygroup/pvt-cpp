# ifndef GOR_GLR_VOLUME_RATIO_UNIT_H
# define GOR_GLR_VOLUME_RATIO_UNIT_H

# include "units.H"

Declare_Physical_Quantity(GORGLRvolumeRatio, "GORGLR_V_V","Undefined",
			  "The gas/oil ratio (GOR) is the ratio of the"
			  "volume of gas that comes out of solution, "
			  "to the volume of oil at standard conditions, "
			  "The gas/liquid ratio (GLR) is the ratio of the "
			  "volume of gas that comes out of solution, to the "
			  "volume of liquid (usually oil+water) at standard "
			  "conditions. Both are a dimensionless ratio "
			  "(volume per volume)");
    
Declare_Unit(SCF_STB, "scf/STB","scf/STB",
         R"DESC(Dimensionless volumen ratio (volume per volume) 
         measured in cubic feet of gas at standard conditions per 
         barrel of liquid (oil,condensate or/and water).)DESC",
         GORGLRvolumeRatio, 0, 1000000000);

Declare_Unit(Mscf_STB, "Mscf/STB","Mscf/STB",
         R"DESC(Dimensionless volumen ratio (volume per volume) 
         measured in thousand of cubic feet of gas at standard conditions per 
         barrel of liquid (oil,condensate or/and water).)DESC",
         GORGLRvolumeRatio, 0, 1000000);

Declare_Unit(MMscf_STB, "MMscf/STB","MMscf/STB",
         R"DESC(Dimensionless volumen ratio (volume per volume) 
         measured in million of cubic feet of gas at standard conditions per 
         barrel of liquid (oil,condensate or/and water).)DESC",
         GORGLRvolumeRatio, 0, 1000);

Declare_Unit(Sm3_Sm3, "Sm3/Sm3","Sm^{3}/Sm^{3}",
         R"DESC(Dimensionless volumen ratio (volume per volume) 
         measured in metrics units, cubic meters of gas at standard conditions
         per cubic meters of liquid (oil,condensate or/and water).)DESC",
         GORGLRvolumeRatio, 0, 178107606.67903525);

// Declare_Unit(Scm3_Scm3, "Scm3/Scm3","Undefined",
//          R"DESC(Dimensionless volumen ratio (volume per volume) 
//          measured in metrics units, cubic meters of gas at standard conditions
//          per cubic centimeters of liquid (oil,condensate or/and water).)DESC",
//          GORGLRvolumeRatio, 0, 5643340857.78781);

// The gas/oil ratio (GOR) and The gas/liquid ratio (GLR) conversions 
       
// To standard cubic feet per stock tank barrels (scf/STB)   
Declare_Conversion(Mscf_STB, SCF_STB, v) { return  1000 * v ; }
Declare_Conversion(MMscf_STB, SCF_STB, v) { return  1000000 * v ; }       
Declare_Conversion(Sm3_Sm3, SCF_STB, v) { return   v / 0.17810760667903525 ; } 

// To thousand standard cubic feet per stock tank barrels (Mscf/STB)
Declare_Conversion(SCF_STB, Mscf_STB, v) { return  v / 1000  ; }
Declare_Conversion(MMscf_STB, Mscf_STB, v) { return  1000 * v ; }       
Declare_Conversion(Sm3_Sm3, Mscf_STB, v) { return  v / 178.10760667903525 ; }

// To million standard cubic feet per stock tank barrels (MMscf/STB)
Declare_Conversion(SCF_STB, MMscf_STB, v) { return  v / 1000000  ; }
Declare_Conversion(Mscf_STB, MMscf_STB, v) { return  v / 1000 ; }       
Declare_Conversion(Sm3_Sm3, MMscf_STB, v) { return v / 178107.60667903525 ; }

// To  standard cubic meters  per standard cubic meters  (Sm3/Sm3)

Declare_Conversion(SCF_STB, Sm3_Sm3, v) { return  v * 0.17810760667903525 ; }
Declare_Conversion(Mscf_STB, Sm3_Sm3, v) { return  v * 178.10760667903525 ; }       
Declare_Conversion(MMscf_STB, Sm3_Sm3, v) { return  v * 178107.60667903525 ; }

# endif //  GOR_GLR_VOLUME_RATIO_UNIT_H
