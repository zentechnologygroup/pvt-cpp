/** This file contains the base machinery needed for calculating pvt
   correlations
   
   Aleph-w Leandro Rabindranath Leon
 */
# ifndef CORRELATION_H
# define CORRELATION_H

# include <typeinfo>
# include <sstream>

# include <ahFunctional.H>
# include <ah-string-utils.H>
# include <ah-stl-utils.H>
# include <ah-comb.H>
# include <ah-zip.H>

# include <pvt-units.H>
# include <biblio.H>

# include <pvt-units.H>
# include <pvt-exceptions.H>

# include "par-list.H"

/** Complex parameter by name.

      `NamedPar` is a quadruple used to represent a correlation
      parameter by name, whose structure is the following:

      1. get<0>(par): a boolean value that indicates whether the
      parameter is or not valid. In some cases, concretely when a
      calculation depends on other calculation, it is needed to know
      whether the result of the previous calculation was or not
      valid. In this case, this field would indicate such thing.

      2. get<1>(par): a string with the name of parameter.

      3. get<2>(par): a double containing the value of parameter.

      4. get<3>(par): a pointer to the unit in which the value is
      represented. 

      @ingroup Correlations
      @author Leandro Rabindranath Leona
   */
using NamedPar = tuple<bool, string, double, const Unit*>;

/** Define a correlation parameter

    @ingroup Correlations
    @author Leandro Rabindranath Leona
 */
struct CorrelationPar
{
  const string name;  /// name of parameter
  const string description; 
  const Unit & unit;
  const VtlQuantity min_val;
  const VtlQuantity max_val;
  mutable bool min_from_author = true; /// true if min val was defined for author
  mutable bool max_from_author = true; /// true if max val was defined for author
  mutable string latex_symbol = "NOT-DEFINED";

private:

  double epsilon = 1e-3;

public:

  using P = pair<string, const Unit *>;

private:

  DynList<P> names_list;
  DynList<P> synonyms;
  
public:

  /// return a Ruby representation of parameter class 
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

  /// Return a ruby class instance of `this`
  string to_ruby_class() const
  {
    ostringstream s;
    s << "CorrelationPar.new('" << name << "', '" << unit.name << "', "
      << min_val.raw() << ", " << max_val.raw() << ")";
    return s.str();
  }

  /// Define a synonym `name` with a unit
  void add_synonym(const string & name, const Unit & unit)
  {
    if (not this->unit.is_sibling(unit))
      {
	ostringstream s;
	s << "CorrelationPar::add_synonym(): synonym unit " << unit.name
	  << " is not physically related to parameter unit " << this->unit.name;
	ALEPHTHROW(WrongSiblingUnit, s.str());
      }

    auto p = make_pair(name, &unit);
    synonyms.append(p);
    names_list.append(p);
  }

  /// Define a synonym `name` whose unit is of name or symbol unit_sym_name
  void add_synonym(const string & name, const string & unit_sym_name)
  {
    auto ptr = Unit::search(unit_sym_name);
    if (ptr == nullptr)
      {
	ostringstream s;
	s << "add_synonym(): unit name or symbol " << unit_sym_name
	  << " not found";
	ALEPHTHROW(WrongSiblingUnit, s.str());
      }
    add_synonym(name, *ptr);
  }

  /// Define a string corresponding to the LaTeX symbol of parameter
  void def_latex_symbol(const string & latex_symbol)
  {
    this->latex_symbol = latex_symbol;
  }

  /// Return a list of pairs containing synonyms name and unit
  const DynList<P> & get_synonyms() const noexcept { return synonyms; }

  /// Returns a list of alias associated to this parameter (including
  /// the name itself) 
  const DynList<P> & names() const { return names_list; }

  /// Return a stringfied version of `this`
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

  /** Build a correlation parameter.

      In this method minimum and maximum values are specified with
      `VtlQuantity` objects containing specific units.

      Note that the parameter value is not given.

      @param[in] name the of parameter.
      @param[in] unit parameter unit.
      @param[in] desc parameter description.
      @param[in] min minimum value of parameter. Conversion is done if
      minimum value unit is different than `unit`.
      @param[in] max maximum value of parameter. Conversion is done if
      maximum value unit is different than `unit`.
      @exception UnitConversionNotFound if there conversion does not
      exist.
      @exception bad_alloc if there is enough memory.
   */
  CorrelationPar(const string & name, const Unit & unit, const string & desc, 
		 const VtlQuantity & min, const VtlQuantity & max)
    : name(name), description(desc), unit(unit),
      min_val(unit, min), max_val(unit, max)
  {
    names_list.append(make_pair(name, &unit));
  }

  /** Build a correlation parameter.

      In this method minimum and maximum values are specified in the
      same unit that the given given value contained in parameter `q`.

      @param[in] name the of parameter
      @param[in] q parameter value specified with  `VtlQuantity` or `Quantity`
      @param[in] desc parameter description
      @param[in] min minimum value of parameter in the same unit than
      `q`
      @param[in] max maximum value of parameter in the same unit than `q`
      @exception bad_alloc if there is enough memory
  */
  CorrelationPar(const string & name, const BaseQuantity q,
		 const string & desc, double min, double max)
    : CorrelationPar(name, q.unit, desc, 
		     VtlQuantity(q.unit, min), VtlQuantity(q.unit, max)) {}

  /// Return `true` if value `val` is valid according to specified
  /// parameter range. Conversion is done if `val` comes in another
  /// unit than `this->unit`
  bool check(const BaseQuantity & val) const noexcept
  {
    if (&val.unit == &unit)
      return val.raw() >= min_val.raw() - epsilon and
	val.raw() <= max_val.raw() + epsilon;

    VtlQuantity real_val(unit, val); // convert to unit for val.unit
    return real_val.raw() >= min_val.raw() - epsilon and
	real_val.raw() <= max_val.raw() + epsilon;
  }

  /// Return `true` if value `q` is valid according to specified
  /// parameter range. It is assumed than `q` unit is `this->unit`
  bool check(double val) const { return check(VtlQuantity(unit, val)); }

  /// Validate and eventually throw exception is parameter value `q`
  /// is not inside specified parameter range.
  void verify(const BaseQuantity & q) const
  {
    ostringstream s;
    if (q.get_value() < min_val.raw() - epsilon)
      {
	s << "Value " << q.get_value() << " is lesser than the minimum value "
	  << min_val << " for precondition ";
	ALEPHTHROW(OutOfParameterRange, s.str());
      }

    if (q.get_value() > max_val.raw() + epsilon)
      {
	s << "Value " << q << " is greater than the maximum value "
	  << max_val << " for precondition ";
	ALEPHTHROW(OutOfParameterRange, s.str());
      }
  }
};


/** Define a correlation.

    Correlation class is the base and main object used for defining a
    correlation. Although this class manages, contains and represents
    the most part of a correlation, this is not intended to be used as
    the final correlation. 

    A correlation correlation has a type and a subtype which define
    families of correlations. For example, correlations related to oil
    are of type `OilCorrelation`. In a similar way, oil correlations
    related to compute the bubble point are of subtype
    `BubblePointPressure`.

    @see Declare_Correlation_Type, Declare_Correlation_Subtype

    @ingroup Correlations
    @author Leandro Rabindranath Leon
 */
struct Correlation
{
  const string type_name; 
  const string subtype_name;
  const string name = "NOT-DEFINED"; // Correlation name
  mutable string latex_symbol = "NOT-DEFINED";
  mutable string author = "NOT-DEFINED";
  const Unit & unit; /// Return unit 
  const double min_val; /// minimum value that can reach expressed in `unit`
  const double max_val; /// maximum value that can reach expressed in `unit`
  mutable bool min_from_author = false; /// 'true' if author specified minimum
  mutable bool max_from_author = false; /// 'true' if author specified maximum
  mutable string title; /// Title as it would be shown is a front-end
  mutable DynList<string> db; /// String describing the source of data
  mutable DynList<string> notes; /// Other related notes
  mutable DynList<const BibEntry*> refs; /// List of references
  mutable bool hidden = false; // indicates that correlation must be
			       // competely hidden at all front-end level

  /// Indicates that correlation must not be shown in front-end calculator
  mutable bool hidden_calc = false;
  
  /// Indicates if correlation must be hidden in grid generation for blackoil
  mutable bool hidden_blackoil_grid = false; 

  /// Indicates if correlation must be hidden in grid generation for drygas
  mutable bool hidden_drygas_grid = false;

  /// Indicates if correlation must be hidden in grid generation for drygas
  mutable bool hidden_wetgas_grid = false;

  /// Unique identifier generated in run-time
  mutable long id = -1;

  /// String indicating the property target name
  mutable string __target_name = "NOT-DEFINED";

  /// List of parameter names
  mutable DynSetTree<string> par_names;

private:

  // List of parameter
  DynList<CorrelationPar> preconditions;
  size_t n = 0; // size of preconditions list

  static size_t counter; // counter used for generating unique ids
  static DynMapTree<string, const Correlation *> tbl;

  // Every correlation is stored here
  static Array<const Correlation *> correlations_tbl;

  // helper function that extract first symbols until finding the
  // second upper case symbol
  void set_target_name() const
  {
    __target_name = to_lower(split_camel_case(name).get_first());
  }

public:

  /// Returns the correlation target name
  const string & target_name() const { return __target_name; }

  /// Return author name
  string author_name() const
  {
    auto pos = name.find(::to_name(__target_name));
    return name.substr(pos + __target_name.size());
  }

  /// Returns the parameters names in the declaration order
  DynList<string> parameters_signature() const
  {
    return
      preconditions.maps<string>([] (const auto & par) { return par.name; });
  }

  /// Returns the names of parameters involved in the correlation. The
  /// first one is the return value. The remainder others are the
  /// parameter names
  DynList<string> names() const
  {
    DynList<string> ret = unitarian_DynList(target_name());
    for (auto it = preconditions.get_it(); it.has_curr(); it.next())
      ret.append(it.get_curr().name);
    return ret;
  }

  /// Return a list to correlation parameter names
  const DynSetTree<string> & parameter_names() const
  {
    return par_names;
  }

  /// Return a list of parameter synonyms along with their units
  DynList<CorrelationPar::P> synonyms() const
  {
    DynList<CorrelationPar::P> ret;
    for (auto it = preconditions.get_it(); it.has_curr(); it.next())
      ret.append(it.get_curr().get_synonyms());

    return ret;
  }

  /// Returns a list with all the possible correlation signatures; that
  /// is all the valid permutations through the parameters names and
  /// their synonyms
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

  /// Search `name` in the correlation parameters. If found, then it
  /// returns a pointer to the `CorrelationPar` object defining the
  /// parameter. Otherwise, it returns `nullptr`. Parameter synonyms
  /// are considered in this search
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

  // helper function for searching a parameter according to its name
  CorrelationPar * search_parameter(const string & par_name) const
  {
    auto par_ptr = preconditions.find_ptr([&par_name] (auto p)
					  { return p.name == par_name; });
    if (par_ptr == nullptr)
      {
	ostringstream s;
	s << "Correlation::search_parameter(): par name " << par_name
	  << " not found";
	ALEPHTHROW(SynonymNotFound, s.str());
      }

    return const_cast<CorrelationPar*>(par_ptr);
  }

public:

  /// Return a list of all the LaTeX symbols. Each entry is a pair
  /// with the parameter name and its corresponding LaTeX symbol
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

  /** Add a parameter alias.

      @param[in] par_name name of paramater
      @param[in] name of parameter alias
      @param[in] unit_name name of the unit for the alias
 */
  void add_par_synonym(const string & par_name, const string & synonym,
		       const string & unit_name)
  {
    auto par_ptr = search_parameter(par_name);
    par_ptr->add_synonym(synonym, unit_name);
    par_names.append(synonym);
  }

  /// Defines `latex_symbol` as LaTeX symbol of parameter `par_name`
  void def_parameter_latex_symbol(const string & par_name,
				  const string & latex_symbol)
  {
    auto par_ptr = search_parameter(par_name);
    par_ptr->def_latex_symbol(latex_symbol);
  }

  /// Enable all hiding parameter for the correlation. The correlation
  /// will not be shown at any front-end level
  void set_hidden()
  {
    hidden = hidden_calc = hidden_blackoil_grid = hidden_drygas_grid =
      hidden_wetgas_grid = true;
  }

  /// Turn on hidden flag for the calculator front-end
  void set_hidden_calc() { hidden_calc = true; }

  /// Turn on hidden flag for the blackoil grid front-end
  void set_hidden_blackoil_grid() { hidden_blackoil_grid = true; }

  /// Turn on hidden flag for the drygas grid front-end
  void set_hidden_drygas_grid() { hidden_drygas_grid = true; }

  /// Turn on hidden flag for the wetgas grid front-end
  void set_hidden_wetgas_grid() { hidden_wetgas_grid = true; }

  /// Generates a json describing all the registered correlations
  static string json_of_all_correlations();

  /// Generate a json describing this correlation
  string to_json() const;

  /// Generate a json with all the correlation belonging to
  /// `subtype_name` correlation family
  static string to_json(const string & subtype_name);

  /// Set the correlation author name
  void set_author(const string & __author) { author = __author; }

  /// Set the correlation title
  void set_title(const string & __title) { title = __title; }

  /// Add a note to data bank list
  void add_db(const string & __db) { db.append(__db); }

  /// Add a note 
  void add_note(const string & note) { notes.append(note); }

  /// Add a reference for the correlation. `tag` parameter is the
  /// BibTeX tag
  void add_ref(const string & tag)
  {
    refs.append(BibEntry::find(tag));
  }

  /// Return a reference to the list of correlation parameters sorted by pass order
  const DynList<CorrelationPar> & get_preconditions() const noexcept
  {
    return preconditions;
  }

  /// Return a list of pointers to correlation parameters sorted by pass order
  const DynList<const CorrelationPar * const> preconditions_list() const 
  {
    return preconditions.maps<const CorrelationPar * const>([] (const auto & par)
      {
	return &par;
      });
  }

  /// Returns the signature position of parameter `par_name`. If not
  /// found return a position greater than the number of parameters
  size_t parameter_pos(const string & par_name) const
  {
    return preconditions.find_index([&par_name] (const auto & par)
				    {
				      return par.name == par_name;
				    });
  }

  /// Return a list with the units of correlation parameters
  DynList<const Unit *> get_par_types() const
  {
    return preconditions.maps<const Unit *>([] (const auto & par)
						 { return &par.unit; });
  }

  /// Return the number of parameters
  size_t get_num_pars() const noexcept { return n; }

  /// Return a string with the correlation author, type, subtype a the name
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

  /// Return a string describing the python call. This implies that
  /// the correlation has been defined in python with exactly the same
  /// parameters.
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

  /** Given a parameter list `vals', it builds a string with the
      signature call. Some such as 

          PbAlMarhoun(0.8 Sgg, 0.9 sg_do, 100 scf/STB, 550 degR)

      @note The unit types and the number of parameter are not validated
      
      @param[in] vals a list of VtlQuantity with parameter names and
      their values
 */
  string call_string(const DynList<VtlQuantity> & vals) const
  {
    return name + "(" + join(vals, ", ") + ")";
  }

  /// Takes a list of doubles `vals`, matches with he preconditions units and
  /// build a signature call
  string call_string(const DynList<double> & vals) const
  {
    return call_string(zip_maps<VtlQuantity>([] (auto t)
      {
	return VtlQuantity(get<0>(t).unit, get<1>(t));
      }, preconditions, vals));
  }

  /// Return a string with the parameter list between
  /// parentheses. Each parameter is composed by its name and unit
  string call_string() const
  {
    return "(" +
      join(preconditions.maps<string>([] (auto & p) { return p.name; })) + ")";
  }

  /// Return a list of all registered correlations
  static DynList<const Correlation *> list() { return tbl.values(); }

  /// Return an array of all registered correlations
  static const Array<const Correlation*> & array() noexcept
  {
    return correlations_tbl;
  }

  /// Return the number of registered correlations
  static size_t num_correlations() noexcept { return correlations_tbl.size(); }

  /** Search a correlation given is name.

      @param[in] name correlation name
      @return a pointer to a `Correlation` object if the correlation
      was found. Otherwise, it returns `nullptr`
 */
  static const Correlation * search_by_name(const string & name)
  {
    auto ptr = tbl.search(name);
    return ptr != nullptr ? ptr->second : nullptr;
  }

  /// Return a list with all registered correlation types
  static DynList<string> type_list()
  {
    DynSetTree<string> s;
    for (auto it = tbl.get_it(); it.has_curr(); it.next())
      s.insert(it.get_curr().second->type_name);

    return s.keys();
  }

  /// Return a list with all registered correlation subtypes
  /// associated to type `type_name`
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

  /// Return a list of `Correlation` pointers associated to subtype
  /// `subtype_name` 
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
	      const Unit & result_unit, double min_val, double max_val,
	      bool min_from_author, bool  max_from_author)
    : type_name(type_name), subtype_name(subtype_name), name(name),
      latex_symbol(latex_symbol), unit(result_unit),
      min_val(min_val), max_val(max_val),
      min_from_author(min_from_author), max_from_author(max_from_author)
  {
    if (tbl.has(name))
      {
	ostringstream s;
	s << "Correlation::Correlation: duplicated correlation name" << name;
	ALEPHTHROW(DuplicatedCorrelationName, s.str());
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
		  result_unit.min_val, result_unit.max_val, false, false) {}

  Correlation(const string & type_name, const string & subtype_name,
	      const string & name, const string & latex_symbol,
	      const Unit & result_unit, double min_val)
    : Correlation(type_name, subtype_name, name, latex_symbol,
		  result_unit, min_val, result_unit.max_val, true, false)
  {
    min_from_author = true;
  }

  virtual ~Correlation() = 0; // Correlation is an abstract class

  /** Register a correlation parameter.

      The order of parameter is determined by the order of insertion
      through this method.

      @param[in] name of parameter
      @param[in] unit reference to unit
      @param[in] description string with parameter description
      @param[in] min minimum value with its unit
      @param[in] max maximum value with its unit
 */
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
	ALEPHTHROW(OutOfParameterRange, s.str());
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

  /// Insert a new parameter whose minimum and maximum values are the
  /// unit limits
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

  /// Return a string describing the correlation
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

  /// Return correlation signature in a dynlist
  DynList<string> to_dynlist() const
  {
    DynList<string> ret = { name };
    ret.append(preconditions.maps<string>([] (auto & p) { return p.to_string(); }));
    return ret;
  }

  /// Generate a Ruby Correlation class
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

  /// Generate a ruby specification for all the registered correlations
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

  /** Print out a full description for the correlation.
  
      @param[in] width used for justifying notes and data bank
      @param[in] left_margin 
   */
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

  /** Compute the correlation value given its parameters.

      The `compute()` method which is virtual, dynamically determines
      the correlation implementation and automatically dispatches it
      the received parameters.

      This method receives a list `pars` of `VtlQuantity` objects
      containing the parameters and their units. The order of
      parameter list must exactly match with the specified and
      expected order in the correlation implementation.

      The parameter `check` specifies whether the parameters bounds
      are or not verified. If `check` is `true`, then each parameter
      range is check respect a the correlation
      specification. Otherwise no one check is done and the
      correlation is computed without any previous checking.

      This method does not check whether the return value is or not
      bound to any specified interval. If you want to do get that,
      then use `compute_and_check()` method instead of this method. 

      Although this method could perfectly be used in a real
      production system, it is not intended to use by the majority of
      back ends and front ends. Its mission is to provide a direct way
      to find the implementation method and execute it.

      @param[in] pars a list of `VtlQuantity` objects containing the
      correlation parameters in the same order.
      @param[in] check indicates whether the parameter bounds must be
      or not checked. By default, this parameter has the value `true`.
      @return A VtlQuantity object corresponding to the computed value
      of the correlation.
      @exception bad_alloc if there is not enough memory.
      @exception OutOfParameterRange if any parameter is out of its
      range.

      Brief example of use
      --------------------

      Let's suppose that you want to use the PbAlMarhoun correlation
      for computing the bubble point. This correlation has the
      following signature:

          Quantity<psia> call(const Quantity<Sgg>& yg,
                              const Quantity<Sg_do>& yo,
                              const Quantity<SCF_STB>& rsb,
                              const Quantity<Rankine>& t) const

      The first thing that we must do is to get a pointer to the
      correlation. There are several ways for getting it, perhaps the
      simplest is through the search method:

          Correlation corr_ptr = search_by_name("PbAlMarhoun");

      Now, you could directly call it as follows:

          auto ret = corr_ptr->compute({Quantity<Sgg>(.8), Quantity<Sg_do>(.9), 
	                                Quantity<SCF_STB>(28), 
					Quantity<Rankine>(560)});

      Since the `DynList` type has a constructor receiving the
      standard type `std::initializer_list`, the parameter list is
      specified between `{}` and the order of of the list corresponds
      with the order of implementation function (`yg`, `yo`, `rsb` and
      `t`). Since `check` is not given, the `true` value is assumed;
      therefore, all the parameters bounds are checked.

      @see compute_and_check execute compute_by_names
   */
  virtual VtlQuantity
  compute(const DynList<VtlQuantity> & pars, bool check = true) const = 0;

  /** Meta-wrapper to `compute()` method.

      This flavor of `compute()` automatically builds the parameter
      list according to the passing order. 

      Example of use
      --------------

      Given a pointer `corr_ptr` to the PbAlMarhoun correlation, you
      could directly compute it thus:

          auto ret = corr_ptr->compute(true, .8, .9, 28, 560);

      In this variant the parameter `check` comes first. This is an
      essential condition to be able of generate the meta call. Next,
      the parameters come exactly in the same specification order
      ((`yg`, `yo`, `rsb` and `t`). 

      @param[in] check indicates whether the parameter bounds must be
      or not checked.
      @param[in] args correlation parameters separated with comma in
      the same specification order.
      @exception bad_alloc if there is not enough memory.
      @exception OutOfParameterRange if any parameter is out of its
      range.
   */
  template <typename ... Args>
  VtlQuantity compute(bool check, Args ... args) const
  {
    DynList<VtlQuantity> pars_list;
    append_in_container(pars_list, args ...);
    return compute(pars_list, check);
  }

  /** Compute the correlation and check if the return satisfies the
      result bounds.

      This function is similar to `compute()` except for the fact that
      the return value is validated.

      @param[in] pars a list of `VtlQuantity` objects containing the
      correlation parameters in the same order.
      @param[in] check indicates whether the parameter bounds must be
      or not checked. By default, this parameter has the value `true`.
      @return A VtlQuantity object corresponding to the computed value
      of the correlation.
      @exception bad_alloc if there is not enough memory.
      @exception OutOfParameterRange if any parameter is out of its
      range.
      @exception OutOfCorrelationRange if the return value is not
      inside of the result interval.
   */
  VtlQuantity compute_and_check(const DynList<VtlQuantity> & pars,
				bool check = true) const
  {
    return verify_result(compute(pars, check));
  }

  /** Not throwing version of `compute()`.

      Regarding the received parameters, this function is equivalent
      to `compute()`. The big difference is that this method does not
      throw any exception. Instead, this method returns a quadruple
      whose structure is as follows:

      1. `get<0>(result)`: a double containing the result. This value
      is valid if there was no error.

      2. `get<1>(result)`: a string with the name of unit result.

      3. `get<2>(result)`: a boolean value indicating if the
      correlation was correctly computed. `true` value indicates that
      everything was okay. Otherwise an error occurred and the `false`
      value is set.

      4. `get<3>(result)`: if an error was detected, then this filed
      contains a string describing the error. Otherwise the string is empty.

      @param[in] pars a list of `VtlQuantity` objects containing the
      correlation parameters in the same order.
      @param[in] check indicates whether the parameter bounds must be
      or not checked. By default, this parameter has the value `true`.
      @return a `tuple<double, string, bool, string>` containing the
      all the necessary information about the operation.
   */
  tuple<double, string, bool, string> execute(DynList<VtlQuantity> & pars,
					      bool check = true) const noexcept
  {
    try
      {
	const auto result = compute(pars, check);
	auto status = check_result(VtlQuantity(unit, result));
	if (status)
	  return make_tuple(result.raw(), unit.name, true, "");

	ostringstream s;
	s << "Correlation result " << result << " is out of range ["
	  << min_val << ", " << max_val << "]";
	OutOfCorrelationRange e(__LINE__, __FILE__, "OutOfCorrelationRange",
				s.str().c_str());
	return make_tuple(result.raw(), unit.name, false, e.what());
      }
    catch (exception &e)
      {
	return make_tuple(0, unit.name, false, e.what());
      }
  }

  /** Compute the correlation receiving a list of doubles, without
      their units.

      This method is similar to `compute()`, excepts that it receive
      their parameters as doubles, without units. In this case, it is
      assumed that the parameter are expressed in the same units for
      which the correlation parameters were specified.
      
      Example of use
      --------------

      Given a pointer `corr_ptr` to the PbAlMarhoun correlation, you
      could directly compute it thus:

          auto ret = corr_ptr->compute({.8, .9, 28, 560});

      @param[in] values list of doubles expressing the correlation
      parameters separated with comma in the same specification order.
      @param[in] check indicates whether the parameter bounds must be
      or not checked.
      @return a double value expressing the result in the
      implementation unit.
      @exception bad_alloc if there is not enough memory.
      @exception OutOfParameterRange if any parameter is out of its
      range.
      @exception InvalidNumberOfParameters if the size of parameter
      list is less than the number of expected parameters.
   */
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
	ALEPHTHROW(InvalidNumberOfParameters, s.str());
      }

    VtlQuantity ret = { unit, compute(pars, check) }; // here conversion is done
    return ret.get_value();
  }

  /** Compute the correlation receiving a list of doubles, without
      their units and check if the return satisfies the specified
      result bounds.

      This function is similar to `compute()` except for the fact that
      the return value is validated.

      @param[in] pars a list of `VtlQuantity` objects containing the
      correlation parameters in the same order.
      @param[in] check indicates whether the parameter bounds must be
      or not checked. By default, this parameter has the value `true`.
      @return A VtlQuantity object corresponding to the computed value
      of the correlation.
      @exception bad_alloc if there is not enough memory.
      @exception OutOfParameterRange if any parameter is out of its
      range.
      @exception OutOfCorrelationRange if the return value is not
      inside of the result interval.
      @exception InvalidNumberOfParameters if the size of parameter
      list is less than the number of expected parameters.
   */
  double
  compute_and_check(const DynList<double> & pars, bool check = true) const
  {
    return verify_result(VtlQuantity(unit, compute(pars, check))).raw();
  }

  /// Simple parameter by name without unit
  using ParByName = pair<string, double>;

  /** Compute the correlation by receiving an unsorted list of pairs
      par-name,value.

      In this method the parameters list could be in any order. They
      are identified by their names.

      Note that in this version the units are not considered. It is
      assumed that every parameter is specified in the same unit in
      which this was specified.      

      @param[in] pair_list a list of pairs everyone containing the
      parameter value and its name.
      @param[in]check indicates whether the parameter bounds must be
      or not checked. By default, this parameter has the value `true`.
      @return A VtlQuantity object corresponding to the computed value
      of the correlation.
      @exception bad_alloc if there is not enough memory.
      @exception OutOfParameterRange if any parameter is out of its
      range.
      @exception ParameterNameNotFound if a parameter name does not
      match with the specified name.
   */
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
	    ALEPHTHROW(ParameterNameNotFound, s.str());
	  }
	vals.append(VtlQuantity(par.unit, ptr_val->second));
      }

    return compute(vals, check);
  }

  using NamedPar = ::NamedPar;

  /** Build a `NamedPar` object.

      @param[in] name the parameter name.
      @param[in] val value of parameter.
      @param[in] uptr pointer to the unit which the parameter value is
      represented.
      @return a NamedPar object representing the parameter
   */
  static NamedPar
  build_names_par(const string & name, double val, const Unit * uptr)
  {
    return NamedPar(true, name, val, uptr);
  }

  /** Compute the correlation by receiving an unsorted list of `NamedPar`
      objects. 

      In this method the parameters list could be in any order. They
      are identified by their names.

      If any parameter `par` has the field `get<0>(par)` in `false`,
      then the calculations is not performed and the exception
      `ParameterNameNotSet` is thrown.

      @param[in] pair_list a list of pairs everyone containing the
      parameters.
      @param[in] check indicates whether the parameter bounds must be
      or not checked. By default, this parameter has the value `true`.
      @return A VtlQuantity object corresponding to the computed value
      of the correlation.
      @exception bad_alloc if there is not enough memory.
      @exception OutOfParameterRange if any parameter is out of its
      range.
      @exception ParameterNameNotFound if a parameter name does not
      match with the specified name.
      @exception ParameterNameNotSet if any parameter has the field
      `get<0>(par)` in `false`. 
   */  
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
	    ALEPHTHROW(ParameterNameNotFound, s.str());
	  }
	
	if (not get<0>(*ptr_val))
	  {
	    ostringstream s;
	    s << "compute_by_names() for correlation " << name
	      << ": parameter name " << par.name << " is not set";
	    ALEPHTHROW(ParameterNameNotSet, s.str());
	  }
	vals.append(VtlQuantity(*get<3>(*ptr_val), get<2>(*ptr_val)));
      }

    return compute(vals, check);
  }

  /** Compute the correlation by receiving a `ParList` object.

      This is the fastest way for computing a correlation using
      parameters by name. 

      The `ParList` object must contain the parameters of type
      `ParPair`. A `ParPair` is defined as a `pair<string, ValPair>`
      where the first field corresponds to the parameter name and the
      second field is another pair defined as `pair<double, const
      Unit*>`, where the first field is the parameter value and the
      second field is the parameter unit.

      @param[in] pair_list a `ParList` object containing the parameters.
      @param[in] check indicates whether the parameter bounds must be
      or not checked. By default, this parameter has the value `true`.
      @return A VtlQuantity object corresponding to the computed value
      of the correlation.
      @exception bad_alloc if there is not enough memory.
      @exception OutOfParameterRange if any parameter is out of its
      range.
      @exception ParameterNameNotFound if a parameter name does not
      match with the specified name.

      @see ParList
   */
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

  /** Meta-wrapper for building a call to 'compute()" which passes
      correlation parameters by name and uses a `ParList` object.

      Since this method receives a `ParList` object, you could 
      avoid the costs of its construction and destruction by previously
      allocating it. 

      The method first inserts the parameters, next it invokes the
      correlation, and finally it removes the parameters from the
      `ParList` object even if a exception is thrown.

      Example of use.
      --------------

      Let's suppose the PbAlMarhoun correlation pointed by `corr_ptr`
      pointer. You could compute it as follows:

          corr_ptr->compute_by_names(true, parlist, NPAR(t), NPAR(yg),
                                     NPAR(rsb), NPAR(yo));

      In this case, the helper macro `NPAR` is used which builds each
      parameter with name homonym to the variable name. For example,
      `NPAR(yo)' build an parameter with name "yo" and the value
      stored in a `VtlQuantity` object `yo`. Please note that since
      units are required, you must use `VtlQuantity` or `Quantity`
      objects. 

      @param[in] check indicates whether the parameter bounds must be
      or not checked.
      @param[in] par_list `ParList` object where the parameters will
      be passed.
      @param[in] args parameters of type `ParPair`. You could pass as
      many as you require.

      @see par, npar, NPAR 
   */
  template <typename ... Args>
  VtlQuantity
  compute_by_names(bool check, ParList & par_list, Args & ...  args) const
  {
    insert_in_container(par_list, args...);
    try
      {
	VtlQuantity ret = compute_by_names(par_list, check);
	remove_from_container(par_list, args...);
	return ret;
      }
    catch (...)
      {
	remove_from_container(par_list, args...);
	throw ;
      }
  }

  /** Helper function for adjusting a correlation computed value
     according to a linear tuning.

     @param[in] val a `VtlQuantity` representing the correlation
     computed result.  
     @param[in] c the tuning c parameter.
     @param[in] m the tuning m parameter.
     @param[in] tuned_unit a pointer to the unit that was used when
     the tuning was done.
     @exception UnitConversionNotFound if `tuned_unit` is not a
     sibling unit of `val.unit`.
     @exception OutOfUnitRange if the tuned value is out of some of
     the involved units. 
   */
  static VtlQuantity tune(const VtlQuantity & val,
			  double c, double m, const Unit & tuned_unit)
  {
    const double r =
      bind_to_unit_limits(c + m*VtlQuantity(tuned_unit, val).raw(), tuned_unit);
    return VtlQuantity(val.unit, VtlQuantity(tuned_unit, r));    
  }

  /** Compute the correlation receiving a list of `VtlQuantity`s,
      check if the return value satisfies the specified result bounds,
      and then applies a linear tuning.

      @param[in] pars a list of `VtlQuantity` objects containing the
      correlation parameters in the same order.
      @param[in] c the tuning c parameter.
      @param[in] m the tuning m parameter.
      @param[in] tuned_unit the unit used when the tuning was done.
      @param[in] check indicates whether the parameter bounds must be
      or not checked. By default, this parameter has the value `true`.
      @return A VtlQuantity object corresponding to the computed value
      of the correlation.
      @exception bad_alloc if there is not enough memory.
      @exception OutOfParameterRange if any parameter is out of its
      range.
      @exception OutOfCorrelationRange if the return value is not
      inside of the result interval.
      @exception InvalidNumberOfParameters if the size of parameter
      list is less than the number of expected parameters.
   */
  VtlQuantity tuned_compute_and_check(const DynList<VtlQuantity> & pars,
				      double c, double m,
				      const Unit & tuned_unit,
				      bool check = true) const
  {
    return tune(compute(pars, check), c, m, tuned_unit);
  }

  /** Not throwing and tuned version of `compute()`.

      Regarding the received parameters, this function is equivalent
      to `compute()`. The big difference is that this method does not
      throw any exception. Instead, this method returns a quadruple
      whose structure is as follows:

      1. `get<0>(result)`: a double containing the result. This value
      is valid if there was no error.

      2. `get<1>(result)`: a string with the name of unit result.

      3. `get<2>(result)`: a boolean value indicating if the
      correlation was correctly computed. `true` value indicates that
      everything was okay. Otherwise an error occurred and the `false`
      value is set.

      4. `get<3>(result)`: if an error was detected, then this filed
      contains a string describing the error. Otherwise the string is empty.

      @param[in] pars a list of `VtlQuantity` objects containing the
      correlation parameters in the same order.
      @param[in] c the tuning c parameter.
      @param[in] m the tuning m parameter.
      @param[in] tuned_unit the unit used when the tuning was done.
      @param[in] check indicates whether the parameter bounds must be
      or not checked. By default, this parameter has the value `true`.
      @return a `tuple<double, string, bool, string>` containing the
      all the necessary information about the operation.
   */
  tuple<double, string, bool, string>
  tuned_execute(DynList<VtlQuantity> & pars, double c, double m,
		const Unit & tuned_unit, bool check = true) const
  {
    VtlQuantity r = VtlQuantity(tuned_unit, c + m*compute(pars, check).raw());
    return make_tuple(r.raw(), unit.name, true, "");
  }

  /** Compute the correlation receiving a list of doubles, without
      their units, and applying a linear tuning.

      @param[in] values list of doubles expressing the correlation
      parameters separated with comma in the same specification order.
      @param[in] c the tuning c parameter.
      @param[in] m the tuning m parameter.
      @param[in] tuned_unit the unit used when the tuning was done.
      @param[in] check indicates whether the parameter bounds must be
      or not checked.
      @return a double value expressing the result in the
      implementation unit.
      @exception bad_alloc if there is not enough memory.
      @exception OutOfParameterRange if any parameter is out of its
      range.
      @exception InvalidNumberOfParameters if the size of parameter
      list is less than the number of expected parameters.
   */
  double tuned_compute(const DynList<double> & values, double c, double m,
		       const Unit & tuned_unit, bool check = true) const
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
	ALEPHTHROW(InvalidNumberOfParameters, s.str());
      }

    VtlQuantity ret = tune(compute(pars, check), c, m, tuned_unit);
    return ret.raw();
  }

  /** Compute a correlation by receiving an unsorted list of pairs
      par-name,value and applies a linear tuning.

      In this method the parameters list could be in any order. They
      are identified by their names.

      Note that in this version the units are not considered. It is
      assumed that every parameter is specified in the same unit in
      which this was specified.      

      @param[in] pair_list a list of pairs everyone containing the
      parameter value and its name.
      @param[in] c the tuning c parameter.
      @param[in] m the tuning m parameter.
      @param[in] tuned_unit the unit used when the tuning was done.
      @param[in]check indicates whether the parameter bounds must be
      or not checked. By default, this parameter has the value `true`.
      @return A VtlQuantity object corresponding to the computed value
      of the correlation.
      @exception bad_alloc if there is not enough memory.
      @exception OutOfParameterRange if any parameter is out of its
      range.
      @exception ParameterNameNotFound if a parameter name does not
      match with the specified name.
    */  
  VtlQuantity
  tuned_compute_by_names(const DynList<ParByName> & pair_list,
			 double c, double m, const Unit & tuned_unit,
			 bool check = true) const
  {
    return tune(compute_by_names(pair_list, check), c, m, tuned_unit);
  }

  /** Compute a correlation by receiving an unsorted list of `NamedPar`
      objects and applies a linear tuning.

      In this method the parameters list could be in any order. They
      are identified by their names.

      If any parameter `par` has the field `get<0>(par)` in `false`,
      then the calculations is not performed and the exception
      `ParameterNameNotSet` is thrown.

      @param[in] pair_list a list of pairs everyone containing the
      parameters.
      @param[in] c the tuning c parameter.
      @param[in] m the tuning m parameter.
      @param[in] tuned_unit the unit used when the tuning was done.
      @param[in] check indicates whether the parameter bounds must be
      or not checked. By default, this parameter has the value `true`.
      @return A VtlQuantity object corresponding to the computed value
      of the correlation.
      @exception bad_alloc if there is not enough memory.
      @exception OutOfParameterRange if any parameter is out of its
      range.
      @exception ParameterNameNotFound if a parameter name does not
      match with the specified name.
      @exception ParameterNameNotSet if any parameter has the field
      `get<0>(par)` in `false`. 
   */
  VtlQuantity
  tuned_compute_by_names(const DynList<NamedPar> & pair_list,
			 double c, double m, const Unit & tuned_unit,
			 bool check = true) const
  {
    return tune(compute_by_names(pair_list, check), c, m, tuned_unit);
  }

  /** Compute the correlation by receiving a `ParList` object, and then
      applies a linear tuning.

      This is the fastest way for computing a correlation using
      parameters by name. 

      The `ParList` object must contain the parameters of type
      `ParPair`. A `ParPair` is defined as a `pair<string, ValPair>`
      where the first field corresponds to the parameter name and the
      second field is another pair defined as `pair<double, const
      Unit*>`, where the first field is the parameter value and the
      second field is the parameter unit.

      @param[in] pair_list a `ParList` object containing the
      parameters.
      @param[in] c the tuning c parameter.
      @param[in] m the tuning m parameter.
      @param[in] tuned_unit the unit used when the tuning was done.
      @param[in] check indicates whether the parameter bounds must be
      or not checked. By default, this parameter has the value `true`.
      @return A VtlQuantity object corresponding to the computed value
      of the correlation.
      @exception bad_alloc if there is not enough memory.
      @exception OutOfParameterRange if any parameter is out of its
      range.
      @exception ParameterNameNotFound if a parameter name does not
      match with the specified name.

      @see ParList
   */
  VtlQuantity
  tuned_compute_by_names(const ParList & par_list,
			 double c, double m, const Unit & tuned_unit,
			 bool check = true) const
  {
    return tune(compute_by_names(par_list, check), c, m, tuned_unit);
  }

  /** Compute the correlation by receiving an unsorted list of pairs
      par-name,value, applies a linear tuning and check the result for
      specified bounds.

      First this method performs everything what
      `tuned_compute_by_names()` does and then check the result
      against an interval. If the result is out of interval, then,
      instead of throwing exception, the result is rounded to the
      nearest end of interval.

      In this method the parameters list could be in any order. They
      are identified by their names.

      Note that in this version the units are not considered. It is
      assumed that every parameter is specified in the same unit in
      which this was specified.      

      @param[in] pair_list a list of pairs everyone containing the
      parameter value and its name.
      @param[in] min_val minimum value of interval.
      @param[in] max_val maximum value of interval.
      @param[in] c the tuning c parameter.
      @param[in] m the tuning m parameter.
      @param[in] tuned_unit the unit used when the tuning was done.
      @param[in] check indicates whether the parameter bounds must be
      or not checked. By default, this parameter has the value `true`.
      @return A VtlQuantity object corresponding to the computed value
      of the correlation.
      @exception bad_alloc if there is not enough memory.
      @exception OutOfParameterRange if any parameter is out of its
      range.
      @exception ParameterNameNotFound if a parameter name does not
      match with the specified name.
   */
  VtlQuantity
  bounded_tuned_compute_by_names(const DynList<ParByName> & par_list,
				 const VtlQuantity & min_val,
				 const VtlQuantity & max_val,
				 double c, double m, const Unit & tuned_unit,
				 bool check = true) const
  {
    const double result =
      tune(compute_by_names(par_list, check), c, m, tuned_unit).raw();
    if (result < min_val)
      return min_val;
    if (result > max_val)
      return max_val;
    return VtlQuantity(unit, result);
  }

  /** Compute the correlation by receiving an unsorted list of `NamedPar`
      objects, applies a linear tuning, and check the result for specified
      bounds. 

      First this method performs everything what
      `tuned_compute_by_names()` does and then check the result
      against an interval. If the result is out of interval, then,
      instead of throwing exception, the result is rounded to the
      nearest end of interval.

      In this method the parameters list could be in any order. They
      are identified by their names.

      If any parameter `par` has the field `get<0>(par)` in `false`,
      then the calculations is not performed and the exception
      `ParameterNameNotSet` is thrown.

      @param[in] pair_list a list of pairs everyone containing the
      parameters.
      @param[in] min_val minimum value of interval.
      @param[in] max_val maximum value of interval.
      @param[in] c the tuning c parameter.
      @param[in] m the tuning m parameter.
      @param[in] tuned_unit the unit used when the tuning was done.
      @param[in] check indicates whether the parameter bounds must be
      or not checked. By default, this parameter has the value `true`.
      @return A VtlQuantity object corresponding to the computed value
      of the correlation.
      @exception bad_alloc if there is not enough memory.
      @exception OutOfParameterRange if any parameter is out of its
      range.
      @exception ParameterNameNotFound if a parameter name does not
      match with the specified name.
      @exception ParameterNameNotSet if any parameter has the field
      `get<0>(par)` in `false`. 
   */
  VtlQuantity
  bounded_tuned_compute_by_names(const DynList<NamedPar> & par_list,
				 const VtlQuantity & min_val,
				 const VtlQuantity & max_val,
				 double c, double m, const Unit & tuned_unit,
				 bool check = true) const
  {
    const double result =
      tune(compute_by_names(par_list, check), c, m, tuned_unit).raw();
     if (result < min_val)
      return min_val;
    if (result > max_val)
      return max_val;
    return VtlQuantity(unit, result);
  }

  /** Compute the correlation by receiving a `ParList` object, applies
      a linear tuning, and check the result for specified bounds.

      First this method performs everything what
      `tuned_compute_by_names()` does and then check the result
      against an interval. If the result is out of interval, then,
      instead of throwing exception, the result is rounded to the
      nearest end of interval.

      The `ParList` object must contain the parameters of type
      `ParPair`. A `ParPair` is defined as a `pair<string, ValPair>`
      where the first field corresponds to the parameter name and the
      second field is another pair defined as `pair<double, const
      Unit*>`, where the first field is the parameter value and the
      second field is the parameter unit.

      @param[in] pair_list a `ParList` object containing the
      parameters.
      @param[in] min_val minimum value of interval.
      @param[in] max_val maximum value of interval.
      @param[in] c the tuning c parameter.
      @param[in] m the tuning m parameter.
      @param[in] tuned_unit the unit used when the tuning was done.
      @param[in] check indicates whether the parameter bounds must be
      or not checked. By default, this parameter has the value `true`.
      @return A VtlQuantity object corresponding to the computed value
      of the correlation.
      @exception bad_alloc if there is not enough memory.
      @exception OutOfParameterRange if any parameter is out of its
      range.
      @exception ParameterNameNotFound if a parameter name does not
      match with the specified name.

      @see ParList
   */
  VtlQuantity
  bounded_tuned_compute_by_names(const ParList & par_list,
				 const VtlQuantity & min_val,
				 const VtlQuantity & max_val,
				 double c, double m, const Unit & tuned_unit,
				 bool check = true) const
  {
    const double result =
      tune(compute_by_names(par_list, check), c, m, tuned_unit).raw();
    if (result < min_val)
      return min_val;
    if (result > max_val)
      return max_val;
    return VtlQuantity(unit, result);
  }

  /** This overloaded version sets the result bound to the unit limits.
   */
  VtlQuantity
  bounded_tuned_compute_by_names(const ParList & par_list,
				 double c, double m, const Unit & tuned_unit,
				 bool check = true) const
  {
    return bounded_tuned_compute_by_names(par_list, unit.min(), unit.max(),
					  c, m, tuned_unit, check);
  }

  /** Return a list of validated and adequately converted correlation
      parameters.

      @param[in] pars a list of `VtlQuantity` objects containing the
      parameters in the same order than the expected for the
      correlation. 
      
      @throw OutOfParameterRange if one of parameters is out of
      correlation range.
      @throw UnitConversionNotFound if there conversion does not
      exist.
      @exception bad_alloc if there is enough memory.
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
	const VtlQuantity & par = get<1>(p);
	if (not (par_name == "p" or par_name == "t") and // TODO lista hash
	    not precondition.check(par))
	  {
	    auto i = get<2>(p) + 1;
	    ostringstream s;
	    s << "Parameter " << i << " (" << par_name << " = " << par.raw()
	      << " " << par.unit.name << ") in correlation " << name 
	      << " does not satisfy application development range ["
	      << precondition.min_val << ", " << precondition.max_val << "]";
	    ALEPHTHROW(OutOfParameterRange, s.str());
	  }
      }

    if (not it.completed())
      {
	ostringstream s;
	s << "number of preconditions " << preconditions.size()
	  << " is different than number of parameters " << pars.size();
	ALEPHTHROW(InvalidNumberOfParameters, s.str());
      }

    return pars;
  }

  /** Given a computed result, it verifies if this is inside of the
      expected interval.

      @param[in] result a `VtlQuantity` corresponding to a possible
      correlation computation.
      @exception OutOfCorrelationRange if `result` is out of the
      expected correlation range.
   */
  VtlQuantity verify_result(const VtlQuantity & result) const
  {
    assert(&result.unit == &unit);
    if (result.get_value() < min_val or result.get_value() > max_val)
      {
	ostringstream s;
	s << "In correlation " << name << " " << subtype_name << " "
	  << type_name << " : return value (" << result.get_value() << " "
	  << result.unit.name << ") is out of range ["
	  << min_val << ", " << max_val << "] defined for the correlation";
	ALEPHTHROW(OutOfCorrelationRange, s.str());
      }

    return result;
  }

  /// Return `true` if `result` is inside of expected correlation
  /// range; `false` otherwise.
  bool check_result(const VtlQuantity & result) const
  {
    VtlQuantity ret = result;
    return ret.get_value() >= min_val and ret.get_value() <= max_val;
  }

  /// Return a list containing the correlation parameter names.
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

  /// Return all the parameter names associated to all correlations
  /// involving `target_name`
  static DynList<string> all_parameter_names(const string & target_name)
  {
    const DynList<const Correlation*> corr_list =
      correlations_tbl.filter([&target_name] (auto ptr)
			      {
				return ptr->target_name() == target_name;
			      });
    DynSetTree<string> names;
    for (auto i = corr_list.get_it(); i.has_curr(); i.next())
      for (auto it = i.get_curr()->par_names.get_it(); it.has_curr(); it.next())
	names.append(it.get_curr());
    return names.keys();
  }
};

/** Convert a `NamedPar` to a `VtlQuantity`.
   
   @ingroup Correlations
   @author Leandro Rabindranath Leon
 */
// construct a VtlQuantity from a parameter by name
inline VtlQuantity par(const Correlation::NamedPar & par)
{
  return VtlQuantity(*get<3>(par), get<2>(par));
}

/** Build a `NamedPar` given a `VtlQuantity` o `Quantity` object.

    @param[in] name the name of parameter.
    @param[in] q a `VtlQuantity` or `Quantity` object containing the
    parameter value along with its unit.
    @ingroup Correlations
    @author Leandro Rabindranath Leon
 */
inline Correlation::NamedPar npar(const string & name, const BaseQuantity & p)
{
  return Correlation::NamedPar(true, name, p.raw(), &p.unit);
}

/** Build a `NamedPar`.

    @param[in] name the name of parameter.
    @param[in] v the value of parameter.
    @param[in] unit a pointer to the unit used for representing the value.
    @ingroup Correlations
    @author Leandro Rabindranath Leon
 */
inline Correlation::NamedPar
npar(const string & name, double v, const Unit & unit)
{
  return Correlation::NamedPar(true, name, v, &unit);
}

/** Build a `NamedPar`.

    @param[in] name the name of parameter.
    @param[in] v the value of parameter.
    @param[in] unit a pointer to the unit used for representing the value.
    @ingroup Correlations
    @author Leandro Rabindranath Leon
 */
inline Correlation::NamedPar
npar(const string & name, double v, const Unit * unit_ptr)
{
  return npar(name, v, *unit_ptr);
}

/** Build a copy of a `NamedPar` object.

    @param[in] name the name of parameter.
    @param[in] par a `NamedPar` from which the parameter will be built.
    @ingroup Correlations
    @author Leandro Rabindranath Leon
*/
inline Correlation::NamedPar npar(const string & name,
				  const Correlation::NamedPar & par)
{
  return Correlation::NamedPar(true, name, get<2>(par), get<3>(par));
}

/** Macro that constructs a parameter by name homonym to a
    `VtlQuantity` variable name. 

    Example of use
    --------------

    Let's suppose variables representing temperature, gor, yg and
    yo. Some such as:

        Quantity<Fahrenheit> t = // temperature
	Quantity<Sgg> yg = ...
	Quantity<SCF_STB> rsb = ...
	Quantity<Sg_do> yo = ...

    Now, we could compute the PbAlMarhoun correlation thus:

        corr_ptr->compute_by_names(true, NPAR(t), NPAR(yg), NPAR(rsb), NPAR(yo));

    Every instance of `NPAR` macro generates a rvalue variable of type
    `ParPair` with the same name of the variable.

    @param[in] par variable of type `VtlQuantity` or `Quantity`
    representing some physical magnitude required to compute the
    correlation. 

    @ingroup Correlations
    @author Leandro Rabindranath Leon
   */

# define NPAR(par) npar(#par, par)


inline Correlation::~Correlation() {}

# define Header_Correlation_Type(type_name, hidden)			\
  struct type_name : public Correlation					\
  {									\
    type_name(const string & subtype_name,				\
	      const string & name, const string & latex_symbol,		\
	      const Unit & result_unit, double min_val, double max_val,	\
	      bool min_from_author, bool max_from_author)		\
      : Correlation(#type_name, subtype_name, name, latex_symbol,	\
		    result_unit, min_val, max_val,			\
		    min_from_author, max_from_author) {}		\
    virtual ~type_name() = 0;						\
    using Correlation::add_parameter;					\
    bool is_hidden = hidden;						\
  };									\
									\
  inline type_name::~type_name() {}

/** Declare a correlation type.

    This macro generates an entire class definition derived from
    `Correlation` class. The idea is to define a family of correlation
    subtypes.

    @param[in] type_name name of type of correlation.

    @see Correlation, 

    @ingroup Correlations, Declare_Correlation_Subtype
    @author Leandro Rabindranath Leon
   */
# define Declare_Correlation_Type(type_name)	\
  Header_Correlation_Type(type_name, false)

# define Declare_Hidden_Correlation_Type(type_name)	\
  Header_Correlation_Type(type_name, true)

Declare_Hidden_Correlation_Type(TmpFunction)

/** Deterministic oil functions.

    This class define an entire family of deterministic functions
    associated to oil as fluid.
   
   @author Leandro Rabindranath Leon
   @ingroup Correlations
 */
Declare_Correlation_Type(OilFunction)

/** Correlation 

    \class OilCorrelation
    @ingroup Correlations
 */
Declare_Correlation_Type(OilCorrelation);
Declare_Correlation_Type(GasCorrelation);
Declare_Correlation_Type(GasFunction);
Declare_Correlation_Type(WaterCorrelation);

/** Declare a correlation subtype.

    This macro generates an entire class definition derived from a
    type of correlation. The idea is to define a family of
    correlations according to a specific physical property. For
    example, the class `BubblePointPressure', which is of type
    `OilCorrelation`, is defined through this method and represents
    the family of correlations estimating the bubble point.

    @param[in] subtype_name name of subtype of correlation.
    @param[in] type_name name of correlation type from which
    `subtype_name` will derive.
    @param[in] symbol string describing the LaTeX symbol to be used
    for representing the physical property associated to the
    correlation family.

    @see Correlation, 

    @ingroup Correlations, Declare_Correlation_Subtype
    @author Leandro Rabindranath Leon
 */
# define Declare_Correlation_Subtype(subtype_name, type_name, symbol)	\
  struct subtype_name : public type_name				\
  {									\
    subtype_name(const string & name, const Unit & result_unit,		\
		 double min_val, double max_val)			\
      : type_name(#subtype_name, name, symbol, result_unit,		\
		  min_val, max_val, true, true) {}			\
									\
    subtype_name(const string & name, const Unit & unit, double min_val) \
      : type_name(#subtype_name, name, symbol, unit,			\
		  min_val, unit.max_val, true, false) {}		\
									\
    subtype_name(const string & name, const Unit & unit)		\
      : type_name(#subtype_name, name, symbol, unit, unit.min_val,	\
		  unit.max_val, false, false) {}			\
    virtual ~subtype_name() = 0;					\
									\
  protected:								\
									\
    using type_name::add_parameter;					\
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
/**
   \class BubblePointPressure
   @ingroup Correlations
 */
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
Declare_Correlation_Subtype(UndersaturatedOilDensity, OilFunction,
			    "\\rho_{oa}");
Declare_Correlation_Subtype(OilInterfacialTension, OilCorrelation,
			    "\\sigma_{go}");
Declare_Correlation_Subtype(DeadOilViscosity, OilCorrelation, "\\mu_{od}");

    // correlations & functions associated to gas
Declare_Correlation_Subtype(HydrocarbonGasPseudocriticalPressure,
			    GasCorrelation, "P_{pcHC}");
Declare_Correlation_Subtype(MixtureGasPseudocriticalPressure, GasCorrelation,
			    "P_{pcM}");
Declare_Correlation_Subtype(GasPseudocriticalPressureC7, GasCorrelation,
			    "P_{pcC_7+}");
Declare_Correlation_Subtype(GasViscosity, GasCorrelation, "\\mu_{g}");
Declare_Correlation_Subtype(GasDensity, GasFunction, "\\rho_{g}")
Declare_Correlation_Subtype(GasVolumeFactor, GasFunction, "B_{g}");
Declare_Correlation_Subtype(GasCompressibility, GasCorrelation, "Z\\ factor");
Declare_Correlation_Subtype(HydrocarbonGasPseudocriticalTemperature,
			    GasCorrelation,
			    "T_{pcHC}");
Declare_Correlation_Subtype(MixtureGasPseudocriticalTemperature, GasCorrelation,
			    "T_{pcM}");
//Declare_Correlation_Subtype(GasSpecificGravity, GasCorrelation, "\\gamma_g");
Declare_Correlation_Subtype(HydrocarbonGasSpecificGravity, GasCorrelation,
			    "\\gamma_{gHC}");
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
Declare_Correlation_Subtype(VaporPressure, WaterCorrelation, "P_{v}");
Declare_Correlation_Subtype(WetGasVolumeFactor, GasFunction, "B_{wg}");
Declare_Correlation_Subtype(WetGasSpecificGravity, GasCorrelation, "Y_{wgR}");
Declare_Correlation_Subtype(EquivalentVolume, GasCorrelation, "V_{eq}");
Declare_Correlation_Subtype(AdditionalGasProduced, GasCorrelation, "G_{pa}");
Declare_Correlation_Subtype(ProducingGasOilRatio, GasFunction, "GOR");

# endif // CORRELATION_H

//  LocalWords:  OutOfParameterRange
