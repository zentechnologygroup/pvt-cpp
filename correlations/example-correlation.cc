
# include <iostream>

# include <correlation.H>
# include <pvt-units.H>

int main()
{
  OilCorrelation c("Test", Celsius::get_instance(), 0, 200);

  cout << c << endl;
  
  return 0;
}
