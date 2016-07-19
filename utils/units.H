
# ifndef UNITS_H
# define UNITS_H

# include <string>
# include <sstream>

# include <tpl_dynMapTree.H>

using namespace std; 

class PhysicalQuantity 
{
  string name = "Undefined";
  string symbol = "Undefined";
  string description = "Undefined";

public:

  PhysicalQuantity(const string & __name, const string & __symbol,
		     const string & desc) noexcept
    : name(__name), symbol(__symbol), description(desc) {}

  const string & get_name() const noexcept { return name; }

  const string & get_symbol() const noexcept { return symbol; }

  const string & get_description() const noexcept { return description; }
};

class Unit 
{
  string name = "Undefined";
  string symbol = "Undefined";
  string description = "Undefined";

  PhysicalQuantity physical_quantity;

  double min_val = 0;
  double max_val = 0;
  
public:

  Unit(const string & __name, const string & __symbol,
       const string & desc, const PhysicalQuantity & phy_quan) noexcept
    : name(__name), symbol(__symbol), description(desc),
      physical_quantity(phy_quan) {}

  const string & get_name() const noexcept { return name; }

  const string & get_symbol() const noexcept { return symbol; }

  const string & get_description() const noexcept { return description; }

  const PhysicalQuantity & get_physical_quantity() const noexcept
  {
    return physical_quantity;
  }

  bool value_is_valid(const double value) const noexcept
  {
    return value >= min_val and value <= max_val;
  }
};

class UnitTable 
{
  DynMapTree<string, Unit*> name_tbl;   // index by unit name
  DynMapTree<string, Unit*> symbol_tbl; // index by symbol name

  static short instance_counter;

public:

  UnitTable()
  {
    if (++instance_counter > 1)
      throw domain_error("UnitTable instanced twice");
  }

  ~UnitTable()
  {
    name_tbl.for_each([] (auto p) { delete p.second; });
  }

  void register_unit(const Unit & unit)
  {
    if (not name_tbl.contains(unit.get_name()))
      {
	ostringstream s;
	s << "unit name " << unit.get_name() << " already exist";
	throw domain_error(s.str());
      }

    if (not symbol_tbl.contains(unit.get_symbol()))
      {
	ostringstream s;
	s << "unit symbol " << unit.get_symbol() << " already exist";
	throw domain_error(s.str());
      }
    
    Unit * ptr = new Unit(unit);
    name_tbl.insert(unit.get_name(), ptr);
    symbol_tbl.insert(unit.get_symbol(), ptr);
  }

  void register_unit(const string & name, const string & symbol,
		     const string & desc, const PhysicalQuantity & phy_quan)
  {
    register_unit(Unit(name, symbol, desc, phy_quan));
  }

  bool exists_name(const string & name) const noexcept
  {
    return name_tbl.contains(name);
  }

  bool exists_symbol(const string & symbol) const noexcept
  {
    return symbol_tbl.contains(symbol);
  }

  Unit * search_by_name(const string & name) const noexcept
  {
    auto p = name_tbl.search(name);
    return p ? p->second : nullptr;
  }

  Unit * search_by_symbol(const string & symbol) const noexcept
  {
    auto p = symbol_tbl.search(symbol);
    return p ? p->second : nullptr;
  }
};

extern UnitTable unit_tbl;

template <class Unit>
class Quantity 
{
  Unit * unit_ptr = nullptr;
  double value;

  void validate(Unit * ptr)
  {
    {
      const string & name = ptr->get_name();
      if (unit_tbl.exists_name(name))
	{
	  ostringstream s;
	  s << "Repeated unit name " << name;
	  throw domain_error(s.str());
	}
    }

    {
      const string & symbol = ptr->get_symbol();
      if (unit_tbl.exists_symbol(symbol))
	{
	  ostringstream s;
	  s << "Repeated unit symbol " << symbol;
	  throw domain_error(s.str());
	}
    }
  }

public:

  Quantity(Unit * __unit_ptr, const double val) 
    : unit_ptr(__unit_ptr), value(val)
  {
    validate(unit_ptr);
  }

  double get_value() const noexcept { return value; }

  operator double() const noexcept { return value; }
};



# endif // UNITS_H
