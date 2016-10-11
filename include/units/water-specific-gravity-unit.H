# ifndef WATER_SPECIFIC_GRAVITY_UNIT_H
# define WATER_SPECIFIC_GRAVITY_UNIT_H

# include <cmath>
# include "units.H"

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
	     R"DESC(unit used to measure density of produce water (brine) 
         in normal conditions denoted as pound per cubic feet)DESC",
	     WaterSpecicGravity, 62.366389027, 75);

Declare_Unit(Disolved_Salt_Percent, "disolvedSaltPercent",
	     R"DESC(Unit used to measure salinity of a brine
	     in normal conditions in percentage of disolved salt)DESC",
	     WaterSpecicGravity, 0, 26.28319558586105);

Declare_Unit(Sgw_sg, "swsg",
	     R"DESC(Specific gravity of a brine in normal conditions, it is
         the ratio of the density of a brine to the density of a
         pure water;)DESC", WaterSpecicGravity, 1, 1.2025708265317492);

Declare_Unit(Disolved_Salt_PPM, "disolvedSaltPPM",
	     R"DESC(Total solid disolved in a brine
	     measure in Part Per Million PPM)DESC",
	     WaterSpecicGravity, 0, 262831.9558586105);

Declare_Unit(Molality_NaCl, "mol_NaCl/Kg_H2O", "Molality of sodium chloride",
  	     WaterSpecicGravity, 0, 6.10071379841343394345);

// Declare_Unit(Disolved_Salt_Fraction, "disolvedSaltFraction", "TODO",
// 	     0, 0.2628319558586105);

// To Molality_NaCl
Declare_Conversion(Disolved_Salt_Percent, Molality_NaCl, s)
{
  return 1000*(s/100) / (58.4428*(1 - s/100));
}

Declare_Conversion(Molality_NaCl, Disolved_Salt_Percent, m)
{
  constexpr double k = 58.4428;
  return m*k / (10 + m*k/100);
}

//To Water Specific Gravity in lb/ft3
Declare_Conversion(Disolved_Salt_Percent, Pwl_lb_ft3, v)
  { return 62.366389027 + 0.438603*v +  0.00160074*v*v ;}

Declare_Conversion(Disolved_Salt_PPM, Pwl_lb_ft3, v)
{ return 62.366389027 + 0.438603*(v/10000) +  0.00160074*(v/10000)*(v/10000) ; }

Declare_Conversion(Sgw_sg, Pwl_lb_ft3, v) { return v * 62.366389027; }

Declare_Conversion(Molality_NaCl, Pwl_lb_ft3, v)
{
  return unit_convert<Disolved_Salt_Percent, Pwl_lb_ft3>
    (unit_convert<Molality_NaCl, Disolved_Salt_Percent>(v));
}

//To Water Salinity in PPM
Declare_Conversion(Pwl_lb_ft3, Disolved_Salt_PPM, v)
{
  return (-0.4386 + sqrt(0.19236995999999998 - 0.00640296*(62.366389027-v))) /
    (0.00320148) * 10000 ;
}

Declare_Conversion(Sgw_sg, Disolved_Salt_PPM, v)
{
  double n = -0.4386 +
    sqrt(0.19236995999999998 - 0.00640296*(62.366389027 -v*62.366389027));
  return n / (0.00320148) * 10000 ;
}

Declare_Conversion(Disolved_Salt_Percent, Disolved_Salt_PPM, v)
{ return  v * 10000  ; }    

Declare_Conversion(Molality_NaCl, Disolved_Salt_PPM, v)
{
  return unit_convert<Disolved_Salt_Percent, Disolved_Salt_PPM>
    (unit_convert<Molality_NaCl, Disolved_Salt_Percent>(v));
}


//To Water Specific Gravity Fraction
Declare_Conversion(Disolved_Salt_Percent, Sgw_sg, v)
  { return (62.368 + 0.438603*v +  0.00160074*v*v) / 62.366389027 ;  }

Declare_Conversion(Disolved_Salt_PPM, Sgw_sg, v)
{
  return (62.366389027 + 0.438603*(v/10000) + 0.00160074*(v/10000)*(v/10000)) /
    62.366389027;
}

Declare_Conversion(Pwl_lb_ft3, Sgw_sg, v) { return  v / 62.366389027;  }

Declare_Conversion(Molality_NaCl, Sgw_sg, v)
{
  return unit_convert<Disolved_Salt_Percent, Sgw_sg>
    (unit_convert<Molality_NaCl, Disolved_Salt_Percent>(v));
}

//To Water SalinityPercent
Declare_Conversion(Pwl_lb_ft3, Disolved_Salt_Percent, v)
{
  double n = -0.4386 + sqrt(0.19236995999999998 - 0.00640296*(62.366389027 - v));
  return n / (0.00320148);
}

Declare_Conversion(Sgw_sg, Disolved_Salt_Percent, v)
{
  double n = -0.4386 +
    sqrt(0.19236995999999998 - 0.00640296*(62.366389027-v*62.366389027));

  return n / (0.00320148);
}

Declare_Conversion(Disolved_Salt_PPM, Disolved_Salt_Percent, v) { return v/10000; }

// to Sodium chloride Molality_NaCl
Declare_Conversion(Pwl_lb_ft3, Molality_NaCl, v)
{
  return unit_convert<Disolved_Salt_Percent, Molality_NaCl>
    (unit_convert<Pwl_lb_ft3, Disolved_Salt_Percent>(v));
}

Declare_Conversion(Disolved_Salt_PPM, Molality_NaCl, v)
{
  return unit_convert<Disolved_Salt_Percent, Molality_NaCl>
    (unit_convert<Disolved_Salt_PPM, Disolved_Salt_Percent>(v));
}

Declare_Conversion(Sgw_sg, Molality_NaCl, v)
{
  return unit_convert<Disolved_Salt_Percent, Molality_NaCl>
    (unit_convert<Sgw_sg, Disolved_Salt_Percent>(v));
}


# endif // WATER_SPECIFIC_GRAVITY_UNIT_H
