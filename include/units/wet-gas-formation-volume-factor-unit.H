# ifndef WET_GAS_FORMATION_VOLUME_FACTOR_UNIT_H
# define WET_GAS_FORMATION_VOLUME_FACTOR_UNIT_H

# include "units.H"

Declare_Physical_Quantity(WetGasFVF, "Bwg","Undefined",
   R"DESC(The formation volume factor of a wet gas is defined as 
   the volume of reservoir gas (at reservoir pressure and temperature) 
   required to produce one stock-tank barrel of liquid at the surface 
   (at standard conditions). The units are either standard cubic feet
   of reservoir gas per stock-tank barrel or barrels of reservoir gas 
   at reservoir conditions pero stock-tank barrel. The symbol is Bwg.)DESC");


Declare_Unit(RCFGas_STB, "RCF Gas/STB","rcf/STB",
    R"DESC(Dimensionless volumen ratio (volume per volume) 
    measured in reservoir cubic feet per 
    barrel at stock-tank conditions.)DESC",
    WetGasFVF, 0, 561.752988048);

Declare_Unit(RBGas_STB, "RB Gas/STB","RB/STB",
    R"DESC(Dimensionless volumen ratio (volume per volume) 
    measured in barrels at reservoir conditions per 
    barrel at stock-tank conditions.)DESC",
    WetGasFVF, 0, 100);


// The formation volume factor conversions 
 
// To "RB Gas/STB" 
Declare_Conversion(RCFGas_STB, RBGas_STB, v) { return 0.00502 * v / 0.0282 ; }

// To "RCF Gas/STB" 
Declare_Conversion(RBGas_STB, RCFGas_STB, v) { return 0.0282 * v / 0.00502 ; }


// Reference: McCain, W. D. Jr. (1990). The Properties of Petroleum Fluids (2nd ed.). Tulsa, OK: PennWell Books.

# endif // WET_GAS_FORMATION_VOLUME_FACTOR_UNIT_H
