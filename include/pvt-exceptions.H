# ifndef PVT_EXCEPTIONS_H
# define PVT_EXCEPTIONS_H

# include <zen-exceptions.H>

DEFINE_ZEN_EXCEPTION(WrongSiblingUnit,
		     "Sibling unit is not physically related");

DEFINE_ZEN_EXCEPTION(WrongUnitRatio, "wrong unit ratio");

DEFINE_ZEN_EXCEPTION(MinMaxReversed,
		     "minimum value is greater than maximum value");

DEFINE_ZEN_EXCEPTION(UnitConversionNotFound, category_msg);

DEFINE_ZEN_EXCEPTION(SynonymNotFound, "Parameter synonum not found");

DEFINE_ZEN_EXCEPTION(DuplicatedCorrelationName, "duplicated correlation name");

DEFINE_ZEN_EXCEPTION(InvalidNumberOfParameters,
		     "invalid number of parameters");

DEFINE_ZEN_EXCEPTION(ParameterNameNotFound, "parameter name not found");

DEFINE_ZEN_EXCEPTION(ParameterNameNotSet, "parameter name not set");

DEFINE_ZEN_EXCEPTION(OutOfUnitRange, "Value is out of unit range");

DEFINE_ZEN_EXCEPTION(OutOfCorrelationRange, "Value is out of correlation range");

DEFINE_ZEN_EXCEPTION(OutOfParameterRange, "Value is out of parameter range");

DEFINE_ZEN_EXCEPTION(WrongValueUndersaturatedCondition,
		     "value is greater than pb")

DEFINE_ZEN_EXCEPTION(WrongValueSaturatedCondition, "value is less than pb")

DEFINE_ZEN_EXCEPTION(WrongValueVaporPressure, "Lo pone Ixhel")

DEFINE_ZEN_EXCEPTION(WrongNonhydrocarbonsConcentrations, "IXHEL")

#endif



