/** This file contains all the machinery needed for tuning pb, rs,
   bob, coa, boa, uod, uob and uoa
   
   Aleph-w Leandro Rabindranath Leon
 */
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
# include "ttuner-units.H"

using Json = nlohmann::json;

# define PVT_INVALID_VALUE UNIT_INVALID_VALUE

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

  DynList<Quantity<psig>> get_pressures() const
  {
    return p.maps<Quantity<psig>>([this] (auto v)
		  { return Quantity<psig>(VtlQuantity(*punit, v)); });
  }

  DynList<VtlQuantity> get_y() const
  {
    return y.maps<VtlQuantity>([this] (auto v)
			       { return VtlQuantity(*yunit, v); });
  } 

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
      is_sorted(p, std::less_equal<double>());
  }

  void validate_sort()
  {
    if (not is_sorted(p, std::less_equal<double>()))
      {
	p.reverse();
	y.reverse();
      }
    if (not is_sorted(p, std::less_equal<double>()))
      ZENTHROW(SamplesUnsorted, ctor_msg() + "pressures are not sorted " +
	       join(p));
  }

  void pre_validate()
  {
    if (punit == nullptr or yunit == nullptr)
      ZENTHROW(InvalidUnit, ctor_msg() + "vector unit are not initialized");
    if (not psig::get_instance().is_sibling(*punit))
      ZENTHROW(InvalidUnit, punit->name + " is not a valid pressure unit");
    if (p.size() <= 1)
      ZENTHROW(LengthMismatch, ctor_msg() +
	       "pressure array size is less or equal than 1");
    if (not p.all([this] (auto p) { return BaseQuantity::is_valid(p, *punit); }))
      ZENTHROW(OutOfUnitRange, ctor_msg() + 
	       "at least a pressure value is out of its unit range " +
	       join(p));
    if (punit != &psig::get_instance())
      {
	mutable_unit_convert(*punit, p, psig::get_instance());
	pb = unit_convert(*punit, pb, psig::get_instance());
	punit = &psig::get_instance();
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

  DynList<DynList<string>> to_dynlist(size_t precision = 17) const
  {
    DynList<DynList<string>> ret =
      zip_maps<DynList<string>>([precision] (auto t)
      {
	return build_dynlist<string>(::to_string(get<0>(t)),
				     ::to_string(get<1>(t), precision));
			       
      }, p, y);
    ret.insert(build_dynlist<string>("p " + punit->name,
				     yname + " " + yunit->name));
    return ret;
  }

  string to_string() const
  {
    ostringstream s;
    s << "For t = " << t << " " << Fahrenheit::get_instance().name;
    if (pb >= 0)
      s << " pb = " << pb << " " << punit->name;
    if (uod >= 0)
      s << " uod = " << uod << " " << CP::get_instance().name;
    if (bobp >= 0)
      s << " bobp = " << bobp << " " << RB_STB::get_instance().name;
    if (uobp >= 0)
      s << " uobp = " << uobp << " " << CP::get_instance().name;
    s << " :" << endl
      << shift_lines_to_left(Aleph::to_string(format_string(to_dynlist())), 2);
    return s.str();
  }

  friend ostream & operator << (ostream & out, const VectorDesc & d)
  {
    return out << d.to_string();
  }

  string py_string() const
  {
    return join(zip_maps<string>([] (auto t) { return "(" + ::to_string(get<0>(t))
	    + ", " + ::to_string(get<1>(t)) + ")"; }, p, y), ", ");
  }

  string ctor_msg() const { return yname + "(" + ::to_string(t) + "): "; }

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

    if (not is_sorted(p, std::less_equal<double>()))
      ZENTHROW(SamplesUnsorted, "split_uo(): pressure values are not sorted " +
	       join(p));

    // First, we must determine (p,uo) corresponding to the inflection point
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
    
    Array<double> p_below = p.take(min_idx);
    Array<double> uob = y.take(min_idx);
    if (not is_inversely_sorted(uob, std::less_equal<double>()))
      ZENTHROW(SamplesUnsorted, "saturated region of uo is not decreasing " +
	       py_string());

    Array<double> p_above, uoa;
    for (size_t i = min_idx; i < p.size(); ++i)
      {
	p_above.append(p(i));
	uoa.append(y(i));
      }

    // Not to forget: minimum point is at p_above(0), uoa(0)

    if (not is_sorted(uoa, std::less_equal<double>()))
      ZENTHROW(SamplesUnsorted, "undersaturated region of uo is not increasing " +
	       py_string());

    if (p_below.size() < 2)
      ZENTHROW(CommandLineError, "Not enough point for estimating uob");

    if (p_above.size() < 2)
      ZENTHROW(CommandLineError, "Not enough point for estimating uoa");

    // In this point (p_below,uob) contains the saturated uo and
    // (p_above, uoa) the undersaturated one.

    // First step we compute uod by extrapolating the two first points
    // of p_below,uob to the cut point p = 0
    const double uod = extrapolate_left(p_below(0), p_below(1),
					uob(0), uob(1), 0);

    // Now we build two lines; 

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
    if (uobp < 0 or pb <= p_below.get_last() or pb >= p_above.get_first())
      { // These are improbable but possible cases that would
	// correspond to a bad model
	pb = p_below.get_last();
	uobp = uob.get_last();
      }

    p_below.append(pb); // (pb,uod) at the end of the saturated
                        // zone for better precision 
    uob.append(uobp);

    // (0,uod) at the beginning of the saturated zone for better precision  
    p_below.insert(VtlQuantity(*punit, Quantity<psig>(0)).raw());
    uob.insert(uod);

    const auto vmax = numeric_limits<double>::max();
    p_above.insert(nextafter(pb, vmax));
    uoa.insert(nextafter(uobp, vmax));

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
# define Define_Get_Min_UO(name, targets...)	\
  static Quantity<CP> name()			\
  {						\
    static size_t count = 0;				\
    static Quantity<CP> ret = CP::get_instance().min();	\
							\
    init_correlations();						\
    if (count++ > 0) /* if has been already executed then result is in ret */ \
      return ret;							\
									\
    auto uo_corr_list = Correlation::array().filter([] (auto corr_ptr)	\
    {			                                                \
      assert(corr_ptr);							\
      return corr_ptr->min_from_author and				\
      is_inside(corr_ptr->target_name(), {targets});			\
    });									\
									\
    if (uo_corr_list.is_empty())					\
      return ret;							\
									\
    ret = uo_corr_list.foldl(CP::get_instance().max(), [] (auto m, auto ptr)\
      {                                                                 \
      	return min(m, VtlQuantity(CP::get_instance(),			\
      				  VtlQuantity(ptr->unit, ptr->min_val))); \
      });								\
									\
    return ret;								\
  }

  Define_Get_Min_UO(min_uo_val, "uod", "uob", "uoa");
  Define_Get_Min_UO(min_uod_val, "uod");

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

    if (not is_sorted(v.p, std::less_equal<double>()))
      ZENTHROW(SamplesUnsorted, "pressure values are not sorted " + join(v.p));

    auto sibling = search_vectors(v.yname);
    if (not sibling.is_empty())
      {
	auto fst = sibling.get_first();
	if (not sibling.all([&v] (auto & p)
			    { return p->yunit->is_sibling(*v.yunit); }))
	  ZENTHROW(InvalidTargetUnit, v.yunit->name + " is not sibling of " +
		   fst->yunit->name);
	if (v.yunit != fst->yunit)
	  mutable_unit_convert(*v.yunit, v.y, *fst->yunit);
      }

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

  PvtData()
  {
    set_ttuner_units();
  }

  PvtData(istream & input)
  {
    set_ttuner_units(); 
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

# define SET_CORRRELATION(tgt_name)					\
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
						      psig::get_instance()),
					 unit_convert(*vdesc.yunit, vdesc.uod,
						      CP::get_instance())));
	  else
	    tuples.insert(vdesc.t,
			  TpSet(vdesc.t, unit_convert(*vdesc.punit, vdesc.pb,
						      psig::get_instance()),
				PVT_INVALID_VALUE));
	else
  	  {
  	    const double p = unit_convert(*vdesc.punit, vdesc.pb,
  					  psig::get_instance());
	    TpSet & t = ptr->second;
  	    if (t.pb > 0 and not are_near(t.pb, p, 1e-5))
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
		       maps<double>([] (auto & t) { return t.pb; }),
		       std::less_equal<double>()))
       ZENTHROW(PressureMismatch,
		"pb values are not sorted through temperatures " +
		join(tuples.values().maps<string>
		     ([] (auto & t) { return "(" + ::to_string(t.t)
			 + "," + ::to_string(t.pb); }), ", "));

    return tuples.maps<TpSet>([] (auto & p) { return p.second; });
  }

  size_t num_temps() const { return tp_sets().size(); }

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

private:
  
  static const DynSetTree<string> var_pars;

public:

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

  // compute pb but assures that unit is psig
  VtlQuantity compute_pb(double t, const Correlation * corr_ptr,
			 double c = 0, double m = 1) const
  {
    if (corr_ptr->target_name() != "pb")
      ZENTHROW(InvalidTargetName, "correlation " + corr_ptr->name +
	       " is not for pb");
    ParList pars = build_correlation_pars();
    pars.insert("t", t, &Fahrenheit::get_instance());
    return VtlQuantity(psig::get_instance(), 
		       corr_ptr->tuned_compute_by_names(pars, c, m,
							psig::get_instance(),
							false));
  }

  ParPair get_pb(double t, double pb) const
  {
    if (pb_corr == nullptr)
      return ParPair("pb", ValPair(pb, &psig::get_instance()));
    VtlQuantity ret = compute_pb(t, pb_corr, c_pb, m_pb);
    assert(&ret.unit == &psig::get_instance());
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
    VtlQuantity ret = corr_ptr->bounded_tuned_compute_by_names
      (pars, min_uo_val(), CP::get_instance().max(), c, m, CP::get_instance(),
       false);
    return ret;
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
	       DynList<double> && __ylab, DynList<double> && __ycorr,
	       const double min_val, const double max_val)
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
	  ytuned = ycorr.maps([c, m, &min_val, &max_val] (auto y)
            {
	      return min(max(min_val, c + m*y), max_val);
	    });
	}
    }
    DynList<string> to_dynlist(size_t precision = 10) const
    {
      return build_dynlist<string>
	(corr_ptr->name, ::to_string(CorrStat::r2(desc)),
	 ::to_string(CorrStat::mse(desc)),
	 ::to_string(CorrStat::sigma(desc)),
	 ::to_string(CorrStat::sumsq(desc)),
	 ::to_string(CorrStat::c(desc), precision),
	 ::to_string(CorrStat::m(desc)),
	 valid ? "true" : "false");
    }
  };

  ConstStats pb_stats(const Correlation * corr_ptr) const
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

    return ConstStats(corr_ptr, move(t), move(pb_lab), move(pb_corr),
		      psig::get_instance().min_val,
		      psig::get_instance().max_val);
  }

  ConstStats uod_stats(const Correlation * corr_ptr) const
  {
    if (corr_ptr->target_name() != "uod")
      ZENTHROW(InvalidTargetName, "correlation " + corr_ptr->name +
	       " is not for uod");
    DynList<double> t, uod_lab, uod_corr;
    auto tp = tp_sets();
    for (auto it = tp.get_it(); it.has_curr(); it.next())
      {
	auto curr = it.get_curr();
	if (not BaseQuantity::is_valid(curr.uod, CP::get_instance()))
	  continue;
	t.append(curr.t);
	uod_lab.append(curr.uod);
	uod_corr.append(compute_uod(curr.t, curr.pb, corr_ptr).raw());
      }
    if (t.is_empty())
      ZENTHROW(VarNameNotFound, "not found data for uod");
    return ConstStats(corr_ptr, move(t), move(uod_lab), move(uod_corr),
		      min_uod_val().raw(), CP::get_instance().max_val);
  }

  ParPair get_uod(const VectorDesc & v, const ParPair & pb_par) const
  {
    if (uod_corr == nullptr)
      return ParPair("uod", ValPair(v.uod, &CP::get_instance()));
    ParList pars = build_correlation_pars();
    pars.insert("t", v.t, &Fahrenheit::get_instance());
    pars.insert(pb_par);
    VtlQuantity ret =
      uod_corr->bounded_tuned_compute_by_names(pars, min_uod_val(),
					       CP::get_instance().max(),
					       c_uod, m_uod, CP::get_instance());
      // uod_corr->tuned_compute_by_names(pars, c_uod, m_uod,
      // 				       CP::get_instance(), false);
    return ParPair("uod", ValPair(ret.raw(), &ret.unit));
  }

private:

  void compute_prologue(const string & target_name, 
			const VectorDesc & pref,
			const Correlation * corr_ptr) const
  {
    assert(pref.punit == &psig::get_instance());
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

  VectorDesc & compute_epilogue(const VectorDesc & pref,
				VectorDesc & result) const
  {
    if (pref.yunit != result.yunit)
      mutable_unit_convert(*result.yunit, result.y, *pref.yunit);
    return result;
  }

  VtlQuantity get_rsb() const
  {
    const ConstDesc * rsb_ptr = search_const("rsb");
    if (rsb_ptr == nullptr)
      ZENTHROW(ConstNameNotFound, "rsb has not been defined");
    return
      VtlQuantity(*rs_unit, VtlQuantity(*rsb_ptr->unit_ptr, rsb_ptr->value));
  }

public:

  VectorDesc compute_rs(const VectorDesc & pref,
			const Correlation * corr_ptr,
			double c = 0, double m = 1) const
  {
    compute_prologue("rs", pref, corr_ptr);

    VectorDesc in = pref;
    const double rsb = get_rsb().raw();
    ParList pars = build_correlation_pars();
    const ParPair pb_par = get_pb(pref);
    const double pb = pb_par.second.first;
    in.p.get_last() = pb;
    for (long i = in.p.size() - 2; i >= 0 and in.p(i) > pb; --i)
      in.p(i) = nextafter(in.p(i + 1), numeric_limits<double>::min());

    pars.insert("t", in.t, &Fahrenheit::get_instance());
    pars.insert(pb_par);
    DynList<double> p_list, rs_list;
    for (auto it = in.p.get_it(); it.has_curr(); it.next())
      {
	const double p = it.get_curr();
	pars.insert("p", p, in.punit);
	const VtlQuantity rs =
	  corr_ptr->tuned_compute_by_names(pars, c, m, *in.yunit, false);
	p_list.append(p);
	rs_list.append(min(rs.raw(), rsb));
	pars.remove("p");
      }
    VectorDesc result(in.t, pb, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		      PVT_INVALID_VALUE, p_list, in.punit, "rs", rs_list,
		      in.yunit);
    return compute_epilogue(pref, result);
  }

  struct VectorStats
  {
    struct Trio
    {
      double t = PVT_INVALID_VALUE;
      DynList<double> plab, p, ylab, ycorr, ytuned;
      Trio(double __t, DynList<double> && __plab, DynList<double> && __p,
	   DynList<double> && __ylab, DynList<double> && __ycorr)
	: t(__t), plab(move(__plab)), p(move(__p)), ylab(move(__ylab)),
	  ycorr(move(__ycorr))
      {
	assert(not plab.is_empty());
	assert(not p.is_empty());
	assert(not ylab.is_empty());
	assert(not ycorr.is_empty());
	assert(equal_length(plab, p, ylab, ycorr));
      }
      Trio() {}
    };
    DynMapTree<double, Trio> temps;
    const Correlation * corr_ptr = nullptr;
    mutable CorrStat::Desc desc;
    mutable bool valid = false, closed = false, dirty = false;
    VectorStats(const Correlation * __corr_ptr) : corr_ptr(__corr_ptr) {}
    void add(double t, DynList<double> && plab, DynList<double> && p,
	     DynList<double> && ylab, DynList<double> && ycorr)
    {
      if (closed)
	ZENTHROW(MetadataException, "VectorStats is already closed");
      if (not equal_length(plab, p, ylab, ycorr))
	dirty = true;
      else
	temps.insert(t, Trio(t, move(plab), move(p), move(ylab), move(ycorr)));
    }
    void close()
    {
      if (closed)
	ZENTHROW(MetadataException, "VectorStats is already closed");

      closed = true;

      if (temps.is_empty())
	{
	  valid = false;
	  return;
	}

      DynList<double> plab, p, ylab, ycorr;
      for (auto it = temps.get_it(); it.has_curr(); it.next())
	{
	  const Trio & pp = it.get_curr().second;
	  plab.append(pp.plab);
	  p.append(pp.p);
	  ylab.append(pp.ylab);
	  ycorr.append(pp.ycorr);
	}

      // order in x axis for regression is p array
      auto l = sort(t_zip(plab, p, ylab, ycorr), [] (auto & t1, auto & t2)
		    { return get<1>(t1) < get<1>(t2); });
      auto t = t_unzip(l);
      desc = CorrStat(get<2>(t)).stats(get<3>(t)); // here regression is done!
      valid = CorrStat::is_valid(desc);
    }
    DynList<string> to_dynlist(size_t precision = 10) const
    {
      if (not closed)
	ZENTHROW(MetadataException, "VectorStats is not closed");
      return build_dynlist<string>(corr_ptr->name, ::to_string(CorrStat::r2(desc)),
				   ::to_string(CorrStat::mse(desc)),
				   ::to_string(CorrStat::sigma(desc)),
				   ::to_string(CorrStat::sumsq(desc)),
				   ::to_string(CorrStat::c(desc), precision),
				   ::to_string(CorrStat::m(desc), precision),
				   valid ? "true" : "false");
    }

    // return a matrix of t, p, ylab, ycorr, ytuned
    DynList<DynList<double>> to_matrix()
    {
      DynList<DynList<double>> ret;
      for (auto it = temps.get_it(); it.has_curr(); it.next())
        {
	  auto & p = it.get_curr();
	  const Trio & q = p.second;
	  DynList<DynList<double>> m =
	    zip_maps<DynList<double>>([temp = q.t] (auto t)
            {
	      return build_dynlist<double>(temp, get<0>(t), get<1>(t), get<2>(t),
					   get<3>(t), get<4>(t));
	    }, q.plab, q.p, q.ylab, q.ycorr, q.ytuned);
	  ret.append(move(m));
	}
      return ret;
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
   VectorDesc (PvtData::*compute_ptr)(const VectorDesc &, const Correlation*,
				      double, double) const) const
  {
    VectorStats stats(corr_ptr);
    size_t k = 0;
    for (auto it = lab_data.get_it(); it.has_curr(); it.next())
      {
	auto lab_ptr = it.get_curr();
	try
	  {
	    const VectorDesc corr_data =
	      (this->*compute_ptr)(*lab_ptr, corr_ptr, 0, 1);
	    // TODO hacer paralelo con lab_data
	    stats.add(corr_data.t, to_dynlist(lab_ptr->p),
		      to_dynlist(corr_data.p), to_dynlist(lab_ptr->y),
		      to_dynlist(corr_data.y));
	    ++k;
	  }
	catch (exception & e)
	  {
	    cout << corr_ptr->name << ": " << e.what() << endl;
	  }
      }
    stats.close();
    return stats;
  }

  VectorStats & stats_epilogue(VectorStats & stats,
			       double min_val, double max_val) const
  {
    if (not stats.valid)
      return stats;

    const double & c = CorrStat::c(stats.desc);
    const double & m = CorrStat::m(stats.desc);
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
    const double min_val = stats.corr_ptr->unit.min_val;
    const double max_val = stats.corr_ptr->unit.max_val;
    return stats_epilogue(stats, min_val, max_val);
  }

public:

  VectorStats rs_stats(const Correlation * corr_ptr) const
  {
    const DynList<const VectorDesc*> rs_lab = stats_prologue("rs", corr_ptr);
    VectorStats stats = compute_stats(rs_lab, corr_ptr, &PvtData::compute_rs);
    return stats_epilogue(stats, 0, get_rsb().raw());
  }

  VectorDesc compute_bob(const VectorDesc & pref,
			 const Correlation * corr_ptr,
			 double c = 0, double m = 1) const
  {
    compute_prologue("bob", pref, corr_ptr);

    ParList pars = build_correlation_pars();
    VectorDesc in = pref;
    const ParPair pb_par = get_pb(pref);
    const double pb = pb_par.second.first;
    in.p.get_last() = pb;
    for (long i = in.p.size() - 2; i >= 0 and in.p(i) > pb; --i)
      in.p(i) = nextafter(in.p(i + 1), numeric_limits<double>::min());

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
	fake_rs.yunit = &rs_corr->unit;
	rs = compute_rs(fake_rs, rs_corr, c_rs, m_rs);
      } // else r is already defined
    rs.make_parallel(in);

    DynList<double> p_list, bob_list;
    for (auto it = zip_it(in.p, rs.y); it.has_curr(); it.next())
      {
	auto t = it.get_curr();
	const double & p = get<0>(t);
	const double & rs_val = get<1>(t);
	pars.insert("p", p, in.punit);
	pars.insert("rs", rs_val, rs.yunit);
	const VtlQuantity bob =
	  corr_ptr->tuned_compute_by_names(pars, c, m, *in.yunit, false);
	p_list.append(p);
	bob_list.append(bob.raw());
	pars.remove("p");
	pars.remove("rs");
      }

    VectorDesc result(in.t, pb, bob_list.get_last(), PVT_INVALID_VALUE,
		      PVT_INVALID_VALUE, p_list, in.punit, "bob", bob_list,
		      &corr_ptr->unit);
    return compute_epilogue(pref, result);
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
    ParPair pb_par = get_pb(pref);
    double pb = pb_par.second.first;

    VectorDesc in = pref;
    in.pb = pb;
    if (pb >= in.p.get_first())
      {
	in.p.get_first() = nextafter(pb, numeric_limits<double>::max());
	for (size_t i = 1; i < in.p.size() and in.p(i) <= pb; ++i)
	  in.p(i) = nextafter(in.p(i - 1), numeric_limits<double>::max());
      }

    pars.insert("t", pref.t, &Fahrenheit::get_instance());
    pars.insert(pb_par);

    DynList<double> p_list, coa_list;
    for (auto it = in.p.get_it(); it.has_curr(); it.next())
      {
	const double p = it.get_curr();
	pars.insert("p", p, pref.punit);
	const VtlQuantity coa =
	  corr_ptr->tuned_compute_by_names(pars, c, m, *pref.yunit, false);
	p_list.append(p);
	coa_list.append(coa.raw());
	pars.remove("p");
      }

    VectorDesc result(pref.t, pb, PVT_INVALID_VALUE, PVT_INVALID_VALUE,
		      PVT_INVALID_VALUE, p_list, pref.punit, "coa", coa_list,
		      &corr_ptr->unit);
    return compute_epilogue(pref, result);
  }

  VectorStats coa_stats(const Correlation * corr_ptr) const
  {
    DynList<const VectorDesc*> coa_lab = stats_prologue("coa", corr_ptr);
    VectorStats stats = compute_stats(coa_lab, corr_ptr, &PvtData::compute_coa);
    return stats_epilogue(stats);
  }

  VtlQuantity compute_bobp(const double t) const
  {
    const DynList<const VectorDesc*> bob_vectors = search_vectors("bob");
    auto ptr = bob_vectors.find_ptr([t] (auto vptr) { return vptr->t == t; });
    if (ptr == nullptr)
      ZENTHROW(VarNameNotFound, "Not found data for bob");
    const VectorDesc * v_ptr = *ptr;
    if (bob_corr == nullptr)
      return VtlQuantity(*v_ptr->yunit, v_ptr->bobp);
    
    ParPair pb_par = get_pb(v_ptr->t, v_ptr->pb);
    ParList pars = build_correlation_pars();
    insert_in_container(pars, pb_par, make_par("rs", get_rsb()),
			ParPair("t", ValPair(t, &Fahrenheit::get_instance())),
			ParPair("p", pb_par.second));
    VtlQuantity bobp =
      bob_corr->tuned_compute_by_names(pars, c_bob, m_bob, *bo_unit, false);
    return bobp;
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

    VectorDesc in = pref;
    ParList pars = build_correlation_pars();
    const ParPair pb_par = get_pb(pref);
    const double pb = pb_par.second.first; // its unit is pb_par.second.second
    in.pb = pb;
    if (pb >= in.p.get_first())
      {
	in.p.get_first() = nextafter(pb, numeric_limits<double>::max());
	for (size_t i = 1; i < in.p.size() and in.p(i) <= pb; ++i)
	  in.p(i) = nextafter(in.p(i - 1), numeric_limits<double>::max());
      }

    if (exist_coa and coa_corr != nullptr)
      coa = compute_coa(coa, coa_corr, c_coa, m_coa);
    else if (coa_corr != nullptr)
      {
	auto fake_coa = in;
	fake_coa.yname = "coa"; // in this way compute_coa will accept it
	fake_coa.yunit = &coa_corr->unit;
	coa = compute_coa(fake_coa, coa_corr, c_coa, m_coa);
      } // else coa is already defined
    coa = coa.make_parallel(in);

    const VtlQuantity bobp = compute_bobp(in.t);
    in.bobp = bobp.raw();
    pars.insert("t", pref.t, &Fahrenheit::get_instance());
    pars.insert(pb_par);
    pars.insert("bobp", bobp);
    
    DynList<double> p_list, boa_list;
    // Start from 1 (not 0) because bobp point must not be included
    for (auto it = zip_it_pos(1, in.p, coa.y); it.has_curr(); it.next())
      {
	auto t = it.get_curr();
	const double & p_val = get<0>(t);
	const double & coa_val = get<1>(t);
	pars.insert("p", p_val, pref.punit);
	pars.insert("coa", coa_val, coa.yunit);
	const VtlQuantity boa =
	  corr_ptr->tuned_compute_by_names(pars, c, m, *in.yunit, false);
	p_list.append(p_val);
	boa_list.append(boa.raw());
	pars.remove("p");
	pars.remove("coa");
      }

    VectorDesc result(pref.t, pb, bobp.raw(), PVT_INVALID_VALUE,
		      PVT_INVALID_VALUE, p_list, pref.punit, "boa",
		      boa_list, &corr_ptr->unit);
    return compute_epilogue(pref, result);
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
    const ParPair pb_par = get_pb(pref);
    const ParPair uod_par = get_uod(pref, pb_par);
    const double pb = pb_par.second.first;
    const double uod = uod_par.second.first;
    in.pb = in.p.get_last() = pb;
    for (long i = in.p.size() - 2; i >= 0 and in.p(i) > pb; --i)
      in.p(i) = nextafter(in.p(i + 1), numeric_limits<double>::min());
    in.uod = uod;

    if (exist_rs and rs_corr != nullptr)
      rs = compute_rs(rs, rs_corr, c_rs, m_rs);
    else if (rs_corr != nullptr)
      {
	auto fake_rs = in;
	fake_rs.yname = "rs"; // in this way compute_coa will accept it
	fake_rs.yunit = &rs_corr->unit;
	rs = compute_rs(fake_rs, rs_corr, c_rs, m_rs);
      } // else rs is already defined
    rs = rs.make_parallel(in);

    if (exist_bob and bob_corr != nullptr)
      bob = compute_bob(bob, bob_corr, c_bob, m_bob);
    else if (bob_corr != nullptr)
      {
	auto fake_bob = in;
	fake_bob.yname = "bob";
	fake_bob.yunit = &bob_corr->unit;
	bob = compute_bob(fake_bob, bob_corr, c_bob, m_bob);
      } // else bob is already defined
    bob = bob.make_parallel(in);

    ParList pars = build_correlation_pars();
    pars.insert("t", pref.t, &Fahrenheit::get_instance());
    pars.insert(pb_par);
    pars.insert(uod_par);

    assert(Quantity<psig>(in.p.get_first()).is_near(0, 1e-1));

    const VtlQuantity min_uo = min_uo_val();
    const Quantity<CP> max_uo = CP::get_instance().max();

    DynList<double> p_list, uob_list;
    for (auto it = zip_it(in.p, rs.y, bob.y); it.has_curr(); it.next())
      {
	auto t = it.get_curr();
	const double & p_val = get<0>(t);
	const double & rs_val = get<1>(t);
	const double & bob_val = get<2>(t);
	pars.insert("p", p_val, pref.punit);
	pars.insert("rs", rs_val, rs.yunit);
	pars.insert("bob", bob_val, bob.yunit);
	const VtlQuantity uob = corr_ptr->bounded_tuned_compute_by_names
	  (pars, min_uo, max_uo, c, m, CP::get_instance(), false);
	p_list.append(p_val);
	uob_list.append(uob.raw());
	remove_from_container(pars, "p", "rs", "bob");
      }

    VectorDesc result(pref.t, pb, PVT_INVALID_VALUE, uob_list.get_first(),
		      uob_list.get_last(), p_list, pref.punit, "uob", uob_list,
		      &corr_ptr->unit);
    return compute_epilogue(pref, result);
  }

  VectorStats uob_stats(const Correlation * corr_ptr) const
  {
    DynList<const VectorDesc*> uob_lab = stats_prologue("uob", corr_ptr);
    VectorStats stats = compute_stats(uob_lab, corr_ptr, &PvtData::compute_uob);
    return stats_epilogue(stats, min_uo_val().raw(), CP::get_instance().max_val);
  }

  VtlQuantity compute_uobp(const double t) const
  {
    const DynList<const VectorDesc*> uob_vectors = search_vectors("uob");
    auto ptr = uob_vectors.find_ptr([t] (auto vptr) { return vptr->t == t; });
    if (ptr == nullptr)
      ZENTHROW(VarNameNotFound, "Not found data for uob");
    const VectorDesc * v_ptr = *ptr;
    if (uob_corr == nullptr)
      return VtlQuantity(*v_ptr->yunit, v_ptr->uobp);
    const ParPair pb_par = get_pb(v_ptr->t, v_ptr->pb);

    ParList pars = build_correlation_pars();
    insert_in_container(pars, pb_par, get_uod(*v_ptr, pb_par),
			make_par("rs", get_rsb()),
			ParPair("t", ValPair(t, &Fahrenheit::get_instance())),
			ParPair("p", pb_par.second));

    return uob_corr->bounded_tuned_compute_by_names
      (pars, min_uo_val(), CP::get_instance().max(), c_uob, m_uob,
       CP::get_instance(), false);
  }

  VectorDesc compute_uoa(const VectorDesc & pref, const Correlation * corr_ptr,
			 double c = 0, double m = 1) const
  {
    compute_prologue("uoa", pref, corr_ptr);

    VectorDesc in = pref;
    const ParPair pb_par = get_pb(pref);
    const double pb = pb_par.second.first; // its unit is pb_par.second.second
    in.pb = pb;
    if (pb >= in.p.get_first())
      {
	in.p.get_first() = nextafter(pb, numeric_limits<double>::max());
	for (size_t i = 1; i < in.p.size() and in.p(i) <= pb; ++i)
	  in.p(i) = nextafter(in.p(i - 1), numeric_limits<double>::max());
      }

    const ParPair uod_par = get_uod(pref, pb_par);
    const double & uod = uod_par.second.first;
    in.uod = uod;

    ParList pars = build_correlation_pars();    
    pars.insert("t", in.t, &Fahrenheit::get_instance());
    pars.insert(pb_par);
    pars.insert(uod_par);

    const VtlQuantity uo_min = min_uo_val();
    const VtlQuantity uo_max = CP::get_instance().max();

    const VtlQuantity uobp = compute_uobp(in.t);
    in.uobp = uobp.raw();
    pars.insert("uobp", uobp);
    DynList<double> p_list, uoa_list;
    for (auto it = in.p.get_it(); it.has_curr(); it.next())
      {
	const double p = it.get_curr();
	pars.insert("p", p, in.punit);
	const VtlQuantity uoa = corr_ptr->bounded_tuned_compute_by_names
	  (pars, uo_min, uo_max, c, m, *in.yunit, false);
	p_list.append(p);
	uoa_list.append(uoa.raw());
	pars.remove("p");
      }

    // // thus we assure than uobp value es less that first uoa
    const double fake_uobp = nextafter(uoa_list.get_first(),
				       numeric_limits<double>::min());

    VectorDesc result(in.t, pb, in.bobp, uod, fake_uobp, p_list, in.punit,
		      "uoa", uoa_list, &corr_ptr->unit);
    return compute_epilogue(pref, result);
  }

  VectorStats uoa_stats(const Correlation * corr_ptr) const
  {
    DynList<const VectorDesc*> uoa_lab = stats_prologue("uoa", corr_ptr);
    VectorStats stats = compute_stats(uoa_lab, corr_ptr, &PvtData::compute_uoa);
    return stats_epilogue(stats, min_uo_val().raw(), CP::get_instance().max_val);
  }

  VectorStats stats(const Correlation * corr_ptr) const
  {
# define DECLARE_STATS(name)				\
    static auto name##_call = [] (const PvtData * ptr,			\
				  const Correlation * corr_ptr)		\
  {									\
    return ptr->name##_stats(corr_ptr);					\
  };
    DECLARE_STATS(rs);
    DECLARE_STATS(bob);
    DECLARE_STATS(coa);
    DECLARE_STATS(boa);
    DECLARE_STATS(uob);
    DECLARE_STATS(uoa);
    static AHDispatcher<string, VectorStats (*)(const PvtData *,
						const Correlation * corr_ptr)>
      dispather = { "rs", rs_call, "bob", bob_call, "coa", coa_call,
		    "boa", boa_call, "uob", uob_call, "uoa", uoa_call };
    return dispather.run(corr_ptr->target_name(), this, corr_ptr);
  }

  VectorDesc compute_values(const VectorDesc & pref,
			    const Correlation * corr_ptr,
			    double c, double m) const
  {
# define DECLARE_CALL(name)						\
    static auto name##_call = [] (const PvtData * ptr, const VectorDesc & pref,	\
				  const Correlation * corr_ptr,		\
				  double c, double m)			\
      {									\
	return ptr->compute_##name(pref, corr_ptr, c, m);		\
      };
    DECLARE_CALL(rs);
    DECLARE_CALL(bob);
    DECLARE_CALL(coa);
    DECLARE_CALL(boa);
    DECLARE_CALL(uob);
    DECLARE_CALL(uoa);
    static AHDispatcher<string,
			VectorDesc (*)(const PvtData*, const VectorDesc&,
				       const Correlation*, double, double)>
      dispatcher = { "rs", rs_call, "bob", bob_call, "coa", coa_call,
		     "boa", boa_call, "uob", uob_call, "uoa", uoa_call };

    const string target_name = corr_ptr->target_name();
    if (not dispatcher.valid_key(target_name))
      ZENTHROW(InvalidTargetName, corr_ptr->name +
	       " has not a valid target_name (" +
	       join(dispatcher.keys(), ", "));

    return dispatcher.run(corr_ptr->target_name(), this, pref, corr_ptr, c, m);
  }

  /** Return a new vector result of inputing the pressure values
      contained in `v` and executing the correlation `corr_ptr`. The
      result is expressed in `out_unit` */
  VectorDesc inputing(const VectorDesc & v, const Correlation * corr_ptr,
		      double c, double m, const Unit * out_unit) const
  {    
    VectorDesc in = v;
    in.yname = corr_ptr->target_name();
    in.yunit = &corr_ptr->unit;
    VectorDesc ret = compute_values(in, corr_ptr, c, m);
    mutable_unit_convert(*in.yunit, ret.y, *out_unit);
    return ret;
  }

  /** Generate new vectors for `corr_ptr` taking temperature and
      pressure values from `in_vectors` */
  DynList<VectorDesc>
  inputing(const DynList<const VectorDesc*> in_vectors,
	   const Correlation * corr_ptr, double c, double m) const
  {
    DynList<VectorDesc> ret;
    
    const string target_name = corr_ptr->target_name();
    const DynList<const VectorDesc*> tgt_vectors = search_vectors(target_name);
    if (tgt_vectors.is_empty())
      return ret;

    const Unit * out_unit = tgt_vectors.get_first()->yunit;

    for (auto it = in_vectors.get_it(); it.has_curr(); it.next())
      {
	auto curr = it.get_curr();
	auto ptr = tgt_vectors.find_ptr([&curr] (auto ptr)
					{ return ptr->t == curr->t; });
	if (ptr != nullptr)
	  continue;
	ret.append(inputing(*curr, corr_ptr, c, m, out_unit));
      }

    return ret;
  }

  /** For each different temperature found in the data set, builds
      missing properties through the set correlations */
  DynList<VectorDesc> auto_inputing()
  {
    PvtData data = *this; // do a copy because the resulting vectors
			  // will be added
    DynList<VectorDesc> ret;

    auto uob_v = data.search_vectors("uob");

    if (rs_corr == nullptr)
      ZENTHROW(CorrelationNotFound, "Correlation for rs is not set");
    else
      {
	auto rs_list = inputing(uob_v, rs_corr, c_rs, m_rs);
	ret.append(rs_list);
	rs_list.for_each([&data] (auto & v) { data.add_vector(v); });
      }

    if (bob_corr == nullptr)
      ZENTHROW(CorrelationNotFound, "Correlation for bob is not set");
    else
      {
	auto bob_list =
	  inputing(data.search_vectors("rs"), bob_corr, c_bob, m_bob);
	ret.append(bob_list);
	bob_list.for_each([&data] (auto & v) { data.add_vector(v); });
      }
      
    if (coa_corr == nullptr)
      ZENTHROW(CorrelationNotFound, "Correlation for coa is not set");
    else
      {
	auto coa_list =
	  inputing(data.search_vectors("uoa"), coa_corr, c_coa, m_coa);
	ret.append(coa_list);
	coa_list.for_each([&data] (auto & v) { data.add_vector(v); });
      }
      
    if (boa_corr == nullptr)
      ZENTHROW(CorrelationNotFound, "Correlation for boa is not set");
    else
      {
	auto boa_list =
	  inputing(data.search_vectors("boa"), boa_corr, c_boa, m_boa);
	ret.append(boa_list);
	boa_list.for_each([&data] (auto & v) { data.add_vector(v); });
      }

    ret.for_each([this] (auto & v) { this->add_vector(v); });
    stabilize_pb();

    return ret;
  }

  VectorDesc auto_inputing(const Quantity<Fahrenheit> & t,
			   const DynList<Quantity<psig>> & p,
			   const string & target_name) const
  {
    CorrDesc desc = search_corr(target_name);
    if (desc.corr_ptr == nullptr)
      ZENTHROW(CorrelationNotFound, "Not set correlation for target_name " +
	       target_name);
    VectorDesc pref(t.raw(), target_name);
    pref.p = p.maps<double>([] (auto & q) { return q.raw(); });
    pref.punit = &psig::get_instance();
    pref.yunit = &desc.corr_ptr->unit;

    VectorDesc ret = compute_values(pref, desc.corr_ptr, desc.c, desc.m);

    const DynList<const VectorDesc*> other_list = search_vectors(target_name);
    if (not other_list.is_empty())
      {
	auto tgt_unit = other_list.get_first()->yunit;
	mutable_unit_convert(*ret.yunit, ret.y, *tgt_unit);
	ret.yunit = tgt_unit;
      }

    return ret;
  }

  DynList<VectorDesc> auto_inputing(const Quantity<Fahrenheit> & t,
				    const DynList<Quantity<psig>> & p) const
  {
    DynList<VectorDesc> ret;
    for (auto & name : { "pb", "rs", "bob", "coa", "boa", "uod", "uob", "uoa"})
      ret.append(auto_inputing(t, p, name));
    return ret;
  }

  /** Inspect all the vectors and guarantees that it exists only a pb
      generated correlation by each different temperature */
  void stabilize_pb()
  {
    struct PbUod
    {
      double pb, uod;
      PbUod() {}
      PbUod(double __pb, double __uod) : pb(__pb), uod(__uod) {}
    };
    static auto rs_stable = [] (const PvtData * pvt,
				const VectorDesc * v, const PbUod * ptr)
      {
	assert(v->yname == "rs");
	v->pb = v->p.get_last() = ptr->pb;
	v->y.get_last() = pvt->get_rsb().raw();
	for (long i = v->p.size() - 2; i >= 0 and v->p(i) > ptr->pb; --i)
	  {
	    v->p(i) = nextafter(v->p(i + 1), numeric_limits<double>::min());
	    v->y(i) = nextafter(v->y(i + 1), numeric_limits<double>::min());
	  }
      };
    static auto bob_stable = [] (const PvtData * pvt,
				 const VectorDesc * v, const PbUod * ptr)
      {
	assert(v->yname == "bob");
	v->pb = v->p.get_last() = ptr->pb;
	v->y.get_last() = v->bobp = pvt->compute_bobp(v->t).raw();
	for (long i = v->p.size() - 2; i >= 0 and v->p(i) > ptr->pb; --i)
	  {
	    v->p(i) = nextafter(v->p(i + 1), numeric_limits<double>::min());
	    v->y(i) = nextafter(v->y(i + 1), numeric_limits<double>::min());
	  }
      };
    static auto coa_stable = [] (const PvtData*,
				 const VectorDesc * v, const PbUod * ptr)
      {
	assert(v->yname == "coa");
	v->pb = ptr->pb;
	size_t n = 1; // number of pressure points above pb
	while (v->p(n) < ptr->pb and n < v->p.size())
	  ++n;
	LineEq coa(v->p(n - 1), v->y(n - 1), v->p(n), v->y(n));
	v->p(0) = nextafter(ptr->pb, numeric_limits<double>::max());
	v->y(0) = coa(v->p(0));
	for (size_t i = 1; i < n; ++i)
	  {
	    v->p(i) = nextafter(v->p(i - 1), numeric_limits<double>::max());
	    v->y(i) = coa(v->p(i));
	  }
      };
    static auto boa_stable = [] (const PvtData * pvt,
				 const VectorDesc * v, const PbUod * ptr)
      {
	assert(v->yname == "boa");
	v->pb = ptr->pb;
	v->bobp = pvt->compute_bobp(v->t).raw();
	size_t n = 1; // number of pressure points above pb
	while (v->p(n) < ptr->pb and n < v->p.size())
	  ++n;
	
	v->p(0) = nextafter(ptr->pb, numeric_limits<double>::max());
	v->y(0) = nextafter(v->bobp, numeric_limits<double>::min());
	for (size_t i = 1; i < n; ++i)
	  {
	    v->p(i) = nextafter(v->p(i - 1), numeric_limits<double>::max());
	    v->y(i) = nextafter(v->y(i - 1), numeric_limits<double>::min());
	  }
      };
    static auto uob_stable = [] (const PvtData * pvt,
				 const VectorDesc * v, const PbUod * ptr)
      {
	assert(v->yname == "uob");
	v->pb = v->p.get_last() = ptr->pb;
	v->uod = ptr->uod;
	v->uobp = v->y.get_last() = pvt->compute_uobp(v->t).raw();
	for (long i = v->p.size() - 2; i >= 0 and v->p(i) > ptr->pb; --i)
	  {
	    v->p(i) = nextafter(v->p(i + 1), numeric_limits<double>::min());
	    v->y(i) = nextafter(v->y(i + 1), numeric_limits<double>::max());
	  }
      };
    static auto uoa_stable = [] (const PvtData * pvt,
				 const VectorDesc * v, const PbUod * ptr)
      {
	assert(v->yname == "uoa");
	v->pb = ptr->pb;
	v->uod = ptr->uod;
	v->uobp = pvt->compute_uobp(v->t).raw();
	v->p(0) = nextafter(ptr->pb, numeric_limits<double>::max());
	v->y(0) = nextafter(v->uobp, numeric_limits<double>::max());
	size_t n = 1; // number of pressure points above pb
	while (v->p(n) < ptr->pb and n < v->p.size())
	  ++n;
	for (size_t i = 1; i < n; ++i)
	  {
	    v->p(i) = nextafter(v->p(i - 1), numeric_limits<double>::max());
	    v->y(i) = nextafter(v->y(i - 1), numeric_limits<double>::max());
	  }	
      };
    static auto uo_stable = [] (const PvtData*,
				const VectorDesc*, const PbUod*) {};
    static AHDispatcher<string, void (*)(const PvtData*,
					 const VectorDesc*, const PbUod *)>
      dispatcher = { "rs", rs_stable, "bob", bob_stable, "coa", coa_stable,
		     "boa", boa_stable,	"uob", uob_stable, "uoa", uoa_stable,
		     "uo", uo_stable };

    if (pb_corr == nullptr)
      ZENTHROW(CorrelationNotFound, "correlation for pb is not set");
    if (uod_corr == nullptr)
      ZENTHROW(CorrelationNotFound, "correlation for uod is not set");

    DynSetTree<double> temps =
      set_unify<double>(vectors, [] (auto & v) { return v.t; });
    DynMapTree<double, PbUod> t_pb = map_unify<PbUod>(temps, [this] (auto t)
      {
	double pb = this->compute_pb(t, pb_corr, c_pb, m_pb).raw();
	double uod = this->compute_uod(t, pb, uod_corr, c_uod, m_uod).raw();
	return PbUod(pb, uod);
      });

    for (auto & v : vectors)
      dispatcher.run(v.yname, this, &v, &t_pb[v.t]);
  }

  /* given ref_vector which contains a property for several
     temperatures, this routine searches for target_name and returns
     samples, probably interpolated, of the target_name property
     according to pressure values ​​stored in ref_vectors

     NOTE: THE MOST IMPORTANT OF THIS ROUTINE IS THE VECTOR OF
     PRESSURES. IT SHOULD BE USED VERY CAREFULLY IF THE Y VECTOR IS
     INTENDED TO BE USED AND, SINCE INTERPOLATION IS LIKELY TO GIVE
     WRONG VALUES */
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

private:

  DynList<Quantity<CP>> uobp_vals() const
  {
    auto vlist = vectors.filter([] (auto & v)
      { return is_inside(v.yname, {"uob", "uoa"}); });

    DynSetTree<Quantity<CP>> vals;
    for (auto it = vlist.get_it(); it.has_curr(); it.next())
      {
	auto & v = it.get_curr();
	vals.insert(VtlQuantity(*v.yunit, v.uobp));
      }
    return vals.keys();
  }

  DynList<Quantity<RB_STB>> bobp_vals() const
  {
    auto vlist = vectors.filter([] (auto & v)
      { return is_inside(v.yname, {"bob", "boa"}); });

    DynSetTree<Quantity<RB_STB>> vals;
    for (auto it = vlist.get_it(); it.has_curr(); it.next())
      {
	auto & v = it.get_curr();
	vals.insert(VtlQuantity(*v.yunit, v.bobp));
      }
    return vals.keys();
  }

public:

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

	if (name_in_data == "uobp" and
	    (not uobp_vals().all([&par] (auto uobp) { return par.check(uobp); })))
	  return false;
	if (name_in_data == "bobp" and
	    (not bobp_vals().all([&par] (auto bobp) { return par.check(bobp); })))
	  return false;	

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

	if (name_in_data == "uobp" and
	    (not uobp_vals().all([&par] (auto uobp) { return par.check(uobp); })))
	  ret.append(make_pair(name_in_data, true));
	if (name_in_data == "bobp" and
	    (not bobp_vals().all([&par] (auto bobp) { return par.check(bobp); })))
	  ret.append(make_pair(name_in_data, true));

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
    if (target_name != "pb" and target_name != "uod" and
	search_vectors(target_name).is_empty())
      ZENTHROW(VarNameNotFound, target_name + " does not exist in this data set");

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

    if (target_name != "pb" and target_name != "uod" and
	search_vectors(target_name).is_empty())
      ZENTHROW(VarNameNotFound, target_name + " does not exist in this data set");
    
    return Correlation::array().
      filter([&target_name, &relax_tbl, &ban_list, this] (auto ptr)
      {
	return ptr->target_name() == target_name and
	  this->can_be_applied(ptr, relax_tbl, ban_list);
      });
  }

  struct StatsDesc
  {
    const Correlation * corr_ptr = nullptr;
    mutable CorrStat::Desc desc;
    mutable DynList<double> t, plab, p, ylab, ycorr, ytuned;
    mutable bool valid = false;
    StatsDesc(const Correlation * __corr_ptr = nullptr) : corr_ptr(__corr_ptr) {}
    StatsDesc(ConstStats & s)
      : corr_ptr(s.corr_ptr), desc(s.desc), t(move(s.t)),
	ylab(move(s.ylab)), ycorr(move(s.ycorr)), ytuned(move(s.ytuned)),
	valid(s.valid and CorrStat::is_valid(desc)) {}
    StatsDesc(VectorStats & s)
      : corr_ptr(s.corr_ptr), desc(s.desc),
	valid(s.valid and CorrStat::is_valid(desc))
    {
      for (auto it = s.temps.get_it(); it.has_curr(); it.next())
	{
	  auto & pp = it.get_curr();
	  const double & temp = pp.first;
	  auto & tr = pp.second;
	  for (auto it = zip_it(tr.plab, tr.p, tr.ylab, tr.ycorr, tr.ytuned);
	       it.has_curr(); it.next())
	    {
	      auto tu = it.get_curr();
	      t.append(temp);
	      plab.append(get<0>(tu));
	      p.append(get<1>(tu));
	      ylab.append(get<2>(tu));
	      ycorr.append(get<3>(tu));
	      ytuned.append(get<4>(tu));
	    }
	}
    }
    DynList<DynList<double>> both_matrix() const
    {
      return transpose(zip_lists(ycorr, ytuned));
    }
    DynList<DynList<double>> simple_matrix() const // without ytuned
    {
      return transpose(zip_lists(ycorr));
    }
    DynList<DynList<double>> cal_matrix() const // without ycorr
    {
      return transpose(zip_lists(ytuned));
    }
    DynList<string> to_dynlist() const
    {
      assert(corr_ptr);
      DynList<string> ret = build_dynlist<string>(corr_ptr->name);
      return ret.append(CorrStat::desc_to_dynlist(desc));
    }
    friend ostream & operator << (ostream & o, const StatsDesc & s)
    {
      o << s.corr_ptr->name << endl
       	<< s.desc << endl
	<<  "t =";
      s.t.for_each([&o] (auto v) { o << " " << v; });
      o << endl
	<< "p =";
      s.p.for_each([&o] (auto v) { o << " " << v; });
      o << endl
	<< "ylab =";
      s.ylab.for_each([&o] (auto v) { o << " " << v; });
      o << endl
	<< "ycorr =";
      s.ycorr.for_each([&o] (auto v) { o << " " << v; });
      o << endl
	<< "ytuned =";
      s.ytuned.for_each([&o] (auto v) { o << " " << v; });
      return o;
    }
  };

  StatsDesc apply(const Correlation * corr_ptr) const
  {
# define DECLARE_STAT(name)						\
    static auto name##_s = [] (const PvtData * pvt_ptr,			\
			       const Correlation * corr_ptr)		\
      {									\
	return pvt_ptr->name##_stats(corr_ptr);				\
      };
    DECLARE_STAT(pb);
    DECLARE_STAT(uod);
    DECLARE_STAT(rs);
    DECLARE_STAT(bob);
    DECLARE_STAT(coa);
    DECLARE_STAT(boa);
    DECLARE_STAT(uob);
    DECLARE_STAT(uoa);
    
    const string target_name = corr_ptr->target_name();
    if (target_name == "pb" or target_name == "uod")
      {
	static
	  AHDispatcher<string, ConstStats (*)(const PvtData*, const Correlation*)>
	  d = { "pb", pb_s, "uod", uod_s };
	ConstStats s = d.run(target_name, this, corr_ptr);
	return StatsDesc(s);
      }
    else
      {
	static
	  AHDispatcher<string,
		       VectorStats (*)(const PvtData*, const Correlation*)>
	  d = { "rs", rs_s, "bob", bob_s, "bob", bob_s, "coa", coa_s,
		"boa", boa_s, "uob", uob_s, "uoa", uoa_s };
	VectorStats s = d.run(target_name, this, corr_ptr);
	return StatsDesc(s);
      }
  }

  enum class AutoApplyType { r2, mse, sigma, sumsq, c, m };

  StatsDesc auto_apply(const string & target_name,
		       const DynSetTree<string> & relax_tbl,
		       const DynSetTree<const Correlation*> & ban_list,
		       double threshold, AutoApplyType type) const
  {
# define AUTO_FILT(__name)						\
    static auto __name##__filter = [] (const DynList<StatsDesc> & l,	\
				       double threshold)		\
      {									\
	return sort(l.filter([threshold] (auto & s)			\
         {								\
	   return s.valid and CorrStat::__name(s.desc) >= threshold;	\
	 }), cmp_##__name);						\
      }  
    static auto cmp_r2 = [] (auto & s1, auto & s2) 
      {
	return fabs(1 - CorrStat::r2(s1.desc)) < fabs(1 - CorrStat::r2(s2.desc));
      };
    static auto cmp_mse = [] (auto & s1, auto & s2)
      {
	return CorrStat::mse(s1.desc) < CorrStat::mse(s2.desc);
      };
    static auto cmp_sigma = [] (auto & s1, auto & s2)
      {
	return CorrStat::sigma(s1.desc) < CorrStat::sigma(s2.desc);
      };
    static auto cmp_sumsq = [] (auto & s1, auto & s2)
      {
	return CorrStat::sumsq(s1.desc) < CorrStat::sumsq(s2.desc);
      };
    static auto cmp_cabs = [] (auto & s1, auto & s2) 
      {
	return CorrStat::cabs(s1.desc) < CorrStat::cabs(s2.desc);
      };
    static auto cmp_m = [] (auto & s1, auto & s2)
      {
	return CorrStat::m(s1.desc) < CorrStat::m(s2.desc);
      };
    AUTO_FILT(r2);
    AUTO_FILT(mse);
    AUTO_FILT(sigma);
    AUTO_FILT(sumsq);
    AUTO_FILT(cabs);
    AUTO_FILT(m);
    static AHDispatcher<AutoApplyType,
     			DynList<StatsDesc> (*)(const DynList<StatsDesc>&, double)>
      dispatcher(AutoApplyType::r2, r2__filter,
		 AutoApplyType::mse, mse__filter,
		 AutoApplyType::sigma, sigma__filter,
		 AutoApplyType::sumsq, sumsq__filter,
		 AutoApplyType::c, cabs__filter,
		 AutoApplyType::m, m__filter);      

    auto corr_list = can_be_applied(target_name, relax_tbl, ban_list);
    auto auto_list = corr_list.maps<StatsDesc>([this] (auto ptr)
      {
	try
	  {
	    return this->apply(ptr);
	  }
	catch (exception & e)
	  {
	    cout << ptr->name << ": " << e.what() << endl;
	    return StatsDesc(ptr);
	  }
      });
    if (auto_list.is_unitarian())
      {
	type = AutoApplyType::c;
	threshold = 0;
      }
    auto ret_list = dispatcher.run(type, auto_list, threshold);
    return ret_list.is_empty() ? StatsDesc() : ret_list.get_first();
  }

  void set_correlation(const StatsDesc & s)
  {
# define Declare_Set_Corr(NAME)				\
    static auto set_##NAME =				\
      [] (PvtData * pvtdata, const StatsDesc & s)	\
      {							\
	pvtdata->NAME##_corr = s.corr_ptr;		\
	pvtdata->c_##NAME = CorrStat::c(s.desc);	\
	pvtdata->m_##NAME = CorrStat::m(s.desc);	\
      }
    Declare_Set_Corr(pb);
    Declare_Set_Corr(rs);
    Declare_Set_Corr(bob);
    Declare_Set_Corr(coa);
    Declare_Set_Corr(boa);
    Declare_Set_Corr(uod);
    Declare_Set_Corr(uob);
    Declare_Set_Corr(uoa);
    static const AHDispatcher<string, void (*)(PvtData*, const StatsDesc&)>
      dispatcher("pb", set_pb, "rs", set_rs, "bob", set_bob, "coa", set_coa,
		 "boa", set_boa, "uod", set_uod, "uob", set_uob, "uoa", set_uoa);

    const string target_name = s.corr_ptr->target_name();
    try
      {
	dispatcher.run(target_name, this, s);
      }
    catch (exception & e)
      {
	ZENTHROW(CorrelationNotApplicable,
		 "Invalid target for correlation " + target_name);
      }
  }

  struct CorrDesc
  {
    const Correlation * corr_ptr = nullptr;
    double c = 0, m = 1;
    CorrDesc(const Correlation * __corr_ptr, double __c = 0, double __m = 1)
      : corr_ptr(__corr_ptr), c(__c), m(__m) {}
  };

  CorrDesc search_corr(const string & target_name) const
  {
# define GET_CORR(name)							\
    static auto name##_c = [] (const PvtData * ptr)				\
      { return CorrDesc(ptr->name##_corr, ptr->c_##name, ptr->m_##name); };
# define CPAIR(name) #name, name##_c
    GET_CORR(pb);
    GET_CORR(rs);
    GET_CORR(bob);
    GET_CORR(coa);
    GET_CORR(boa);
    GET_CORR(uod);
    GET_CORR(uob);
    GET_CORR(uoa);
    static AHDispatcher<string, CorrDesc (*)(const PvtData*)> dispatcher =
      { CPAIR(pb), CPAIR(rs), CPAIR(bob), CPAIR(coa), CPAIR(boa), CPAIR(uod),
	CPAIR(uob), CPAIR(uoa) };
    return dispatcher.run(target_name, this);
  }

  DynList<StatsDesc>
  auto_apply(const DynSetTree<string> & relax_tbl,
	     const DynSetTree<const Correlation*> & ban_list,
	     const double threshold, const AutoApplyType & type,
	     const size_t n = 1) 
  {
# define ADD_AUTO(name, s)			\
    if (s.valid)				\
      {						\
	corr_map.insert(#name, s);		\
	name##_corr = s.corr_ptr;		\
	c_##name = CorrStat::c(s.desc);		\
	m_##name = CorrStat::m(s.desc);		\
      }

    DynMapTree<string, StatsDesc> corr_map;

    if (pb_corr == nullptr)
      {
	auto tpset = tp_sets();
	StatsDesc s = tpset.is_unitarian() ?
	  auto_apply("pb", relax_tbl, ban_list, threshold, AutoApplyType::c) :
	  auto_apply("pb", relax_tbl, ban_list, threshold, AutoApplyType::r2);
	ADD_AUTO(pb, s);
      }
    else
      corr_map.insert("pb", apply(pb_corr));

    if (rs_corr == nullptr)
      {
	StatsDesc s = auto_apply("rs", relax_tbl, ban_list, threshold, type);
	ADD_AUTO(rs, s);
      }
    else
      corr_map.insert("rs", apply(rs_corr));
	
    if (bob_corr == nullptr)
      {
	StatsDesc s = auto_apply("bob", relax_tbl, ban_list, threshold, type);
	ADD_AUTO(bob, s);
      }
    else
      corr_map.insert("bob", apply(bob_corr));

    if (coa_corr == nullptr)
      {
	StatsDesc s = auto_apply("coa", relax_tbl, ban_list, threshold, type);
	ADD_AUTO(coa, s);
      }
    else
      corr_map.insert("coa", apply(coa_corr));

    boa_corr = &BoaMcCain::get_instance();
    c_boa = 0;
    m_boa = 1;

    if (uod_corr == nullptr)
      {
	auto tpset = tp_sets();
	StatsDesc s = tpset.is_unitarian() ?
	  auto_apply("uod", relax_tbl, ban_list, threshold, AutoApplyType::c) :
	  auto_apply("uod", relax_tbl, ban_list, threshold, AutoApplyType::r2);
	ADD_AUTO(uod, s);
      }
    else
      corr_map.insert("uod", apply(uod_corr));

    if (uob_corr == nullptr)
      {
	StatsDesc s = auto_apply("uob", relax_tbl, ban_list, threshold, type);
	ADD_AUTO(uob, s);
      }
    else
      corr_map.insert("uob", apply(uod_corr));

    if (uoa_corr == nullptr)
      {
	StatsDesc s = auto_apply("uoa", relax_tbl, ban_list, threshold, type);
	ADD_AUTO(uoa, s);
      }
    else
      corr_map.insert("uoa", apply(uoa_corr));

    static const DynList<string> dynamic_names =
      { "rs", "bob", "coa", "uob", "uoa" };
    if (n > 1)
      {
	assert(pb_corr);
	assert(rs_corr);
	assert(bob_corr);
	assert(coa_corr);
	assert(boa_corr);
	assert(uod_corr);
	assert(uob_corr);
	assert(uoa_corr);
	auto_inputing();
      }

    for (size_t i = 1; i < n; ++i)
      {	
	static const DynList<string> static_names = { "pb", "uod" };
	for (auto target_name : static_names)
	  {
	    auto stats = auto_apply(target_name, relax_tbl, ban_list,
				    0, PvtData::AutoApplyType::c);
	    if (not stats.valid)
	      ZENTHROW(MetadataException,
		       "cannot compute automatic correlation for "
		       + target_name + " in iteration " + ::to_string(i));
	    set_correlation(stats);
	    corr_map[target_name] = stats;
	  }

	for (auto target_name : dynamic_names)
	  {
	    auto stats = auto_apply(target_name, relax_tbl, ban_list,
				    threshold, type);
	    if (not stats.valid)
	      ZENTHROW(MetadataException,
		       "cannot compute automatic correlation for "
		       + target_name + " in iteration " + ::to_string(i));
	    set_correlation(stats);
	    corr_map[target_name] = stats;
	  }
      }

    static DynList<string> name_order =
      { "pb", "rs", "bob", "coa", "uod", "uob", "uoa" };
    return name_order.maps<StatsDesc>([&corr_map] (const auto & name)
      {
	return corr_map[name]; 
      });
  }
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
	ZENTHROW(OutOfUnitRange, ptr->ctor_msg() + "pb is out of unit range");
    };
  static auto validate_pb_in_p = [] (VectorDesc* ptr)
    {
      if (not ptr->p.exists([pb = ptr->pb] (auto p) { return p == pb; }))
	ZENTHROW(PressureMismatch, ptr->ctor_msg() +
		 "p vector does not contain pb");
    };
  static auto validate_bo = [] (VectorDesc* ptr)
    {
      if (not RB_STB::get_instance().is_sibling(*ptr->yunit))
	ZENTHROW(InvalidUnit, ptr->ctor_msg() +
		 ptr->yunit->name + "unit is invalid for bo");
      if (not BaseQuantity::is_valid(ptr->bobp, *ptr->yunit))
	ZENTHROW(OutOfUnitRange, ptr->ctor_msg() + "bobp = " +
		 ::to_string(ptr->bobp) + " is out of unit range");
      if (ptr->yunit != & RB_STB::get_instance())
	{
	  mutable_unit_convert(*ptr->yunit, ptr->y, RB_STB::get_instance());
	  ptr->bobp =
	    unit_convert(*ptr->yunit, ptr->bobp, RB_STB::get_instance());
	  ptr->yunit = &RB_STB::get_instance();
	}
      if (not ptr->y.all([u = ptr->yunit] (auto b)
			 { return BaseQuantity::is_valid(b, *u); }))
	ZENTHROW(OutOfUnitRange, ptr->ctor_msg() +
		 "at least a bo value is out of its unit range " +
		 join(ptr->y));
    };
  static auto validate_uo = [] (VectorDesc* ptr)
    {
      if (not CP::get_instance().is_sibling(*ptr->yunit))
	ZENTHROW(InvalidUnit, ptr->ctor_msg() + ptr->yunit->name +
		 " unit is invalid for uo");
      if (not BaseQuantity::is_valid(ptr->uobp, *ptr->yunit))
	ZENTHROW(OutOfUnitRange, ptr->ctor_msg() + "uobp = " +
		 ::to_string(ptr->bobp) + " value is out of unit range");
      if (not ptr->y.all([u = ptr->yunit] (auto b)
			 { return BaseQuantity::is_valid(b, *u); }))
	ZENTHROW(OutOfUnitRange, ptr->ctor_msg() +
		 "at least an uo value is out of its unit range" +
		 join(ptr->y));
      if (ptr->yunit != &CP::get_instance())
	{
	  mutable_unit_convert(*ptr->yunit, ptr->y, CP::get_instance());
	  ptr->uobp = unit_convert(*ptr->yunit, ptr->uobp, CP::get_instance());
	  ptr->uod = unit_convert(*ptr->yunit, ptr->uod, CP::get_instance());
	  ptr->yunit = &CP::get_instance();
	}
    };    
  static auto pb_validate = [] (VectorDesc* ptr)
    {
      ZENTHROW(InvalidTargetName, ptr->ctor_msg()  +
	       "pb cannot be a target name");
    };
  static auto rs_validate = [] (VectorDesc* ptr)
    {
      ptr->validate_sort();
      validate_pb(ptr);
      if (not SCF_STB::get_instance().is_sibling(*ptr->yunit))
	ZENTHROW(InvalidUnit, ptr->yunit->name + " is invalid for coa");
      if (ptr->p.get_last() != ptr->pb)
	ZENTHROW(PressureMismatch, ptr->ctor_msg() +
		 "last value of p = " + ::to_str(ptr->p.get_last()) +
		 " is not pb = " + to_str(ptr->pb));
      if (not is_sorted(ptr->y, std::less_equal<double>()))
	ZENTHROW(SamplesUnsorted, ptr->ctor_msg() +
		 "rs samples are not sorted " + ptr->py_string());
      if (not ptr->y.all([u = ptr->yunit] (auto r)
			 { return BaseQuantity::is_valid(r, *u); }))
	ZENTHROW(OutOfUnitRange, ptr->ctor_msg() +
		 "at least a value of rs is out of its unit range" +
		 join(ptr->y));
      mutable_unit_convert(*ptr->yunit, ptr->y, *rs_unit);
      ptr->yunit = rs_unit;
    };
  static auto bob_validate = [] (VectorDesc* ptr)
    {
      ptr->validate_sort();
      validate_pb(ptr);
      validate_bo(ptr);
      if (not is_sorted(ptr->y, std::less_equal<double>()))
	ZENTHROW(SamplesUnsorted, ptr->ctor_msg() +
		 "bob samples are sorted " + ptr->py_string());

      if (ptr->p.exists([pb = ptr->pb] (auto p) { return p == pb; }))
	{
	  if (not ptr->y.exists([bobp = ptr->bobp] (auto v)
				{ return v == bobp; }))
	    ZENTHROW(SampleInvalid, ptr->ctor_msg() + "bob samples " +
		     join(ptr->y) + " have not bobp = " + to_str(ptr->bobp));
	}
      else
	{
	  ptr->p.append(ptr->pb);
	  ptr->y.append(ptr->bobp);
	}
      if (ptr->pb < ptr->p.get_last())
	ZENTHROW(PressureMismatch, ptr->ctor_msg() + "pb = " + to_str(ptr->pb) +
		 " is less than last value of p = " + to_str(ptr->p.get_last()));
      if (ptr->bobp < ptr->y.get_last())
	ZENTHROW(SampleInvalid, ptr->ctor_msg() + "bobp = " + to_str(ptr->bobp)
		 + " is less than last value of bob = " +
		 to_str(ptr->y.get_last()));
      mutable_unit_convert(*ptr->yunit, ptr->y, *bo_unit);
      ptr->yunit = bo_unit;
    };
  static auto coa_validate = [] (VectorDesc* ptr)
    {
      validate_pb(ptr);
      if (not psia_1::get_instance().is_sibling(*ptr->yunit))
	ZENTHROW(InvalidUnit, ptr->ctor_msg() + ptr->yunit->name +
		 " unit is invalid for uo");
      const size_t & n = ptr->p.size();
      if (n == 2*ptr->y.size())
	{
	  Array<double> pressures(n/2);
	  for (size_t i = 0, j = n/2; j < n; ++i, ++j)
	    pressures.append((ptr->p(i) + ptr->p(j))/2);
	  ptr->p.swap(pressures);
	}
      else if (n != ptr->y.size())
	ZENTHROW(LengthMismatch, ptr->ctor_msg() +
		 "pressure array must twice o same size than coa");      
      ptr->validate_sort();
      if (not is_inversely_sorted(ptr->y, std::less_equal<double>()))
	ZENTHROW(SamplesUnsorted, ptr->ctor_msg() +
		 "coa samples are not inversely sorted " + ptr->py_string());
      if (not ptr->y.all([u = ptr->yunit] (auto c)
			 { return BaseQuantity::is_valid(c, *u); }))
	ZENTHROW(OutOfUnitRange, ptr->ctor_msg() + 
		 "at least a coa value is out of its unit range " +
		 join(ptr->y));
      mutable_unit_convert(*ptr->yunit, ptr->y, *co_unit);
      ptr->yunit = co_unit;
    };
  static auto boa_validate = [] (VectorDesc* ptr)
    {
      ptr->validate_sort();
      validate_pb(ptr);
      validate_bo(ptr);
      if (not is_inversely_sorted(ptr->y, std::less_equal<double>()))
	ZENTHROW(SamplesUnsorted, ptr->ctor_msg() +
		 "boa samples are not inversely sorted " +
		 ptr->py_string());
      if (ptr->p.exists([pb = ptr->pb] (auto p) { return p == pb; }))
	ZENTHROW(SampleInvalid, ptr->ctor_msg() + "p vector " +
		 join(ptr->p) + " contains pb = " + to_str(ptr->pb));
      if (ptr->y.exists([bobp = ptr->bobp] (auto v) { return v == bobp; }))
       	ZENTHROW(SampleInvalid, ptr->ctor_msg() + "boa vector " +
		 join(ptr->y) + " contains bobp = " + to_str(ptr->bobp));
      // ptr->p.insert(nextafter(ptr->pb, numeric_limits<double>::max()));
      // ptr->y.insert(nextafter(ptr->bobp, numeric_limits<double>::min()));
      if (not ptr->y.all([u = ptr->yunit] (auto b)
      			 { return BaseQuantity::is_valid(b, *u); }))
      	ZENTHROW(OutOfUnitRange, ptr->ctor_msg() +
      		 "at least a boa value is out of its unit range " +
		 join(ptr->y));
      mutable_unit_convert(*ptr->yunit, ptr->y, *bo_unit);
      ptr->yunit = bo_unit;
    };
  static auto uod_validate = [] (VectorDesc* ptr)
    {
      ZENTHROW(InvalidTargetName, ptr->ctor_msg() +
	       "uod cannot be a target name");
    };
  static auto uob_validate = [] (VectorDesc* ptr)
    {
      ptr->validate_sort();
      validate_pb(ptr);
      validate_pb_in_p(ptr);
      validate_uo(ptr);
      if (not is_inversely_sorted(ptr->y, std::less_equal<double>()))
	ZENTHROW(SamplesUnsorted, ptr->ctor_msg() +
		 "uob samples are not inversely sorted " + ptr->py_string());
      if (BaseQuantity::is_valid(ptr->uod, *ptr->yunit)) // was uod defined?
	{ // yes ==> validate that first entries correspond to it
	  if (not VtlQuantity(*ptr->yunit, ptr->y(0)).is_near(ptr->uod, 1))
	    ZENTHROW(SampleInvalid, ptr->ctor_msg() +
		     "uod sample " + ::to_string(ptr->uod) +
		     " is not near of first value of uob " +
		     ::to_string(ptr->y(0)));

	  if (not Quantity<psig>(ptr->p(0)).is_near(0, 1))
	    ZENTHROW(SampleInvalid, ptr->ctor_msg() + "first pressure value " +
		     Quantity<psig>(ptr->p(0)).to_string() + " is not "
		     "zero given that uod value is present");
	} 
      else if (are_near(ptr->p(0), 0, 1e-1)) /// is uod inside uob vector?
	ptr->uod = ptr->y(0);
      else
	ZENTHROW(SampleInvalid, ptr->ctor_msg() + "first pressure value = " +
		 to_str(ptr->p(0)) + " is not near from zero");
      if (ptr->uod > ptr->y.get_first())
	ZENTHROW(SampleInvalid, ptr->ctor_msg() + "uod = " + to_str(ptr->uod) +
		 " is greater than first uob value = " + to_str(ptr->y(0)));
      if (ptr->y.get_first() != ptr->uod)
	{
	  ptr->p.insert(0);
	  ptr->y.insert(ptr->uod);
	}
      if (ptr->y.get_last() != ptr->uobp)
	ZENTHROW(OutOfRange, ptr->ctor_msg() + "uobp = " + to_str(ptr->uobp) +
		 " is not equal than last value of uob = " +
		 to_str(ptr->y.get_last()));
      mutable_unit_convert(*ptr->yunit, ptr->y, *uo_unit);
      ptr->yunit = uo_unit;
    };
  static auto uoa_validate = [] (VectorDesc* ptr)
    {
      ptr->validate_sort();
      validate_pb(ptr);
      if (not BaseQuantity::is_valid(ptr->uod, *ptr->yunit))
	ZENTHROW(OutOfUnitRange, ptr->ctor_msg() + "uod = " +
		 to_str(ptr->uod) + " is out of its unit range");
      if (ptr->p.get_first() <= ptr->pb)
	ZENTHROW(SampleInvalid, ptr->ctor_msg() + "first p = " +
		 to_str(ptr->p.get_first()) + " is less than pb = " +
		 to_str(ptr->pb));
      // if (ptr->uobp > ptr->y.get_first())
      // 	ZENTHROW(SampleInvalid, ptr->ctor_msg() + "uobp = " +
      // 		 to_str(ptr->uobp) + " is greater than first uoa = " +
      // 		 to_str(ptr->y.get_first()));
      validate_uo(ptr);
      if (not is_sorted(ptr->y, std::less_equal<double>()))
	ZENTHROW(SamplesUnsorted, ptr->ctor_msg() +
		 "uoa samples are not sorted " + ptr->py_string());
      mutable_unit_convert(*ptr->yunit, ptr->y, *uo_unit);
      ptr->yunit = uo_unit;
      // ptr->p.insert(nextafter(ptr->pb, numeric_limits<double>::max()));
      // ptr->y.insert(nextafter(ptr->uobp, numeric_limits<double>::max()));
    };
  static auto uo_validate = [] (VectorDesc* ptr)
    {
      ptr->validate_sort();
      if (not ptr->yunit->is_sibling(CP::get_instance()))
	ZENTHROW(InvalidUnit, ptr->ctor_msg() + ptr->yunit->name +
		 " is not sibling of CP");
      if (not ptr->y.all([u = ptr->yunit] (auto uo)
			 { return BaseQuantity::is_valid(uo, *u); }))
	ZENTHROW(OutOfUnitRange, ptr->ctor_msg() + 
		 "at least an uo value is out of its unit range" +
		 join(ptr->y));
      if (ptr->yunit != &CP::get_instance())
	{
	  mutable_unit_convert(*ptr->yunit, ptr->y, CP::get_instance());
	  ptr->yunit = &CP::get_instance();
	}
      mutable_unit_convert(*ptr->yunit, ptr->y, *uo_unit);
      ptr->yunit = uo_unit;
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
