#ifndef HEAD_UNIT_H
#define HEAD_UNIT_H

# include "units/units.H"

Declare_Physical_Quantity(Head, "H","Undefined",  R"DESC(The static head of a pump is the maximum height 
										(pressure) it can deliver. The capability of the pump 
										at a certain RPM can be read from its Q-H curve 
										(flow vs. height). ");
										)DESC");


Declare_Unit(headFeet, "Hft","Undefined", "The static head of a pump measured in feet", Head, 0, 16000); 

Declare_Unit(headInch, "Hin","Undefined", "The static head of a pump measured in inches", Head, 0, 192000); 

Declare_Unit(headMeter, "Hmts","Undefined", "The static head of a pump measured in meters", Head, 0, 4876.8); 

// To headFoot 
Declare_Conversion(headInch, headFoot, H) { return 8.3333333333333333e-2 * H; }
Declare_Conversion(headMeter, headFoot, H) { return 3.28083989501 * H; }

// To headInch
Declare_Conversion(headMeter, headInch, H) { return 39.3700787402 * H; }
Declare_Conversion(headFoot, headInch, H) { return 12 * H; }

// To headMeter
Declare_Conversion(headFoot, headMeter, H) { return 0.3048 * H; }
Declare_Conversion(headInch, headMeter, H) { return 0.0254 * H; }


#endif // HEAD_UNIT_H

