# ifndef PVT_TUNER_H
# define PVT_TUNER_H

# include <json.hpp>
# include <ah-stl-utils.H>
# include <ah-dispatcher.H>

# include <line.H>
# include <lfit.H>

# include <correlations/pvt-correlations.H>
# include <correlations/correlation-stats.H>
# include "metadata-exceptions.H"

using Json = nlohmann::json;

# define PVT_INVALID_VALUE (-1) 

struct VectorDesc
{
  // contain these values "pb", "rs", "bob", "coa", "boa", "uod", "uob", "uoa"
  static const DynSetTree<string> valid_target_names;

  bool is_valid_target_name(const string & target_name) const noexcept
  {  
    return valid_target_names.has(target_name);
  }

  mutable double t = 0; //  unit will always be in Fahrenheit
  mutable double pb = PVT_INVALID_VALUE; // bubble point pressure in the
				       // same unit that p 
  mutable double bobp = PVT_INVALID_VALUE; // in RB_STB
  mutable double uod = PVT_INVALID_VALUE; // dead oil viscosity in cP
  mutable double uobp = PVT_INVALID_VALUE; // oil viscosity at pb in cP
  mutable Array<double> p;
  const Unit * punit = nullptr;

  mutable string yname = "no-name";
  const Unit * yunit = nullptr;
  mutable Array<double> y;

  double pmin() const
  {
    assert(p.get_first() < p.get_last());
    return p.get_first();
  }
  
  double pmax() const
  {
    assert(p.get_first() < p.get_last());
    return p.get_last();
  }

  Json to_json() const
  {
    Json j;
    j["t"] = t;
    j["pb"] = pb;
    j["bobp"] = bobp;
    j["uobp"] = uobp;
    j["uod"] = uod;
    j["punit"] = punit->name;
    j["target_name"] = yname;
    j["target_unit"] = yunit->name;
    j["p"] = to_vector(p);
    j["y"] = to_vector(y);
    return j;
  }

  VectorDesc(const Json & j)
    : t(j["t"]), pb(j["pb"]), bobp(j["bobp"]), uod(j["uod"]), uobp(j["uobp"]),
      p(to_Array(j["p"].get<vector<double>>())),
      punit(Unit::search_by_name(j["punit"])),
      yname(j["target_name"].get<string>()),
      yunit(Unit::search_by_name(j["target_unit"])),
      y(to_Array(j["y"].get<vector<double>>()))
  {
    if (punit == nullptr)
      ZENTHROW(UnitNotFound, "pressure unit " + j["punit"].get<string>() +
	       " not found");
    if (&punit->physical_quantity != &Pressure::get_instance())
      ZENTHROW(PressureMismatch, "unit " + punit->name +
	       " does not represent pressure");
    if (yunit == nullptr)
      ZENTHROW(UnitNotFound, "unit for " + yname + " not found");
  }

  bool is_valid() const noexcept
  {
    return t > 0 and pb > 0 and punit != nullptr and yunit != nullptr and
      yname != "no-name" and p.size() > 1 and p.size() == y.size() and
      is_sorted(p);
  }

  void validate_sort()
  {
    if (not is_sorted(p))
      {
	p.reverse();
	y.reverse();
      }
    if (not is_sorted(p))
      ZENTHROW(SamplesUnsorted, "pressures are not sorted");
  }

  void pre_validate()
  {
    if (punit == nullptr or yunit == nullptr)
      ZENTHROW(InvalidUnit, "vector unit are not initialized");
    if (not psia::get_instance().is_sibling(*punit))
      ZENTHROW(InvalidUnit, punit->name + " is not a valid pressure unit");
    if (p.size() <= 1)
      ZENTHROW(LengthMismatch, "Pressure array size is less or equal tan 1");
    if (not p.all([this] (auto p) { return BaseQuantity::is_valid(p, *punit); }))
      ZENTHROW(OutOfUnitRange,
	       "at least a pressure value is out of its unit range");
    if (punit != &psia::get_instance())
      {
	mutable_unit_convert(*punit, p, psia::get_instance());
	pb = unit_convert(*punit, pb, psia::get_instance());
	punit = &psia::get_instance();
      }
  }

  void post_validate()
  {
    if (p.size() != y.size())
      ZENTHROW(LengthMismatch, "pressure array and " + yname +
	       " array have different sizes");
  }

  inline 
  VectorDesc(double t, double pb, double bobp, double uod, double uobp,
	     const Array<double> & p, const Unit * punit,
	     const string & yname, const Array<double> & y, const Unit * yunit);

  VectorDesc() {}

  VectorDesc(double t, const string & name) : t(t), yname(name)
  {
    if (t <= 0)
      ZENTHROW(InvalidRange, "t value " + ::to_string(t) +
	       " less or equal to zero");
  }

  CorrStat::Desc stats(const Array<double> & yc, const Unit * unit_ptr) const
  {
    CorrStat s = y;
    return s.stats(yunit == unit_ptr ? yc : unit_convert(*unit_ptr, yc, *yunit));
  }

  bool operator < (const VectorDesc & rhs) const noexcept
  {
    if (yname == rhs.yname)
      return t < rhs.t;
    
    return yname < rhs.yname;
  }

  bool has_same_t_pb(const VectorDesc & v) const noexcept
  {
    return t == v.t and pb == v.pb;
  }

  // Return true if this is "parallel" to v; this is if this and v
  // share the same t values and p vectors
  bool has_same_tp(const VectorDesc & v) const noexcept
  {
    return t == v.t and
      zip_all([] (auto t) { return get<0>(t) == get<1>(t); }, p, v.p);
  }

  bool is_parallel(const VectorDesc & v) const noexcept
  {
    return has_same_tp(v);
  }

  size_t get_yindex(double yval) const
  {
    auto ret = y.find_index([yval] (auto v) { return v == yval; });
    if (ret >= y.size())
      ZENTHROW(ValueNotFound, "y value " + ::to_string(yval) + " not found");
    return ret;
  }

  /** Compute the y value according to input pressure pval. 
      
      The value of pval is searched in the p array. If this one is
      found, then the parallel y value in the array y is
      returned. Otherwise, we fall into the following cases:

      1- pval belongs to [p[0], p[n-1]]. In this case y is computed by
         interpolation between the two surrounding values to pval
      2- pval is less than p[0]. In this case y is computed by
         extrapolation between p[0] and p[1]
      3- oval is greater than p[n-1]. In this case  is computed by
         extrapolation between p[n-2] and p[n-1]     
   */
  double gety(double pval) const
  {
    const long n = p.size();
    long i = binary_search(p, pval);
    if (i < 0 or i >= n)
      ZENTHROW(ValueNotFound, ::to_string(pval) + " not found in data set");

    const double pentry = p(i);
    if (pentry == pval)
      return y(i);

    if (pval < pentry)
      --i;

    double result = 0;
    if (i < 0)
      result = extrapolate_left(p(0), p(1), y(0), y(1), pval);
    else if (i >= n - 1)
      result = extrapolate_right(p(n - 2), p(n - 1), y(n - 2), y(n - 1), pval);
    else
      result = interpolate(p(i), p(i + 1), y(i), y(i + 1), pval);

    if (result < yunit->min_val)
      return yunit->min_val;
    if (result > yunit->max_val)
      return yunit->max_val;
    return result;
  }

  /// Compute the y values corresponding to pressure values in pvals
  /// through interpolation
  template <template <typename> class C>
  DynList<double> gety(const C<double> & pvals) const
  {
    return pvals.maps([this] (auto & p) { return this->gety(p); });
  }

  // Transform pressures of this to pressures of src and computes new
  // values for y through interpolation
  VectorDesc & make_parallel(const VectorDesc & src)
  {
    if (t != src.t)
      ZENTHROW(MetadataException,
	       "trying to do parallel two vector of different t");
    if (is_parallel(src))
      return *this;
    Array<double> newy = gety(src.p);
    p = src.p;
    y = move(newy);
    return *this;
  }

  VectorDesc merge_with(const VectorDesc & y_vector) const
  {
    if (not has_same_t_pb(y_vector))
      ZENTHROW(MetadataException,
	       "vectors to be merged don't share same t and pb");
    Array<double> pressures = p; 
    if (y_vector.yname == "coa")
      pressures.append(pressures);
    return VectorDesc(y_vector.t, y_vector.pb, max(bobp, y_vector.bobp),
		      max(uod, y_vector.uod), max(uobp, y_vector.uobp),
		      pressures, punit, y_vector.yname,
		      y_vector.gety(p), y_vector.yunit);
  }

  DynList<ParPair> to_parpair() const
  {
    return y.maps<ParPair>([this] (auto y)
			   { return ParPair(yname, ValPair(y, yunit)); });
  }

  DynList<DynList<string>> to_dynlist() const
  {
    DynList<DynList<string>> ret = zip_maps<DynList<string>>([] (auto t)
      {
	return build_dynlist<string>(::to_string(get<0>(t)),
				     ::to_string(get<1>(t), 10));
			       
      }, p, y);
    ret.insert(build_dynlist<string>("p " + punit->name,
				     yname + " " + yunit->name));
    return ret;
  }

  string to_string() const
  {
    ostringstream s;
    s << "For t = " << t << " " << Fahrenheit::get_instance().name;
    if (pb > 0)
      s << " pb = " << pb << " " << punit->name;
    if (uod > 0)
      s << " uod = " << uod << " " << CP::get_instance().name;
    if (bobp > 0)
      s << " bobp = " << bobp << " " << RB_STB::get_instance().name;
    if (uobp > 0)
      s << " uobp = " << uobp << " " << CP::get_instance().name;
    s << " :" << endl
      << shift_lines_to_left(Aleph::to_string(format_string(to_dynlist())), 2);
    return s.str();
  }

  friend ostream & operator << (ostream & out, const VectorDesc & d)
  {
    return out << d.to_string();
  }

  /** Assuming that `this` contains values for viscosity (uo) in both
      regions saturated and undersaturated, this method split uo in
      two vector named uob and uob respectively. The bubble point and
      uod values are computed.

      For computing pb TODO ...

   */
  pair<VectorDesc, VectorDesc> split_uo() const
  {
    if (yname != "uo")
      ZENTHROW(InvalidTargetName, "yname " + yname + " is not \"uo\"");

    if (&yunit->physical_quantity != &DynamicViscosity::get_instance())
      ZENTHROW(InvalidTargetUnit, "yunit " + yunit->name +
	       " is not an unit for oil formation volume ratio");

    if (not is_sorted(p))
      ZENTHROW(SamplesUnsorted, "split_uo(): pressure values are not sorted");

    // First, we must determine p,uo corresponding to the inflection point
    size_t min_idx = 0;
    double min_uo = numeric_limits<double>::max();
    for (auto it = enum_zip_it(y); it.has_curr(); it.next())
      {
	auto curr = it.get_curr();
	if (get<0>(curr) < min_uo)
	  {
	    min_idx = get<1>(curr);
	    min_uo = get<0>(curr);
	  }
      } // after this min_y is the index of minimum value of uo
    
    Array<double> p_below = p.take(min_idx + 1);
    Array<double> uob = y.take(min_idx + 1);
    if (not is_inversely_sorted(uob))
      ZENTHROW(SamplesUnsorted, "saturated region of uo is not decreasing");

    Array<double> p_above, uoa;
    for (size_t i = min_idx + 1; i < p.size(); ++i)
      {
	p_above.append(p(i));
	uoa.append(y(i));
      }

    if (not is_sorted(uoa))
      ZENTHROW(SamplesUnsorted, "undersaturated region of uo is not increasing");

    if (p_below.size() < 2)
      ZENTHROW(CommandLineError, "Not enough point for estimating uob");

    if (p_above.size() < 2)
      ZENTHROW(CommandLineError, "Not enough point for estimating uoa");

    assert(p_below.get_last() < p_above.get_first());
    assert(uob.get_last() < uoa.get_first());

    // In this point (p_below,uob) contains the saturated uo and
    // (p_above, uoa) the undersaturated one.

    // First step we compute uod by extrapolating the two first points
    // of p_below,uob to the cut point p = 0
    const double uod = extrapolate_left(p_below(0), p_below(1),
					uob(0), uob(1), 0);

    // Now we build two lines.

    // The first line is compounded by the two last points of
    // p_below,uob
    const size_t & nb = p_below.size();
    const LineEq lbelow =
      { p_below(nb - 2), uob(nb - 2), p_below(nb - 1), uob(nb - 1) };

    // The second line is compounded by the two first points of p_above,uoa
    const LineEq labove = { p_above(0), uoa(0), p_above(1), uoa(1) };

    // Now the intersection between these lines will give us an
    // approximation for the minimum uo, which would correspond to the
    // bubble point and uobp

    auto p = lbelow.intersection(labove);
    double & pb = p.first;
    double & uobp = p.second;
    if (uobp < 0 or pb <= p_below.get_last())
      { // These are improbable but possible cases that would
	// correspond to a bad model
	pb = p_below.get_last();
	uobp = uob.get_last();
      }
    else
      {
	p_below.append(pb); // (pb,uod) at the end of the saturated
			    // zone for better precision 
	uob.append(uobp);
      }
    
    p_below.insert(0); // (0,uod) at the beginning of the saturated
		       // zone for better precision  
    uob.insert(uod);

    VectorDesc v_below(t, pb, PVT_INVALID_VALUE, uod, uobp,
		       p_below, punit, "uob", uob, yunit);
    VectorDesc v_above(t, pb, PVT_INVALID_VALUE, uod, uobp,
		       p_above, punit, "uoa", uoa, yunit);

    return make_pair(move(v_below), move(v_above));
  }
};

struct ConstDesc
{
  const string name = "no-name";
  const double value = 0;
  const Unit * unit_ptr = nullptr;

  string to_cplot() const { return "--" + name + " " + to_str(value); }

  // only take in account the name in order to index and sort by name
  // in a binary search tree
  bool operator < (const ConstDesc & rhs) const noexcept
  {
    return name < rhs.name;
  }

  ConstDesc(const string name, double value, const Unit * unit_ptr) noexcept
    : name(name), value(value), unit_ptr(unit_ptr) {}

  ConstDesc(const string & name = "no-name") : name(name) {}

  Json to_json() const
  {
    Json j;
    j["name"] = name;
    j["value"] = value;
    j["unit"] = unit_ptr->name;
    return j;
  }

  ConstDesc(const Json & j)
    : name(j["name"].get<string>()), value(j["value"]),
      unit_ptr(Unit::search_by_name(j["unit"]))
  {
    if (unit_ptr == nullptr)
      ZENTHROW(UnitNotFound, "unit for " + name + " not found");
  }

  string to_string() const
  {
    return name + " " + ::to_string(value) + " " + unit_ptr->name;
  }

  friend ostream & operator << (ostream & out, const ConstDesc & d)
  {
    return out << d.to_string();
  }

  DynList<string> to_dynlist() const
  {
    return build_dynlist<string>(name, ::to_string(value), unit_ptr->name);
  }
};

constexpr size_t Dim_Pars_List = 100;

struct PvtData
{
  DynSetTree<ConstDesc> const_values; // indexed and sorted by name
  DynSetTree<VectorDesc> vectors;
  DynSetTree<string> names;

    const Correlation * pb_corr = nullptr;
  double c_pb = 0, m_pb = 1;    
  const Correlation * rs_corr = nullptr;
  double c_rs = 0, m_rs = 1;    
  const Correlation * bob_corr = nullptr;
  double c_bob = 0, m_bob = 1;    
  const Correlation * boa_corr = nullptr;
  double c_boa = 0, m_boa = 1;    
  const Correlation * coa_corr = nullptr;
  double c_coa = 0, m_coa = 1;    
  const Correlation * uod_corr = nullptr;
  double c_uod = 0, m_uod = 1;    
  const Correlation * uob_corr = nullptr;
  double c_uob = 0, m_uob = 1;    
  const Correlation * uoa_corr = nullptr;
  double c_uoa = 0, m_uoa = 1;

  /// Return a sorted list of all seen pressure values
  DynList<double> all_pressures() const
  {
    DynSetTree<double> pressures;
    for (auto & v : vectors)
      v.p.for_each([&pressures] (auto p) { pressures.insert(p); });
    return pressures.keys();
  }

  /// Return a sorted list of all seen bubble point pressures
  DynList<double> all_pb() const
  {
    DynSetTree<double> pbs;
    for (auto & v : vectors)
      if (v.pb > 0)
	pbs.insert(v.pb);
    return pbs.keys();
  }

  /// Return the minimum pressure value
  double pmin() const
  {
    return vectors.foldl(numeric_limits<double>::max(),
			 [] (double acu, auto & v)
			 { return min(acu, v.pmin()); });
  }

  /// Return the minimum pressure value
  double pmax() const
  {
    return vectors.foldl(0, [] (double acu, auto & v)
			 { return max(acu, v.pmax()); });
  }

  /// Return a sorted list of all seen temperatures
  DynList<double> get_temperatures() const
  {
    DynSetTree<double> s;
    vectors.for_each([&s] (auto & v) { s.append(v.t); });
    return s.maps([] (auto v) { return v; });
  }

# define CORR_PARS(NAME, s, dft_corr)					\
  if (NAME##_corr)							\
    s << " --" #NAME " " << NAME##_corr->name << " --c-"#NAME" "	\
      << to_str(c_##NAME) << " --m-"#NAME" " << to_str(m_##NAME);	\
  else									\
    s << " --" #NAME " " << dft_corr::get_instance().name;

  string cplot_consts() const
  {
    return const_values.foldl<string>("", [] (auto & acu, auto & c)
      {
	return acu + c.to_cplot() + " ";
      });
  }

  string cplot_corrs()
  {
    ostringstream s;
    CORR_PARS(pb, s, PbAlMarhoun);
    CORR_PARS(rs, s, RsAlMarhoun);
    CORR_PARS(bob, s, BobAlmarhoun);
    CORR_PARS(boa, s, BoaMcCain);
    CORR_PARS(coa, s, CoaVasquezBeggs);
    CORR_PARS(uod, s, UodBeal);
    CORR_PARS(uob, s, UobBeggsRobinson);
    CORR_PARS(uoa, s, UoaBeal);
    s << " --cob CobMcCainEtAl ";
    return s.str();
  }

  bool defined() const
  {
    return not (const_values.is_empty() and vectors.is_empty());
  }

# define CORR_TEST(NAME) if (target_name == #NAME)	\
    return make_tuple(NAME##_corr, c_##NAME, m_##NAME)

  tuple<const Correlation*, double, double>
  get_corr(const string & target_name) const
  {
    CORR_TEST(pb);
    CORR_TEST(rs);
    CORR_TEST(bob);
    CORR_TEST(boa);
    CORR_TEST(coa);
    CORR_TEST(uod);
    CORR_TEST(uob);
    CORR_TEST(uoa);
    ZENTHROW(InvalidTargetName, "target name " + target_name + " not found");
  }

  bool are_all_correlations_defined() const
  {
    static DynList<string> names =
      { "pb", "rs", "bob", "boa", "coa", "uod", "uob", "uob" };
    return names.all([this] (auto & name)
		     {
		       return get<0>(this->get_corr(name)) != nullptr;
		     });
  }

  /// Return a list of target name properties that have not defined a
  /// correlation
  DynList<string> missing_correlations() const
  {
    static DynList<string> names =
      { "pb", "rs", "bob", "boa", "coa", "uod", "uob", "uob" };
    return names.filter([this] (auto & name)
			{
			  return not get<0>(this->get_corr(name));
			});
  }

  static Json to_json(const Correlation * corr_ptr, double c, double m)
  {
    Json j;
    j["name"] = corr_ptr ? corr_ptr->name : "null";
    j["c"] = c;
    j["m"] = m;
    return j;
  }

  static
  void load_corr_from_json(const Json & j, const Correlation *& corr_ptr,
			   double & c, double & m, const string & target_name)
  {
    c = j["c"];
    m = j["m"];
    const string name = j["name"];
    corr_ptr = Correlation::search_by_name(name);
    if (corr_ptr == nullptr)
      return;
    if (corr_ptr->target_name() != target_name)
      ZENTHROW(CorrelationNotApplicable, "correlation " + name +
	       " is not for " + target_name);
  }

  static string to_string( const Correlation * corr_ptr, double c, double m)
  {
    ostringstream s;
    s << corr_ptr->name << " c = " << c << " m = " << m;
    return s.str();
  }

  /// Helper function for building a string describing the correlation
  static DynList<string>  corr_to_list(const string & tgt,
				       const Correlation * corr_ptr,
				       double c, double m)
  {
    if (corr_ptr)
      return build_dynlist<string>(tgt, " = ", corr_ptr->name, " c = ",
				   to_str(c), " m = ", to_str(m));
    return build_dynlist<string>(tgt, " = ", "undefined", " c = ", "undefined",
				 " m = ", "undefined");
  }

# define Corr_List(NAME) corr_to_list(#NAME, NAME##_corr, c_##NAME, m_##NAME)

  string corr_list() const
  {
    return Aleph::to_string
      (format_string
       (build_dynlist<DynList<string>>(Corr_List(pb), Corr_List(rs),
				       Corr_List(bob), Corr_List(boa), 
				       Corr_List(coa), Corr_List(uod),
				       Corr_List(uob), Corr_List(uoa))));
  }

  Json to_json() const
  {
    Json j;
    j["pb_corr"] = to_json(pb_corr, c_pb, m_pb);
    j["rs_corr"] = to_json(rs_corr, c_rs, m_rs);
    j["bob_corr"] = to_json(bob_corr, c_bob, m_bob);
    j["boa_corr"] = to_json(boa_corr, c_boa, m_boa);
    j["uod_corr"] = to_json(uod_corr, c_uod, m_uod);
    j["uob_corr"] = to_json(uob_corr, c_uob, m_uob);
    j["uoa_corr"] = to_json(uoa_corr, c_uoa, m_uoa);
    j["coa_corr"] = to_json(coa_corr, c_coa, m_coa);
    j["vectors"] =
      to_vector(vectors.maps<Json>([] (auto &v) { return v.to_json(); }));
    j["constants"] =
      to_vector(const_values.maps<Json>([] (auto &v) { return v.to_json(); }));
    return j;
  }

  string const_list() const
  {
    DynList<DynList<string>> l = const_values.maps<DynList<string>>
      ([] (auto & c) { return c.to_dynlist(); });
    l.insert(build_dynlist<string>("name", "value", "unit"));
    return Aleph::to_string(format_string(l));
  }

  string to_string() const
  {
    ostringstream s;
    s << "Constants:" << endl
      << shift_lines_to_left(const_list(), 2) << endl
      << "Correlations:" << endl
      << shift_lines_to_left(corr_list(), 2) << endl
      << "Variables:" << endl;
    for (auto it = vectors.get_it(); it.has_curr(); it.next())
      {
	auto & v = it.get_curr();
	s << shift_lines_to_left(v.to_string(), 2);
	if (&v != &vectors.get_last())
	  s << endl << endl;
      }
    return s.str();
  }

  friend ostream & operator << (ostream & out, const PvtData & d)
  {
    return out << d.to_string();
  }

  void add_const(const ConstDesc & c)
  {
    if (const_values.has(c))
      ZENTHROW(DuplicatedConstName, "constant name " + c.name +
	       " is already inserted");
    const_values.append(c);
    names.append(c.name);
  }

  void add_const(const string & name, double v, const Unit & unit)
  {
    add_const(ConstDesc(name, v, &unit));
  }

  void rm_const(const string & name)
  {
    if (search_const(name) == nullptr)
      ZENTHROW(ConstNameNotFound, "const name " + name + " not found");
    const_values.remove(ConstDesc(name));
    names.remove(name);
  }

  DynList<const VectorDesc*> search_vectors(const string & name) const noexcept
  {
    return vectors.ptr_filter([&name] (auto & v) { return v.yname == name; });
  }

  pair<const Unit*, const Unit*> search_units(const string & target_name) const
  {
    auto v = search_vectors(target_name);
    if (v.is_empty())
      ZENTHROW(VarNameNotFound, target_name + " not found");

    auto & desc_ptr = v.get_first();
    return make_pair(desc_ptr->punit, desc_ptr->yunit);
  }

  void add_vector(const VectorDesc & v)
  {
    if (vectors.has(v))
      ZENTHROW(DuplicatedVarName, "add_vector(): name " + v.yname +
	       " with t = " + ::to_string(v.t) + " is already defined");

    if (&v.punit->physical_quantity != &Pressure::get_instance())
      ZENTHROW(InvalidPressureUnit, v.punit->name + " is not for pressure");

    if (not is_sorted(v.p))
      ZENTHROW(SamplesUnsorted, "pressure values are not sorted");

    auto sibling = search_vectors(v.yname);
    if (not sibling.is_empty() and not sibling.all([&v] (auto & p)
        { return p->yunit->is_sibling(*v.yunit); }))
      ZENTHROW(InvalidTargetUnit, v.yunit->name + " is not sibling");

    names.append("p");
    names.append("pb");
    names.append("t");
    names.append("uod");
    names.append("uobp");
    names.append("bobp");
    names.append(v.yname);
    vectors.insert(v);
  }

  VectorDesc rm_vector(double t, const string & target_name)
  {
    auto l = search_vectors(target_name);
    if (l.is_empty())
      ZENTHROW(VarNameNotFound, "variable " + target_name + " not found");
    auto ptr = l.find_ptr([&target_name, &t] (auto & ptr)
			  { return ptr->yname == target_name and ptr->t == t; });
    if (ptr == nullptr)
      ZENTHROW(VarNameNotFound, "variable " + target_name +
	       " is to associated to t = " + ::to_string(t));
    VectorDesc ret = **ptr;
    vectors.remove(VectorDesc(t, target_name));
    if (not vectors.exists([&target_name, &t] (auto &v)
			   { return v.yname == target_name and v.t == t; }))
      names.remove(target_name);
    return ret;
  }

  void
  add_vector(double t, double pb, double bobp, double uod, double uobp,
	     const Array<double> & p, const Unit & punit,
	     const string yname, const Array<double> & y, const Unit & yunit)
  {
    add_vector(VectorDesc(t, pb, bobp, uod, uobp, p, &punit, yname, y, &yunit));
  }

  void split_uo()
  {
    const DynList<const VectorDesc*> uo_vectors =  search_vectors("uo");
    if (uo_vectors.is_empty())
      ZENTHROW(VarNameNotFound, "data does not contain uo");
    for (auto it = uo_vectors.get_it(); it.has_curr(); it.next())
    {
      auto uo_ptr = it.get_curr();
      auto p = uo_ptr->split_uo();
      add_vector(p.first);
      add_vector(p.second);
    }
    
    try // Now we must be sure that the resulting spit vector are
	// ordered by temperature and bubble point
      {
	tp_sets(); // this call would throw exceptions if the split uo
		   // vector would be unsorted
      }
    catch (PressureMismatch)
      {
	ZENTHROW(PressureMismatch, "split_uo gives unsorted pb points");
      }
  }

  PvtData() {}

  PvtData(istream & input)
  {
    Json j = Json::parse(input);
    load_corr_from_json(j["pb_corr"], pb_corr, c_pb, m_pb, "pb");
    load_corr_from_json(j["rs_corr"], rs_corr, c_rs, m_rs, "rs");
    load_corr_from_json(j["bob_corr"], bob_corr, c_bob, m_bob, "bob");
    load_corr_from_json(j["boa_corr"], boa_corr, c_boa, m_boa, "boa");
    load_corr_from_json(j["coa_corr"], coa_corr, c_coa, m_coa, "coa");
    load_corr_from_json(j["uob_corr"], uob_corr, c_uob, m_uob, "uob");
    load_corr_from_json(j["uoa_corr"], uoa_corr, c_uoa, m_uoa, "uoa");
    load_corr_from_json(j["uod_corr"], uod_corr, c_uod, m_uoa, "uod");
    for (const ConstDesc & c : j["constants"])
      add_const(c);

    for (const Json & jv : j["vectors"])
      add_vector(jv);
  }

  const ConstDesc * search_const(const string & name) const noexcept
  {
    return const_values.search(ConstDesc(name));
  }

  struct Sample
  {
    double t = PVT_INVALID_VALUE, pb = PVT_INVALID_VALUE,
      bobp = PVT_INVALID_VALUE, uod = PVT_INVALID_VALUE,
      uobp = PVT_INVALID_VALUE;
    DynList<double> pvals;
    const Unit * punit = nullptr;
    const string name = "no-name";
    DynList<double> yvals;
    const Unit * yunit = nullptr;
    Sample() {}
    Sample(double __t, double __pb, double __bobp, double __uod, double __uobp,
	   const DynList<double> & __pvals, const Unit * __punit,
	   const string & __name,
	   const DynList<double> & __yvals, const Unit * __yunit)
      : t(__t), pb(__pb), bobp(__bobp), uod(__uod), uobp(__uobp),
	pvals(move(__pvals)), punit(__punit), name(__name),
	yvals(move(__yvals)), yunit(__yunit) {}
    friend ostream & operator << (ostream & out, const Sample & s)
    {
      out << "t    = " << s.t << endl
	  << "pb   = " << s.pb << endl
	  << "bobp = " << s.bobp << endl
	  << "uod  = " << s.uod << endl
	  << "uobp = " << s.uobp << endl
	  << "name = " << s.name << endl
	  << "p =";
      s.pvals.for_each([&out] (auto p) { out << " " << p; });
      out << endl
	  << "y =";
      s.yvals.for_each([&out] (auto y) { out << " " << y; });
      return out;
    }
  };

# define SET_CORRRELATION(tgt_name)	\
  void set_##tgt_name(const Correlation * corr_ptr, double c = 0, double m = 1)\
  {									\
    if (corr_ptr->target_name() != #tgt_name)				\
      ZENTHROW(InvalidTargetName, "correlation " + corr_ptr->name +	\
	       " is not for target " #tgt_name);			\
    tgt_name##_corr = corr_ptr;						\
    c_##tgt_name = c;							\
    m_##tgt_name = m;							\
  }									\
									\
  void rm_##tgt_name##_correlation()					\
  {									\
    tgt_name##_corr = nullptr;						\
    c_##tgt_name = 0;							\
    m_##tgt_name = 1;							\
  }

  SET_CORRRELATION(pb);
  SET_CORRRELATION(rs);
  SET_CORRRELATION(bob);
  SET_CORRRELATION(coa);
  SET_CORRRELATION(boa);
  SET_CORRRELATION(uod);
  SET_CORRRELATION(uob);
  SET_CORRRELATION(uoa);

  /// Return a list with all parallel vectors to v, included v
  // TODO"revisar y eventualmente reahcer
  DynList<const VectorDesc*> parallel_vectors(const VectorDesc & v)
    const noexcept
  {
    auto ret = build_dynlist<const VectorDesc*>(&v);
    for (auto it = vectors.get_it(); it.has_curr(); it.next())
      {
	auto & curr_v = it.get_curr();
	if (&curr_v == &v)
	  continue;
	if (v.is_parallel(curr_v))
	  ret.append(&curr_v);
      }
    return ret;
  }

  struct TpSet
  {
    double t = PVT_INVALID_VALUE, pb = PVT_INVALID_VALUE,
      uod = PVT_INVALID_VALUE;
    TpSet() {}
    TpSet(double __t, double __pb, double __uod)
      : t(__t), pb(__pb), uod(__uod) {}
  };

  DynList<TpSet> tp_sets() const
  { // key is temperature
    DynMapTree<double, TpSet> tuples;
    for (auto it = vectors.get_it(); it.has_curr(); it.next())
      {
  	const VectorDesc & vdesc = it.get_curr();
	if (vdesc.yname == "uo")
	  continue;
  	auto ptr = tuples.search(vdesc.t);
  	if (ptr == nullptr)
	  if (vdesc.yname == "uob" or vdesc.yname == "uoa")
	    tuples.insert(vdesc.t, TpSet(vdesc.t,
					 unit_convert(*vdesc.punit, vdesc.pb,
						      psia::get_instance()),
					 unit_convert(*vdesc.yunit, vdesc.uod,
						      CP::get_instance())));
	  else
	    tuples.insert(vdesc.t,
			  TpSet(vdesc.t, unit_convert(*vdesc.punit, vdesc.pb,
						      psia::get_instance()),
				PVT_INVALID_VALUE));
	else
  	  {
  	    const double p = unit_convert(*vdesc.punit, vdesc.pb,
  					  psia::get_instance());
	    TpSet & t = ptr->second;
  	    if (t.pb > 0 and t.pb != p)
  	      {
		cout << "Found two contradictory pairs t, pb: "
		     << ptr->first << ","
		     << t.pb << " != " << vdesc.t << "," << vdesc.pb << endl;
  		abort();
  	      }
	    if (vdesc.yname == "uob" or vdesc.yname == "uoa")
	      t.uod = unit_convert(*vdesc.yunit, vdesc.uod, CP::get_instance());
	    else if (t.uod > 0 and t.uod != vdesc.uod)
	      {
  		cout << "Found two ambiguous uod values for t = " << vdesc.t
		     << " pb = " << vdesc.pb << " " << t.uod << " != "
		     << vdesc.uod << endl;
		abort();
	      }
  	  }
      }

     if (not is_sorted(tuples.values().
		       maps<double>([] (auto & t) { return t.pb; })))
       ZENTHROW(PressureMismatch,
		"pb values are not sorted through temperatures");

    return tuples.maps<TpSet>([] (auto & p) { return p.second; });
  }

  /** Takes the data set and return a list of list with the required
      input vector needed for computing corr_ptr
  */
  struct CorrInput
  {
        // contains pressures on the which the correlation must be evaluated
    const VectorDesc * p_ref = nullptr;

    // contain another vectors which yname must be input to the
    // correlation and that are parallel to p_ref
    DynList<VectorDesc> vlist;
    double t = PVT_INVALID_VALUE, pb = PVT_INVALID_VALUE,
      bobp = PVT_INVALID_VALUE, uod = PVT_INVALID_VALUE,
      uobp = PVT_INVALID_VALUE;
    CorrInput() {}
    CorrInput(double __t, double __pb, double __uod)
      : t(__t), pb(__pb), uod(__uod) {}
    CorrInput(const VectorDesc * __p_ref, const DynList<VectorDesc> & __vlist)
      : p_ref(__p_ref), vlist(__vlist),
	t(p_ref->t), pb(p_ref->pb), uod(p_ref->uod)
    {
      for (auto it = vlist.get_it(); it.has_curr(); it.next())
	{
	  const VectorDesc & v = it.get_curr();
	  if (v.bobp != PVT_INVALID_VALUE)
	    bobp = v.bobp;
	  if (uod == PVT_INVALID_VALUE and v.uod != PVT_INVALID_VALUE)
	    {
	      assert(uobp == PVT_INVALID_VALUE);
	      uod = v.uod;
	      assert(v.uobp != PVT_INVALID_VALUE);
	      uobp = v.uobp;
	    }
	  if (v.uobp != PVT_INVALID_VALUE)
	    assert(uod != PVT_INVALID_VALUE);
	}
    }
    friend ostream & operator << (ostream & out, const CorrInput & ci)
    {
      out << "P: t = " << ci.t << " pb = " << ci.pb << " uod = " << ci.uod
	  << endl
	  << *ci.p_ref << endl
	  << "additional properties:" << endl;
      ci.vlist.for_each([&out] (auto & v) { out << v << endl; });
      cout << "================" << endl;
      return out;
    }
  };

  DynList<CorrInput>
  build_input_for_correlation(const Correlation * corr_ptr) const
  {
    static DynSetTree<string> var_pars =
      { "p", "pb", "uod", "bobp", "uobp", "t" };
    const string target_name = corr_ptr->target_name();
    const bool is_static_target = target_name == "pb" or target_name == "uod";

    if (is_static_target)
      {
	return tp_sets().maps<CorrInput>([] (auto t)
					 { return CorrInput(t.t, t.pb, t.uod); });
	  
      }

    const DynList<const VectorDesc*> src_vectors =
      search_vectors(corr_ptr->target_name());

    if (src_vectors.is_empty() and not is_static_target)
      ZENTHROW(MetadataException, "Not vectors found for target name " +
	       corr_ptr->target_name());

    DynMapTree<double, DynList<const VectorDesc*>> vtbl;

    for (auto it = corr_ptr->get_preconditions().get_it();
	 it.has_curr(); it.next())
      {
	const CorrelationPar & par = it.get_curr();
	const auto & names_list =
	  par.names().maps<string>([] (auto p) { return p.first; });

	if (names_list.exists([&] (auto name) { return var_pars.has(name); }))
	  continue;
	if (names_list.exists([this] (auto name)
			      { return this->search_const(name) != nullptr; }))
	  continue;

	bool name_found = false;
	for (auto it = names_list.get_it(); it.has_curr(); it.next())
	  {
	    auto & name = it.get_curr();
	    const DynList<const VectorDesc*> vlist = search_vectors(name);
	    if (not vlist.is_empty())
	      {
		for (auto it = vlist.get_it(); it.has_curr(); it.next())
		  {
		    const VectorDesc * vptr = it.get_curr();
		    vtbl[vptr->t].append(vptr);
		  }
		name_found = true;
		break; 
	      }
	  }

	if (not name_found)
	  ZENTHROW(ParameterNameNotFound, "parameter " + par.name +
		   " required for computing " + corr_ptr->name +
		   " was not found");
      }

    // Here we grouped in vtbl the vectors by temperatures. Now, for each
    // temperature value, we build a list of parallel vectors required
    // for computing corr_ptr

    DynList<DynList<const VectorDesc*>> vals;
    for (auto it = vtbl.get_it(); it.has_curr(); it.next())
      {
	auto & p = it.get_curr();
	const double & t = p.first;
	const DynList<const VectorDesc*> & vlist = p.second;
	const VectorDesc * fst_vptr = vlist.get_first();

	// now we test if remaining vector are parallel to the first one
	for (auto it = vlist.get_it(1); it.has_curr(); it.next())
	  {
	    auto curr_vptr = it.get_curr();
	    if (not curr_vptr->is_parallel(*fst_vptr))
	      {
		ostringstream s;
		s << "For temperature " << t << ": vector " << fst_vptr->yname
		  << " is not parallel to " << curr_vptr->yname;
		ZENTHROW(MetadataException, s.str());
	      }
	  }
	// here all the vectors of vlist are parallel. So we put them
	// in the result
	vals.append(move(vlist));
      }

    // Now we take src_vectors, which contains the vectors containing
    // the pressures and target_name references and we build the result
    DynList<CorrInput> ret;
    bool has_additional_vectors = false;
    for (auto it = src_vectors.get_it(); it.has_curr(); it.next())
      {
	const VectorDesc * vptr = it.get_curr();
	auto ptr = vtbl.search(vptr->t);
	if (ptr == nullptr)
	  ret.append(CorrInput(vptr, {}));
	else
	  {
	    has_additional_vectors = true;
	    ret.append(CorrInput(vptr,
				 ptr->second.maps<VectorDesc>([vptr] (auto yptr)
              {
		return vptr->merge_with(*yptr);
	      })));
	  }
      }
    
    if (ret.is_empty() and not is_static_target)
      ZENTHROW(MetadataException,
	       "It was not possible to build input data for " + corr_ptr->name);

    if (has_additional_vectors)
      return ret.filter([] (auto & c) { return not c.vlist.is_empty(); });

    return ret;
  }

  struct CorrOutput
  {
    const Correlation * corr_ptr = nullptr;
    DynList<double> t;
    DynList<DynList<double>> p;
    DynList<const Unit*> punits;
    DynList<DynList<double>> y;
    CorrOutput(const Correlation * __corr_ptr) : corr_ptr(__corr_ptr) {}
    friend ostream & operator << (ostream & out, const CorrOutput & co)
    {
      out << co.corr_ptr->name << endl;
      if (co.p.is_empty())
	for (auto it = zip_it(co.t, co.y); it.has_curr(); it.next())
	  {
	    auto t = it.get_curr();
	    out << "t = " << get<0>(t) << " degF y " << get<1>(t).get_first()
		<< " " << co.corr_ptr->unit.symbol << endl;
	  }
      else
	for (auto it = zip_it(co.t, co.p, co.punits, co.y); it.has_curr();
	     it.next())
	  {
	    auto t = it.get_curr();
	    out.precision(10);
	    out << "t = " << get<0>(t) << " degF (p in " << get<2>(t)->name
		<< " and y in " << co.corr_ptr->unit.name << ")" << endl
		<< ::to_string(format_string(zip_maps<DynList<string>>
					     ([] (auto t)
              {
		return build_dynlist<string>(::to_string(get<0>(t)),
					     ::to_string(get<1>(t), 10));
	      }, get<1>(t), get<3>(t)))) << endl;
	  }
      return out;
    }
  };

  ParList build_correlation_pars() const 
  {
    ParList pars(Dim_Pars_List);
    for (auto it = const_values.get_it(); it.has_curr(); it.next())
      {
	const ConstDesc & desc = it.get_curr();
	pars.insert(desc.name, desc.value, desc.unit_ptr);
      }
    return pars;
  }

  VtlQuantity compute_pb(double t, const Correlation * corr_ptr,
			 double c = 0, double m = 1) const
  {
    if (corr_ptr->target_name() != "pb")
      ZENTHROW(InvalidTargetName, "correlation " + corr_ptr->name +
	       " is not for pb");
    ParList pars = build_correlation_pars();
    pars.insert("t", t, &Fahrenheit::get_instance());
    return corr_ptr->tuned_compute_by_names(pars, c, m, false); 
  }

  ParPair get_pb(double t, double pb) const
  {
    if (pb_corr == nullptr)
      return ParPair("pb", ValPair(pb, &psia::get_instance()));
    VtlQuantity ret = compute_pb(t, pb_corr, c_pb, m_pb);
    return ParPair("pb", ValPair(ret.raw(), &ret.unit));
  }

  ParPair get_pb(const VectorDesc & v) const
  {
    return get_pb(v.t, v.pb);
  }

  VtlQuantity compute_uod(double t, double pb,  const Correlation * corr_ptr,
			 double c = 0, double m = 1) const
  {
    if (corr_ptr->target_name() != "uod")
      ZENTHROW(InvalidTargetName, "correlation " + corr_ptr->name +
	       " is not for uod");
    ParList pars = build_correlation_pars();
    pars.insert("t", t, &Fahrenheit::get_instance());
    pars.insert(get_pb(t, pb));
    return corr_ptr->tuned_compute_by_names(pars, c, m, false); 
  }

  struct ConstStats
  {
    const Correlation * corr_ptr = nullptr;
    DynList<double> t;
    DynList<double> ylab;
    DynList<double> ycorr;
    DynList<double> ytuned;
    CorrStat::Desc desc;
    bool valid = false;
    ConstStats(const Correlation * __corr_ptr, DynList<double> && __t,
	    DynList<double> && __ylab, DynList<double> && __ycorr)
      : corr_ptr(__corr_ptr), t(move(__t)), ylab(__ylab),
	ycorr(__ycorr), desc(CorrStat(move(__ylab)).stats(move(__ycorr))),
	valid(CorrStat::is_valid(desc))
    {
      assert(not t.is_empty());
      assert(not ylab.is_empty());
      assert(not ycorr.is_empty());
      if (ylab.is_unitarian())
	CorrStat::c(desc) = -CorrStat::c(desc);
      if (valid)
	{
	  const double & c = CorrStat::c(desc);
	  const double & m = CorrStat::m(desc);
	  ytuned = ycorr.maps([c, m] (auto y) { return c + m*y; });
	}
    }
    DynList<string> to_dynlist() const
    {
      return build_dynlist<string>(corr_ptr->name, ::to_string(CorrStat::r2(desc)),
				   ::to_string(CorrStat::mse(desc)),
				   ::to_string(CorrStat::sigma(desc)),
				   ::to_string(CorrStat::sumsq(desc)),
				   ::to_string(CorrStat::c(desc)),
				   ::to_string(CorrStat::m(desc)),
				   valid ? "true" : "false");
    }
  };

  ConstStats pb_stats(const Correlation * corr_ptr)
  {
    if (corr_ptr->target_name() != "pb")
      ZENTHROW(InvalidTargetName, "correlation " + corr_ptr->name +
	       " is not for pb");
    DynList<double> t, pb_lab, pb_corr;
    auto tp = tp_sets();
    size_t k = 0;
    for (auto it = tp.get_it(); it.has_curr(); it.next())
      {
	auto curr = it.get_curr();
	try
	  {
	    t.append(curr.t);
	    pb_lab.append(curr.pb);
	    pb_corr.append(compute_pb(curr.t, corr_ptr).raw());
	    ++k;
	  }
	catch (...) { /* ignore it */ }
      }

    if (k == 0)
      ZENTHROW(MetadataException, "cannot compute any pb value");

    return ConstStats(corr_ptr, move(t), move(pb_lab), move(pb_corr));
  }

  ConstStats uod_stats(const Correlation * corr_ptr)
  {
    if (corr_ptr->target_name() != "uod")
      ZENTHROW(InvalidTargetName, "correlation " + corr_ptr->name +
	       " is not for uod");
    DynList<double> t, pb_lab, pb_corr;
    auto tp = tp_sets();
    for (auto it = tp.get_it(); it.has_curr(); it.next())
      {
	auto curr = it.get_curr();
	t.append(curr.t);
	pb_lab.append(curr.pb);
	pb_corr.append(compute_uod(curr.t, curr.pb, corr_ptr).raw());
      }
    return ConstStats(corr_ptr, move(t), move(pb_lab), move(pb_corr));
  }

  ParPair get_uod(const VectorDesc & v, const ParPair & pb_par) const
  {
    if (uod_corr == nullptr)
      return ParPair("uod", ValPair(v.uod, &CP::get_instance()));
    ParList pars = build_correlation_pars();
    pars.insert("t", v.t, &Fahrenheit::get_instance());
    pars.insert(pb_par);
    VtlQuantity ret = uod_corr->tuned_compute_by_names(pars, c_uod, m_uod, false);
    return ParPair("uod", ValPair(ret.raw(), &ret.unit));
  }

private:

  void compute_prologue(const string & target_name, 
			const VectorDesc & pref,
			const Correlation * corr_ptr) const
  {
    if (pref.yname != target_name)
      ZENTHROW(InvalidTargetName, "compute_" + target_name +
	       "(): input vector is not for " + target_name);
    if (corr_ptr->target_name() != target_name)
      ZENTHROW(InvalidTargetName, "compute_" + target_name +
	       "rs(): correlation " + corr_ptr->name + " is not for " +
	       target_name);
    if (not pref.yunit->is_sibling(corr_ptr->unit))
      ZENTHROW(MetadataException, "yunit " + pref.yunit->name +
	       " is not sibling of " + corr_ptr->unit.name);
  }

public:

  VectorDesc compute_rs(const VectorDesc & pref,
			const Correlation * corr_ptr,
			double c = 0, double m = 1) const
  {
    compute_prologue("rs", pref, corr_ptr);

    const ConstDesc * rsb_ptr = search_const("rsb");
    if (rsb_ptr == nullptr)
      ZENTHROW(ConstNameNotFound, "rsb has not been defined");
    const double & rsb = rsb_ptr->value;
    ParList pars = build_correlation_pars();
    const ParPair pb_par = get_pb(pref);
    const double pb = pb_par.second.first;
    VectorDesc in = pref;
    in.p.get_last() = pb; // eventually change pb for the tuned one

    pars.insert("t", in.t, &Fahrenheit::get_instance());
    pars.insert(pb_par);
    VectorDesc ret(in.t, "rs");
    ret.pb = pb, ret.punit = in.punit, ret.yunit = &corr_ptr->unit;
    for (auto it = in.p.get_it(); it.has_curr(); it.next())
      {
	const double p = it.get_curr();
	pars.insert("p", p, in.punit);
	const VtlQuantity rs = corr_ptr->tuned_compute_by_names(pars, c, m, false);
	ret.p.append(p);
	ret.y.append(min(rs.raw(), rsb));
	pars.remove("p");
      }
    return ret;
  }

  struct VectorStats
  {
    struct Trio
    {
      DynList<double> p, ylab, ycorr, ytuned;
      Trio(DynList<double> && __p,
	   DynList<double> && __ylab, DynList<double> && __ycorr)
	: p(move(__p)), ylab(move(__ylab)), ycorr(move(__ycorr))
      {
	assert(not p.is_empty());
	assert(not ylab.is_empty());
	assert(not ycorr.is_empty());
	assert(equal_length(p, ylab, ycorr));
      }
      Trio() {}
    };
    DynMapTree<double, Trio> temps;
    const Correlation * corr_ptr = nullptr;
    mutable CorrStat::Desc desc;
    mutable bool valid = false, closed = false, dirty = false;
    VectorStats(const Correlation * __corr_ptr) : corr_ptr(__corr_ptr) {}
    void add(double t, DynList<double> && p,
	     DynList<double> && ylab, DynList<double> && ycorr)
    {
      if (closed)
	ZENTHROW(MetadataException, "VectorStats is already closed");
      if (not equal_length(p, ylab, ycorr))
	{
	  cout << corr_ptr->name << endl;
	  p.for_each([] (auto p) { cout << " " << p; }); cout << endl;
	  ylab.for_each([] (auto p) { cout << " " << p; }); cout << endl;
	  ycorr.for_each([] (auto p) { cout << " " << p; }); cout << endl;
	  dirty = true;
	}
      else
	temps.insert(t, Trio(move(p), move(ylab), move(ycorr)));
    }
    void close()
    {
      if (closed)
	ZENTHROW(MetadataException, "VectorStats is already closed");

      closed = true;

      if (temps.is_empty())
	return;

      DynList<double> p, ylab, ycorr;
      for (auto it = temps.get_it(); it.has_curr(); it.next())
	{
	  const Trio & pp = it.get_curr().second;
	  p.append(pp.p);
	  ylab.append(pp.ylab);
	  ycorr.append(pp.ycorr);
	}
      auto l = sort(t_zip(p, ylab, ycorr), [] (auto & t1, auto & t2)
		    { return get<0>(t1) < get<0>(t2); });
      auto t = t_unzip(l);
      // if (not dirty)
      // 	{
	  desc = CorrStat(get<1>(t)).stats(get<2>(t));
	  valid = CorrStat::is_valid(desc);
      // 	}
      // else
      // 	valid = false;
    }
    DynList<string> to_dynlist() const
    {
      if (not closed)
	ZENTHROW(MetadataException, "VectorStats is not closed");
      return build_dynlist<string>(corr_ptr->name, ::to_string(CorrStat::r2(desc)),
				   ::to_string(CorrStat::mse(desc)),
				   ::to_string(CorrStat::sigma(desc)),
				   ::to_string(CorrStat::sumsq(desc)),
				   ::to_string(CorrStat::c(desc)),
				   ::to_string(CorrStat::m(desc)),
				   valid ? "true" : "false");
    }
  };

private:

  DynList<const VectorDesc*> stats_prologue(const string & target_name,
					    const Correlation * corr_ptr) const
  {
    if (corr_ptr->target_name() != target_name)
      ZENTHROW(InvalidTargetName, "correlation " + corr_ptr->name +
	       " is not for " + target_name);

    DynList<const VectorDesc*> ret_lab = search_vectors(target_name);
    if (ret_lab.is_empty())
      ZENTHROW(VarNameNotFound, target_name +
	       " data is not found in this data set");
    return ret_lab;
  }

  VectorStats compute_stats
  (const DynList<const VectorDesc*> & lab_data,
   const Correlation * corr_ptr,
   VectorDesc (PvtData::*compute_ptr)(const VectorDesc & pref,
				      const Correlation * corr_ptr,
				      double c, double m) const) const
  {
    VectorStats stats(corr_ptr);
    size_t k = 0;
    for (auto it = lab_data.get_it(); it.has_curr(); it.next())
      {
	auto lab_ptr = it.get_curr();
	try
	  {
	    VectorDesc corr_data = (this->*compute_ptr)(*lab_ptr, corr_ptr, 0, 1);
	    DynList<double> p, ylab, ycorr;
	    for (auto lab_it = zip_it(lab_ptr->p, lab_ptr->y),
		   corr_it = zip_it(corr_data.p, corr_data.y);
		 lab_it.has_curr() and corr_it.has_curr();)
	      {
		auto tlab = lab_it.get_curr();
		auto tcorr = corr_it.get_curr();
		const double & p_lab = get<0>(tlab);
		const double & p_corr = get<0>(tcorr);
		if (p_lab < p_corr)
		  lab_it.next();
		else if (p_lab > p_corr)
		  corr_it.next();
		{
		  p.append(p_lab);
		  ylab.append(get<1>(tlab));
		  ycorr.append(get<1>(tcorr));
		  lab_it.next();
		  corr_it.next();
		}
	      }
	    // TODO aqui emparejar lab_ptr con corr_data
	    stats.add(corr_data.t, move(p), move(ylab), move(ycorr));
	    ++k;
	  }
	catch (...) { /* ignore it */ }
      }
    if (k == 0)
      ZENTHROW(MetadataException, "cannot compute any statistics for " +
	       corr_ptr->name);
    stats.close();
    return stats;
  }

  VectorStats & stats_epilogue(VectorStats & stats, double max_val) const
  {
    if (not stats.valid)
      return stats;
    
    const double & c = CorrStat::c(stats.desc);
    const double & m = CorrStat::m(stats.desc);
    const double & min_val = stats.corr_ptr->unit.min_val;
    for (auto it = stats.temps.get_it(); it.has_curr(); it.next())
      {
	auto & p = it.get_curr();
	p.second.ytuned =
	  p.second.ycorr.maps<double>([c, m, min_val, max_val] (auto val)
          {
	    return min(max(min_val, c + m*val), max_val);
	  });
      }
    return stats;
  }

  VectorStats & stats_epilogue(VectorStats & stats) const
  {
    const double max_val = stats.corr_ptr->unit.max_val;
    return stats_epilogue(stats, max_val);
  }

public:

  VectorStats rs_stats(const Correlation * corr_ptr) const
  {
    const DynList<const VectorDesc*> rs_lab = stats_prologue("rs", corr_ptr);
    VectorStats stats = compute_stats(rs_lab, corr_ptr, &PvtData::compute_rs);
    auto rsb_ptr = search_const("rsb");
    if (rsb_ptr == nullptr)
      ZENTHROW(ConstNameNotFound, "data set does not contain rsb");
    const double rsb = VtlQuantity(*rsb_ptr->unit_ptr, rsb_ptr->value).raw();

    return stats_epilogue(stats, rsb);
  }

  VectorDesc compute_bob(const VectorDesc & pref,
			 const Correlation * corr_ptr,
			 double c = 0, double m = 1) const
  {
    compute_prologue("bob", pref, corr_ptr);

    ParList pars = build_correlation_pars();
    const ParPair pb_par = get_pb(pref);
    const double pb = pb_par.second.first;
    VectorDesc in = pref;
    in.p.get_last() = pb; // eventually change pb for the tuned one

    pars.insert("t", in.t, &Fahrenheit::get_instance());
    pars.insert(pb_par);

    VectorDesc rs;
    bool exist_rs = false;
    auto rs_list = search_vectors("rs");
    if (not rs_list.is_empty())
      {
	auto rs_ptr =
	  rs_list.find_ptr([t = in.t] (auto ptr) { return ptr->t == t; });
	if (rs_ptr != nullptr)
	  {
	    exist_rs = true;
	    rs = **rs_ptr;
	  }
      }

    if (not exist_rs and rs_corr == nullptr)
      ZENTHROW(VarNameNotFound, "Not found rs vector neither correlation");

    if (exist_rs and rs_corr != nullptr)
      rs = compute_rs(rs, rs_corr, c_rs, m_rs);
    else if (rs_corr != nullptr)
      {
	VectorDesc fake_rs = in;
	fake_rs.yname = "rs"; // in this way compute_coa will accept it
	rs = compute_rs(fake_rs, rs_corr, c_rs, m_rs);
      }
    // else r is alredy defined
    rs.make_parallel(in);

    DynList<double> p_list, bob_list;
    for (auto it = zip_it(in.p, rs.y); it.has_curr(); it.next())
      {
	auto t = it.get_curr();
	const double & p = get<0>(t);
	const double & rs_val = get<1>(t);
	pars.insert("p", p, in.punit);
	pars.insert("rs", rs_val, rs.yunit);
	const VtlQuantity bob = corr_ptr->tuned_compute_by_names(pars, c, m, false);
	p_list.append(p);
	bob_list.append(bob.raw());
	pars.remove("p");
	pars.remove("rs");
      }

    return VectorDesc(in.t, pb, bob_list.get_last(), PVT_INVALID_VALUE,
		      PVT_INVALID_VALUE, p_list, in.punit, "bob", bob_list,
		      &corr_ptr->unit);
  }

  VectorStats bob_stats(const Correlation * corr_ptr) const
  {
    DynList<const VectorDesc*> bob_lab = stats_prologue("bob", corr_ptr);
    VectorStats stats = compute_stats(bob_lab, corr_ptr, &PvtData::compute_bob);
    return stats_epilogue(stats);
  }

  VectorDesc compute_coa(const VectorDesc & pref,
			 const Correlation * corr_ptr,
			 double c = 0, double m = 1) const
  {
    compute_prologue("coa", pref, corr_ptr);

    ParList pars = build_correlation_pars();
    const ParPair pb_par = get_pb(pref);
    const double pb = pb_par.second.first;

    pars.insert("t", pref.t, &Fahrenheit::get_instance());
    pars.insert(pb_par);

    DynList<double> p_list, coa_list;
    for (auto it = pref.p.get_it(); it.has_curr(); it.next())
      {
	const double p = it.get_curr();
	pars.insert("p", p, pref.punit);
	const VtlQuantity coa = corr_ptr->tuned_compute_by_names(pars, c, m, false);
	p_list.append(p);
	coa_list.append(coa.raw());
	pars.remove("p");
      }
    return VectorDesc(pref.t, pb, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		      PVT_INVALID_VALUE, p_list, pref.punit, "coa", coa_list,
		      &corr_ptr->unit);
  }

  VectorStats coa_stats(const Correlation * corr_ptr) const
  {
    DynList<const VectorDesc*> coa_lab = stats_prologue("coa", corr_ptr);
    VectorStats stats = compute_stats(coa_lab, corr_ptr, &PvtData::compute_coa);
    return stats_epilogue(stats);
  }

  // In this case we return a pair with the result and the used coa correlation
  VectorDesc compute_boa(const VectorDesc & pref, const Correlation * corr_ptr,
			 double c = 0, double m = 1) const
  {
    if (pref.yname != "boa")
      ZENTHROW(InvalidTargetName, "compute_boa(): input vector is not for boa");
    if (corr_ptr->target_name() != "boa")
      ZENTHROW(InvalidTargetName, "compute_boa(): correlation " + corr_ptr->name +
	       " is not for boa");

    VectorDesc coa;
    bool exist_coa = false;
    {
      const DynList<const VectorDesc*> coa_list = search_vectors("coa");
      if (not coa_list.is_empty())
	{
	  auto ptr =
	    coa_list.find_ptr([t = pref.t] (auto ptr) { return ptr->t == t; });
	  if (ptr != nullptr)
	    {
	      coa = **ptr;
	      exist_coa = true;
	    }
	}
    }
    if (not exist_coa and coa_corr == nullptr)
      ZENTHROW(VarNameNotFound,
	       "Not found experimental data neither correlation for coa");

    VectorDesc bob;
    bool exist_bob = false;
    {
      const DynList<const VectorDesc*> bob_list = search_vectors("bob");
      auto ptr =
	bob_list.find_ptr([t = pref.t] (auto ptr) { return ptr->t == t; });
      if (ptr != nullptr)
	{
	  bob = **ptr;
	  exist_bob = true;
	}
    }
    
    VectorDesc in = pref;
    ParList pars = build_correlation_pars();
    const ParPair pb_par = get_pb(pref);
    const double pb = pb_par.second.first;
    in.p.get_first() = nextafter(pb, numeric_limits<double>::max());

    if (exist_coa and coa_corr != nullptr)
      // in this case coa is already parallel to in
	coa = compute_coa(coa, coa_corr, c_coa, m_coa);
    else if (coa_corr != nullptr)
      {
	auto fake_coa = in;
	fake_coa.yname = "coa"; // in this way compute_coa will accept it
	coa = compute_coa(fake_coa, coa_corr, c_coa, m_coa);
      }
    // else coa is already defined
    coa = coa.make_parallel(in);

    if (exist_bob and bob_corr != nullptr)
      bob = compute_bob(bob, bob_corr, c_bob, m_bob).make_parallel(in);
    else if (bob_corr != nullptr)
      {
	auto fake_bob = in;
	fake_bob.yname = "bob";
	bob = compute_bob(fake_bob, bob_corr, c_bob, m_bob).make_parallel(in);
      }
    // else bob is already defined
    bob = bob.make_parallel(in);

    const double & bobp = bob.y.get_last();
    pars.insert("t", pref.t, &Fahrenheit::get_instance());
    pars.insert(pb_par);
    pars.insert("bobp", bobp, bob.yunit);
    
    DynList<double> p_list, boa_list;
    // Start from 1 (not 0) because bobp point must not be included
    for (auto it = zip_it_pos(1, in.p, coa.y); it.has_curr(); it.next())
      {
	auto t = it.get_curr();
	const double & p_val = get<0>(t);
	const double & coa_val = get<1>(t);
	pars.insert("p", p_val, pref.punit);
	pars.insert("coa", coa_val, coa.yunit);
	const VtlQuantity coa = corr_ptr->tuned_compute_by_names(pars, c, m, false);
	p_list.append(p_val);
	boa_list.append(coa.raw());
	pars.remove("p");
	pars.remove("coa");
      }
    return VectorDesc(pref.t, pb, bobp, PVT_INVALID_VALUE,
		      PVT_INVALID_VALUE, p_list, pref.punit, "boa",
		      boa_list, &corr_ptr->unit);
  }

  VectorStats boa_stats(const Correlation * corr_ptr) const
  {
    DynList<const VectorDesc*> boa_lab = stats_prologue("boa", corr_ptr);
    VectorStats stats = compute_stats(boa_lab, corr_ptr, &PvtData::compute_boa);
    return stats_epilogue(stats);
  }

  VectorDesc compute_uob(const VectorDesc & pref, const Correlation * corr_ptr,
			 double c = 0, double m = 1) const
  {
    compute_prologue("uob", pref, corr_ptr);

    VectorDesc rs;
    bool exist_rs = false;
    {
      const DynList<const VectorDesc*> rs_list = search_vectors("rs");
      if (not rs_list.is_empty())
	{
	  auto ptr =
	    rs_list.find_ptr([t = pref.t] (auto ptr) { return ptr->t == t; });
	  if (ptr != nullptr)
	    {
	      rs = **ptr;
	      exist_rs = true;
	    }
	}
    }
    if (not exist_rs and rs_corr == nullptr)
      ZENTHROW(VarNameNotFound,
	       "Not found experimental data neither correlation for rs");
    
    VectorDesc bob;
    bool exist_bob = false;
    {
      const DynList<const VectorDesc*> bob_list = search_vectors("bob");
      auto ptr =
	bob_list.find_ptr([t = pref.t] (auto ptr) { return ptr->t == t; });
      if (ptr != nullptr)
	{
	  bob = **ptr;
	  exist_bob = true;
	}
    }

    if (not exist_bob and bob_corr == nullptr)
      ZENTHROW(VarNameNotFound,
	       "Not found experimental data neither correlation for bob");

    VectorDesc in = pref;
    ParList pars = build_correlation_pars();
    const ParPair pb_par = get_pb(pref);
    const ParPair uod_par = get_uod(pref, pb_par);
    const double & pb = pb_par.second.first;
    const double & uod = uod_par.second.first;
    in.p.get_last() = pb;
    in.y.get_first() = in.uod = uod;

    if (exist_rs and rs_corr != nullptr)
      // in this case coa is already parallel to in
      rs = compute_rs(rs, rs_corr, c_rs, m_rs);
    else if (rs_corr != nullptr)
      {
	auto fake_rs = in;
	fake_rs.yname = "rs"; // in this way compute_coa will accept it
	rs = compute_rs(fake_rs, rs_corr, c_rs, m_rs);
      }
    // else rs is already defined
    rs = rs.make_parallel(in);

    if (exist_bob and bob_corr != nullptr)
      bob = compute_bob(bob, bob_corr, c_bob, m_bob).make_parallel(in);
    else if (bob_corr != nullptr)
      {
	auto fake_bob = in;
	fake_bob.yname = "bob";
	bob = compute_bob(fake_bob, bob_corr, c_bob, m_bob).make_parallel(in);
      }
    // else bob is already defined
    bob = bob.make_parallel(in);

    pars.insert("t", pref.t, &Fahrenheit::get_instance());
    pars.insert(pb_par);
    pars.insert(uod_par);

    DynList<double> p_list, uob_list;
    // Start from 1 (not 0) because bobp point must not be included
    for (auto it = zip_it(in.p, rs.y, bob.y); it.has_curr(); it.next())
      {
	auto t = it.get_curr();
	const double & p_val = get<0>(t);
	const double & rs_val = get<1>(t);
	const double & bob_val = get<2>(t);
	pars.insert("p", p_val, pref.punit);
	pars.insert("rs", rs_val, rs.yunit);
	pars.insert("bob", bob_val, bob.yunit);
	const VtlQuantity uob =
	  corr_ptr->tuned_compute_by_names(pars, c, m, false);
	p_list.append(p_val);
	uob_list.append(uob.raw());
	pars.remove("p");
	pars.remove("rs");
	pars.remove("bob");
      }
    return VectorDesc(pref.t, pb, PVT_INVALID_VALUE, uob_list.get_first(),
		      uob_list.get_last(), p_list, pref.punit, "uob",
		      uob_list, &corr_ptr->unit);
  }

  VectorStats uob_stats(const Correlation * corr_ptr) const
  {
    DynList<const VectorDesc*> uob_lab = stats_prologue("uob", corr_ptr);
    VectorStats stats = compute_stats(uob_lab, corr_ptr, &PvtData::compute_uob);
    return stats_epilogue(stats);
  }

  VectorDesc compute_uoa(const VectorDesc & pref, const Correlation * corr_ptr,
			 double c = 0, double m = 1) const
  {
    compute_prologue("uoa", pref, corr_ptr);

    ParList pars = build_correlation_pars();
    
    const ParPair pb_par = get_pb(pref);
    const double & pb = pb_par.second.first;
    VectorDesc in = pref;
    in.p.get_first() = nextafter(pb, numeric_limits<double>::max());

    const ParPair uod_par = get_uod(pref, pb_par);
    const double & uod = uod_par.second.first;
    in.y.get_first() = in.uod = uod;

    pars.insert("t", in.t, &Fahrenheit::get_instance());
    pars.insert(pb_par);
    pars.insert(uod_par);
    pars.insert("uobp", in.uobp, in.yunit);
    DynList<double> p_list, uoa_list;

    cout << in << endl;
    for (auto it = in.p.get_it(); it.has_curr(); it.next())
      {
	const double p = it.get_curr();
	pars.insert("p", p, in.punit);
	try
	  {
	    const VtlQuantity uoa =
	      corr_ptr->tuned_compute_by_names(pars, c, m, false);
	    p_list.append(p);
	    uoa_list.append(uoa.raw());
	  }
	catch (exception & e)
	  {
	    cout << corr_ptr->name << " " << e.what() << endl;
	  }
	pars.remove("p");
      }

    // thus we assure than uobp value es less that first uoa
    const double uobp = nextafter(uoa_list.get_first(),
				  numeric_limits<double>::min());

    // TODO: aquí hay un problema. uoa puede añadir valores, lo que
    // hace que no cuadre el arreglo de laboratorio
    return VectorDesc(in.t, pb, in.bobp, uod, uobp, p_list, in.punit,
		      "uoa", uoa_list, &corr_ptr->unit);
  }

  VectorStats uoa_stats(const Correlation * corr_ptr) const
  {
    DynList<const VectorDesc*> uoa_lab = stats_prologue("uoa", corr_ptr);
    VectorStats stats = compute_stats(uoa_lab, corr_ptr, &PvtData::compute_uoa);
    return stats_epilogue(stats);
  }

  VectorDesc compute_values(const VectorDesc & pref,
			    const DynList<VectorDesc> & vlist,
			    const Correlation * corr_ptr,
			    double c, double m,
			    ParList & parameters) const
  {
    parameters.insert("t", pref.t, &Fahrenheit::get_instance());
    auto pb_par = get_pb(pref);
    parameters.insert(pb_par);
    parameters.insert(get_uod(pref, pb_par));

    parameters.insert("bobp", pref.bobp, &RB_STB::get_instance());
    parameters.insert("uobp", pref.uobp, &CP::get_instance());
    DynList<ParPair> p_pars =
      pref.p.maps<ParPair>([u = pref.punit] (auto p)
			   {
			     return ParPair("p", ValPair(p, u));
			   });
    DynList<DynList<ParPair>> corr_inputs =
      vlist.maps<DynList<ParPair>>([] (auto & v) { return v.to_parpair(); });
    corr_inputs.insert(move(p_pars));
    corr_inputs = transpose(corr_inputs);
    // TODO si para cualquiera de estos target_name hay una
    // correlación definida ==> sustituir en la columna los
    // valores calculados por los calculados mediante la
    // correlación. Tiene que ser aquí porque
    // build_input_for_correlation retorna punteros a vectores, no
    // vectores en sí mismos
    DynList<double> y;
    for (auto it = corr_inputs.get_it(); it.has_curr(); it.next())
      {
	auto & pars = it.get_curr();
	// insert in parameters list each required dynamic parameter
	// TODO: igual aquí: si hay correlación caluclar por ella
	pars.for_each([&parameters] (auto & p) { parameters.insert(p); });
	VtlQuantity val =
	  corr_ptr->tuned_compute_by_names(parameters, c, m, false);
	y.append(val.raw());
	pars.for_each([&parameters] (auto & p) { parameters.remove(p); });
      }
    parameters.remove("bobp");
    parameters.remove("uobp");
    parameters.remove("pb");
    parameters.remove("uod");
    parameters.remove("t");

    const string target_name = corr_ptr->target_name();

    // TODO: cuadrar puntas bobp o uobo si target es bob, boba, uob, o uoa
    const double bobp = pref.bobp;
    const double uod = pref.uod;
    const double uobp = pref.uobp;

    return VectorDesc(pref.t, pref.pb, bobp, uod, uobp, pref.p, pref.punit,
		      corr_ptr->target_name(), y, &corr_ptr->unit);
  }

  

  CorrOutput compute_values(const Correlation * corr_ptr,
			    double c = 0, double m = 1) const
  {
    ParList parameters = { Dim_Pars_List };
    for (auto it = const_values.get_it(); it.has_curr(); it.next())
      {
	const ConstDesc & desc = it.get_curr();
	parameters.insert(desc.name, desc.value, desc.unit_ptr);
      }

    CorrOutput ret(corr_ptr);
    DynList<CorrInput> input = build_input_for_correlation(corr_ptr);
    DynList<double> y;
    for (auto it = input.get_it(); it.has_curr(); it.next())
      {
	const CorrInput in = it.get_curr();
	// TODO for each one of this properties compute it if a
	// correlation is defined 
	const bool dynamic_correlation = in.p_ref != nullptr;
	if (not dynamic_correlation)
	  {
	    parameters.insert("t", in.t, &Fahrenheit::get_instance());
	    parameters.insert("pb", in.pb, &psia::get_instance());
	    parameters.insert("uod", in.uod, &CP::get_instance());
	    VtlQuantity val =
	      corr_ptr->tuned_compute_by_names(parameters, c, m, false);
	    y.append(val.raw());
	    parameters.remove("pb");
	    parameters.remove("uod");
	    parameters.remove("t");
	  }
	else
	  {
	    VectorDesc r = compute_values(*in.p_ref, in.vlist, corr_ptr, c, m,
					  parameters);
	    ret.y.append(to_dynlist(r.y));
	    ret.p.append(to_dynlist(r.p));
	    ret.punits.append(r.punit);
	  }
	ret.t.append(in.t);
      }
    return ret;
  }

  // TODO inputing aquí

  DynList<VectorDesc>
  inputing(const DynList<const VectorDesc*> in_vectors,
	   const Correlation * corr_ptr)
  {
    DynList<VectorDesc> ret;



    return ret;
  }

  // TODO luego de aquí construir por cada propiedad build_rs_values
  // ... según cascada que tendria el grid

  /*
    given ref_vector which contains a property for several
    temperatures, this routine searches for target_name and returns
    the samples, probably interpolated, of the target_name property
    according to pressure values ​​stored in ref_vectors

    NOTE: THE MOST IMPORTANT OF THIS ROUTINE IS THE VECTOR OF
    PRESSURES. IT SHOULD BE USED VERY CAREFULLY IF THE Y VECTOR IS
    INTENDED TO BE USED AND, SINCE INTERPOLATION IS LIKELY TO GIVE
    WRONG VALUES
  */
  DynList<Sample> get_pars(const DynList<const VectorDesc*> & ref_vectors,
			   const string & target_name) const
  {
    DynList<Sample> ret;
    if (ref_vectors.is_empty())
      return ret;

    DynList<const VectorDesc*> samples = search_vectors(target_name);
    if (samples.is_empty())
      return ret;

    const VectorDesc * fst_vector = ref_vectors.get_first();

    auto ptr = samples.get_first();
    const Unit * punit = ptr->punit;
    const Unit * yunit = ptr->yunit;

    ret.append(Sample(ptr->t, ptr->pb, ptr->bobp, ptr->uod, ptr->uobp, 
		      to_dynlist(fst_vector->p), punit, target_name,
		      ptr->gety(fst_vector->p), yunit));

    for (auto it = zip_it_pos(1, samples, ref_vectors); it.has_curr(); it.next())
      {
	auto t = it.get_curr();
	const VectorDesc * sample_ptr = get<0>(t);
	const VectorDesc * vector_ptr = get<1>(t);
	DynList<double> p = to_dynlist(unit_convert(*vector_ptr->punit,
						    vector_ptr->p, *punit));
	DynList<double> y = unit_convert(*sample_ptr->yunit,
					 sample_ptr->gety(p), *yunit);
	ret.append(Sample(sample_ptr->t, ptr->pb, ptr->bobp, ptr->uod,
			  ptr->uobp, p, punit, target_name, y, yunit));
      }

    return ret;
  }

  // Return true if corr_ptr parameters matches with the data
  // contained in this set
  bool matches_with_pars(const Correlation * corr_ptr) const
  {
    return corr_ptr->get_preconditions().all([this] (auto & par)
      {
	return par.names().exists([this] (auto & p)
          {
	    return names.has(p.first);
	  });
      });
  }

  DynList<const Correlation*>
  matches_with_pars(const string & target_name) const
  {
    return Correlation::array().filter([this, &target_name] (auto corr_ptr)
      {
	return corr_ptr->target_name() == target_name and
	  this->matches_with_pars(corr_ptr);
      });
  }

  enum class Type { NotFound, Constant, Vector };

  //                   name  , true if it is a constant
  using ParType = pair<string, Type>;

  // Return true if corr_ptr can be applied; that is, if all values
  // required by the correlation re available and these match with
  // correlation development ranges
  bool can_be_applied(const Correlation * corr_ptr,
		      const DynSetTree<string> & relax_tbl,
		      const DynSetTree<const Correlation*> & ban_list) const
  {
    if (ban_list.has(corr_ptr))
      return false;

    static DynSetTree<string> no_pressure__properties = { "pb", "uod" };

    const string target_name = corr_ptr->target_name();
    DynList<const VectorDesc*> ref_vectors = search_vectors(target_name);    
    if (ref_vectors.is_empty() and not no_pressure__properties.has(target_name))
      return false;

    for (auto it = corr_ptr->get_preconditions().get_it(); it.has_curr();
	 it.next())
      {
	const CorrelationPar & par = it.get_curr();
	auto * ptr = par.names().find_ptr([this] (auto & p)
					  {
					    return names.has(p.first);
					  });
	if (ptr == nullptr)
	  return false; // this parameter is not in the data set

	const string & name_in_data = ptr->first;
	if (name_in_data == "t" or name_in_data == "p" or
	    name_in_data == "pb" or name_in_data == "uod" or
	    relax_tbl.contains(name_in_data) or
	    par.get_synonyms().exists([&relax_tbl] (auto & p)
				      { return relax_tbl.has(p.first); }))
	  continue;

	const ConstDesc * const_ptr = search_const(name_in_data);
	if (const_ptr) // is name_in_data as constant?
	  {
	    if ((not par.check(VtlQuantity(*const_ptr->unit_ptr,
					   const_ptr->value))))
	      return false;
	    continue;
	  }

	auto pars_list = get_pars(ref_vectors, name_in_data);
	if (not pars_list.all([&par] (auto & s)
             {
	       return s.yvals.all([&par, yunit = s.yunit] (auto v)
				{
				  return par.check(VtlQuantity(*yunit, v));
				});
	     }))
	  return false;
      }

    return true;
  }

  // Return a list of parameters avoiding application
  DynList<pair<string, bool>>
  list_restrictions(const Correlation * corr_ptr,
		    const DynSetTree<string> & relax_tbl) const
  {
    DynList<pair<string, bool>> ret;
    auto ref_vectors = search_vectors(corr_ptr->target_name());
    for (auto it = corr_ptr->get_preconditions().get_it(); it.has_curr();
	 it.next())
      {
	auto & par = it.get_curr();
	auto * ptr = par.names().find_ptr([this] (auto & p)
					  {
					    return names.has(p.first);
					  });
	if (ptr == nullptr)
	  {
	    ret.append(make_pair(par.name, false));
	    continue;
	  }

	const string & name_in_data = ptr->first;
	if (name_in_data == "t" or name_in_data == "p" or
	    name_in_data == "pb" or name_in_data == "uod" or
	    relax_tbl.contains(name_in_data) or
	    par.get_synonyms().exists([&relax_tbl] (auto & p)
				      { return relax_tbl.has(p.first); }))
	  continue;

	const ConstDesc * const_ptr = search_const(name_in_data);
	if (const_ptr) // is name_in_data as constant?
	  {
	    if ( (not par.check(VtlQuantity(*const_ptr->unit_ptr,
					    const_ptr->value))))
	      ret.append(make_pair(name_in_data, true));
	    continue;
	  }

	auto pars_list = get_pars(ref_vectors, name_in_data);
	if (not pars_list.all([&par] (auto & s)
              {
		return s.yvals.all([&par, yunit = s.yunit] (auto v)
				 {
				   return par.check(VtlQuantity(*yunit, v));
				 });
	      }))
	  ret.append(make_pair(name_in_data, true));
      }

    return ret;
  }

  DynList<pair<const Correlation*, DynList<pair<string, bool>>>>
  list_restrictions(const string & target_name,
		    const DynSetTree<string> & relax_tbl,
		    const DynSetTree<const Correlation*> & ban_list) const
  {
    auto corr_list =
      Correlation::array().filter([&target_name, &ban_list] (auto ptr)
      {
	return ptr->target_name() == target_name and not ban_list.has(ptr);
      });
    
    DynList<pair<const Correlation*, DynList<pair<string, bool>>>> ret;
    for (auto it = corr_list.get_it(); it.has_curr(); it.next())
      {
	auto corr_ptr = it.get_curr();
	auto restrictions = list_restrictions(corr_ptr, relax_tbl);
	if (not restrictions.is_empty())
	  ret.append(make_pair(corr_ptr, move(restrictions)));
      }

    return ret;
  }

  DynList<const Correlation*>
  can_be_applied(const string & target_name,
		 const DynSetTree<string> & relax_tbl = {},
		 const DynSetTree<const Correlation*> & ban_list = {}) const
  {
    if (not VectorDesc::valid_target_names.has(target_name))
      ZENTHROW(InvalidTargetName, target_name + " target name is not valid");
    
    return Correlation::array().
      filter([&target_name, &relax_tbl, &ban_list, this] (auto ptr)
      {
	return ptr->target_name() == target_name and
	  this->can_be_applied(ptr, relax_tbl, ban_list);
      });
  }

  void load_consts(const Correlation * corr_ptr, ParList & pars) const
  {
    DynList<Sample> samples;
    for (auto it = corr_ptr->get_preconditions().get_it(); it.has_curr();
	 it.next())
      {
	auto & par = it.get_curr();
	auto * ptr = par.names().find_ptr([this] (auto & p)
					  { return names.has(p.first); });
	if (ptr == nullptr)
	  ZENTHROW(VarNameNotFound, "correlation parameter " + par.name +
		   " not found in data set");

	const string & name_in_data = ptr->first;
	const ConstDesc * const_ptr = search_const(name_in_data);
	if (const_ptr)
	  pars.insert(par.name, const_ptr->value, const_ptr->unit_ptr);
      }
  }
  struct PbDesc
  {
    double t, pb_lab, pb_corr;
    PbDesc(double t, double pb_lab, double pb_corr)
      : t(t), pb_lab(pb_lab), pb_corr(pb_corr) {}
  };

    //       t     pb_lab, pb_corr
  DynList<PbDesc> pbapply(const Correlation * corr_ptr) const
  {
    if (corr_ptr->target_name() != "pb")
      ZENTHROW(InvalidTargetName, "correlation " + corr_ptr->name +
	       " is not for pb");

    DynList<TpSet> tpsets = tp_sets();
    ParList pars; // parameter to be passed to correlation corr_ptr
    load_consts(corr_ptr, pars);
    DynList<PbDesc> ret;
    for (auto it = tpsets.get_it(); it.has_curr(); it.next())
      {
	auto pp = it.get_curr();
	const double & t = pp.t;
	const double & pb_lab = pp.pb;
	pars.insert("t", t, &Fahrenheit::get_instance());
	auto result = corr_ptr->compute_by_names(pars, false);
	auto pb_ret = VtlQuantity(psia::get_instance(), result).raw();
	ret.append(PbDesc(t, pb_lab, pb_ret));
	pars.remove("t");
      }
    return ret;
  }

  struct UodDesc
  {
    double t, uod_lab, uod_corr;
    UodDesc(double t, double uod_lab, double uod_corr)
      : t(t), uod_lab(uod_lab), uod_corr(uod_corr) {}
  };  

  DynList<UodDesc> uodapply(const Correlation * corr_ptr) const
  {
    if (corr_ptr->target_name() != "uod")
      ZENTHROW(InvalidTargetName, "correlation " + corr_ptr->name +
	       " is not for uod");

    DynList<TpSet> tpsets = tp_sets();
    ParList pars; // parameter to be passed to correlation corr_ptr
    load_consts(corr_ptr, pars);
    DynList<UodDesc> ret;
    for (auto it = tpsets.get_it(); it.has_curr(); it.next())
      {
	auto pp = it.get_curr();
	const double & t = pp.t;
	const double & uod_lab = pp.uod;
	pars.insert("t", t, &Fahrenheit::get_instance());
	auto result = corr_ptr->compute_by_names(pars, false);
	auto uod_ret = VtlQuantity(CP::get_instance(), result).raw();
	ret.append(UodDesc(t, uod_lab, uod_ret));
	pars.remove("t");
      }
    return ret;
  }

  struct CorrDesc
  {
    double t, pb, bobp, uod, uobp;
    DynList<double> p, y, yc;
    CorrDesc(double t, double pb, double bobp, double uod, double uobp,
	     DynList<double> & p, DynList<double> & y, DynList<double> & yc)
      : t(t), pb(pb), bobp(bobp), uod(uod), uobp(uobp),
	p(move(p)), y(move(y)), yc(move(yc)) {}
  };
};

VectorDesc::VectorDesc(double t, double pb, double bobp,
		       double uod, double uobp,
		       const Array<double> & p, const Unit * punit,
		       const string & yname,
		       const Array<double> & y, const Unit * yunit)
  : t(t), pb(pb), bobp(bobp), uod(uod), uobp(uobp), p(p), punit(punit),
    yname(yname), yunit(yunit), y(y)
{
  if (t <= 0)
    ZENTHROW(InvalidRange, "t value " + ::to_string(t) +
	     " less or equal to zero");
  if (pb <= 0 and yname != "uo")
    ZENTHROW(InvalidRange, "pb value " + ::to_string(pb) +
	     " less or equal to zero");

  static auto validate_pb = [] (VectorDesc* ptr)
    {
      if (not BaseQuantity::is_valid(ptr->pb, *ptr->punit))
	ZENTHROW(OutOfUnitRange, "pb is out of unit range");
    };
  static auto validate_pb_in_p =
    [] (VectorDesc* ptr, const string & target_name)
    {
      if (not ptr->p.exists([pb = ptr->pb] (auto p) { return p == pb; }))
	ZENTHROW(PressureMismatch,
		 "VectorDesc ctor (" + target_name +
		 "): p vector does not contain pb");
    };
  static auto validate_bo = [] (VectorDesc* ptr)
    {
      if (not RB_STB::get_instance().is_sibling(*ptr->yunit))
	ZENTHROW(InvalidUnit, ptr->yunit->name + " is invalid for bo");
      if (not BaseQuantity::is_valid(ptr->bobp, *ptr->yunit))
	ZENTHROW(OutOfUnitRange, "bobp value is out of unit range");
      if (ptr->yunit != & RB_STB::get_instance())
	{
	  mutable_unit_convert(*ptr->yunit, ptr->y, RB_STB::get_instance());
	  ptr->bobp =
	    unit_convert(*ptr->yunit, ptr->bobp, RB_STB::get_instance());
	  ptr->yunit = &RB_STB::get_instance();
	}
      if (not ptr->y.all([u = ptr->yunit] (auto b)
			 { return BaseQuantity::is_valid(b, *u); }))
	ZENTHROW(OutOfUnitRange, "at least a bo value is out of its unit range");
    };
  static auto validate_uo = [] (VectorDesc* ptr)
    {
      if (not CP::get_instance().is_sibling(*ptr->yunit))
	ZENTHROW(InvalidUnit, ptr->yunit->name + " is invalid for uo");
      if (not BaseQuantity::is_valid(ptr->uobp, *ptr->yunit))
	ZENTHROW(OutOfUnitRange, "uobp value is out of unit range");
      if (not ptr->y.all([u = ptr->yunit] (auto b)
			 { return BaseQuantity::is_valid(b, *u); }))
	ZENTHROW(OutOfUnitRange, "at least an uo value is out of its unit range");
      if (ptr->yunit != &CP::get_instance())
	{
	  mutable_unit_convert(*ptr->yunit, ptr->y, CP::get_instance());
	  ptr->uobp = unit_convert(*ptr->yunit, ptr->uobp, CP::get_instance());
	  ptr->uod = unit_convert(*ptr->yunit, ptr->uod, CP::get_instance());
	  ptr->yunit = &CP::get_instance();
	}
    };    
  static auto pb_validate = [] (VectorDesc*)
    {
      ZENTHROW(InvalidTargetName,
	       "VectorDesc ctor (pb): pb cannot be a target name");
    };
  static auto rs_validate = [] (VectorDesc* ptr)
    {
      ptr->validate_sort();
      validate_pb(ptr);
      if (not SCF_STB::get_instance().is_sibling(*ptr->yunit))
	ZENTHROW(InvalidUnit, ptr->yunit->name + " is invalid for coa");
      if (ptr->p.get_last() != ptr->pb)
	ZENTHROW(PressureMismatch,
		 "VectorDesc ctor (rs): last value of p is not pb");
      if (not is_sorted(ptr->y))
	ZENTHROW(SamplesUnsorted,
		 "VectorDesc ctor: rs samples are not sorted");
      if (not ptr->y.all([u = ptr->yunit] (auto r)
			 { return BaseQuantity::is_valid(r, *u); }))
	ZENTHROW(OutOfUnitRange,
		 "at least a value of rs is out of its unit range");
    };
  static auto bob_validate = [] (VectorDesc* ptr)
    {
      ptr->validate_sort();
      validate_pb(ptr);
      validate_bo(ptr);
      if (not is_sorted(ptr->y))
	ZENTHROW(SamplesUnsorted,
		 "VectorDesc ctor: bob samples are sorted");
      if (ptr->p.exists([pb = ptr->pb] (auto p) { return p == pb; }))
	{
	  if (not ptr->y.exists([bobp = ptr->bobp] (auto v) { return v == bobp; }))
	    ZENTHROW(SampleInvalid,
		     "VectorDesc ctor: bob samples have not bobp");
	}
      else
	{
	  if (ptr->y.exists([bobp = ptr->bobp] (auto v) { return v == bobp; }))
	    ZENTHROW(SampleInvalid,
		     "VectorDesc ctor: bob samples have bobp but p not pb");
	  ptr->p.append(ptr->pb);
	  ptr->y.append(ptr->bobp);
	}
      if (ptr->pb < ptr->p.get_last())
	ZENTHROW(PressureMismatch,
		 "VectorDesc ctor (bob): pb value is less than last value of p");
      if (ptr->bobp < ptr->y.get_last())
	ZENTHROW(SampleInvalid,
		 "VectorDesc ctor: bobp is less than last value of bob");
    };
  static auto coa_validate = [] (VectorDesc* ptr)
    {
      validate_pb(ptr);
      if (not psia_1::get_instance().is_sibling(*ptr->yunit))
	ZENTHROW(InvalidUnit, ptr->yunit->name + " is invalid for uo");
      const size_t & n = ptr->p.size();
      if (n == 2*ptr->y.size())
	{
	  Array<double> pressures(n/2);
	  for (size_t i = 0, j = n/2; j < n; ++i, ++j)
	    pressures.append((ptr->p(i) + ptr->p(j))/2);
	  ptr->p.swap(pressures);
	}
      else if (n != ptr->y.size())
	ZENTHROW(LengthMismatch,
		 "pressure array must twice o same size than coa");
      
      ptr->validate_sort();
      if (not is_inversely_sorted(ptr->y))
	ZENTHROW(SamplesUnsorted,
		 "VectorDesc ctor: coa samples are not inversely sorted");
      if (not ptr->y.all([u = ptr->yunit] (auto c)
			 { return BaseQuantity::is_valid(c, *u); }))
	ZENTHROW(OutOfUnitRange,
		 "at least a coa value is out of its unit range");
    };
  static auto boa_validate = [] (VectorDesc* ptr)
    {
      ptr->validate_sort();
      validate_pb(ptr);
      validate_bo(ptr);
      if (not is_inversely_sorted(ptr->y))
	ZENTHROW(SamplesUnsorted,
		 "VectorDesc ctor: boa samples are not inversely sorted");
      if (ptr->p.exists([pb = ptr->pb] (auto p) { return p == pb; }))
	ZENTHROW(SampleInvalid, "p vector contains pb");
      if (ptr->y.exists([bobp = ptr->bobp] (auto v) { return v == bobp; }))
	ZENTHROW(SampleInvalid, "boa vector contains bobp");
      ptr->p.insert(nextafter(ptr->pb, numeric_limits<double>::max()));
      ptr->y.insert(nextafter(ptr->bobp, numeric_limits<double>::min()));
      if (ptr->p(0) >= ptr->p(1))
	ZENTHROW(PressureMismatch,
		 "VectorDesc ctor (boa): pb next is greater than first p value");
      if (ptr->y(0) < ptr->y(1))
	{
	  cout << ptr->y(0) << " < " << ptr->y(1) << endl;
	  exit(0);
	ZENTHROW(SampleInvalid,
		 "VectorDesc ctor (boa):bobp next is less than first boa value");
	}
      if (not ptr->y.all([u = ptr->yunit] (auto b)
			 { return BaseQuantity::is_valid(b, *u); }))
	ZENTHROW(OutOfUnitRange,
		 "at least a boa value is out of its unit range");
    };
  static auto uod_validate = [] (VectorDesc*)
    {
      ZENTHROW(InvalidTargetName,
	       "VectorDesc ctor (uod): uod cannot be a target name");
    };
  static auto uob_validate = [] (VectorDesc* ptr)
    {
      ptr->validate_sort();
      validate_pb(ptr);
      validate_pb_in_p(ptr, "uob");
      validate_uo(ptr);
      if (not is_inversely_sorted(ptr->y))
	ZENTHROW(SamplesUnsorted,
		 "VectorDesc ctor: uob samples are not inversely sorted");
      if (BaseQuantity::is_valid(ptr->uod, *ptr->yunit)) // was uod defined?
	{ // yes ==> validate that first entries correspond to it
	  if (ptr->y(0) != ptr->uod)
	    ZENTHROW(SampleInvalid,
		     "uod sample is not the first value of uob");
	     
	  if (ptr->p(0) != 0)
	    ZENTHROW(SampleInvalid, "first pressure value is not "
		     "zero given that uod value is present");
	} 
      else if (ptr->p(0) == 0) /// is uod inside uob vector?
	ptr->uod = ptr->y(0);
      else 
	{ // uod value is not inside this set ==> we compute it
	  LineEq l = { ptr->p(0), ptr->y(0), ptr->p(1), ptr->y(1) };
	  ptr->p.insert(0);
	  ptr->uod = l(0); // extrapolated to 0
	  ptr->y.insert(ptr->uod); 
	}
      if (ptr->p.get_first() != 0)
	ZENTHROW(SampleInvalid, "first pressure value is not zero");
      if (ptr->uod > ptr->y.get_first())
	ZENTHROW(SampleInvalid, "uod value is greater than first uob value");
      if (ptr->y.get_first() != ptr->uod)
	{
	  ptr->p.insert(0);
	  ptr->y.insert(ptr->uod);
	}
      if (ptr->y.get_last() != ptr->uobp)
	ZENTHROW(OutOfRange, "uobp is not equal than last value of uob");
    };
  static auto uoa_validate = [] (VectorDesc* ptr)
    {
      ptr->validate_sort();
      validate_pb(ptr);
      if (not BaseQuantity::is_valid(ptr->uod, *ptr->yunit))
	ZENTHROW(OutOfUnitRange, "uod value is out of its unit range");
      if (ptr->p.get_first() < ptr->pb)
	ZENTHROW(SampleInvalid, "first p value is greater or equal that pb");
      if (ptr->uobp >= ptr->y.get_first())
	ZENTHROW(SampleInvalid, "uobp es greater or equal that first uoa value");
      validate_uo(ptr);
      if (not is_sorted(ptr->y))
	ZENTHROW(SamplesUnsorted,
		 "VectorDesc ctor: uob samples are not sorted");
      if (ptr->y.get_first() != ptr->uobp)
	{
	  auto vmax = numeric_limits<double>::max();
	  ptr->p.insert(nextafter(ptr->pb, vmax));
	  ptr->y.insert(nextafter(ptr->uobp, vmax));
	}
      cout << "ptr" << endl
      << *ptr << endl;
    };
  static auto uo_validate = [] (VectorDesc* ptr)
    {
      ptr->validate_sort();
      if (not ptr->yunit->is_sibling(CP::get_instance()))
	ZENTHROW(InvalidUnit, ptr->yunit->name + " is not sibling of CP");
      if (not ptr->y.all([u = ptr->yunit] (auto uo)
			 { return BaseQuantity::is_valid(uo, *u); }))
	ZENTHROW(OutOfUnitRange,
		 "at least an uo value is out of its unit range");
      if (ptr->yunit != &CP::get_instance())
	{
	  mutable_unit_convert(*ptr->yunit, ptr->y, CP::get_instance());
	  ptr->yunit = &CP::get_instance();
	}
    };
    
  static AHDispatcher<string, void (*)(VectorDesc*)> dispather =
    { "pb", pb_validate, "rs", rs_validate, "bob", bob_validate,
      "coa", coa_validate, "boa", boa_validate, "uod", uod_validate,
      "uob", uob_validate, "uoa", uoa_validate, "uo", uo_validate };

  pre_validate();
  dispather.run(yname, this);
  post_validate(); // this method must be here, after the dispatcher!
}


# endif
