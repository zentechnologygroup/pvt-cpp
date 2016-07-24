
# include <units.H>

Declare_Physical_Quantity(Distance, "d",
			  "Measure about how far appart an object is");
Declare_Unit(Cm, "cm", "1/100 meter", Distance, 0,
	     numeric_limits<double>::max());
Declare_Unit(Km, "Km", "1000 meters", Distance, 0,
	     numeric_limits<double>::max());

template <> double
convert<Cm, Km>(const double & val , const Cm &, const Km &)
{
  return val/(1000*100);
}

template <> double
convert<Km, Cm>(const double & val , const Km &, const Cm &)
{
  return 1000*100*val;
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

Declare_Physical_Quantity(Time, "t", "The mistery of the life");

int main(int argc, char *argv[])
{
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

  cout << "cm = " << dist_cm << endl
       << "km = " << dist_km << endl
       << "d1_cm = " << d1_cm << endl
       << "d2_cm = " << d2_cm << endl;
  
  return 0;
}
