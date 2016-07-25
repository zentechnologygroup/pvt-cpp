
# ifndef UNITS_H
# define UNITS_H

# include <memory>
# include <string>
# include <sstream>
# include <limits>

# include <tpl_dynMapTree.H>

# include <unititem.H>

using namespace std; 

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

  static DynList<PhysicalQuantity> quantities()
  {
    return tbl.items().map<PhysicalQuantity>([] (auto p)
      {
	return *static_cast<const PhysicalQuantity*>(p);
      });
  }
};

# define Declare_Physical_Quantity(name, symbol, desc)		      \
  struct name : public PhysicalQuantity				      \
  {								      \
								      \
    using PhysicalQuantity::PhysicalQuantity;			      \
								      \
    static const name & get_instance()				      \
    {								      \
      static name instance(#name, symbol, desc);		      \
      return instance;						      \
    }								      \
								      \
  private:							      \
								      \
    name() : PhysicalQuantity(#name, symbol, desc) {}		      \
								      \
  public:							      \
								      \
    name(const name&) = delete;					      \
    void operator = (const name&) = delete;			      \
  };

extern UnitItemTable unit_tbl;

inline DynList<UnitItem> units()
{
  return unit_tbl.items().map<UnitItem>([] (auto p) { return *p; });
}

# define Declare_Unit(name, symbol, desc, physical_quantity_name, min, max) \
  struct name : public UnitItem						\
  {									\
    template <typename U> friend class Quantity;			\
    									\
    const PhysicalQuantity & physical_quantity;				\
    const double min_val = min;						\
    const double max_val = max;						\
									\
  private:								\
									\
    name()								\
      : UnitItem(#name, symbol, desc),					\
      physical_quantity(physical_quantity_name::get_instance())		\
      {									\
	unit_tbl.register_item(#name, symbol, desc);			\
      }									\
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
			       min, max, Unit1, Unit2)			\
  Declare_Unit(name, symbol, desc, physical_quantity_name, min, max);	\
  template <> struct Combine_Units<Unit1, Unit2> { using type = name; };

template <class SrcUnit, class TgtUnit> extern
double convert(const double & /* val */);

template <typename U1, typename U2>
struct Combine_Units
{
  using type = U1;
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

  void check_value()
  {
    if (value >= unit.min_val and value <= unit.max_val)
      return;

    ostringstream s;
    s << "Value " << value << " is not inside in [" << unit.min_val << ","
      << unit.max_val;
    throw domain_error(s.str());
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
    check_physical_units(q);
    value = convert<SrcUnit, UnitName>(q.value);
    check_value();
  }

  template <class SrcUnit>
  Quantity & operator = (const Quantity<SrcUnit> & q)
  {
    if ((void*) &q == (void*) this)
      return *this;
    
    check_physical_units(q);
    value = convert<SrcUnit, UnitName>(q.value);
    check_value();

    return *this;
  }

  template <class TgtUnit> operator TgtUnit() const
  {
    TgtUnit ret;
    ret.value = convert<UnitName, TgtUnit>(value);
    ret.check_value();
    return ret;
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
};

# define Hybrid_Add(type)						\
  template <class UnitName> inline					\
  Quantity<UnitName> operator + (type lhs, const Quantity<UnitName> & rhs) \
  {									\
    return Quantity<UnitName>(lhs + rhs.get_value());			\
  }									\
									\
  template <class UnitName> inline					\
  Quantity<UnitName> operator + (const Quantity<UnitName> & lhs, type rhs) \
  {									\
    return rhs+lhs;							\
  }

# define Hybrid_Sub(type)						\
  template <class UnitName> inline					\
  Quantity<UnitName> operator - (type lhs, const Quantity<UnitName> & rhs) \
  {									\
    return Quantity<UnitName>(lhs - rhs.get_value());			\
  }									\
									\
  template <class UnitName> inline					\
  Quantity<UnitName> operator - (const Quantity<UnitName> & lhs, type rhs) \
  {									\
    return Quantity<UnitName>(lhs.get_value() - rhs);			\
  }

# define Hybrid_Mult(type)						\
  template <class UnitName> inline					\
  Quantity<UnitName> operator * (type lhs, const Quantity<UnitName> & rhs) \
  {									\
    return Quantity<UnitName>(lhs*rhs.get_value());			\
  }									\
									\
  template <class UnitName> inline					\
  Quantity<UnitName> operator * (const Quantity<UnitName> & lhs, type rhs) \
  {									\
    return rhs*lhs;							\
  }

# define Hybrid_Div(type)						\
  template <class UnitName> inline					\
  Quantity<UnitName> operator / (type lhs, const Quantity<UnitName> & rhs) \
  {									\
    return Quantity<UnitName>(lhs / rhs.get_value());			\
  }									\
									\
  template <class UnitName> inline					\
  Quantity<UnitName> operator / (const Quantity<UnitName> & lhs, type rhs) \
  {									\
    return Quantity<UnitName>(lhs.get_value() / rhs);			\
  }

# define Hybrid_Ops(type)			\
  Hybrid_Add(type);				\
  Hybrid_Sub(type);				\
  Hybrid_Mult(type);				\
  Hybrid_Div(type);

Hybrid_Ops(int);
Hybrid_Ops(long);
Hybrid_Ops(float);
Hybrid_Ops(double);


# endif // UNITS_H
