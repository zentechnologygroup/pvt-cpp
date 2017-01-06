
# ifndef MULTIUNITMAP_H
# define MULTIUNITMAP_H

# include <tuple>
# include <initializer_list>
# include <ahSort.H>
# include <ahFunctional.H>
# include <tpl_dynMapTree.H>

using namespace std;

class Unit;

class CompoundUnitTbl
{
  struct Cmp
  {
    bool operator () (const DynList<string> & l1,
		      const DynList<string> & l2) const noexcept
    {
      return lesser(sort(l1), sort(l2));
    }
  };

  DynMapTree<DynList<string>, const Unit * const, Avl_Tree, Cmp> tbl;

public:

  const Unit * search(const DynList<string> & names)
  {
    auto p = tbl.search(names);
    return p == nullptr ? nullptr : p->second;
  }

  bool insert(const DynList<string> & l, const Unit & unit)
  {
    return tbl.insert(l, &unit) != nullptr;
  }

  DynList<pair<DynList<string>, string>> items()
  {
    using T = pair<DynList<string>, string>;

    return tbl.items_ptr().maps<T>([] (auto p)
      {
	return make_pair(p->first, p->second->name);
      });
  }
};


# endif // MULTIUNITMAP_H
