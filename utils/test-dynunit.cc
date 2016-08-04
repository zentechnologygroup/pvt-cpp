
# include <units.H>

    // Distance declarations
Declare_Physical_Quantity(Distance, "d",
			  "Measure about how far appart an object is");
Declare_Unit(Centimeter, "cm", "1/100 meter", Distance,
	     0, numeric_limits<double>::max());
Declare_Unit(Kilometer, "km", "1000 meters", Distance,
	     0, numeric_limits<double>::max());
Declare_Unit(Meter, "mt", "Standard measure of length", Distance,
	     0, numeric_limits<double>::max());
Declare_Unit(Mile, "mi", "English unit of length", Distance,
	     0, numeric_limits<double>::max());
template <> double
unit_convert<Centimeter, Kilometer>(double v) { return v/(1000*100); }
template <> double
unit_convert<Kilometer, Centimeter>(double v) { return 1000*100*v; }
template <> double unit_convert<Kilometer, Meter>(double v) { return 1000*v; }
template <> double unit_convert<Meter, Kilometer>(double v) { return v/1000; }
template <> double
unit_convert<Kilometer, Mile>(double v) { return v/1609.344; }
template <> double
unit_convert<Mile, Kilometer>(double v) { return 1609.344*v; }

// Time declarations
Declare_Physical_Quantity(Time, "t", "The mistery of the life");
Declare_Unit(Second, "s", "base unit time", Time,
	     0, numeric_limits<double>::max());
Declare_Unit(Minute, "m", "60 seconds", Time,
	     0, numeric_limits<double>::max());
Declare_Unit(Hour, "h", "60 minutes", Time,
	     0, numeric_limits<double>::max());
template <> double unit_convert<Second, Minute>(double v) { return v/60; }
template <> double unit_convert<Second, Hour>(double v) { return v/3600; }
template <> double unit_convert<Hour, Second>(double v) { return 3600*v; }
template <> double unit_convert<Hour, Minute>(double v) { return 60*v; }
template <> double unit_convert<Minute, Hour>(double v) { return v/60; }
template <> double unit_convert<Minute, Second>(double v) { return v/60; }

// Speed declarations
Declare_Physical_Quantity(Speed, "v", "Rate of change of position");
Declare_Compound_Unit(Km_h, "km/h", "Standard measure of speed", Speed,
		      0, numeric_limits<double>::max(), Kilometer, Hour);
Declare_Compound_Unit(Mt_s, "mt/s", "some more physically familiar", Speed,
		      0, numeric_limits<double>::max(), Meter, Second);
Declare_Compound_Unit(Mi_h, "mi/h", "English measure of speed", Speed,
		      0, numeric_limits<double>::max(), Mile, Hour);
template <> double unit_convert<Km_h, Mt_s>(double v) { return 1000*v/3600; }
template <> double unit_convert<Km_h, Mi_h>(double v) { return v/1609.344; }

// Mass declarations
Declare_Physical_Quantity(Mass, "m", "Resistance of an object to acceleration");
Declare_Unit(Kilogram, "kg", "mass unit of the metric system", Mass,
	     0, numeric_limits<double>::max());
Declare_Unit(Gram, "gm", "1/1000 of a Kg", Mass,
	     0, numeric_limits<double>::max());
Declare_Unit(Pound, "lb", "0.45359237 Kg", Mass,
	     0, numeric_limits<double>::max());
template <> double unit_convert<Kilogram, Gram>(double v) { return v/1000; }
template <> double
unit_convert<Kilogram, Pound>(double v) { return 0.45359237*v; }
template <> double
unit_convert<Gram, Kilogram>(double v) { return 1000*v; }
template <> double unit_convert<Pound, Kilogram>(double v) { return v/0.45359237; }
template <> double unit_convert<Pound, Gram>(double v)
{
  return unit_convert<Kilogram, Pound>(unit_convert<Pound, Kilogram>(v));
}
template <> double unit_convert<Gram, Pound>(double v)
{
  return unit_convert<Kilogram, Pound>(unit_convert<Gram, Kilogram>(v));
}

Declare_Physical_Quantity(Energy, "E",
			  R"(Very mysterious property of something that can be
			    transfered o transformed in order to do some work)");
Declare_Compound_Unit(Joule, "J", "Energy unit in ISU", Energy, 0,
 		      numeric_limits<double>::max(), Kilogram, Mt_s);
Declare_Unit(Calorie, "Cal", "Calorie", Energy, 0,
	     numeric_limits<double>::max());
template <> double unit_convert<Joule, Calorie>(double v) { return 4.18*v; }
template <> double unit_convert<Calorie, Joule>(double v) { return v/4.18; }

void test()
{
  Unit_Converter<Joule, Calorie>(); 
  Unit_Converter<Calorie, Joule>();
  
}

int main(int argc, char *argv[])
{
  test();
  return 0;
}
