

# ifndef INTERFACIAL_TENSION_UNIT_H
# define INTERFACIAL_TENSION_UNIT_H

# include "units.H"

Declare_Physical_Quantity(InterfacialTension, "sg", "Undefined",
			  R"DESC(Interfacial or surface tension exists when two phases are present. 
			  These phases can be gas/oil, oil/water, or gas/water. Interfacial tension 
			  is the force that holds the surface of a particular phase together 
			  and is normally measured in dynes/cm)DESC");

Declare_Unit(dynes_cm, "dynes/cm","dyne/cm",
	     R"DESC(nterfacial or surface tension measured in dyne per centimeter)DESC",
	     InterfacialTension, 0 , 1000000);

Declare_Unit(N_m, "N/m","N/m",
	     R"DESC(nterfacial or surface tension measured in newton per meter)DESC",
	     InterfacialTension, 0 , 1000);

Declare_Unit(mN_m, "mN/m","mN/m",
	     R"DESC(nterfacial or surface tension measured in milinewton per meter)DESC",
	     InterfacialTension, 0 , 1000000);

Declare_Unit(gram_force_cm, "gram-force/cm","gf/cm",
	     R"DESC(nterfacial or surface tension measured in gram force per centimeter)DESC",
	     InterfacialTension, 0 , 1019.716213);

Declare_Unit(pound_force_inch, "pound-force/inch","lbf/in",
	     R"DESC(nterfacial or surface tension measured in pound force per inch)DESC",
	     InterfacialTension, 0 , 5.710147098);

// interfacial Tension Conversion
// To dynes per centimeter
Declare_Conversion(N_m, dynes_cm, v) { return 1000 * v ; }
Declare_Conversion(mN_m, dynes_cm, v) { return v ; }
Declare_Conversion(gram_force_cm, dynes_cm, v) { return 980.6649999788 * v ; }
Declare_Conversion(pound_force_inch, dynes_cm, v) { return 175126.8369864 * v ; }

// To Newtons per meters
Declare_Conversion(dynes_cm, N_m, v) { return 0.001 * v  ; }
Declare_Conversion(mN_m, N_m, v) { return  0.001 * v  ; }
Declare_Conversion(gram_force_cm, N_m, v) { return  0.9806649999786 * v  ; }
Declare_Conversion(pound_force_inch, N_m, v) { return  0175.1268369864 * v  ; }

// To milinewtons per meters
Declare_Conversion(dynes_cm, mN_m, v) { return  v  ; }
Declare_Conversion(N_m, mN_m, v) { return 1000 * v ; }
Declare_Conversion(gram_force_cm, mN_m, v) { return 980.6649999788 * v ; }
Declare_Conversion(pound_force_inch, mN_m, v) { return 175126.8369864 * v ; }

// To gram-force/cm
Declare_Conversion(dynes_cm, gram_force_cm, v) { return 0.001019716213 * v ; }
Declare_Conversion(N_m, gram_force_cm, v) { return 1.019716213 * v ; }
Declare_Conversion(mN_m, gram_force_cm, v) { return 0.001019716213 * v ; }
Declare_Conversion(pound_force_inch, gram_force_cm, v) { return 178.5796750065 * v ; }

// To pound-force/inch
Declare_Conversion(dynes_cm, pound_force_inch, v) { return 0.000005710147098 * v ; }
Declare_Conversion(N_m, pound_force_inch, v) { return  0.005710147098 * v  ; }
Declare_Conversion(mN_m, pound_force_inch, v) { return 0.000005710147098 * v  ; }
Declare_Conversion(gram_force_cm, pound_force_inch, v) { return  0.005599741403739 * v  ; }

# endif // INTERFACIAL_TENSION_UNIT_H



