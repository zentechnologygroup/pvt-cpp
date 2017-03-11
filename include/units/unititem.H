# ifndef DESC_TABLE_H
# define DESC_TABLE_H

# include <memory>
# include <sstream>
# include <string>

# include <ah-string-utils.H>
# include <tpl_dynMapTree.H>

struct UnitItem
{
  const std::string name        = "Undefined";
  const std::string symbol      = "Undefined";
  const std::string description = "Undefined";
  const std::string latex_symbol = "Undefined";

  UnitItem(const std::string & __name, const std::string & __symbol,
	   const std::string & desc) noexcept
    : name(__name), symbol(__symbol), description(desc)
  {
    // empty
  }

  UnitItem(const std::string & name, const std::string & symbol,
	   const std::string & latex_symbol,
	   const std::string & desc) noexcept
    : name(name), symbol(symbol), description(desc), latex_symbol(latex_symbol)
  {
    // empty
  }

  string to_string() const
  {
    ostringstream s;
    s << "UnitItem name = " << name << endl
      << "Symbol        = " << symbol << endl
      << "Description   = " << description;
    return s.str();
  }

  string to_string(size_t width, size_t left_margin = 0) const
  {
    const string margin(left_margin, ' ');
    ostringstream s;
    s << margin << "UnitItem name = " << name << endl
      << margin << "Symbol        = " << symbol << endl
      << margin << "Description   = "
      << align_text_to_left_except_first(description, width, 16);
    return s.str();
  }

  friend ostream & operator << (ostream & out, const UnitItem & ui)
  {
    return out << ui.to_string();
  }
};

class UnitItemTable
{
  DynMapTree<std::string, const UnitItem * const> name_tbl; // index by unit
  DynMapTree<std::string, const UnitItem * const> symbol_tbl; // index by symbol

public:

  DynList<const UnitItem * const> items() const
  {
    return name_tbl.values();
  }

  DynList<string> names() const { return name_tbl.keys(); }

  DynList<string> symbols() const { return symbol_tbl.keys(); }

  // returns a iterator to pair<string, UnitItem*>
  auto get_it() { return name_tbl.get_it(); }

  void register_item(const UnitItem * ptr)
  {
    if (name_tbl.contains(ptr->name))
      {
	ostringstream s;
	s << " name " << ptr->name << " already exist";
	throw domain_error(s.str());
      }

    if (symbol_tbl.contains(ptr->symbol))
      {
	ostringstream s;
	s << "unit symbol " << ptr->symbol << " already exist";
	throw domain_error(s.str());
      }
    
    name_tbl.insert(ptr->name, ptr);
    symbol_tbl.insert(ptr->symbol, ptr);
  }

  bool exists_name(const string & name) const noexcept
  {
    return name_tbl.contains(name);
  }

  bool exists_symbol(const string & symbol) const noexcept
  {
    return symbol_tbl.contains(symbol);
  }

  const UnitItem * search_by_name(const string & name) const noexcept
  {
    auto p = name_tbl.search(name);
    return p ? p->second : nullptr;
  }

  const UnitItem * search_by_symbol(const string & symbol) const noexcept
  {
    auto p = symbol_tbl.search(symbol);
    return p ? p->second : nullptr;
  }

  size_t size() const noexcept { return name_tbl.size(); }

  void validate(const UnitItem * ptr, const string & str)
  {
    {
      const string & name = ptr->name;
      if (exists_name(str))
	{
	  ostringstream s;
	  s << "Repeated " << str << " name " << name;
	  throw domain_error(s.str());
	}
    }

    {
      const string & symbol = ptr->symbol;
      if (exists_symbol(str))
	{
	  ostringstream s;
	  s << "Repeated " << str << " symbol " << symbol;
	  throw domain_error(s.str());
	}
    }
  }
};


# endif // DESC_TABLE_H
