# ifndef PAR_LIST_H
# define PAR_LIST_H

# include <string>

# include <tpl_dynSetHash.H>

# include <correlations/correlation.H>

using namespace std;

using ValPair = pair<double, const Unit*>;
using ParPair = pair<string, ValPair>;

using HashParTbl =
  DynMapHashTable<string, ValPair, Dft_Pair_Cmp<string, ValPair>, ODhashTable>;

class ParList
{
  HashParTbl tbl;

  static const size_t tbl_size = 30;

public:

  ParList() : tbl(tbl_size) {}

  void insert(const string & name, double val, const Unit * unit_ptr)
  {
    tbl.insert(name, make_pair(val, unit_ptr));
  }

  VtlQuantity search(const string & name) const
  {
    auto p = tbl.search(name);
    if (p == nullptr)
      throw domain_error("Parameter name " + name + " not found");
    ValPair vpair = p->second;
    return VtlQuantity(*vpair.second, vpair.first);
  }

  void remove(const string & name)
  {
    tbl.remove(name);
  }
};


# endif // PAR_LIST_H
