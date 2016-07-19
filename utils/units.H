
# ifndef UNITS_H
# define UNITS_H

# include <string>
# include <sstream>
# include <limits>

# include <tpl_dynMapTree.H>

using namespace std; 

struct PhysicalQuantity 
{
  const string name;
  const string symbol;
  const string description;

  PhysicalQuantity(const string & __name, const string & __symbol,
		     const string & desc) noexcept
    : name(__name), symbol(__symbol), description(desc) {}
};

class PhysicalQuantityTable
{
  DynMapTree<string, PhysicalQuantity> name_tbl;
};

struct Unit 
{
  const string name;
  const string symbol;
  const string description;

  const PhysicalQuantity physical_quantity;

  double min_val = 0;
  double max_val = 0;
  
  Unit(const string & __name, const string & __symbol,
       const string & desc, const PhysicalQuantity & phy_quan) noexcept
    : name(__name), symbol(__symbol), description(desc),
      physical_quantity(phy_quan) {}

  bool value_is_valid(const double value) const noexcept
  {
    return value >= min_val and value <= max_val;
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
