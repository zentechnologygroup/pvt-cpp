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

# define CONSTANT_PAR(__name, __unit)			\
  VtlQuantity __name;					\
  const Unit * __name##_unit = &__unit::get_instance();	\
							\
  void set_##name(double val, const Unit & unit)	\
  {							\
    if (not unit.is_sibling(*__name##_unit))		\
      ZENTHROW(UnitNotFound, "unit " + unit.name + " is not for " +	\
	       __name##_unit->physical_quantity.name);			\
    __name = VtlQuantity(unit, val);					\
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
# define TEST_PARAMETER(required, name, value, unit_ptr)	\
  if ((required.exists([&name](auto & p) { return p.first == name or	\
	    p.second.exists([&name](auto & s) { return s == name; }); }))) \
    pars_list.insert(name, value, unit_ptr)

  /**************** end of fluid properties parameters */

  void load_constant_parameters(const DynList<const Correlation*> & l)
  {
    auto required_pars = DefinedCorrelation::parameter_list(l);
				  
    TEST_PARAMETER(required_pars, #api, pars_list);
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
