
# ifndef FVF_VOLUME_RATIO_UNIT_H
# define FVF_VOLUME_RATIO_UNIT_H

# include <units.H>


Declare_Physical_Quantity(FVFvolumeRatio, "FVFV_V",
     R"DESC( this is a dimensionless ratio (volume per volume) 
     that denoted The oil formation volume factor (FVF) this
     relates the volume of oil at stock-tank 
     conditions to the volume of oil at elevated 
     pressure and temperature in the reservoir. )DESC");

    
Declare_Unit(RB_STB, "RB/STB",
         R"DESC(Dimensionless volumen ratio (volume per volume) 
         measured in barrels at reservoir conditions per 
         barrel at stock-tank conditions.)DESC",
         FVFvolumeRatio, 0.5, 10);

Declare_Unit(m3_m3, "m3/m3",
         R"DESC(Dimensionless volumen ratio (volume per volume) 
         measured in cubic meters at reservoir conditions per 
         cubic meters at stock-tank conditions.)DESC",
         FVFvolumeRatio, 0.5, 10);


// The oil formation volume factor (FVF) volume conversions 
       
// To cubic meters at reservoir conditions per cubic meters at stock-tank conditions (m3/m3)
   
Declare_Conversion(RB_STB, m3_m3, v) { return  v ; }


// To  barrels at reservoir conditions per barrel at stock-tank conditions (RB/STB)

Declare_Conversion(m3_m3, RB_STB, v) { return  v ; }


# endif //  FVF_VOLUME_RATIO_UNIT_H