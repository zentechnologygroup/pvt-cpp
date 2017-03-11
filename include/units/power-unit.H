#ifndef POWER_UNIT_H
#define POWER_UNIT_H

# include "units/units.H"

Declare_Physical_Quantity(Power, "P","Undefined", "power is the rate of doing work. It is the amount of energy consumed per unit time");

Declare_Unit(HorsePower, "hp","Undefined", "Unit of measurement of power hp", Power, 0, 2000);

Declare_Unit(watt, "W","Undefined", "Unit of measurement of power watt", Power, 0, 1491399.74316);

// To HorsePower
Declare_Conversion(watt, HorsePower, V) { return 1.3410220896e-3 * V; }

// To watt
Declare_Conversion(HorsePower, watt, V) { return 745.699871582 * V; }

#endif
