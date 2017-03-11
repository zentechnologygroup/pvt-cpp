# ifndef GAS_SPECIFIC_GRAVITY_UNIT_H
# define GAS_SPECIFIC_GRAVITY_UNIT_H

# include "units.H"

Declare_Physical_Quantity(GasSpecificGravity, "Yg","Undefined",
		R"DESC(The specific gravity of a gas, γ, is the 
		ratio of the density of the gas at standard pressure
		and temperature to the density of air at the same 
		standard pressure and temperature. The Society of Petroleum 
		Engineers (SPE) defines standard temperature to be 15°C 
		[288.15°K, 59°F] and standard pressure to be 100 kPa.)DESC");

Declare_Unit(Sgg, "Sgg","air=1",
	     R"DESC(it is defined as the ratio of the density of a gas,
	     compared to the density of air in the same 
	     pressure and temperature condition (ussualy standard condition)
	     . it is a dimensionless ratio (density per density)DESC",
	     GasSpecificGravity, 0.5, 5);

Declare_Unit(rhog_kg_m3_atStandCond, "kg/m3atStandCond","kg/m^{3}\\ at\\ STP",
	     R"DESC(gas density at standard condition.
	     Acording to SPE: 15°C [288.15°K, 59°F] and 100 kPa.
	     . Air density in this standard pressure and temperature 
	     is 1.225 kg/m3)DESC", GasSpecificGravity, 0.6125, 6.125);

Declare_Unit(rhog_lb_ft3_atStandCond, "lb/ft3atStandCond","lb/ft^{3}\\ at\\ STP",
	     R"DESC(gas density at standard condition.
	     Acording to SPE: 15°C [288.15°K, 59°F] and 100 kPa.
	     . Air density in this standard pressure and temperature 
	     is 0.0764740771 lb/ft3)DESC", GasSpecificGravity,
	     0.03823703855, 0.38237038549999997);


// To specific gravity of a gas

Declare_Conversion(rhog_kg_m3_atStandCond, Sgg, v) { return  v / 1.225 ; }
Declare_Conversion(rhog_lb_ft3_atStandCond, Sgg, v) { return v / 0.0764740771 ; }

// To gas density at standard condition
// Acording to SPE: 15°C [288.15°K, 59°F] and 100 kPa
// measured in kg/m3

Declare_Conversion(Sgg, rhog_kg_m3_atStandCond, v) { return v * 1.225; }
Declare_Conversion(rhog_lb_ft3_atStandCond, rhog_kg_m3_atStandCond, v)
{ return v * 16.0184999578 ; }


// To gas density at standard condition
// Acording to SPE: 15°C [288.15°K, 59°F] and 100 kPa
// measured in lb/ft3

Declare_Conversion(Sgg, rhog_lb_ft3_atStandCond, v) { return v * 0.0764740771; }
Declare_Conversion(rhog_kg_m3_atStandCond, rhog_lb_ft3_atStandCond, v)
{ return v * 0.0624278180 ; }


       
// The density of air, (Greek: rho) (air density), is the mass per
// unit volume of Earth's atmosphere. Air density, like air pressure,
// decreases with increasing altitude. It also changes with variation
// in temperature or humidity. At sea level and at 15 °C air has a 
// density of approximately 1.225 kg/m3 (0.001225 g/cm3, 0.0023769
// slug/ft3, 0.0765 lbm/ft3) according to ISA (International Standard Atmosphere).

# endif // GAS_SPECIFIC_GRAVITY_UNIT_H
