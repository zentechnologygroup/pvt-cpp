

# ifndef NON_HYDROCARBONS_FRACTION_UNIT_H
# define NON_HYDROCARBONS_FRACTION_UNIT_H

# include <units.H>

Declare_Physical_Quantity(NonHydrocarbonsFraction, "fvi",
			  R"DESC(volume fraction of a non-hydrocarbon component)DESC");

Declare_Unit(Mole_Fraction, "MoleFraction",
	     R"DESC(volume fraction of a non-hydrocarbon component 
	     base on a division mole by mole)DESC",
	     NonHydrocarbonsFraction, 0 , 1);


Declare_Unit(Volume_Fraction, "VolumeFraction",
	     R"DESC(volume fraction of a non-hydrocarbon component 
	     base on a division volume by volume)DESC",
	     NonHydrocarbonsFraction, 0, 1);



Declare_Unit(Mole_Percent, "MolePercent",
	     R"DESC(Mole percentage of a non-hydrocarbon component 
	     base on a division mole by mole)DESC",
	     NonHydrocarbonsFraction, 0 , 100);


Declare_Unit(Volume_Percent, "VolumePercent",
	     R"DESC(volume percentage of a non-hydrocarbon component 
	     base on a division volume by volume)DESC",
	     NonHydrocarbonsFraction, 0, 100);


// isothermal compresibility conversions


// To Mole_Fraction
Declare_Conversion(Volume_Fraction, Mole_Fraction, v) { return v ; }
Declare_Conversion(Mole_Percent, Mole_Fraction, v) { return v / 100 ; }
Declare_Conversion(Volume_Percent, Mole_Fraction, v) { return v / 100 ; }

// To Volume_Fraction
Declare_Conversion(Mole_Fraction, Volume_Fraction, v) { return v ; }
Declare_Conversion(Mole_Percent, Volume_Fraction, v) { return v / 100 ; }
Declare_Conversion(Volume_Percent, Volume_Fraction, v) { return v / 100 ; }

// To Mole_Percent
Declare_Conversion(Mole_Fraction, Mole_Percent, v) { return 100 * v ; }
Declare_Conversion(Volume_Fraction, Mole_Percent, v) { return 100 * v ; }
Declare_Conversion(Volume_Percent, Mole_Percent, v) { return  v ; }

// To Volume_Percent
Declare_Conversion(Mole_Fraction, Volume_Percent, v) { return 100 * v ; }
Declare_Conversion(Volume_Fraction, Volume_Percent, v) { return 100 *  v ; }
Declare_Conversion(Mole_Percent, Volume_Percent, v) { return v ; }



# endif // NON_HYDROCARBONS_FRACTION_UNIT_H


