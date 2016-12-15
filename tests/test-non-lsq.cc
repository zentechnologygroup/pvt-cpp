
# include <iostream>

# include <ah-zip.H>
# include <htlist.H>
# include <array_it.H>

using namespace std;

double residuals(unsigned, const double * x,
		 double *, void * pairs)
{
  const DynList<tuple<double, double>> * l =
    (const DynList<tuple<double, double>> *) pairs;
  double sum = 0;
  for (auto it = l->get_it(); it.has_curr(); it.next())
    {
      auto t = it.get_curr();
      auto s = fabs(get<0>(t) - (x[0] + x[1]*get<1>(t)));
      sum += s;
    }      

  //cout << "x[0] = " << x[0] << " x[1] = " << x[1] << endl;
  //cout << "sum = " << sum << endl;
  return sum;
}

DynList<double> rs = { 287.701, 521.132, 801.125, 967.446, 982.957, 1075.46 };
DynList<double> rsvelarde =
  { 355.914, 598.431, 853.873, 993.521, 1023.47, 1109.97 };
DynList<double> rskartoatmodjoschmidt =
  { 146.976, 294.238, 441.608, 515.32, 530.492, 573.105 };
DynList<double> rsdindorukchristman =
  { 175.269, 320.131, 459.897, 528.488, 542.516, 581.765 };


int main()
{
  auto l1 = t_zip(rs, rsvelarde);
  auto l2 = t_zip(rs, rskartoatmodjoschmidt);
  auto l3 =  t_zip(rs, rsdindorukchristman);

  double sol = 0;
  double lb[] = { -HUGE_VAL, 0};
  double ub[] = { HUGE_VAL, 1 };

  
  {
    cout << "Solving" << endl;
    l3.for_each([] (auto t)
		{ cout << "(" << get<0>(t) << "," << get<1>(t) << ")"; });
    cout << endl;
    auto opt = nlopt_create(NLOPT_LN_COBYLA, 2);
    nlopt_set_lower_bounds(opt, lb);
    nlopt_set_upper_bounds(opt, ub);
    nlopt_set_min_objective(opt, residuals, &l3);

    double x[2] = { 0, 0 };
    auto status = nlopt_optimize(opt, x, &sol);
    if (status < 0)
      cout << "NO SOLUTION: " << status << endl;

    cout << "x[0] = " << x[0] << " x[1] = " << x[1] << " sum = " << sol <<  endl
	 << endl;
  }
  return 0;

  {
    auto opt = nlopt_create(NLOPT_LN_COBYLA, 2);
    nlopt_set_lower_bounds(opt, lb);
    nlopt_set_upper_bounds(opt, ub);
    nlopt_set_min_objective(opt, residuals, &l1);

    double x[2] = { 0, 0 };
    auto status = nlopt_optimize(opt, x, &sol);
    if (status < 0)
      cout << "NO SOLUTION: " << status << endl;

    cout << "x[0] = " << x[0] << " x[1] = " << x[1] << endl
	 << endl;
  }

  {
    auto opt = nlopt_create(NLOPT_LN_COBYLA, 2);
    nlopt_set_lower_bounds(opt, lb);
    nlopt_set_upper_bounds(opt, ub);
    nlopt_set_min_objective(opt, residuals, &l2);

    double x[2] = { 0, 0 };
    auto status = nlopt_optimize(opt, x, &sol);
    if (status < 0)
      cout << "NO SOLUTION: " << status << endl;

    cout << "x[0] = " << x[0] << " x[1] = " << x[1] << endl
	 << endl;
  }

}
