
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


template <class SrcUnit, class TgtUnit> extern
double convert(const double & /* val */,
	       const SrcUnit & /* src_unit */,
	       const TgtUnit & /* tgt_unit */);

template <class UnitName>
class Quantity 
{
public:

  const UnitName & unit; 
  double value;

private:

  template <class SrcUnit>
  void check_physical_units()
  {
    if (&unit.physical_quantity == &SrcUnit::get_instance().physical_quantity)
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

  Quantity(double val = 0) : unit(UnitName::get_instance()), value(val)
  {
    check_value();
  }

  double get_value() const noexcept { return value; }

  operator double() const noexcept { return value; }

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
    check_physical_units<SrcUnit>();
    value = convert(q.value, SrcUnit::get_instance(), unit);
    check_value();
  }

  template <class SrcUnit>
  Quantity & operator = (const Quantity<SrcUnit> & q)
  {
    if (&q == this)
      return *this;
    
    check_physical_units<SrcUnit>();
    value = convert(q.value, SrcUnit::get_instance(), unit);
    check_value();

    return *this;
  }

  template <class TgtUnit> operator TgtUnit() const
  {
    TgtUnit ret;
    ret = convert<UnitName, TgtUnit>(value, unit, TgtUnit::get_instance());
    ret.check_value();
    return ret;
  }
};

# endif // UNITS_H
