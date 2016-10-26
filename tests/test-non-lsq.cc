
# include <nlopt.h>

# include <iostream>

# include <ah-zip.H>
# include <htlist.H>
# include <array_it.H>

using namespace std;

double residuals1(unsigned n, const double * x,
		 double * grad, void * pairs)
{
  const DynList<tuple<double, double>> * l =
    (const DynList<tuple<double, double>> *) pairs;
  double sum = 0;
  double g = 0;
  grad[0] = 0;
  for (auto it = l->get_it(); it.has_curr(); it.next())
    {
      auto t = it.get_curr();
      auto s = fabs(get<0>(t) - (x[0] + x[1]*get<1>(t)));
      g += x[1]/fabs(x[1]);
      sum += s;
    }      

  grad[1] = g;

  //cout << "sum = " << sum << endl;
  return sum;
}

double residuals2(unsigned n, const double * x,
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

  //cout << "sum = " << sum << endl;
  return sum;
}

DynList<double> rs = { 287.701, 521.132, 801.125, 967.446, 982.957, 1075.46 };
DynList<double> rsvelarde =
  { 355.914, 598.431, 853.873, 993.521, 1023.47, 1109.97 };
DynList<double> rskartoatmodjoschmidt =
  { 146.976, 294.238, 441.608, 515.32, 530.492, 573.105 };


int main()
{
  auto l1 = t_zip(rs, rsvelarde);
  auto l2 = t_zip(rs, rskartoatmodjoschmidt);
  double sol[2] = { -1, -10 };
    double lb[] = { -HUGE_VAL, 0};
  double ub[] = { HUGE_VAL, 1 };

  {
    auto opt = nlopt_create(NLOPT_LD_MMA, 2);
    nlopt_set_lower_bounds(opt, lb);
    nlopt_set_upper_bounds(opt, ub);
    nlopt_set_min_objective(opt, residuals1, &l1);

    double x[2] = { 0, 0 };
    auto status = nlopt_optimize(opt, x, sol);
    if (status < 0)
      cout << "NO SOLUTION: " << status << endl;

    cout << "x[0] = " << x[0] << " x[1] = " << x[1] << endl;
  }

  cout << endl; return 0;

  {
    auto opt = nlopt_create(NLOPT_GD_MLSL, 2);
    nlopt_set_lower_bounds(opt, lb);
    nlopt_set_upper_bounds(opt, ub);
    nlopt_set_min_objective(opt, residuals2, &l2);

    double x[2] = { 0, 0 };
    auto status = nlopt_optimize(opt, x, sol);
    if (status < 0)
      cout << "NO SOLUTION: " << status << endl;

    cout << "x[0] = " << x[0] << " x[1] = " << x[1] << endl;
  }


}
