
# ifndef BUBBLE_POINT_PRESSURE_H
# define BUBBLE_POINT_PRESSURE_H

# include <correlation.H>

Declare_Correlation_Subtype(BubblePointPressure, OilCorrelation);


struct PbAlMarhounCorrelation : public BubblePointPressure
{
  PbAlMarhounCorrelation()
    : BubblePointPressure(psia::get_instance(), 130, 3573)
  {
    add_parameter("yg", Sg_do::get_instance().symbol, 0.752, 1.367);
    add_parameter("yo", Sg_do::get_instance().symbol, 0.937707, 0.803521);
    add_parameter("rsb", SCF_STB::get_instance().symbol, 26, 1602);
    add_parameter("temperature", Rankine::get_instance().symbol, 533.67, 699.67);
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
