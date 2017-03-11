# ifndef BIBLIO_H
# define BIBLIO_H

# include <istream>
# include <string>

# include <htlist.H>
# include <tpl_dynMapTree.H>

using namespace std;

class BibEntry;

class BibEntry
{
  const string id;
  const string entry; // translated from BibTeX entry to string
  static DynMapTree<string, const BibEntry*> tbl;
  
public:
  
  BibEntry(const string & id, const string & entry) : id(id), entry(entry)
  {
    tbl.insert(id, this);
  }
  const string & get_id() const noexcept { return id; }
  const string & get_entry() const noexcept { return entry; }
  static DynList<const BibEntry *> entries() { return tbl.values(); }
  static const BibEntry * search(const string & tag)
  {
    auto p = tbl.search(tag);
    return p ? p->second : nullptr;
  }
  static const BibEntry * find(const string & tag)
  {
    auto p = tbl.search(tag);
    if (p == nullptr)
      throw domain_error("Bibliographical tag " + tag + " not found");
    return p->second;
  }
  const string to_string() const { return entry; }
  friend ostream & operator << (ostream & out, const BibEntry & b)
  {
    return out << b.to_string();
  }
};
# endif // BIBLIO_H
