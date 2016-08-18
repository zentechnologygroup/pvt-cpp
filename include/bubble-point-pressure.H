
# ifndef BUBBLE_POINT_PRESSURE_H
# define BUBBLE_POINT_PRESSURE_H

# include <correlation.H>

Declare_Correlation_Subtype(BubblePointPressure, OilCorrelation);

# define Declare_Correlation(__name, subtype_name, ret_unit, min_ret, max_ret) \
  struct __name : public subtype_name					\
  {									\
									\
    static const __name & get_instance()				\
    {									\
      static __name instance;						\
      return instance;							\
    }									\
									\
  private:								\
									\
    __name() : PhysicalQuantity(#__name, symbol, desc) {}		\
									\
  public:								\
									\
    __name(const __name&) = delete;					\
    void operator = (const __name&) = delete;				\
  };

class PbAlMarhounCorrelation : public BubblePointPressure
{
  Quantity<psia> compute(const Quantity<Sgg> & yg,     // gas specific gravity 
			 const Quantity<Sg_do> & yo,    // oil specific gravity 
			 const Quantity<SCF_STB> & rsb, // Solution GOR at Pb
			 const Quantity<Rankine> & temperature) const 
  {
    double r = 0.00538088 * pow(rsb, 0.715082) * pow(yg, -1.87784);
    r *= pow(yo.get_value(), 1437);
    r += pow(temperature, 1.32657);

    return Quantity<psia>(r);
  }

public:

  PbAlMarhounCorrelation()
    : BubblePointPressure("BubblePointPressure", psia::get_instance(), 130, 3573)
  {
    add_parameter("yg", Sgg::get_instance().symbol, 0.752, 1.367);
    add_parameter("yo", Sg_do::get_instance().symbol, 0.803521, 0.937707);
    add_parameter("rsb", SCF_STB::get_instance().symbol, 26, 1602);
    add_parameter("temperature", Rankine::get_instance().symbol, 533.67, 699.67);
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);
    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & yp = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); it.next();
    const auto & temperature = it.get_curr();
    return compute(yg, yp, rsb, temperature);
  }

  Quantity<psia> operator ()
  (const Quantity<Sgg> & yg,               // gas specific gravity 
   const Quantity<Sg_do> & yo,             // oil specific gravity 
   const Quantity<SCF_STB> & rsb,          // Solution GOR at Pb
   const Quantity<Rankine> & temperature)
  {
    return compute({yg, yo, rsb, temperature});
  }
};


# endif // BUBBLE_POINT_PRESSURE_H
