# ifndef Dynamic_VISCOSITY_UNIT_H
# define Dynamic_VISCOSITY_UNIT_H

# include "units.H"

Declare_Physical_Quantity(DynamicViscosity, "U", "U",
                R"DESC(The viscosity of a fluid is a measure of its resistance
               to gradual deformation by shear stress or tensile stress.
               The dynamic (shear) viscosity of a fluid expresses its 
               resistance to shearing flows, where adjacent layers
               move parallel to each other with different speeds.)DESC");

Declare_Unit(kg_mxs, "kg/m*s","kg/m \\cdot s",
         "Dynamic Viscosity measured as kilogram divided into meter per second.",
         DynamicViscosity, 0, 10000000);

Declare_Unit(lb_ftxs, "lb/ft*s","lb/ft \\cdot s",
         "Dynamic Viscosity measured as pound divided into foot per second.",
          DynamicViscosity, 0, 6719689.75);

Declare_Unit(g_cmxs, "g/cm*s","g/cm \\cdot s",
         "Dynamic Viscosity measured as gram divided into centimeter per second.",
          DynamicViscosity, 0, 100000000);

Declare_Unit(CP, "cP","cP",
         "Dynamic Viscosity measured as centipoise.",
          DynamicViscosity, 0, 10000000000);

Declare_Unit(Paxs, "Pa*s","Pa \\cdot s",
         "Dynamic Viscosity measured as Pascal per second.",
          DynamicViscosity, 0, 10000000);

Declare_Unit(lb_ftxh, "lb/ft*h","lb/ft \\cdot h",
         "Dynamic Viscosity measured as pound divided into foot per hour.",
          DynamicViscosity, 0, 24190881500);

Declare_Unit(Poise, "poise","P",
         "Dynamic Viscosity measured as poise.",
          DynamicViscosity, 0, 100000000);

Declare_Unit(mP, "mP","mP", "Dynamic Viscosity measured as micropoise.",
          DynamicViscosity, 0, 100000000e6);

// Dynamic Viscosity Conversions
  
// To Kg/m*s
Declare_Conversion(lb_ftxs, kg_mxs, v) { return 1.4881639439 * v; }
Declare_Conversion(g_cmxs, kg_mxs, v)  { return 0.1 * v; }
Declare_Conversion(CP, kg_mxs, v)  { return 0.001 * v; }
Declare_Conversion(Paxs, kg_mxs, v)  { return  v; }
Declare_Conversion(lb_ftxh, kg_mxs, v) { return 0.0004133789 * v; }
Declare_Conversion(Poise, kg_mxs, v)  { return 0.1 * v; }
Declare_Conversion(mP, kg_mxs, v)  { return 0.01e-5 * v; }


// to Poise
Declare_Conversion(kg_mxs, Poise, v)  { return 10 * v ; }
Declare_Conversion(lb_ftxs, Poise, v)  { return 14.8816394388 * v ; }
Declare_Conversion(CP, Poise, v)  { return 0.01 * v ; }
Declare_Conversion(Paxs, Poise, v)  { return 10 * v ; }
Declare_Conversion(lb_ftxh, Poise, v) { return 0.0041337890 * v; }
Declare_Conversion(g_cmxs, Poise, v) { return  v; }
Declare_Conversion(mP, Poise, v) { return v/1e6; }


// to g/cm*s
Declare_Conversion(kg_mxs, g_cmxs, v)  { return 10 * v ; }
Declare_Conversion(lb_ftxs, g_cmxs, v)  { return 14.8816394388 * v ; }
Declare_Conversion(CP, g_cmxs, v)  { return 0.01 * v ; }
Declare_Conversion(Paxs, g_cmxs, v)  { return 10 * v ; }
Declare_Conversion(lb_ftxh, g_cmxs, v) { return 0.0041337890 * v; }
Declare_Conversion(Poise, g_cmxs, v) { return  v; }
Declare_Conversion(mP, g_cmxs, v)
{
  return unit_convert<Poise, g_cmxs>(unit_convert<mP, Poise>(v));
}

// to cP
Declare_Conversion(kg_mxs, CP, v)  { return 1000 * v ; }
Declare_Conversion(lb_ftxs, CP, v)  { return 1488.1639438785 * v ; }
Declare_Conversion(g_cmxs, CP, v)  { return 100 * v ; }
Declare_Conversion(Paxs, CP, v)  { return 1000 * v ; }
Declare_Conversion(lb_ftxh, CP, v) { return 0.4133789006 * v; } 
Declare_Conversion(Poise, CP, v)  { return 100 * v ; }
Declare_Conversion(mP, CP, v) { return v/1e4; }

// to Pa*s
Declare_Conversion(kg_mxs, Paxs, v)  { return  v ; }
Declare_Conversion(lb_ftxs, Paxs, v)  { return 1.4881639438785 * v ; }
Declare_Conversion(g_cmxs, Paxs, v)  { return 0.1 * v ; }
Declare_Conversion(CP, Paxs, v)  { return 0.001 * v ; }
Declare_Conversion(lb_ftxh, Paxs, v) { return 0.0004133789006 * v; } 
Declare_Conversion(Poise, Paxs, v)  { return 0.1 * v ; }
Declare_Conversion(mP, Paxs, v)
{
  return unit_convert<Poise, Paxs>(unit_convert<mP, Poise>(v));
  //return unit_convert<Poise, Paxs>(unit_convert<Paxs, Poise>(v));
}

// to lb/ft*h
Declare_Conversion(kg_mxs, lb_ftxh, v)  { return 2419.08815 * v ; }
Declare_Conversion(lb_ftxs, lb_ftxh, v)  { return 3599.9997618938 * v ; }
Declare_Conversion(CP, lb_ftxh, v)  { return 2.41908815 * v ; }
Declare_Conversion(Paxs, lb_ftxh, v)  { return 2419.08815 * v ; }
Declare_Conversion(Poise, lb_ftxh, v) { return 241.908815 * v; }
Declare_Conversion(g_cmxs, lb_ftxh, v) { return 241.908815 * v; }
Declare_Conversion(mP, lb_ftxh, v)
{
  return unit_convert<Poise, lb_ftxh>(unit_convert<mP, Poise>(v));
}

// to lb/ft*s 
Declare_Conversion(kg_mxs, lb_ftxs, v)  { return 0.6719689750 * v ; }
Declare_Conversion(g_cmxs, lb_ftxs, v)  { return 0.0671968975 * v ; }
Declare_Conversion(CP, lb_ftxs, v)  { return 0.000671968975 * v ; }
Declare_Conversion(Paxs, lb_ftxs, v)  { return 0.6719689750 * v ; }
Declare_Conversion(lb_ftxh, lb_ftxs, v) { return 0.0002777778 * v; }
Declare_Conversion(Poise, lb_ftxs, v)  { return 0.0671968975 * v ; }
Declare_Conversion(mP, lb_ftxs, v)
{
  return 6.719689751e-8 * v;
  //return unit_convert<Poise, lb_ftxs>(unit_convert<mP, Poise>(v));
}

// to mP TODO
Declare_Conversion(Poise, mP, v) { return 1e6 * v; }
Declare_Conversion(lb_ftxs, mP, v)
{
  return 14881639.436 * v;
  //return unit_convert<Poise, mP>(unit_convert<lb_ftxs, Poise>(v));
}

Declare_Conversion(CP, mP, v) { return v*1e4; }
Declare_Conversion(Paxs, mP, v)
{
  return unit_convert<Poise, mP>(unit_convert<Paxs, Poise>(v));
}

Declare_Conversion(lb_ftxh, mP, v)
{
  return unit_convert<Poise, mP>(unit_convert<lb_ftxh, Poise>(v));
}

Declare_Conversion(g_cmxs, mP, v)
{
  return unit_convert<Poise, mP>(unit_convert<g_cmxs, Poise>(v));
}

Declare_Conversion(kg_mxs, mP, v)
{
  return unit_convert<Poise, mP>(unit_convert<kg_mxs, Poise>(v));
}

# endif // Dynamic_VISCOSITY_UNIT_H 

