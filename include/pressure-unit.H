
# ifndef PRESSURE_UNIT_H
# define PRESSURE_UNIT_H

# include <units.H>


Declare_Physical_Quantity(Pressure, "P",
			  R"D(Pressure is the force applied perpendicularly
                              to The surface of an object per unit
                              area over which that force is distributed)D");

Declare_Unit(Bar, "bar",
	     "(Pressure unit equivalent to ten newtons per square centimeter",
	     Pressure, -1.0135326860, numeric_limits<double>::max());

Declare_Unit(Pascal, "Pa",
	     R"D(SI unit of pressure used for quantifying internal pressure, 
                 stress, Young's modulus and ultimate tensile strength.
                 It is defined as one newton per square meter)D",
	     Pressure, -101353.3362952215, numeric_limits<double>::max());

Declare_Unit(kPascal, "kPa", "1 kPa = 1000 Pa,", Pressure,
	     -101.3532686010, numeric_limits<double>::max());

Declare_Unit(psia, "psia",
	     R"D(Pounds per square inch absolute (psia)
                 Used for making clear that the pressure is relative
                 to a vacuum rather than the ambient atmospheric pressure.
                 Since atmospheric pressure at sea level is around 14.7 psi)D",
	     Pressure, 0, numeric_limits<double>::max());

Declare_Unit(psig, "psig",
	     R"D(Pound-force per square inch. Unit of pressure of stress based
                 on avoirdupois units. It is the pressure resulting from a force
                 of one pound-force applied to an area of one square inch)D",
	     Pressure, -14.7, numeric_limits<double>::max());

Declare_Unit(Atmosphere, "atm", "unit of pressure defined as 101325 Pa",
	     Pressure, -1.00027898968870666706, numeric_limits<double>::max());

// pressure conversions 
  
// To atmophere (atm)
Declare_Conversion(Bar, Atmosphere, v) { return v * 0.986923267; }
Declare_Conversion(Pascal, Atmosphere, v) { return v * 9.86923267e-6; }
Declare_Conversion(kPascal, Atmosphere, v) { return v * 9.86923267e-3; }
Declare_Conversion(psia, Atmosphere, v) { return  v / 14.695948775; }
Declare_Conversion(psig, Atmosphere, v) { return (v + 14.695948775)/14.695948775; }

// to Bar 
Declare_Conversion(Atmosphere, Bar, v)  { return v * 1.0132499658; }
Declare_Conversion(Pascal, Bar, v)  { return v * 1.0e-5; }
Declare_Conversion(kPascal, Bar, v)  { return v * 1.0e-2; }
Declare_Conversion(psia, Bar, v)  { return v / 14.503773773; }
Declare_Conversion(psig, Bar, v) { return (v + 14.695948775) / 14.503773773; }

// To Pascal
Declare_Conversion(Atmosphere, Pascal, v) { return ((double) (1.0)/9.86923267e-6)*v; } 
Declare_Conversion(Bar, Pascal, v) { return  v * 100000.0; }
Declare_Conversion(psia, Pascal, v) { return  (v - 14.695948775)/1.4503773773e-4; }
Declare_Conversion(psig, Pascal, v) { return (v + 14.695948775) / 1.4503773773e-4; }
Declare_Conversion(kPascal, Pascal, v) { return v * 1000; }

// To kPascal
Declare_Conversion(Atmosphere, kPascal, v) { return v * 101.32499658; }
Declare_Conversion(Bar, kPascal, v) { return  v * 100.0000; }
Declare_Conversion(psia, kPascal, v) { return  v / 1.4503773773e-1; }
Declare_Conversion(psig, kPascal, v) { return (v + 14.695948775) / 1.4503773773e-1; }
Declare_Conversion(Pascal, kPascal, v) { return v / 1000; }

// To Absolute Pound Force per Square Inch (psia)
Declare_Conversion(Atmosphere, psia, v) { return v * 14.695948775; }
Declare_Conversion(Bar, psia, v) { return v * 14.503773773; }
Declare_Conversion(Pascal, psia, v) { return 1.4503773773e-4*v + 14.695948775; }
Declare_Conversion(kPascal, psia, v) { return v * 1.4503773773e-1; }
Declare_Conversion(psig, psia, v) { return v + 14.695948775; }

// To Pound Force per Square Inch (psig)
Declare_Conversion(psia, psig, v) { return v - 14.695948775; }
Declare_Conversion(Atmosphere, psig, v) { return v * 14.695948775 - 14.695948775; }
Declare_Conversion(Bar, psig, v) { return v * 14.503773773 - 14.695948775; }
Declare_Conversion(Pascal, psig, v) { return v * 1.4503773773e-4; }
Declare_Conversion(kPascal, psig, v) { return v * 1.4503773773e-1 - 14.695948775; }
  

# endif //  PRESSURE_UNIT_H
