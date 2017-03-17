# ifndef PRODUCING_GAS_OIL_RATIO_IMPL_H
# define PRODUCING_GAS_OIL_RATIO_IMPL_H

# include <correlations/producing-gas-oil-ratio.H>

inline double
Rsp1::impl(const double & ogr) noexcept
{
  // Transformation from STB/MMscf to STB/SCF
  const double ogratio = unit_convert<STB_MMscf, STB_SCF>(ogr);
  return 1.0/ogratio;
}

# endif // PRODUCING_GAS_OIL_RATIO_IMPL_H
