# ifndef CPLOT_H
# define CPLOT_H

# include <correlations/pvt-correlations.H>
# include <correlations/defined-correlation.H>
# include <correlations/correlation-invoker.H>
# include <correlations/uo-min.H>
# include <metadata/ttuner-units.H>

class Cplot
{
protected:

  CorrelationInvoker inv;

public:

  struct Grid
  {
    struct Row
    {
      bool is_pb = false;
      bool exception = false;
      Array<double> row;

      friend ostream & operator << (ostream & out, const Row & row)
      {
	out << (row.is_pb ? "true," : "false,")
	    << (row.exception ? "true," : "false,");
	return join(row.row, ",", out);
      }
    };

    size_t precision = 6;

  private:

    DynMapTree<string, size_t> name_to_col; // maps property name to column index
    Array<const Unit*> units; // units for each column
    Row curr_row;
    Array<Row> grid;

  public:

    void define_header() {}

    template <typename ... Args> void
    define_header(const string & name, const Unit & unit, const Args & ... args)
    {
      const size_t ncol = units.size(); // this the new column index
      units.append(&unit);
      name_to_col.insert(name, ncol);
      define_header(args...);
    }

    Grid() {}

    template <typename ... Args> Grid(const Args & ... args)
      : Grid()
    {
      define_header(args...);
    }

    size_t ncol() const noexcept { return units.size(); }

    size_t nrow() const noexcept { return grid.size(); }

    void put_col(const bool & is_pb, const bool & exception)
    {
      curr_row.is_pb = is_pb;
      curr_row.exception = exception;
    }

    template <typename ... Args> 
    void put_col(const bool & is_pb, const bool & exception,
		 const double & val, const Args & ... args)
    {
      curr_row.row.append(val);
      put_col(is_pb, exception, args...);
    }

    template <typename ... Args> 
    void put_col(const bool & is_pb, const bool & exception,
		 const VtlQuantity & val, const Args & ... args)
    {
      put_col(is_pb, exception, val.raw(), args...);
    }

    template <typename ... Args> 
    void put_row(const bool & is_pb, const bool & exception,
		 const Args & ... args)
    {
      put_col(is_pb, exception, args...);
      grid.append(move(curr_row));
      assert(curr_row.row.is_empty());
      curr_row.row.reserve(ncol());
    }

    size_t col_index(const string & col_name) const
    {
      auto p = name_to_col.search(col_name);
      if (p == nullptr)
	ALEPHTHROW(NameNotFound, "column name " + col_name + " not found");
      return p->second;
    }

    const Unit * unit(const size_t col_idx) const
    {
      assert(col_idx < ncol());
      return units(col_idx);
    }

    const Unit * unit(const string & col_name) const
    {
      return units(col_index(col_name));
    }

    DynList<double> col(const size_t col_idx) const
    {
      assert(col_idx < ncol());
      DynList<double> ret;
      for (size_t i = 0, n = nrow(); i < n; ++i)
	ret.append(grid(i).row(col_idx));
      return ret;
    }

    DynList<double> col(const string col_name) const
    {
      return col(col_index(col_name));
    }

    friend ostream & operator << (ostream & out, const Grid & grid)
    {
      auto col_names = sort(grid.name_to_col.items(), [] (auto & p1, auto & p2)
			    { return p1.second < p2.second; });

      out << "pbrow " << Unit::null_unit.name << ",exception "
	  << Unit::null_unit.name << ","
	  << join(zip_maps<string>([] (auto t)
				   {
				     const string & name = get<0>(t).first;
				     const Unit * unit_ptr = get<1>(t);
				     return name + " " + unit_ptr->name;
				   }, col_names, grid.units), ",") << endl;
      if (grid.grid.is_empty())
	return out;

      out.precision(grid.precision);
      for (auto it = grid.grid.get_it(); it.has_curr(); it.next())
	{
	  const Row & row = it.get_curr();
	  out << row << endl;
	}

      return out;
    }
  };

protected:

  Grid grid;

public:

  bool check = false;

  Cplot(const Unit & tunit, const Unit & punit, bool __check)
    : inv(tunit, punit), check(__check) {}

# define CONSTANT_PAR(NAME, __unit)			\
  VtlQuantity NAME;					\
  const Unit * NAME##_unit = &__unit::get_instance();	\
							\
  void set_##NAME(double val, const Unit & unit)	\
  {							\
    if (not unit.is_sibling(*NAME##_unit))		\
      ALEPHTHROW(InvalidUnit, "unit " + unit.name + " is not for " +	\
	       NAME##_unit->physical_quantity.name);			\
    NAME = VtlQuantity(unit, val);					\
  }									\
									\
  void test_##NAME() const						\
  {									\
    if (NAME.is_null())							\
      ALEPHTHROW(ValueNotFound, #NAME " value has not been set");		\
  }

  CONSTANT_PAR(api, Api);
  CONSTANT_PAR(rsb, SCF_STB);
  CONSTANT_PAR(yg, Sgg);
  CONSTANT_PAR(tsep, Fahrenheit);
  CONSTANT_PAR(tsep2, Fahrenheit);
  CONSTANT_PAR(psep, psia);
  CONSTANT_PAR(ogr, STB_MMscf);
  CONSTANT_PAR(n2, MolePercent);
  CONSTANT_PAR(co2, MolePercent);
  CONSTANT_PAR(h2s, MolePercent);
  CONSTANT_PAR(nacl, Molality_NaCl);

  /** Given a list of required parameters, this function tests if
      `name` is inside. It this is the case, then `name` is inserted
      in `pars_list`.

      `required` parameter is a list of pairs. Each pair is composed
      by the parameter name and a list of parameter alias.

      This method is intended to be used for constant parameters.
   */
  static void
  test_parameter(const DynList<pair<string, DynList<string>>> & required,
		 const string & name, const VtlQuantity & par,
		 ParList & pars_list)
  {
    if (required.exists([&name] (auto & p) { return p.first == name or
	    p.second.exists([&name] (auto & s) { return s == name; }); }))
      pars_list.insert(name, par);
  }

  ParList load_constant_parameters(const DynList<const Correlation*> & l)
  {
    ParList pars_list;
    auto required_pars = DefinedCorrelation::parameter_list(l);
				  
    test_parameter(required_pars, "api", api, pars_list);
    test_parameter(required_pars, "rsb", rsb, pars_list);
    test_parameter(required_pars, "yg", yg, pars_list);
    test_parameter(required_pars, "tsep", tsep, pars_list);
    test_parameter(required_pars, "psep", psep, pars_list);
    test_parameter(required_pars, "n2", n2, pars_list);
    test_parameter(required_pars, "co2", co2, pars_list);
    test_parameter(required_pars, "h2s", h2s, pars_list);
    test_parameter(required_pars, "nacl", nacl, pars_list);

    return pars_list;
  }
  
  bool are_blackoil_parameters_set() const noexcept
  {
    return api != VtlQuantity::null_quantity and
      rsb != VtlQuantity::null_quantity and
      yg != VtlQuantity::null_quantity and
      tsep != VtlQuantity::null_quantity and
      psep != VtlQuantity::null_quantity;
  }

# define Declare_Correlation_Common(NAME)	\
  void test_##NAME##_corr() const					\
  {									\
    if (NAME##_corr == nullptr)						\
      ALEPHTHROW(CorrelationNotFound, "Correlation for " #NAME		\
	       " has not been set");					\
  }

  /* Declare a new correlation type. That is, a correlation pointer of
     name NAME_corr and a setter function of name set_NAME().
						
     NAME is the expected target name of correlation and CORR is the
     correlation by default.

     The setter validates that the CORR target name is equal to NAME
     and that the returning unit is sibling.     
  */
# define Declare_Correlation(NAME)					\
  const Correlation * NAME##_corr = nullptr;				\
									\
  Declare_Correlation_Common(NAME);					\
									\
  void set_##NAME(const Correlation & corr)				\
  {									\
    if (corr.target_name() != #NAME)					\
      ALEPHTHROW(InvalidTargetType, corr.name +				\
	       " is not for " #NAME);					\
    NAME##_corr = &corr;						\
  }									\
									\
  void set_##NAME##_corr(const Correlation * corr_ptr)			\
  {									\
    set_##NAME(*corr_ptr);						\
  }   

# define Declare_Optional_Correlation(NAME, CORR)			\
  const Correlation * NAME##_corr = &CORR::get_instance();		\
									\
  Declare_Correlation_Common(NAME);					\
									\
  void set_##NAME(const Correlation & corr)				\
  {									\
    assert(NAME##_corr);						\
    if (NAME##_corr->target_name() != #NAME)				\
      ALEPHTHROW(InvalidTargetType, NAME##_corr->name +			\
	       " is not for " #NAME " (probably set with macro)");	\
    if (corr.target_name() != #NAME)					\
      ALEPHTHROW(InvalidTargetType, corr.name + " is not for " #NAME);	\
    NAME##_corr = &corr;						\
  }									\
									\
  void set_##NAME(const Correlation * corr_ptr)				\
  {									\
    set_##NAME(*corr_ptr);						\
  }
  
# define Declare_Tuned_Correlation(NAME)				\
  Declare_Correlation(NAME);						\
  mutable double c_##NAME = 0;						\
  mutable double m_##NAME = 1;						\
									\
  void set_##NAME(const Correlation & __##NAME##_corr,			\
		  double c, double m) noexcept				\
  {									\
    set_##NAME(__##NAME##_corr);					\
    c_##NAME = c;							\
    m_##NAME = m;							\
  }									\
									\
  void set_##NAME(const Correlation * __##NAME##_corr,			\
		  double c, double m) noexcept				\
  {									\
    set_##NAME(*__##NAME##_corr, c, m);					\
  }

  Declare_Tuned_Correlation(pb);
  Declare_Tuned_Correlation(rs);
  Declare_Tuned_Correlation(bob);
  Declare_Tuned_Correlation(boa);
  Declare_Tuned_Correlation(cob);
  Declare_Tuned_Correlation(coa);
  Declare_Tuned_Correlation(uod);
  Declare_Tuned_Correlation(uob);
  Declare_Tuned_Correlation(uoa);
  Declare_Tuned_Correlation(zfactor);

  /* Verify that corr_ptr has the correct target name */
  static void
  verify_correlation(const Correlation * corr_ptr, const string & target_name)
  {
    if (corr_ptr == nullptr)
      ALEPHTHROW(CorrelationNotFound, "Correlation for " + target_name +
	       " has not been defined");
    if (corr_ptr->target_name() != target_name)
      ALEPHTHROW(InvalidTargetType,
	       corr_ptr->name + "  is not for" + target_name);
  }

  Declare_Optional_Correlation(ppchc, PpchcStanding);
  Declare_Optional_Correlation(ppcm_mixing, PpcmKayMixingRule);
  Declare_Optional_Correlation(adjustedppcm, AdjustedppcmWichertAziz);
  Declare_Optional_Correlation(tpchc, TpchcStanding);
  Declare_Optional_Correlation(tpcm_mixing, TpcmKayMixingRule);
  Declare_Optional_Correlation(adjustedtpcm, AdjustedtpcmWichertAziz);
  Declare_Optional_Correlation(cg, CgMattarBA);
  Declare_Optional_Correlation(ug, UgCarrKB);
  Declare_Optional_Correlation(bwb, BwbSpiveyMN);
  Declare_Optional_Correlation(bwa, BwaSpiveyMN);
  Declare_Optional_Correlation(uw, UwMcCain);
  Declare_Optional_Correlation(pw, PwSpiveyMN);
  Declare_Optional_Correlation(cwb, CwbSpiveyMN);
  Declare_Optional_Correlation(cwa, CwaSpiveyMN);
  Declare_Optional_Correlation(rsw, RswSpiveyMN);
  Declare_Optional_Correlation(sgo, SgoBakerSwerdloff);
  Declare_Optional_Correlation(sgw, SgwJenningsNewman);
  
protected:
  
  VtlQuantity t_q, p_q;
  double temperature, pressure;

  ParPair pb_par, t_par, p_par, p_pb, rs_pb, tpr_par, ppr_par, rs_par,
    coa_par, z_par, rsw_par, bw_par;

  VtlQuantity bobp, uobp, pobp, uod, bwbp, tpr, rs, coa, yghc, ppchc, ppcm,
    tpchc, tpcm, adjustedppcm, adjustedtpcm, ppr, pb_q, bo, uo, po, z, cg,
    bg, ug, pg, rsw, cwa, bw, pw, cw, ppw, uw, sgo, sgw;
  VtlQuantity min_uo;

  ParList pb_pars, rs_pars, uod_pars, bo_pars, co_pars, uo_pars, po_pars,
    ug_pars, bw_pars, uw_pars, pw_pars, rsw_pars, cw_pars, cwa_pars, cg_pars,
    sgo_pars, sgw_pars;

  DefinedCorrelation rs_dcorr, co_dcorr, bo_dcorr, uo_dcorr, po_dcorr, bw_dcorr,
    cw_dcorr;

public:

  virtual void correlations_are_set() const = 0;
  virtual void temperature_calculations() = 0;
  virtual void pressure_calculations() = 0;
  virtual void pop_temperature_parameters() = 0;
  virtual void put_row(const bool pb_row) = 0;
  
  virtual Grid generate_grid(const DynList<double> & t_values,
			     const DynList<double> & p_values) = 0;

  virtual Grid
  generate_rows(const DynList<pair<double, double>> & tp_values) = 0;

  static DefinedCorrelation
  define_correlation(const VtlQuantity & pb,
		     const Correlation * below_corr_ptr, double cb, double mb,
		     const Unit & bunit,
		     const Correlation * above_corr_ptr,
		     double ca, double ma, const Unit & aunit)
  {
    DefinedCorrelation ret("p", pb.unit);
    ret.add_tuned_correlation(below_corr_ptr, pb_unit->min(),
			      pb, cb, mb, bunit);
    ret.add_tuned_correlation(above_corr_ptr, pb.next(),
			      pb_unit->max(), ca, ma, aunit);
    return ret;
  }

  static DefinedCorrelation
  define_correlation(const VtlQuantity & pb,
		     const Correlation * below_corr_ptr, 
		     const Correlation * above_corr_ptr)
  {
    return define_correlation(pb, below_corr_ptr, 0, 1, *pb_unit,
			      above_corr_ptr, 0, 1, *pb_unit);
  }  
};

struct BlackoilGrid : public Cplot
{
  using Cplot::Cplot;

  void correlations_are_set() const
  {
    verify_correlation(pb_corr, "pb");
    verify_correlation(rs_corr, "rs");
    verify_correlation(bob_corr, "bob");
    verify_correlation(cob_corr, "cob");
    verify_correlation(coa_corr, "coa");
    verify_correlation(boa_corr, "boa");
    verify_correlation(uod_corr, "uod");
    verify_correlation(uob_corr, "uob");
    verify_correlation(uoa_corr, "uoa");
    verify_correlation(zfactor_corr, "zfactor");
  }

  void init()
  {
    correlations_are_set();
    test_api();
    test_rsb();
    test_yg();
    test_tsep();
    test_psep();
    test_h2s();
    test_co2();
    test_n2();
    test_nacl();
 
    test_pb_corr();
    test_rs_corr();
    test_bob_corr();
    test_boa_corr();
    test_uod_corr();
    test_cob_corr();
    test_coa_corr();
    test_uob_corr();
    test_uoa_corr();
    test_ppchc_corr();
    test_tpchc_corr();
    test_ppcm_mixing_corr();
    test_tpcm_mixing_corr();
    test_adjustedppcm_corr();
    test_adjustedtpcm_corr();
    test_zfactor_corr();
    test_cg_corr();
    test_ug_corr();
    test_bwb_corr();
    test_bwa_corr();
    test_uw_corr();
    test_pw_corr();
    test_rsw_corr();
    test_cwb_corr();
    test_cwa_corr();
    test_sgo_corr();
    test_sgw_corr();

    /* Calculation of constants for Z */
    yghc = inv.compute_exc(YghcWichertAziz::correlation(), true, PPAR(yg),
			   PPAR(n2), PPAR(co2), PPAR(h2s)); 
    ppchc = inv.compute_exc(ppchc_corr, true, PPAR(yghc), PPAR(n2),
			    PPAR(co2), PPAR(h2s));
    ppcm = inv.compute_exc(ppcm_mixing_corr, true, PPAR(ppchc), PPAR(n2),
			   PPAR(co2), PPAR(h2s));
    tpchc = tpchc_corr->compute(check, yghc);
    tpcm = inv.compute_exc(tpcm_mixing_corr, true, PPAR(tpchc),
			   PPAR(n2), PPAR(co2), PPAR(h2s));
    adjustedppcm = inv.compute_exc(adjustedppcm_corr, true, PPAR(ppcm),
				   PPAR(tpcm), PPAR(co2), PPAR(h2s));
    adjustedtpcm = inv.compute_exc(adjustedtpcm_corr, true, PPAR(tpcm),
				   PPAR(co2), PPAR(h2s));
    /* End calculation constants for z */

    min_uo = min_uo_val();

    /* Initialization of correlation parameter lists */
    pb_pars = load_constant_parameters({pb_corr});
    rs_pars = load_constant_parameters({rs_corr, &RsAbovePb::get_instance()});
    uod_pars = load_constant_parameters({uod_corr});
    bo_pars = load_constant_parameters({bob_corr, boa_corr});
    co_pars = load_constant_parameters({cob_corr, coa_corr});
    uo_pars = load_constant_parameters({uob_corr, uoa_corr});
    po_pars = load_constant_parameters({&PobBradley::get_instance(),
	  &PoaBradley::get_instance()});
    ug_pars = load_constant_parameters({ug_corr});
    insert_in_container(ug_pars, ppar("tpc", adjustedtpcm),
			ppar("ppc", adjustedppcm));
    bw_pars = load_constant_parameters({bwb_corr, bwa_corr});
    uw_pars = load_constant_parameters({uw_corr});
    pw_pars = load_constant_parameters({pw_corr});
    rsw_pars = load_constant_parameters({rsw_corr});
    cw_pars = load_constant_parameters({cwb_corr, cwa_corr});
    cwa_pars = load_constant_parameters({cwa_corr});
    cg_pars.insert(ppar("ppc", ppcm));
    sgo_pars = load_constant_parameters({sgo_corr});

    grid = Grid("t", inv.t_unit,
		"pb", pb_corr->unit,
		"uod", uod_corr->unit,
		"p", inv.p_unit,
		"rs", rs_corr->unit,
		"co", cob_corr->unit,
		"bo", bob_corr->unit,
		"uo", uob_corr->unit,
		"po", PobBradley::get_instance().unit,
		"zfactor", Zfactor::get_instance(),
		"cg", cg_corr->unit,
		"bg", Bg::get_instance().unit,
		"ug", ug_corr->unit,
		"pg", Pg::get_instance().unit,
		"bw", bwb_corr->unit,
		"uw", uw_corr->unit,
		"pw", pw_corr->unit,
		"rsw", rsw_corr->unit,
		"cw", cwb_corr->unit,
		"sgo", sgo_corr->unit,
		"sgw", sgw_corr->unit);
  }

  void temperature_calculations()
  {
    t_q = VtlQuantity(inv.t_unit, temperature);
    t_par = ppar("t", t_q);
    tpr = Tpr::get_instance().call(t_q, adjustedtpcm);
    tpr_par = PPAR(tpr);
    pb_q = inv.tcompute(pb_corr, c_pb, m_pb, *pb_unit, check, pb_pars, t_par);
    if (pb_q.is_null())
      return;
    
    pb_par = ppar("pb", pb_q);
    p_pb = ppar("p", pb_q);

    const Quantity<CP> min_uod = min_uod_val(uod_corr);
    uod = inv.bcompute(uod_corr, c_uod, m_uod, *uo_unit, min_uod,
		       CP::get_instance().max(), check, uod_pars, t_par, pb_par);
	
    insert_in_container(rs_pars, t_par, pb_par);
    rs_dcorr = define_correlation(pb_q, rs_corr, c_rs, m_rs, *rs_unit,
				  &RsAbovePb::get_instance(), 0, 1,
				  *rs_unit);

    insert_in_container(co_pars, t_par, pb_par);
    co_dcorr = define_correlation(pb_q, cob_corr, c_cob, m_cob, *co_unit,
				  coa_corr, c_coa, m_coa, *co_unit);
    bo_dcorr = define_correlation(pb_q, bob_corr, c_bob, m_bob, *bo_unit,
				  boa_corr, c_boa, m_boa, *bo_unit);

    insert_in_container(uo_pars, t_par, pb_par, PPAR(uod)); 
    uo_dcorr = define_correlation(pb_q, uob_corr, c_uob, m_uob, *uo_unit,
				  uoa_corr, c_uoa, m_uoa, *uo_unit);
    uo_dcorr.set_min(min_uo);
	
    po_dcorr = define_correlation(pb_q, &PobBradley::get_instance(),
				  &PoaBradley::get_instance());
	
    bw_dcorr = define_correlation(pb_q, bwb_corr, bwa_corr);

    cw_dcorr = define_correlation(pb_q, cwb_corr, cwa_corr);

    insert_in_container(bo_pars, t_par, pb_par);
    bobp = inv.tcompute(bob_corr, c_bob, m_bob, *bo_unit, check,
			bo_pars, p_pb, rs_pb);
	
    uobp = inv.tcompute(uob_corr, c_uob, m_uob, *uo_unit, check,
			uo_pars, p_pb, rs_pb, ppar("bob", bobp));
	
    bo_pars.insert(PPAR(bobp));

    uo_pars.insert("uobp", uobp.raw(), &uobp.unit);

    pobp = inv.compute(&PobBradley::get_instance(), check, po_pars,
		       rs_pb, ppar("bob", bobp));

    bwbp = inv.compute(bwb_corr, check, bw_pars, t_par, ppar("p", pb_q));

    insert_in_container(po_pars, pb_par, PPAR(pobp));
    insert_in_container(ug_pars, t_par, tpr_par);
    insert_in_container(bw_pars, t_par, pb_par, PPAR(bwbp));
    cg_pars.insert(tpr_par);
    uw_pars.insert(t_par);
    pw_pars.insert(t_par);
    rsw_pars.insert(t_par);
    cw_pars.insert(t_par);
    cwa_pars.insert(t_par);
    sgo_pars.insert(t_par);
    sgw_pars.insert(t_par);
  }

  void pop_temperature_parameters()
  {
    remove_from_container(co_pars, "pb", t_par);
    remove_from_container(bo_pars, "bobp", "pb", t_par);
    remove_from_container(uo_pars, "uobp", "pb", "uod", t_par);
    remove_from_container(po_pars, "pb", "pobp");
    remove_from_container(ug_pars, t_par, tpr_par);
    remove_from_container(bw_pars, t_par, pb_par, "bwbp");
    sgo_pars.remove(t_par);
    sgw_pars.remove(t_par);
    cg_pars.remove(tpr_par);
    uw_pars.remove(t_par);
    pw_pars.remove(t_par);
    cw_pars.remove(t_par);
    rsw_pars.remove(t_par);
  }

  void pressure_calculations()
  {
    ppr = Ppr::get_instance().call(p_q, adjustedppcm);
    ppr_par = PPAR(ppr);
    rs = inv.dcompute(rs_dcorr, check, p_q, rs_pars, p_par);
    rs = min(rs, rsb);
    rs_par = PPAR(rs);
    coa = inv.dcompute(co_dcorr, check, p_q, co_pars, p_par);
    coa_par = PPAR(coa);
    bo = inv.dcompute(bo_dcorr, check, p_q, bo_pars, p_par, rs_par, coa_par);
    uo = inv.dcompute(uo_dcorr, check, p_q, uo_pars, p_par, rs_par, 
		       ppar("bob", bo));
    po = inv.dcompute(po_dcorr, check, p_q, po_pars, p_par, rs_par, coa_par,
		      ppar("bob", bo));

    if (p_q <= pb_q)
      z = inv.tcompute(zfactor_corr, c_zfactor, m_zfactor, *z_unit, check,
		       ppr_par, tpr_par);
    z_par = PPAR(z);
    cg = inv.compute(cg_corr, check, cg_pars, ppr_par, z_par);
    bg = Bg::get_instance().call(t_q, p_q, z);
    ug = inv.compute(ug_corr, check, ug_pars, p_par, ppr_par, z_par);
    pg = Pg::get_instance().call(yg, t_q, p_q, z);
    rsw = inv.compute(rsw_corr, check, rsw_pars, p_par);
    rsw_par = PPAR(rsw);
    cwa = inv.compute(cwa_corr, check, cwa_pars, p_par, rsw_par);
    bw = inv.dcompute(bw_dcorr, check, p_q, bw_pars, p_par, PPAR(cwa));
    bw_par = PPAR(bw);
    pw = inv.compute(pw_corr, check, pw_pars, p_par, bw_par);
    cw = inv.dcompute(cw_dcorr, check, p_q, cw_pars, p_par, z_par, 
		      PPAR(bg), rsw_par, bw_par, PPAR(cwa));
    ppw = PpwSpiveyMN::get_instance().call(t_q, p_q);
    uw = inv.compute(uw_corr, check, uw_pars, p_par, PPAR(ppw));
    sgo = inv.compute(sgo_corr, check, sgo_pars, p_par);
    sgw = inv.compute(sgw_corr, check, sgw_pars, p_par);
  }

  void put_row(const bool pb_row)
  {
    grid.put_row(pb_row, inv.exception_thrown, t_q, pb_q, uod, p_q, rs,
		 coa, bo, uo, po, z, cg, bg, ug, pg, bw, uw, pw, rsw,
		 cw, sgo, sgw);
  }

  Grid generate_grid(const DynList<double> & t_values,
		     const DynList<double> & p_values)
  {
    init();    
    for (auto t_it = t_values.get_it(); t_it.has_curr(); t_it.next())
      {
	temperature = t_it.get_curr();

	temperature_calculations();

	const auto pb = pb_q.raw();
	const double next_pb = nextafter(pb, numeric_limits<double>::max());
	const VtlQuantity next_pb_q = { pb_q.unit, next_pb };

	const VtlQuantity first_p_point(inv.p_unit, p_values.get_first());
	const bool first_p_above_pb = VtlQuantity(pb_q.unit,first_p_point) > pb_q;

	size_t i = 0;
	for (auto p_it = p_values.get_it(); p_it.has_curr(); ) // pressure loop
	  {
	    pressure = p_it.get_curr();
	    p_q = VtlQuantity(inv.p_unit, pressure);
	    p_par = ppar("p", p_q);

	    bool pb_row = false; /* true if this line concerns to bubble point */

	    /* WARNING: these predicates must be evaluated exactly in
	       this order */
	    if (p_q <= pb_q or (not (i < 2)) or first_p_above_pb)
	      p_it.next();
	    else
	      {
		pb_row = true;
		p_par = ppar("p", ++i == 1 ? pb_q : next_pb_q);
		p_q = VtlQuantity(*p_par.second.second, p_par.second.first);
		assert(i <= 2);
	      }		
	    pressure_calculations();
	    put_row(pb_row);
	  }
	pop_temperature_parameters();
      }
    return move(grid);
  }

  Grid generate_rows(const DynList<pair<double, double>> & tp_values)
  {
    init();    
    for (auto it = tp_values.get_it(); it.has_curr(); it.next())
      {
	const auto & p = it.get_curr();
	temperature = p.first;
	pressure = p.second;

	temperature_calculations();
	pressure_calculations();
	put_row(false);
	pop_temperature_parameters();
      }
    return move(grid);
  }
};

struct SimpleGrid : public Cplot
{
  using Cplot::Cplot;

  void correlations_are_set() const
  {
    verify_correlation(pb_corr, "pb");
    verify_correlation(rs_corr, "rs");
    verify_correlation(bob_corr, "bob");
    verify_correlation(cob_corr, "cob");
    verify_correlation(coa_corr, "coa");
    verify_correlation(boa_corr, "boa");
    verify_correlation(uod_corr, "uod");
    verify_correlation(uob_corr, "uob");
    verify_correlation(uoa_corr, "uoa");
    verify_correlation(zfactor_corr, "zfactor");
  }

  void init()
  {
    correlations_are_set();
    test_api();
    test_rsb();
    test_yg();
    test_tsep();
    test_psep();
    test_h2s();
    test_co2();
    test_n2();
    test_nacl();
 
    test_pb_corr();
    test_rs_corr();
    test_bob_corr();
    test_boa_corr();
    test_uod_corr();
    test_cob_corr();
    test_coa_corr();
    test_uob_corr();
    test_uoa_corr();
    test_ppchc_corr();
    test_tpchc_corr();
    test_ppcm_mixing_corr();
    test_tpcm_mixing_corr();
    test_adjustedppcm_corr();
    test_adjustedtpcm_corr();
    test_zfactor_corr();

    /* Calculation of constants for Z */
    yghc = inv.compute_exc(YghcWichertAziz::correlation(), true, PPAR(yg),
			   PPAR(n2), PPAR(co2), PPAR(h2s)); 
    ppchc = inv.compute_exc(ppchc_corr, true, PPAR(yghc), PPAR(n2),
			    PPAR(co2), PPAR(h2s));
    ppcm = inv.compute_exc(ppcm_mixing_corr, true, PPAR(ppchc), PPAR(n2),
			   PPAR(co2), PPAR(h2s));
    tpchc = tpchc_corr->compute(check, yghc);
    tpcm = inv.compute_exc(tpcm_mixing_corr, true, PPAR(tpchc),
			   PPAR(n2), PPAR(co2), PPAR(h2s));
    adjustedppcm = inv.compute_exc(adjustedppcm_corr, true, PPAR(ppcm),
				   PPAR(tpcm), PPAR(co2), PPAR(h2s));
    adjustedtpcm = inv.compute_exc(adjustedtpcm_corr, true, PPAR(tpcm),
				   PPAR(co2), PPAR(h2s));
    /* End calculation constants for z */

    min_uo = min_uo_val();

    /* Initialization of correlation parameter lists */
    pb_pars = load_constant_parameters({pb_corr});
    rs_pars = load_constant_parameters({rs_corr, &RsAbovePb::get_instance()});
    uod_pars = load_constant_parameters({uod_corr});
    bo_pars = load_constant_parameters({bob_corr, boa_corr});
    co_pars = load_constant_parameters({cob_corr, coa_corr});
    uo_pars = load_constant_parameters({uob_corr, uoa_corr});

    grid = Grid("t", inv.t_unit,
		"pb", pb_corr->unit,
		"uod", uod_corr->unit,
		"p", inv.p_unit,
		"rs", rs_corr->unit,
		"co", cob_corr->unit,
		"bo", bob_corr->unit,
		"uo", uob_corr->unit,
		"zfactor", Zfactor::get_instance());
  }

  void temperature_calculations()
  {
    t_q = VtlQuantity(inv.t_unit, temperature);
    t_par = ppar("t", t_q);
    tpr = Tpr::get_instance().call(t_q, adjustedtpcm);
    tpr_par = PPAR(tpr);
    pb_q = inv.tcompute(pb_corr, c_pb, m_pb, *pb_unit, check, pb_pars, t_par);
    if (pb_q.is_null())
      return;
    
    pb_par = ppar("pb", pb_q);
    p_pb = ppar("p", pb_q);

    const Quantity<CP> min_uod = min_uod_val(uod_corr);
    uod = inv.bcompute(uod_corr, c_uod, m_uod, *uo_unit, min_uod,
		       CP::get_instance().max(), check, uod_pars, t_par, pb_par);
	
    insert_in_container(rs_pars, t_par, pb_par);
    rs_dcorr = define_correlation(pb_q, rs_corr, c_rs, m_rs, *rs_unit,
				  &RsAbovePb::get_instance(), 0, 1,
				  *rs_unit);

    insert_in_container(co_pars, t_par, pb_par);
    co_dcorr = define_correlation(pb_q, cob_corr, c_cob, m_cob, *co_unit,
				  coa_corr, c_coa, m_coa, *co_unit);
    bo_dcorr = define_correlation(pb_q, bob_corr, c_bob, m_bob, *bo_unit,
				  boa_corr, c_boa, m_boa, *bo_unit);

    insert_in_container(uo_pars, t_par, pb_par, PPAR(uod)); 
    uo_dcorr = define_correlation(pb_q, uob_corr, c_uob, m_uob, *uo_unit,
				  uoa_corr, c_uoa, m_uoa, *uo_unit);
    uo_dcorr.set_min(min_uo);

    insert_in_container(bo_pars, t_par, pb_par);
    bobp = inv.tcompute(bob_corr, c_bob, m_bob, *bo_unit, check,
			bo_pars, p_pb, rs_pb);
	
    uobp = inv.tcompute(uob_corr, c_uob, m_uob, *uo_unit, check,
			uo_pars, p_pb, rs_pb, ppar("bob", bobp));
	
    bo_pars.insert(PPAR(bobp));

    uo_pars.insert("uobp", uobp.raw(), &uobp.unit);
  }

  void pop_temperature_parameters()
  {
    remove_from_container(rs_pars, "pb", t_par);
    remove_from_container(co_pars, "pb", t_par);
    remove_from_container(bo_pars, "bobp", "pb", t_par);
    remove_from_container(uo_pars, "uobp", "pb", "uod", t_par);
  }

  void pressure_calculations()
  {
    ppr = Ppr::get_instance().call(p_q, adjustedppcm);
    ppr_par = PPAR(ppr);
    rs = inv.dcompute(rs_dcorr, check, p_q, rs_pars, p_par);
    rs = min(rs, rsb);
    rs_par = PPAR(rs);
    coa = inv.dcompute(co_dcorr, check, p_q, co_pars, p_par);
    coa_par = PPAR(coa);
    bo = inv.dcompute(bo_dcorr, check, p_q, bo_pars, p_par, rs_par, coa_par);
    uo = inv.dcompute(uo_dcorr, check, p_q, uo_pars, p_par, rs_par,
		      ppar("bob", bo));
    if (p_q <= pb_q)
      z = inv.tcompute(zfactor_corr, c_zfactor, m_zfactor, *z_unit, check,
		       ppr_par, tpr_par);
    z_par = PPAR(z);
  }

  void put_row(const bool pb_row)
  {
    grid.put_row(pb_row, inv.exception_thrown, t_q, pb_q, uod, p_q, rs, coa,
		 bo, uo);
  }

  Grid generate_grid(const DynList<double> & t_values,
		     const DynList<double> & p_values)
  {
    init();    
    for (auto t_it = t_values.get_it(); t_it.has_curr(); t_it.next())
      {
	temperature = t_it.get_curr();

	temperature_calculations();

	const auto pb = pb_q.raw();
	const double next_pb = nextafter(pb, numeric_limits<double>::max());
	const VtlQuantity next_pb_q = { pb_q.unit, next_pb };

	const VtlQuantity first_p_point(inv.p_unit, p_values.get_first());
	const bool first_p_above_pb = VtlQuantity(pb_q.unit,first_p_point) > pb_q;

	size_t i = 0;
	for (auto p_it = p_values.get_it(); p_it.has_curr(); ) // pressure loop
	  {
	    pressure = p_it.get_curr();
	    p_q = VtlQuantity(inv.p_unit, pressure);
	    p_par = ppar("p", p_q);

	    bool pb_row = false; /* true if this line concerns to bubble point */

	    /* WARNING: these predicates must be evaluated exactly in
	       this order */
	    if (p_q <= pb_q or (not (i < 2)) or first_p_above_pb)
	      p_it.next();
	    else
	      {
		pb_row = true;
		p_par = ppar("p", ++i == 1 ? pb_q : next_pb_q);
		p_q = VtlQuantity(*p_par.second.second, p_par.second.first);
		assert(i <= 2);
	      }		

	    pressure_calculations();
	    put_row(pb_row);
	  }
	pop_temperature_parameters();
      }
    return move(grid);
  }

  Grid generate_rows(const DynList<pair<double, double>> & tp_values)
  {
    init();    
    for (auto it = tp_values.get_it(); it.has_curr(); it.next())
      {
	const auto & p = it.get_curr();
	temperature = p.first;
	pressure = p.second;

	temperature_calculations();
	pressure_calculations();
	put_row(false);
	pop_temperature_parameters();
      }
    return move(grid);
  }
};

# endif
