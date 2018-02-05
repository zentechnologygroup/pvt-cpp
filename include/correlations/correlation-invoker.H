#ifndef CORRELATION_INVOKER_H
#define CORRELATION_INVOKER_H

# include <correlations/correlation.H>
# include <correlations/defined-correlation.H>

/** Correlation invoker class.

    This class provides a family of generic function for calling
    correlation computations.

   @author Leandro Rabindranath Leon
   @ingroup Correlations
 */
struct CorrelationInvoker
{
  bool report_exceptions = false;
  bool exception_thrown = false;
  ParList * pars_ptr = nullptr;
  DynList<string> exception_list;
  double pressure = 0, temperature = 0;
  const Unit * t_unit = nullptr;
  const Unit * p_unit = nullptr;

  /**

   */
  CorrelationInvoker(ParList & pars_list,
		     const Unit * __t_unit, const Unit * __p_unit)
    : pars_ptr(&pars_list), t_unit(__t_unit), p_unit(__p_unit) {}

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
  template <typename ... Args>
  VtlQuantity tcompute(const Correlation * corr_ptr, double c, double m,
		       const Unit & tuned_unit, bool check,
		       const Args & ... args)
  {
    try
      {
	if (not insert_in_pars_list(*pars_ptr, args...))
	  return VtlQuantity::null_quantity;

	auto ret =
	  corr_ptr->tuned_compute_by_names(*pars_ptr, c, m, tuned_unit, check);
	remove_from_container(pars_ptr, args...);
	return ret;
      }
    catch (exception & e)
      {
	if (report_exceptions)
	  store_exception(corr_ptr->name, e);
   
	remove_from_container(*pars_ptr, args ...);
      }
    return VtlQuantity::null_quantity;
  }
  
  template <typename ... Args>
  VtlQuantity bcompute(const Correlation * corr_ptr,
		       double c, double m, const Unit & tuned_unit,
		       const VtlQuantity & min_val,
		       const VtlQuantity & max_val,
		       bool check, const Args & ... args)
  {
    try
      {
	if (not insert_in_pars_list(*pars_ptr, args...))
	  return VtlQuantity::null_quantity;

	auto ret = corr_ptr->bounded_tuned_compute_by_names(*pars_ptr,
							    min_val, max_val,
							    c, m, tuned_unit,
							    check);
	remove_from_container(*pars_ptr, args...);
	return ret;
      }
    catch (exception & e)
      {
	if (report_exceptions)
	  store_exception(corr_ptr->name, e);
   
	remove_from_container(*pars_ptr, args ...);
      }
    return VtlQuantity::null_quantity;
  }

  template <typename ... Args>
  VtlQuantity compute(const Correlation * corr_ptr, bool check,
		      const Args & ... args)
  {
    try
      {
	if (not insert_in_pars_list(*pars_ptr, args...))
	  return VtlQuantity::null_quantity;

	auto ret = corr_ptr->compute_by_names(*pars_ptr, check);
	remove_from_container(*pars_ptr, args...);
	return ret;
      }
    catch (exception & e)
      {
	if (report_exceptions)
	  store_exception(corr_ptr->name, e);
   
	remove_from_container(*pars_ptr, args ...);
      }
    return VtlQuantity::null_quantity;
  }

  template <typename ... Args>
  VtlQuantity compute_exc(const Correlation * corr_ptr, bool check,
			  const Args & ... args)
  {
    try
      {
	if (not insert_in_pars_list(*pars_ptr, args...))
	  return VtlQuantity::null_quantity;

	auto ret = corr_ptr->compute_by_names(*pars_ptr, check);
	remove_from_container(*pars_ptr, args...);
	return ret;
      }
    catch (exception & e)
      {
	remove_from_container(*pars_ptr, args ...);
	cout << "ERROR initializing " << correlation_call(corr_ptr, args...)
	     << "@ " << e.what();
	abort();
      }
    return VtlQuantity::null_quantity;
  }

  static bool valid_args() { return true; }

  template <typename ... Args> static 
  bool valid_args(const VtlQuantity & par, const Args & ... args)
  {
    if (par.is_null())
      return false;
    return valid_args(args...);
  }

private:
  
  bool
  insert_in_pars_list(const DefinedCorrelation&, const VtlQuantity&)
  {
    return true;
  }

  template <typename ... Args>
  bool insert_in_pars_list(const DefinedCorrelation & corr,
			   const VtlQuantity & p_q,
			   const Correlation::NamedPar & par,
			   const Args & ... args)
  {
    if (get<2>(par) == Unit::Invalid_Value)
      {
	const string & par_name = get<1>(par);   
	if (corr.search_parameters(p_q).contains(par_name))
	  return false; // here the correlation would receive
			// Invalid_Value and would fail 
      }
 
    pars_ptr->insert(par);
    if (insert_in_pars_list(corr, p_q, args...))
      return true;

    pars_ptr->remove(par); // If recursive insertion fails ==> remove par
    
    return false;
  }

public:

  template <typename ... Args>
  VtlQuantity dcompute(const DefinedCorrelation & corr, bool check,
		       const VtlQuantity & p_q,
		       const Args & ... args)
  {
    if (not insert_in_pars_list(corr, p_q, args...))
      return VtlQuantity::null_quantity;
 
    try
      {
	auto ret = corr.compute_by_names(*pars_ptr, check);
	remove_from_container(*pars_ptr, args ...);
	return ret;
      }
    catch (UnitConversionNotFound) {}
    catch (exception & e)
      {
	if (report_exceptions)
	  {
	    auto triggering_corr_ptr = corr.search_correlation(p_q);
	    string names = corr.correlations().
	      foldl<string>("", [triggering_corr_ptr] (auto & acu, auto ptr)
			    {
			      if (triggering_corr_ptr == ptr)
				return acu + "*" + ptr->name + " ";
			      return acu + ptr->name + " ";
			    });
	    store_exception("{ " + names + "}", e);
	  }

	remove_from_container(*pars_ptr, args ...);
      }
    return VtlQuantity::null_quantity;
  }
};

# define CALL(corr_name, var, args...)				\
  VtlQuantity var;						\
  try								\
    {								\
      if (CorrelationInvoker::valid_args(args))			\
	var = corr_name::get_instance().call(args);		\
    }								\
  /* catch (UnitConversionNotFound) {} */			\
  catch (exception & e)						\
    {								\
      store_exception(corr_name::get_instance().name, e);	\
    }

#endif
