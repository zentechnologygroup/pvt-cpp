# ifndef PVT_EXCEPTIONS_H
# define PVT_EXCEPTIONS_H

# include <zen-exceptions.H>

DEFINE_ZEN_EXCEPTION(SynonymNotFound, "Parameter synonym not found");

DEFINE_ZEN_EXCEPTION(DuplicatedCorrelationName, "duplicated correlation name");

DEFINE_ZEN_EXCEPTION(InvalidNumberOfParameters,
		     "invalid number of parameters");

DEFINE_ZEN_EXCEPTION(CorrelationNotFound, "Correlation not found");

DEFINE_ZEN_EXCEPTION(ParameterNameNotFound, "parameter name not found");

DEFINE_ZEN_EXCEPTION(ParameterNameNotSet, "parameter name not set");

DEFINE_ZEN_EXCEPTION(OutOfCorrelationRange, "Value is out of correlation range");

DEFINE_ZEN_EXCEPTION(OutOfParameterRange, "Value is out of parameter range");

DEFINE_ZEN_EXCEPTION(WrongValueUndersaturatedCondition,
		     "For an undersaturated condition P must be greater than Pb.")

DEFINE_ZEN_EXCEPTION(WrongValueSaturatedCondition,
		     "For a saturated condition P must be less than "
		     "or equal to Pb.")

DEFINE_ZEN_EXCEPTION(WrongValueVaporPressure,
		     "Computed vapor pressure is greater than P. "
		     "Reconsider the value of P or T.")

DEFINE_ZEN_EXCEPTION(WrongNonhydrocarbonsConcentrations,
		     "Invalid values for nonhydrocarbon concentrations.")

DEFINE_ZEN_EXCEPTION(WrongCombinationInputValues,
		     "Wrong combination of input values. "
		     "Refer to the method description.")

#endif

