
# ifndef GOR_GLR_UNIT_H
# define GOR_GLR_UNIT_H

# include <units.H>


Declare_Physical_Quantity(volumeRatio, "V_V",
     R"DESC(The gas/oil ratio (GOR) is the ratio of the 
        volume of gas that comes out of solution, 
        to the volume of oil at standard conditions, The gas/liquid ratio (GLR) is the ratio of the 
        volume of gas that comes out of solution, 
        to the volume of liquid (usually oil+water) at standard conditions. 
        Both are a dimensionless ratio (volume per volume))DESC");

    
Declare_Unit(SCF_STB, "scf/STB",
         R"DESC(Dimensionless volumen ratio (volume per volume) 
         measured in cubic feet of gas at standard conditions per 
         barrel of liquid (oil,condensate or/and water).)DESC",
         Density, 0, 1000000000000);

Declare_Unit(Mscf_STB, "Mscf/STB",
         R"DESC(Dimensionless volumen ratio (volume per volume) 
         measured in thousand of cubic feet of gas at standard conditions per 
         barrel of liquid (oil,condensate or/and water).)DESC",
         Density, 0, 1000000000);


Declare_Unit(MMscf_STB, "MMscf/STB",
         R"DESC(Dimensionless volumen ratio (volume per volume) 
         measured in million of cubic feet of gas at standard conditions per 
         barrel of liquid (oil,condensate or/and water).)DESC",
         Density, 0, 1000);

Declare_Unit(Sm3_Sm3, "Sm3/Sm3",
         R"DESC(Dimensionless volumen ratio (volume per volume) 
         measured in metrics units, cubic meters of gas at standard conditions per 
         cubic meters of liquid (oil,condensate or/and water).)DESC",
         Density, 0, 5643340857.78781);



// The gas/oil ratio (GOR) and The gas/liquid ratio (GLR) conversions 
       
// To standard cubic feet per stock tank barrels (scf/STB)
   
Declare_Conversion(MSCF_STB, scf_STB, v) { return  v * 1000  ; }
Declare_Conversion(MMSCF_STB, scf_STB, v) { return  v * 1000000 ; }       
Declare_Conversion(Sm3_Sm3, scf_STB, v) { return  v * 0.1772  ; }


// To thousand standard cubic feet per stock tank barrels (Mscf/STB)

Declare_Conversion(scf_STB, MSCF_STB, v) { return  v / 1000  ; }
Declare_Conversion(MMSCF_STB, MSCF_STB, v) { return  v * 1000 ; }       
Declare_Conversion(Sm3_Sm3, MSCF_STB, v) { return  v * 0.0001772  ; }

// To million standard cubic feet per stock tank barrels (MMscf/STB)

Declare_Conversion(scf_STB, MMSCF_STB, v) { return  v / 1000000  ; }
Declare_Conversion(MSCF_STB, MMSCF_STB, v) { return  v / 1000 ; }       
Declare_Conversion(Sm3_Sm3, MMSCF_STB, v) { return  v * 0.0000001772   ; }

// To  standard cubic meters  per standard cubic meters  (Sm3/Sm3)

Declare_Conversion(SCF_STB, Sm3_Sm3, v) { return  v / 0.1772  ; }
Declare_Conversion(MSCF_STB, Sm3_Sm3, v) { return  v / 0.0001772 ; }       
Declare_Conversion(MMSCF_STB, Sm3_Sm3, v) { return  v / 0.0000001772  ; }


# endif //  GOR_GLR_UNIT_H