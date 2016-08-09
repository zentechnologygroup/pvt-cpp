
# ifndef UNITS_H
# define UNITS_H

# include <memory>
# include <typeinfo>
# include <type_traits>
# include <string>
# include <sstream>
# include <limits>

# include <tpl_dynSetHash.H>
# include <tpl_dynMapTree.H>

# include <unititem.H>

using namespace std;

/** Defines a physical magnitude

     @author Leandro Rabindranath Leon
 */
class PhysicalQuantity : public UnitItem
{
  using UnitItem::UnitItem;
  static UnitItemTable tbl;

protected:

  PhysicalQuantity(const string & name,
		   const string & symbol,
		   const string & desc)
    : UnitItem(name, symbol, desc)
  {
    tbl.register_item(this);
  }

public:

  // Return all the defined  physical magnitudes  
  static DynList<const PhysicalQuantity *> quantities()
  {
    return tbl.items().map<const PhysicalQuantity*>([] (auto p)
      {
	return static_cast<const PhysicalQuantity*>(p);
      });
  }
};

/** Defines a new physical magnitude

    @param[in] name of the new physical quantity
    @param[in] symbol string used as unit symbol
    @param[in] desc description 

 */
# define Declare_Physical_Quantity(__name, symbol, desc)	\
  struct __name : public PhysicalQuantity			\
  {								\
    								\
    static const __name & get_instance()			\
    {								\
      static __name instance;					\
      return instance;						\
    }								\
    								\
  private:							\
    								\
    __name() : PhysicalQuantity(#__name, symbol, desc) {}	\
    								\
  public:							\
    								\
    __name(const __name&) = delete;				\
    void operator = (const __name&) = delete;			\
  };

/** Unit base class

 */
struct Unit : public UnitItem
{
  const PhysicalQuantity & physical_quantity;
  const double min_val = 0;
  const double max_val = 0;

  string to_string() const
  {
    ostringstream s;
    s << "Unit name         = " << name << endl
      << "symbol            = " << symbol << endl
      << "physical quantity = " << physical_quantity.name << endl
      << "min               = " << min_val << endl
      << "max               = " << max_val;
    return s.str();
  }

  friend ostream & operator << (ostream & out, const Unit & unit)
  {
    return out << unit.to_string();
  }

private:

  static UnitItemTable tbl;
  static DynSetTree<const Unit *> unit_tbl;

public:

  /// Return a list of all declared and used units
  static DynList<const Unit*> units() { return unit_tbl.keys(); }

  /** Search the unit associated to a name
      
      @param[in] name string containing the unit name
      @return constant pointer to the symbol. If the name is not
      found, then `nullptr` is returned
  */
  static const Unit * search_by_name(const string & symbol)
  {
    const UnitItem * ptr = tbl.search_by_name(symbol);
    const Unit * unit_ptr = static_cast<const Unit*>(ptr);
    return unit_ptr;
  }

  /** Search the unit associated to a symbol
      
      @param[in] symbol string containing the unit symbol
      @return constant pointer to the symbol. If the symbol is not
      found, then `nullptr` is returned
  */
  static const Unit * search_by_symbol(const string & symbol)
  {
    auto ptr = tbl.search_by_symbol(symbol);
    const Unit * unit_ptr = static_cast<const Unit*>(ptr);
    return unit_ptr;
  }

protected:

  Unit(const string & name, const string & symbol, const string & desc,
       const PhysicalQuantity & phy_q, double min, double max)
    : UnitItem(name, symbol, desc), physical_quantity(phy_q),
      min_val(min), max_val(max)
  {
    tbl.register_item(this);
    unit_tbl.insert(this);
  }
};

using Unit_Convert_Fct_Ptr = double (*)(double);

using UnitHashTbl = DynMapTree<pair<string, string>, Unit_Convert_Fct_Ptr>;

# include "multiunitmap.H"

extern UnitHashTbl __unit_name_name_tbl;
extern UnitHashTbl __unit_name_symbol_tbl;
extern UnitHashTbl __unit_symbol_name_tbl;
extern UnitHashTbl __unit_symbol_symbol_tbl;
extern CompoundUnitTbl __compound_unit_tbl;

template <typename...> struct __always_false : std::false_type {};

inline const Unit * search_compound_unit(const string & uname1,
					 const string & uname2)
{
  return __compound_unit_tbl.search({uname1, uname2});
}

/* Default compound unit meta function */
template <typename ...Units> struct Combine_Units
{
  Combine_Units()
  {
    static_assert(__always_false<Units...>::value,
		  "Compound unit not specified");
  }	
};

// this template performs the conversion. In absence of definition the
// compiler falls here and emits an error due to the static_assert
template <class SrcUnit, class TgtUnit> inline
double unit_convert(double /* val */)
{
  static_assert(__always_false<SrcUnit, TgtUnit>::value,
		"No specialization exists!");
  return 0;
}

template <class SrcUnit, class TgtUnit>
class UnitConverter
{
  static double convert(double val)
  {
    return unit_convert<SrcUnit, TgtUnit>(val);
  }

  Unit_Convert_Fct_Ptr fct_ptr = nullptr;

public:

  UnitConverter()
  {
    const Unit & src_instance = SrcUnit::get_instance();
    const Unit & tgt_instance = TgtUnit::get_instance();

    const string & src_name = src_instance.name;
    const string & tgt_name = tgt_instance.name;

    if (&src_instance.physical_quantity != &tgt_instance.physical_quantity)
      {
	ostringstream s;
	s << "Conversion from " << src_name << " to " << tgt_name
	  << " does not shae the same physical quantities ("
	  << src_instance.physical_quantity.name << ", "
	  << tgt_instance.physical_quantity.name << ")";
	throw domain_error(s.str());
      }
    
    const string & src_symbol = src_instance.symbol;
    const string & tgt_symbol = tgt_instance.symbol;

    if (__unit_name_name_tbl.has(make_pair(src_name, tgt_name)))
      {
	ostringstream s;
	s << "Conversion from unit name " << src_name << " to unit name "
	  << tgt_name << " has already been registered";
	throw domain_error(s.str());
      }

    if (__unit_name_symbol_tbl.has(make_pair(src_name, tgt_symbol)))
      {
	ostringstream s;
	s << "Conversion from unit name " << src_name << " to symbol name "
	  << tgt_symbol << " has already been registered";
	throw domain_error(s.str());
      }

    if (__unit_symbol_name_tbl.has(make_pair(src_symbol, tgt_name)))
      {
	ostringstream s;
	s << "Conversion from symbol name " << src_symbol << " to unit name "
	  << tgt_name << " has already been registered";
	throw domain_error(s.str());
      }

    if (__unit_symbol_symbol_tbl.has(make_pair(src_symbol, tgt_symbol)))
      {
	ostringstream s;
	s << "Conversion from symbol name " << src_symbol << " to symbol name "
	  << tgt_symbol << " has already been registered";
	throw domain_error(s.str());
      }

    fct_ptr = &UnitConverter::convert;

    __unit_name_name_tbl.insert(make_pair(src_name, tgt_name), fct_ptr);
    __unit_name_symbol_tbl.insert(make_pair(src_name, tgt_symbol), fct_ptr);
    __unit_symbol_name_tbl.insert(make_pair(src_symbol, tgt_name), fct_ptr);
    __unit_symbol_symbol_tbl.insert(make_pair(src_symbol, tgt_symbol), fct_ptr);

    assert(__unit_name_name_tbl.has(make_pair(src_name, tgt_name)));
    assert(__unit_name_symbol_tbl.has(make_pair(src_name, tgt_symbol)));
    assert(__unit_symbol_name_tbl.has(make_pair(src_symbol, tgt_name)));
    assert(__unit_symbol_symbol_tbl.has(make_pair(src_symbol, tgt_symbol)));
  }

  Unit_Convert_Fct_Ptr operator () () const noexcept { return fct_ptr; }
};

inline double unit_convert_name_to_name(const string & src_name,
					double val,
					const string & tgt_name)
{
  auto p = __unit_name_name_tbl.search(make_pair(src_name, tgt_name));
  if (p == nullptr)
    {
      ostringstream s;
      s << "Conversion from unit name " << src_name << " to unit name "
	<< tgt_name << " has not been registered";
      throw domain_error(s.str());
    }

  auto fct = p->second;

  return (*fct)(val);
}

inline double unit_convert_name_to_symbol(const string & src_name,
					  double val,
					  const string & tgt_symbol)
{
  auto p = __unit_name_symbol_tbl.search(make_pair(src_name, tgt_symbol));
  if (p == nullptr)
    {
      ostringstream s;
      s << "Conversion from unit name " << src_name << " to unit symbol "
	<< tgt_symbol << " has not been registered";
      throw domain_error(s.str());
    }

  auto fct = p->second;

  return (*fct)(val);
}

inline double unit_convert_symbol_to_name(const string & src_symbol,
					  double val,
					  const string & tgt_name)
{
  auto p = __unit_symbol_name_tbl.search(make_pair(src_symbol, tgt_name));
  if (p == nullptr)
    {
      ostringstream s;
      s << "Conversion from symbol name " << src_symbol << " to unit name "
	<< tgt_name << " has not been registered";
      throw domain_error(s.str());
    }

  auto fct = p->second;

  return (*fct)(val);
}

inline double unit_convert_symbol_to_symbol(const string & src_symbol,
					    double val,
					    const string & tgt_symbol)
{
  auto p = __unit_symbol_symbol_tbl.search(make_pair(src_symbol, tgt_symbol));
  if (p == nullptr)
    {
      ostringstream s;
      s << "Conversion from symbol name " << src_symbol << " to symbool name "
	<< tgt_symbol << " has not been registered";
      throw domain_error(s.str());
    }

  auto fct = p->second;

  return (*fct)(val);
}

/** Declare a new unit

    @param[in] __name of unit
    @param[in] symbol of unit
    @param[in] desc description
    @param[in] physical_quantity reference to a previusly defined
    `Physical_Quantity` object associated to the unit
    @param[in] min minimum value of the unit
    @param[in] max maximum value of the unit
*/
# define Declare_Unit(__name, symbol, desc, physical_quantity, min, max) \
  class __name : public Unit						\
  {									\
    __name() : Unit(#__name, symbol, desc,				\
		    physical_quantity::get_instance(), min, max) {}	\
									\
  public:								\
									\
    static const __name & get_instance()				\
    {									\
      static __name instance;						\
      return instance;							\
    }									\
									\
    __name(const __name&) = delete;					\
    void operator = (const __name&) = delete;				\
  };


# define Declare_Conversion(Unit1, Unit2, val)		\
  UnitConverter<Unit1, Unit2> __uc_##Unit1##_to_##Unit2;	\
  template <> double unit_convert<Unit1, Unit2>(double val)

/** Declare a compound unit; that is a unit composed by two units

    @param[in] __name of compound unit
    @param[in] symbol of unit
    @param[in] desc description
    @param[in] physical_quantity_name reference to the physical
    quantity associated to the new unit
    @param[in] min minimum value of the unit
    @param[in] max maximum value of the unit
    @param[in] Unit1 first unit from left to right 
    @param[in] Unit2 second unit from left to right 
*/
# define Declare_Compound_Unit(__name, symbol, desc, physical_quantity_name, \
			       min, max, Unit1, Unit2)			\
  Declare_Unit(__name, symbol, desc, physical_quantity_name, min, max);	\
  template <> struct Combine_Units<Unit1, Unit2>			\
  {									\
    using type = __name;						\
    Combine_Units()							\
      {									\
	__compound_unit_tbl.insert({Unit1::get_instance().name,		\
	      Unit2::get_instance().name}, __name::get_instance());	\
      }									\
    static const Combine_Units<Unit1, Unit2> __cu_trigger;		\
  };									\
  const Combine_Units<Unit1, Unit2> Combine_Units<Unit1, Unit2>::__cu_trigger;

/** Declare a compound unit; that is a unit composed by three units

    @param[in] name of compound unit
    @param[in] symbol of unit
    @param[in] desc description
    @param[in] physical_quantity_name reference to the physical
    quantity associated to the new unit
    @param[in] min minimum value of the unit
    @param[in] max maximum value of the unit
    @param[in] Unit1 first unit from left to right 
    @param[in] Unit2 second unit from left to right 
    @param[in] Unit3 second unit from left to right 
*/
# define Declare_Compound_Unit3(__name, symbol, desc, physical_quantity_name, \
				min, max, Unit1, Unit2, Unit3)		\
  Declare_Unit(__name, symbol, desc, physical_quantity_name, min, max);	\
  template <> struct Combine_Units<Unit1, Unit2, Unit3>			\
  {									\
    using type = __name;						\
    Combine_Units()							\
      {									\
	__compound_unit_tbl.insert({Unit1::get_instance().name,		\
	      Unit2::get_instance().name, Unit3::get_instance()},	\
	  __name::get_instance());					\
      }									\
  };

class BaseQuantity
{
public:

  const Unit & unit;
  
protected:

  double value;

  BaseQuantity(const Unit & __unit, double val = 0) : unit(__unit), value(val) {}
  
  // helper for validating that value is in [min_val, max_val]. It
  // throws range_error if value is not in the interval
  void check_value()
  {
    if (value >= unit.min_val and value <= unit.max_val)
      return;

    ostringstream s;
    s << "Value " << value << " is not inside in [" << unit.min_val << ","
      << unit.max_val << "]";
    throw range_error(s.str());
  }

  // throw exception if the units do not share the same physical quantity
  void check_physical_units(const BaseQuantity & q) const
  {
    if ((void*) &unit.physical_quantity == (void*) &(q.unit.physical_quantity))
      return;

    ostringstream s;
    s << "Units do not refer to the same physical quantities" << endl
      << "Source physical quantity = " << unit.physical_quantity.name << endl
      << "target physical quantity = " << q.unit.physical_quantity.name;
    throw domain_error(s.str());
  }

  void verify_same_unit(const Unit & __unit) const
  {
    if (&this->unit == &__unit)
      return;

    ostringstream s;
    s << "Different units: " << unit.name << " != " << __unit.name;
    throw domain_error(s.str());
  }

public:

  double get_value() const noexcept { return value; }

      /// Return the stringfied value (the unit symbol is concatenated)
  string to_string() const
  {
    ostringstream s;
    s << value << " " << unit.symbol;
    return s.str();
  }

  friend ostream & operator << (ostream & out, const BaseQuantity & q)
  {
    return out << q.to_string();
  }
};

class VtlQuantity; // forward declaration

/** Quantity 

 */
template <class UnitName>
class Quantity : public BaseQuantity
{
  // assign to value the value contained in q converted to UnitName
  // and validates that both units refer to the same physical quantity
  // and the converted value is inside the valid range
  template <class SrcUnit>
  void assign_converted(const Quantity<SrcUnit> & q)
  {
    check_physical_units(q);
    value = unit_convert<SrcUnit, UnitName>(q.get_value());
    check_value();
  }

public:

  Quantity(double val = 0) 
    : BaseQuantity(UnitName::get_instance(), val)
  {
    check_value(); // value must be inside the specified range
  }

  // direct copy constructor
  Quantity(const Quantity & q) noexcept : BaseQuantity(q.unit, q.get_value()) {}

  // direct copy assignment 
  Quantity & operator = (const Quantity & q) noexcept
  {
    if (&q == this)
      return *this;

    value = q.get_value();

    return *this;
  }

  /// Inter unit constructor. Perform the conversion
  template <class SrcUnit>
  Quantity(const Quantity<SrcUnit> & q)
    : BaseQuantity(UnitName::get_instance())
  {
    assign_converted(q);
  }

  /// Inter unit assignment. Perform the conversion
  template <class SrcUnit>
  Quantity & operator = (const Quantity<SrcUnit> & q)
  {
    if ((void*) &q == (void*) this)
      return *this;

    assign_converted(q);

    return *this;
  }

  inline Quantity(const VtlQuantity & q);

  inline Quantity & operator = (const VtlQuantity & q);

  Quantity & operator += (const Quantity & rhs) 
  {
    value += rhs.get_value();
    check_value();
    return *this;
  }

  Quantity operator + (const Quantity & rhs) const
  {
    Quantity ret(*this);
    ret += rhs;
    return ret;
  }

  Quantity & operator -= (const Quantity & rhs)
  {
    value -= rhs.get_value();
    check_value();
    return *this;
  }

  Quantity operator - (const Quantity & rhs) const
  {
    Quantity ret(*this);
    ret -= rhs;
    return ret;
  }

  template <class U>
  Quantity<typename Combine_Units<UnitName, U>::type>
  operator * (const Quantity<U> & rhs) const
  {
    using T = typename Combine_Units<UnitName, U>::type;
    return Quantity<T>(value * rhs.get_value());
  }

  template <class U>
  Quantity<typename Combine_Units<UnitName, U>::type>
  operator / (const Quantity<U> & rhs) const
  {
    using T = typename Combine_Units<UnitName, U>::type;
    return Quantity<T>(value / rhs.get_value());
  }

  template <class U>
  bool operator < (const Quantity<U> & rhs) const
  {
    Quantity r = { rhs }; // here the conversion is done
    return value < r.get_value();
  }

  template <class U>
  bool operator <= (const Quantity<U> & rhs) const
  {
    Quantity r = { rhs }; // here the conversion is done
    return value <= r.get_value();
  }

  template <class U>
  bool operator > (const Quantity<U> & rhs) const
  {
    Quantity r = { rhs }; // here the conversion is done
    return value > r.get_value();
  }

  template <class U>
  bool operator >= (const Quantity<U> & rhs) const
  {
    Quantity r = { rhs }; // here the conversion is done
    return value >= r.get_value();
  }

  template <class U>
  bool operator == (const Quantity<U> & rhs) const
  {
    Quantity r = { rhs }; // here the conversion is done
    return value == r.get_value();
  }

  template <class U>
  bool operator != (const Quantity<U> & rhs) const
  {
    return not (*this == rhs);
  }

  /// Return `this` converted to `Quantity<U>
  template <class U> Quantity convert() const
  {
    return Quantity<U>(*this);
  }

  inline Quantity & operator += (const VtlQuantity & rhs) const;
  inline Quantity & operator -= (const VtlQuantity & rhs) const;
  inline VtlQuantity operator + (const VtlQuantity & rhs) const;
  inline VtlQuantity operator - (const VtlQuantity & rhs) const;
  inline VtlQuantity  operator * (const VtlQuantity &) const;
  inline VtlQuantity  operator / (const VtlQuantity &) const;
};

template <class UnitName> inline
Quantity<UnitName> operator + (double lhs, const Quantity<UnitName> & rhs)
{
  return Quantity<UnitName>(lhs + rhs.get_value());
}

template <class UnitName> inline
Quantity<UnitName> operator - (double lhs, const Quantity<UnitName> & rhs)
{
  return Quantity<UnitName>(lhs - rhs.get_value());
}

template <class UnitName> inline
Quantity<UnitName> operator * (double lhs, const Quantity<UnitName> & rhs)
{
  return Quantity<UnitName>(lhs*rhs.get_value());
}

template <class UnitName> inline					
Quantity<UnitName> operator * (const Quantity<UnitName> & lhs, double rhs)
{
  return Quantity<UnitName>(lhs.get_value()*rhs);
}

template <class UnitName> inline
Quantity<UnitName> operator / (double lhs, const Quantity<UnitName> & rhs)
{
  return Quantity<UnitName>(lhs / rhs.get_value());
}

template <class U> inline
bool operator < (double lhs, const Quantity<U> & rhs)
{
  return Quantity<U>(lhs) < rhs;
}

template <class U> inline
bool operator < (const Quantity<U> & lhs, double rhs)
{
  return lhs < Quantity<U>(rhs);
}

template <class U> inline
bool operator <= (const Quantity<U> & lhs, double rhs)
{
  return lhs <= Quantity<U>(rhs);
}

template <class U> inline
bool operator <= (double lhs, const Quantity<U> & rhs)
{
  return Quantity<U>(lhs) <= rhs;
}

template <class U> inline
bool operator > (const Quantity<U> & lhs, double rhs)
{
  return lhs > Quantity<U>(rhs);
}

template <class U> inline
bool operator > (double lhs, const Quantity<U> & rhs)
{
  return Quantity<U>(lhs) > rhs;
}

template <class U> inline
bool operator >= (const Quantity<U> & lhs, double rhs)
{
  return lhs >= Quantity<U>(rhs);
}

template <class U> inline				
bool operator >= (double lhs, const Quantity<U> & rhs) 
{
  return Quantity<U>(lhs) >= rhs;
  }

template <class U> inline
bool operator == (const Quantity<U> & lhs, double rhs)
{
  return lhs == Quantity<U>(rhs);
}

template <class U> inline
bool operator == (double lhs, const Quantity<U> & rhs)
{
  return Quantity<U>(lhs) == rhs;
}

template <class U> inline
bool operator != (const Quantity<U> & lhs, double rhs)
{
  return not (lhs == rhs);
}

template <class U> inline
bool operator != (double lhs, const Quantity<U> & rhs)
{
  return not (lhs == rhs);
}

class VtlQuantity : public BaseQuantity
{
  // return the compund unit corresponding to uname1 x uname2
  const Unit & verify_compound(const Unit & unit1, const Unit & unit2) const
  {
    auto unit_ptr = search_compound_unit(unit1.name, unit2.name);
    if (unit_ptr != nullptr)
      return *unit_ptr;
    ostringstream s;
    s << "There is no compund unit between " << unit1.name << " and "
      << unit2.name;
    throw domain_error(s.str());
  }

public:

  VtlQuantity(const string & unit_name, double val = 0)
    : BaseQuantity(*Unit::search_by_name(unit_name), val)
      // TODO: validar si no se enuentra la unidad
  {
    check_value();
  }

  VtlQuantity(const Unit & unit, double val = 0)
    : BaseQuantity(unit, val)
  {
    check_value();
  }

  VtlQuantity(const VtlQuantity & q) noexcept : BaseQuantity(q.unit, q.value) {}

  VtlQuantity(const string & unit_name, const VtlQuantity & q)
    : BaseQuantity(*Unit::search_by_name(unit_name), q.get_value())
  {
    if (&unit == &q.unit)
      value = q.value;
    else
      value = unit_convert_name_to_name(q.unit.name, q.get_value(), unit.name);
    check_value();
  }

  VtlQuantity(const Unit & unit, const VtlQuantity & q)
    : BaseQuantity(unit)
  {
    if (&unit == &q.unit)
      value = q.value;
    else
      {
	value = unit_convert_name_to_name(q.unit.name, q.get_value(), unit.name);
	check_value();
      }
  }

  VtlQuantity & operator = (const VtlQuantity & q)
  {
    if (this == &q)
      return *this;

    if (&unit.physical_quantity == &q.unit.physical_quantity)
      {
	value = q.get_value();
	return *this;
      }

    value = unit_convert_name_to_name(q.unit.name, q.get_value(), unit.name);
    check_value();

    return *this;
  }

  template <class U>
  VtlQuantity(const Quantity<U> & q) : BaseQuantity(q.unit, q.get_value()) {}

  template <class U>
  VtlQuantity & operator = (const Quantity<Unit> & q)
  {
    if (&unit.physical_quantity == &q.unit.physical_quantity)
      {
	value = q.get_value();
	return *this;
      }

    value = unit_convert_name_to_name(q.unit.name, q.get_value(), unit.name);
    check_value();

    return *this;
  }

  VtlQuantity & operator += (double rhs)
  {
    value += rhs;
    check_value();
    return *this;
  }

  VtlQuantity & operator -= (double rhs)
  {
    value -= rhs;
    check_value();
    return *this;
  }

  VtlQuantity & operator += (const VtlQuantity & rhs)
  {
    value += rhs.get_value();
    check_value();
    return *this;
  }

  VtlQuantity operator + (const VtlQuantity & rhs) const
  {
    VtlQuantity ret(*this);
    ret += rhs;
    return ret;
  }

  VtlQuantity & operator -= (const VtlQuantity & rhs)
  {
    value -= rhs.get_value();
    check_value();
    return *this;
  }

  VtlQuantity operator - (const VtlQuantity & rhs) const
  {
    VtlQuantity ret(*this);
    ret -= rhs;
    return ret;
  }

  VtlQuantity operator * (const VtlQuantity & rhs) const
  {
    return VtlQuantity(verify_compound(unit, rhs.unit).name,
		       value*rhs.get_value());
  }

  VtlQuantity operator / (const VtlQuantity & rhs) const
  {
    return VtlQuantity(verify_compound(unit, rhs.unit).name,
		       value/rhs.get_value());
  }

  template <class U> VtlQuantity
  operator * (const Quantity<U> & rhs) const
  {
    return VtlQuantity(verify_compound(unit, rhs.unit).name,
		       value*rhs.get_value());
  }

  template <class U> VtlQuantity
  operator / (const Quantity<U> & rhs) const
  {
    return VtlQuantity(verify_compound(unit, rhs.unit).name,
		       value/rhs.get_value());
  }

private:

  VtlQuantity build_tmp(const VtlQuantity & rhs) const
  {
    check_physical_units(rhs);
    VtlQuantity q(unit, rhs); // here conversion is done
    q.check_value();
    return q;
  }

public:

  bool operator < (const VtlQuantity & rhs) const noexcept
  {
    if (&unit == &rhs.unit)
      return value < rhs.value;
    return value < build_tmp(rhs).get_value();
  }    

  bool operator <= (const VtlQuantity & rhs) const noexcept
  {
    if (&unit == &rhs.unit)
      return value <= rhs.value;
    return value <= build_tmp(rhs).get_value();
  }

  bool operator > (const VtlQuantity & rhs) const noexcept
  {
    if (&unit == &rhs.unit)
      return value > rhs.value;
    return value > build_tmp(rhs).get_value();
  }    

  bool operator >= (const VtlQuantity & rhs) const noexcept
  {
    if (&unit == &rhs.unit)
      return value >= rhs.value;
    return value >= build_tmp(rhs).get_value();
  }    

  bool operator == (const VtlQuantity & rhs) const noexcept
  {
    if (&unit == &rhs.unit)
      return value == rhs.value;
    return value == build_tmp(rhs).get_value();
  }    

  bool operator != (const VtlQuantity & rhs) const noexcept
  {
    return not (*this == rhs);
  }

  bool operator < (double rhs) const noexcept { return value < rhs; }
  bool operator <= (double rhs) const noexcept { return value <= rhs; }
  bool operator > (double rhs) const noexcept { return value > rhs; }
  bool operator >= (double rhs) const noexcept { return value >= rhs; }
  bool operator == (double rhs) const noexcept { return value == rhs; }
  bool operator != (double rhs) const noexcept { return value != rhs; }
};

inline bool operator < (double lhs, const VtlQuantity & rhs) noexcept
{
  return lhs < rhs.get_value();
}

inline bool operator > (double lhs, const VtlQuantity & rhs) noexcept
{
  return lhs > rhs.get_value();
}

inline bool operator <= (double lhs, const VtlQuantity & rhs) noexcept
{
  return lhs <= rhs.get_value();
}

inline bool operator >= (double lhs, const VtlQuantity & rhs) noexcept
{
  return lhs >= rhs.get_value();
}

inline bool operator == (double lhs, const VtlQuantity & rhs) noexcept
{
  return lhs == rhs.get_value();
}

inline bool operator != (double lhs, const VtlQuantity & rhs) noexcept
{
  return lhs != rhs.get_value();
}

template <class UnitName>
Quantity<UnitName>::Quantity(const VtlQuantity & q)
  : BaseQuantity(q.unit, q.get_value()) {}

template <class UnitName>
Quantity<UnitName> & Quantity<UnitName>::operator = (const VtlQuantity & q)
{
  if (&unit.physical_quantity == &q.unit.physical_quantity)
    {
      value = q.get_value();
      return *this;
    }
  
  value = unit_convert_name_to_name(q.unit.name, q.get_value(), unit.name);

  return *this;
}

template <class UnitName> Quantity<UnitName> &
Quantity<UnitName>::operator += (const VtlQuantity & rhs) const
{
  verify_same_unit(rhs.unit);
  value += rhs.get_value();
  check_value();
  return *this;
}

template <class UnitName> Quantity<UnitName> &
Quantity<UnitName>::operator -= (const VtlQuantity & rhs) const
{
  verify_same_unit(rhs.unit);
  value -= rhs.get_value();
  check_value();
  return *this;
}

template <class UnitName> VtlQuantity
Quantity<UnitName>::operator + (const VtlQuantity & rhs) const
{
  verify_same_unit(rhs.unit);
  VtlQuantity ret(*this);
  ret += rhs;
  return ret;
}

template <class UnitName> VtlQuantity
Quantity<UnitName>::operator - (const VtlQuantity & rhs) const
{
  verify_same_unit(rhs.unit);
  VtlQuantity ret(*this);
  ret -= rhs;
  return ret;
}

template <class UnitName> VtlQuantity
Quantity<UnitName>::operator * (const VtlQuantity & rhs) const
{
  return VtlQuantity(verify_compound(unit, rhs.unit).name, value*rhs.get_value());
}

template <class UnitName> VtlQuantity
Quantity<UnitName>::operator / (const VtlQuantity & rhs) const
{
  return VtlQuantity(verify_compound(unit, rhs.unit).name, value/rhs.get_value());
}


# endif // UNITS_H
