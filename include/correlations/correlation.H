
# ifndef CORRELATION_H
# define CORRELATION_H

# include <typeinfo>
# include <sstream>

# include <ahFunctional.H>
# include <ah-string-utils.H>
# include <ah-stl-utils.H>
# include <ah-comb.H>
# include <ah-zip.H>

# include <utils.H>

# include <units/pvt-units.H>
# include <biblio.H>

# include "pvt-exceptions.H"

# include "par-list.H"

struct CorrelationPar
{
  const string name;
  const string description;
  const Unit & unit;
  const VtlQuantity min_val;
  const VtlQuantity max_val;
  mutable bool min_from_author = true;
  mutable bool max_from_author = true;
  mutable string latex_symbol = "NOT-DEFINED";

private:

  double epsilon = 1e-3;

public:
  
  using P = pair<string, const Unit *>;

private:

  DynList<P> names_list;
  DynList<P> synonyms;
  
public:

  static string ruby_class()
  {
    return R"Ruby(
       class CorrelationPar
       attr_reader :name, :unit, :min_val, :max_val
       def initialize(name, unit, min_val, max_val)
         @name = name
         @unit = unit
         @min_val = min_val
         @max_val = max_val
       end
     end
    )Ruby";
  }

  string to_ruby_class() const
  {
    ostringstream s;
    s << "CorrelationPar.new('" << name << "', '" << unit.name << "', "
      << min_val.raw() << ", " << max_val.raw() << ")";
    return s.str();
  }

  void add_synonym(const string & name, const Unit & unit)
  {
    if (not this->unit.is_sibling(unit))
      {
	ostringstream s;
	s << "CorrelationPar::add_synonym(): synonym unit " << unit.name
	  << " is not physically related to parameter unit " << this->unit.name;
	ZENTHROW(WrongSiblingUnit, s.str());
      }

    auto p = make_pair(name, &unit);
    synonyms.append(p);
    names_list.append(p);
  }

  void add_synonym(const string & name, const string & unit_sym_name)
  {
    auto ptr = Unit::search(unit_sym_name);
    if (ptr == nullptr)
      {
	ostringstream s;
	s << "add_synonym(): unit name or symbol " << unit_sym_name
	  << " not found";
	ZENTHROW(WrongSiblingUnit, s.str());
      }
    add_synonym(name, *ptr);
  }

  void def_latex_symbol(const string & latex_symbol)
  {
    this->latex_symbol = latex_symbol;
  }

  const DynList<P> & get_synonyms() const noexcept { return synonyms; }

  /// Returns a list of alias associated to this parameter (including
  /// the name itself) 
  const DynList<P> & names() const { return names_list; }

  void set_epsilon(double ratio = 0.01)
  {
    Unit::validate_ratio(ratio);
    epsilon = ratio*(max_val.raw() - min_val.raw());
  }

  double get_epsilon() const noexcept { return epsilon; }

  double step_size(size_t n) const noexcept
  {
    return (max_val.raw() - min_val.raw()) / n;
  }

  string to_string() const
  {
    ostringstream s;
    s << name << " " << unit.symbol;
    return s.str();
  }

  DynList<string> to_list_string() const
  {
    return { name, unit.symbol, min_val.to_string(), max_val.to_string() };
  }

  CorrelationPar(const string & name, const Unit & unit, const string & desc, 
		 const VtlQuantity & min, const VtlQuantity & max)
    : name(name), description(desc), unit(unit),
      min_val(unit, min), max_val(unit, max)
  {
    names_list.append(make_pair(name, &unit));
  }

  CorrelationPar(const string & name, const BaseQuantity q,
		 const string & desc, double min, double max)
    : CorrelationPar(name, q.unit, desc, 
		     VtlQuantity(q.unit, min), VtlQuantity(q.unit, max)) {}

  bool check(const BaseQuantity & val) const noexcept
  {
    if (&val.unit == &unit)
      return val.raw() >= min_val.raw() - epsilon and
	val.raw() <= max_val.raw() + epsilon;

    VtlQuantity real_val(unit, val); // convert to unit for val.unit
    return real_val.raw() >= min_val.raw() - epsilon and
	real_val.raw() <= max_val.raw() + epsilon;
  }

  bool check(double val) const { return check(VtlQuantity(unit, val)); }

  void verify(const BaseQuantity & q) const
  {
    ostringstream s;
  
    if (q.get_value() < min_val.raw() - epsilon)
      {
	s << "Value " << q.get_value() << " is lesser than the minimum value "
	  << min_val << " for precondition ";
	ZENTHROW(OutOfParameterRange, s.str());
      }

    if (q.get_value() > max_val.raw() + epsilon)
      {
	s << "Value " << q << " is greater than the maximum value "
	  << max_val << " for precondition ";
	ZENTHROW(OutOfParameterRange, s.str());
      }
  }
};

struct Correlation
{
  const string type_name;
  const string subtype_name;
  const string name = "NOT-DEFINED";
  mutable string latex_symbol = "NOT-DEFINED";
  mutable string author = "NOT-DEFINED";
  const Unit & unit;
  const double min_val;
  const double max_val;
  mutable string title;
  mutable DynList<string> db;
  mutable DynList<string> notes;
  mutable DynList<const BibEntry*> refs;
  mutable bool hidden = false; // indicates whether correlation is or not hidden
  mutable bool hidden_calc = false;
  mutable bool hidden_grid = false;
  mutable long id = -1;
  mutable string __target_name = "NOT-DEFINED";

  mutable DynSetTree<string> par_names;

private:

  DynList<CorrelationPar> preconditions;
  size_t n = 0;

  static size_t counter;
  static DynMapTree<string, const Correlation *> tbl;

  static Array<const Correlation *> correlations_tbl;

  void set_target_name() const
  {
    __target_name = to_lower(split_camel_case(name).get_first());
  }

public:

  /// Returns the correlation target name
  const string & target_name() const { return __target_name; }

  // Returns the parameters names in the declaration order
  DynList<string> parameters_signature() const
  {
    return
      preconditions.maps<string>([] (const auto & par) { return par.name; });
  }

  // Returns the names of parameters involved in the correlation. The
  // first one is the return value. The remainder others are the
  // parameter names
  DynList<string> names() const
  {
    DynList<string> ret = unitarian_DynList(target_name());
    for (auto it = preconditions.get_it(); it.has_curr(); it.next())
      ret.append(it.get_curr().name);
    return ret;
  }

  const DynSetTree<string> & parameter_names() const
  {
    return par_names;
  }

  DynList<CorrelationPar::P> synonyms() const
  {
    DynList<CorrelationPar::P> ret;
    for (auto it = preconditions.get_it(); it.has_curr(); it.next())
      ret.append(it.get_curr().get_synonyms());

    return ret;
  }

  // Returns a list with all the possible correlation signatures; that
  // is all the valid permutations through the parameters names and
  // their synonyms
  DynList<DynList<string>> names_and_synonyms() const
  {
    DynList<DynList<string>> ret;
    for (auto it = preconditions.get_it(); it.has_curr(); it.next())
      {
	auto par = it.get_curr();
	DynList<string> col;
	col.append(par.name);
	par.get_synonyms().for_each([&col] (auto p) { col.append(p.first); });
	ret.append(col);
      }
    return build_perms(ret);
  }

  const CorrelationPar * search_par_name(const string & name) const
  {
    for (auto it = preconditions.get_it(); it.has_curr(); it.next())
      {
	const auto & par = it.get_curr();
	if (par.names().exists([&name] (const auto & p)
			       { return p.first == name; }))
	  return &par;
      }
    return nullptr;
  }

private:
  
  CorrelationPar * search_parameter(const string & par_name) const
  {
    auto par_ptr = preconditions.find_ptr([&par_name] (auto p)
					  { return p.name == par_name; });
    if (par_ptr == nullptr)
      {
	ostringstream s;
	s << "Correlation::add_par_synonym(): par name " << par_name
	  << " not found";
	ZENTHROW(SynonymNotFound, s.str());
      }

    return const_cast<CorrelationPar*>(par_ptr);
  }

  public:

  static DynList<pair<string, string>> latex_symbols()
  {
    DynMapTree<string, string> sbltbl;
    correlations_tbl.for_each([&sbltbl] (auto p)
      {
	p->get_preconditions().for_each([&sbltbl] (const auto & par)
          {
	    sbltbl.emplace(par.name, par.latex_symbol);
	  });
      });
    return sbltbl.items();
  }

  void add_par_synonym(const string & par_name, const string & synonym,
		       const string & unit_name)
  {
    auto par_ptr = search_parameter(par_name);
    par_ptr->add_synonym(synonym, unit_name);
    par_names.append(synonym);
  }

  void def_parameter_latex_symbol(const string & par_name,
				  const string & latex_symbol)
  {
    auto par_ptr = search_parameter(par_name);
    par_ptr->def_latex_symbol(latex_symbol);
  }

  void set_hidden() { hidden = hidden_calc = hidden_grid = true; }

  void set_hidden_calc() { hidden_calc = true; }

  void set_hidden_grid() { hidden_grid = true; }

  static string json_of_all_correlations();

  string to_json() const;

  static string to_json(const string & subtype_name);

  void set_author(const string & __author) { author = __author; }

  void set_title(const string & __title) { title = __title; }

  void add_db(const string & __db) { db.append(__db); }

  void add_note(const string & note) { notes.append(note); }

  void add_ref(const string & tag)
  {
    refs.append(BibEntry::find(tag));
  }

  const DynList<CorrelationPar> & get_preconditions() const noexcept
  {
    return preconditions;
  }

  const DynList<const CorrelationPar * const> preconditions_list() const 
  {
    return preconditions.maps<const CorrelationPar * const>([] (const auto & par)
      {
	return &par;
      });
  }

  // Returns the signature position of parameter `par_name`. If not
  // found return a position greater than the number of parameters
  size_t parameter_pos(const string & par_name) const
  {
    return preconditions.find_index([&par_name] (const auto & par)
				    {
				      return par.name == par_name;
				    });
  }

  DynList<const Unit *> get_par_types() const
  {
    return preconditions.maps<const Unit *>([] (const auto & par)
						 { return &par.unit; });
  }

  size_t get_num_pars() const noexcept { return n; }

  virtual string correlation_name() const
  {
    ostringstream s;
    s << author << endl
      << type_name << " : " << subtype_name << " : "
      << demangle(typeid(*this).name());
    if (hidden)
      s << " (WARNING: hidden correlation)";
    return s.str();
  }

  DynList<VtlQuantity> double_to_pars(const DynList<double> & vals) const
  {
    return zipEq(preconditions_list(), vals).maps<VtlQuantity>([] (auto p)
      {
	return VtlQuantity(p.first->name, p.second);
      });
  }

  string python_call(const DynList<VtlQuantity> & vals) const
  {
    size_t count = get_num_pars();
    ostringstream s;
    s << name << "Correlation(";
    zipEq(preconditions_list(), pointers_list(vals)).
      for_each([&s, &count] (auto p)
      {
	VtlQuantity q(p.first->unit, *p.second);
	s << q.get_value();
	if (--count)
	  s << ", ";
      });
    s << ")";

    return s.str();
  }    

  string call_string(const DynList<VtlQuantity> & vals) const
  {
    size_t count = get_num_pars();
    ostringstream s;
    s << name << "(";

    zipEq(preconditions_list(), pointers_list(vals)).
      for_each([&s, &count] (auto p)
      {
	VtlQuantity q(p.first->unit, *p.second);
	s << q;
	if (--count)
	  s << ", ";
      });
    s << ")";

    return s.str();
  }

  string call_string(const DynList<double> & vals) const
  {
    return
      call_string(zipEq(preconditions_list(), vals).maps<VtlQuantity>([] (auto p)
        {
	  return VtlQuantity(p.first->unit, p.second);
	}));
  }

  string call_string() const
  {
    ostringstream s;
    s << name << "(";
    const auto & last = preconditions.get_last();
    for (auto it = preconditions.get_it(); it.has_curr(); it.next())
      {
	const auto & curr = it.get_curr();
	s << curr.name;
	if (&curr != &last)
	  s << ", ";
      }
    s << ")";
    return s.str();
  }

  static DynList<const Correlation *> list() { return tbl.values(); }

  static const Array<const Correlation*> & array() noexcept
  {
    return correlations_tbl;
  }

  static size_t num_correlations() noexcept { return correlations_tbl.size(); }

  static const Correlation * search_by_name(const string & name)
  {
    auto ptr = tbl.search(name);
    return ptr != nullptr ? ptr->second : nullptr;
  }

  static DynList<string> type_list()
  {
    DynSetTree<string> s;
    for (auto it = tbl.get_it(); it.has_curr(); it.next())
      s.insert(it.get_curr().second->type_name);

    return s.keys();
  }

  static DynList<string> subtype_list(const string & type_name)
  {
    DynSetTree<string> s;
    for (auto it = tbl.get_it(); it.has_curr(); it.next())
      {
	auto p = it.get_curr();
	if (p.second->type_name == type_name)
	  s.insert(p.second->subtype_name);
      }

    return s.keys();
  }

  static DynList<const Correlation*> list(const string & subtype_name)
  {
    DynSetTree<const Correlation*> s;
    for (auto it = tbl.get_it(); it.has_curr(); it.next())
      {
	auto p = it.get_curr();
	if (p.second->subtype_name == subtype_name)
	  s.insert(p.second);
      }

    return s.keys();
  }

protected:

  Correlation(const string & type_name, const string & subtype_name,
	      const string & name, const string & latex_symbol,
	      const Unit & result_unit,
	      double min_val, double max_val)
    : type_name(type_name), subtype_name(subtype_name), name(name),
      latex_symbol(latex_symbol), unit(result_unit),
      min_val(min_val), max_val(max_val)
  {
    if (tbl.has(name))
      {
	ostringstream s;
	s << "Correlation::Correlation: duplicated correlation name" << name;
	ZENTHROW(DuplicatedCorrelationName, s.str());
      }

    tbl.insert(name, this);
    correlations_tbl.append(this);
    id = counter++;
    set_target_name();
  }

  Correlation(const string & type_name, const string & subtype_name,
	      const string & name, const string & latex_symbol,
	      const Unit & result_unit)
    : Correlation(type_name, subtype_name, name, latex_symbol, result_unit,
		  result_unit.min_val, result_unit.max_val) {}

  Correlation(const string & type_name, const string & subtype_name,
	      const string & name, const string & latex_symbol,
	      const Unit & result_unit, double min_val)
    : Correlation(type_name, subtype_name, name, latex_symbol,
		  result_unit, min_val, result_unit.max_val) {}

  virtual ~Correlation() = 0; // Correlation is abstract

  CorrelationPar &
  add_parameter(const string & name, const Unit & unit,
		const string & description,
		const VtlQuantity & min, const VtlQuantity & max)
  {
    auto min_aux = VtlQuantity(unit, min);
    auto max_aux = VtlQuantity(unit, max);
    if (min_aux > max_aux)
      {
	ostringstream s;
	s << "minimum value " << min_aux << " is greater than maximum value "
	  << max_aux << " for parameter " << name;
	ZENTHROW(OutOfParameterRange, s.str());
      }

    CorrelationPar par(name, unit, description, min, max);
    CorrelationPar & ret = preconditions.append(move(par));
    ++n;
    par_names.append(name);
    return ret;
  }

  CorrelationPar &  add_parameter(const string & name, const Unit & unit,
				  const string & description,
				  const VtlQuantity & min)
  {
    CorrelationPar & ret = add_parameter(name, unit, description,
					 min, VtlQuantity(unit, unit.max_val));
    ret.max_from_author = false;
    return ret;
  }

  CorrelationPar & add_parameter(const string & name, const Unit & unit,
				 const string & description)
  {
    CorrelationPar & ret = add_parameter(name, unit, description,
					 VtlQuantity(unit, unit.min_val),
					 VtlQuantity(unit, unit.max_val));
    ret.min_from_author = ret.max_from_author = false;
    return ret;
  }

  CorrelationPar add_parameter(const string & name, const Unit & unit,
			       const string & description,
			       double min, double max)
  {
    return add_parameter(name, unit, description,
			 VtlQuantity(unit, min), VtlQuantity(unit, max));
  }

public:

  string to_string() const
  {
    size_t i = 1;
    auto prows = preconditions.maps<DynList<string>>([&i] (const auto & c)
      {
	DynList<string> ret = { "    " };
	ret.append(c.to_list_string());
	ret.append(std::to_string(i++));
	return ret;
      });

    prows.insert(rep<string>(6, ""));
    prows.insert({ "    ", "name", "unit", "min", "max", "number" });
    auto mat = format_string(prows);

    ostringstream s;
    s << correlation_name() << endl
      << "  Return type = " << unit.symbol << endl
      << "  Min result  = " << min_val << endl
      << "  Max result  = " << max_val << endl
      << endl
      << "  Parameters (" << get_num_pars() << "):" << endl
      << ::to_string(mat);

    return s.str();
  }

  static string ruby_class()
  {
    ostringstream s;
    s << "class Correlation" << endl
      << "attr_reader :name, :unit, :min_val, :max_val, :pars" << endl
      << "def initialize(name, unit, min_val, max_val, *args)" << endl
      << "@name = name" << endl
      << "@unit = name" << endl
      << "@min_val = min_val" << endl
      << "@max_val = max_val" << endl
      << "@pars = *args" << endl
      << "end" << endl
      << "end";
    return s.str();
  }

  static string to_ruby_def() 
  {
    ostringstream s;

    s << CorrelationPar::ruby_class() << endl
      << endl
      << ruby_class() << endl
      << endl
      << "$correlations_tbl = " << endl
      << "  { " << endl;

    auto last_corr = correlations_tbl.get_last();
    for (auto corr_it = correlations_tbl.get_it();
	 corr_it.has_curr(); corr_it.next())
      {
	auto corr = corr_it.get_curr();
	s << "'" << corr->name << "' => Correlation.new('" << corr->name << "', "
	  << "'" << corr->unit.name << "', " << corr->min_val << ", "
	  << corr->max_val;
	for (auto it = corr->preconditions.get_it(); it.has_curr(); it.next())
	  {
	    const auto & par = it.get_curr();
	    s << ", " << par.to_ruby_class();
	  }
	s << ")";
	if (corr != last_corr)
	  s << ", " << endl;
      }

    s << "  }" << endl;

    return s.str();
  }

  string full_desc(size_t width = 60, size_t left_margin = 4) const
  {
    ostringstream s;
    if (not title.empty())
      s << align_text_to_left(title, 60) << endl
	<< endl;
    s << to_string() << endl;

    if (not db.is_empty())
      s << "  DATA BANK:" << endl
	<< endl;
    for (auto it = db.get_it(); it.has_curr(); it.next())
      s << align_text_to_left(it.get_curr(), width, left_margin) << endl
	<< endl;

    if (not notes.is_empty())
      s << "  Notes:" << endl
	<< endl;

    for (auto it = notes.get_it(); it.has_curr(); it.next())
      s << align_text_to_left(it.get_curr(), width, left_margin) << endl
	<< endl;

    if (not refs.is_empty())
      s << "  References:" << endl
	<< endl;

    for (auto it = refs.get_it(); it.has_curr(); it.next())
      s << shift_lines_to_left(it.get_curr()->to_string(), left_margin) << endl
	<< endl;

    return s.str();
  }

  friend ostream & operator << (ostream & out, const Correlation & c)
  {
    return out << c.to_string();
  }

  virtual VtlQuantity
  compute(const DynList<VtlQuantity> &, bool check = true) const = 0;

  template <typename ... Args>
  VtlQuantity compute(bool check, Args ... args) const
  {
    DynList<VtlQuantity> pars_list;
    append_in_container(pars_list, args ...);
    return compute(pars_list, check);
  }

  VtlQuantity compute_and_check(const DynList<VtlQuantity> & pars,
				bool check = true) const
  {
    return verify_result(compute(pars, check));
  }

  tuple<double, string, bool, string> execute(DynList<VtlQuantity> & pars,
					      bool check = true) const
  {
    try
      {
	const auto result = compute(pars, check);
	auto status = check_result(VtlQuantity(unit, result));
	if (status)
	  return make_tuple(result.raw(), unit.symbol, true, "");

	ostringstream s;
	s << "Correlation result " << result << " is out of range ["
	  << min_val << ", " << max_val << "]";
	OutOfCorrelationRange e(__LINE__, __FILE__, "OutOfCorrelationRange",
				s.str().c_str());
	return make_tuple(result.raw(), unit.symbol, false, e.what());
      }
    catch (exception &e)
      {
	return make_tuple(0, unit.symbol, false, e.what());
      }
  }

  double compute(const DynList<double> & values, bool check = true) const
  {
    DynList<VtlQuantity> pars;
    auto it = get_pair_it(preconditions, values);
    for (;it.has_curr(); it.next())
      {
	auto p = it.get_curr();
	pars.append(VtlQuantity(p.first.unit, p.second));
      }

    if (it.has_curr())
      {
	ostringstream s;
	s << "Correlation::compute: number of effective arguments "
	  << values.size()
	  << " is different from number of declared parameters "
	  << get_num_pars();
	ZENTHROW(InvalidNumberOfParameters, s.str());
      }

    VtlQuantity ret = { unit, compute(pars, check) }; // here conversion is done
    return ret.get_value();
  }

  double
  compute_and_check(const DynList<double> & pars, bool check = true) const
  {
    return verify_result(VtlQuantity(unit, compute(pars, check))).raw();
  }

  using ParByName = pair<string, double>;

  /// Compute correlation by receiving an unseorted list of pair par-name,value
  VtlQuantity
  compute_by_names(const DynList<ParByName> & pair_list,
		   bool check = true) const
  {
    DynList<VtlQuantity> vals;
    for (auto it = preconditions.get_it(); it.has_curr(); it.next())
      {
	const auto & par = it.get_curr();
	auto ptr_val = pair_list.find_ptr([&par] (auto p)
          {
	    return par.names().exists([&p] (const auto & pp)
	      {
		return p.first == pp.first;
	      });

	  });
	
	if (ptr_val == nullptr)
	  {
	    ostringstream s;
	    s << "compute_by_names() for correlation " << name
	      << ": parameter name " << par.name << " was not found";
	    ZENTHROW(ParameterNameNotFound, s.str());
	  }
	vals.append(VtlQuantity(par.unit, ptr_val->second));
      }

    return compute(vals, check);
  }

  using NamedPar = tuple<bool, string, double, const Unit*>;

  VtlQuantity compute_by_names(const DynList<NamedPar> & pair_list,
			       bool check = true) const
  {
    DynList<VtlQuantity> vals;
    for (auto it = preconditions.get_it(); it.has_curr(); it.next())
      {
	const auto & par = it.get_curr();
	auto ptr_val = pair_list.find_ptr([&par] (auto t)
          {
	    return par.names().exists([&t] (const auto & p)
	      {
		return p.first == get<1>(t); 
	      });
	  });
	
	if (ptr_val == nullptr)
	  {
	    ostringstream s;
	    s << "compute_by_names() for correlation " << name
	      << ": parameter name " << par.name << " was not found";
	    ZENTHROW(ParameterNameNotFound, s.str());
	  }
	
	if (not get<0>(*ptr_val))
	  {
	    ostringstream s;
	    s << "compute_by_names() for correlation " << name
	      << ": parameter name " << par.name << " is not set";
	    ZENTHROW(ParameterNameNotSet, s.str());
	  }
	vals.append(VtlQuantity(*get<3>(*ptr_val), get<2>(*ptr_val)));
      }

    return compute(vals, check);
  }

  VtlQuantity
  compute_by_names(const ParList & par_list, bool check = true) const
  {
    DynList<VtlQuantity> vals;
    for (auto it = preconditions.get_it(); it.has_curr(); it.next())
      {
	auto & par = it.get_curr();
	VtlQuantity val = par_list.search(par.names());
	vals.append(val);
      }

    return compute(vals, check);
  }
  
  VtlQuantity tuned_compute_and_check(const DynList<VtlQuantity> & pars,
				      double c, double m,
				      bool check = true) const
  {
    return verify_result(VtlQuantity(unit, c + m*compute(pars, check).raw()));
  }

  tuple<double, string, bool, string> tuned_execute(DynList<VtlQuantity> & pars,
						    double c, double m,
						    bool check = true) const
  {
    try
      {
	const auto result = c + m*compute(pars, check).raw();
	auto status = check_result(VtlQuantity(unit, result));
	if (status)
	  return make_tuple(result, unit.symbol, true, "");

	ostringstream s;
	s << "Correlation result " << result << " is out of range ["
	  << min_val << ", " << max_val << "]";
	return make_tuple(result, unit.symbol, false, s.str());
      }
    catch (exception &e)
      {
	return make_tuple(0, unit.symbol, false, e.what());
      }
  }

  double tuned_compute(const DynList<double> & values, double c, double m,
		       bool check = true) const
  {
    DynList<VtlQuantity> pars;
    auto it = get_pair_it(preconditions, values);
    for (;it.has_curr(); it.next())
      {
	auto p = it.get_curr();
	pars.append(VtlQuantity(p.first.unit, p.second));
      }

    if (it.has_curr())
      {
	ostringstream s;
	s << "Correlation::compute: number of effective arguments "
	  << values.size()
	  << " is different from number of declared parameters "
	  << get_num_pars();
	ZENTHROW(InvalidNumberOfParameters, s.str());
      }

    VtlQuantity ret = { unit, c + m*compute(pars, check).raw() };
    return ret.raw();
  }

  VtlQuantity
  tuned_compute_by_names(const DynList<ParByName> & pair_list,
			 double c, double m, bool check = true) const
  {
    return VtlQuantity(unit, c + m*compute_by_names(pair_list, check).raw());
  }

  VtlQuantity
  tuned_compute_by_names(const DynList<NamedPar> & pair_list,
			 double c, double m, bool check = true) const
  {
    return VtlQuantity(unit, c + m*compute_by_names(pair_list, check).raw());
  }

  VtlQuantity
  tuned_compute_by_names(const ParList & par_list,
			 double c, double m, bool check = true) const
  {
    return VtlQuantity(unit, c + m*compute_by_names(par_list, check).raw());
  }

  double
  tuned_compute_and_check(const DynList<double> & pars, double c, double m,
			  bool check = true) const
  {
    return verify_result(VtlQuantity(unit, c + m*compute(pars, check))).raw();
  }

  /** return a list of validated and adequately converted correlation
      parameters
      
      @throw domain_error if there is an conversion error
      @throw range_error if a parameter is out of precondition range
  */
  const DynList<VtlQuantity> &
  verify_preconditions(const DynList<VtlQuantity> & pars) const
  {
    auto it = get_enum_zip_it(preconditions, pars);
    for (/* already initialized */; it.has_curr(); it.next())
      {
	auto p = it.get_curr();
	const auto & precondition = get<0>(p);
	const string & par_name = precondition.name;
	const auto & par = get<1>(p);
	if (not (par_name == "p" or par_name == "t") and // TODO lista hash
	    not precondition.check(par))
	  {
	    auto i = get<2>(p) + 1;
	    ostringstream s;
	    s << "Parameter " << i << " (" << par_name << " = " << par
	      << ") in correlation " << name 
	      << " does not satisfy application development range"; 
	    ZENTHROW(OutOfParameterRange, s.str());
	  }
      }

    if (not it.completed())
      {
	ostringstream s;
	s << "number of preconditions " << preconditions.size()
	  << " is different than number of parameters " << pars.size();
	ZENTHROW(InvalidNumberOfParameters, s.str());
      }

    return pars;
  }

  VtlQuantity verify_result(const VtlQuantity & result) const
  {
    assert(&result.unit == &unit);
    if (result.get_value() < min_val or result.get_value() > max_val)
      {
	ostringstream s;
	s << "In correlation " << name << " " << subtype_name << " " << type_name
	  << " : return value (" << result.get_value() << " "
	  << result.unit.symbol << ") is out of range ["
	  << min_val << ", " << max_val << "] defined for the correlation";
	ZENTHROW(OutOfCorrelationRange, s.str());
      }

    return result;
  }

  bool check_result(const VtlQuantity & result) const
  {
    VtlQuantity ret = result;
    return ret.get_value() >= min_val and ret.get_value() <= max_val;
  }

  static DynList<string> all_parameter_names()
  {
    DynSetTree<string> names;
    for (auto it = correlations_tbl.get_it(); it.has_curr(); it.next())
      it.get_curr()->preconditions.for_each([&names] (auto p)
					    {
					      names.insert(p.name);
					    });
    return names.keys();
  }
};

inline Correlation::~Correlation() {}

class CorrelationInvoker
{
  const Correlation * correlation_ptr = nullptr;
  char * json_buffer = nullptr;
  const size_t buf_sz = 0;
  DynList<double> pars;

public:

  CorrelationInvoker(long id, char buf[], size_t sz)
    : correlation_ptr(Correlation::array()[id]), json_buffer(buf), buf_sz(sz) {}

  void push(double val) { pars.append(val); }

  long call();
};

/// returns a proxy id for future invocation of correlation with id
extern long open_correlation_call(long id, char buf[], size_t sz);

/// pushes parameter val to the correlation proxy id. Return 1 if
/// everything is ok; 0 otherwise 
extern long push_correlation_call(long proxy_id, double val);

/// performs the call and puts in the buffer specified in
/// `open_correlation_call()` the json containing the result. Returns
/// 1 if everything is ok; 0 otherwise
extern long exec_correlation_call(long proxy_id);

/// free the proxy 
extern long free_correlation_call(long proxy_id);

# define Header_Correlation_Type(type_name, hidden)	\
  struct type_name : public Correlation					\
  {									\
    type_name(const string & subtype_name,				\
	      const string & name, const string & latex_symbol,		\
	      const Unit & result_unit, double min_val, double max_val)	\
      : Correlation(#type_name, subtype_name, name, latex_symbol,	\
		    result_unit, min_val, max_val) {}			\
    virtual ~type_name() = 0;						\
    using Correlation::add_parameter;					\
    bool is_hidden = hidden;						\
  };									\
									\
  inline type_name::~type_name() {}

# define Declare_Correlation_Type(type_name)	\
  Header_Correlation_Type(type_name, false)

# define Declare_Hidden_Correlation_Type(type_name)	\
  Header_Correlation_Type(type_name, true)

Declare_Hidden_Correlation_Type(TmpFunction)
Declare_Correlation_Type(OilFunction)
Declare_Correlation_Type(OilCorrelation);
Declare_Correlation_Type(GasCorrelation);
Declare_Correlation_Type(GasFunction);
Declare_Correlation_Type(WaterCorrelation);

# define Declare_Correlation_Subtype(subtype_name, type_name, symbol)	\
  struct subtype_name : public type_name				\
  {									\
    subtype_name(const string & name, const Unit & result_unit,		\
		 double min_val, double max_val)			\
      : type_name(#subtype_name, name, symbol, result_unit,		\
		  min_val, max_val) {}					\
									\
    subtype_name(const string & name, const Unit & unit, double min_val) \
      : type_name(#subtype_name, name, symbol, unit,			\
		  min_val, unit.max_val) {}				\
									\
    subtype_name(const string & name, const Unit & unit)		\
      : type_name(#subtype_name, name, symbol, unit, unit.min_val,	\
		  unit.max_val) {}					\
    virtual ~subtype_name() = 0;					\
									\
  protected:								\
									\
    using type_name::add_parameter;					\
									\
  };									\
									\
  inline subtype_name::~subtype_name() {}

template <typename F> struct compute_arity;

template <typename Ret, typename ...Args> struct compute_arity<Ret(Args...)>
{
  static constexpr std::size_t value = sizeof...(Args);
};

template <typename Func, typename T, std::size_t ... Is>
decltype(auto)
  call_compute(Func&& f, const DynList<T>& pars, std::index_sequence<Is...>)
{
  std::vector<T> v = to_vector(pars);
 
  return std::forward<Func>(f)(v.at(Is)...);
}
 
# define Correlation_Singleton(name)					\
  public:								\
									\
  static const name & get_instance()					\
  {									\
    static name instance;						\
    return instance;							\
  }									\
									\
  static const Correlation * correlation()				\
  {									\
    return & get_instance();						\
  }									\
									\
  name(const name&) = delete;						\
  void operator = (const name&) = delete;				\
  									\
private:

template <class CorrType, typename ... Args>
auto direct_call(const Correlation & corr_ptr, Args ... args)
{
  return static_cast<CorrType*>(&corr_ptr)->call(args...);
}

template <class CorrType, typename ... Args>
auto tuned_direct_call(const Correlation & corr_ptr, double c, double m,
		       Args ... args)
{
  double r = static_cast<CorrType*>(&corr_ptr)->call(args...);
  return c + m*r;
}

template <class CorrType, typename ... Args>
auto direct_impl(const Correlation & corr_ptr, Args ... args)
{
  return static_cast<CorrType*>(&corr_ptr)->impl(args...);
}

template <class CorrType, typename ... Args>
auto tuned_direct_impl(const Correlation & corr_ptr, double c, double m,
		       Args ... args)
{
  double r = static_cast<CorrType*>(&corr_ptr)->impl(args...);
  return c + m*r;
}

Declare_Correlation_Subtype(TmpCorrelation, TmpFunction, "");

    // correlations & functions associated to oil
Declare_Correlation_Subtype(BubblePointPressure, OilCorrelation, "P_b");
Declare_Correlation_Subtype(SolutionGasOilRelation, OilCorrelation, "R_s");
Declare_Correlation_Subtype(UndersaturatedOilVolumeFactor,
			    OilCorrelation, "B_{oa}");
Declare_Correlation_Subtype(SaturatedOilVolumeFactor, OilCorrelation, "B_{ob}");
Declare_Correlation_Subtype(SaturatedOilViscosity, OilCorrelation, "\\mu_{ob}");
Declare_Correlation_Subtype(UndersaturatedOilViscosity, OilCorrelation,
			    "\\mu_{oa}");
Declare_Correlation_Subtype(SaturatedOilCompressibility, OilCorrelation,
			    "c_{ob}");
Declare_Correlation_Subtype(UndersaturatedOilCompressibility, OilCorrelation,
			    "c_{oa}");
Declare_Correlation_Subtype(SaturatedOilDensity, OilFunction, "\\rho_{ob}");
Declare_Correlation_Subtype(UndersaturatedOilDensity, OilFunction, "\\rho_{oa}");
Declare_Correlation_Subtype(OilInterfacialTension, OilCorrelation, "\\sigma_{go}");
Declare_Correlation_Subtype(DeadOilViscosity, OilCorrelation, "\\mu_{od}");

    // correlations & functions associated to gas
Declare_Correlation_Subtype(HydrocarbonGasPseudocriticalPressure, GasCorrelation, "P_{pcHC}");
Declare_Correlation_Subtype(MixtureGasPseudocriticalPressure, GasCorrelation, "P_{pcM}");
Declare_Correlation_Subtype(GasPseudocriticalPressureC7, GasCorrelation,
			    "P_{pcC_7+}");
Declare_Correlation_Subtype(GasViscosity, GasCorrelation, "\\mu_{g}");
Declare_Correlation_Subtype(GasDensity, GasFunction, "\\rho_{g}")
Declare_Correlation_Subtype(GasVolumeFactor, GasFunction, "B_{g}");
Declare_Correlation_Subtype(GasCompressibility, GasCorrelation, "Z\\ factor");
Declare_Correlation_Subtype(HydrocarbonGasPseudocriticalTemperature, GasCorrelation,
			    "T_{pcHC}");
Declare_Correlation_Subtype(MixtureGasPseudocriticalTemperature, GasCorrelation,
			    "T_{pcM}");
//Declare_Correlation_Subtype(GasSpecificGravity, GasCorrelation, "\\gamma_g");
Declare_Correlation_Subtype(HydrocarbonGasSpecificGravity, GasCorrelation, "\\gamma_{gHC}");
Declare_Correlation_Subtype(GasIsothermalCompressibility, GasCorrelation,
			    "c_{g}");
Declare_Correlation_Subtype(GasPseudoreducedPressure, GasFunction, "P_{pr}");
Declare_Correlation_Subtype(GasPseudoreducedTemperature, GasFunction, "T_{pr}");

    // correlations & functions associated to water
Declare_Correlation_Subtype(WaterDensity, WaterCorrelation, "\\rho_{w}");
Declare_Correlation_Subtype(PureWaterDensity, WaterCorrelation, "\\rho_{pw}");
Declare_Correlation_Subtype(WaterViscosity, WaterCorrelation, "\\mu_{w}");
//Declare_Correlation_Subtype(WaterVolumeFactor, WaterCorrelation, "B_{w}");
Declare_Correlation_Subtype(SaturatedWaterVolumeFactor, WaterCorrelation,
			    "B_{wb}");
Declare_Correlation_Subtype(UndersaturatedWaterVolumeFactor, WaterCorrelation,
			    "B_{wa}");
Declare_Correlation_Subtype(WaterInterfacialTension, WaterCorrelation,
			    "\\sigma_{gw}");
//Declare_Correlation_Subtype(WaterIsothermalCompressibility, WaterCorrelation, "c_{w}");
Declare_Correlation_Subtype(SaturatedWaterIsothermalCompressibility,
			    WaterCorrelation, "c_{wb}");
Declare_Correlation_Subtype(UndersaturatedWaterIsothermalCompressibility,
			    WaterCorrelation, "c_{wa}");
Declare_Correlation_Subtype(SolutionGasWaterRelation, WaterCorrelation,
			    "R_{sw}");
Declare_Correlation_Subtype(VaporPressure, WaterCorrelation,
			    "P_{v}");
Declare_Correlation_Subtype(WetGasVolumeFactor, GasFunction, "B_{wg}");
Declare_Correlation_Subtype(WetGasSpecificGravity, GasCorrelation, "Y_{wgR}");
Declare_Correlation_Subtype(EquivalentVolume, GasCorrelation, "V_{eq}");
Declare_Correlation_Subtype(AdditionalGasProduced, GasCorrelation, "G_{pa}");
Declare_Correlation_Subtype(ProducingGasOilRatio, GasFunction, "GOR");

# endif // CORRELATION_H
