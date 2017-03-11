# ifndef NON_HYDROCARBONS_FRACTION_UNIT_H
# define NON_HYDROCARBONS_FRACTION_UNIT_H

# include "units.H"

Declare_Physical_Quantity(NonHydrocarbonsFraction, "fvi", "Undefined",
			  R"DESC(volume fraction of a non-hydrocarbon component)DESC");

Declare_Unit(MoleFraction, "MoleFraction","mol\\ fraction",
	     R"DESC(volume fraction of a non-hydrocarbon component 
	     base on a division mole by mole)DESC",
	     NonHydrocarbonsFraction, 0 , 1);

Declare_Unit(VolumeFraction, "VolumeFraction","vol\\ fraction",
	     R"DESC(volume fraction of a non-hydrocarbon component 
	     base on a division volume by volume)DESC",
	     NonHydrocarbonsFraction, 0, 1);

Declare_Unit(MolePercent, "MolePercent","mol\\%",
	     R"DESC(Mole percentage of a non-hydrocarbon component 
	     base on a division mole by mole)DESC",
	     NonHydrocarbonsFraction, 0 , 100);

Declare_Unit(VolumePercent, "VolumePercent","vol\\%",
	     R"DESC(volume percentage of a non-hydrocarbon component 
	     base on a division volume by volume)DESC",
	     NonHydrocarbonsFraction, 0, 100);

// isothermal compresibility conversions
// To Mole_Fraction
Declare_Conversion(VolumeFraction, MoleFraction, v) { return v ; }
Declare_Conversion(MolePercent, MoleFraction, v) { return v / 100 ; }
Declare_Conversion(VolumePercent, MoleFraction, v) { return v / 100 ; }

// To Volume_Fraction
Declare_Conversion(MoleFraction, VolumeFraction, v) { return v ; }
Declare_Conversion(MolePercent, VolumeFraction, v) { return v / 100 ; }
Declare_Conversion(VolumePercent, VolumeFraction, v) { return v / 100 ; }

// To Mole_Percent
Declare_Conversion(MoleFraction, MolePercent, v) { return 100 * v ; }
Declare_Conversion(VolumeFraction, MolePercent, v) { return 100 * v ; }
Declare_Conversion(VolumePercent, MolePercent, v) { return  v ; }

// To Volume_Percent
Declare_Conversion(MoleFraction, VolumePercent, v) { return 100 * v ; }
Declare_Conversion(VolumeFraction, VolumePercent, v) { return 100 *  v ; }
Declare_Conversion(MolePercent, VolumePercent, v) { return v ; }

# endif // NON_HYDROCARBONS_FRACTION_UNIT_H
