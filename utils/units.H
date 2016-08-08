
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

/** 

# Units management

This file implements a generic units converter which allows to write
formulas in some units and transparently tries to manage automatic
unit conversion if the units are changed.

## Physical Quantities

Units are associated to a `PhysicalQuantity` class. A "physical
quantity" defines a numeric magnitude associated to a phenomenon. In
order to specify a physical quantity, you should use the macro
`Declare_Physical_Quantity(name, symbol, desc)`. By invoking this
macro you creates a new class directly derived from `PhysicalQuantity`
with a name, a symbol and a description. For example:

    Declare_Physical_Quantity(Length, "L", "Distance between two points");

In this case, a new class name `L` is created with associated Length,
symbol `L` and a description.

Each `PhysicalQuantity` derived class is a singleton. That means that
at any time there is only an instance of the class. As such, you
cannot to instantiate any `PhysicalQuantity` derived class. However,
you can access to the instance through the static method:

    const Length & Length::get_instance()

For the previous example, you could do:

    const Length & length = Length::get_instance();

in order to get a reference to the singleton class `Length`. With this
reference, you could directly access the class attributes `name`,
`symbol` and `description` respectively. The class is instantiated the
first time it is used.

You could also access all the physical quantities through the
static method:

    DynList<const PhysicalQuantity *> unit::quantities()

which returns a list of constant pointers to all the physical
quantities being used.

## Units

Any physical quantity is expressed in units. An unit is a previusly
agreed magnitude of a physical quantity which is used for indicating
values. For example, we could establish that a "light year" is the
distance traversed by the light during a year. So, big distances could
be expressed as quantums of a light year.

An unit is defined through the macro:

    Declare_Unit(name, symbol, desc, physical_quantity_name, min, max)

where `name` is the name of unit, `symbol` is the string used for
symbolizing it, `desc` is a description of the unit,
`physical_quantity_name` is the physical quantity to which the unit
refers to and min and max are the minimum and maximum values
respectively that can have a physical quantity expressed with the
unit. For example, we could define a light year as follows:
    
    Declare_Unit(LightYear, "ly", 
                 "distance traversed by the light during a year",
		 Length, numeric_limits<double>::min(), 
		 numeric_limits<double>::max());

In this case, a new class called `LightYear` is introduced to the
class name system. As with the physical quantities, any unit class is
a singleton that cannot be instantiated, but can be referenced with
the static method:

    const LightYear & LightYear::get_instance()

which returns a constant reference to the singleton `Lightyear`.

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

Many unities are composed two o more others unities. By example, the
speed, which is the rate of change of position of an object, is
represented by the length of an interval by unit of time. In this
case, the unit is denoted as compound and it is declared through:

    Declare_Compound_Unit(name, symbol, desc, physical_quantity_name, 
                          min, max, list-of-units);

The only difference between `Declare_Unit` and `Declare_Compound_Unit`
lies on the fact that the last takes a list of two or more units. In
the case of the speed, we could define it as follows:

    Declare_Compound_Unit(Km_h, "km/h", "Standard measure of speed", 
                          Speed, 0, numeric_limits<double>::max(), 
			  Kilometer, Hour);

Here we are defining the speed unit `Km_h` (Kilometers by hour)
representing the physical quantity `Speed` between the interval `[0,
max]` (max is the maximum value represented with a `double`) and
composed by the units `Kilometer` and `Hour`.

## Validations

The library performs some validations, concretely those related to the
names and symbols. In that sense, neither the physical quantities nor
the units cannot have the same names or symbols. 

The exception `domain_error` is thrown if a duplicated name or symbol
is detected. These checking are lazily done in running time, at the
moment of the first use of a physical quantity or a name

## Units conversion

The heart of this conversion approach underlies on the following
generic template function:

    template <class SrcUnit, class TgtUnit> double unit_convert(double val);

The goal of this function is to covert a magnitude `val` expressed in
units of type `SrcUnit` to a magnitude expressed in units of type
`TgtUnit` which is the return value. It is the user's responsibility
to write as many functions conversions as units are going to be used
and invoked.

As example, let us suppose that we have already defined the
`Kilometer` unit. Then, the conversion between `LightYear` and
`Kilometer` could be written thus:

    template <class LightYear, class Kilometer> 
    double unit_convert(double val)
    {
      return 9.4604e15 * val;
    }

So, when a quantity expressed in `LightYear`'s is passed to a function
managing kilometers, the above conversion function is called.

Note that the opposite is not allowed while the function
`unit_convert<Kilometer, LightYear>()` is not still written.

## Quantities

The goal of this library is to manage several units and their
conversions in a type safe way. In order to achieve that, the
numerical computations should be done through objects of type
`Quantity`.

`Quantity<Unit>` is a template class whose parameter type is the unit
used for expressing the quantity. Consider for example the following
code snippet:

    Quantity<LightYear> dist = 1000;

defines a variable `dist` containing a `double` whose value is 1000
lightyears.

### Conversions

Conversion are automatically done through the constructors,
assignments, arithmetical operations and comparisons.

A conversion between two unities is valid if and only if they relate
the same physical quantity. By example, meters could be converted to
kilometers because they relate the physical quantity `Length`. At the
contrary, an conversion between meters and seconds will be not valid.

For this version, the library performs this checking at running time,
while a conversion is being performed. A `domain_error` exception will
be thrown if a conversion between physically unrelated unities is
done.

Casting between `Quantity` objects is not allowed. Perform valid
conversion through the constructor.

#### Constructor and assignment

Given a quantity in a unit `U1`, you can get its equivalent in another
unit `U2` by simply instantiating the `Quantity<U2>` copy
constructor. For example:

    Quantity<Meter> dist_mt = 1000;
    Quantity<Kilometer> dist_km = dist_mt; // valid and converted to 1 km

You can also convert via the assignment. For example:

    dist_mt = 10000; // here we are assigning a scalar
    dist_km = dist_mt; // it is converted to 10 km

As suggested in the previous example, you can perfectly construct and
assign `Quantity` objects directly passing them a scalar. Of course,
in this case no conversion is done.

#### Arithmetical operations

The library exports the fundamental arithmetical operations: addition,
subtraction, multiplication and division.

Any arithmetical operation can be done between a `Quantity` object and
a scalar. the scalar can be a `int`, `long`, `float` or `double`
constant or a variable. In this case, no conversion is performed.

Addition and subtraction between quantities expressed in different
units can be done as long as the involved units relate to the same
physical quantity. For example:

    Quantity<Mile> dist_mi = 100;
    dist_mi += dist_km + dist_mt; // + performs conversion from mt to km

In this case, the binary arithmetical operator `+` converts the
second operand to kilometers and then perform the addition. The result
is in kilometers. Afterward, for `+=` operator the kilometers are
converted to miles and added to `dist_mi`.

For `+` and `-` binary operators, the second operand is converted to
the unit of first one. A `domain_error` exception would be thrown if
the units do not relate to the same physical quantity. Addition and
subtraction between quantities relating to different physical
magnitudes are not allowed.

For the `*` and `/` binary operators, no conversion is done because
these operations physically would mean another physical quantity with
units that are function of the operands. For example, an unit of speed
could be `km/h`.

In order to the result of `*` and `/` to be valid, there must exist a
unit linking the units of the two operands. Basically, this is the
goal of a compound unit. So, when a `*` or `/` are invoked, the
library searches the compound unit linking the unities of their
operands. The code should not compile if there is no exist such
compound unit. For example, you could compute the speed as follows:

    Quantity<Hour> time_h = 1;
    auto speed = dist_km/time_h; // valid because Km_h unit was defined

In this case, the division is valid because the speed unit `Km_h` was
defined as a compound unit involving the `Kilometer` and `Hour` units.

If a binary `*` or `/`  is written and the needed compound unit does
not exist, then the construct will not compile

The `*=` and `/=` operators are not defined.

#### Arithmetical expression and compound units

You could extend the syntactic validity of `*` and `/` operator by
specifying compound units. 

For example, this operation:

    auto dist_mt_square = dist_mt*dist_mt;

will not compile because `*` on both operands `Quantity<Meter>` is not
defined. You could however become valid the above expression by doing:

    Declare_Compound_Unit(Mt2, "mt2", "", Distance, 0,
                          numeric_limits<double>::max(), Meter, Meter);

In the same way, you could define

    Declare_Compound_Unit(Mt3, "mt3", "", Distance, 0,
                          numeric_limits<double>::max(), Mt2, Meter);

which would allow

    auto s = dist_meter*dist_meter*dist_meter;

According to the parsing rules for arithmetical expressions, the
compiler processes form left to right. So, the first
`dist_meter*dist_meter` is done which produces a `mt2` unit. Then this
result is multiplied as `(dist_meter*dist_meter)*dist_meter` which
matches with the `mt3` declared unit. Note however that the expression

    auto s = dist_meter*(dist_meter*dist_meter);

will not compile because a compound unit involving meter and square
meter does not exist (although it exists square meter with meter). In
many cases is not worth declaring a compound unit only for satisfying
the previous expression, because you could solve the matter by writing
the expression in an appropriated way. In addition, it is ambiguous and
potentially dangerous to have several units expressing the same thing,

#### Accessing the scalar value

In some cases, especially those involving complicated arithmetical
transformations, it is very probable that you get compiler syntactic
errors without chance of defining a compound unit. A way for
by-passing this is to get the scalar value of the quantity and then
perform all the operations with scalar values. In order to do that,
you could use the `get_value()` method.

As recommendation, encapsulate these scalar value on a function. For
example:

    Quantity<Mt3> volume(const Quantity<Meter> & length,
                         const Quantity<Meter> & width,
			 const Quantity<Meter> & height)
    {
      double l = length.get_value();
      double w = width.get_value();
      double h = height.get_value();
      return Quantity<Mt3>(l*w*h);
   }

In this way, other length units could be used and their conversions
transparently done when the `volume()` function is called.
*/

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
    tbl.register_item(name, symbol, desc);
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

/** Unit base class

 */
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

  /// Return a list of all declared and used units
  static DynList<const Unit*> units() { return unit_tbl.keys(); }

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

  /** Search the unit associated to a name
      
      @param[in] name string containing the unit name
      @return constant pointer to the symbol. If the name is not
      found, then `nullptr` is returned
  */
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

/** Declare a new unit

    @param[in] name of unit
    @param[in] symbol of unit
    @param[in] desc description
    @param[in] physical_quantity_name reference to a previusly defined
    `Physical_Quantity` object associated to the unit
    @param[in] min minimum value of the unit
    @param[in] max maximum value of the unit
*/
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

template <typename...> struct __always_false : std::false_type {};

/* Default compound unit meta function */
template <typename ...Units> struct Combine_Units
{
  Combine_Units()
  {
    static_assert(__always_false<Units...>::value,
		  "Compound unit not specified");
  }	
};

/** Declare a compound unit; that is a unit composed by several units

    @param[in] name of compound unit
    @param[in] symbol of unit
    @param[in] desc description
    @param[in] physical_quantity_name reference to the physical
    quantity associated to the new unit
    @param[in] min minimum value of the unit
    @param[in] max maximum value of the unit
    @param[in] Units list of units compounding the new unit
*/
# define Declare_Compound_Unit(name, symbol, desc, physical_quantity_name, \
			       min, max, Units...)			\
  Declare_Unit(name, symbol, desc, physical_quantity_name, min, max);	\
  template <> struct Combine_Units<Units>				\
  {									\
    using type = name;							\
  };

// this template performs the conversion. In absence of definition the
// compiler falls here and emits an error due to the static_assert
template <class SrcUnit, class TgtUnit> inline
double unit_convert(double /* val */)
{
  static_assert(__always_false<SrcUnit, TgtUnit>::value,
		"No specialization exists!");
}

using unit_convert_fct_ptr = double (*)(double);

using UnitHashTbl =
  DynMapHash<pair<const char*, const char*>, unit_convert_fct_ptr>;

extern UnitHashTbl __unit_name_name_tbl;
extern UnitHashTbl __unit_name_symbol_tbl;
extern UnitHashTbl __unit_symbol_name_tbl;
extern UnitHashTbl __unit_symbol_symbol_tbl;

template <class SrcUnit, class TgtUnit>
class Unit_Converter
{
  static double convert(double val)
  {
    return unit_convert<SrcUnit, TgtUnit>(val);
  }

public:

  Unit_Converter()
  {
    const Unit & src_instance = SrcUnit::get_instance();
    const Unit & tgt_instance = TgtUnit::get_instance();

    const char * src_name = src_instance.name.c_str();
    const char * tgt_name = tgt_instance.name.c_str();

    if (&src_instance.physical_quantity != &tgt_instance.physical_quantity)
      {
	ostringstream s;
	s << "Conversion from " << src_name << " to " << tgt_name
	  << " does not shae the same physical quantities ("
	  << src_instance.physical_quantity.name << ", "
	  << tgt_instance.physical_quantity.name << ")";
	throw domain_error(s.str());
      }
    
    const char * src_symbol = src_instance.symbol.c_str();
    const char * tgt_symbol = tgt_instance.symbol.c_str();

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

    auto fct_ptr = &Unit_Converter::convert;

    __unit_name_name_tbl.insert(make_pair(src_name, tgt_name), fct_ptr);
    __unit_name_symbol_tbl.insert(make_pair(src_name, tgt_symbol), fct_ptr);
    __unit_symbol_name_tbl.insert(make_pair(src_symbol, tgt_name), fct_ptr);
    __unit_symbol_symbol_tbl.insert(make_pair(src_symbol, tgt_symbol), fct_ptr);
  }
};

inline double unit_convert_name_to_name(const string & src_name,
					double val,
					const string & tgt_name)
{
  auto p = __unit_name_name_tbl.search(make_pair(src_name.c_str(),
						 tgt_name.c_str()));
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
  auto p = __unit_name_symbol_tbl.search(make_pair(src_name.c_str(),
						   tgt_symbol.c_str()));
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
  auto p = __unit_symbol_name_tbl.search(make_pair(src_symbol.c_str(),
						   tgt_name.c_str()));
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
  auto p = __unit_symbol_symbol_tbl.search(make_pair(src_symbol.c_str(),
						     tgt_symbol.c_str()));
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

class BaseQuantity
{
public:

  const Unit & unit;
  
protected:

  double value;

  BaseQuantity(const Unit & __unit, double val = 0)
    : unit(__unit), value(val) {}
  
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

  Quantity operator + (const Quantity & rhs)
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
public:

  VtlQuantity(const string & unit_name, double val = 0)
    : BaseQuantity(*Unit::search_by_name(unit_name), val) {}

  VtlQuantity(const VtlQuantity & q) noexcept : BaseQuantity(q.unit, q.value) {}

  VtlQuantity(const string & unit_name, const VtlQuantity & q)
    : BaseQuantity(*Unit::search_by_name(unit_name), q.get_value())
  {
    if (&unit == &q.unit)
      value = q.value;
    else
      value = unit_convert_name_to_name(q.unit.name, q.get_value(), unit.name);
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

    return *this;
  }

  VtlQuantity & operator += (const VtlQuantity & rhs)
  {
    value += rhs.get_value();
    check_value();
    return *this;
  }

  VtlQuantity operator + (const VtlQuantity & rhs)
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

  VtlQuantity operator - (const VtlQuantity & rhs)
  {
    VtlQuantity ret(*this);
    ret -= rhs;
    return ret;
  }

  // template <class U>
  // VtlQuantity operator * (const Quantity<U> & rhs) const
  // {
  //   using T = typename Combine_Units<UnitName, U>::type;
  //   return Quantity<T>(value * rhs.get_value());
  // }

};


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


# endif // UNITS_H
