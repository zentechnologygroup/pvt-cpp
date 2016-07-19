
# ifndef TEMPERATURE_UNIT_H
# define TEMPERATURE_UNIT_H

# include <units.H>

struct Temperature : public PhysicalQuantity
{
  Temperature()
    : PhysicalQuantity("Temperature",
		       "Objetive and comparative measure of hot or cold", "T")
  {}
};

struct Kelvin : public Unit
{
  Kelvin()
    : Unit(
  {
    min_val = 0;
    max_val = numeric_limits<double>::max();
  }
};


# endif // TEMPERATURE_UNIT_H
