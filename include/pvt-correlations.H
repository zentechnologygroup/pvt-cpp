# ifndef PVT_CORRELATIONS_H
# define PVT_CORRELATIONS_H

# include <bubble-point-pressure.H>

struct CorrelationInstantiater
{
  CorrelationInstantiater()
  {
    PbAlMarhounCorrelation::get_instance();
  }
};

# endif // PVT_CORRELATIONS_H
