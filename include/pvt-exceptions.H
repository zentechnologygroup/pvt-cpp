# ifndef PVT_EXCEPTIONS_H
# define PVT_EXCEPTIONS_H

# include <zen-exceptions.H>

DEFINE_ZEN_EXCEPTION(OutOfUnitRange, "Value is out of unit range");

DEFINE_ZEN_EXCEPTION(OutOfCorrelationRange, "Value is out of correlation range");

DEFINE_ZEN_EXCEPTION(OutOfParameterRange, "Value is out of parameter range");

DEFINE_ZEN_EXCEPTION(WrongValueUndersaturatedCondition,
		     "value is greater than pb")

DEFINE_ZEN_EXCEPTION(WrongValueSaturatedCondition, "value is less than pb")

DEFINE_ZEN_EXCEPTION(WrongValueVaporPressure, "Lo pone Ixhel")

DEFINE_ZEN_EXCEPTION(WrongNonhydrocarbonsConcentrations, "IXHEL")

#endif



