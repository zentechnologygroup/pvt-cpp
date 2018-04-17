# ifndef PVT_EXCEPTIONS_H
# define PVT_EXCEPTIONS_H

# include <aleph-exceptions.H>

DEFINE_ALEPH_EXCEPTION(SynonymNotFound, "Parameter synonym not found");

DEFINE_ALEPH_EXCEPTION(DuplicatedCorrelationName, "duplicated correlation name");

DEFINE_ALEPH_EXCEPTION(InvalidNumberOfParameters,
		     "invalid number of parameters");

DEFINE_ALEPH_EXCEPTION(CorrelationNotFound, "Correlation not found");

DEFINE_ALEPH_EXCEPTION(InvalidTargetType, "Correlation target is invalid found");

DEFINE_ALEPH_EXCEPTION(ParameterNameNotFound, "parameter name not found");

DEFINE_ALEPH_EXCEPTION(ParameterNameNotSet, "parameter name not set");

DEFINE_ALEPH_EXCEPTION(OutOfCorrelationRange, "Value is out of correlation range");

DEFINE_ALEPH_EXCEPTION(OutOfParameterRange, "Value is out of parameter range");

DEFINE_ALEPH_EXCEPTION(WrongValueUndersaturatedCondition,
		     "For an undersaturated condition P must be greater than Pb.")

DEFINE_ALEPH_EXCEPTION(WrongValueSaturatedCondition,
		     "For a saturated condition P must be less than "
		     "or equal to Pb.")

DEFINE_ALEPH_EXCEPTION(WrongValueVaporPressure,
		     "Computed vapor pressure is greater than P. "
		     "Reconsider the value of P or T.")

DEFINE_ALEPH_EXCEPTION(WrongNonhydrocarbonsConcentrations,
		     "Invalid values for non-hydrocarbon concentrations.")

DEFINE_ALEPH_EXCEPTION(WrongCombinationInputValues,
		     "Wrong combination of input values. "
		     "Refer to the method description.")

#endif

