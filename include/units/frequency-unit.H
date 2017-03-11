#ifndef FREQUENCY_UNIT_H
#define FREQUENCY_UNIT_H

# include "units/units.H"

Declare_Physical_Quantity(Frequency, "f","Undefined", "Number of occurrences of a repeating event per unit time");

Declare_Unit(Hertz, "Hz","Undefined", "Unit of frequency in the International System of Units (SI) and is defined as one cycle per second", Frequency, 0, 120);

Declare_Unit(Revolution_per_minute, "RPM","Undefined", "Unit of frequency and is defined as revolution (cycle) per minute", Frequency, 0, 7200);



// To Revolution_per_minute
Declare_Conversion(Hertz, Revolution_per_minute, V) { return 60 * V; }

// To Hertz
Declare_Conversion(Revolution_per_minute, Hertz, V) { return 1.66666666666666667e-2 * V; }


#endif
