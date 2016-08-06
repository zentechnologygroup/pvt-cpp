
# include "test-utils.H"
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
Declare_Conversion(Centimeter, Kilometer, v) { return v/(1000*100); }
Declare_Conversion(Kilometer, Centimeter, v) { return 1000*100*v; }
Declare_Conversion(Kilometer, Meter, v) { return 1000*v; }
Declare_Conversion(Meter, Kilometer, v) { return v/1000; }
Declare_Conversion(Kilometer, Mile, v) { return v/1609.344; }
Declare_Conversion(Mile, Kilometer, v) { return 1609.344*v; }

// Time declarations
Declare_Physical_Quantity(Time, "t", "The mistery of the life");
Declare_Unit(Second, "s", "base unit time", Time,
	     0, numeric_limits<double>::max());
Declare_Unit(Minute, "m", "60 seconds", Time,
	     0, numeric_limits<double>::max());
Declare_Unit(Hour, "h", "60 minutes", Time,
	     0, numeric_limits<double>::max());
Declare_Conversion(Second, Minute, v) { return v/60; }
Declare_Conversion(Second, Hour, v) { return v/3600; }
Declare_Conversion(Hour, Second, v) { return 3600*v; }
Declare_Conversion(Hour, Minute, v) { return 60*v; }
Declare_Conversion(Minute, Hour, v) { return v/60; }
Declare_Conversion(Minute, Second, v) { return v/60; }

// Speed declarations
Declare_Physical_Quantity(Speed, "v", "Rate of change of position");
Declare_Compound_Unit(Km_h, "km/h", "Standard measure of speed", Speed,
		      0, numeric_limits<double>::max(), Kilometer, Hour);
Declare_Compound_Unit(Mt_s, "mt/s", "some more physically familiar", Speed,
		      0, numeric_limits<double>::max(), Meter, Second);
Declare_Compound_Unit(Mi_h, "mi/h", "English measure of speed", Speed,
		      0, numeric_limits<double>::max(), Mile, Hour);
//template <> double unit_convert<Km_h, Mt_s>(double v) { return 1000*v/3600; }
Declare_Conversion(Km_h, Mt_s, val) { return 1000*val/3600; }
Declare_Conversion(Km_h, Mi_h, v) { return v/1609.344; }

// Mass declarations
Declare_Physical_Quantity(Mass, "m", "Resistance of an object to acceleration");
Declare_Unit(Kilogram, "kg", "mass unit of the metric system", Mass,
	     0, numeric_limits<double>::max());
Declare_Unit(Gram, "gm", "1/1000 of a Kg", Mass,
	     0, numeric_limits<double>::max());
Declare_Unit(Pound, "lb", "0.45359237 Kg", Mass,
	     0, numeric_limits<double>::max());
Declare_Conversion(Kilogram, Gram, v) { return v/1000; }
Declare_Conversion(Kilogram, Pound, v) { return 0.45359237*v; }
Declare_Conversion(Gram, Kilogram, v) { return 1000*v; }
Declare_Conversion(Pound, Kilogram, v) { return v/0.45359237; }
Declare_Conversion(Pound, Gram, v)
{
  return unit_convert<Kilogram, Pound>(unit_convert<Pound, Kilogram>(v));
}
Declare_Conversion(Gram, Pound, v)
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
Declare_Conversion(Joule, Calorie, v) { return 4.18*v; }
Declare_Conversion(Calorie, Joule, v) { return v/4.18; }

void register_conversions()
{
   __compound_unit_tbl.items().for_each([] (auto p)
    {
      p.first.for_each([] (auto s) { cout << s << " "; });
      cout << "--> " << p.second << endl;
    });
}

void test()
{
  VtlQuantity dist("Kilometer", 1000);

  cout << dist.unit << endl;
  return;
  dist += 50;
  dist -= 50;
  dist += Quantity<Kilometer>(50);
  dist -= Quantity<Kilometer>(50);
  dist += VtlQuantity("Kilometer", 50);
  dist -= VtlQuantity("Kilometer", 50);

  test_assert(dist == 1000);
  test_assert(1000 == dist);

  VtlQuantity time("Hour", 1);
  VtlQuantity meters("Meter", 1000);
  VtlQuantity seconds("Second", 3600);

  VtlQuantity speed1 = dist/time;
  VtlQuantity speed2 = meters/seconds;
  Quantity<Km_h> speed3 = speed2;

  cout << "Speed1 = " << speed1 << endl
       << "Speed2 = " << speed2 << endl
       << "Speed2 in speed1 = " << VtlQuantity(speed2.unit.name, speed1) << endl
       << "Speed3 = " << speed3 << endl;
}

int main()
{
  register_conversions();
  test();
  return 0;
}
