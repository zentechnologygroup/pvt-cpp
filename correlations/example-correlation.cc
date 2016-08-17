
# include <iostream>

# include <pvt-units.H>
# include <pvt-correlations.H>

int main()
{
  PbAlMarhounCorrelation c;
  // c.add_parameter("p1", "degF", 40, 90);
  // c.add_parameter("p2", "psia", 4, 10);

  cout << c << endl;
  
  return 0;
}
