#ifndef UNIT_EXCEPTIONS_H
#define UNIT_EXCEPTIONS_H

# include <zen-exceptions.H>

DEFINE_ZEN_EXCEPTION(WrongSiblingUnit,
		     "Sibling unit is not physically related");

DEFINE_ZEN_EXCEPTION(WrongUnitRatio, "wrong unit ratio");

DEFINE_ZEN_EXCEPTION(MinMaxReversed,
		     "minimum value is greater than maximum value");

DEFINE_ZEN_EXCEPTION(UnitConversionNotFound, category_msg);

#endif
