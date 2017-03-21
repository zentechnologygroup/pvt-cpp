
# include <correlations/defined-correlation.H>

int main()
{
  DefinedCorrelation corr("p", psia::get_instance());

  corr.add_correlation(nullptr, 21, 30);
  corr.add_correlation(nullptr, 10, 20);
  corr.add_correlation(nullptr, 0, 3);
  try
    {
      corr.add_correlation(nullptr, 2, 2);
      assert(false);
    }
  catch (domain_error) {}

  corr.interval_list().for_each([] (const auto & i)
    {
      cout << "[" << i.start << ", " << i.end << "]" << endl;
    });

  for (double i = 0; i < 32; ++i)
    {
      auto interval = corr.search_interval(i);
      cout << "v = " << i << " : ";
      if (interval)
	cout << "[" << interval->start << ", " << interval->end << "]" << endl;
      else
	cout << " not found" << endl; 
    }
  
  return 0;
}
