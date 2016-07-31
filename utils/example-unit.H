
# include <tclap/CmdLine.h>
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

Quantity<Mi_h>
speed(const Quantity<Kilometer> & dist, const Quantity<Hour> & time)
{
  return Quantity<Mi_h>(dist/time);
}

void test(int argc, char *argv[])
{

}

int main(int argc, char *argv[])
{
  
  return 0;
}
