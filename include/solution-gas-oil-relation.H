# ifndef SOLUTION_GAS_OIL_RELATION_H
# define SOLUTION_GAS_OIL_RELATION_H

# include <correlation.H>

Declare_Correlation_Subtype(SolutionGasOilRelation, OilCorrelation);

class RsAlShammasi : public SolutionGasOilRelation
{
  Correlation_Singleton(RsAlShammasi);

  RsAlShammasi()
    : SolutionGasOilRelation("RsAlShammasi", SCF_STB::get_instance(), 6, 3298.6)
  {
    add_parameter("yg", Sgg::get_instance(), 0.51, 3.44);
    add_parameter("pb", psia::get_instance(), 31.7, 7127);
    add_parameter("p", psia::get_instance(), Quantity<Atmosphere>(1));

    // min and max of api are put inverted because Sg_do unit is inverse
    add_parameter("yo", Sg_do::get_instance(),
		  Quantity<Api>(63.7), Quantity<Api>(6));
    
    add_parameter("t", Fahrenheit::get_instance(), 74, 341.6);
    add_parameter("rsb", SCF_STB::get_instance());
  }

  Quantity<SCF_STB> compute(const Quantity<Sgg> & yg,
			    const Quantity<psia> & pb,
			    const Quantity<psia> & p,
			    const Quantity<Sg_do> & yo,
			    const Quantity<Fahrenheit> & t,
			    const Quantity<SCF_STB> & rsb) const
  {
    if (p >= pb)
      return rsb;
    
    constexpr double c1 = 5.527215;
    constexpr double c2 = -1.841408;
    constexpr double c3_inv = 1/0.783716;

    const double raw_yo = yo.get_value();
    const double raw_yg = yg.get_value();
    const double raw_p  = p.get_value();

    const double rs =
      pow(raw_p/(pow(raw_yo, c1)*exp(c2*floor(raw_yo*raw_yg))), c3_inv) /
      ((t.get_value() + 460) * raw_yg); 

 // / ((T + 460) * Yg)


 //  ((P / (Yo**c1 * exp(c2 * floor(Yo * Yg)) )) ** (1/c3)) / ((T + 460) * Yg)

 //    const double rs =
 //      pow(raw_p/pow(raw_yo, c1) * exp(c2*floor(raw_yo*raw_yg)), c3_inv) /
 //      ((t.get_value() + 460) * raw_yg);

    return Quantity<SCF_STB>(rs);
  }

public:

  virtual VtlQuantity compute(const DynList<VtlQuantity> & __pars) const
  {
    auto pars = verify_preconditions(__pars);
    
    auto it = pars.get_it();
    const auto & yg = it.get_curr(); it.next();
    const auto & pb = it.get_curr(); it.next();
    const auto & p = it.get_curr(); it.next();
    const auto & yo = it.get_curr(); it.next();
    const auto & t = it.get_curr(); it.next();
    const auto & rsb = it.get_curr(); 

    return compute(yg, pb, p, yo, t, rsb);
  }

  Quantity<SCF_STB> operator () (const Quantity<Sgg> & yg,
				 const Quantity<psia> & pb,
				 const Quantity<psia> & p,
				 const Quantity<Sg_do> & yo,
				 const Quantity<Fahrenheit> & t,
				 const Quantity<SCF_STB> & rsb) const
  {
    return compute({yg, pb, p, yo, t, rsb});
  }
};

# endif // SOLUTION_GAS_OIL_REKATION_H
