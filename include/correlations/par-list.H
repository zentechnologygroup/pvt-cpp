# ifndef PAR_LIST_H
# define PAR_LIST_H

# include <string>

# include <tpl_odhash.H>

# include <pvt-units.H>
# include <pvt-exceptions.H>

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

  static const size_t tbl_size = 100;

  ParList(size_t sz = tbl_size)
    : tbl(sz, first_par_hash, second_par_hash, ParEqual())
  {
    // empty
  }

  void insert(const string & name, double val, const Unit * unit_ptr)
  {
    ParPair p(name, ValPair(val, unit_ptr)); // porque es más rápido que &&
    tbl.insert(p);
  }

  void insert(const tuple<string, double, const Unit*> & par)
  {
    insert(get<0>(par), get<1>(par), get<2>(par));
  }

  void insert(const NamedPar & par)
  {
    if (not get<0>(par))
      ZENTHROW(ParameterNameNotSet, "Correlation parameter " + get<1>(par) +
	       " has not been set");
    
    insert(get<1>(par), get<2>(par), get<3>(par));
  }

  VtlQuantity search(const string & name) const
  {
    ParPair pp; pp.first = name;
    auto p = tbl.search(pp);
    if (p == nullptr)
      ZENTHROW(ParameterNameNotFound, "Parameter name " + name + " not found");

    const ValPair & vpair = p->second;
    return VtlQuantity(*vpair.second, vpair.first);
  }

  // Find a parameter from the possible names in `names`. `names` is a
  // list that contains the names of the parameters and their synonyms
  VtlQuantity search(const DynList<pair<string, const Unit*>> & names) const
  {
    ParPair pp;
    for (auto it = names.get_it(); it.has_curr(); it.next())
      {
	const auto & p = it.get_curr();
	const string & name = p.first;
	pp.first = name; // no copy of second
	auto ptr = tbl.search(pp);
	if (ptr)
	  {
	    const ValPair & val_pair = ptr->second;
	    VtlQuantity v(*val_pair.second, val_pair.first);
	    return VtlQuantity(*p.second, v);
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
    ZENTHROW(ParameterNameNotFound, s.str());
  }

  VtlQuantity operator () (const string & name) const
  {
    return search(name);
  }

  VtlQuantity
  operator () (const DynList<pair<string, const Unit*>> & names) const
  {
    return search(names);
  }

  void remove(const string & name)
  {
    tbl.remove(ParPair(name, ValPair(0, &Unit::null_unit)));
  }

  void remove(const tuple<string, double, const Unit*> & par)
  {
    remove(get<0>(par));
  }

  void remove(const NamedPar & par)
  {
    remove(get<1>(par));
  }
};


# endif // PAR_LIST_H
