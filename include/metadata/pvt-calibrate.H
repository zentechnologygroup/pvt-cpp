# ifndef PVT_CALIBRATE_H
# define PVT_CALIBRATE_H

# include <json.hpp>
# include <ah-stl-utils.H>

# include <lfit.H>

# include <correlations/correlation-stats.H>
# include "metadata-exceptions.H"

using Json = nlohmann::json;

struct VectorDesc
{
  const double t = 0; //  unit will always be in Fahrenheit
  const double pb = -1; // bubble point pressure in the same unit that p
  const double bobp = -1; // in RB_STB
  const double uod = -1; // dead oil viscosity in cP
  const double uobp = -1; // oil viscosity at pb in cP
  const Array<double> p;
  const Unit * punit = nullptr;

  const string yname = "no-name";
  const Unit * yunit = nullptr;
  mutable Array<double> y;

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

  void validate()
  {
    if (p.size() <= 1)
      ZENTHROW(LengthMismatch, "Pressure array size is less or equal tan 1");
    if (p.size() != y.size())
      ZENTHROW(LengthMismatch, "pressure array and " + yname +
	       " array have different sizes");
    if (not is_sorted(p))
      ZENTHROW(SamplesUnsorted, "pressures are not sorted");
  }

  VectorDesc(double t, double pb, double bobp, double uod, double uobp,
	     const Array<double> & p, const Unit * punit,
	     const string & yname, const Unit * yunit, const Array<double> & y)
    : t(t), pb(pb), bobp(bobp), uod(uod), uobp(uobp), p(p), punit(punit),
      yname(yname), yunit(yunit), y(y)
  {
    validate();
  }

  VectorDesc() {}

  VectorDesc(double t, const string & name) : t(t), yname(name) {}

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

  size_t get_yindex(double yval) const noexcept
  {
    return y.find_index([yval] (auto v) { return v == yval; });
  }

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

  pair<VectorDesc, VectorDesc> split_bo() const
  {
    if (yname != "bo")
      ZENTHROW(InvalidTargetName, "yname " + yname + " is not \"bo\"");

    if (&yunit->physical_quantity != &FVFvolumeRatio::get_instance())
      ZENTHROW(InvalidTargetUnit, "yunit " + yunit->name +
	       " is not an unit for oil formation volume ratio");

    size_t max_idx = 0;
    double max_y = 0;
    for (auto it = enum_zip_it(y); it.has_curr(); it.next())
      {
	auto curr = it.get_curr();
	if (get<0>(curr) > max_y)
	  {
	    max_idx = get<1>(curr);
	    max_y = get<0>(curr);
	  }
      }

    Array<double> p_below = p.take(max_idx + 1);
    Array<double> bob = y.take(max_idx + 1);
    if (not is_sorted(bob))
      ZENTHROW(SamplesUnsorted, "saturated region of bo is not increasing");

    Array<double> p_above, boa;
    p_above.append(nextafter(p_below.get_last(), numeric_limits<double>::max()));
    boa.append(nextafter(bob.get_last(), 0));
    for (size_t i = max_idx + 1; i < p.size(); ++i)
      {
	p_above.append(p(i));
	boa.append(y(i));
      }
    
    if (not is_inversely_sorted(boa))
      ZENTHROW(SamplesUnsorted, "undersaturated region of bo is not decreasing");

    LFit lm_below(p_below, bob); 
    LFit lm_above(p_above, boa); 

    const double & cb = lm_below.c, mb = lm_below.m,
      ca = lm_above.c, ma = lm_above.m;

    double pb = (ca - cb) / (mb - ma);
    double bobp = (cb/mb - ca/ma) / (1/mb - 1/ma);

    VectorDesc v_below(t, pb, bobp, -1, -1, p_below, punit, "bob", yunit, bob);
    VectorDesc v_above(t, pb, bobp, -1, -1, p_above, punit, "boa", yunit, boa);

    return make_pair(move(v_below), move(v_above));
  }

  pair<VectorDesc, VectorDesc> split_uo() const
  {
    if (yname != "uo")
      ZENTHROW(InvalidTargetName, "yname " + yname + " is not \"uo\"");

    if (&yunit->physical_quantity != &DynamicViscosity::get_instance())
      ZENTHROW(InvalidTargetUnit, "yunit " + yunit->name +
	       " is not an unit for oil formation volume ratio");

    const double max_val = numeric_limits<double>::max();
    size_t min_idx = 0;
    double min_y = numeric_limits<double>::max();
    for (auto it = enum_zip_it(y); it.has_curr(); it.next())
      {
	auto curr = it.get_curr();
	if (get<0>(curr) < min_y)
	  {
	    min_idx = get<1>(curr);
	    min_y = get<0>(curr);
	  }
      }
    
    Array<double> p_below = p.take(min_idx + 1);
    Array<double> uob = y.take(min_idx + 1);
    if (not is_inversely_sorted(uob))
      ZENTHROW(SamplesUnsorted, "saturated region of uo is not decreasing");

    Array<double> p_above, uoa;
    p_above.append(nextafter(p_below.get_last(), max_val));
    uoa.append(nextafter(uob.get_last(), max_val));
    for (size_t i = min_idx + 1; i < p.size(); ++i)
      {
	p_above.append(p(i));
	uoa.append(y(i));
      }

    if (not is_sorted(uoa))
      ZENTHROW(SamplesUnsorted, "undersaturated region of uo is not increasing");

    LFit lm_below(p_below, uob); 
    LFit lm_above(p_above, uoa); 

    const double & cb = lm_below.c, mb = lm_below.m,
      ca = lm_above.c, ma = lm_above.m;

    double pb = (ca - cb) / (mb - ma);
    double uobp = (cb/mb - ca/ma) / (1/mb - 1/ma);
    double uod = lm_below.predict(0).first;

    VectorDesc v_below(t, pb, -1, uod, uobp, p_below, punit, "uob", yunit, uob);
    VectorDesc v_above(t, pb, -1, uod, uobp, p_above, punit, "uoa", yunit, uoa);

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

  string cplot_consts() const
  {
    return const_values.foldl<string>("", [] (auto & acu, auto & c)
      {
	return acu + c.to_cplot() + " ";
      });
  }

  using Tcorr = tuple<const Correlation**, double*, double*>;
  using Pcorr = pair<string, Tcorr>;

  DynMapTree<string, Tcorr> corr_tbl =
    {
      Pcorr("pb", Tcorr(&pb_corr, &c_pb, &m_pb)),
      Pcorr("rs", Tcorr(&rs_corr, &c_rs, &m_rs)),
      Pcorr("bob", Tcorr(&bob_corr, &c_bob, &m_bob)),
      Pcorr("boa", Tcorr(&boa_corr, &c_boa, &m_boa)),
      Pcorr("coa", Tcorr(&coa_corr, &c_coa, &m_coa)),
      Pcorr("uob", Tcorr(&uob_corr, &c_uob, &m_uob)),
      Pcorr("uod", Tcorr(&uoa_corr, &c_uoa, &m_uoa)),
      Pcorr("uoa", Tcorr(&uoa_corr, &c_uoa, &m_uoa))
    };

  string cplot_corrs()
  {
    ostringstream s;
    for (auto it = corr_tbl.get_it(); it.has_curr(); it.next())
      {
	auto & curr = it.get_curr();
	const string & tgt = curr.first;
	cout << "Processing " << tgt << endl;
	auto t = curr.second;
	assert(get<0>(t));
	auto corr_ptr = *get<0>(t);
	if (corr_ptr == nullptr)
	  ZENTHROW(InvalidTargetName, "correlation for " + tgt + " not found");
	const double c = *get<1>(t), m = *get<2>(t);
	cout << "c = " << c << " m = " << m << endl;
	s << "--" << tgt << " " << corr_ptr->name << " --c-" << tgt << " "
	  << to_str(c) << " --m-" << tgt << " " << to_str(m) << " ";
      }
    return s.str();
  }

  bool defined() const
  {
    return not (const_values.is_empty() and vectors.is_empty());
  }

  tuple<const Correlation*, double, double>
  get_corr(const string & target_name)
  {
    auto ptr = corr_tbl.search(target_name);
    if (ptr == nullptr)
      ZENTHROW(InvalidTargetName, "target name " + target_name + " not found");
    Tcorr t = ptr->second;
    return make_tuple(*get<0>(t), *get<1>(t), *get<2>(t));
  }

  bool are_all_correlations_defined() const
  {
    return corr_tbl.all([] (auto p) { return get<0>(p.second); });
  }

  DynList<string> missing_correlations() const
  {
    return corr_tbl.filter([] (auto p) { return not get<0>(p.second); }).
      maps<string>([] (auto p) { return p.first; });
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

  static DynList<string>
  corr_to_list(const string & tgt,
	       const Correlation * corr_ptr, double c, double m)
  {
    if (corr_ptr)
      return build_dynlist<string>(tgt + " = " + corr_ptr->name, "c = " +
				   to_str(c), "m = " + to_str(m));
    return build_dynlist<string>(tgt + " = undefined", "c = undefined",
				   "m = undefined");
  }

# define Corr_List(NAME) corr_to_list(#NAME, NAME##_corr, c_##NAME, m_##NAME)

  DynList<DynList<string>> corr_list() const
  {
    return build_dynlist<DynList<string>>(Corr_List(pb), Corr_List(rs),
					  Corr_List(bob), Corr_List(boa), 
					  Corr_List(coa), Corr_List(uod),
					  Corr_List(uob), Corr_List(uoa));
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

  string to_string() const
  {
    DynList<DynList<string>> const_list =
      const_values.maps<DynList<string>>
      ([] (auto & c) { return c.to_dynlist(); });
    const_list.insert(build_dynlist<string>("name", "value", "unit"));

    auto corrs = corr_list();
    ostringstream s;
    s << "Constants:" << endl
      << shift_lines_to_left(Aleph::to_string(format_string(const_list)), 2)
      << endl
      << "Correlations:" << endl
      << shift_lines_to_left(Aleph::to_string(format_string(corr_list())), 2)
      << endl
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
    add_vector(VectorDesc(t, pb, bobp, uod, uobp, p, &punit, yname, &yunit, y));
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

  using Sample =
  tuple<double, double, double, double, double, // t, pb, bobp, uod, uobp
	const Unit*, DynList<double>, // pressure unit &values
	string, // property name
	const Unit*, DynList<double>>; // target unit & values
  static const double & s_t(Sample & s) { return get<0>(s); }
  static const double & s_pb(Sample & s) { return get<1>(s); }
  static const double & s_bobp(Sample & s) { return get<2>(s); }
  static const double & s_uod(Sample & s) { return get<3>(s); }
  static const double & s_uobp(Sample & s) { return get<4>(s); }
  static const Unit* s_punit(Sample & s) { return get<5>(s); }
  static DynList<double> & s_pvals(Sample & s) { return get<6>(s); }
  static string & s_yname(Sample & s) { return get<7>(s); }
  static const Unit* s_yunit(Sample & s) { return get<8>(s); }
  static DynList<double> & s_yvals(Sample & s) { return get<9>(s); }

  // given ref_vector which contains a property for different
  // temperatures, this routine searches for target_name and returns
  // the samples, probably interpolated, of the target_name property
  // according to pressure values ​​stored in ref_vectors 
  DynList<Sample> get_pars(const DynList<const VectorDesc*> & ref_vectors,
			   const string & target_name) const
  {
    DynList<Sample> ret;
    DynList<const VectorDesc*> samples = search_vectors(target_name);
    if (samples.is_empty())
      return ret;

    const VectorDesc * fst_vector = ref_vectors.get_first();

    auto ptr = samples.get_first();
    const Unit * punit = ptr->punit;
    const Unit * yunit = ptr->yunit;

    ret.append(Sample(ptr->t, ptr->pb, ptr->bobp, ptr->uod, ptr->uobp, punit,
		      to_dynlist(fst_vector->p),
		      target_name, yunit, ptr->gety(fst_vector->p)));

    for (auto it = zip_it_pos(1, samples, ref_vectors); it.has_curr(); it.next())
      {
	auto t = it.get_curr();
	const VectorDesc * sample_ptr = get<0>(t);
	const VectorDesc * vector_ptr = get<1>(t);
	DynList<double> p = to_dynlist(unit_convert(*vector_ptr->punit,
						    vector_ptr->p, *punit));
	DynList<double> y = unit_convert(*sample_ptr->yunit,
					 sample_ptr->gety(p), *yunit);
	ret.append(Sample(sample_ptr->t, ptr->pb, ptr->bobp, ptr->uod, ptr->uobp,
			  punit, move(p), target_name, yunit, move(y)));
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
		      const DynSetTree<string> & relax_tbl) const
  {
    auto ref_vectors = search_vectors(corr_ptr->target_name());
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
	       const DynList<double> & yvals = s_yvals(s);
	       const Unit * yunit = s_yunit(s);
	       return yvals.all([&par, yunit] (auto v)
				{
				  return par.check(VtlQuantity(*yunit, v));
				});
	     }))
	  return false;
      }

    return true;
  }

  bool can_be_applied(const Correlation * corr_ptr) const
  {    
    return can_be_applied(corr_ptr, DynSetTree<string>());
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
		const DynList<double> & yvals = s_yvals(s);
		const Unit * yunit = s_yunit(s);
		return yvals.all([&par, yunit] (auto v)
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
		    const DynSetTree<string> & relax_tbl) const
  {
    auto corr_list = Correlation::array().filter([&target_name] (auto ptr)
      {
	return ptr->target_name() == target_name;
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
		 const DynSetTree<string> & relax_tbl) const
  {
    return Correlation::array().filter([&target_name, &relax_tbl, this] (auto ptr)
      {
	return ptr->target_name() == target_name and
	  this->can_be_applied(ptr, relax_tbl);
      });
  }

  DynList<const Correlation*> can_be_applied(const string & target_name) const
  {
    return can_be_applied(target_name, DynSetTree<string>());
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
  using Ttuple = tuple<double, double, double>;
  DynList<Ttuple> tp_sets() const
  {
    DynMapTree<double, Ttuple> tuples;
    for (auto it = vectors.get_it(); it.has_curr(); it.next())
      {
  	const VectorDesc & vdesc = it.get_curr();
	if (vdesc.yname == "bo" or vdesc.yname == "uo")
	  continue;
	// TODO: aquí hay error
  	auto ptr = tuples.search(vdesc.t);
  	if (ptr == nullptr)
	  if (vdesc.yname == "uob" or vdesc.yname == "uoa")
	    tuples.insert(vdesc.t, Ttuple(vdesc.t,
					  unit_convert(*vdesc.punit, vdesc.pb,
						       psia::get_instance()),
					  unit_convert(*vdesc.yunit, vdesc.uod,
						       CP::get_instance())));
	  else
	    tuples.insert(vdesc.t,
			  Ttuple(vdesc.t, unit_convert(*vdesc.punit, vdesc.pb,
						       psia::get_instance()), -1));
	else
  	  {
  	    const double p = unit_convert(*vdesc.punit, vdesc.pb,
  					  psia::get_instance());
	    Ttuple & t = ptr->second;
  	    if (get<1>(t) > 0 and get<1>(t) != p)
  	      {
  		ostringstream s;
  		s << "Found two contradictory pairs t,pb: " << ptr->first << ","
  		  << get<1>(t) << " != " << vdesc.t << "," << vdesc.pb;
  		ZENTHROW(PressureMismatch, s.str());
  	      }
	    if (vdesc.yname == "uob" or vdesc.yname == "uoa")
	      get<2>(t) = unit_convert(*vdesc.yunit, vdesc.uod,
				       CP::get_instance());
	    else if (get<2>(t) > 0 and get<2>(t) != vdesc.uod)
	      {
		ostringstream s;
  		s << "Found two ambiguous uod values for t = " << vdesc.t
		  << " pb = " << vdesc.pb << " " << get<2>(t) << " != "
		  << vdesc.uod;
  		ZENTHROW(AmbiguousDeadViscosity, s.str());
	      }
  	  }
      }

    if (not is_sorted(tuples.values().
		      maps<double>([] (auto & t) { return get<1>(t); })))
      ZENTHROW(PressureMismatch,
  	       "pb values are not sorted through temperatures");

    return tuples.maps<Ttuple>([] (auto & p) { return p.second; });
  }

    //       t     pb_lab, pb_corr
  DynList<tuple<double, double, double>>
  pbapply(const Correlation * corr_ptr) const
  {
    if (corr_ptr->target_name() != "pb")
      ZENTHROW(InvalidTargetName, "correlation " + corr_ptr->name +
	       " is not for pb");

    DynList<Ttuple> tuples = tp_sets();
    ParList pars; // parameter to be passed to correlation corr_ptr
    load_consts(corr_ptr, pars);
    DynList<tuple<double, double, double>> ret;
    for (auto it = tuples.get_it(); it.has_curr(); it.next())
      {
	auto pp = it.get_curr();
	const double & t = get<0>(pp);
	const double & pb_lab = get<1>(pp);
	pars.insert("t", t, &Fahrenheit::get_instance());
	auto result = corr_ptr->compute_by_names(pars, false);
	auto pb_ret = VtlQuantity(psia::get_instance(), result).raw();
	ret.append(make_tuple(t, pb_lab, pb_ret));
	pars.remove("t");
      }
    return ret;
  }

      //       t     pb_lab, uod_corr
  DynList<tuple<double, double, double>>
  uodapply(const Correlation * corr_ptr) const
  {
    if (corr_ptr->target_name() != "uod")
      ZENTHROW(InvalidTargetName, "correlation " + corr_ptr->name +
	       " is not for uod");

    DynList<Ttuple> tuples = tp_sets();
    ParList pars; // parameter to be passed to correlation corr_ptr
    load_consts(corr_ptr, pars);
    DynList<tuple<double, double, double>> ret;
    for (auto it = tuples.get_it(); it.has_curr(); it.next())
      {
	auto pp = it.get_curr();
	const double & t = get<0>(pp);
	const double & uod_lab = get<2>(pp);
	pars.insert("t", t, &Fahrenheit::get_instance());
	auto result = corr_ptr->compute_by_names(pars, false);
	auto uod_ret = VtlQuantity(CP::get_instance(), result).raw();
	ret.append(make_tuple(t, uod_lab, uod_ret));
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
    for (auto it = vector_ptr->p.get_it(); it.has_curr(); it.next())
      {
	auto & p = it.get_curr();
	pars.insert("p", p, punit);
	try
	  {
	    VtlQuantity r = corr_ptr->tuned_compute_by_names(pars, c, m, false);
	    if (is_rs)
	      r = min(r, rsb);
	    pvals.append(p);
	    vals.append(r.raw());
	    pars.tbl.for_each([] (auto p)
              {
		cout << " " << p.first << " = " << p.second.first;
	      });
	    cout << " = " << c + m*r.raw() << endl;
	  }
	catch (...) {} 
	pars.remove("p");
      }
    return VectorDesc(vector_ptr->t, vector_ptr->pb, vector_ptr->bobp,
		      vector_ptr->uod, vector_ptr->uobp, pvals, punit,
		      corr_ptr->target_name(), &corr_ptr->unit, vals);
  }

  //                  t,      pb,     bobp,   uod,   uobp
  using TYPE = tuple<double, double, double, double, double,
		     //  p,            y,                 yc		     
		     DynList<double>, DynList<double>, DynList<double>>;

  tuple<const Unit*, const Unit*, DynList<TYPE>>
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
    DynList<TYPE> ret;
    for (auto it = zip_it(samples, target_vectors); it.has_curr(); it.next())
      {
	auto tt = it.get_curr();
	Sample & desc = get<0>(tt);
	const VectorDesc * ref_ptr = get<1>(tt);
	const Unit * src_punit = s_punit(desc);
	const double & temp = s_t(desc);
	const double & pb = ref_ptr->pb;
	const double & bobp = ref_ptr->bobp;
	const double & uod = ref_ptr->uod;
	const double & uobp = ref_ptr->uobp;
	const Unit * par_unit = s_yunit(desc);
	const string & par_name = s_yname(desc);
	assert(temp == ref_ptr->t);
	pars.insert("t", temp, &Fahrenheit::get_instance());
	pars.insert("pb", pb, src_punit);
	pars.insert("uod", uod, &CP::get_instance());
	pars.insert("bobp", bobp, &RB_STB::get_instance());
	pars.insert("uobp", uobp, &CP::get_instance());
	DynList<double> pvals = unit_convert(*src_punit, s_pvals(desc), *punit);
	DynList<double> yvals = s_yvals(desc);
	DynList<double> plist, ylist, yclist;
	for (auto it = zip_it(pvals, yvals, ref_ptr->y); it.has_curr(); it.next())
	  {
	    auto t = it.get_curr();
	    const double & p = get<0>(t); // pressure from desc
	    const double & parval = get<1>(t);
	    const double & y = get<2>(t);
	    pars.insert("p", p, punit);
	    pars.insert(par_name, parval, par_unit);
	    
	    // pars.tbl.for_each([par_unit] (auto p)
            //   {
	    // 	cout << p.first << " = " << p.second.first
	    // 	     << " " << p.second.second->name << " ";
	    //   });
	    // cout << endl;

	    try
	      {
		VtlQuantity result =
		  VtlQuantity(*yunit, corr_ptr->compute_by_names(pars, false));

		plist.append(p);
		ylist.append(y);
		yclist.append(result.raw());
		//cout << target_name << " = " << result << endl;
	      }
	    catch (exception & e)
	      {
		// cout << e.what() << endl;
	      }
	    pars.remove("p");
	    pars.remove(par_name);
	  }
	ret.append(TYPE(temp, bobp, pb, uod, uobp, plist, ylist, yclist));
	pars.remove("t");
	pars.remove("pb");
	pars.remove("uod");
	pars.remove("uobp");
	pars.remove("bobp");
      }

    return make_tuple(punit, yunit, ret);
  }

  using T =
    tuple<const Correlation*, DynList<double>, DynList<double>, CorrStat::Desc>;

  T pbstats(const Correlation * corr_ptr) const
  {
    if (corr_ptr->target_name() != "pb")
      ZENTHROW(InvalidProperty, "correlation " + corr_ptr->name +
	       " is not for pb");

    if (not can_be_applied(corr_ptr))
      ZENTHROW(CorrelationNotApplicable, "correlation " + corr_ptr->name +
	       " is not applicable to this data set");

    auto vals = pbapply(corr_ptr);
    auto t = t_unzip(vals);
    DynList<double> & y = get<1>(t);
    DynList<double> & yc = get<2>(t);

    return T(corr_ptr, get<0>(t), y, CorrStat(move(y)).stats(move(yc)));
  }

  T uodstats(const Correlation * corr_ptr) const
  {
    if (corr_ptr->target_name() != "uod")
      ZENTHROW(InvalidProperty, "correlation " + corr_ptr->name +
	       " is not for uod");

    if (not can_be_applied(corr_ptr))
      ZENTHROW(CorrelationNotApplicable, "correlation " + corr_ptr->name +
	       " is not applicable to this data set");

    auto vals = uodapply(corr_ptr);
    auto t = t_unzip(vals);
    DynList<double> & y = get<1>(t);
    DynList<double> & yc = get<2>(t);

    return T(corr_ptr, get<0>(t), y, CorrStat(move(y)).stats(move(yc)));
  }

  T istats(const Correlation * corr_ptr) const
  {
    //cout << "ystats " << corr_ptr->target_name() << endl;
    // using TYPE = tuple<double, double, double, double,
    // 		     //  p,            y,                 yc		     
    // 		     DynList<double>, DynList<double>, DynList<double>>;
    auto vals = iapply(corr_ptr);

    using Trio = tuple<double, double, double>;
    DynList<Trio> l;
    for (auto it = get<2>(vals).get_it(); it.has_curr(); it.next())
      {
	auto & t = it.get_curr();
	DynList<double> & p = get<5>(t);
	DynList<double> & y = get<6>(t);
	DynList<double> & yc = get<7>(t);	 
	l.append(t_zip(move(p), move(y), move(yc)));
      }

    in_place_sort(l, [] (const Trio & t1, const Trio & t2)
		  { // sorting by pressure
		    return get<0>(t1) < get<0>(t2);
		  }); 

    //           yc,          p,       y
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
    	y.append(second.second);
    	yc.append(pp.first);
      }

    auto stats = CorrStat(yc).stats(y);

    return make_tuple(corr_ptr, move(p), move(y), move(stats));
  }

  T stats(const Correlation * corr_ptr) const
  {
    const string tgt_name = corr_ptr->target_name();
    if (tgt_name == "pb")
      return pbstats(corr_ptr);
    else if (tgt_name == "uod")
      return uodstats(corr_ptr);
    else
      return istats(corr_ptr);
  }

  pair<double, double> cm(const Correlation * corr_ptr) const
  {
    T s = stats(corr_ptr);
    return make_pair(CorrStat::c(get<3>(s)), CorrStat::m(get<3>(s)));
  }
};

# endif
