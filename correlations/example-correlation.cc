
# include <iostream>

# include <correlation.H>
# include <pvt-units.H>

int main()
{
  OilCorrelation c("Test", Celsius::get_instance(), 0, 200);
  c.add_parameter("p1", "degF", 40, 90);
  c.add_parameter("p2", "psia", 4, 10);

  cout << c << endl;
  
  return 0;
}
