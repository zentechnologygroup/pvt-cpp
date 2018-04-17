# ifndef PAR_LIST_H
# define PAR_LIST_H

# include <string>

# include <tpl_odhash.H>

# include <pvt-units.H>
# include <pvt-exceptions.H>

using namespace std;

struct Correlation;

using ValPair = pair<double, const Unit*>;
using ParPair = pair<string, ValPair>;

inline ParPair make_ppar(const string & name, const VtlQuantity & par)
{
  return ParPair(name, ValPair(par.raw(), &par.unit));
}

inline ParPair ppar(const string & name, const VtlQuantity & par)
{
  return make_ppar(name, par);
}

# define PPAR(NAME) make_ppar(#NAME, NAME)

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

inline ParPair build_corrrelation_par(const string & par_name, double par_val,
				      const Unit * par_unit_ptr)
{
  return ParPair(par_name, ValPair(par_val, par_unit_ptr));
}

using HashParTbl = ODhashTable<ParPair, ParEqual>;

/** Define a list of parameters by name to be passed to a correlation
    for its calculation.

    This class defines an special way for passing parameters to a
    correlation. This is, until now, the fastest known way for compute
    a correlation value passing parameter by value. The main advantage
    of by value passing is that the parameter could be passed in any
    order. This feature, combined with the parameter naming, easily
    allows to program generic routines that receive correlations and
    compute them in a general way.

    The core of speed underlies in the use of a hash table for quickly
    inserting and restoring the parameters.

    @ingroup Correlations
    @author Leandro Rabindranath Leon
 */
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

  void insert(const string & name, double val, const Unit & unit)
  {
    ParPair p(name, ValPair(val, &unit)); // porque es más rápido que &&
    tbl.insert(p);
  }

  void insert(const string & name, double val, const Unit * unit_ptr)
  {
    insert(name, val, *unit_ptr);
  }

  void insert(const tuple<string, double, const Unit*> & par)
  {
    insert(get<0>(par), get<1>(par), get<2>(par));
  }

  void insert(const string & name, const VtlQuantity & q)
  {
    insert(name, q.raw(), &q.unit);
  }

  void insert(const NamedPar & par)
  {
    if (not get<0>(par))
      ALEPHTHROW(ParameterNameNotSet, "Correlation parameter " + get<1>(par) +
	       " has not been set");
    
    insert(get<1>(par), get<2>(par), get<3>(par));
  }

  void insert(const ParPair & par)
  {
    insert(par.first, par.second.first, par.second.second);
  }
  
  template <typename ... Args>
  ParList(const Args & ... args)
    : ParList()
  {
    insert_in_container(*this, args...);
  }

  VtlQuantity search(const string & name) const
  {
    ParPair pp; pp.first = name;
    auto p = tbl.search(pp);
    if (p == nullptr)
      ALEPHTHROW(ParameterNameNotFound, "Parameter name " + name + " not found");

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
    ALEPHTHROW(ParameterNameNotFound, s.str());
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

  void remove(const ParPair & par)
  {
    remove(par.first);
  }

  friend ostream & operator << (ostream & out, const ParList & pars)
  {
    for (auto it = pars.tbl.get_it(); it.has_curr(); it.next())
      {
	auto p = it.get_curr();
	out << "(" << p.first << " = " << p.second.first << " "
	    << p.second.second->name << ")";
      }
    return out;
  }
};


# endif // PAR_LIST_H
