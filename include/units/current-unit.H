#ifndef CURRENT_UNIT_H
#define CURRENT_UNIT_H

# include "units/units.H"

Declare_Physical_Quantity(Current, "I","Undefined", "Flow of electrical charge");

Declare_Unit(Ampere, "amp","Undefined", "One coulomb per second", Current, 0, 1000);

#endif
