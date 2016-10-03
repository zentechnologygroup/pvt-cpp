
#ifndef COMPRESSIBILITY_UNIT_H
#define COMPRESSIBILITY_UNIT_H 1

# include <units.H>

Declare_Physical_Quantity(Compressibility, "Compressibility",
			  "Inverse of a pressure that force is distributed");

Declare_Unit(psia_1, "psia^-1", "Inverse of psia", Compressibility, 1e-6, 1e7);



#endif // COMPRESSIBILITY_UNIT_H
