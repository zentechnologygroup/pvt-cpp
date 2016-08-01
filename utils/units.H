
# ifndef UNITS_H
# define UNITS_H

# include <memory>
# include <type_traits>
# include <string>
# include <sstream>
# include <limits>

# include <tpl_dynMapTree.H>

# include <unititem.H>

using namespace std;

/** # Units management

    This file implements a generic units converter which allows to
    write formulas in some units and transparently tries to manage
    automatic unit conversion if the units are changed.

    ## Physical Quantities

    Units are associated to a `PhysicalQuantity` class. A "physical
    quantity" defines a numeric magnitude associated to a
    phenomenon. In order to specify a physical quantity, you should
    use the macro `Declare_Physical_Quantity(name, symbol, desc)`. By
    invoking this macro you creates a new class directly derived from
    `PhysicalQuantity` with a name, a symbol and a description. For
    example:

        Declare_Physical_Quantity(Length, "L", "Distance between two points");

    In this case, a new class name `L` is created with associated
    Length, symbol `L` and a description.

    Each `PhysicalQuantity` derived class is a singleton. That means
    that at any time there is only an instance of the class. As such,
    you cannot to instantiate any `PhysicalQuantity` derived
    class. However, you can access to the instance through the static
    method:

        const Length & Length::get_instance()

    For the previous example, you could do:

        const Length & length = Length::get_instance();

    in order to get a reference to the singleton class `Length`. With
    this reference, you could directly access the class attributes
    `name`, `symbol` and `description` respectively. The class is
    instantiated the first time it is used.

    You could also access all the physical quantities through the
    static method:

        DynList<const PhysicalQuantity *> unit::quantities()

    which returns a list of constant pointers to all the physical
    quantities being used.

    ## Units

    Any physical quantity is expressed in units. An unit is a
    previusly agreed magnitude of a physical quantity which is used
    for indicating values. For example, we could establish that a
    "light year" is the distance traversed by the light during a
    year. So, big distances could be expressed as quantums of a light
    year. 

    An unit is defined through the macro:

        Declare_Unit(name, symbol, desc, physical_quantity_name, min, max)

    where `name` is the name of unit, `symbol` is the string used for
    symbolizing it, `desc` is a description of the unit,
    `physical_quantity_name` is the physical quantity to which the
    unit refers to and min and max are the minimum and maximum values
    respectively that can have a physical quantity expressed with the
    unit. For example, we could define a light year as follows:
    
        Declare_Unit(LightYear, "ly", 
	             "distance traversed by the light during a year",
		     Length, numeric_limits<double>::min(), 
		     numeric_limits<double>::max());

    In this case, a new class called `LightYear` is introduced to the
    class name system. As with the physical quantities, any unit class
    is a singleton that cannot be instantiated, but can be referenced
    with the static method:

        const LightYear & LightYear::get_instance()

    which return a constant reference to the singleton `Lightyear`.

    Any declared unit class has the following attributes:

    . `name`: the name of unit
    . `symbol`; the string used as symbol of the unit
    . `description`
    . `physical_quantity`: constant reference to the physical quantity
       to which the unit relates

    These attributes can be directly accessed.

    Also, you could know all the used units through the following
    static method:

        DynList<const PhysicalQuantity *> Unit::quantities()

    ### Compound units

    Many unities are composed two o more others unities. By example,
    the speed, which is the rate of change of position of an object,
    is represented by the length of an interval by unit of time. In
    this case, the unit is denoted as compound and it is declared
    through: 

        Declare_Compound_Unit(name, symbol, desc, physical_quantity_name, 
	                      min, max, list-of-units);

    The only difference between `Declare_Unit` and
    `Declare_Compound_Unit` lies on the fact that the last takes a
    list of two or more units. In the case of the speed, we could
    define it as follows: 

        Declare_Compound_Unit(Km_h, "km/h", "Standard measure of speed", 
	                      Speed, 0, numeric_limits<double>::max(), 
			      Kilometer, Hour);

    Here we are defining the speed unit `Km_h` (Kilometers by hour)
    representing the physical quantity `Speed` between the interval
    `[0, max]` (max is the maximum value represented with a `double`)
    and composed by the units `Kilometer` and `Hour`.

    ## Units conversion

    The heart of this conversion approach underlies on the following
    generic template function:

        template <class SrcUnit, class TgtUnit> double unit_convert(double val);

    The goal of this function is to covert a magnitude `val` expressed
    in units of type `SrcUnit` to a magnitude expressed in units of
    type `TgtUnit` which is the return value. It is the user's
    responsibility to write as many functions conversions as units are
    going to be used and invoked.

    As example, let us suppose that we have already defined the
    `Kilometer` unit. Then, the conversion between `LightYear` and
    `Kilometer` could be written thus:

        template <class LightYear, class Kilometer> 
	double unit_convert(double val)
	{
	  return 9.4604e15 * val;
	}

   So, when a quantity expressed in `LightYear`'s is passed to a
   function managing kilometers, the above conversion function is
   called. 

   Note that the opposite is not allowed while the function
   `unit_convert<Kilometer, LightYear>()` is not still written.

   ## Quantities

   The goal of this library is to manage several units and their
   conversions in a type safe way. In order to achieve that, the
   numerical computations should be done through objects of type
   `Quantity`. 

   `Quantity<Unit>` is a template class whose parameter type is the
   unit used for expressing the quantity. Consider for example the
   following code snippet:

       Quantity<LightYear> dist = 1000;

    defines a variable `dist` containing a `double` whose value is
    1000 lightyears. 

    In or
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
    tbl.register_item(name, symbol, desc);
  }

public:

  static DynList<const PhysicalQuantity *> quantities()
  {
    return tbl.items().map<const PhysicalQuantity*>([] (auto p)
      {
	return static_cast<const PhysicalQuantity*>(p);
      });
  }
};

# define Declare_Physical_Quantity(name, symbol, desc)	\
  struct name : public PhysicalQuantity			\
  {							\
							\
    static const name & get_instance()			\
    {							\
      static name instance(#name, symbol, desc);	\
      return instance;					\
    }							\
							\
  private:						\
							\
    using PhysicalQuantity::PhysicalQuantity;		\
							\
    name() : PhysicalQuantity(#name, symbol, desc) {}	\
							\
  public:						\
							\
    name(const name&) = delete;				\
    void operator = (const name&) = delete;		\
  };

struct Unit : public UnitItem
{
  template <typename U> friend class Quantity;

  const PhysicalQuantity & physical_quantity;
  const double min_val = 0;
  const double max_val = 0;

private:

  static UnitItemTable tbl;
  static DynSetTree<const Unit *> unit_tbl;

public:

  static DynList<const Unit*> units() { return unit_tbl.keys(); }

  static const Unit * search_by_symbol(const string & symbol)
  {
    auto ptr = tbl.search_by_symbol(symbol);
    const Unit * unit_ptr = static_cast<const Unit*>(ptr);
    return unit_ptr;
  }

  static const Unit * search_by_name(const string & symbol)
  {
    auto ptr = tbl.search_by_name(symbol);
    const Unit * unit_ptr = static_cast<const Unit*>(ptr);
    return unit_ptr;
  }

protected:

  Unit(const string & name, const string & symbol, const string & desc,
       const PhysicalQuantity & phy_q, double min, double max)
    : UnitItem(name, symbol, desc), physical_quantity(phy_q),
      min_val(min), max_val(max)
  {
    tbl.register_item(name, symbol, desc);
    unit_tbl.insert(this);
  }
};

# define Declare_Unit(name, symbol, desc, physical_quantity_name, min, max) \
  class name : public Unit						\
  {									\
    name()								\
      : Unit(#name, symbol, desc,					\
	     physical_quantity_name::get_instance(), min, max) {}	\
									\
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
  };

# define Declare_Compound_Unit(name, symbol, desc, physical_quantity_name, \
			       min, max, Unit1, Units...)		\
  Declare_Unit(name, symbol, desc, physical_quantity_name, min, max);	\
  template <> struct Combine_Units<Unit1, Units> { using type = name; };


template <typename...> struct __always_false : std::false_type {};

template <class SrcUnit, class TgtUnit> inline
double unit_convert(double /* val */)
{
  static_assert(__always_false<SrcUnit, TgtUnit>::value,
		"No specialization exists!");
}

using UnitConvertFct = double (*)(double);

template <typename U1, typename U2> 	  //typename ...Units>
struct Combine_Units
{
  using type = U1;

  Combine_Units()
  {
    static_assert(__always_false<U1, U2>::value, "ERROR");
  }
};

template <class UnitName>
class Quantity 
{
public:

  const UnitName & unit; 
  double value;

private:

  template <class SrcUnit>
  void check_physical_units(const Quantity<SrcUnit> & q)
  {
    if ((void*) &unit.physical_quantity == (void*) &(q.unit.physical_quantity))
      return;

    ostringstream s;
    s << "Units do not refer to the same physical quantities" << endl
      << "Source physical quantity = " << unit.physical_quantity.name << endl
      << "target physical quantity = " << SrcUnit::get_instance().name;
    throw domain_error(s.str());
  }

  // helper for validating that value is in [min_val, max_val]
  void check_value()
  {
    if (value >= unit.min_val and value <= unit.max_val)
      return;

    ostringstream s;
    s << "Value " << value << " is not inside in [" << unit.min_val << ","
      << unit.max_val << "]";
    throw domain_error(s.str());
  }

  template <class SrcUnit>
  void assign_converted(const Quantity<SrcUnit> & q)
  {
    check_physical_units(q);
    value = unit_convert<SrcUnit, UnitName>(q.value);
    check_value();
  }

public:

  Quantity(double val = 0)
    : unit(UnitName::get_instance()), value(val)
  {
    check_value();
  }

  Quantity(const Quantity & q) : unit(q.unit), value(q.value) {}

  Quantity & operator = (const Quantity & q)
  {
    if (&q == this)
      return *this;

    value = q.value;

    return *this;
  }

  double get_value() const noexcept { return value; }

  string to_string() const
  {
    ostringstream s;
    s << value << " " << unit.symbol;
    return s.str();
  }

  friend ostream & operator << (ostream & out, const Quantity & q)
  {
    return out << q.to_string();
  }

  template <class SrcUnit>
  Quantity(const Quantity<SrcUnit> & q)
    : unit(UnitName::get_instance())
  {
    assign_converted(q);
  }

  template <class SrcUnit>
  Quantity & operator = (const Quantity<SrcUnit> & q)
  {
    if ((void*) &q == (void*) this)
      return *this;

    assign_converted(q);

    return *this;
  }

  Quantity & operator += (const Quantity & rhs)
  {
    value += rhs.value;
    check_value();
    return *this;
  }

  Quantity operator + (const Quantity & rhs)
  {
    Quantity ret(*this);
    ret += rhs;
    return ret;
  }

  Quantity & operator -= (const Quantity & rhs)
  {
    value -= rhs.value;
    check_value();
    return *this;
  }

  Quantity operator - (const Quantity & rhs)
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
    return Quantity<T>(value * rhs.value);
  }

  template <class U>
  Quantity<typename Combine_Units<UnitName, U>::type>
  operator / (const Quantity<U> & rhs) const
  {
    using T = typename Combine_Units<UnitName, U>::type;
    return Quantity<T>(value / rhs.value);
  }

  template <class U>
  bool operator < (const Quantity<U> & rhs) const
  {
    Quantity r = { rhs }; // here the conversion is done
    return value < r.value;
  }

  template <class U>
  bool operator <= (const Quantity<U> & rhs) const
  {
    Quantity r = { rhs }; // here the conversion is done
    return value <= r.value;
  }

  template <class U>
  bool operator > (const Quantity<U> & rhs) const
  {
    Quantity r = { rhs }; // here the conversion is done
    return value > r.value;
  }

  template <class U>
  bool operator >= (const Quantity<U> & rhs) const
  {
    Quantity r = { rhs }; // here the conversion is done
    return value >= r.value;
  }

  template <class U>
  bool operator == (const Quantity<U> & rhs) const
  {
    Quantity r = { rhs }; // here the conversion is done
    return value == r.value;
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
};

# define Hybrid_Add(type)						\
  template <class UnitName> inline					\
  Quantity<UnitName> operator + (type lhs, const Quantity<UnitName> & rhs) \
  {									\
    return Quantity<UnitName>(lhs + rhs.get_value());			\
  }

# define Hybrid_Sub(type)						\
  template <class UnitName> inline					\
  Quantity<UnitName> operator - (type lhs, const Quantity<UnitName> & rhs) \
  {									\
    return Quantity<UnitName>(lhs - rhs.get_value());			\
  }

# define Hybrid_Mult(type)						\
  template <class UnitName> inline					\
  Quantity<UnitName> operator * (type lhs, const Quantity<UnitName> & rhs) \
  {									\
    return Quantity<UnitName>(lhs*rhs.get_value());			\
  }

# define Hybrid_Div(type)						\
  template <class UnitName> inline					\
  Quantity<UnitName> operator / (type lhs, const Quantity<UnitName> & rhs) \
  {									\
    return Quantity<UnitName>(lhs / rhs.get_value());			\
  }

# define Hybrid_Cmp(type)				\
  template <class U> inline				\
  bool operator < (type lhs, const Quantity<U> & rhs)	\
  {							\
    return Quantity<U>(lhs) < rhs;			\
  }							\
							\
  template <class U> inline				\
  bool operator < (const Quantity<U> & lhs, type rhs)	\
  {							\
    return lhs < Quantity<U>(rhs);			\
  }							\
							\
  template <class U> inline				\
  bool operator <= (const Quantity<U> & lhs, type rhs)	\
  {							\
    return lhs <= Quantity<U>(rhs);			\
  }							\
							\
  template <class U> inline				\
  bool operator <= (type lhs, const Quantity<U> & rhs)	\
  {							\
    return Quantity<U>(lhs) <= rhs;			\
  }							\
							\
  template <class U> inline				\
  bool operator > (const Quantity<U> & lhs, type rhs)	\
  {							\
    return lhs > Quantity<U>(rhs);			\
  }							\
							\
  template <class U> inline				\
  bool operator > (type lhs, const Quantity<U> & rhs)	\
  {							\
    return Quantity<U>(lhs) > rhs;			\
  }							\
							\
  template <class U> inline				\
  bool operator >= (const Quantity<U> & lhs, type rhs)	\
  {							\
    return lhs >= Quantity<U>(rhs);			\
  }							\
							\
  template <class U> inline				\
  bool operator >= (type lhs, const Quantity<U> & rhs)	\
  {							\
    return Quantity<U>(lhs) >= rhs;			\
  }							\
							\
  template <class U> inline				\
  bool operator == (const Quantity<U> & lhs, type rhs)	\
  {							\
    return lhs == Quantity<U>(rhs);			\
  }							\
							\
  template <class U> inline				\
  bool operator == (type lhs, const Quantity<U> & rhs)	\
  {							\
    return Quantity<U>(lhs) == rhs;			\
  }							\
							\
  template <class U> inline				\
  bool operator != (const Quantity<U> & lhs, type rhs)	\
  {							\
    return not (lhs == rhs);				\
  }							\
							\
  template <class U> inline				\
  bool operator != (type lhs, const Quantity<U> & rhs)	\
  {							\
    return not (lhs == rhs);				\
  }

# define Hybrid_Ops(type)			\
  Hybrid_Add(type);				\
  Hybrid_Sub(type);				\
  Hybrid_Mult(type);				\
  Hybrid_Div(type);				\
  Hybrid_Cmp(type);

Hybrid_Ops(int);
Hybrid_Ops(long);
Hybrid_Ops(float);
Hybrid_Ops(double);


# endif // UNITS_H
