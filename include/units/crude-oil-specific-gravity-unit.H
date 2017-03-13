
# ifndef CRUDE_OIL_SPECIFIC_GRAVITY_UNIT_H
# define CRUDE_OIL_SPECIFIC_GRAVITY_UNIT_H 

# include "units.H"

Declare_Physical_Quantity(OilGravity, "rho_o", "\\rho",
			  R"DESC(Oil gravity is relates to specific density 
                                 measurements of liquids petroleum products 
		                 at dead oil conditions)DESC");

Declare_Unit(Api, "api","\\degree{API}",
	     R"DESC(The API gravity is a method used for measuring 
                    the density of petroleum and it is considered one
                    of the most important standards of The American Petroleum 
                    Institute API gravity is a measure of how heavy or
                    light a petroleum liquid is compared to water)DESC",
	     OilGravity, 4, 110);


Declare_Unit(Sg_do, "sg_do","water=1\\ at\\ 60\\ \\degree{F}",
	     R"DESC(This specific gravity is related to the calculations
                    of the API and it is defined a density ratio of 
                    a dead crude oil at 60 degrees Fahrenheit compared 
                    with the density of freshwater as reference substance
                    at the same temperature.)DESC",
	     OilGravity, 0.58592, 1.04428);


// Crude Oil Specific Gravity conversions        
// To API
       
Declare_Conversion(Sg_do, Api, v) { return  141.5 / v - 131.5; }
       
          
// To Specific Gravity sg

Declare_Conversion(Api, Sg_do, v) { return  141.5 / ( v + 131.5 ); }


# endif // CRUDE_OIL_SPECIFIC_GRAVITY_UNIT_H
