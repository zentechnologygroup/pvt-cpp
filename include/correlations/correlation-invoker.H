#ifndef CORRELATION_INVOKER_H
#define CORRELATION_INVOKER_H

# include <correlations/correlation.H>

struct CorrelationInvoker
{
  bool report_exceptions = false;
  bool exception_thrown = false;
  DynList<string> exception_list;
  double pressure = 0, temperature = 0;
  const Unit * t_unit = nullptr;
  const Unit * p_unit = nullptr;

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

  template <typename ... Args> inline
  VtlQuantity tcompute(const Correlation * corr_ptr,
		       double c, double m, const Unit & tuned_unit, bool check,
		       ParList & pars_list, const Args & ... args)
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
    //catch (UnitConversionNotFound) {}
    catch (exception & e)
      {
	if (report_exceptions)
	  store_exception(corr_ptr->name, e);
   
	remove_from_container(pars_list, args ...);
      }
    return VtlQuantity::null_quantity;
  }

};



#endif
