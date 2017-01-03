# ifndef PAR_LIST_H
# define PAR_LIST_H

# include <string>

# include <tpl_odhash.H>

# include <units/pvt-units.H>

using namespace std;

using ValPair = pair<double, const Unit*>;
using ParPair = pair<string, ValPair>;

using NamedPar = tuple<bool, string, double, const Unit*>;

struct ParEqual
{
  bool operator () (const ParPair & p1, const ParPair & p2) const
  {
    return p1.first == p2.first;
  }
};

inline size_t first_par_hash(const ParPair & p)
{
  return dft_hash_fct(p.first);
}

inline size_t second_par_hash(const ParPair & p)
{
  return snd_hash_fct(p.first);
}

using HashParTbl = ODhashTable<ParPair, ParEqual>;

class ParList
{
public:
  HashParTbl tbl;

  static const size_t tbl_size = 40;

public:

  ParList(size_t sz = tbl_size)
    : tbl(sz, first_par_hash, second_par_hash, ParEqual())
  {
    // empty
  }

  void insert(const string & name, double val, const Unit * unit_ptr)
  {
    tbl.insert(make_pair(name, make_pair(val, unit_ptr)));
  }

  void insert(const NamedPar & par)
  {
    if (not get<0>(par))
      throw domain_error("Correlation parameter " + get<1>(par) +
			 " has not been set");
    
    insert(get<1>(par), get<2>(par), get<3>(par));
  }

  VtlQuantity search(const string & name) const
  {
    auto p = tbl.search(make_pair(name, make_pair(0, &Unit::null_unit)));
    if (p == nullptr)
      throw domain_error("Parameter name " + name + " not found");
    ValPair vpair = p->second;
    return VtlQuantity(*vpair.second, vpair.first);
  }

  VtlQuantity search(const DynList<pair<string, const Unit*>> & names) const
  {
    for (auto it = names.get_it(); it.has_curr(); it.next())
      {
	const auto & p = it.get_curr();
	const string & name = p.first;
	auto ptr = tbl.search(make_pair(name, make_pair(0, &Unit::null_unit)));
	if (ptr)
	  {
	    ValPair vpair = ptr->second;
	    VtlQuantity ret(*vpair.second, vpair.first);
	    if (vpair.second == p.second)
	      return ret;
	    return VtlQuantity(*p.second, ret);
	  }
      }
    ostringstream s;
    s << "name or aliases for {";
    for (auto it = names.get_it(); it.has_curr(); it.next())
      {
	const auto & p = it.get_curr();
	const string & name = p.first;
	s << name;
	if (&p != &names.get_last())
	  s << ", ";
      }
    s << "} have not been found in parameter list";
    throw domain_error(s.str());
  }

  VtlQuantity operator () (const string & name) const
  {
    return search(name);
  }

  VtlQuantity operator () (const DynList<pair<string, const Unit*>> & names) const
  {
    return search(names);
  }

  void remove(const string & name)
  {
    tbl.remove(make_pair(name, make_pair(0, &Unit::null_unit)));
  }

  void remove(const NamedPar & par)
  {
    remove(get<1>(par));
  }
};


# endif // PAR_LIST_H
