#ifndef PSEUDO_REDUCED_TEMP_UNIT_H
#define PSEUDO_REDUCED_TEMP_UNIT_H 1

# include "units.H"

Declare_Physical_Quantity(TemperatureRatio, "TemperatureRatio", "Undefined",
			  "ration between two temperatures");


Declare_Unit(PseudoReducedTemperature, "Tpr","Dmnl", "Ratio between two temperatures",
	     TemperatureRatio, 1e-6, 1e6);


#endif // PSEUDO_REDUCED_TEMP_UNIT_H

