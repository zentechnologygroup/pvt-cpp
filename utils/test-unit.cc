
# include <cmath>
# include <units.H>
# include "test-utils.H"

Declare_Physical_Quantity(Distance, "d",
			  "Measure about how far appart an object is");
Declare_Unit(Cm, "cm", "1/100 meter", Distance,
	     0, numeric_limits<double>::max());
Declare_Unit(Km, "km", "1000 meters", Distance,
	     0, numeric_limits<double>::max());
Declare_Unit(Mt, "mt", "Standard measure of length", Distance,
	     0, numeric_limits<double>::max());
Declare_Unit(Mi, "mi", "English unit of length", Distance,
	     0, numeric_limits<double>::max());

template <> double convert<Cm,Km>(const double & val) { return val/(1000*100); }

template <> double convert<Km, Cm>(const double & val) { return 1000*100*val; }

template <> double convert<Km, Mt>(const double & val) { return 1000*val; }

template <> double convert<Km, Mi>(const double & val) { return val/1609.344; }

template <> double convert<Mi, Km>(const double & val) { return 1609.344*val; }

Declare_Physical_Quantity(Time, "t", "The mistery of the life");
Declare_Unit(Second, "s", "base unit time", Time,
	     0, numeric_limits<double>::max());
Declare_Unit(Minute, "m", "60 seconds", Time,
	     0, numeric_limits<double>::max());
Declare_Unit(Hour, "h", "60 minutes", Time,
	     0, numeric_limits<double>::max());

template <> double
convert<Second, Minute>(const double & val) { return val/60; }

template <> double
convert<Second, Hour>(const double & val) { return val/3600; }

template <> double
convert<Hour, Second>(const double & val) { return 3600*val; }

template <> double
convert<Hour, Minute>(const double & val) { return 60*val; }

template <> double
convert<Minute, Hour>(const double & val) { return val/60; }

template <> double
convert<Minute, Second>(const double & val) { return val/60; }

Declare_Physical_Quantity(Speed, "v", "Rate of change of position");
Declare_Compound_Unit(km_h, "km/h", "Standard measure of speed", Speed,
		      0, numeric_limits<double>::max(), Km, Hour);
Declare_Compound_Unit(mt_s, "mt/s", "some more physically familiar", Speed,
		      0, numeric_limits<double>::max(), Mt, Second);
Declare_Compound_Unit(mi_h, "mi/h", "English measure of speed", Speed,
		      0, numeric_limits<double>::max(), Mi, Hour);

template <> double convert<km_h, mt_s>(const double & val)
{
  return 1000*val/3600;
}

template <> double convert<km_h, mi_h>(const double & val)
{
  return val/1609.344;
}

void test_speed()
{
  Quantity<Km> dist = 1000;
  test_assert((Quantity<Mt>) dist == Quantity<Mt>(1000*1000));
  test_assert((Quantity<Cm>) dist == 1000*1000*100);
  test_assert(Quantity<Mi>(dist) == 1000/1609.344);
}


Declare_Physical_Quantity(Temperature, "T", "Quantity of hot or cold");
Declare_Unit(Kelvin, "K", "Absolute scale of temperature", Temperature,
	     0, numeric_limits<double>::max());
Declare_Unit(Celsius, "C",
	     "Scale where 0 is the freezing point and 100 is boiling one",
	     Temperature, -273.15, numeric_limits<double>::max());
Declare_Unit(Fahrenheit, "F",
	     "Scale based on the freezing point of brine",
	     Temperature, -469.67, numeric_limits<double>::max());
Declare_Unit(Rankine, "Ra",
	     "Absolute scale of temperature", Temperature,
	     0, numeric_limits<double>::max());


int main()
{
  test_speed();

  cout << Temperature::get_instance() << endl
       << endl
       << Cm::get_instance().physical_quantity << endl
       << Cm::get_instance() << endl
       << endl;

  Quantity<Cm> dist_cm(100000);
  Quantity<Cm> d1_cm(dist_cm);

  Quantity<Km> dist_km(dist_cm);

  Quantity<Cm> d2_cm(dist_km);

  Quantity<Km> d1_km(100);

  d2_cm = d1_km;
  d2_cm = 2*d1_cm;
  d2_cm = d1_cm*2;

  // double d = d2_cm;


  cout << "cm = " << dist_cm << endl
       << "km = " << dist_km << endl
       << "d1_cm = " << d1_cm << endl
       << "d2_cm = " << d2_cm << endl;

  cout << "Physical quantities:" << endl;
  PhysicalQuantity::quantities().for_each([] (auto p)
   {
     cout << p.name << endl;
   });

  cout << endl
       << "Units:" << endl;
  units().for_each([] (auto p) { cout << p.name << endl; });
  cout << endl
       << endl;

  return 0;
}
