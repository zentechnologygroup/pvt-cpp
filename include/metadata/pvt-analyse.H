# ifndef PVT_CALIBRATE_H
# define PVT_CALIBRATE_H

# include "empirical-data.H"

class PvtAnalyzer
{
  EmpiricalData data;

  void check_data() const
  {
    data.check_constant("t", Temperature::get_instance());
    data.check_constant("pb", Pressure::get_instance());
    data.check_constant("rsb", GORGLRvolumeRatio::get_instance());
  }
};

# endif //PVT_CALIBRATE_H
