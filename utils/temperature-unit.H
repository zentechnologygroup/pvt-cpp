
# ifndef TEMPERATURE_UNIT_H
# define TEMPERATURE_UNIT_H

# include <units.H>

struct Temperature 
{
  PhysicalQuantity * physical_quantity_ptr = nullptr;
};

struct Kelvin : public Temperature
{
  Unit 
  Kelvin()
  // falta
  {

  }
};


# endif // TEMPERATURE_UNIT_H
