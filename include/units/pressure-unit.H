
# ifndef PRESSURE_UNIT_H
# define PRESSURE_UNIT_H

# include "units.H"

Declare_Physical_Quantity(Pressure, "P", "P",
			  R"DESC(Pressure is the force applied perpendicular
			  to the surface of an object per unit area over which
			  that force is distributed)DESC");

Declare_Unit(Bar, "bar","bar",
	     R"DESC(A bar is a metric measurement unit of pressure. One bar
             is equivalent to ten newtons (N) per square centimeter (cm2))DESC",
	     Pressure, 0, 2068.43);

Declare_Unit(Pascal, "Pa","Pa",
	     R"DESC(The pascal is the SI derived unit of pressure used to quantify 
                    internal pressure, stress, Young's modulus and
                    ultimate tensile strength. It is defined as one newton
                    per square metre.)DESC", Pressure, 0, 206843405.31);

Declare_Unit(kPascal, "kPa","kPa",
         "a kPascal is a common multiple unit of the pascal 1 kPa = 1000 Pa,",
          Pressure, 0, 206843.41);

Declare_Unit(mPascal, "MPa","MPa",
         "a MPascal is a common multiple unit of the pascal 1 MPa = 1000000 Pa,",
          Pressure, 0, 206.843410);

Declare_Unit(psia, "psia","psia",
	     R"DESC(Pounds per square inch absolute (psia) is used to make
	            it clear that the pressure is relative to a vacuum rather than
	            the ambient atmospheric pressure. Since atmospheric pressure 
	            at sea level is around 14.7 psi)DESC", Pressure,
	     0, 29985.3);

Declare_Unit(psig, "psig","psig",
	     R"DESC(Pound-force per square inch is a unit of pressure or of
                    stress based on avoirdupois units. It is the pressure
                    resulting from a force of one pound-force applied to an
                    area of one square inch)DESC", Pressure,
	     -14.69594877551, 30000);

Declare_Unit(Atmosphere, "atm","atm",
	     R"DESC(Pound-force per square inch is a unit of pressure or of
                    stress based on avoirdupois units. It is the pressure
                    resulting from a force of one pound-force applied to 
                    an area of one square inch)DESC", Pressure, 0, 2041.39);


// pressure conversions

// To Pascal
Declare_Conversion(Atmosphere, Pascal, v) { return v * 101324.99658; }
Declare_Conversion(Bar, Pascal, v) { return  v * 100000.0; }
Declare_Conversion(psia, Pascal, v) { return  v * 6894.757293178308 ; }
Declare_Conversion(psig, Pascal, v) { return (v + 14.695948775) * 6894.757293178308; }
Declare_Conversion(kPascal, Pascal, v) { return v * 1e3; }
Declare_Conversion(mPascal, Pascal, v) { return v * 1e6; }

// To atmophere (atm)
Declare_Conversion(Bar, Atmosphere, v) { return v * 0.986923267; }
Declare_Conversion(Pascal, Atmosphere, v)  { return v * 9.86923267e-6; }
Declare_Conversion(kPascal, Atmosphere, v)  { return v * 9.86923267e-3; }
Declare_Conversion(psia, Atmosphere, v)  { return  v  / 14.69594877551; }
Declare_Conversion(psig, Atmosphere, v)  { return (v + 14.69594877551) / 14.69594877551; }
Declare_Conversion(mPascal, Atmosphere, v)
{
  return unit_convert<Pascal, Atmosphere>(unit_convert<mPascal, Pascal>(v));
}

// to Bar 
Declare_Conversion(Atmosphere, Bar, v)  { return v * 1.0132499658; }
Declare_Conversion(Pascal, Bar, v)  { return v * 1.0e-5; }
Declare_Conversion(kPascal, Bar, v)  { return v * 1.0e-2; }
Declare_Conversion(psia, Bar, v)  { return v * 0.0689475729; }
Declare_Conversion(psig, Bar, v) { return (v + 14.695948775) * 0.0689475729; }
Declare_Conversion(mPascal, Bar, v)
{
  return unit_convert<Pascal, Bar>(unit_convert<mPascal, Pascal>(v));
}

// To kPascal
Declare_Conversion(Atmosphere, kPascal, v) { return v * 101.32499658; }
Declare_Conversion(Bar, kPascal, v) { return  v * 100.0000; }
Declare_Conversion(psia, kPascal, v) { return  v * 6.894757293178308; }
Declare_Conversion(psig, kPascal, v) { return (v + 14.695948775) * 6.894757293178308 ; }
Declare_Conversion(Pascal, kPascal, v) { return v / 1000; }
Declare_Conversion(mPascal, kPascal, v) { return v * 1000; }

// To mPascal
Declare_Conversion(Atmosphere, mPascal, v) { return v * 0.10132499658; }
Declare_Conversion(Bar, mPascal, v) { return  v * 0.1; }
Declare_Conversion(psia, mPascal, v) { return  v * 6.894757293178308e-3; }
Declare_Conversion(psig, mPascal, v)
{ return ((v + 14.695948775) * 6.894757293178308) / 1000 ; }
Declare_Conversion(Pascal, mPascal, v) { return v / 1e6; }
Declare_Conversion(kPascal, mPascal, v) { return v / 1e3; 
}

// To Absolute Pound Force per Square Inch (psia)
Declare_Conversion(Atmosphere, psia, v) { return v * 14.695948775; }
Declare_Conversion(Bar, psia, v) { return v * 14.503773773; }
Declare_Conversion(Pascal, psia, v) { return v * 1.4503773773e-4; }
Declare_Conversion(kPascal, psia, v) { return v * 1.4503773773e-1; }
Declare_Conversion(psig, psia, v) { return v + 14.695948775; }
Declare_Conversion(mPascal, psia, v)
{
  const double v_pascal = unit_convert<mPascal, Pascal>(v);
  return unit_convert<Pascal, psia>(v_pascal);
}

// To Pound Force per Square Inch (psig)
Declare_Conversion(psia, psig, v) { return v - 14.695948775; }
Declare_Conversion(Atmosphere, psig, v)
{ return v * 14.695948775 - 14.695948775; }
Declare_Conversion(Bar, psig, v) { return v * 14.503773773 - 14.695948775; }
Declare_Conversion(Pascal, psig, v) { return v * 1.4503773773e-4 - 14.695948775; }
Declare_Conversion(kPascal, psig, v)
{ return v * 1.4503773773e-1 - 14.695948775; }
Declare_Conversion(mPascal, psig, v)
{
  return unit_convert<Pascal, psig>(unit_convert<mPascal, Pascal>(v));
}
  

# endif // PRESSURE_UNIT_H



