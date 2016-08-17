
# ifndef BUBBLE_POINT_PRESSURE_H
# define BUBBLE_POINT_PRESSURE_H

# include <correlation.H>

Declare_Correlation_Subtype(BubblePointPressure, OilCorrelation);


struct PbAlMarhounCorrelation : public BubblePointPressure
{
  PbAlMarhounCorrelation()
    : BubblePointPressure(psia::get_instance(), 130, 3573)
  {

  }

  Quantity<psia> operator ()
  (const Quantity<Sg_do> & yg, // gas specific gravity 
   const Quantity<Sg_do> & yo,
   const Quantity<SCF_STB> & rsb,
   const Quantity<Rankine> & temperature)
  {
    double r = 0.00538088 * pow(rsb, 0.715082) * pow(yg, -1.87784);
    r *= pow(yo.get_value(), 1437);
    r += pow(temperature, 1.32657);

    return Quantity<psia>(r);
  }
};


# endif // BUBBLE_POINT_PRESSURE_H
