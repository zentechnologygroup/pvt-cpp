
# include <iostream>

# include <units/pvt-units.H>
# include <correlations/pvt-correlations.H>

int main()
{
  Correlation::list().for_each([] (auto p) { cout << *p << endl; });

  const PbAlMarhoun & c = PbAlMarhoun::get_instance();
  // c.add_parameter("p1", "degF", 40, 90);
  // c.add_parameter("p2", "psia", 4, 10);

  cout << c(0.8, 0.9, 40, 550) << endl;
  
  return 0;
}
