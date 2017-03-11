#ifndef FLOWRATE_UNIT_H
#define FLOWRATE_UNIT_H

# include "units/units.H"

Declare_Physical_Quantity(FlowRate, "Q","Undefined", "Volume rate of some fluid flow that is transported through a given cross-sectional area");

Declare_Unit(BPD, "bpd","Undefined", "Volume rate measured in barrels per day", FlowRate, 0, 1e10);

Declare_Unit(GPM, "gpm","Undefined", "Volume rate measured in U.S. Gallon Per Minute ", FlowRate, 0, 1e10);

Declare_Unit(CMD, "cmd","Undefined", "Volume rate measured in cubic meter per day ", FlowRate, 0, 1e10);

Declare_Unit(CMS, "cms","Undefined", "Volume rate measured in cubic meter per second ", FlowRate, 0, 1e10);


// to BPD 
Declare_Conversion(GPM, BPD, V) { return 34.2857142857 * V; }
Declare_Conversion(CMD, BPD, V) { return 6.28981077043 * V; }
Declare_Conversion(CMS, BPD, V) { return 543439.650565* V; }


// to GPM 
Declare_Conversion(BPD, GPM, V) { return 2.91666666667e-2 * V; }
Declare_Conversion(CMD, GPM, V) { return 0.183452814138 * V; }
Declare_Conversion(CMS, GPM, V) { return 15850.3231415 * V; }


// to CMD
Declare_Conversion(BPD, CMD, V) { return 0.158987294928 * V; }
Declare_Conversion(GPM, CMD, V) { return 5.45099296896 * V; }
Declare_Conversion(CMS, CMD, V) { return 86400 * V; }


// to CMS 
Declare_Conversion(BPD, CMS, V) { return 1.84013072833e-6 * V; }
Declare_Conversion(CMD, CMS, V) { return 1.15740740741e-5  * V; }
Declare_Conversion(GPM, CMS, V) { return 0.0000630901964 * V; }


#endif
