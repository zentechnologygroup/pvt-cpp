
# ifndef CORRELATION_H
# define CORRELATION_H

# include <typeinfo>
# include <sstream>

# include <ahFunctional.H>
# include <ah-string-utils.H>
# include <ah-stl-utils.H>

# include <traits.hpp>
# include <utils.H>
# include <pvt-units.H>
# include <biblio.H>

struct CorrelationPar
{
  const string name;
  const Unit & unit;
  const VtlQuantity min_val;
  const VtlQuantity max_val;

private:

  bool check_active = true;

public:

  void set_active() noexcept { check_active = true; }

  void set_inactive() noexcept { check_active = false; }

  bool is_active() const noexcept { return check_active; }

  DynList<string> to_list_string() const
  {
    return { name, unit.symbol, min_val.to_string(), max_val.to_string() };
  }

  CorrelationPar(const string & name, const Unit & unit,
		 const VtlQuantity min, const VtlQuantity max)
    : name(name), unit(unit), min_val(unit, min), max_val(unit, max) {}

  CorrelationPar(const string & name, BaseQuantity & q, double min, double max)
    : CorrelationPar(name, q.unit,
		     VtlQuantity(q.unit, min), VtlQuantity(q.unit, max)) {}

  bool check(const BaseQuantity & val) const noexcept
  {
    return val.get_value() >= min_val and val.get_value() <= max_val and
      &val.unit == &unit;
  }

  void verify(const BaseQuantity & q) const
  {
    if (not check_active)
      return;
    
    ostringstream s;
  
    if (q.get_value() < min_val)
      {
	s << "Value " << q.get_value() << " is lesser than the minimum value "
	  << min_val << " for precondition ";
	throw range_error(s.str());
      }

    if (q.get_value() > max_val)
      {
	s << "Value " << q << " is greater than the maximum value "
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
  mutable string author;
  const Unit & unit;
  const double min_val;
  const double max_val;
  mutable string title;
  mutable DynList<string> db;
  mutable DynList<string> notes;
  mutable DynList<const BibEntry*> refs;
  mutable bool hidden = false; // indicates whether correlation is or not hidden
  mutable long id = -1;

private:

  DynList<CorrelationPar> preconditions;
  size_t n = 0;

  static size_t counter;
  static DynMapTree<string, const Correlation * const> tbl;

  static Array<const Correlation * > correlations_tbl;
  
public:

  void set_hidden() { hidden = true; }

  static string to_json();

  void set_author(const string & __author) { author = __author; }

  void set_title(const string & __title) { title = __title; }

  void add_db(const string & __db) { db.append(__db); }

  void add_note(const string & note) { notes.append(note); }

  void add_ref(const string & tag)
  {
    refs.append(BibEntry::find(tag));
  }

  void inactive_preconditions()
  {
    for (auto it = preconditions.get_it(); it.has_curr(); it.next())
      it.get_curr().set_inactive();
  }

  void active_preconditions()
  {
    for (auto it = preconditions.get_it(); it.has_curr(); it.next())
      it.get_curr().set_active();
  }

  void active_precondition(size_t i)
  {
    preconditions.nth(i).set_active();
  }

  void inactive_precondition(size_t i)
  {
    preconditions.nth(i).set_inactive();
  }

  const DynList<CorrelationPar> & get_preconditions() const noexcept
  {
    return preconditions;
  }

  const DynList<const CorrelationPar * const> preconditions_list() const 
  {
    return preconditions.map<const CorrelationPar * const>([] (const auto & par)
      {
	return &par;
      });
  }

  DynList<const Unit *> get_par_types() const
  {
    return preconditions.map<const Unit *>([] (const auto & par)
						 { return &par.unit; });
  }

  size_t get_num_pars() const noexcept { return n; }

  virtual string correlation_name() const
  {
    ostringstream s;
    s << author << endl
      << type_name << " : " << subtype_name << " : "
      << demangle(typeid(*this).name());
    if (hidden)
      s << " (WARNING: hidden correlation)";
    return s.str();
  }

  DynList<VtlQuantity> double_to_pars(const DynList<double> & vals) const
  {
    return zipEq(preconditions_list(), vals).map<VtlQuantity>([] (auto p)
      {
	return VtlQuantity(p.first->name, p.second);
      });
  }

  string python_call(const DynList<VtlQuantity> & vals) const
  {
    size_t count = get_num_pars();
    ostringstream s;
    s << name << "Correlation(";
    zipEq(preconditions_list(), pointers_list(vals)).
      for_each([&s, &count] (auto p)
      {
	VtlQuantity q(p.first->unit, *p.second);
	s << q.get_value();
	if (--count)
	  s << ", ";
      });
    s << ")";

    return s.str();
  }    

  string call_string(const DynList<VtlQuantity> & vals) const
  {
    size_t count = get_num_pars();
    ostringstream s;
    s << name << "(";

    zipEq(preconditions_list(), pointers_list(vals)).
      for_each([&s, &count] (auto p)
      {
	VtlQuantity q(p.first->unit, *p.second);
	s << q;
	if (--count)
	  s << ", ";
      });
    s << ")";

    return s.str();
  }

  string call_string(const DynList<double> & vals) const
  {
    return
      call_string(zipEq(preconditions_list(), vals).map<VtlQuantity>([] (auto p)
        {
	  return VtlQuantity(p.first->unit, p.second);
	}));
  }

  static DynList<const Correlation * const> list() { return tbl.values(); }

  static const Array<const Correlation*> & array() noexcept
  {
    return correlations_tbl;
  }

  static size_t num_correlations() noexcept { return correlations_tbl.size(); }

  static const Correlation * search_by_name(const string & name)
  {
    auto ptr = tbl.search(name);
    return ptr != nullptr ? ptr->second : nullptr;
  }

  static DynList<const Correlation*> type_list()
  {

  }

  static DynList<const Correlation*> subtype_list(const string & type_name)
  {

  }

  static DynList<const Correlation*> list(const string & subtype_name)
  {

  }

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
    correlations_tbl.append(this);
    id = counter++;
  }

  Correlation(const string & type_name, const string & subtype_name,
	      const string & name, const Unit & result_unit)
    : Correlation(type_name, subtype_name, name, result_unit,
		  result_unit.min_val, result_unit.max_val) {}

  Correlation(const string & type_name, const string & subtype_name,
	      const string & name, const Unit & result_unit, double min_val)
    : Correlation(type_name, subtype_name, name, result_unit,
		  min_val, result_unit.max_val) {}

  virtual ~Correlation() = 0; // Correlation is abstract

  void add_parameter(const string & name, const Unit & unit,
		     const VtlQuantity & min, const VtlQuantity & max)
  {
    auto min_aux = VtlQuantity(unit, min);
    auto max_aux = VtlQuantity(unit, max);
    if (min_aux > max_aux)
      {
	ostringstream s;
	s << "minimum value " << min_aux << " is greater than maximum value "
	  << max_aux << " for parameter " << name;
	throw domain_error(s.str());
      }
    
    preconditions.emplace(name, unit, min_aux, max_aux);
    ++n;
  }

  void add_parameter(const string & name, const Unit & unit,
		     const VtlQuantity & min)
  {
    add_parameter(name, unit, min, VtlQuantity(unit, unit.max_val));
  }

  void add_parameter(const string & name, const Unit & unit)
  {
    add_parameter(name, unit, VtlQuantity(unit, unit.min_val),
		  VtlQuantity(unit, unit.max_val));
  }

  void add_parameter(const string & name, const Unit & unit,
		     double min, double max)
  {
    add_parameter(name, unit, VtlQuantity(unit, min), VtlQuantity(unit, max));
  }

public:

  string to_string() const
  {
    size_t i = 1;
    auto prows = preconditions.map<DynList<string>>([&i] (const auto & c)
      {
	DynList<string> ret = { "    " };
	ret.append(c.to_list_string());
	ret.append(std::to_string(i++));
	return ret;
      });

    prows.insert(rep<string>(6, ""));
    prows.insert({ "    ", "name", "unit", "min", "max", "number" });
    auto mat = format_string(prows);

    ostringstream s;
    s << correlation_name() << endl
      << "  Return type = " << unit.symbol << endl
      << "  Min result  = " << min_val << endl
      << "  Max result  = " << max_val << endl
      << endl
      << "  Parameters (" << get_num_pars() << "):" << endl
      << ::to_string(mat);

    return s.str();
  }

  string full_desc(size_t width = 60, size_t left_margin = 4) const
  {
    ostringstream s;
    if (not title.empty())
      s << align_text_to_left(title, 60) << endl
	<< endl;
    s << to_string() << endl;

    if (not db.is_empty())
      s << "  DATA BANK:" << endl
	<< endl;
    for (auto it = db.get_it(); it.has_curr(); it.next())
      s << align_text_to_left(it.get_curr(), width, left_margin) << endl
	<< endl;

    if (not notes.is_empty())
      s << "  Notes:" << endl
	<< endl;

    for (auto it = notes.get_it(); it.has_curr(); it.next())
      s << align_text_to_left(it.get_curr(), width, left_margin) << endl
	<< endl;

    if (not refs.is_empty())
      s << "  References:" << endl
	<< endl;

    for (auto it = refs.get_it(); it.has_curr(); it.next())
      s << shift_lines_to_left(it.get_curr()->to_string(), left_margin) << endl
	<< endl;

    return s.str();
  }

  friend ostream & operator << (ostream & out, const Correlation & c)
  {
    return out << c.to_string();
  }

  virtual VtlQuantity compute(const DynList<VtlQuantity> &) const = 0;

  VtlQuantity compute_and_check(const DynList<VtlQuantity> & pars) const
  {
    return verify_result(compute(pars));
  }

  double compute(const DynList<double> & values) const
  {
    auto unit_ptrs = preconditions.map<const Unit * const>([] (const auto & par)
      {
	return &par.unit;
      });
    auto pars = zipEq(unit_ptrs, values).map<VtlQuantity>([] (auto p)
      {
	return VtlQuantity(*p.first, p.second);
      });

    VtlQuantity ret = { unit, compute(pars) }; // here conversion is done
    return ret.get_value();
  }

  double compute_and_check(const DynList<double> & pars) const
  {
    return verify_result(VtlQuantity(unit, compute(pars))).get_value();
  }

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
	    VtlQuantity curr_par(precondition.unit, par); // conversion is done
	    precondition.verify(curr_par);
	    ret.append(curr_par);
	  }
	catch (exception & e)
	  {
	    ostringstream s;
	    s << e.what() << endl
	      << "Processing parameter " << i << " ("
	      << preconditions.nth(i - 1).name << ")";
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
    assert(&result.unit == &unit);
    if (result.get_value()  < min_val or result.get_value() > max_val)
      {
	ostringstream s;
	s << "In correlation " << name << " " << subtype_name << " " << type_name
	  << " : return value " << result.get_value() << " "
	  << result.unit.symbol << " is out of range ["
	  << min_val << ", " << max_val << "]" << endl
	  << "Error was detected while converting from " << result
	  << " to " << result.unit.symbol;
	throw range_error(s.str());
      }

    return result;
  }

  bool check_result(const VtlQuantity & result) const
  {
    VtlQuantity ret = result;
    return ret.get_value() >= min_val and ret.get_value() <= max_val;
  }
};

inline Correlation::~Correlation() {}

class CorrelationInvoker
{
  const Correlation * correlation_ptr = nullptr;
  char * json_buffer = nullptr;
  const size_t buf_sz = 0;
  DynList<double> pars;

public:

  CorrelationInvoker(long id, char buf[], size_t sz)
    : correlation_ptr(Correlation::array()[id]), json_buffer(buf), buf_sz(sz) {}

  void push(double val) { pars.append(val); }

  long call();
};

/// returns a proxy id for future invocation of correlation with id
extern long open_correlation_call(long id, char buf[], size_t sz);

/// pushes parameter val to the correlation proxy id. Return 1 if
/// everything is ok; 0 otherwise 
extern long push_correlation_call(long proxy_id, double val);

/// performs the call and puts in the buffer specified in
/// `open_correlation_call()` the json containing the result. Returns
/// 1 if everything is ok; 0 otherwise
extern long exec_correlation_call(long proxy_id);

/// free the proxy 
extern long free_correlation_call(long proxy_id);

# define Declare_Correlation_Type(type_name)	\
  struct type_name : public Correlation		\
  {						\
    type_name(const string & subtype_name, const string & name,		\
	      const Unit & result_unit, double min_val, double max_val)	\
      : Correlation(#type_name, subtype_name, name, result_unit,	\
		    min_val, max_val) {}				\
    virtual ~type_name() = 0;						\
    using Correlation::add_parameter;					\
  };									\
									\
  inline type_name::~type_name() {}

Declare_Correlation_Type(OilFunction)
Declare_Correlation_Type(OilCorrelation);
Declare_Correlation_Type(GasCorrelation);
Declare_Correlation_Type(GasFunction);
Declare_Correlation_Type(WaterCorrelation);

# define Declare_Correlation_Subtype(subtype_name, type_name)	\
  struct subtype_name : public type_name			\
  {								\
    subtype_name(const string & name, const Unit & result_unit,	\
		 double min_val, double max_val)	\
      : type_name(#subtype_name, name, result_unit, min_val, max_val) {} \
									\
    subtype_name(const string & name, const Unit & unit, double min_val) \
      : type_name(#subtype_name, name, unit, min_val, unit.max_val) {} \
									\
    subtype_name(const string & name, const Unit & unit)		\
      : type_name(#subtype_name, name, unit, unit.min_val, unit.max_val) {} \
    virtual ~subtype_name() = 0;					\
  protected:								\
    using type_name::add_parameter;					\
  };									\
									\
  inline subtype_name::~subtype_name() {}


template <typename F> struct compute_arity;

template <typename Ret, typename ...Args> struct compute_arity<Ret(Args...)>
{
  static constexpr std::size_t value = sizeof...(Args);
};

template <typename Func, typename T, std::size_t ... Is>
decltype(auto)
  call_compute(Func&& f, const DynList<T>& pars, std::index_sequence<Is...>)
{
  std::vector<T> v = to_vector(pars);
 
  return std::forward<Func>(f)(v.at(Is)...);
}
 
template <typename Func, typename T>
decltype(auto) call_compute(Func&& f, const DynList<T>& pars)
{
  constexpr std::size_t N =
    compute_arity<std::remove_pointer_t<std::decay_t<Func>>>::value;
  return call_compute(std::forward<Func>(f), pars, std::make_index_sequence<N>());
}

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

    // correlations & functions associated to oil
Declare_Correlation_Subtype(BubblePointPressure, OilCorrelation);
Declare_Correlation_Subtype(SolutionGasOilRelation, OilCorrelation);
Declare_Correlation_Subtype(OilFormationVolumeFactor, OilCorrelation);
Declare_Correlation_Subtype(SaturatedOilViscosity, OilCorrelation);
Declare_Correlation_Subtype(UndersaturatedOilViscosity, OilCorrelation);
Declare_Correlation_Subtype(OilCompressibility, OilCorrelation);
Declare_Correlation_Subtype(OilDensity, OilFunction);
Declare_Correlation_Subtype(OilInterfacialTension, OilCorrelation);
Declare_Correlation_Subtype(DeadOilViscosity, OilCorrelation);  


    // correlations & functions associated to gas
Declare_Correlation_Subtype(GasPseudoCriticalPressure, GasCorrelation);
Declare_Correlation_Subtype(GasPseudoCriticalPressureC7, GasCorrelation);
Declare_Correlation_Subtype(GasViscosity, GasCorrelation);
Declare_Correlation_Subtype(GasDensity, GasFunction)
Declare_Correlation_Subtype(GasVolumeFactor, GasFunction);
Declare_Correlation_Subtype(GasCompressibility, GasCorrelation);
Declare_Correlation_Subtype(GasPseudoCriticalTemperature, GasCorrelation);
Declare_Correlation_Subtype(GasIsothermalCompressibility, GasCorrelation);
Declare_Correlation_Subtype(GasPseudoReducedRatio, GasFunction);

    // correlations & functions associated to water
Declare_Correlation_Subtype(WaterDensity, WaterCorrelation);
Declare_Correlation_Subtype(WaterViscosity, WaterCorrelation);
Declare_Correlation_Subtype(WaterVolumeFactor, WaterCorrelation);

# endif // CORRELATION_H
