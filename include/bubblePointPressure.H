
# ifndef BUBBLEPOINTPRESSURE_H
# define BUBBLEPOINTPRESSURE_H

# include <correlation.H>

struct BubblePointPressure : public OilCorrelation
{
  using OilCorrelation::OilCorrelation;
};

struct PbAlMarhounCorrelation : public BubblePointPressure
{
  
};


# endif // BUBBLEPOINTPRESSURE_H
