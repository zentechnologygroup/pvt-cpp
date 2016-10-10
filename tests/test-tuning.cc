
# include <correlations/correlation-stats.H>
# include <metadata/empirical-data.H>

int main()
{
  ifstream file("data.csv");

  EmpiricalData e;
  e.add_samples(file);

  return 0;
}
