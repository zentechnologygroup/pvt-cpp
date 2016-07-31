
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
template <> double convert<Centimeter, Kilometer>(double v) { return v/(1000*100); }
template <> double convert<Kilometer, Centimeter>(double v) { return 1000*100*v; }
template <> double convert<Kilometer, Meter>(double val) { return 1000*val; }
template <> double convert<Meter, Kilometer>(double val) { return val/1000; }
template <> double convert<Kilometer, Mile>(double val) { return val/1609.344; }
template <> double convert<Mile, Kilometer>(double val) { return 1609.344*val; }

// Time declarations
Declare_Physical_Quantity(Time, "t", "The mistery of the life");
Declare_Unit(Second, "s", "base unit time", Time,
	     0, numeric_limits<double>::max());
Declare_Unit(Minute, "m", "60 seconds", Time,
	     0, numeric_limits<double>::max());
Declare_Unit(Hour, "h", "60 minutes", Time,
	     0, numeric_limits<double>::max());
template <> double convert<Second, Minute>(double val) { return val/60; }
template <> double convert<Second, Hour>(double val) { return val/3600; }
template <> double convert<Hour, Second>(double val) { return 3600*val; }
template <> double convert<Hour, Minute>(double val) { return 60*val; }
template <> double convert<Minute, Hour>(double val) { return val/60; }
template <> double convert<Minute, Second>(double val) { return val/60; }

// Speed declarations
Declare_Physical_Quantity(Speed, "v", "Rate of change of position");
Declare_Compound_Unit(Km_h, "km/h", "Standard measure of speed", Speed,
		      0, numeric_limits<double>::max(), Kilometer, Hour);
Declare_Compound_Unit(Mt_s, "mt/s", "some more physically familiar", Speed,
		      0, numeric_limits<double>::max(), Meter, Second);
Declare_Compound_Unit(Mi_h, "mi/h", "English measure of speed", Speed,
		      0, numeric_limits<double>::max(), Mile, Hour);
template <> double convert<Km_h, Mt_s>(double val) { return 1000*val/3600; }
template <> double convert<Km_h, Mi_h>(double val) { return val/1609.344; }

// Mass declarations
Declare_Physical_Quantity(Mass, "m", "Resistance of an object to acceleration");
Declare_Unit(Kilogram, "kg", "mass unit of the metric system", Mass,
	     0, numeric_limits<double>::max());
Declare_Unit(Gram, "gm", "1/1000 of a Kg", Mass, 0, numeric_limits<double>::max());
Declare_Unit(Pound, "lb", "0.45359237 Kg", Mass, 0, numeric_limits<double>::max());
template <> double convert<Kilogram, Gram>(double val) { return val/1000; }
template <> double convert<Kilogram, Pound>(double val) { return 0.45359237*val; }
template <> double convert<Gram, Kilogram>(double val) { return 1000*val; }
template <> double convert<Pound, Kilogram>(double val) { return val/0.45359237; }
template <> double convert<Pound, Gram>(double val)
{
  return convert<Kilogram, Pound>(convert<Pound, Kilogram>(val));
}
template <> double convert<Gram, Pound>(double val)
{
  return convert<Kilogram, Pound>(convert<Gram, Kilogram>(val));
}

Declare_Physical_Quantity(Energy, "E",
			  R"(Very mysterious property of something that can be
			    transfered o transformed in order to do some work)");
Declare_Compound_Unit(Joule, "J", "Energy unit in ISU", Energy, 0,
 		      numeric_limits<double>::max(), Kilogram, Mt_s);
Declare_Unit(Calorie, "Cal", "Calorie", Energy, 0, numeric_limits<double>::max());
template <> double convert<Joule, Calorie>(double val) { return 4.18*val; }
template <> double convert<Calorie, Joule>(double val) { return val/4.18; }

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
  test_assert((Quantity<Centimeter>) dist == Quantity<Centimeter>(1000*1000*100));
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

  Quantity<Mt_s> speed_mt_s = { speed };
  Quantity<Joule> energy = (0.5*mass)*(speed_mt_s*speed_mt_s); // kinetic energy

  cout << "Distance = " << dist << endl
       << "Time     = " << time << endl
       << "Speed    = " << speed << endl
       << "Energy   = " << energy << endl
       << "         = " << (Quantity<Calorie>) energy << endl
       << endl;
}

Quantity<Mi_h> speed(const Quantity<Kilometer> & dist, const Quantity<Hour> & time)
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
