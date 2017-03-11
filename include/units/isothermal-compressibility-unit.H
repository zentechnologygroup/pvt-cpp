

# ifndef ISOTHERMAL_COMPRESSIBILIY_UNIT_H
# define ISOTHERMAL_COMPRESSIBILIY_UNIT_H

# include "units.H"

Declare_Physical_Quantity(IsothermalCompressibility, "C","Undefined",
			  R"DESC(Isothermal compressibility is the 
			  change in volume of a hydrocarbon system as the pressure 
			  changes while temperature remains constant.)DESC");


Declare_Unit(Pascal_1, "Pa^-1","Pa^{-1}", R"DESC(Unit of isothermal compressibility declared as 1/Pa)DESC", 
	     IsothermalCompressibility, 1.450377e-12, 1.450377e-6);

Declare_Unit(mPa_1, "MPa^-1","MPa^{-1}", R"DESC(Unit of isothermal compressibility declared as 1/MPa)DESC", 
	     IsothermalCompressibility, 1.450377e-6, 1.450377);

Declare_Unit(psia_1, "psia^-1","psia^{-1}", R"DESC(Unit of isothermal compressibility declared as 1/psia)DESC", 
	     IsothermalCompressibility, 1e-8, 1e-2);

Declare_Unit(Bar_1, "bar^-1","bar^{-1}", R"DESC(Unit of isothermal compressibility declared as 1/bar)DESC",
	     IsothermalCompressibility, 1.450377e-7, 1.450377e-1);

Declare_Unit(Atmosphere_1, "atm^-1","atm^{-1}",
	     R"DESC(Unit of isothermal compressibility declared as 1/atm)DESC",
	     IsothermalCompressibility, 1.469595e-7, 1.469595e-1);


// Isothermal Compressility conversions 

// to Pa^-1
Declare_Conversion(mPa_1, Pascal_1, v) { return 1e-6 * v ; }
Declare_Conversion(psia_1, Pascal_1, v) { return 1.450377377302092e-4 * v ; }
Declare_Conversion(Bar_1, Pascal_1, v) { return 1e-5 * v ; }
Declare_Conversion(Atmosphere_1, Pascal_1, v) { return 9.869232667160129e-6 * v ; }

// to MPa^-1
Declare_Conversion(Pascal_1, mPa_1, v) { return 1e6 * v ; }
Declare_Conversion(psia_1, mPa_1, v) { return 1.450377377302092e2 * v ; }
Declare_Conversion(Bar_1, mPa_1, v) { return 10 * v ; }
Declare_Conversion(Atmosphere_1, mPa_1, v) { return 9.869232667160129 * v ; }

// To psia^-1
Declare_Conversion(Pascal_1, psia_1, v) { return 6894.757293168362 * v ; }
Declare_Conversion(mPa_1, psia_1, v) { return 6894.757293168362e-6 * v ; }
Declare_Conversion(Bar_1, psia_1, v) { return 6.894757293168362e-2 * v ; }
Declare_Conversion(Atmosphere_1, psia_1, v) { return 6.804596390987774e-2 * v ; }

// To bar^-1
Declare_Conversion(Pascal_1, Bar_1, v) { return 1e5 * v ; }
Declare_Conversion(mPa_1, Bar_1, v) { return 1e-1 * v ; }
Declare_Conversion(psia_1, Bar_1, v) { return 14.503773773020919 * v ; }
Declare_Conversion(Atmosphere_1, Bar_1, v) { return 9.869232667160127e-1 * v ; }

// To atm^-1
Declare_Conversion(Pascal_1, Atmosphere_1, v) { return 101325 * v ; }
Declare_Conversion(mPa_1, Atmosphere_1, v) { return 101325e-6 * v ; }
Declare_Conversion(psia_1, Atmosphere_1, v) { return 14.695948775513449 * v ; }
Declare_Conversion(Bar_1, Atmosphere_1, v) { return 1.01325 * v ; }

  
# endif // ISOTHERMAL_COMPRESSIBILIY_UNIT_H



