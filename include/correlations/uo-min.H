# ifndef UO_MIN_H
# define UO_MIN_H

# include <correlations/pvt-correlations.H>

# define Define_Get_Min_UO(name, targets...)	\
  inline Quantity<CP> name()			\
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

inline Quantity<CP> min_uod_val(const Correlation * corr_ptr)
{
  if (corr_ptr->min_from_author)
    return Quantity<CP>(VtlQuantity(corr_ptr->unit, corr_ptr->min_val));

  return min_uod_val();
}


# endif
