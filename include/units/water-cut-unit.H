# ifndef WATER_CUT_UNIT_H
# define WATER_CUT_UNIT_H

# include <cmath>
# include "units.H"
 

Declare_Physical_Quantity(WaterCut, "wc","Undefined",
	    R"DESC(The ratio of water produced compared to the 
      volume of total liquids produced. The water cut in
       waterdrive reservoirs. )DESC");

Declare_Unit(wc_percent, "wc_p","Undefined",
	     R"DESC(The ratio of water produced compared to the 
      volume of total liquids measured in percentage)DESC",
	     WaterCut, 0, 100);

Declare_Unit(wc_fraction, "wc_f","Undefined",
       R"DESC(The ratio of water produced compared to the 
      volume of total liquids measured in fraction)DESC",
       WaterCut, 0, 1);




// To wc_percent
Declare_Conversion(wc_fraction, wc_percent, v) { return v*100 ;}


// To wc_percent
Declare_Conversion( wc_percent, wc_fraction, v) { return v/100 ;}


# endif // WATER_CUT_UNIT_H
