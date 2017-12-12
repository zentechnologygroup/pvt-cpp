# ifndef CPLOT_H
# define CPLOT_H

# include <correlations/pvt-correlations.H>

struct Cplot
{
  mutable VtlQuantity api   = VtlQuantity::null_quantity;
  mutable VtlQuantity rsb   = VtlQuantity::null_quantity;
  mutable VtlQuantity yg    = VtlQuantity::null_quantity;
  mutable VtlQuantity tsep  = VtlQuantity::null_quantity;
  mutable VtlQuantity tsep2 = VtlQuantity::null_quantity;
  mutable VtlQuantity psep  = VtlQuantity::null_quantity;
  mutable VtlQuantity nacl  = VtlQuantity::null_quantity;
  mutable VtlQuantity hs2   = VtlQuantity::null_quantity;
  mutable VtlQuantity n2    = VtlQuantity::null_quantity;
  mutable VtlQuantity co2   = VtlQuantity::null_quantity;
  mutable VtlQuantity ogr   = VtlQuantity::null_quantity;

# define Declare_Correlation(name)		\
  const Correlation * name##_corr = nullptr;	\
  mutable double c_##name = 0;			\
  mutable double m_##name = 1;			\
						\
}; 

# endif
