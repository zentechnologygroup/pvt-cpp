Quantity<dynes_cm>
SgwJenningsNewman__correlation__fct(const Quantity<Fahrenheit>& t,
                                    const Quantity<psia>& p)
{
  return SgwJenningsNewman::get_instance()(t, p);
}

double
SgwJenningsNewman__correlation__valid__fct(double t, double p)
{
  return SgwJenningsNewman::get_instance()(t, p).raw();
}
