
# ifndef CORRELATION_H
# define CORRELATION_H

# include <typeinfo>
# include <sstream>
# include <utils.H>
# include <units.H>

struct CorrelationPrecondition
{
  const Unit & unit;
  const double min_val;
  const double max_val;

  CorrelationPrecondition(const Unit & unit, double min, double max)
    : unit(unit), min_val(min), max_val(max)
  {
    ostringstream s;
    if (min_val < unit.min_val)
      {
	s << "Minimum value " << min_val
	  << " for precondition is lesser than the minimum value "
	  << unit.min_val << " for unit " << unit.name;
	throw range_error(s.str());
      }
    if (max_val > unit.max_val)
      {
	s << "Maximum value " << max_val
	  << " for precondition is greater than the maximum value "
	  << unit.max_val << " for unit " << unit.name;
	throw range_error(s.str());
      }
  }

  CorrelationPrecondition(const BaseQuantity & q, double min, double max)
    : CorrelationPrecondition(q.unit, min, max) {}    

  bool check(const BaseQuantity & val) const noexcept
  {
    return val.get_value() >= min_val and val.get_value() <= max_val and
      &val.unit == &unit;
  }

  void verify(const BaseQuantity & q) const
  {
    ostringstream s;
    if (&q.unit != &unit)
      {
	s << "Unit " << q.unit.name << " is different from " << unit.name;
	throw domain_error(s.str());
	
      }

    if (q.get_value() < min_val)
      {
	s << "Value " << q.get_value() << " is lesser than the minimum value "
	  << min_val << " for precondition ";
	throw range_error(s.str());
      }

    if (q.get_value() > max_val)
      {
	s << "Value " << q.get_value() << " is greater than the minimum value "
	  << max_val << " for precondition ";
	throw range_error(s.str());
      }
  }
};


struct Correlation
{
  const string name;
  const Unit & result_unit;
  
  Correlation(const string & name, const Unit & result_unit)
    : name(name), result_unit(result_unit) {}

  virtual string type() const = 0;

  string to_string() const
  {
    ostringstream s;
    s << "Correlation " << name << " of type " << type()
      << " with result type " << result_unit;
    return s.str();
  }
};

# define Correlation_Type() \
  virtual string type() const { return demangle(typeid(*this).name()); }

struct OilCorrelation : public Correlation
{
  Correlation_Type();
  using Correlation::Correlation;
};


struct GasCorrelation : public Correlation
{
  Correlation_Type();
  using Correlation::Correlation;
};


struct WaterCorrelation : public Correlation
{
  Correlation_Type();
  using Correlation::Correlation;
};



# endif // endl
