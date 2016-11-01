# ifndef SATURATED_OIL_COMPRESSIBILITY_IMPL_H
# define SATURATED_OIL_COMPRESSIBILITY_IMPL_H

# include <correlations/saturated-oil-compressibility.H>


inline Quantity<psia_1>
CobMcCainEtAl::impl(const Quantity<Api> & api,
		    const Quantity<SCF_STB> & rsb,
		    const Quantity<Rankine> & t,
		    const Quantity<psia> & p,
		    const Quantity<psia> & pb) const
{
  const double co = exp(-7.573 - 1.450*log(p) - 0.383*log(pb) + 1.402*log(t) +
			0.256*log(api) + 0.449*log(rsb));
  return Quantity<psia_1>(co);
}




 # endif // SATURATED_OIL_COMPRESSIBILITY_IMPL_H
