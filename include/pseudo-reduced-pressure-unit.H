#ifndef PSEUDO_REDUCED_PRESSURE_UNIT_H
#define PSEUDO_REDUCED_PRESSURE_UNIT_H

# include <units.H>

Declare_Physical_Quantity(PressureRatio, "PressureRatio",
			  "Ratio between two pressures");


Declare_Unit(PseudoReducedPressure, "Ppr", "Ratio between two pressures",
	     PressureRatio, 1e-6, 1e6);


#endif
