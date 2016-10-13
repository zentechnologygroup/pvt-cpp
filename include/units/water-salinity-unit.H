# ifndef WATER_SALINITY_UNIT_H
# define WATER_SALINITY_UNIT_H

# include <units.H>

Declare_Physical_Quantity(waterSalinity, "Sw",
		R"DESC(Water salinity is the saltiness or dissolved salt content of a body of water.)DESC");

Declare_Unit(PPM, "ppm",
	     R"DESC(Water salinity measured in parts per millions of salt content)DESC",
	     waterSalinity, 0, 1000000);

Declare_Unit(saltPercent, "%salt",
	     R"DESC(Water salinity measured in percent of salt content)DESC", waterSalinity, 0, 100);



// To ppm
Declare_Conversion(saltPercent, PPM, v) { return  10000 * v ; }


// To %salt
Declare_Conversion(PPM, saltPercent, v) { return  0.0001 * v  ; }


# endif // WATER_SALINITY_UNIT_H
