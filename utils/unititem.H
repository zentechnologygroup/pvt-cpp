# ifndef DESC_TABLE_H
# define DESC_TABLE_H

# include <sstream>
# include <string>
# include <tpl_dynMapTree.H>

struct UnitItem
{
  const std::string name;
  const std::string symbol;
  const std::string description;

  UnitItem(const std::string & __name, const std::string & __symbol,
	      const std::string & desc) noexcept
    : name(__name), symbol(__symbol), description(desc) {}

    string to_string() const
    {
      ostringstream s;
      s << "UnitItem name = " << name << endl
	<< "Symbol        = " << symbol << endl
	<< "Description   = " << description;
      return s.str();
    }

  friend ostream & operator << (ostream & out, const UnitItem & ui)
  {
    return out << ui.to_string();
  }
};

class UnitItemTable
{
  DynMapTree<std::string, UnitItem*> name_tbl;   // index by unit name
  DynMapTree<std::string, UnitItem*> symbol_tbl; // index by symbol name

public:

  auto get_it() { return name_tbl.get_it(); }

  ~UnitItemTable()
  {
    name_tbl.for_each([] (auto p) { delete p.second; });
  }

  void register_item(const UnitItem & unit)
  {
    if (name_tbl.contains(unit.name))
      {
	ostringstream s;
	s << " name " << unit.name << " already exist";
	throw domain_error(s.str());
      }

    if (symbol_tbl.contains(unit.symbol))
      {
	ostringstream s;
	s << "unit symbol " << unit.symbol << " already exist";
	throw domain_error(s.str());
      }
    
    UnitItem * ptr = new UnitItem(unit);
    name_tbl.insert(unit.name, ptr);
    symbol_tbl.insert(unit.symbol, ptr);
  }

  void register_item(const string & name, const string & symbol,
		     const string & desc)
  {
    register_item(UnitItem(name, symbol, desc));
  }

  bool exists_name(const string & name) const noexcept
  {
    return name_tbl.contains(name);
  }

  bool exists_symbol(const string & symbol) const noexcept
  {
    return symbol_tbl.contains(symbol);
  }

  UnitItem * search_by_name(const string & name) const noexcept
  {
    auto p = name_tbl.search(name);
    return p ? p->second : nullptr;
  }

  UnitItem * search_by_symbol(const string & symbol) const noexcept
  {
    auto p = symbol_tbl.search(symbol);
    return p ? p->second : nullptr;
  }

  size_t size() const noexcept { return name_tbl.size(); }
};


# endif // DESC_TABLE_H
