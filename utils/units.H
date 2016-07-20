
# ifndef UNITS_H
# define UNITS_H

# include <string>
# include <sstream>
# include <limits>

# include <tpl_dynMapTree.H>

# include <unititem.H>

using namespace std; 

using PhysicalQuantity = UnitItem;

using Unit = UnitItem;

    // we store here both Physical quantities and units 
using UnitTable = UnitItemTable;


class Quantity 
{
  PhysicalQuantity * physical_quantity_ptr = nullptr; // its address in unit_tbl
  Unit * unit_ptr = nullptr;                      // its address in unit_tbl
  double value;                                   // magnitude

  static UnitTable unit_tbl; // contains all the physical quantities and units

  // checks that UnitItem object is registered in unit_tbl
  template <class U> void validate(U * ptr, const string & type_name)
  {
    {
      const string & name = ptr->name;
      if (unit_tbl.exists_name(name))
	{
	  ostringstream s;
	  s << "Repeated " << type_name << " name " << name;
	  throw domain_error(s.str());
	}
    }

    {
      const string & symbol = ptr->symbol;
      if (unit_tbl.exists_symbol(symbol))
	{
	  ostringstream s;
	  s << "Repeated " << type_name << " symbol " << symbol;
	  throw domain_error(s.str());
	}
    }
  }

  void validate_physical_quantity()
  {
    validate(physical_quantity_ptr, "PhysicalQuantity");
  }

  void validate_unit()
  {
    validate(unit_ptr, "Unit");
  }

public:

  Quantity(PhysicalQuantity * __physical_quantity,
	   Unit *              __unit_ptr,
	   const double        val) 
    : physical_quantity_ptr(__physical_quantity), unit_ptr(__unit_ptr),
      value(val)
  {
    validate_unit();
    validate_physical_quantity();
  }

  double get_value() const noexcept { return value; }

  operator double() const noexcept { return value; }

  const PhysicalQuantity & get_physical_quantity() const noexcept
  {
    return *physical_quantity_ptr;
  }

  const Unit & get_unit() const noexcept
  {
    return *unit_ptr;
  }

};





# endif // UNITS_H
