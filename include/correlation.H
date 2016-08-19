
# ifndef CORRELATION_H
# define CORRELATION_H

# include <typeinfo>
# include <sstream>

# include <ah-string-utils.H>

# include <utils.H>
# include <pvt-units.H>

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

struct Correlation
{
  const string type_name;
  const string subtype_name;
  const string name;
  const Unit & unit;
  const double min_val;
  const double max_val;

private:

  DynList<CorrelationPar> preconditions;

  static DynMapTree<string, const Correlation * const> tbl;
  
public:

  const DynList<CorrelationPar> & get_preconditions() const noexcept
  {
    return preconditions;
  }

  virtual string correlation_name() const
  {
    ostringstream s;
    s << type_name << " : " << subtype_name << " : "
      << demangle(typeid(*this).name());
    return s.str();
  }

  static DynList<const Correlation * const> list() { return tbl.values(); }

protected:

  Correlation(const string & type_name, const string & subtype_name,
	      const string & name, const Unit & result_unit,
	      double min_val, double max_val)
    : type_name(type_name), subtype_name(subtype_name), name(name),
      unit(result_unit), min_val(min_val), max_val(max_val)
  {
    if (tbl.has(name))
      {
	ostringstream s;
	s << "Correlation::Correlation: duplicated correlation name" << name;
	throw domain_error(s.str());
      }

    tbl.insert(name, this);
  }

  virtual ~Correlation() = 0; // Correlation is abstract

  void add_parameter(const string & name, const Unit & unit,
		     double min, double max)
  {
    if (min >= max)
      {
	ostringstream s;
	s << "minimum value " << min << " is greater than maximum value " << max
	  << " for parameter " << name;
	throw domain_error(s.str());
      }
    
    preconditions.emplace(name, unit, min, max);
  }

  void add_parameter(const string & name, const string & unit_symbol,
		     double min, double max)
  {
    auto unit_ptr = Unit::search_by_symbol(unit_symbol);
    if (unit_ptr == nullptr)
      {
	ostringstream s;
	s << "Unit symbol " << unit_symbol << " not found for parameter "
	  << name << " in correlation " << correlation_name();
	throw domain_error(s.str());
      }

    add_parameter(name, *unit_ptr, min, max);
  }

public:

  string to_string() const
  {
    auto prows = preconditions.map<DynList<string>>([] (const auto & c)
      {
	DynList<string> ret = { "    " };
	ret.append(c.to_list_string());
	return ret;
      });

    ostringstream s;
    s << correlation_name() << endl
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

  virtual VtlQuantity compute(const DynList<VtlQuantity> &) const = 0;

  /** return a list of validated and adequately converted correlation
      parameters
      
      @throw domain_error if there is an conversion error
      @throw range_error if a parameter is out of precondition range
  */
  DynList<VtlQuantity>
  verify_preconditions(const DynList<VtlQuantity> & pars) const
  {
    using PreIt = DynList<CorrelationPar>::Iterator;
    using ParIt = DynList<VtlQuantity>::Iterator;
    Pair_Iterator<PreIt, ParIt> it(preconditions.get_it(), pars.get_it());
    DynList<VtlQuantity> ret;
    for (size_t i = 1; it.has_curr(); it.next(), ++i)
      {
	auto p = it.get_curr();
	auto precondition = p.first;
	auto par = p.second;
	try
	  {
	    VtlQuantity curr_par(par.unit, par); // conversion should be done
	    precondition.verify(curr_par);
	    ret.append(curr_par);
	  }
	catch (exception & e)
	  {
	    ostringstream s;
	    s << e.what() << endl
	      << "Processing parameter " << i;
	    throw domain_error(s.str());
	  }
      }

    if (not it.was_traversed())
      {
	ostringstream s;
	s << "number of preconditions " << preconditions.size()
	  << " is different than number of parameters " << pars.size();
	throw domain_error(s.str());
      }

    return ret;
  }

  VtlQuantity verify_result(const VtlQuantity & result) const
  {
    VtlQuantity ret = result;
    if (ret.get_value()  < min_val or ret.get_value() > max_val)
      {
	ostringstream s;
	s << "Return value is out of range [" << min_val << ", "
	  << max_val << "]";
	throw range_error(s.str());
      }

    return ret;
  }
};

inline Correlation::~Correlation() {}
//inline double Correlation::compute(const DynList<double> &) { return 0; }

# define Declare_Correlation_Type(type_name)	\
  struct type_name : public Correlation		\
  {						\
    type_name(const string & subtype_name, const string & name,		\
	      const Unit & result_unit,					\
	      const double min_val, const double max_val)		\
      : Correlation(#type_name, subtype_name, name, result_unit,	\
		    min_val, max_val) {}				\
    virtual ~type_name() = 0;						\
    using Correlation::add_parameter;					\
  };									\
									\
  inline type_name::~type_name() {}

Declare_Correlation_Type(OilCorrelation);

Declare_Correlation_Type(GasCorrelation);

Declare_Correlation_Type(WaterCorrelation);

# define Declare_Correlation_Subtype(subtype_name, type_name)	\
  struct subtype_name : public type_name			\
  {								\
    subtype_name(const string & name, const Unit & result_unit,	\
		 const double min_val, const double max_val)	\
      : type_name(#subtype_name, name, result_unit, min_val, max_val) {} \
    virtual ~subtype_name() = 0;					\
  protected:								\
    using type_name::add_parameter;					\
  };									\
									\
  inline subtype_name::~subtype_name() {}


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


# endif // endl
