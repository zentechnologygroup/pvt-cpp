# ifndef WATER_SPECIFIC_GRAVITY_UNIT_H
# define WATER_SPECIFIC_GRAVITY_UNIT_H

# include <cmath>
# include <units.H>

// SOURCE: Effect of Salinity on the density of brine (Data from
// international Critical Tables ii 1927, 79)  

// SOURCE: McCain W.D, Jr: The properties of Petroleum Fluid 2nd. ed
// Tulsa, OK PennWell

// SOURCE: Carlos Banzer S, Correlaciones Numericas P.V.T. Instituto
// de Investigaciones Petroleras Universidad del Zulia   

Declare_Physical_Quantity(WaterSpecicGravity, "Sgw",
	    R"DESC(density of a brine (in this case formation 
        water) in normal conditions based on total
        solid disolved, it is defined as the ratio
        of the density of the brine to the density
        pure water, both taken at the same pressure
        and temperature (usually atmopheric pressure
        to the seal level and 60 degF) )DESC");

Declare_Unit(Pwl_lb_ft3, "pwl_lb/ft3",
	     R"DESC(unit used to measure desity of produce water (brine) 
         in normal conditions denoted as pound per cubic feet)DESC",
	     WaterSpecicGravity, 62.366389027, 75);

Declare_Unit(Sgw_Percent, "sgwPercent",
	     R"DESC(Unit used to measure salinity of a brine
	     in normal conditions in percentage of disolved solid)DESC",
	     WaterSpecicGravity, 0, 26.28319558586105);

Declare_Unit(Sgw_sg, "swsg",
	     R"DESC(Specific gravity of a brine in normal conditions, it is
         the ratio of the density of a brine to the density of a
         pure water;)DESC", WaterSpecicGravity, 1, 1.2025708265317492);

Declare_Unit(Sgw_PPM, "swppm",
	     R"DESC(Total solid disolved in a brine
	     measure in Part Per Million PPM)DESC",
	     WaterSpecicGravity, 0, 262831.9558586105);

//To Water Specific Gravity in lb/ft3
Declare_Conversion(Sgw_Percent, Pwl_lb_ft3, v)
  { return 62.366389027 + 0.438603*v +  0.00160074*v*v ;}

Declare_Conversion(Sgw_PPM, Pwl_lb_ft3, v)
{ return 62.366389027 + 0.438603*(v/10000) +  0.00160074*(v/10000)*(v/10000) ; }

Declare_Conversion(Sgw_sg, Pwl_lb_ft3, v) { return v * 62.366389027; }


//To Water Specific Gravity Fraction
Declare_Conversion(Sgw_Percent, Sgw_sg, v)
  { return (62.368 + 0.438603*v +  0.00160074*v*v) / 62.366389027 ;  }

Declare_Conversion(Sgw_PPM, Sgw_sg, v)
{
  return (62.366389027 + 0.438603*(v/10000) + 0.00160074*(v/10000)*(v/10000)) /
    62.366389027;
}

Declare_Conversion(Pwl_lb_ft3, Sgw_sg, v) { return  v / 62.366389027;  }


//To Water Salinity in PPM
Declare_Conversion(Pwl_lb_ft3, Sgw_PPM, v)
{
  return (-0.4386 + sqrt(0.19236995999999998 - 0.00640296*(62.366389027-v))) /
    (0.00320148) * 10000 ;
}

Declare_Conversion(Sgw_sg, Sgw_PPM, v)
{
  double n = -0.4386 +
    sqrt(0.19236995999999998 - 0.00640296*(62.366389027 -v*62.366389027));
  return n / (0.00320148) * 10000 ;
}

Declare_Conversion( Sgw_Percent, Sgw_PPM, v) { return  v * 10000  ; }    

//To Water SalinityPercent
Declare_Conversion(Pwl_lb_ft3, Sgw_Percent, v)
{
  double n = -0.4386 + sqrt(0.19236995999999998 - 0.00640296*(62.366389027 - v));
  return n / (0.00320148);
}

Declare_Conversion(Sgw_sg, Sgw_Percent, v)
{
  double n = -0.4386 +
    sqrt(0.19236995999999998 - 0.00640296*(62.366389027-v*62.366389027));

  return n / (0.00320148);
}

Declare_Conversion(Sgw_PPM, Sgw_Percent, v) { return v/10000; }

# endif // WATER_SPECIFIC_GRAVITY_UNIT_H
