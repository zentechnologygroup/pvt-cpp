
# ifndef CORRELATION_H
# define CORRELATION_H

# include <typeinfo>
# include <sstream>

# include <ah-string-utils.H>

# include <utils.H>
# include <units.H>

struct CorrelationPar
{
  const string name;
  const Unit & unit;
  const double min_val;
  const double max_val;

  DynList<string> to_list_string() const
  {
    return { name, unit.symbol, to_string(min_val), to_string(max_val) };
  }

  CorrelationPar(const string & name, const Unit & unit,
		 double min, double max)
    : name(name), unit(unit), min_val(min), max_val(max)
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

  CorrelationPar(const string & name, BaseQuantity & q, double min, double max)
    : CorrelationPar(name, q.unit, min, max) {}    

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


class Correlation
{
  // parameters must be sorted in declaration order and the last one
  // corresponds to te return value
  const string name;
  const Unit & unit;
  const double min_val;
  const double max_val;
  DynList<CorrelationPar> pars;
  
public:

  virtual string type() const = 0;

  Correlation(const string & name, const Unit & result_unit,
	      const double min_val, const double max_val)
    : name(name), unit(result_unit), min_val(min_val), max_val(max_val) {}

  void add_parameter(const string & name, const Unit & unit,
		     double min, double max)
  {
    pars.emplace(name, unit, max, min);
  }

  void add_parameter(const string & name, const string & unit_symbol,
		     double min, double max)
  {
    auto unit_ptr = Unit::search_by_symbol(unit_symbol);
    if (unit_ptr == nullptr)
      {
	ostringstream s;
	s << "Unit symbol " << unit_symbol << " not found for parameter "
	  << name << " in correlation " << type();
	throw domain_error(s.str());
      }
    
    add_parameter(name, *unit_ptr, max, min);
  }

  string to_string() const
  {
    auto prows = pars.map<DynList<string>>([] (const auto & c)
      {
	DynList<string> ret = { "    " };
	ret.append(c.to_list_string());
	return ret;
      });

    ostringstream s;
    s << type() << endl
      << "  Name        = " << name << endl
      << "  Return type = " << unit.symbol << endl
      << "  Min result  = " << min_val << endl
      << "  Max result  = " << max_val << endl
      << "  Parametters:" << endl 
      << ::to_string(format_string(prows));

    return s.str();
  }

  friend ostream & operator << (ostream & out, const Correlation & c)
  {
    return out << c.to_string();
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
