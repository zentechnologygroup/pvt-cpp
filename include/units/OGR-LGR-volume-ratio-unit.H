# ifndef OGR_LGR_VOLUME_RATIO_UNIT_H
# define OGR_LGR_VOLUME_RATIO_UNIT_H

# include "units.H"

Declare_Physical_Quantity(OGRLGRvolumeRatio, "OGRLGR_V_V", "Undefined",
			  R"DESC(The condensate/gas ratio (OGR) is the ratio of the
			  volume of hydrocarbons that condensate to the volume of gas 
			  (both at standard conditions). OGR refers to the amount of surface 
			  condensate that can be vaporized in a surface gas at a specific 
			  pressure and temperature. It is denoted mathematically as rs.
			  The liquid/gas ratio (LGR) is the ratio of the volume of liquid 
			  (usually hydrocarbon + water) to the volume of gas (both at standard conditions).
			  OGR and LGR are dimensionless ratios (volume per volume).)DESC");
    
Declare_Unit(STB_SCF, "STB/scf","STB/scf",
         R"DESC(Dimensionless volume ratio (volume per volume) 
         measured in barrels of liquid per cubic feet of gas 
	 at standard conditions.)DESC",
         OGRLGRvolumeRatio, 1e-9, 1e-4);

Declare_Unit(STB_Mscf, "STB/Mscf","STB/Mscf",
         R"DESC(Dimensionless volume ratio (volume per volume) 
         measured in barrels of liquid per thousand of cubic feet of gas 
	 at standard conditions.)DESC",
         OGRLGRvolumeRatio, 1e-6, 1e-1);

Declare_Unit(STB_MMscf, "STB/MMscf","STB/MMscf",
         R"DESC(Dimensionless volume ratio (volume per volume) 
         measured in measured in barrels of liquid per million of cubic feet of gas 
	 at standard conditions.)DESC",
         OGRLGRvolumeRatio, 1e-3, 1e2);

Declare_Unit(Sm3Liquid_Sm3, "Sm3 Liquid/Sm3","Sm^{3}/Sm^{3}",
         R"DESC(Dimensionless volume ratio (volume per volume) 
	 measured in cubic meters of liquid per cubic meters of gas 
	 at standard conditions.)DESC",
         OGRLGRvolumeRatio, 5.614583e-9, 5.614583e-4);


// The condensate/gas ratio (OGR) and the liquid/gas ratio (LGR) conversions 
        
// To stock tank barrels per standard cubic feet (STB/scf)    
Declare_Conversion(STB_Mscf, STB_SCF, v) { return v / 1000 ; }
Declare_Conversion(STB_MMscf, STB_SCF, v) { return v / 1000000 ; }       
Declare_Conversion(Sm3Liquid_Sm3, STB_SCF, v) { return 0.17810760667903525 * v ; } 

// To stock tank barrels per thousand standard cubic feet (STB/Mscf)
Declare_Conversion(STB_SCF, STB_Mscf, v) { return  1000 * v ; }
Declare_Conversion(STB_MMscf, STB_Mscf, v) { return  v / 1000 ; }       
Declare_Conversion(Sm3Liquid_Sm3, STB_Mscf, v) { return 178.10760667903525 * v ; }

// To stock tank barrels per million standard cubic feet (STB/MMscf)
Declare_Conversion(STB_SCF, STB_MMscf, v) { return 1000000 * v ; }
Declare_Conversion(STB_Mscf, STB_MMscf, v) { return 1000 * v ; }       
Declare_Conversion(Sm3Liquid_Sm3, STB_MMscf, v) { return 178107.60667903525 * v ; }

// To standard cubic meters per standard cubic meters (Sm3/Sm3)
Declare_Conversion(STB_SCF, Sm3Liquid_Sm3, v) { return  v / 0.17810760667903525 ; }
Declare_Conversion(STB_Mscf, Sm3Liquid_Sm3, v) { return  v / 178.10760667903525 ; }       
Declare_Conversion(STB_MMscf, Sm3Liquid_Sm3, v) { return  v / 178107.60667903525 ; }


// Reference: AAPG Wiki (2017). Petroleum reservoir fluid properties. http://wiki.aapg.org/Petroleum_reservoir_fluid_properties

# endif //  OGR_LGR_VOLUME_RATIO_UNIT_H
