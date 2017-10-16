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

  const double t = 0; //  unit will always be in Fahrenheit
  const double pb = PVT_INVALID_VALUE; // bubble point pressure in the
				       // same unit that p 
  const double bobp = PVT_INVALID_VALUE; // in RB_STB
  mutable double uod = PVT_INVALID_VALUE; // dead oil viscosity in cP
  const double uobp = PVT_INVALID_VALUE; // oil viscosity at pb in cP
  mutable Array<double> p;
  const Unit * punit = nullptr;

  const string yname = "no-name";
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
  }

  void post_validate()
  {
    if (p.size() != y.size())
      ZENTHROW(LengthMismatch, "pressure array and " + yname +
	       " array have different sizes");
  }

  VectorDesc(double t, double pb, double bobp, double uod, double uobp,
	     const Array<double> & p, const Unit * punit,
	     const string & yname, const Array<double> & y, const Unit * yunit)
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
	validate_pb_in_p(ptr, "bob");
	validate_bo(ptr);
	if (not is_sorted(ptr->y))
	  ZENTHROW(SamplesUnsorted,
		   "VectorDesc ctor: bob samples are sorted");
	if (not ptr->y.exists([bobp = ptr->bobp] (auto v) { return v == bobp; }))
	  ZENTHROW(SampleInvalid,
		   "VectorDesc ctor: bob samples does not have bobp");
	if (ptr->p.get_last() != ptr->pb)
	  ZENTHROW(PressureMismatch,
		   "VectorDesc ctor (bob): last value of p is not pb");
	if (ptr->y.get_last() != ptr->bobp)
	  ZENTHROW(PressureMismatch,
		   "VectorDesc ctor: last value of bob vector is not bobp");
      };
    static auto coa_validate = [] (VectorDesc* ptr)
      {
	validate_pb(ptr);
	validate_pb_in_p(ptr, "coa");
	if (not psia_1::get_instance().is_sibling(*ptr->yunit))
	  ZENTHROW(InvalidUnit, ptr->yunit->name + " is invalid for uo");
	const size_t & n = ptr->p.size();
	if ((n % 2) != 0)
	  ZENTHROW(LengthMismatch, "pressure array size must be even");
	if (n != 2*ptr->y.size())
	  ZENTHROW(LengthMismatch, "pressure array must twice size than coa");
	Array<double> pressures(n/2);
	for (size_t i = 0, j = n/2; j < n; ++i, ++j)
	  pressures.append((ptr->p(i) + ptr->p(j))/2);
	ptr->p.swap(pressures);
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
	if (ptr->y.get_first() != ptr->bobp)
	  {
	    ptr->p.insert(nextafter(ptr->pb, numeric_limits<double>::max()));
	    ptr->y.insert(nextafter(ptr->bobp, numeric_limits<double>::min()));
	  }

	if (not ptr->p.exists([pb = ptr->pb] (auto p) { return p == pb; }))
	  ZENTHROW(PressureMismatch,
		   "VectorDesc ctor (boa): p vector does not contain pb");
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
	validate_uo(ptr);
	if (not is_sorted(ptr->y))
	  ZENTHROW(SamplesUnsorted,
		   "VectorDesc ctor: uob samples are not sorted");
	if (ptr->y.get_first() < ptr->uobp)
	  ZENTHROW(OutOfRange, "uobp value is less than first uoa");
	if (ptr->y.get_first() != ptr->uobp)
	  {
	    auto vmax = numeric_limits<double>::max();
	    ptr->p.insert(nextafter(ptr->pb, vmax));
	    ptr->y.insert(nextafter(ptr->uobp, vmax));
	  }
      };
    static auto uo_validate = [] (VectorDesc*)
      {
	// TODO efectuar las validaciones del caso
      };
    
    static AHDispatcher<string, void (*)(VectorDesc*)> dispather =
      { "pb", pb_validate, "rs", rs_validate, "bob", bob_validate,
	"coa", coa_validate, "boa", boa_validate, "uod", uod_validate,
	"uob", uob_validate, "uoa", uoa_validate, "uo", uo_validate };

    pre_validate();
    dispather.run(yname, this);
    post_validate(); // this method must be here, after the dispatcher!
  }

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

    if (i < 0)
      return extrapolate_left(p(0), p(1), y(0), y(1), pval);
    else if (i >= n - 1)
      return extrapolate_right(p(n - 2), p(n - 1), y(n - 2), y(n - 1), pval);
    else
      return interpolate(p(i), p(i + 1), y(i), y(i + 1), pval);
  }

  template <template <typename> class C>
  DynList<double> gety(const C<double> & pvals) const
  {
    return pvals.maps([this] (auto & v) { return this->gety(v); });
  }

  DynList<DynList<string>> to_dynlist() const
  {
    DynList<DynList<string>> ret = zip_maps<DynList<string>>([] (auto t)
      {
	return build_dynlist<string>(::to_string(get<0>(t)),
				     ::to_string(get<1>(t)));
			       
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

      For computing pb

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
    const double max_val = numeric_limits<double>::max();
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

    if (pb <= p_below.get_last())
      { // This is an improbable but possible case that would
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

    // and the point immediately next to pb,uobp at the beginning of
    // undersaturated zone
    p_above.insert(nextafter(pb, max_val)); 
    uoa.insert(nextafter(uobp, max_val));

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

  /// Return a list with all parallel vector to v, included v
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

  void add_vector(VectorDesc && v)
  {
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
    vectors.insert(move(v));
  }

  void add_vector(const VectorDesc & v)
  {
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

  void rm_vector(double t, const string & target_name)
  {
    auto l = search_vectors(target_name);
    if (l.is_empty())
      ZENTHROW(VarNameNotFound, "variable " + target_name + " not found");
    if (not l.exists([&target_name, &t] (auto & ptr)
		     { return ptr->yname == target_name and ptr->t == t; }))
      ZENTHROW(VarNameNotFound, "variable " + target_name +
	       " is to associated to t = " + ::to_string(t));
    vectors.remove(VectorDesc(t, target_name));
    if (not vectors.exists([&target_name, &t] (auto &v)
			   { return v.yname == target_name and v.t == t; }))
      names.remove(target_name);
  }

  void
  add_vector(double t, double pb, double uod, double bobp, double uobp,
	     const Array<double> & p, const Unit & punit,
	     const string yname, const Array<double> & y, const Unit & yunit)
  {
    add_vector(VectorDesc(t, pb, bobp, uod, uobp, p, &punit, yname, y, &yunit));
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
  };

  /*
    given ref_vector which contains a property for different
    temperatures, this routine searches for target_name and returns
    the samples, probably interpolated, of the target_name property
    according to pressure values ​​stored in ref_vectors
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
	ret.append(Sample(sample_ptr->t, ptr->pb, ptr->bobp,
			  ptr->uod, ptr->uobp, to_dynlist(p), punit,
			  target_name, to_dynlist(y), yunit));
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
		 const DynSetTree<string> & relax_tbl,
		 const DynSetTree<const Correlation*> & ban_list) const
  {
    return Correlation::array().
      filter([&target_name, &relax_tbl, &ban_list, this] (auto ptr)
      {
	return ptr->target_name() == target_name and
	  this->can_be_applied(ptr, relax_tbl, ban_list);
      });
  }

  DynList<Sample> get_vectors(const Correlation * corr_ptr,
			      const DynList<const VectorDesc*> target_vectors,
			      ParList & pars) const
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
	if (name_in_data == "t" or name_in_data == "p" or
	    name_in_data == "pb" or name_in_data == "uod" or
	    name_in_data == "uobp" or name_in_data == "bobp")
	  continue;

	const ConstDesc * const_ptr = search_const(name_in_data);
	if (const_ptr)
	  {
	    pars.insert(par.name, const_ptr->value, const_ptr->unit_ptr);
	    continue;
	  }

	auto s = get_pars(target_vectors, name_in_data);
	samples.append(s);
      }

    return samples;
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

  //                     t,     pb,     uod,
  struct TpSet
  {
    double t = PVT_INVALID_VALUE, pb = PVT_INVALID_VALUE,
      uod = PVT_INVALID_VALUE;
    TpSet() {}
    TpSet(double __t, double __pb, double __uod)
      : t(__t), pb(__pb), uod(__uod) {}
  };

  DynList<TpSet> tp_sets() const
  {
    DynMapTree<double, TpSet> tuples;
    for (auto it = vectors.get_it(); it.has_curr(); it.next())
      {
  	const VectorDesc & vdesc = it.get_curr();
	if (vdesc.yname == "bo" or vdesc.yname == "uo")
	  continue;
	// TODO: aquí hay error
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
		cout << "Found two contradictory pairs t,pb: "
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

  VectorDesc build_samples(const VectorDesc * vector_ptr,
			   const Correlation * corr_ptr,
			   double c, double m) const
  {
    bool is_rs = corr_ptr->target_name() == "rs";
    const ConstDesc * rsb_ptr = search_const("rsb");
    if (rsb_ptr == nullptr)
      ZENTHROW(InvalidConst, "Data set does not contain rsb value");
    VtlQuantity rsb = { *rsb_ptr->unit_ptr, rsb_ptr->value };
    DynList<double> pvals;
    DynList<double> vals;
    ParList pars;
    const Unit * punit = vector_ptr->punit;
    pars.insert("t", vector_ptr->t, &Fahrenheit::get_instance());
    pars.insert("pb", vector_ptr->pb, punit);
    pars.insert("uod", vector_ptr->uod, &CP::get_instance());
    pars.insert("bobp", vector_ptr->bobp, &RB_STB::get_instance());
    pars.insert("uobp", vector_ptr->uobp, &CP::get_instance());
    load_consts(corr_ptr, pars);

    DynList<const VectorDesc*> par_vectors = parallel_vectors(*vector_ptr);
    auto & p = par_vectors.get_first()->p;
    using T = tuple<string, const Unit*, double>;
    DynList<DynList<T>> vars;
    vars.append(p.maps<T>([vector_ptr] (auto p)
			  { return T("p", vector_ptr->punit, p); }));
    if (not par_vectors.is_unitarian())
      for (auto it = par_vectors.get_it(1); it.has_curr(); it.next())
	{
	  auto & curr = it.get_curr();
	  DynList<T> l;
	  for (auto it = curr->y.get_it(); it.has_curr(); it.next())
	    l.append(T(curr->yname, curr->yunit, it.get_curr()));
	  vars.append(move(l));
	}
    vars = transpose(vars);

    for (auto it = vars.get_it(); it.has_curr(); it.next())
      {
	auto & curr = it.get_curr();
	curr.for_each([&pars] (auto & t)
		      { pars.insert(get<0>(t), get<2>(t), get<1>(t)); });
	try
	  {
	    VtlQuantity r = corr_ptr->tuned_compute_by_names(pars, c, m, false);
	    if (is_rs)
	      r = min(r, rsb);
	    pvals.append(get<2>(curr.get_first()));
	    vals.append(r.raw());
	  }
	catch (exception & e) {}
	curr.for_each([&pars] (auto & t) { pars.remove(get<0>(t)); });
      }
    assert(p.size() == vals.size());
    return VectorDesc(vector_ptr->t, vector_ptr->pb, vector_ptr->bobp,
		      vector_ptr->uod, vector_ptr->uobp, pvals, punit,
		      corr_ptr->target_name(), vals, &corr_ptr->unit);
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

  ParPair pb_par(const VectorDesc & v, const Unit * punit_ptr) const
  {
    if (pb_corr == nullptr) // if pb_corr is not set then return experimental pb
      return build_corrrelation_par("pb", v.pb, punit_ptr);

    // otherwise, we will compute the pb that would generate cplot
    ParList pars;
    for (auto it = pb_corr->parameter_names().get_it(); it.has_curr(); it.next())
      {
	const string & par_name = it.get_curr();
	auto const_ptr = search_const(par_name);
	if (const_ptr != nullptr) // nullptr should be get with temp parameter
	  pars.insert(par_name, const_ptr->value, const_ptr->unit_ptr);
      }
    pars.insert("t", v.t, &Fahrenheit::get_instance());
    VtlQuantity ret = pb_corr->tuned_compute_by_names(pars, c_pb, m_pb);
    return build_corrrelation_par("pb", ret.raw(), &ret.unit);
  }

  tuple<const Unit*, const Unit*, DynList<CorrDesc>>
  iapply(const Correlation * corr_ptr) const
  {
    const string target_name = corr_ptr->target_name();
    const DynList<const VectorDesc*> target_vectors =
      search_vectors(target_name);
    if (target_vectors.is_empty())
      ZENTHROW(VarNameNotFound, "Correlation target " + target_name +
	       " not found in data set");

    auto first_sample_ptr = target_vectors.get_first();

    ParList pars; // parameter to be passed to correlation corr_ptr
    DynList<Sample> samples = get_vectors(corr_ptr, target_vectors, pars);
    if (samples.is_empty())
      samples.append(get_pars(target_vectors, target_name));

    const Unit * punit = first_sample_ptr->punit;
    const Unit * yunit = first_sample_ptr->yunit;
  // Using TYPE = tuple<double, double, double, double, DynList<double>,
  // 		      DynList<double>, DynList<double>>;
    DynList<CorrDesc> ret;
    for (auto it = zip_it(samples, target_vectors); it.has_curr(); it.next())
      {
	auto tt = it.get_curr();
	Sample & desc = get<0>(tt);
	const VectorDesc * ref_ptr = get<1>(tt);
	const Unit * src_punit = desc.punit;
	const double & temp = desc.t;
	const double & pb = ref_ptr->pb;
	const double & bobp = ref_ptr->bobp;
	const double & uod = ref_ptr->uod;
	const double & uobp = ref_ptr->uobp;
	const Unit * par_unit = desc.yunit;
	const string & par_name = desc.name;
	assert(temp == ref_ptr->t);
	pars.insert("t", temp, &Fahrenheit::get_instance());
	// TODO: si hay correlaciones definidas entonces calcular las
	// siguientes 4 propiedades mediante ellas
	//pars.insert("pb", pb, src_punit);
	pars.insert(pb_par(*ref_ptr, src_punit));
	pars.insert("uod", uod, &CP::get_instance());
	pars.insert("bobp", bobp, &RB_STB::get_instance());
	pars.insert("uobp", uobp, &CP::get_instance());
	DynList<double> pvals = unit_convert(*src_punit, desc.pvals, *punit);
	DynList<double> yvals = desc.yvals;
	DynList<double> plist, ylist, yclist;
	for (auto it = zip_it(pvals, yvals, ref_ptr->y); it.has_curr(); it.next())
	  {
	    auto t = it.get_curr();
	    const double & p = get<0>(t); // pressure from desc
	    const double & parval = get<1>(t);
	    const double & y = get<2>(t);
	    pars.insert("p", p, punit);
	    pars.insert(par_name, parval, par_unit);	    
	    try
	      {
		VtlQuantity result =
		  VtlQuantity(*yunit, corr_ptr->compute_by_names(pars, false));

		plist.append(p);
		ylist.append(y);
		yclist.append(result.raw());
	      }
	    catch (exception & e) {}
	    pars.remove("p");
	    pars.remove(par_name);
	  }
	ret.append(CorrDesc(temp, bobp, pb, uod, uobp, plist, ylist, yclist));
	pars.remove("t");
	pars.remove("pb");
	pars.remove("uod");
	pars.remove("uobp");
	pars.remove("bobp");
      }

    return make_tuple(punit, yunit, ret);
  }

  struct PbStats
  {
    const Correlation * corr_ptr = nullptr;
    DynList<double> t;
    DynList<double> pb_lab;
    DynList<double> pb_corr;
    CorrStat::Desc desc;
    bool valid;
    PbStats(const Correlation * corr_ptr, DynList<double> && t,
	    DynList<double> && pb_lab, DynList<double> && pb_corr)
      : corr_ptr(corr_ptr), t(move(t)), pb_lab(pb_lab),
	pb_corr(pb_corr), desc(CorrStat(move(pb_lab)).stats(move(pb_corr)))
    {
      assert(not this->pb_lab.is_empty());
      assert(not this->pb_corr.is_empty());

      if (this->pb_lab.is_unitarian())
	{
	  assert(this->pb_corr.is_unitarian());
	  CorrStat::c(desc) = -CorrStat::c(desc);
	}
    }
    PbStats(const Correlation * corr_ptr) : corr_ptr(corr_ptr), valid(false) {}
  };

  PbStats pbstats(const Correlation * corr_ptr) const
  {
    if (corr_ptr->target_name() != "pb")
      ZENTHROW(InvalidProperty, "correlation " + corr_ptr->name +
	       " is not for pb");
    try
      {
	auto vals = pbapply(corr_ptr);
	return PbStats(corr_ptr, vals.maps<double>([] (auto & d) { return d.t; }),
		       vals.maps<double>([] (auto & d) { return d.pb_lab; }),
		       vals.maps<double>([] (auto & d) { return d.pb_corr; }));
      }
    catch (exception & e)
      {
	return PbStats(corr_ptr);
      }
  }

  struct UodStats
  {
    const Correlation * corr_ptr = nullptr;
    DynList<double> t;
    DynList<double> uod_lab;
    DynList<double> uod_corr;
    CorrStat::Desc desc;
    bool valid;
    UodStats(const Correlation * corr_ptr, DynList<double> && t,
	    DynList<double> && uod_lab, DynList<double> && uod_corr)
      : corr_ptr(corr_ptr), t(move(t)), uod_lab(uod_lab),
	uod_corr(uod_corr), desc(CorrStat(move(uod_lab)).stats(move(uod_corr)))
    {
      assert(not this->uod_lab.is_empty());
      assert(not this->uod_corr.is_empty());

      if (this->uod_lab.is_unitarian())
	{
	  assert(this->uod_corr.is_unitarian());
	  CorrStat::c(desc) = -CorrStat::c(desc);
	}
    }
    UodStats(const Correlation * corr_ptr) : corr_ptr(corr_ptr), valid(false) {}
  };
  
  UodStats uodstats(const Correlation * corr_ptr) const
  {
    if (corr_ptr->target_name() != "uod")
      ZENTHROW(InvalidProperty, "correlation " + corr_ptr->name +
	       " is not for uod");
    try
      {
	auto vals = uodapply(corr_ptr);
	return UodStats(corr_ptr, vals.maps<double>([] (auto & d) { return d.t; }),
			vals.maps<double>([] (auto & d) { return d.uod_lab; }),
			vals.maps<double>([] (auto & d) { return d.uod_corr; }));
      }
    catch (exception & e)
      {
	return UodStats(corr_ptr);
      }
  }

  struct StatsDesc
  {
    const Correlation * corr_ptr = nullptr;
    CorrStat::Desc desc;
    DynList<double> ylab; // experimental values
    DynList<double> ycorr; // values obtained from corr_ptr
    bool valid;

    StatsDesc(PbStats && stats)
      : corr_ptr(stats.corr_ptr), desc(stats.desc), ylab(move(stats.pb_lab)),
	ycorr(move(stats.pb_corr)), valid(stats.valid) {}

    StatsDesc(UodStats && stats)
      : corr_ptr(stats.corr_ptr), desc(stats.desc), ylab(move(stats.uod_lab)),
	ycorr(move(stats.uod_corr)), valid(stats.valid) {}

    StatsDesc(const Correlation * corr_ptr, DynList<double> & ylab,
	      DynList<double> & ycorr)
      : corr_ptr(corr_ptr), desc(CorrStat(ylab).stats(ycorr)),
	ylab(move(ylab)), ycorr(move(ycorr)), valid(true) {}

    StatsDesc(const Correlation * corr_ptr) : corr_ptr(corr_ptr), valid(false) {}
  };

  /* Given corr_ptr, this routine builds a linear regression as follows:

     values given by corr_ptr
     ^
     |
     x                                              o
     |
     |
     |
     x
     |
     |
     x                                    o
     |
     |
     x                         o
     |
     |
     x           o
     |
     |
     x    o
     |
     |
     |
     -----x------x------x------x----------x---------x-------> ylab 
     (these values are sorted by pressure)              (experimental values)       

     The line 
      
        c + m Correlation(pars) 

     best fitting is computed.

     So, the calibrated way would be

     Calibrated(corr pars) = c + m Correlation(corr pars)
   */
  StatsDesc istats(const Correlation * corr_ptr) const
  {
    try
      {
	auto vals = iapply(corr_ptr);
	using Trio = tuple<double, double, double>;
	DynList<Trio> l;
	for (auto it = get<2>(vals).get_it(); it.has_curr(); it.next())
	  {
	    CorrDesc & t = it.get_curr();
	    l.append(t_zip(move(t.p), move(t.y), move(t.yc)));
	  }

	in_place_sort(l, [] (const Trio & t1, const Trio & t2)
		      { // sorting by pressure
			return get<0>(t1) < get<0>(t2);
		      });

	//           y,          p,       yc
	DynMapTree<double, pair<double, double>> ptbl;
	for (auto it = l.get_it(); it.has_curr(); it.next())
	  {
	    const auto & t = it.get_curr();
	    const double & p = get<0>(t);
	    const double & y = get<1>(t);
	    const double & yc = get<2>(t);
	    auto ptr = ptbl.search(y);
	    if (ptr == nullptr)
	      ptbl.append(y, make_pair(p, yc));
	    else if (yc > ptr->second.second)
	      ptr->second = make_pair(p, yc);
	  }

	DynList<double> p, y, yc;
	for (auto it = ptbl.get_it(); it.has_curr(); it.next())
	  {
	    auto pp = it.get_curr();
	    auto & second = pp.second;
	    p.append(second.first);
	    y.append(pp.first);
	    yc.append(second.second);
	  }

	return StatsDesc(corr_ptr, y, yc);
      }
    catch (exception &e)
      {
	return StatsDesc(corr_ptr);
      }
  }

  StatsDesc stats(const Correlation * corr_ptr) const
  {
    const string tgt_name = corr_ptr->target_name();
    if (tgt_name == "pb")
      return StatsDesc(pbstats(corr_ptr));
    else if (tgt_name == "uod")
      return StatsDesc(uodstats(corr_ptr));
    else
      return istats(corr_ptr);
  }

  pair<double, double> cm(const Correlation * corr_ptr) const
  {
    const StatsDesc s = stats(corr_ptr);
    return make_pair(CorrStat::c(s.desc), CorrStat::m(s.desc));
  }

  struct AutoDesc
  {
    const Correlation * corr_ptr = nullptr;
    CorrStat::Desc d;
    AutoDesc(const Correlation * __corr_ptr, const CorrStat::Desc & d)
      : corr_ptr(__corr_ptr), d(d) {}
    AutoDesc(const Correlation * __corr_ptr) : corr_ptr(__corr_ptr) {}
    AutoDesc() {}
    bool is_valid() const noexcept
    {
      return corr_ptr != nullptr and CorrStat::is_valid(d);
    }
    friend ostream & operator << (ostream & out, const AutoDesc & a)
    {
      return out << "  " << a.corr_ptr->name << endl
		 << "  c = " << CorrStat::c(a.d) << endl
		 << "  m = " << CorrStat::m(a.d);
    }
  };

  enum class AutoApplyType { r2, mse, sigma, sumsq, c, m };

# define AUTO_FILT(__name)			\
  static auto __name##__filter = [] (const DynList<AutoDesc> & l,	\
				     double threshold)			\
  {									\
    return sort(l.filter([threshold] (auto & a)				\
    {									\
      return CorrStat::is_valid(a.d) and CorrStat::__name(a.d) >= threshold; \
    }), cmp_##__name);							\
  }
  
# define AUTO_FILTER(__name) AUTO_FILT(__name, cmp_##__name)

  AutoDesc auto_apply(const string & target_name,
		      const DynSetTree<string> & relax_tbl,
		      const DynSetTree<const Correlation*> & ban_list,
		      const double threshold,
		      const AutoApplyType & type = AutoApplyType::r2) const
  {
    static auto cmp_r2 = [] (auto & a1, auto & a2) 
      {
	return fabs(1 - CorrStat::r2(a1.d)) < fabs(1 - CorrStat::r2(a2.d));
      };
    static auto cmp_mse = [] (auto & a1, auto & a2)
      {
	return CorrStat::mse(a1.d) < CorrStat::mse(a2.d);
      };
    static auto cmp_sigma = [] (auto & a1, auto & a2)
      {
	return CorrStat::sigma(a1.d) < CorrStat::sigma(a2.d);
      };
    static auto cmp_sumsq = [] (auto & a1, auto & a2)
      {
	return CorrStat::sumsq(a1.d) < CorrStat::sumsq(a2.d);
      };
    static auto cmp_c = [] (auto & a1, auto & a2)
      {
	return CorrStat::c(a1.d) < CorrStat::c(a2.d);
      };
    static auto cmp_m = [] (auto & a1, auto & a2)
      {
	return CorrStat::m(a1.d) < CorrStat::m(a2.d);
      };
    AUTO_FILT(r2);
    AUTO_FILT(mse);
    AUTO_FILT(sigma);
    AUTO_FILT(sumsq);
    AUTO_FILT(c);
    AUTO_FILT(m);
    static AHDispatcher<AutoApplyType,
     			DynList<AutoDesc> (*)(const DynList<AutoDesc>&, double)>
      dispatcher(AutoApplyType::r2, r2__filter,
		 AutoApplyType::mse, mse__filter,
		 AutoApplyType::sigma, sigma__filter,
		 AutoApplyType::sumsq, sumsq__filter,
		 AutoApplyType::c, c__filter,
		 AutoApplyType::m, m__filter);      

    auto corr_list = can_be_applied(target_name, relax_tbl, ban_list);
    auto auto_list = corr_list.maps<AutoDesc>([this] (auto ptr)
      {
	return AutoDesc(ptr, this->stats(ptr).desc);
      });
    auto ret_list = dispatcher.run(type, auto_list, threshold);
    return ret_list.is_empty() ? AutoDesc() : ret_list.get_first();
  }    

  DynList<AutoDesc>
  auto_apply(const DynSetTree<string> & relax_tbl,
	     const DynSetTree<const Correlation*> & banned_list,
	     const double threshold,
	     const AutoApplyType & type = AutoApplyType::r2) const
  {
    static DynList<string> properties =
      { "pb", "rs", "bob", "coa", "boa", "uod", "uob", "uoa" };

    DynMapTree<string, AutoDesc> m;
    for (auto & target_name : properties)
      {
	auto a =
	  auto_apply(target_name, relax_tbl, banned_list, threshold, type);
	if (a.is_valid())
	  m[target_name] = a;
      }

    if (m.has("coa"))
      m["boa"] = AutoDesc(&BoaMcCain::get_instance());
    else if (m.has("boa"))
      {
	auto & boa_desc = m["boa"];
	auto boa_corr = boa_desc.corr_ptr;
	if (boa_corr != &BoaMcCain::get_instance())
	  {
	    const string author = boa_corr->author_name();
	        // Select coa correlation involving to boa
	    auto coa_corr_ptr = Correlation::array().find_ptr([&author] (auto ptr)
              {
		return ptr->target_name() == "coa" and
		  contains(ptr->name, author);
	      });
	    assert(coa_corr_ptr != nullptr);
	    m["coa"] = AutoDesc(*coa_corr_ptr);
	    boa_desc.corr_ptr = &BoaMcCain::get_instance();
	    assert(m["boa"].corr_ptr == &BoaMcCain::get_instance());
	    assert(CorrStat::c(m["boa"].d) == CorrStat::c(boa_desc.d));
	    assert(CorrStat::m(m["boa"].d) == CorrStat::m(boa_desc.d));
	  }
      }

        // Through this last pass we assure the same order than properties
    DynList<AutoDesc> ret;
    for (auto & target_name : properties)
      {
	auto ptr = m.search(target_name);
	if (ptr != nullptr)
	  ret.append(ptr->second);
      }

    return ret;
  }

# define Declare_Set_Corr(NAME)						\
  static auto set_##NAME =						\
			[] (PvtData * pvtdata, const AutoDesc & a)	\
  {									\
    pvtdata->NAME##_corr = a.corr_ptr;					\
    pvtdata->c_##NAME = CorrStat::c(a.d);				\
    pvtdata->m_##NAME = CorrStat::m(a.d);				\
  }

  void set_correlation(const AutoDesc & d)
  {
    Declare_Set_Corr(pb);
    Declare_Set_Corr(rs);
    Declare_Set_Corr(bob);
    Declare_Set_Corr(coa);
    Declare_Set_Corr(boa);
    Declare_Set_Corr(uod);
    Declare_Set_Corr(uob);
    Declare_Set_Corr(uoa);
    static const AHDispatcher<string, void (*)(PvtData*, const AutoDesc&)>
      dispatcher("pb", set_pb, "rs", set_rs, "bob", set_bob, "coa", set_coa,
		 "boa", set_boa, "uod", set_uod, "uob", set_uob, "uoa", set_uoa);

    const string target_name = d.corr_ptr->target_name();
    try
      {
	dispatcher.run(target_name, this, d);
      }
    catch (exception & e)
      {
	ZENTHROW(CorrelationNotApplicable,
		 "Invalid target for correlation " + target_name);
      }
  }
};

# endif
