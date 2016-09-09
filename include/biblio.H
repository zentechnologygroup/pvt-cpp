
# ifndef BIBLIO_H
# define BIBLIO_H

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

  DynList<const BibEntry *> entries() const
  {
    return tbl.values();
  }
};



# endif // BIBLIO_H
