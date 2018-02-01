#ifndef CORRELATION_INVOKER_H
#define CORRELATION_INVOKER_H

# include <correlations/correlation.H>
# include <correlations/defined-correlation.H>

/** Correlation invoker class

   @author Leandro Rabindranath Leon
   @ingroup Correlations
 */
struct CorrelationInvoker
{
  bool report_exceptions = false;
  bool exception_thrown = false;
  DynList<string> exception_list;
  double pressure = 0, temperature = 0;
  const Unit * t_unit = nullptr;
  const Unit * p_unit = nullptr;

  ParList pars_list;

  /*********** The following attributes define fluid properties */

  VtlQuantity api;
  VtlQuantity rsb;
  VtlQuantity yg;
  VtlQuantity tsep;
  VtlQuantity tsep2;
  VtlQuantity psep;
  VtlQuantity ogr;
  VtlQuantity n2;
  VtlQuantity co2;
  VtlQuantity h2;
  VtlQuantity nacl;

  void test_parameter(const DynList<pair<string, DynList<string>>> & required,
		      const string & name, const Unit * unit_ptr)
  {
    if ((required.exists([&par] (const auto & p) { return p.first == get<1>(par) or
      p.second.exists([&par] (const auto & s) { return s == get<1>(par); }); }))
      }

  /**************** end of fluid properties parameters */

  void load_constant_parameters(const DynList<const Correlation*> & l)
  {
    auto required_pars = DefinedCorrelation::parameter_list(l);
				  
    test_parameter(required_pars, api_par, pars_list);
    test_parameter(required_pars, rsb_par, pars_list);
    test_parameter(required_pars, yg_par, pars_list);
    test_parameter(required_pars, tsep_par, pars_list);
    test_parameter(required_pars, psep_par, pars_list);
    test_parameter(required_pars, n2_par, pars_list);
    test_parameter(required_pars, co2_par, pars_list);
    test_parameter(required_pars, h2s_par, pars_list);
    test_parameter(required_pars, nacl_par, pars_list);
  }

  /** The following attributes define correlation for different
      physical properties */

  

  /***************** end of correlation attributes */

  void reset_exceptions()
  {
    exception_list.empty();
    exception_thrown = false;
  }

  
private:

  void store_exception(const string & corr_name, const exception & e)
  {
    exception_thrown = true;
    ostringstream s;
    s << corr_name << ": " << temperature << " " << t_unit->name << ", "
      << pressure << " " << p_unit->name << ": " << e.what() << endl;
    exception_list.append(s.str());
  }

public:

  /** Tuned correlation calculation.

      Compute the correlation value using a linear tuning.

      If an exception is detected or thrown and `report_exceptions' is
      `true`, then the description is stored in `exception_list`.

      @param[in] corr_ptr pointer to correlation.
      @param[in] c tuning parameter.
      @param[in] m tuning parameter.
      @param[in]  tuned_unit a pointer to the unit that was used when
      the tuning was done.
      @param[in] check indicates whether the parameter bounds must be
      or not checked.
   */
  template <typename ... Args> inline
  VtlQuantity tcompute(const Correlation * corr_ptr, double c, double m,
		       const Unit & tuned_unit, bool check,
		       const Args & ... args)
  {
    try
      {
	if (not insert_in_pars_list(pars_list, args...))
	  return VtlQuantity::null_quantity;

	auto ret =
	  corr_ptr->tuned_compute_by_names(pars_list, c, m, tuned_unit, check);
	remove_from_container(pars_list, args...);
	return ret;
      }
    catch (exception & e)
      {
	if (report_exceptions)
	  store_exception(corr_ptr->name, e);
   
	remove_from_container(pars_list, args ...);
      }
    return VtlQuantity::null_quantity;
  }

  
  template <typename ... Args> inline
  VtlQuantity bcompute(const Correlation * corr_ptr,
		       double c, double m, const Unit & tuned_unit,
		       const VtlQuantity & min_val,
		       const VtlQuantity & max_val,
		       bool check, const Args & ... args)
  {
    try
      {
	if (not insert_in_pars_list(pars_list, args...))
	  return VtlQuantity::null_quantity;

	auto ret = corr_ptr->bounded_tuned_compute_by_names(pars_list,
							    min_val, max_val,
							    c, m, tuned_unit,
							    check);
	remove_from_container(pars_list, args...);
	return ret;
      }
    catch (exception & e)
      {
	if (report_exceptions)
	  store_exception(corr_ptr->name, e);
   
	remove_from_container(pars_list, args ...);
      }
    return VtlQuantity::null_quantity;
  }

  template <typename ... Args> inline
  VtlQuantity bcompute(const Correlation * corr_ptr,
		       double c, double m, const Unit & tuned_unit,
		       bool check, ParList & pars_list, const Args & ... args)
  {
    const VtlQuantity min_val = corr_ptr->unit.min();
    const VtlQuantity max_val = corr_ptr->unit.max();
    return bcompute(corr_ptr, c, m, tuned_unit, min_val, max_val,
		    check, pars_list, args...);
  }

};



#endif
