

# ifndef ISOTHERMAL_COMPRESSIBILIY_UNIT_H
# define ISOTHERMAL_COMPRESSIBILIY_UNIT_H

# include <units.H>

Declare_Physical_Quantity(IsothermalCompressility, "C",
			  R"DESC(Isothermal compressibility is the 
			  change in volume of a oil system as the pressure 
			  changes while temperature remains constant. )DESC");


Declare_Unit(Bar_1, "bar_1", "Isothermal compressibility mesuared in 1/bar",
	     IsothermalCompressility, 0, 14.503773773 );

Declare_Unit(Pascal_1, "Pa_1",
	     "Isothermal compressibility mesuared in 1/Pascal",
	     IsothermalCompressility, 0, 1.4503773773e-4);

Declare_Unit(psi_1, "psi_1",
	     "Isothermal compressibility mesuared in 1/psi",
	     IsothermalCompressility, 0, 1);


Declare_Unit(Atmosphere_1, "atm_1",
	     "Isothermal compressibility mesuared in 1/Atmosphere",
	     IsothermalCompressility, 0, 14.9659487755);

// IsothermalCompressilityconversions  
// To bar-1
Declare_Conversion(Pascal_1, Bar_1, v) { return 100000 * v ; }
Declare_Conversion(psi_1, Bar_1, v) { return 14.503773773 * v ; }
Declare_Conversion(Atmosphere_1, Bar_1, v) { return 0.986923266716 * v ; }

// to Pa-1
Declare_Conversion(Bar_1, Pascal_1, v) { return 0.00001 * v ; }
Declare_Conversion(psi_1, Pascal_1, v) { return 1.4503773773e-4 * v ; }
Declare_Conversion(Atmosphere_1, Pascal_1, v) { return 9.86923266716e-6 * v ; }


// To psi-1
Declare_Conversion(Bar_1, psi_1, v) { return 6.89475729317e-2 * v ; }
Declare_Conversion(Pascal_1, psi_1, v) { return 6894.75729317 * v ; }
Declare_Conversion(Atmosphere_1, psi_1, v) { return 6.80459639099e-2 * v ; }

// To atm-1
Declare_Conversion(Bar_1, Atmosphere_1, v) { return 1.01325 * v ; }
Declare_Conversion(Pascal_1, Atmosphere_1, v) { return 101325 * v ; }
Declare_Conversion(psi_1, Atmosphere_1, v) { return 14.9659487755 * v ; }

  
# endif // ISOTHERMAL_COMPRESSIBILIY_UNIT_H



