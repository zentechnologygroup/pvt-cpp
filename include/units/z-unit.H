
# ifndef Z_UNIT_H
# define Z_UNIT_H

# include "units.H"

Declare_Physical_Quantity(CompressibilityFactor, "Z", "Z",
			  "Compressibility factor (Z): ratio of the molar volume of a gas to the molar volume of an ideal gas at the same temperature and pressure.")

Declare_Unit(Zfactor, "zFactor","Undefined", "Compressibility factor", CompressibilityFactor,
	     0.2, 2);

# endif // Z_UNIT_H
