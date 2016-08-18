
# ifndef BUBBLE_POINT_PRESSURE_H
# define BUBBLE_POINT_PRESSURE_H

# include <correlation.H>

Declare_Correlation_Subtype(BubblePointPressure, OilCorrelation);


# define Correlation_Singleton(name)					\
  public:								\
									\
  static const name & get_instance()					\
  {									\
    static name instance;						\
    return instance;							\
  }									\
									\
  name(const name&) = delete;						\
  void operator = (const name&) = delete;				\
  									\
private:

template <class Corr> 
inline void instantiate_correlation()
{
  auto i = Corr::get_instance();
};

# define Instantiate_Correlation(name)		\
  extern 
 
class PbAlMarhounCorrelation : public BubblePointPressure
{
  Correlation_Singleton(PbAlMarhounCorrelation);

  Quantity<psia> compute(const Quantity<Sgg> & yg,     // gas specific gravity 
			 const Quantity<Sg_do> & yo,    // oil specific gravity 
			 const Quantity<SCF_STB> & rsb, // Solution GOR at Pb
			 const Quantity<Rankine> & temperature) const 
  {
    double r = 0.00538088 * pow(rsb, 0.715082) * pow(yg, -1.87784) *
      pow(yo, 1437) * pow(temperature, 1.32657);

    return Quantity<psia>(r);
  }

  PbAlMarhounCorrelation()
    : BubblePointPressure("BubblePointPressure", psia::get_instance(), 130, 3573)
  {
    add_parameter("yg", Sgg::get_instance().symbol, 0.752, 1.367);
    add_parameter("yo", Sg_do::get_instance().symbol, 0.803521, 0.937707);
    add_parameter("rsb", SCF_STB::get_instance().symbol, 26, 1602);
    add_parameter("temperature", Rankine::get_instance().symbol, 533.67, 699.67);
  }

public:

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);
    
    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & yp = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); it.next();
    const auto & temperature = it.get_curr();

    auto ret = compute(yg, yp, rsb, temperature);

    return verify_result(ret);
  }

  Quantity<psia> operator () 
  (const Quantity<Sgg> & yg,               // gas specific gravity 
   const Quantity<Sg_do> & yo,             // oil specific gravity 
   const Quantity<SCF_STB> & rsb,          // Solution GOR at Pb
   const Quantity<Rankine> & temperature) const
  {
    return compute({yg, yo, rsb, temperature});
  }
};


// TODO: encontrar la manera de que el singleton tenga una
// referencia. Tal vez con punero 


# endif // BUBBLE_POINT_PRESSURE_H
