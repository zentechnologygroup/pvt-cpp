# ifndef GAS_FORMATION_VOLUME_FACTOR_UNIT_H
# define GAS_FORMATION_VOLUME_FACTOR_UNIT_H

# include "units.H"

Declare_Physical_Quantity(GasFVF, "Bg","Undefined",
   R"DESC(The formation volume factor of gas is defined 
   as the ratio of the volume of gas at the reservoir 
   temperature and pressure to the volume at the standard 
   or surface temperature and pressure (ps and Ts). 
   It is given the symbol Bg and is often expressed 
   in either cubic feet of reservoir volume per standard 
   cubic foot of gas or barrels of reservoir volume per 
   standard cubic foot of gas.)DESC");
  
Declare_Unit(RCF_SCF, "rcf/scf","rcf/scf",
     R"DESC(Gas Formation Volume Factor, gas volume 
     relationship measured as reservoir
     cubic feet into standard cubic feet)DESC",
     GasFVF, 0, 10);

Declare_Unit(RM3_SM3, "rm3/sm3","Rm^{3}/Sm^{3}",
     R"DESC(Gas Formation Volume Factor, gas volume 
     relationship measured as reservoir
     cubic meters into standard cubic meter)DESC",
     GasFVF, 0, 10);

Declare_Unit(RCF_MSCF, "rcf/Mscf","rcf/Mscf",
    R"DESC(Gas Formation Volume Factor, gas volume 
    relationship measured as reservoir
    cubic feet into thousand standard cubic feet)DESC",
    GasFVF, 0, 10000);

Declare_Unit(RB_SCF, "RB/scf","RB/scf",
    R"DESC(Gas Formation Volume Factor, gas volume 
    relationship measured as reservoir
    barrels into standard cubic feet)DESC",
    GasFVF, 0, 1.7810766679);

Declare_Unit(RB_MSCF, "RB/Mscf","RB/Mscf",
    R"DESC(Gas Formation Volume Factor, gas volume 
    relationship measured as reservoir
    barrels into thousand standard cubic feet)DESC",
    GasFVF, 0, 1781.0766679);

// The The formation volume factor conversions 
    
// To "rcf/scf" 
Declare_Conversion(RM3_SM3, RCF_SCF, v) { return v ; }
Declare_Conversion(RCF_MSCF, RCF_SCF, v) { return 0.001 * v ; }
Declare_Conversion(RB_SCF, RCF_SCF, v) { return 5.61458333333 * v ; }
Declare_Conversion(RB_MSCF, RCF_SCF, v) { return 0.00561458333333 * v ; }

// To "rm3/sm3"
Declare_Conversion(RCF_SCF, RM3_SM3, v) { return v ; } 
Declare_Conversion(RCF_MSCF, RM3_SM3, v) { return 0.001 * v ; } 
Declare_Conversion(RB_SCF, RM3_SM3, v) { return 5.61458333333 * v ; }
Declare_Conversion(RB_MSCF, RM3_SM3, v) { return 0.00561458333333 * v ; }

// To "rcf/Mscf"
Declare_Conversion(RCF_SCF, RCF_MSCF, v) { return 1000 * v ; }
Declare_Conversion(RM3_SM3, RCF_MSCF, v) { return 1000 * v ; }  
Declare_Conversion(RB_SCF, RCF_MSCF, v) { return 5.61458333333e3 * v ; }
Declare_Conversion(RB_MSCF, RCF_MSCF, v) { return 5.61458333333 * v ; }

// To "RB/scf"
Declare_Conversion(RCF_SCF, RB_SCF, v) { return 0.178107606679 * v ; }
Declare_Conversion(RM3_SM3, RB_SCF, v) { return 0.178107606679 * v ; } 
Declare_Conversion(RCF_MSCF, RB_SCF, v) { return 0.178107606679e-3 * v ; }
Declare_Conversion(RB_MSCF, RB_SCF, v) { return 0.001 * v ; }

// To "RB/Mscf"
Declare_Conversion(RCF_SCF, RB_MSCF, v) { return 178.107606679 * v ; }
Declare_Conversion(RM3_SM3, RB_MSCF, v) { return 178.107606679 * v ; }
Declare_Conversion(RCF_MSCF, RB_MSCF, v) { return 0.178107606679 * v ; }
Declare_Conversion(RB_SCF, RB_MSCF, v) { return 1000 * v ; }


# endif // GAS_FORMATION_VOLUME_FACTOR_UNIT_H
