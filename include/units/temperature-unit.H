
# ifndef TEMPERATURE_UNIT_H
# define TEMPERATURE_UNIT_H

# include "units.H"

Declare_Physical_Quantity(Temperature, "T","Undefined",
			  "Average thermal energy of the particles in a substance");

Declare_Unit(Kelvin, "degK","K",
	     "Absolute scale where the zero is the lowest temperature",
	     Temperature, 0, 672.03886667);
Declare_Unit(Celsius, "degC","\\degree{C}",
	     "Scale based on water freezing (0) and boiling point (100)",
	     Temperature, -273.15, 398.88888889);
Declare_Unit(Fahrenheit, "degF","\\degree{F}", 
	     "Scale based on brine freezing (0) and boiling point (100)",
	     Temperature, -459.67, 750);
Declare_Unit(Rankine, "degR","\\degree{R}", "Absolute scale of temperature", Temperature,
	     0, 1209.67);

// To Fahrenheit 
Declare_Conversion(Kelvin, Fahrenheit, t) { return 1.8 * t - 459.67; }
Declare_Conversion(Celsius, Fahrenheit, t) { return 1.8 * t + 32; }
Declare_Conversion(Rankine, Fahrenheit, t) { return t - 459.67; }


// To Celsius
Declare_Conversion(Rankine, Celsius, t) { return (t - 491.67) / 1.8; }
Declare_Conversion(Kelvin, Celsius, t) { return t - 273.15; }
Declare_Conversion(Fahrenheit, Celsius, t) { return (t - 32) / 1.8; }


// To Rankine
Declare_Conversion(Fahrenheit, Rankine, t) { return t + 459.67; }
Declare_Conversion(Celsius, Rankine, t) { return (t + 273.15) * 1.8; }
Declare_Conversion(Kelvin, Rankine, t) { return 1.8 * t; }


// To Kelvin
Declare_Conversion(Fahrenheit, Kelvin, t) { return (t + 459.67) / 1.8; }
Declare_Conversion(Rankine, Kelvin, t) { return t / 1.8; }
Declare_Conversion(Celsius, Kelvin, t) { return t + 273.15; }


# endif // TEMPERATURE_UNIT_H
