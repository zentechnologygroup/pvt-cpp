
# include <cmath>
# include <ahSort.H>
# include <units.H>
# include "test-utils.H"

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
  Quantity<Kilometer> dist = 10;
  dist += 50;
  dist -= 10;
  dist = 10*dist;
  dist = 500 + dist; // final value must be 1000

  Quantity<Kilometer> v = { Quantity<Centimeter>(10) };

  test_assert(dist == 1000);
  test_assert(dist < 1000.001);
  test_assert(dist > 999.999);
  test_assert(dist < Quantity<Kilometer>(1000.00000001));
  test_assert(dist != 1000.0001);
  test_assert(1000 == dist);
  test_assert(1000.001 > dist);
  test_assert(999.999 < dist);
  test_assert(1000.0001 != dist);
  test_assert(dist >  Quantity<Kilometer>(999.99999999));
  test_assert(1000 >= dist);
  test_assert(dist >= 1000);
  test_assert(dist >= Quantity<Kilometer>(1000));
  test_assert(dist <= 1000);
  test_assert(1000 <= dist);
  test_assert(dist <= Quantity<Centimeter>(1000*1000*100));
  test_assert(1000.0001 >= dist);
  test_assert(dist >= 999.99999);
  test_assert(dist >= (Quantity<Centimeter>(1000*1000*100 - .000000001)));

  test_assert(dist == Quantity<Kilometer>(1000));
  test_assert(dist < Quantity<Kilometer>(1000.001));
  test_assert(dist > Quantity<Kilometer>(999.999));
  test_assert(dist != Quantity<Kilometer>(1000.0001));

  test_assert((Quantity<Meter>) dist == Quantity<Meter>(1000*1000));
  test_assert((Quantity<Centimeter>) dist ==
	      Quantity<Centimeter>(1000*1000*100));
  test_assert((Quantity<Mile>) dist == Quantity<Mile>(1000/1609.344));

  test_assert(dist == Quantity<Centimeter>(1000*1000*100));
  test_assert((Quantity<Centimeter>) dist == 1000*1000*100);
  test_assert((Quantity<Meter>) dist == 1000*1000);
  test_assert((Quantity<Mile>) dist == 1000/1609.344);
  test_assert(1000*1000*100 == (Quantity<Centimeter>) dist);
  test_assert(1000*1000 == (Quantity<Meter>) dist);
  test_assert(1000/1609.344 == (Quantity<Mile>) dist);

  Quantity<Hour> time = 1;

  test_assert((Quantity<Minute>) time == 60);
  test_assert((Quantity<Second>) time == 60*60);
  test_assert(60 == (Quantity<Minute>) time);
  test_assert(60*60 == (Quantity<Second>) time);
  test_assert(Quantity<Minute>(60) == (Quantity<Minute>) time);
  test_assert(Quantity<Second>(60*60) == (Quantity<Second>) time);

  Quantity<Km_h> speed = dist/time;

  test_assert(speed == 1000);
  test_assert(1000 == speed);
  test_assert(speed == Quantity<Km_h>(1000));
  test_assert((Quantity<Mi_h>) speed == 1000/1609.344);
  test_assert((Quantity<Mi_h>) speed != 1000/1609.34);
  test_assert(1000/1609.344 == (Quantity<Mi_h>) speed);
  test_assert(1000/1609.34 != (Quantity<Mi_h>) speed);

  Quantity<Kilogram> mass = 70; // my body mass!

  const double speed_mt_s = speed.get_value();
  Quantity<Joule> energy = (0.5*mass)*Quantity<Mt_s>(speed_mt_s*speed_mt_s); // kinetic energy

  cout << "Distance = " << dist << endl
       << "Time     = " << time << endl
       << "Speed    = " << speed << endl
       << "Energy   = " << energy << endl
       << "         = " << (Quantity<Calorie>) energy << endl
       << endl;
}

Quantity<Mi_h>
speed(const Quantity<Kilometer> & dist, const Quantity<Hour> & time)
{
  auto s = dist/time;
  return s;
}

int main()
{
  auto dist = Quantity<Meter>(1000000);
  auto time = Quantity<Minute>(60);
  auto s = speed(dist, time);
  cout << "dist = " << dist << endl
       << "time = " << time << endl
       << "speed = " << s << endl
       << endl;

  test();

  cout << "Physical Quantities:" << endl;
  PhysicalQuantity::quantities().for_each([] (auto p)
    {
      cout << "Physical quantity name   = " << p->name << endl
	   << "Physical quantity symbol = " << p->symbol << endl
	   << "Physical quantity desc   = " << p->description << endl
	   << endl;
    });
  cout << endl
       << "Units:" << endl
       << endl
       << endl;
  sort(Unit::units(), [] (auto p1, auto p2)
       { return p1->physical_quantity.name < p2->physical_quantity.name; })
    .for_each([] (auto p)
	      {
		cout << "    Unit name   = " << p->name << endl
		     << "    Unit symbol = " << p->symbol << endl
		     << "    Unit desc: " << p->description << endl
		     << "    Physical unit: " << p->physical_quantity.name << endl
		     << endl;
	      });
  cout << endl
       << endl;

  return 0;
}
