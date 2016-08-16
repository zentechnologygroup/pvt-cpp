
# ifndef BUBBLE_POINT_PRESSURE_H
# define BUBBLE_POINT_PRESSURE_H

# include <correlation.H>

Declare_Correlation_Subtype(BubblePointPressure, OilCorrelation);


struct PbAlMarhounCorrelation : public BubblePointPressure
{
  using BubblePointPressure::BubblePointPressure;
};


# endif // BUBBLE_POINT_PRESSURE_H
